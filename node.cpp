#define DIMENSIONS 2

#include "Node.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Constructor which initializes Next and Previous reference vectors with size of DIMENSIONS (here 2 - PRIORITY, EXPIRY)
Node::Node()
{
	nextNodes = std::vector<Node*>(DIMENSIONS);
	prevNodes = std::vector<Node*>(DIMENSIONS);
}

//	Method which updates next and previous nodes' references across all the dimensions i.e. across all the linkedlists.
//	Node on which this method is called removes itself from all the associated linkedlists
void Node::remove()
{
	for(int i = 0 ; i < DIMENSIONS ; i++)
	{
		nextNodes[i]->prevNodes[i] = prevNodes[i];
		prevNodes[i]->nextNodes[i] = nextNodes[i];
	}
}
