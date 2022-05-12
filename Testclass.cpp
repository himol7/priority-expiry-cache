#include "Testclass.h"
#include "Pec.h"
#include "Node.h"
#include <assert.h>  
#include <iostream>

using namespace std;

Testclass::Testclass(){}


//	This test case performs 7 insertions with monotonically increasing priority and checks if the node with least priority is evicted every time.
void Testclass::evictNodeWithLeastPriorityTest()
{
	myLog("Test Case : Evicting node with least priority ");
	PEC pec = PEC(3);	// Cache Size 3

	int maxRuns = 4;

	std::string key = "Key ";
	std::string value = "Value ";

	pec.set(key + "0", value + "0", 0, 1000);
	pec.set(key + "1", value + "1", 1, 1000);
	pec.set(key + "2", value + "2", 2, 1000);

	for(int i = 3 ; i < maxRuns ; i++)
	{
		pec.set(key + to_string(i), value + to_string(i), i, 1000);

		//	Checks if the three latest inserted key-value pairs exist
		assert(pec.checkIfKeyExists(key + to_string(i)));		
		assert(pec.checkIfKeyExists(key + to_string(i-1)));	
		assert(pec.checkIfKeyExists(key + to_string(i-2)));			
		
		//	Checks if the key-value pair with minimum priority is evicted
		assert(!pec.checkIfKeyExists(key + to_string(i-3)));

		//	Checks if the three maximum priority values exist
		assert(pec.checkIfPriorityLinkedlistExists(i));
		assert(pec.checkIfPriorityLinkedlistExists(i-1));
		assert(pec.checkIfPriorityLinkedlistExists(i-2));

		//	Checks if the linkedlist with minimum priority is erased
		assert(!pec.checkIfPriorityLinkedlistExists(i-3));
	}

	myLog("PASS");
}


//	This test case performs 7 insertions with monotonically increasing expiry times and checks if the node with earliest expiry is evicted every time.
void Testclass::evictNodeWithLeastExpiryTest()
{
	myLog("Test Case : Evicting node with least expiry ");
	PEC pec = PEC(3);	// Cache Size 3

	int maxRuns = 4;

	std::string key = "Key ";
	std::string value = "Value ";

	pec.set(key + "0", value + "0", 5, 10);
	pec.set(key + "1", value + "1", 5, 20);
	pec.set(key + "2", value + "2", 5, 30);

	for(int i = 3 ; i < maxRuns ; i++)
	{
		wait(10);	//	At every 10 milliseconds, 1 key with the earliest time will expire

		pec.set(key + to_string(i), value + to_string(i), 5, 30);	//	The shift in time is 30 milliseconds 

		//	Checks if the three most recent key-value pairs exist
		assert(pec.checkIfKeyExists(key + to_string(i)));		
		assert(pec.checkIfKeyExists(key + to_string(i-1)));	
		assert(pec.checkIfKeyExists(key + to_string(i-2)));			
		
		//	Checks if the key which has expired at t-30 milliseconds does not exist
		assert(!pec.checkIfKeyExists(key + to_string(i-3)));

		// 	Checks if the size of Expiry Linkedlists Map is always 3
		assert(pec.getSizeOfExpiryLinkedlistMap() == 3);
	}

	myLog("PASS");
}

//	This test case checks is the Least Recently Used Key Value Pair is evicted when there are multiple pairs available with least priority
void Testclass::evictLRUPairForLeastPriorityTest()
{
	myLog("Test Case : Evicting Least Recently Used Node for Least Priority when multiple nodes are available ");
	PEC pec = PEC(4);	// Cache size 4

	pec.set("Key 1", "Value 1", 3, 1000);
	pec.set("Key 2", "Value 2", 3, 1000);
	pec.set("Key 3", "Value 3", 3, 1000);
	pec.set("Key 4", "Value 4", 3, 1000);
	
	for(int i = 0 ; i < 5 ; i++)	//	-> brings Key 2 at front of the Priority 3 list
		pec.get("Key 2");	//	get() function called arbitrary number of times 

	for(int i = 0 ; i < 3 ; i++)	//	-> brings Key 1 at front of the Priority 3 list
		pec.set("Key 1", "Value 1", 3, 1000);	//	set() function called arbitrary number of times 

	// Key 3 should be at the end of the Priority 3 list now

	pec.set("Key 5", "Value 5", 3, 1000);

	assert(!pec.checkIfKeyExists("Key 3"));	//	Key 3 is evicted since it is least recently used key with least priority
	assert(pec.checkIfKeyExists("Key 5"));	//	Key 5 is inserted

	myLog("PASS");
}


