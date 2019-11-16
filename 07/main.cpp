/*
7_1. Солдаты ДД
Ограничение времени	1 секунда
Ограничение памяти	500Mb
Ввод	стандартный ввод или input.txt
Вывод	стандартный вывод или output.txt
В одной военной части решили построить в одну шеренгу по росту. Т.к. часть была далеко не образцовая, 
то солдаты часто приходили не вовремя, а то их и вовсе приходилось выгонять из шеренги за плохо 
начищенные сапоги. Однако солдаты в процессе прихода и ухода должны были всегда быть выстроены 
по росту – сначала самые высокие, а в конце – самые низкие. За расстановку солдат отвечал прапорщик, 
который заметил интересную особенность – все солдаты в части разного роста.

Ваша задача состоит в том, чтобы помочь прапорщику правильно расставлять солдат, 
а именно для каждого приходящего солдата указывать, перед каким солдатом в строе
он должен становится. Требуемая скорость выполнения команды - O(log n) в среднем.

В реализации используйте декартовы деревья.

Формат ввода
Первая строка содержит число N – количество команд (1 ≤ N ≤ 90 000).

В каждой следующей строке содержится описание команды: 
число 1 и X если солдат приходит в строй (X – рост солдата, натуральное число до 100 000 включительно) 
и число 2 и Y если солдата, стоящим в строе на месте Y надо удалить из строя. Солдаты в строе нумеруются с нуля.

Формат вывода
На каждую команду 1 (добавление в строй) вы должны выводить число K – номер позиции,
на которую должен встать этот солдат (все стоящие за ним двигаются назад).

Пример
Ввод	
5
1 100
1 200
1 50
2 1
1 150
Вывод
0
0
2
1
*/
#include <iostream>
#include <queue>
#include <utility>
#include <cassert>

struct TreeNode {
	int value;
	int priority;
	int count;
	TreeNode* left = nullptr;
	TreeNode* right = nullptr;
	TreeNode(const int value, const int priority, TreeNode* left = nullptr, TreeNode* right = nullptr)
		: value(value), priority(priority), left(left), right(right) {
		refresh_count();
	}
	void refresh_count() {
		int c = 1;
		if (left != nullptr) c += left->count;
		if (right != nullptr) c += right->count;
		count = c;
	}
};

class Treap {
public:
	explicit Treap() {}
	~Treap();

	int Insert(const int value);
	void Delete(const int index);

	void Print() { 
		std::cout << std::endl;
		BFS([](TreeNode* n) {std::cout <<n <<" "<< n->value << " " << n->priority << " " << n->count <<" " 
			                           << n->left<< " " << n->right << std::endl; });
		std::cout << std::endl;
	};
private:
	TreeNode* Merge(TreeNode* L, TreeNode* R);

	std::pair<TreeNode*, TreeNode*> Split(TreeNode* root, const int v);

	TreeNode* FindByIndex(TreeNode* root, const int index) const;
	TreeNode* FindByValue(TreeNode* root, const int v) const;

	template <typename Callback>
	void BFS(Callback f);

	TreeNode* root=nullptr;
};

TreeNode* Treap::Merge(TreeNode* L, TreeNode* R) {
	if (L == nullptr) return R;
	if (R == nullptr) return L;

	if (L->priority > R->priority)
	{

		//TreeNode* node = new TreeNode(L->value, L->priority, L->left, Merge(L->right, R));
		//delete L;
		L->right = Merge(L->right, R);
		L->refresh_count();
		return L;
	}
	else
	{
		//TreeNode* node = new TreeNode(R->value, R->priority, Merge(L, R->left), R->right);
		//delete R;
		R->left = Merge(L, R->left);
		R->refresh_count();
		return R;
	}
}

std::pair<TreeNode*, TreeNode*> Treap::Split(TreeNode* root, const int v) {
	std::pair<TreeNode*, TreeNode*> p;
	TreeNode* L = nullptr;
	TreeNode* R = nullptr;
	if (root != nullptr) {
		if (root->value >= v)
		{
			if (root->right != nullptr) {
				p = Split(root->right, v);
				R = p.second;
			}
			//L = new TreeNode(root->value, root->priority, root->left, p.first);
			root->right = p.first;
			root->refresh_count();
			L = root;
		}
		else
		{
			if (root->left != nullptr) {
				p = Split(root->left, v);
				L = p.first;
			}
			root ->left = p.second;
			root->refresh_count();
			R = root;
		}
	}
	return std::pair<TreeNode*, TreeNode*>(L, R);
}

template <typename Callback>
void Treap::BFS(Callback f) {
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

Treap::~Treap() {
	BFS([](TreeNode* node) {delete node; });
}

int Treap::Insert(const int value) {
	int priority = rand();

	std::pair<TreeNode*, TreeNode*> p=Split(root, value);

	int res =( p.first==nullptr?0:p.first->count);
	
	TreeNode* n = new TreeNode(value, priority);
	root=Merge( Merge(p.first, n), p.second);

	return res;
}

void Treap::Delete(const int index) {
	//std::cout << "Delete begin " << index<< std::endl;
	TreeNode* n = FindByIndex(root, index);
	//std::cout << n<< std::endl;
	assert(n != nullptr);
	int v = n->value;

	//std::cout << "delete value:" << v <<std::endl;

	std::pair<TreeNode*, TreeNode*> p1;
	std::pair<TreeNode*, TreeNode*> p2;
	p1=Split(root, v+1);
	//std::cout << p1.first << " " << p1.second << std::endl;
	p2=Split(p1.second, v);
	//std::cout << p2.first << " " << p2.second << std::endl;
	delete p2.first;
	root=Merge(p1.first, p2.second);
	//std::cout << "Delete end" << std::endl;
}

TreeNode* Treap::FindByIndex(TreeNode* root, const int index) const {
	assert(root != nullptr);	

	if (root->left!=nullptr) {
	    if (root->left->count==index) {
		return root;
	    }
	    if (root->left->count>index) {
		return FindByIndex(root->left, index);
	    }
	}

	if (root->right != nullptr) {
		return FindByIndex(root->right, index - 1 - (root->left!=nullptr? root->left->count :0));
	}		

	return root;
}

TreeNode* Treap::FindByValue(TreeNode* root, const int v) const {
	assert(root != nullptr);

	if (root->value == v) {
		return root;
	}
	if (root->value <= v) {
		return FindByValue(root->left, v);
	}
	else {
		return FindByValue(root->right, v);
	}
}

int main() {
	srand(1);
	Treap t;
	int N = 0;
	std::cin >> N;
	for (int i = 0; i < N; i++) {
		int command = 0;
		std::cin >> command;
		int X = 0;
		int Y = 0;
		switch (command) {
		case 1:			
			std::cin >> X;
			std::cout << t.Insert(X)<<std::endl;
			break;
		case 2:			
			std::cin >> Y;
			t.Delete(Y);
			break;
		}
		//t.Print();
	}
	return 0;
}
