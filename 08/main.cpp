/*
Задача 8. Хеш-таблица (10 баллов)
Реализуйте структуру данных типа “множество строк” на основе динамической
хеш-таблицы с открытой адресацией. Хранимые строки непустые и состоят из
строчных латинских букв.
Хеш-функция строки должна быть реализована с помощью вычисления значения
многочлена методом Горнера.
Начальный размер таблицы должен быть равным 8-ми. Перехеширование
выполняйте при добавлении элементов в случае, когда коэффициент
заполнения таблицы достигает 3/4.
Структура данных должна поддерживать операции добавления строки в
множество, удаления строки из множества и проверки принадлежности
данной строки множеству.

1_2. Для разрешения коллизий используйте двойное хеширование.
Формат входных данных
Каждая строка входных данных задает одну операцию над множеством. Запись
операции состоит из типа операции и следующей за ним через пробел строки,
над которой проводится операция.
Тип операции  – один из трех символов:
    +  означает добавление данной строки в множество;
    -  означает удаление  строки из множества;
    ?  означает проверку принадлежности данной строки множеству.
При добавлении элемента в множество НЕ ГАРАНТИРУЕТСЯ, что он отсутствует в
этом множестве. При удалении элемента из множества НЕ ГАРАНТИРУЕТСЯ, что он
 присутствует в этом множестве.
Формат выходных данных
Программа должна вывести для каждой операции одну из двух строк OK или FAIL.
Для операции '?': OK, если элемент присутствует в множестве. FAIL иначе.
Для операции '+': FAIL, если добавляемый элемент уже присутствует в множестве
и потому не может быть добавлен. OK иначе.
Для операции '-': OK, если элемент присутствовал в множестве и успешно удален.
FAIL иначе.

stdin
+ hello
+ bye
? bye
+ bye
- bye
? bye
? hello

stdout
OK
OK
OK
FAIL
OK
FAIL
OK

*/

#include <iostream>
#include <string>
#include <cassert>
#include <utility>

class HashTable {
public:
    explicit HashTable(const int capacity);

    ~HashTable();

    HashTable(const HashTable &) = delete;
    HashTable(const HashTable &&) = delete;
    HashTable &operator=(const HashTable &) = delete;
    HashTable &operator=(const HashTable &&) = delete;

    bool Has(const std::string& s) const;
    bool Add(const std::string& s);
    bool Remove(const std::string& s);
private:
    unsigned int CalcHash(const std::string& s) const {
        //вычисление хэша многочленом методом Горнера
        unsigned int hash = 0;
        for (const char& ch : s) {
            hash += (hash * 37 + ch) % capacity;
        }
        return hash;
    }

    unsigned int CalcHash2(const std::string& s) const {
        //в качестве второго хэша - (сумма первого и последнего символа) * 2 + 1 -- чтобы нечётное число получилось
        return ((s[0] + s[s.length() - 1]) * 2 + 1) % capacity;
    }

    struct HashTableNode {
        std::string value;
        unsigned int hash = 0;
        bool is_empty = false;  //Признак, что элемент совободен. Проставляется в true при удалении.

        HashTableNode(std::string  value, const unsigned int hash) : value(std::move(value)), hash(hash) {}
    };
    using PHashTableNode = HashTableNode*;

    size_t capacity = 0; //размер хэштаблицы
    size_t size = 0;     //количество элементов
    PHashTableNode* table = nullptr; //собственно хэштаблица - динамический массив указателей на HashTableNode
};

HashTable::HashTable(const int capacity) :
        capacity(capacity) {
    table = new PHashTableNode[capacity]{nullptr};
}

HashTable::~HashTable() {
    for (size_t i = 0; i < capacity;i++) {
        if (table[i] != nullptr) delete table[i];
    }
    delete[] table;
}

bool HashTable::Has(const std::string& s) const {
    assert(!s.empty());
    unsigned int hash = CalcHash(s);
    size_t pos = hash % capacity;
    unsigned int hash2 = CalcHash2(s);
    PHashTableNode first = table[pos];
    size_t c = 0;
    while (table[pos] != nullptr) {
        PHashTableNode p = table[pos];
        if ((p == first) && (c++ != 0)) {
            //У нас может случиться ситуация, когда в таблице все элементы не nullptr, но есть свободные элементы
            //с is_emppty==true. В этом случае выходим из цикла, когда снова попадём в первый рассмотренный элемент.
            break;
        }
        if (!p->is_empty) {
            if ((p->hash == hash) && (p->value == s)) {
                //Нашли значение
                return true;
            }
        }
        pos = (pos + hash2) % capacity;
    }
    //Не нашли
    return false;
}

bool HashTable::Add(const std::string& s) {
    assert(!s.empty());
    if (size * 4 == capacity * 3) {
        //Если size == 0.75 * capacity, увеличим таблицу в 2 раза и сделаем рехэширование
        PHashTableNode* oldTable = table;
        table = new PHashTableNode[capacity * 2]{ nullptr };
        capacity *= 2;
        size = 0;
        for (size_t i = 0; i < capacity / 2; i++) {
            if (oldTable[i] != nullptr) {
                if (!oldTable[i]->is_empty) {
                    Add(oldTable[i]->value);
                }
                delete oldTable[i];
            }
        }
        delete oldTable;
    }

    unsigned int hash = CalcHash(s);
    size_t pos = hash % capacity;
    unsigned int hash2 = CalcHash2(s);
    while (table[pos] != nullptr) {
        PHashTableNode p = table[pos];
        //При добавлении трюк с проверкой попадания в первый расмотренный элемент не нужен.
        //Если попадём на is_empty, запишем в него добавляемое значение и выйдем из цикла.
        if (p->is_empty) {
            p->is_empty = false;
            p->hash = hash;
            p->value = s;
            size++;
            return true;
        }
        else {
            if ((p->hash == hash) && (p->value == s)) {
                //Пришли в элемент, в котором уже лежит это значение - добавить не можем.
                return false;
            }
        }
        pos = (pos + hash2) % capacity;
    }
    //Создаём новый элемент в таблице
    table[pos] = new HashTableNode(s, hash);
    size++;
    return true;
}

bool HashTable::Remove(const std::string& s) {
    assert(!s.empty());
    unsigned int hash = CalcHash(s);
    size_t pos = hash % capacity;
    unsigned int hash2 = CalcHash2(s);
    int c = 0;
    PHashTableNode first = table[pos];
    while (table[pos] != nullptr) {
        PHashTableNode p = table[pos];
        if ((p == first) && (c++ != 0)) {
            //У нас может случиться ситуация, когда в таблице все элементы не nullptr, но есть свободные элементы
            //с is_emppty==true. В этом случае выходим из цикла, когда снова попадём в первый рассмотренный элемент.
            break;
        }
        if (!p->is_empty) {
            if ((p->hash == hash) && (p->value == s)) {
                //Нашли удаляемое значение, проставим is_empty=true.
                p->is_empty = true;
                size--;
                return true;
            }
        }
        pos = (pos + hash2) % capacity;
    }
    //Не нашли
    return false;
}

int main() {
    HashTable table(8);

    char command = ' ';
    std::string value;
    while (std::cin >> command >> value) {
        switch (command) {
            case '?':
                std::cout << (table.Has(value) ? "OK" : "FAIL") << std::endl;
                break;
            case '+':
                std::cout << (table.Add(value) ? "OK" : "FAIL") << std::endl;
                break;
            case '-':
                std::cout << (table.Remove(value) ? "OK" : "FAIL") << std::endl;
                break;
            default:
                break;
        }
    }

    return 0;
}
