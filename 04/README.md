Даны неотрицательные целые числа n,k и массив целых чисел из [0..10^9] размера n. 
Требуется найти k-ю порядковую статистику. т.е. напечатать число, 
которое бы стояло на позиции с индексом k (0..n-1) в отсортированном массиве. 
Напишите нерекурсивный алгоритм.
Требования к дополнительной памяти: O(n). Требуемое среднее время работы: O(n).
Функцию Partition следует реализовывать методом прохода двумя итераторами в одном направлении. 
Описание для случая прохода от начала массива к концу:
Выбирается опорный элемент. Опорный элемент меняется с последним элементом массива.
Во время работы Partition в начале массива содержатся элементы, не бОльшие опорного. 
Затем располагаются элементы, строго бОльшие опорного. В конце массива лежат нерассмотренные элементы. 
Последним элементом лежит опорный.
Итератор (индекс) i указывает на начало группы элементов, строго бОльших опорного.
Итератор j больше i, итератор j указывает на первый нерассмотренный элемент.
Шаг алгоритма. Рассматривается элемент, на который указывает j. Если он больше опорного, то сдвигаем j.
Если он не больше опорного, то меняем a[i] и a[j] местами, сдвигаем i и сдвигаем j.
В конце работы алгоритма меняем опорный и элемент, на который указывает итератор i.


4_2. Реализуйте стратегию выбора опорного элемента “медиана трёх”. 
Функцию Partition реализуйте методом прохода двумя итераторами от конца массива к началу.

In
10 4
1 2 3 4 5 6 7 8 9 10
Out
5

In
10 0
3 6 5 7 2 9 8 10 4 1
Out
1

In
10 9
0 0 0 0 0 0 0 0 0 1
Out
1
