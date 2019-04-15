#include "Tables.h"

//TODO: fix memory leak
//when we create methods, we create a vector of Attr* 
//calling operator= on vector will naively copy pointers
//when returning to the caller, garbage collection
//will delete the vector and apparently each pointer in the vector
//
//possible solution: implement deep copy of vector and Attrs
//this will involve overriding for each Attr subclass
Method::~Method()
{
	/*for (Attr* attr : attrs)
	{
		delete attr;
	}*/
}