//	This test case checks if the expired keys are evicted in succcession when size hits max capacity
void Testclass::evictExpiredKeysWhenSizeExceededTest()
{
	myLog("Test Case : Successively Evicting Expired Keys whenever Cache Size reaches capacity ");
	PEC pec = PEC(3);	//	Cache Size 3

	pec.set("Key 1", "Value 1", 4, 100);
	pec.set("Key 2", "Value 2", 4, 100);
	pec.set("Key 3", "Value 3", 4, 200);

	wait(100);

	pec.set("Key 4", "Value 4", 5, 200);

	assert(pec.checkIfKeyExists("Key 1") != pec.checkIfKeyExists("Key 2"));	//	Either Key 1 or Key 2 is evicted when Key 4 is inserted
	assert(pec.checkIfKeyExists("Key 4"));	//	Key 4 is inserted

	pec.set("Key 5", "Value 5", 5, 300);

	assert(!pec.checkIfKeyExists("Key 1") && !pec.checkIfKeyExists("Key 2"));	//	Key 1 and Key 2 both are evicted
	assert(pec.checkIfKeyExists("Key 5"));	//	Key 5 is inserted

	myLog("PASS");
}


//	This test case checks if the key-value pairs are NOT evicted when the size hits max capacity and NO MORE KEY-VALUE PAIRS ARE INSERTED
void Testclass::noEvictionsWhenSizeNotExceededTest()
{
	myLog("Test Case : No Nodes evicted unless set() or get() is invoked ");
	PEC pec = PEC(3);	// Cache Size 3

	pec.set("Key 1", "Value 1", 4, 100);
	pec.set("Key 2", "Value 2", 4, 100);
	pec.set("Key 3", "Value 3", 4, 200);

	wait(200);

	assert(pec.checkIfKeyExists("Key 1"));	//	Key 1 exists even when it is expired
	assert(pec.checkIfKeyExists("Key 2"));	//	Key 2 exists even when it is expired
	assert(pec.checkIfKeyExists("Key 3"));	//	Key 3 exists even when it is expired

	myLog("PASS");
}


//	This test case checks if the value of the node is updated correctly and it is not the Least Recently Used Key-Value pair once updated
void Testclass::updateValueTest()
{
	myLog("Test Case : Check if Value is correctly updated and LRU principle is maintained ");
	PEC pec = PEC(3);	// Cache Size 3

	pec.set("Key 1", "Value 1", 4, 100);
	pec.set("Key 2", "Value 2", 4, 100);
	pec.set("Key 3", "Value 3", 4, 200);

	pec.set("Key 1", "Value 1 Override", 4, 100);	//	Updates value for Key 1

	pec.set("Key 4", "Value 4", 4, 100);

	assert(pec.checkIfKeyExists("Key 1"));	//	Key 1 exists
	assert(pec.getValue("Key 1") == "Value 1 Override");	//	Value for Key 1 is updated
	assert(!pec.checkIfKeyExists("Key 2"));	//	Key 2 is evicted when Key 4 is inserted
	assert(pec.checkIfKeyExists("Key 4"));	//	Key 4 is inserted

	myLog("PASS");
}


//	This test case checks if the Priority Linkedlist gets erased if no nodes of that particular priority exists
void Testclass::emptyPriorityLinkedlistErasedTest()
{
	myLog("Test Case : Priority Linkedlist with no nodes is erased ");
	PEC pec = PEC(3);	// Cache Size 3

	pec.set("Key 1", "Value 1", 4, 100);
	pec.set("Key 2", "Value 2", 5, 200);
	pec.set("Key 3", "Value 3", 5, 200);

	pec.set("Key 4", "Value 4", 6, 200);	

	assert(pec.checkIfKeyExists("Key 4"));	//	Key 4 is inserted
	assert(!pec.checkIfKeyExists("Key 1"));	//	Key 1 with the least priority should be evicted when Key 4 is inserted
	assert(!pec.checkIfPriorityLinkedlistExists(4));	//	No Linkedlist with priority 4 should exist
	assert(pec.getSizeOfExpiryLinkedlistMap() == 1);	//	1 Expiry Linkedlist with 3 nodes should exist

	myLog("PASS");
}


