#include "RegAllocator.h"



void RegAllocator::computeLiveIntervals(IntervalList& intervals, std::vector<BasicBlock>& bbs)
{
	//loop through blocks in reverse order
	for (int i = bbs.size() - 1; i >= 0; --i)
	{
		//define IntervalList for basicblock b
		IntervalList bIntervals;
		//define other useful variables
		BasicBlock b = bbs[i];
		std::vector<SSA::Instruction*> instructions = b.getInstructions();
		int instructionsSize = instructions.size();
		int startIndex = b.getSSAstart();
		int endIndex = b.getSSAend();

		//calculate live set, which is union of b's successors' liveIn
		//and b's successors' phi inputs from b
		std::list<SSA::OperandUse> live;
		for (int j : b.getSucc()) {
			for (SSA::OperandUse opd : bbs[j].getLiveIn())
			{
				if(std::find(live.cbegin(), live.cend(), opd) == live.cend() && opd.isVar())
				{
					live.push_back(opd);
				}
			}
			//find phi functions and add their inputs from b to live set
			for (SSA::Instruction* instruction : bbs[j].getInstructions())
			{
				if (instruction->isPhi())
				{
					std::list<std::pair<int, SSA::Operand*>> phiSrcs = instruction->getPhiSrcs();
					//find phi src with b as basic block input
					for (std::pair<int, SSA::Operand*> phiSrc : phiSrcs)
					{
						//if phiSrc has basicblock as input and isn't already in live set, we add to live set
						SSA::OperandUse opd(phiSrc.second);
						if (phiSrc.first == b.getIndex() && std::find(live.cbegin(), live.cend(), opd) == live.cend() && opd.isVar())
						{
							live.push_back(opd);
						}
					}
				}
			}
		}
		//add intervals of entire basic block for each operand in live set
		for (SSA::OperandUse opd : live)
		{
			bIntervals.addRange(opd, startIndex, endIndex);
		}

		//loop over instructions in reverse order
		for(int j = instructionsSize-1; j >= 0; --j)
		{
			SSA::Instruction* instruction = instructions[j];

			//set from for opd to current instruction index and remove opd from live
			SSA::OperandUse opd(instruction->getDest());
			bIntervals.setFrom(opd, startIndex + j, endIndex);
			live.remove(opd);

			//for each src of instruction, add a new range from 0 to current index, and add to live set
			for (SSA::Operand* srcOpd : instruction->getSrcs())
			{
				//CLOSED interval for to(subtract 1)
				bIntervals.addRange(SSA::OperandUse(srcOpd), startIndex, startIndex + j - 1);
				//make sure its not already in live set
				if(std::find(live.cbegin(), live.cend(), *srcOpd) == live.cend() && srcOpd->isVar())
				{
					live.push_back(SSA::OperandUse(srcOpd));
				}
			}
			//THIS PART KINDA SKETCHY, NOT SURE IF IM SUPPOSED TO DO THIS
			//do the same for phi srcs, since there is different protocol go get its sources
			if(instruction->isPhi())
			{
				for (std::pair<int, SSA::Operand*> pair : instruction->getPhiSrcs())
				{
					SSA::Operand* srcOpd = pair.second;
					//CLOSED interval for to(subtract 1)
					bIntervals.addRange(SSA::OperandUse(srcOpd), startIndex, startIndex + j - 1);
					//make sure its not already in live set
					if(std::find(live.cbegin(), live.cend(), *srcOpd) == live.cend() && srcOpd->isVar())
					{
						live.push_back(SSA::OperandUse(srcOpd));
					}
				}
			}
		}

		//remove phi outputs from live set
		for (SSA::Instruction* instruction : instructions)
		{
			if (instruction->isPhi())
			{
				live.remove(SSA::OperandUse(instruction->getDest()));
			}
		}

		//check if b is a loop header
		int loopEnd = loopHeader(bbs, b.getIndex());
		//loop is header if loopEnd isn't -1
		if (loopEnd > -1)
		{
			//for each operand in live, add range from end of b to beginning of loopEnd
			for(SSA::OperandUse opd : live)
			{
				bIntervals.addRange(opd, endIndex, bbs[loopEnd].getStartLine());
			}
		}

		//set b's livein to live
		for(SSA::OperandUse opd : live)
		{
			b.addLiveIn(opd);
		}
		//add bIntervals to intervals
		intervals += bIntervals;
	}
	//output for testing
	//printf("\n%s---\n", intervals.getStr().c_str());
}

