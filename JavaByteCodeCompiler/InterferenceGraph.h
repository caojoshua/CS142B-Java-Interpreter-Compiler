
#ifndef INTGRAPH_H
#define INTGRAPH_H

#include "SSAInstruction.h"
#include <list>
#include <stack>
#include <map>

class InterferenceGraph
{
public:
	class Node
	{
	public:
		//variables
		SSA::OperandUse op;
		std::list<SSA::OperandUse> edges;
		//functions
		Node() : op() {}
		Node(SSA::OperandUse op) : op(op) {}
		Node(const Node& other);
		bool operator==(const Node& other) const;
		void addEdge(SSA::OperandUse op);
		void removeEdge(SSA::OperandUse op);
		int numEdges() const;
	};
private:
	std::list<Node> nodes;
public:
	InterferenceGraph();
	~InterferenceGraph();
	InterferenceGraph(const InterferenceGraph& other);
	void addNode(SSA::OperandUse op);
	void addEdge(SSA::OperandUse op1, SSA::OperandUse op2);
	std::list<Node> getNodes();
	void removeNode(Node n);
	void removeNode(SSA::OperandUse op);
	Node getNode(SSA::OperandUse op);
	bool empty();
};



#endif
