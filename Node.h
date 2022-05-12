#ifndef NODE_H
#define NODE_H

#include <string>
#include <iostream>
#include <vector>
#include <thread>        
#include <chrono>    

using namespace std;

class Node
{
public:

	// Storing Key, Value, Expiry time, Priority in the Node
	std::string key;
	std::string val;
	long expiry;
	int priority;

	// Vectors which contain Previous and Next references across all the dimensions for this node.
	std::vector<Node*> nextNodes;
	std::vector<Node*> prevNodes;

	Node();

	// method to remove this node from all the linkedlists.
	void remove();
};

// Static Logger Functions which are used to log stuff across the files 
static void myLog(std::string s)
{
	cout << s << endl; 
}

static void myLog(int i)
{
	cout << i << endl; 
}

// Static Wait Function to add sleep time
static void wait(int milliseconds)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}
#endif