/*
6_4. Обход дерева в порядке level-order
Ограничение времени	0.4 секунды
Ограничение памяти	64Mb
Ввод	стандартный ввод или input.txt
Вывод	стандартный вывод или output.txt
Дано число N < 10^6 и последовательность целых чисел из [-2^31..2^31] длиной N.
Требуется построить бинарное дерево поиска, заданное наивным порядком вставки. 
Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, 
то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
Выведите элементы в порядке level-order (по слоям, “в ширину”).

Формат ввода
В первой строке записано число N.
Во последующих N строках — целые числа исходной последовательности.

Формат вывода
N целых чисел, разделенных пробелом.

Пример 1
Ввод	
10
10
5
4
7
9
8
6
3
2
1
Вывод
10 5 4 7 3 6 9 2 8 1
Пример 2
Ввод	
10
9
8
6
10
4
3
2
5
1
7
Вывод
9 8 10 6 4 7 3 5 2 1
Пример 3
Ввод	
10
1
7
10
9
4
3
8
5
2
6
Вывод
1 7 4 10 3 5 9 2 6 8
*/
#include <iostream>
#include <queue>

struct TreeNode {
	const int value;
	TreeNode* left = nullptr;
	TreeNode* right = nullptr;
	TreeNode(int value) : value(value) {}
};

class NaiveTree {
public:
	explicit NaiveTree() {};
	~NaiveTree();
	NaiveTree(const NaiveTree&) = delete;
	NaiveTree(const NaiveTree&&) = delete;
	NaiveTree& operator=(const NaiveTree&) = delete;
	NaiveTree& operator=(const NaiveTree&&) = delete;

	void Insert(const int value);
	template <typename Callback>
	void BFS(Callback f); 
private:
	TreeNode* root=nullptr;
};

template <typename Callback>
void NaiveTree::BFS(Callback f) {
	std::queue<TreeNode*> q;
	q.push(root);
	while (!(q.empty())) {
		TreeNode* node = q.front();
		q.pop();
		if (node != nullptr) {
			q.push(node->left);
			q.push(node->right);
			f(node);
		}
	}
}

NaiveTree::~NaiveTree() {		
	BFS([](TreeNode* node) {delete node; });
}

void NaiveTree::Insert(const int value) {
	TreeNode** node=&root;
	while (*node != nullptr) {
		if ((*node)->value <= value) {
			node = &(*node)->right;
		}
		else {
			node = &(*node)->left;
		}
	}
	*node = new TreeNode(value);
}

int main() {
	NaiveTree t;
	int N=0, K=0;
	std::cin >> N;
	for (int i = 0; i < N; i++) {
		std::cin >> K;
		t.Insert(K);
	}

	t.BFS([](TreeNode* node) {std::cout << node->value << " "; });

	return 0;
}
