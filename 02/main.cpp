#include <assert.h>
#include <iostream>
#include <cstring>

const int initial_capacity = 16;

class Heap {
public:
	bool empty() const { return size == 0; };
	int top() const;
	int pop();
	void push(int value);
private:
	int* buffer = new int[initial_capacity];
	int capacity = initial_capacity;
	int size = 0;

	void sift_down(int index);
	void sift_up(int index);
public:
	~Heap() { if (buffer != nullptr) delete[] buffer; };
};

//просеивание элемента вниз
void Heap::sift_down(int index) {
	assert((index >= 0) && (index < size));

	int child1_index = (index << 1) + 1;
	if (child1_index >= size) {
		//нет потомков
		return; 
	}

	if (child1_index + 1 == size) {
		//есть только 1 потомок		
		if ((buffer[index]) >= (buffer[child1_index])) {
			return;
		}
		std::swap(buffer[index],buffer[child1_index]);
		//дальше просеиваться не имеет смысла
		return;
	}
	
	int child2_index = child1_index + 1;

	if ((buffer[index] >= buffer[child1_index]) && (buffer[index] >= buffer[child2_index])) {		
		//текущий элемент больше или равен дочерним
		return;
	}

	//меняемся местами с бОльшим из дочерних элементов и пытаемся просеиться дальше
	if (buffer[child1_index] >= buffer[child2_index]) {
		std::swap(buffer[index], buffer[child1_index]);
		sift_down(child1_index);
	}
	else {
		std::swap(buffer[index], buffer[child2_index]);
		sift_down(child2_index);
	}	
}

//просеивание элемента вверх
void Heap::sift_up(int index) {
	assert((index >= 0) && (index < size));

	if (index == 0) {
		//корневой элемент
		return;
	}

	int parent_index = (index - 1) >> 1;
	if (buffer[parent_index] < buffer[index]) {
		std::swap(buffer[parent_index], buffer[index]);
		sift_up(parent_index);
	}
}

int Heap::top() const {
	assert(!empty());

	return buffer[0];
}

int Heap::pop() {
	assert(!empty());

	int result = buffer[0];
	if (--size > 0) {
		//в корень записываем последний элемент и просеиваем его вниз
		buffer[0] = buffer[size];
		sift_down(0);
	}

	return result;
}

void Heap::push(int value) {
	if (size == capacity) {
		//надо перевыделить место
		//новый размер буфера увеличим вдвое
		capacity <<= 1;
		int* new_buffer = new int[capacity];
		std::memcpy(new_buffer, buffer, size*sizeof(int));
		delete[] buffer;
		buffer = new_buffer;
	}

	buffer[size] = value;
	sift_up(size++);
}

class Stack {
public:
	bool empty() const;
	int top() const;
	int pop();
	void push(int value);
private:
	int* buffer = new int[initial_capacity];
	int* head = buffer;
	int size = initial_capacity;
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

int main() {
	int n;
	std::cin >> n;
	//бабушкина корзина на основе кучи
	Heap bucket;
	for (int i = 0; i < n; i++) {
		int fruit;
		std::cin >> fruit;
		bucket.push(fruit);
	}
	int K;
	std::cin >> K;

	int p = 0;
	while (!bucket.empty()) {
		int m = 0;
		//Вовочкина корзина, в неё он на текущем подходе будет складывать огрызки.
		//Можно было бы для неё тоже использовать Heap, но мы возьмём Stack, так как со стеком операции имеют сложность O(1).
		Stack bucket2;
		//Пока бабушкина корзина не пуста и масса уже набранных за подход фруктов + масса самого тяжёлого не превышает K
		while ((!bucket.empty()) && (m + bucket.top() <= K)) {
			int fruit = bucket.pop();
			//вытаскиваем самый тяжёлый и добавляем его массу в m
			m += fruit;
			//отгрызаем бОльшую половину от фрукта (сдвигом вправа на 1 бит вместо деления на 2)
			fruit >>= 1;
			if (fruit > 0) {
				//если от фрукта ещё что-то осталось, кладём огрызок в Вовочкину корзину
				bucket2.push(fruit);
			}
		}
		//перекладываем огрызки фруктов из Вовочкиной в бабушкину корзину
		while (!bucket2.empty()) {
			bucket.push(bucket2.pop());
		}
		p++;
	}

	std::cout << p << std::endl;

	return 0;
}
