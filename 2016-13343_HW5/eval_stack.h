#pragma once

/*
* friend class With stack & Procedure
* each node element of a stack
* stores hashVal & pointer
* has pointer to before element
*/
class EvalStackNode {
	int hashvalue;
	int pointer;
	EvalStackNode* before;
	EvalStackNode(int hashvalue, int pointer) {
		this->hashvalue = hashvalue;
		this->pointer = pointer;
		before = 0;
	}
	friend class EvalStack;
	friend class Procedure;
};

/*
* friend class Procedure
* represents the evaluation stack
* has basic stack operations: isEmpty, Pop, Push
* hash wipeStack for error occuring and needs to wipe the stack clean
*/
class EvalStack {
	EvalStackNode* last;
	EvalStack() {
		last = 0;
	}

	bool isEmpty() {
		return last == 0;
	}
	void Pop() {
		if (!isEmpty()) {
			last = last->before;
		}
	}
	void Push(int hashvalue, int pointer) {
		EvalStackNode* temp = new EvalStackNode(hashvalue, pointer);
		temp->before = last;
		last = temp;
	}
	friend class Procedure;
	void wipeStack() {
		while (last != 0) {
			EvalStackNode* temp = last;
			last = last->before;
			delete temp;
		}
	}
};