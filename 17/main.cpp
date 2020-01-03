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
#include <map>
#include <memory>
#include <vector>

struct Node {
    std::map<char, std::shared_ptr<Node>> go;
    std::shared_ptr<Node> parent;
    std::shared_ptr<Node> suffix_link;
    std::vector<int> offsets;
    char symbol;
    bool is_terminal = false;

    explicit Node(const std::shared_ptr<Node> &parent, const char symbol) : parent(parent), symbol(symbol) {}

    explicit Node() : parent(nullptr), symbol('\0') {}

    std::shared_ptr<Node> FindSuffixLink();
};

std::shared_ptr<Node> Node::FindSuffixLink() {
    if (suffix_link != nullptr) {
        return suffix_link;
    }
    std::shared_ptr<Node> u = parent;
    char c = symbol;
    decltype(u->go.find(c)) v;
    do {
        u = u->FindSuffixLink();
        v = u->go.find(c);
    } while ((u->parent != nullptr) && (v == u->go.end()));
    if ((u->suffix_link == u) && (parent == u)) {
        suffix_link = u;
    } else if (v != u->go.end()) {
        suffix_link = v->second;
    } else {
        suffix_link = u;
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

    ~Trie() = default;

    bool Has(const std::string &key) const;

    bool Add(const std::string &key, const int offset);

    bool Remove(const std::string &key);

    void Print() const;

    void AddAll(std::string &t);

    void FindAll(const std::string &s) const;

private:
    int count = 0;
    std::string templ;
    std::shared_ptr<Node> root;

    static void print(const std::shared_ptr<Node> &node, const std::string &current);

    // Возвращает пару bool: первый - о наличии строки
    // второй - о единственности узла и необходимости его удалить.
    static std::pair<bool, bool> remove(
            std::shared_ptr<Node> &node, const std::string &key, int current_index);
};

Trie::Trie() {
    root = std::make_shared<Node>();
    root->suffix_link = root;
}

void Trie::FindAll(const std::string &s) const {
    std::vector<int> counts(s.length(), 0);
    std::shared_ptr<Node> current = root;
    for (int i = 0; i < s.length(); i++) {
        char symbol = s[i];
        auto next = current->go.find(symbol);
        while ((next == current->go.end()) && (current != root)) {
            current = current->FindSuffixLink();
            next = current->go.find(symbol);
        }
        if (next != current->go.end()) {
            current = next->second;
        }
        //if (current->is_terminal) {
            auto x = current;
            while (x!=x->FindSuffixLink()) {
                if (x->is_terminal) {
                    for (const int offset : x->offsets) {
                        if ((i - offset + 1 >= 0) && (i - offset + 1 < counts.size())) {
                            counts[i - offset + 1]++;
                        }
                    }
                }
                x = x->FindSuffixLink();
            }
        //}
    }
    for (int i = 0; i < (int) s.length() - (int) templ.length() + 1; i++) {
        //std::cerr << counts[i] << ' ';
        if (counts[i] == count) {
            std::cout << i << ' ';
        }
    }
}

bool Trie::Has(const std::string &key) const {
    std::shared_ptr<Node> current = root;
    for (char symbol : key) {
        auto next = current->go.find(symbol);
        if (next == current->go.end()) return false;
        current = next->second;
    }
    return current->is_terminal;
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
    count++;
    // Если терминальная, значит, строка уже есть.
    if (current->is_terminal) {
        return false;
    }
    current->is_terminal = true;
    return true;
}

bool Trie::Remove(const std::string &key) {
    return remove(root, key, 0).first;
}

std::pair<bool, bool> Trie::remove(
        std::shared_ptr<Node> &node, const std::string &key, int current_index) {
    if (current_index == key.length()) {
        if (!node->is_terminal) return std::make_pair(false, false);
        node->is_terminal = false;
        return std::make_pair(true, node->go.empty());
    }

    auto next = node->go.find(key[current_index]);
    if (next == node->go.end()) {
        return std::make_pair(false, false);
    }

    auto result = remove(next->second, key, current_index + 1);

    if (!result.first) { // Не нашли
        return result;
    }
    if (!result.second) { // Не нужно удалить ссылку на дочерний
        return result;
    }
    node->go.erase(key[current_index]);
    return std::make_pair(true, !node->is_terminal && node->go.empty());
}

void Trie::Print() const {
    print(root, "");
}

void Trie::print(const std::shared_ptr<Node> &node, const std::string &current) {
    if (node->is_terminal) {
        std::cout << current << std::endl;
    }
    for (const auto go : node->go) {
        print(go.second, current + go.first);
    }
}

void Trie::AddAll(std::string &t) {
    templ = std::move(t);
    int i = 0;
    while (i < templ.size()) {
        if (templ[i] != '?') {
            std::string word;
            while ((i < templ.size()) && (templ[i] != '?')) {
                word += templ[i];
                i++;
            }
            int offset = i;
            Add(word, offset);
        }
        i++;
    }
}

int main() {
    std::string t;
    std::string s;

    std::cin >> t >> s;

    if (s.length() >= t.length()) {
        Trie trie;
        trie.AddAll(t);
        trie.FindAll(s);
    }

    return 0;
}
