#include "Frame.h"



Frame::Frame(int maxLocals)
{
	//set localVars to vectors wtih size maxLocals and all values initialized to 0
	localVars = std::vector<int>(maxLocals, 0);
	operandStack = std::stack<int>();
}


Frame::~Frame()
{
}

void Frame::setLocalVar(uint8_t index, int val)
{
	if (index >= localVars.size())
	{
		printf("attempted to set local variable %d when max locals is %d", index, localVars.size());
		exit(1);
	}
	localVars[index] = val;
}

int Frame::getLocalVar(uint8_t index)
{
	if (index >= localVars.size())
	{
		printf("attempted to get local variable %d when max locals is %d", index, localVars.size());
		exit(1);
	}
	return localVars[index];
}

void Frame::push(int val)
{
	//printf("pushing %d\n", val);
	operandStack.push(val);
}

int Frame::pop()
{
	if (operandStack.size() < 1)
	{
		printf("attempted to pop from operand stack when stack size was 0");
		exit(1);
	}
	int top = operandStack.top();
	operandStack.pop();
	//printf("popping %d\n", top);
	return top;
}

std::pair<int, int> Frame::popTwo()
{
	int second = pop();
	int first = pop();
	return std::pair<int, int>(first, second);
}

std::vector<int> Frame::popN(int n)
{
	std::vector<int> v(n, 0);
	for (int i = 0; i < n; i++)
	{
		v[n - i - 1] = pop();
	}
	return v;
}

