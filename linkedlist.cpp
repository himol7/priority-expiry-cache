#define PRIORITY 0
#define EXPIRY 1

#include "Node.h"
#include "Linkedlist.h"
#include <iostream>

using namespace std;

Linkedlist::Linkedlist(int providedDimension)
{		
	// If providedDimension is 0 -> the class instantiates an object for a Priority Linkedlist
	// If providedDimension is 1 -> the class instantiates an object for an Expiry Linkedlist
	dimension = providedDimension;

	head = new Node();
	tail = new Node();

	head->nextNodes[dimension] = tail;
	tail->prevNodes[dimension] = head;
}

//	Updates pointers for the node and the linkedlist of preset dimension
//	Adds node in front of the linkedlist right after HEAD of the linkedlist
void Linkedlist::addNode(Node* node)
{
	node->nextNodes[dimension] = head->nextNodes[dimension];
	node->prevNodes[dimension] = head;
	head->nextNodes[dimension] = node;
	node->nextNodes[dimension]->prevNodes[dimension] = node;
}

//	Method to bring the most recently used node to front of the linkedlist to maintain LRU principle
//	This is only done if the preset dimension is PRIORITY i.e. it is a Priority Linkedlist
void Linkedlist::moveToFront(Node* node)
{
	if(dimension != PRIORITY)
		return;

	node->nextNodes[PRIORITY]->prevNodes[PRIORITY] = node->prevNodes[PRIORITY];
	node->prevNodes[PRIORITY]->nextNodes[PRIORITY] = node->nextNodes[PRIORITY];

	node->nextNodes[PRIORITY] = head->nextNodes[PRIORITY];
	node->prevNodes[PRIORITY] = head;
	head->nextNodes[PRIORITY] = node;
	node->nextNodes[PRIORITY]->prevNodes[PRIORITY] = node;
}