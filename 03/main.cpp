/*
3_1. Первые k элементов длинной последовательности.
Дана очень длинная последовательность целых чисел длины n. 
Требуется вывести в отсортированном виде её первые k элементов. 
Последовательность может не помещаться в память. 
Время работы O(n * log(k)). Память O(k). Использовать слияние.

In
9 4
3 7 4 5 6 1 15 4 2

Out
1 2 3 4
*/

#include <iostream>
#include <cstring>
#include <algorithm>

//функция слияния, вызываемая из MergeSort
// dst - результирующий массив
// arr1 - первый кусок
// arr2 - второй кусок
// в самой MergeSort в dst передаём весь участок, где последовательно лежат данные и arr1 и arr2, кроме того arr1 отдельно отливаем в буфер
// по факту в памяти это должно выглядеть вот так:
//                                       arr2
//                                        |
//                                        V 
// dst->{arr1[0],arr1[1],...,arr1[size1-1],arr2[0],arr2[1],...,arr2[size2-1]}
//
// arr1->{arr1[0],arr1[1],...,arr1[size1-1]}
void merge(int* dst, int* arr1, int* arr2, int size1, int size2) {
	int p = 0;
	int p1 = 0;
	int p2 = 0;
	
	while ((p1 < size1) && (arr1[p1] <= arr2[0])) {
		//вдруг нам повезло, и в начале первого массива лежат элементы,
		//меньшие или равные нулевому элементу из второго массива
		//мы тогда данные не копируем, а только двигаем индексы
		p++;
		p1++;
	}
	//пока не дошли до конца первого или второго массива
	while ((p1 < size1) && (p2 < size2)) {
		if (arr1[p1] <= arr2[p2]) {
			dst[p++] = arr1[p1++];
		}
		else {
			dst[p++] = arr2[p2++];
		}
	}
	//если остались элементы в первом куске - копируем их при помощи memcpy
	if (p1 < size1) {
		std::memcpy(&dst[p], &arr1[p1], (size1 - p1)*sizeof(int));
	}
	//если остались элементы во втором куске - их копировать не надо, они уже будут на своих местах
}

//восходящая сортировка слиянием (нерекурсивная)
void MergeSort(int* arr, int size) {
	int* buf = new int[size];

	for (int i = 1; i < size; i *= 2) {
		for (int i1 = 0; i1 <= size - i; i1 += 2 * i) {
			//скопируем первый кусок во временный буфер
			std::memcpy(buf, &arr[i1], i*sizeof(int));
			//сливать будем в исходный массив на место скопированного первого куска + второго куска
			//если нам повезёт, начало первого массива и/или хвост второго в merge копировать не придётся
			//больше всего повезёт на уже отсортированном массиве, тогда в merge ничего не будет копироваться.
			merge(&arr[i1], buf, &arr[i1] + i, i, std::min(i, size - (i1 + i)));
		}
	}
	delete[] buf;
}

//функция слияния первых k элементов из двух массивов
//res - результат размером k, arr1 может быть короче чем k, arr2 всегда будет размером k
void merge_k(int* res, int* arr1, int* arr2, int size1, int k) {
	int p = 0;
	int p1 = 0;
	int p2 = 0;

	//пока не дошли до конца первого или второго массива и не набрали k элементов в результирующий массив
	while ((p1 < size1) && (p2 < k) && (p < k)) {
		if (arr1[p1] < arr2[p2]) {
			res[p++] = arr1[p1++];
		}
		else {
			res[p++] = arr2[p2++];
		}
	}
	while (p < k) {
		//сюда мы можем попасть только в случае когда размер arr1 меньше k и arr1 уже закончился
		//поэтому копируем элементы из arr2
		res[p++] = arr2[p2++];
	}
}

int main() {
	int n, k;
	std::cin >> n >> k;

	//в этот массив читаем входные данные
	//его же будем сортировать MergeSort-ом
	int* arr1 = new int[k];
	//здесь будет храниться результат от предыдущего слияния k элементов 
	int* arr2 = new int[k];
	//сюда будет возвращаться результат слияния первых k элементов из arr1 и arr2 
	int* res = new int[k];

	int j = 0;
	for (int i = 0; i < n; i++) {
		//читаем элементы в массив arr1
		std::cin >> arr1[j++];		
		//если прочитали очередные k элементов или все n (i при этом будет равно n -1) 
		if ((j == k) || (i == n - 1)) {
			//выполняем сортировку слиянием прочитанного куска
			MergeSort(arr1, j);
			if (i != j - 1) {
				//это не первый кусок - делаем merge первых k элементов из arr1 и arr2
				merge_k(res, arr1, arr2, j, k);
				//меняем местами указатели res и arr2
				std::swap(res, arr2);
			}
			else {
				//это первый кусок, мержить пока ничего не надо
				//просто меняем местами указатели arr1 и arr2
				std::swap(arr1, arr2);
			}
			//обнуляем счётчик для следующего куска из k элементов			
			j = 0;
		}		
	}

	//выводим результат из arr2
	//цикл до std::min(n,k), а то вдруг n<k
	for (int i = 0; i < std::min(n,k); i++) {
		std::cout << arr2[i] <<" ";
	}

	delete[] res;
	delete[] arr2;
	delete[] arr1;

	return 0;
}
