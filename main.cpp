#include "Testclass.h" 

#include <iostream>
using namespace std;

int main()
{
	Testclass testclass = Testclass();

	testclass.evictNodeWithLeastPriorityTest();
	testclass.evictNodeWithLeastExpiryTest();
	testclass.evictLRUPairForLeastPriorityTest();
	testclass.evictExpiredKeysWhenSizeExceededTest();
	testclass.noEvictionsWhenSizeNotExceededTest();
	testclass.updateValueTest();
	testclass.emptyPriorityLinkedlistErasedTest();
	testclass.emptyExpiryLinkedlistErasedTest();
	testclass.updatePriorityTest();
	testclass.sequentialOperationsTest();

	return 0;
}