void RegAllocator::chaitin(RegMap& map, InterferenceGraph IGorig)
{
	//copy IGorig, lazy way
	InterferenceGraph IG(IGorig);
	//x86 has 6 nodes
	short k = 6;
	//stack to hold colorable variables
	std::stack<InterferenceGraph::Node> colorable;
	//spill set, variables that won't be colored
	std::list<InterferenceGraph::Node> spillSet;

	//loop until interference graph is empty
	while(!IG.empty())
	{
		//loop while there is a nodee with <k edges
		while (true)
		{
			//find node w greatest number of edges less than k
			InterferenceGraph::Node n;
			//we need to know if a node exists with num edges less than k to decide whether to exit loop
			bool exists = false;
			for (InterferenceGraph::Node node : IG.getNodes())
			{
				//set new max node if n has greater num edges than previous and is less than k
				int numEdges = node.numEdges();
				if (numEdges >= n.numEdges() && numEdges < k)
				{
					n = InterferenceGraph::Node(node);
					exists = true;
				}
			}
			//break if we couldn't find node with <k edges
			if(!exists)
			{
				break;
			}
			//remove node from graph and push to stack
			IG.removeNode(n);
			colorable.push(n);
		}
		//spill node if any nodes remain in graph
		//idea is that if we spill node, we might find node with <k neighbors
		if(!IG.empty())
		{
			//we will spill node with greatest num of edges
			InterferenceGraph::Node n;
			for (InterferenceGraph::Node node : IG.getNodes())
			{
				if(node.numEdges() > n.numEdges())
				{
					n = InterferenceGraph::Node(node);
				}
			}
			//add to spill set and remove from graph
			spillSet.push_back(n);
			IG.removeNode(n);
		}
	}
	//if spill set isn't empty, spill node to memory, remove from original graph, and break out of loop
	//insert spill code(not sure how to do this yet)
	if(!spillSet.empty())
	{

	}
	//pop nodes from stack and color
	while(!colorable.empty())
	{
		InterferenceGraph::Node n = colorable.top();
		colorable.pop();
		//get neighboring colors, set an index to true if the register with associated index is taken
		std::vector<bool> neighboringColors{ false, false, false, false, false, false };
		for(SSA::OperandUse op : n.edges)
		{
			int reg = map.getReg(op);
			if(reg != -1)
			{
				neighboringColors[reg] = true;
			}
		}
		//pick the register with smallest value
		for(int i = 0; i < k; ++i)
		{
			if (!neighboringColors[i])
			{
				map.insert(n.op, i);
				break;
			}
		}
	}
}



int RegAllocator::loopHeader(std::vector<BasicBlock>& bbs, int b)
{
	//need to call b's successors when we call recursive helper
	//if we pass b as succ, it will think that the loop has already finished
	for (int succ : bbs[b].getSucc())
	{
		int result = isLoopHeaderR(bbs, std::list<int>(), b, succ, b);
		if (result > -1)
		{
			return result;
		}
	}
	return -1;
}

int RegAllocator::isLoopHeaderR(std::vector<BasicBlock>& bbs, std::list<int>& visited, int src, int b, int prev)
{
	//dont continue along this path if we already visited b
	if(std::find(visited.cbegin(), visited.cend(), b) != visited.cend())
	{
		return -1;
	}
	//add b to visited nodes
	visited.push_back(b);
	//if we reached the src, return value of 
	if (b == src)
	{
		return prev;
	}
	for (int succ : bbs[b].getSucc())
	{
		int result = isLoopHeaderR(bbs, visited, src, succ, b);
		if (result > -1)
		{
			return result;
		}
	}
	return -1;
}

RegAllocator::~RegAllocator()
{
}

void RegAllocator::regAlloc(SSAoutput& ssa)
{
	for (SSAmethod& m : ssa.getOutput())
	{
		printf("%s\n", m.getName().c_str());
		//compute intervals
		IntervalList intervals;
		computeLiveIntervals(intervals, m.getBasicBlocks());
		//construct interference graph
		InterferenceGraph IG;
		intervals.constructInterferenceGraph(IG);
		//color nodes
		RegMap map;
		chaitin(map, IG);
		m.addRegMap(map);
		//output map
		printf("%s\n", map.getStr().c_str());
	}
}