//	This test case checks if the Expiry Linkedlist gets erased if no nodes of that particular expiry exxist
void Testclass::emptyExpiryLinkedlistErasedTest()
{
	myLog("Test Case : Expiry Linkedlist with no nodes is erased ");
	PEC pec = PEC(3);	// Cache Size 3

	pec.set("Key 1", "Value 1", 4, 100);
	pec.set("Key 2", "Value 2", 5, 200);
	pec.set("Key 3", "Value 3", 5, 200);

	wait(100);

	pec.set("Key 4", "Value 4", 5, 200);
	
	assert(pec.checkIfKeyExists("Key 4"));	//	Key 6 is inserted
	assert(!pec.checkIfKeyExists("Key 1"));	//	Key 1 with the earliest expiry should be evicted when Key 4 is inserted
	assert(pec.getSizeOfExpiryLinkedlistMap() == 2);	//	2 Linkedlists with at least 1 node in each of them exist
	assert(!pec.checkIfPriorityLinkedlistExists(4));	//	No linkedlist with Priority 4 exists since Key 1 is evicted
	
	myLog("PASS");
}


//	This test case checks if a key value pair is correctly evicted after updating priority of a certain key-value pair
void Testclass::updatePriorityTest()
{
	myLog("Test Case : Check if least priority node is removed after priorities of some nodes are updated ");
	PEC pec = PEC(3);	// Cache Size 3

	pec.set("Key 1", "Value 1", 3, 100);
	pec.set("Key 2", "Value 2", 4, 200);
	pec.set("Key 3", "Value 3", 5, 200);

	pec.set("Key 2", "Value 2", 6, 100);	//	Updates priority of Key 1 from 4 to 6

	pec.set("Key 4", "Value 4", 6, 200);

	assert(pec.checkIfKeyExists("Key 4"));	//	Key 4 is inserted
	assert(!pec.checkIfKeyExists("Key 1"));	//	Key 1 should be evicted since it is LRU for Priority 3 when Key 4 is inserted

	pec.set("Key 5", "Value 5", 6, 200);

	assert(pec.checkIfKeyExists("Key 5"));	//	Key 5 is inserted
	assert(!pec.checkIfKeyExists("Key 3"));	//	Key 3 should be evicted since it is LRU for Priority 5 when Key 5 is inserted
	assert(pec.checkIfKeyExists("Key 4"));	//	Key 4 should exist with Priority 6 
	assert(pec.checkIfKeyExists("Key 2"));	//	Key 2 should exist with Priority 6 

	myLog("PASS");
}


//	This test case performs several set() and get() operations on the cache and verifies its state after every operation
void Testclass::sequentialOperationsTest()
{
	myLog("Test Case : Perform multiple set() and get() operations and verfiy cache state after every operation ");
	PEC pec = PEC(3);	// Cache Size 3

	pec.set("Key 1", "Value 1", 1, 1000);
	pec.set("Key 2", "Value 2", 1, 1000);
	pec.set("Key 3", "Value 3", 1, 1000);

	assert(pec.checkIfKeyExists("Key 1"));	//	Key 1 should exist
	assert(pec.checkIfKeyExists("Key 2"));	//	Key 2 should exist
	assert(pec.checkIfKeyExists("Key 3"));	//	Key # should exist

	pec.set("Key 1", "Value 1 Override", 1, 1000);

	assert(pec.getValue("Key 1") == "Value 1 Override");	//	Value 1 should be updated to Value 1 Overrride
	assert(pec.get("Key 2") == "Value 2");	//	Verify Value 2 for Key 2
	assert(pec.get("Key 3") == "Value 3");	//	Verify Value 3 for Key 3

	pec.set("Key 3", "Value 3", 3, 1000);	//	Updated Priority of Key 3

	assert(pec.checkIfPriorityLinkedlistExists(3));	//	Verify Linkedlist with Priority 3 exists
	assert(pec.checkIfPriorityLinkedlistExists(1));	//	Verify Linkedlist with Priority 1 exists

	pec.set("Key 4", "Value 4", 3, 100);	

	assert(pec.checkIfKeyExists("Key 4"));	//	Key 4 should exist in the cache
	assert(!pec.checkIfKeyExists("Key 1"));	//	Key 1 should be evicted since it is LRU with least priority when Key 4 is inserted
	assert(pec.checkIfKeyExists("Key 3"));	//	Key 3 should exist
	assert(pec.checkIfKeyExists("Key 2"));	//	Key 2 should exist

	wait(100);

	assert(pec.get("Key 4") == "Key Has Expired");	//	Key 4 should expire

	myLog("PASS");
}








