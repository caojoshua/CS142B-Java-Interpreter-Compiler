#include "InterferenceGraph.h"



InterferenceGraph::InterferenceGraph()
{
}


InterferenceGraph::~InterferenceGraph()
{
}

InterferenceGraph::InterferenceGraph(const InterferenceGraph & other)
{
	for (Node n : other.nodes)
	{
		nodes.push_back(Node(n));
	}
}

void InterferenceGraph::addNode(SSA::OperandUse op)
{
	nodes.push_back(Node(op));
}

void InterferenceGraph::addEdge(SSA::OperandUse op1, SSA::OperandUse op2)
{
	//don't add edge if they are some op
	if(op1 == op2)
	{
		return;
	}
	//find two nodes with operands corresponding to parameters
	Node* n1;
	Node* n2;
	for (Node& n : nodes)
	{
		if (op1 == n.op)
		{
			n1 = &n;
		}
		if (op2 == n.op)
		{
			n2 = &n;
		}
	}
	//add the edges two both nodes
	n1->addEdge(n2->op);
	n2->addEdge(n1->op);
}

std::list<InterferenceGraph::Node> InterferenceGraph::getNodes()
{
	return nodes;
}

void InterferenceGraph::removeNode(Node n)
{
	removeNode(n.op);
}

void InterferenceGraph::removeNode(SSA::OperandUse op)
{
	//first remove node from the list
	nodes.remove(Node(op));
	//remove all edges going to op
	for (Node& n : nodes)
	{
		n.removeEdge(op);
	}
}

InterferenceGraph::Node InterferenceGraph::getNode(SSA::OperandUse op)
{
	for (Node& n : nodes)
	{
		if (n.op == op)
		{
			return n;
		}
	}
	return Node();
}

bool InterferenceGraph::empty()
{
	return nodes.empty();
}

InterferenceGraph::Node::Node(const Node & other)
{
	op = SSA::OperandUse(other.op);
	for (SSA::OperandUse edge : other.edges)
	{
		edges.push_back(SSA::OperandUse(edge));
	}
}

bool InterferenceGraph::Node::operator==(const Node & other) const
{
	return op == other.op;
}

void InterferenceGraph::Node::addEdge(SSA::OperandUse op)
{
	edges.push_back(op);
}

void InterferenceGraph::Node::removeEdge(SSA::OperandUse op)
{
	edges.remove(op);
}

int InterferenceGraph::Node::numEdges() const
{
	return edges.size();
}
