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
#include <vector>
#include <string>
#include <algorithm>

std::vector<int> z_function(const std::string &s) {
    int n = s.length();
    std::vector<int> z(n, 0);
    int l = 0;
    int r = 0;
    for (int i = 1; i < n; i++) {
        z[i] = std::max(0, std::min(z[i - l], r - i + 1));
        while ((i + z[i] < n) && (s[z[i]] == s[i + z[i]])) {
            z[i]++;
        }
        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }
    return z;
}

int main() {
    std::string t, s;
    std::cin >> t >> s;

    std::vector<int> z = z_function(t + '#' + s);

    int p = t.length();
    int n = s.length();
    for (int i = p + 1; i < n + p + 1; i++) {
        if (z[i] == p) {
            std::cout << (i - p - 1) << ' ';
        }
    }

    return 0;
}

