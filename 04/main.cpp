/*
4_2.
Порядковая статистика
Ограничение времени	1 секунда
Ограничение памяти	16Mb
Ввод	стандартный ввод или input.txt
Вывод	стандартный вывод или output.txt
Даны неотрицательные целые числа n, k и массив целых чисел из диапазона [0..109] размера n.
Требуется найти k-ю порядковую статистику. т.е. напечатать число, которое бы стояло на позиции с индексом k ∈[0..n-1] в отсортированном массиве.
Напишите нерекурсивный алгоритм.
Требования к дополнительной памяти: O(n).
Требуемое среднее время работы: O(n).
Функцию Partition следует реализовывать методом прохода двумя итераторами в одном направлении.
Описание для случая прохода от начала массива к концу:
Выбирается опорный элемент.
Опорный элемент меняется с последним элементом массива.
Во время работы Partition в начале массива содержатся элементы, не бОльшие опорного. Затем располагаются элементы, строго бОльшие опорного. В конце массива лежат нерассмотренные элементы. Последним элементом лежит опорный.
Итератор (индекс) i указывает на начало группы элементов, строго бОльших опорного.
Итератор j больше i, итератор j указывает на первый нерассмотренный элемент.
Шаг алгоритма. Рассматривается элемент, на который указывает j. Если он больше опорного, то сдвигаем j. Если он не больше опорного, то меняем a[i] и a[j] местами, сдвигаем i и сдвигаем j.
В конце работы алгоритма меняем опорный и элемент, на который указывает итератор i.

4_2. Реализуйте стратегию выбора опорного элемента “медиана трёх”. Функцию Partition реализуйте методом прохода двумя итераторами от конца массива к началу.

Пример
Ввод	
10 4
1 2 3 4 5 6 7 8 9 10
Вывод
5
*/

#include <iostream>

//Функция выбирает опорный элемент и перераспределяет 
//все меньшие элементы массива слева от опорного, 
//все большие - справа от опорного.
//Возвращает индекс опорного элемента после переупорядочивания.
int partition(int* arr, int first, int last) {
	//ищем опорный элемент медианой из трёх: первого, последнего и среднего элемента массива 
	int mid = (first + last) / 2;
	if (arr[mid] < arr[first]) {
		std::swap(arr[first], arr[mid]);
	}	
	if (arr[last] < arr[first]) {
		std::swap(arr[first], arr[last]);
	}
	if (arr[mid]<arr[last]) {
		std::swap(arr[mid], arr[last]);
	}			
	//опорный элемент окажется в последнем элементе массива
	int pivot = arr[last];
	//проход по массиву в обратном порядке
	//i - позиция, начиная с которой будут элементы больше опорного
	//j - позиция крайнего непроверенного элемента.
	//с позиции j+1 до i все элементы меньше опорного
	int i=last, j=i-1;
	while (true) {
		if (j == first - 1) {
			//дошли до начала массива
			//меняем местами опорный и i-ый элемент
			std::swap(arr[last], arr[i]);			
			//возвращаем i
			return i;
		}
		if (arr[j] < pivot) {
			j--;
		}
		else {
			std::swap(arr[--i], arr[j--]);
		}		
	}
}

//функция находит k-ую порядковую статистику в массиве arr размером size
int find_order_stat_k(int* arr, int size, int k) {
	int left = 0;
	int right = size - 1;
	while (true) {
		int m = partition(arr, left, right);
		if (m == k) {
			//если после очередного применения partition вернула значение = k,
			//то всё, мы нашли нужный элемент, вернём его
			return arr[m];
		}
		//иначе сдвинем границы для следующего вызова partition
		if (m > k) {
			right = m;
		}
		else {
			left = m + 1;
		}
	}
}

int main() {
	int n=0, k=0;
	std::cin >> n >> k;
	int* arr = new int[n];
	for (int i = 0; i < n; i++) {
		std::cin >> arr[i];
	}

	std::cout << find_order_stat_k(arr, n, k) << std::endl;

	delete[] arr;
	return 0;
}
