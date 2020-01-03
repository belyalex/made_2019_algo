/*
 Задача 16. Поиск подстроки (5 баллов)
Найдите все вхождения шаблона в строку. Длина шаблона – p, длина строки ­– n. Время O(n + p), доп. память – O(p).
Вариант 2. С помощью z-функции.
Формат входного файла
Шаблон, символ перевода строки, строка.
Формат выходного файла
Позиции вхождения шаблона в строке.
Время: 100мс, память 3MB

stdin
aba
abacababa

stdout
0 4 6

 */

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

//Поиск всех вхождений шаблона t в строке s при помощи z-функции с доп. памятью O(p), где p - длина шаблона t
void FindAllWithZfunction(const std::string& t, const std::string &s) {
    int n = s.length();
    int p = t.length();
    std::vector<int> z(p,0);
    z.push_back(0);
    int l = 0;
    int r = 0;
    //Вычисляем значения z-функции для шаблона t
    for (int i = 1; i < p; i++) {
        z[i]=std::max(0, std::min(z[i - l], r - i + 1));
        while ((i + z[i] < n) && (t[z[i]] == t[i + z[i]])) {
            z[i]++;
        }
        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }
    int Z=0;
    l=0;
    r=0;
    //Проходим по строке, для каждой позиции в строке вычисляем значение z-функции, но не сохраняем его в вектор.
    for (int i=0; i<n; i++) {
        Z=std::max(0, std::min(z[i - l], r - i + 1));
        while ((i + Z < n) && (t[Z] == s[i + Z])) {
            Z++;
        }
        if (i + Z - 1 > r) {
            l = i;
            r = i + Z - 1;
        }
        if (Z == p) {
            //Нашли очередное вхождение - напечатаем его
            std::cout << i << " ";
        }
    }
}

int main() {
    std::string t, s;
    std::cin >> t >> s;

    FindAllWithZfunction(t, s);

    return 0;
}
