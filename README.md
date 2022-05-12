# How to run the program

- Unzip **pec.zip** and make sure they all underlying files exist in the same folder. Install a C++ 14 compiler if there isn't any.
- **Testclass.cpp** contains all the tests which are run on the program
- Run <code>g++ -std=c++14 node.cpp linkedlist.cpp pec.cpp testclass.cpp main.cpp -o main && ./main</code>


# Notes

- The code operates on these two dimensions : Priority and Expiry. The underlying data structures are extensible to N dimensions.
	- I am maintaning doubly linkedlists based on priority and expiry. 
	- Over those linkedlists, I am creating ordered maps by using priority / expiry values as Key and the Linkedlists as values.
	- This helps to access nodes of a certain priority or a certain expiry in O(lg(n)) time.

- Further for each node, I am maintaining vectors containing references of next and previous nodes based on each dimension. This
	facilitates removing the node from all the linkedlists at once in O(number of dimensions) time. That is O(1) for this implementation.

- When a Key Value Pair with Priority lower than all existing Priorities is inserted in case when Cache is full, I first do evict operation and then the insert operation. 

- The Node stores Key, Value, Expiry time and Priority of the Key-Value pair. In the documentation, occasionally the terms Node and Key-Value pair are used interchangabely.

## Data structures used
1. <code>std::unordered_map&lt;std::string, Node*> keyToNodes</code> to store Key to Node Mapping
2. <code>std::map&lt;int, Linkedlist> priorityToLists</code> to store Priority to Linkedlist Mapping
3. <code>std::map&lt;long, Linkedlist> expiryToLists</code> to store Expiry to Linkedlist Mapping


# Time Complexities

## set()
- Insert into main Key-Value Map: O(1) 
- Priority
	- Insert into Priority-Nodes Map: O(lg(n)) 
	- Insert into Priority based Linkedlist: O(1)
- Expiry
	- Insert into Expiry-Nodes Map: O(lg(n)) 
	- Insert into Expiry based Linkedlist: O(1)

Overall complexity: **O(lg(n))**

## get()
- Get value from Key-Value Map: O(1)
	- To maintain LRU - moveToFront() invoked - O(1)
	- If key has already expired - removing the node from all Linkedlists - O(#dimensions) - O(1) for this implementation

Overall complexity: **O(1)**

## evictItem()
- When item is removed based on Expiry Time
	- Obtain node with least expiry time: O(1)
	- Remove node from Key-Value Map: O(1)
	-	- Remove node from Expiry based Linkedlist: O(1)
		- Remove entry from Expiry-Nodes Map: O(lg(n))
- When item is removed based on Priority
	- Obtain Least Recently Used node with least priority: O(1)
	- Remove node from Key-Value Map: O(1)
	-	- Remove node from Priority based Linkedlist: O(1)
		- Remove entry from Priority-Nodes Map: O(lg(n))

Overall complexity: **O(lg(n))**

