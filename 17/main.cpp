/*
Задача 17. Шаблон с ? (5 баллов)
Шаблон поиска задан строкой длины m, в которой кроме обычных символов могут встречаться символы “?”. Найти позиции
всех вхождений шаблона в тексте длины n. Каждое вхождение шаблона предполагает, что все обычные символы совпадают
с соответствующими из текста, а вместо символа “?” в тексте встречается произвольный символ. Гарантируется, что
сам “?” в тексте не встречается.
Время работы - O(n + m + Z), где Z - общее число вхождений подстрок шаблона “между вопросиками” в исходном тексте.
 (Ахо-Корасик)

m ≤ 5000, n ≤ 2000000. Время: 10с, память 32Mb.

Формат входного файла
Шаблон, символ перевода строки, строка.
Формат выходного файла
Позиции вхождения шаблона в строке.
stdin
ab??aba
ababacaba

stdout
2
 */

#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

struct Node {
    std::unordered_map<char, std::shared_ptr<Node>> go;
    std::weak_ptr<Node> parent;
    std::weak_ptr<Node> suffix_link;
    std::vector<int> offsets;
    char symbol;
    bool is_terminal = false;

    explicit Node(const std::shared_ptr<Node> &parent, const char symbol) : parent(parent), symbol(symbol) {}

    Node() : symbol('\0') {}

    void Destroy() {
        //В процессе хождения по бору у нас могли образоваться циклические ссылки. Зачистим.
        for (auto &item : go) {
            if (item.second != nullptr) {
                auto tmp = item.second;
                item.second.reset();
                tmp->Destroy();
            }
        }
    }

    std::weak_ptr<Node> FindSuffixLink();

    std::shared_ptr<Node> Go(char ch);
};

std::shared_ptr<Node> Node::Go(char ch) {
    auto it = go.find(ch);
    if (it == go.end()) {
        if (parent.lock() == nullptr) {
            //попали в корень
            return go[ch] = suffix_link.lock();
        } else {
            return go[ch] = FindSuffixLink().lock()->Go(ch);
        }
    }
    return it->second;
}

std::weak_ptr<Node> Node::FindSuffixLink() {
    if (suffix_link.lock() != nullptr) {
        return suffix_link;
    }
    std::shared_ptr<Node> u = parent.lock();
    char c = symbol;
    decltype(u->Go(c)) v;
    do {
        u = u->FindSuffixLink().lock();
        v = u->Go(c);
    } while ((u->parent.lock() != nullptr) && (v == nullptr));
    if ((u->suffix_link.lock() == u) && (parent.lock() == u)) {
        suffix_link = u;
    } else {
        suffix_link = v;
    }
    return suffix_link;
}

class Trie {
public:
    Trie();

    Trie(const Trie &) = delete;

    Trie(Trie &&) = delete;

    Trie &operator=(const Trie &) = delete;

    Trie &operator=(Trie &&) = delete;

    ~Trie() { root->Destroy(); };

    bool Add(const std::string &key, int offset);

    std::shared_ptr<Node> GetRoot() const { return root; };
private:
    std::shared_ptr<Node> root;
};

Trie::Trie() {
    root = std::make_shared<Node>();
    root->suffix_link = root; //в корне суффиксная ссылка указывает тоже на корень.
}

bool Trie::Add(const std::string &key, const int offset) {
    std::shared_ptr<Node> current = root;
    for (char symbol : key) {
        auto next = current->go.find(symbol);
        if (next == current->go.end()) {
            current = current->go[symbol] = std::make_shared<Node>(current, symbol);
        } else {
            current = next->second;
        }
    }
    current->offsets.push_back(offset);
    // Если терминальная, значит, строка уже есть.
    if (current->is_terminal) {
        return false;
    }
    current->is_terminal = true;
    return true;
}

//Поиск шаблона templ в строке s. Шаблон может содержать знаки '?', означающие, что на этом месте может находиться
//любой символ
void FindAll(const std::string &templ, const std::string &s) {
    Trie trie;
    //Счётчик слов в шаблоне
    int words_count = 0;
    size_t i = 0;
    //Построение бора по отдельным словам из шаблона
    while (i < templ.size()) {
        if (templ[i] != '?') {
            std::string word;
            while ((i < templ.size()) && (templ[i] != '?')) {
                word += templ[i];
                i++;
            }
            int offset = i;
            trie.Add(word, offset);
            words_count++;
        }
        i++;
    }
    //Поиск по бору
    std::vector<int> counts(s.length(), 0); //для каждой позиции в строке храним кол-во найденных слов с учётом смещений
    std::shared_ptr<Node> current = trie.GetRoot();
    for (i = 0; i < s.length(); i++) {
        char symbol = s[i];
        current = current->Go(symbol);
        auto x = current;
        while (x != x->FindSuffixLink().lock()) {
            if (x->is_terminal) {
                for (const int offset : x->offsets) {
                    if ((i - offset + 1 >= 0) && (i - offset + 1 < counts.size())) {
                        counts[i - offset + 1]++;
                    }
                }
            }
            x = x->FindSuffixLink().lock();
        }
    }
    for (i = 0; i < (int) s.length() - (int) templ.length() + 1; i++) {        
        if (counts[i] == words_count) {
            //Если в какой-то позиции найдены все слова из шаблона,
            //значит нашли в этой позиции вхождение всего шаблона
            std::cout << i << ' ';
        }
    }
}

int main() {
    std::string t;
    std::string s;

    std::cin >> t >> s;

    if (s.length() >= t.length()) {
        FindAll(t, s);
    }

    return 0;
}
