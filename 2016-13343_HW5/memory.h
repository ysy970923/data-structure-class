#pragma once
#include "exception.h"
#include "symbol_table.h"
#include "eval_stack.h"
using namespace std;

#define MEMORY_SIZE 100

/*
* friend class with Memory
* binary node for memory tree
* lchild, rchild
* initializes childs to 0
*/
class MemoryNode {
private:
	friend class Procedure;
	friend class Memory;
	int lchild;
	int rchild;
	bool symbolConnected;
	MemoryNode() {
		lchild = 0;
		rchild = 0;
		symbolConnected = false;
	}
};

/*
* friend class with Procedure
* int free_list_root: root of free_list
* Node ID's are 1 ~ MEMORY_SIZE
* as free_list_root as root, free_list are linked by right child sequentially
* int getChild(int nodeID, char LorR): returns the child of the given node id to toWhat
* void changeChild(int nodeID, char LorR, int toWhat): changes the child of the given node id
* int Alloc(): returns free memory slot for use
* void FreeMemory(int root): frees the memory tree used from root back to free list
* void Print(): prints memory array
*/
class Memory {
private:
	friend class Procedure;
	MemoryNode* memoryList;
	int free_list_root;
	HistoryStack* history;
	Memory() {
		memoryList = 0;
		free_list_root = 0;
		history = 0;
	};
	Memory(HistoryStack& cache) {
		this->history = &cache;
		free_list_root = 1;
		memoryList = new MemoryNode[MEMORY_SIZE];
		for (int i = 0; i < MEMORY_SIZE - 1; i++) {
			memoryList[i].rchild = i + 2;
		}
		memoryList[MEMORY_SIZE - 1].rchild = 0;
	}
	int getChild(int nodeID, char LorR);
	void changeChild(int nodeID, char LorR, int toWhat);
	void changeChild(int nodeID, char LorR, int toWhat, bool cacheTrue);
	int Alloc();
	void garbageCollect();
	void garbageCollect(int index);
	void SaveMemory(int root);
	void Print();
};

/*
* returns the child of the given node id
* LorR: 'L' if left child, 'R' if right child
*/
int Memory::getChild(int nodeID, char LorR) {
	int index = nodeID - 1;
	if (LorR == 'L') {
		return memoryList[index].lchild;
	}
	else if (LorR == 'R') {
		return memoryList[index].rchild;
	}
	else {
		throw NOT_VALID_CHILD;
	}
}

/*
* changes the child of the given node id to toWhat
* LorR: 'L' if left child, 'R' if right child
*/
void Memory::changeChild(int nodeID, char LorR, int toWhat) {
	int index = nodeID - 1;
	if (LorR == 'L') {
		history->Push(nodeID, memoryList[index].lchild, 'L');
		memoryList[index].lchild = toWhat;
	}
	else if (LorR == 'R') {
		history->Push(nodeID, memoryList[index].rchild, 'R');
		memoryList[index].rchild = toWhat;
	}
	else {
		throw NOT_VALID_CHILD;
	}
}

/*
* changes the child of the given node id to toWhat
* LorR: 'L' if left child, 'R' if right child
*/
void Memory::changeChild(int nodeID, char LorR, int toWhat, bool cacheTrue) {
	int index = nodeID - 1;
	if (LorR == 'L') {
		memoryList[index].lchild = toWhat;
	}
	else if (LorR == 'R') {
		memoryList[index].rchild = toWhat;
	}
	else {
		throw NOT_VALID_CHILD;
	}
}


/*
* returns free memory slot for use
* allocate free_list_root and move free_list_root to next right child linked slot
* free-list is always right child linked starting from free_list_root until right child is 0
*/
int Memory::Alloc() {
	if (free_list_root != 0) {
		int returnVal = free_list_root;
		free_list_root = memoryList[free_list_root - 1].rchild;
		return returnVal;
	}
	else
		throw FULL_MEMORY_EXCEPTION;
}

/*
* if symbol is using this memory ==> reset boolean that was used for garbage check
* if lchild == 0, this means this memory is in free-list ==> no need for change
* if no symbol is using this memory ==> free memory
*/
void Memory::garbageCollect(int index) {
	if (index >= 0) {
		if (memoryList[index].symbolConnected) {
			memoryList[index].symbolConnected = false;
			return;
		}
		if (memoryList[index].lchild == 0)
			return;

		memoryList[index].rchild = free_list_root;
		memoryList[index].lchild = 0;
		free_list_root = index+1;
	}
}

/*
* iterate over memory and free if it is garbage
*/
void Memory::garbageCollect() {
	for (int i = 0; i < MEMORY_SIZE; i++) {
		garbageCollect(i);
	}
}

/*
* mark all elements in the tree starting from root is being used by a symbol
* mark root and recurrsively mark children
*/
void Memory::SaveMemory(int root) {
	int rootIndex = root - 1;
	if (root > 0) {
		if (memoryList[rootIndex].symbolConnected) 
			return;
		memoryList[rootIndex].symbolConnected = true;
		SaveMemory(memoryList[rootIndex].lchild);
		SaveMemory(memoryList[rootIndex].rchild);
	}
}

//prints memory array
void Memory::Print() {
	cout << endl << "Memory table = " << endl;
	int temp = 0;
	for (int i = 0; i < MEMORY_SIZE; i++) {
		cout << "Node ID " << i + 1 << ": " << memoryList[i].lchild << ", " << memoryList[i].rchild << ", " << memoryList[i].symbolConnected <<endl;
	}
}
