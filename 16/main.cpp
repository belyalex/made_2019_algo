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
#include <deque>

std::deque<int> z_function(const std::string &s, int p) {
    int n = s.length();
    std::deque<int> z;
    z.push_back(0);
    int l = 0;
    int r = 0;
    for (int i = 1; i < n; i++) {
        z.push_back(std::max(0, std::min(z[i - l], r - i + 1)));
        while ((i + z.back() < n) && (s[z.back()] == s[i + z.back()])) {
            z.back()++;
        }
        if (i + z.back() - 1 > r) {
            l = i;
            r = i + z.back() - 1;
        }

        if (z.back() == p) {
            std::cout << i - p - 1 << " ";
        }
        if (i>p) {
            //В деке будет не больше p + 1 элементов.
            //Ограничение по памяти O(p) соблюдено.
            z.pop_back();
        }
    }
    return z;
}

int main() {
    std::string t, s;
    std::cin >> t >> s;

    int p = t.length();

    s=t+"#"+s;

    z_function(s, p);

    return 0;
}
