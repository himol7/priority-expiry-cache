// Dimensions:
// 0 - Priority
// 1 - Expiry

#define PRIORITY 0
#define EXPIRY 1
#define DIMENSIONS 2

#include "Node.h"
#include "Linkedlist.h"
#include "Pec.h"

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <iostream>
#include <chrono>  

using namespace std;

PEC::PEC(int capacity)
{
	maxCapacity = capacity;	//	Setting maximum size of the cache
	dimensions = DIMENSIONS;	//	Setting dimensions of the cache - 2 (PRIORITY, EXPIRY)
	size = 0;	//	initialising current size of the cache to be 0
}

void PEC::set(std::string key, std::string val, int priority, int givenExpiry)
{
	Node* node;

	//	setting Expiry time of a key-value pair to be current time + given expiry time in milliseconds
	//	If Expiry Time of a node is 100 ms and current time is t, the final expiry time for the key-value pair will be t+100 milliseconds
	long expiry = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() + (long) givenExpiry;		

	if(size+1 > maxCapacity)
	{
		evictItem();	//	If the cache is already full, we evict a key-value pair before inserting a new one
	}

	if(keyToNodes.find(key) == keyToNodes.end())	
	{
		node = new Node();
		node->key = key;
		node->val = val;
		node->priority = priority;
		node->expiry = expiry;

		if(priorityToLists.find(priority) == priorityToLists.end())
		{
			priorityToLists.insert({priority, Linkedlist(PRIORITY)});	//	Creating a new Linkedlist of provided priority if it does not exist
		}

		priorityToLists.find(priority)->second.addNode(node);	//	Adding the node to linkedlist of the provided priority

		if(expiryToLists.find(expiry) == expiryToLists.end())
		{
			expiryToLists.insert({expiry, Linkedlist(EXPIRY)});	//	Creating a new Linkedlist of provided expiry if it does not exist
		}

		expiryToLists.find(expiry)->second.addNode(node);	//	Adding the node to linkedlist of the provided expiry

		keyToNodes.insert({key, node});	//	Adding node to Key - Node map
		size++;	//	Incrementing size since new node is added
	}
	else
	{
		node = keyToNodes[key];
		node->val = val;		

		if(node->priority != priority)	//	Removing the node from current priority linkedlist and adding it to new priority linkedlist iff priority has been updated
		{
			node->remove();
			if(priorityToLists.find(node->priority)->second.head->nextNodes[PRIORITY] == priorityToLists.find(node->priority)->second.tail)
				priorityToLists.erase(node->priority);	//	Erasing the map entry if the priority linkedlist is empty

			node->priority = priority;	//	Updating node's priority

			if(priorityToLists.find(priority) == priorityToLists.end())
				priorityToLists.insert({priority, Linkedlist(PRIORITY)});

			priorityToLists.find(priority)->second.addNode(node);	//	Adding node to new priority linkedlist

			expiryToLists.find(node->expiry)->second.addNode(node);	//	Adding node to the expiry linkedlist since it was dissociated from it earlier
		}
		else
		{
			priorityToLists.find(priority)->second.moveToFront(node);	//	Maintaining the LRU principle if priority is not updated
		}
	}
}


std::string PEC::get(std::string key)
{
	Node* node;
	long currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	if(keyToNodes.find(key) == keyToNodes.end())	//	If key does not exist in Key - Node map, return "Key Node Available"
		return "Key Not Available";

	if(keyToNodes[key]->expiry < currentTime)	//	If key has already expired, evict the key and return "Key Has Expired"
	{
		node = keyToNodes[key];
		node->remove();

		if(priorityToLists.find(node->priority)->second.head->nextNodes[PRIORITY] == priorityToLists.find(node->priority)->second.tail)
			priorityToLists.erase(node->priority);	//	Erasing priority linkedlist if it's empty after removing the node

		if(expiryToLists.find(node->expiry)->second.head->nextNodes[EXPIRY] == expiryToLists.find(node->expiry)->second.tail)
			expiryToLists.erase(node->expiry);	//	Erasing expiry linkedlist if it's empty after removing the node

		return "Key Has Expired";
	}

	node = keyToNodes[key]; 
	priorityToLists.find(node->priority)->second.moveToFront(node);	//	Maintaining LRU principle
	return node->val;	//	Return node's current value
}

