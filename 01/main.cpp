#include <assert.h>
#include <iostream>
#include <cstring>

const int initial_size = 16;

class Stack {
public:
	bool empty() const;
	int top() const;
	int pop();
	void push(int value);
private:
	int* buffer = new int[initial_size];
	int* head = buffer;
	int size = initial_size;
public:
	~Stack() { if (buffer != nullptr) delete[] buffer; };
};


bool Stack::empty() const {
	return buffer == head;
}
int Stack::top() const {
	assert(!empty());
	return *head;
}

int Stack::pop() {
	assert(!empty());
	return *(--head);
}

void Stack::push(int value) {
	if (buffer + size == head) {
		int* new_buffer = new int[size * 2];
		std::memcpy(new_buffer, buffer, size*sizeof(int));
		delete[] buffer;
		buffer = new_buffer;
		head = new_buffer + size;
		size *= 2;
	}
	*(head++) = value;
}


class Queue {
public:
	bool empty() const;
	void push(int value);
	int pop();

	Queue() {};
	~Queue() {};
private:
	Stack s1;
	Stack s2;
};

bool Queue::empty() const {
	return s1.empty() && s2.empty();
}

void Queue::push(int value) {
	s1.push(value);
}

int Queue::pop() {
	if (!s2.empty()) {
		return s2.pop();
	}
	while (!s1.empty()) {
		s2.push(s1.pop());
	}
	assert(!s2.empty());
	return s2.pop();	
}

int main() {
	int commands_count = 0;
	std::cin >> commands_count;

	Queue queue;
	for (int i = 0; i < commands_count; ++i) {
		int command = 0;
		int value = 0;
		std::cin >> command >> value;
		if (command == 3) {
			queue.push(value);
		}
		else if (command == 2) {
			if (queue.empty()) {
				if (value != -1) {
					std::cout << "NO";
					return 0;
				}
			}
			else if (queue.pop() != value) {
				std::cout << "NO";
				return 0;
			}
		}
	}
	std::cout << "YES";

	return 0;
}

