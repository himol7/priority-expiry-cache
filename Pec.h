#ifndef PEC_H
#define PEC_H

#include "Node.h"
#include "Linkedlist.h"

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <iostream>
#include <chrono>  

using namespace std;

class PEC
{

private:

	int size;
	int maxCapacity;
	int dimensions;

	std::unordered_map<std::string, Node*> keyToNodes;
	std::map<int, Linkedlist> priorityToLists;
	std::map<long, Linkedlist> expiryToLists;

public:

	//	Implementation Methods
	PEC(int capacity);
	void set(std::string key, std::string val, int priority, int expiry);
	std::string get(std::string key);
	void evictItem();

	// Methods used for Testing and Verification
	void displayCache();
	bool checkIfKeyExists(std::string key);
	std::string getValue(std::string key);
	bool checkIfPriorityLinkedlistExists(int priority);
	int getSizeOfExpiryLinkedlistMap();
};

#endif