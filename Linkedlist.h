#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "Node.h"
#include <string>
#include <iostream>
#include <vector>

using namespace std;

class Linkedlist
{
private:
	int dimension;

public:

	Node* head;
	Node* tail;

	Linkedlist(int providedDimension);

	void addNode(Node* node);

	void moveToFront(Node* node);
};

#endif