void PEC::evictItem()
{
	long currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	auto expiryToListsIterator = expiryToLists.begin();	//	Since ordered map is used to store Expiry-Linkedlist Mapping, first entry of the map will have minimum expiry time

	if(expiryToListsIterator->first <= currentTime)	//	Check if any expired key is available
	{
		//	Selecting first node from linkedlist of the least available expiry lesser than current time
		Node* node = expiryToListsIterator->second.head->nextNodes[EXPIRY];
		int priority = node->priority;
		keyToNodes.erase(node->key);	//	Erase the key from Key to Node Map
		node->remove();	//	Dissociate node from all the linkedlists
		
		if(expiryToListsIterator->second.head->nextNodes[EXPIRY] == expiryToListsIterator->second.tail)
			expiryToLists.erase(expiryToListsIterator);		//	Erasing expiry linkedlist if it's empty after removing the node

		if(priorityToLists.find(priority)->second.head->nextNodes[PRIORITY] == priorityToLists.find(priority)->second.tail)
			priorityToLists.erase(priority);	//	Erasing priority linkedlist if it's empty after removing the node

		size--;	//	Decrementing the size
		return;
	}

	//	If there is no expired key available, following code is executed

	auto priorityToListsIterator = priorityToLists.begin();	//	As ordered map is used to store Priority-Linkedlist Mapping, first entry of the map will have minimum priority

	//	Selecting the least recently used node from Priority Linkedlist which is node right before the tail node

	Node* node = priorityToListsIterator->second.tail->prevNodes[PRIORITY];
	long expiry = node->expiry;
	keyToNodes.erase(node->key);	//	Erase the key from Key to Node Map
	node->remove();	//	Dissociate node from all the linkedlists

	if(priorityToListsIterator->second.head->nextNodes[PRIORITY] == priorityToListsIterator->second.tail)
		priorityToLists.erase(priorityToListsIterator);		//	Erasing priority linkedlist if it's empty after removing the node

	if(expiryToLists.find(expiry)->second.head->nextNodes[EXPIRY] == expiryToLists.find(expiry)->second.tail)
		expiryToLists.erase(expiry);	//	Erasing expiry linkedlist if it's empty after removing the node

	size--;		//	Decrementing the size
}





//	Following methods are written for debugging and testing purposes
//	They do not affect/interfere the actual functioning of PEC

//	Method to view current value in the cache
void PEC::displayCache()
{
	cout << string(100, '#') << endl;
	Node* node;
	cout << "Key to Value Map" << endl;
	auto keyToNodesIterator = keyToNodes.begin();
	while(keyToNodesIterator != keyToNodes.end())
	{
		cout << "Key : " << keyToNodesIterator->first << " Value : " << keyToNodesIterator->second->val << endl;
		keyToNodesIterator++; 
	}
	cout << endl;

	cout << "Priority Based Linkedlist Map" << endl;
	auto priorityToListsIterator = priorityToLists.begin();
	while(priorityToListsIterator != priorityToLists.end())
	{
		cout << "Priority : " << priorityToListsIterator->first << " ";

		node = priorityToListsIterator->second.head->nextNodes[PRIORITY];
		while(node != priorityToListsIterator->second.tail)
		{
			cout << node->key << ", " << node->val << " ";
			node = node->nextNodes[PRIORITY];
		}
		cout << endl;
		priorityToListsIterator++;
	}
	cout << endl;

	cout << "Expiry Based Linkedlist Map" << endl;
	auto expiryToListsIterator = expiryToLists.begin();
	while(expiryToListsIterator != expiryToLists.end())
	{
		cout << "Expiry : " << expiryToListsIterator->first << " ";

		node = expiryToListsIterator->second.head->nextNodes[EXPIRY];
		while(node != expiryToListsIterator->second.tail)
		{
			cout << node->key << ", " << node->val << " ";
			node = node->nextNodes[EXPIRY];
		}
		cout << endl;
		expiryToListsIterator++;
	}
	cout << string(100, '#') << endl;
}

//	Method to check if a key exists in the Key - Node Map
bool PEC::checkIfKeyExists(std::string key)
{	
	if(keyToNodes.find(key) != keyToNodes.end())
		return true;
	return false;
}

//	Method to get value for a key without invoking get() method
std::string PEC::getValue(std::string key)
{
	return keyToNodes[key]->val;
}

//	Method to check if a linkedlist for a particular priority exists or not
bool PEC::checkIfPriorityLinkedlistExists(int priority)
{
	if(priorityToLists.find(priority) != priorityToLists.end())
		return true;
	return false;
}

//	Method to get size of Expiry-Linkedlist Map
int PEC::getSizeOfExpiryLinkedlistMap()
{
	return expiryToLists.size();
}

