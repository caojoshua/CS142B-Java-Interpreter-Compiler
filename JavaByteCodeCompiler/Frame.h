
#ifndef FRAME_H
#define FRAME_H

#include <vector>
#include <stack>
//#include <utility>

class Frame
{
private:
	std::vector<int> localVars;
	std::stack<int> operandStack;
public:
	Frame(int maxLocals);
	~Frame();

	//set local var with given index to val
	void setLocalVar(uint8_t index, int val);
	//return value of local var with given index
	int getLocalVar(uint8_t index);
	//push val onto stack
	void push(int val);
	//pop val from stack and return it
	int pop();
	//pop two values from stack and return as pair
	//second from top of stack is first, top of stack is second
	//this is useful for commutative properties e.g. A-B
	std::pair<int, int> popTwo();
	//pop N values from stack and return as vector
	//0th element is last element popped, n-1th element is first element popped
	//use this for passing N arguments to methods
	std::vector<int> popN(int n);
};

#endif

