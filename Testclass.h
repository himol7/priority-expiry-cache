#ifndef TESTCLASS_H
#define TESTCLASS_H

#include "Pec.h"
#include "Node.h"
#include <assert.h>  
#include <iostream>

using namespace std;

class Testclass
{
public:

	Testclass();
	void evictNodeWithLeastPriorityTest();
	void evictNodeWithLeastExpiryTest();
	void evictLRUPairForLeastPriorityTest();
	void evictExpiredKeysWhenSizeExceededTest();
	void noEvictionsWhenSizeNotExceededTest();
	void updateValueTest();
	void emptyPriorityLinkedlistErasedTest();
	void emptyExpiryLinkedlistErasedTest();
	void updatePriorityTest();
	void sequentialOperationsTest();
};

#endif