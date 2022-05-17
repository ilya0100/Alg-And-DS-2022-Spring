/*
    Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N.
    Требуется построить бинарное дерево, заданное наивным порядком вставки.
    Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K,
    то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
    Требования: Рекурсия запрещена. Решение должно поддерживать передачу функции сравнения снаружи.

    2_3. Выведите элементы в порядке post-order (снизу вверх).
*/

#include <iostream>
#include <cassert>
#include <sstream>
#include <stack>
#include <functional>


template <typename T>
class IsLessDefault {
 public:
    bool operator()(const T& l, const T& r) const { return l < r; }
};


template <typename T>
class PrintToStream {
 public:
    PrintToStream(std::ostream& _output): output(_output) {}
    void operator()(const T& value) {
        output << value;
    }

 private:
    std::ostream& output;
};


template <typename T, typename Compare = IsLessDefault<T>>
class BinaryTree {
 public:
    BinaryTree(): root(nullptr), cmp(IsLessDefault<T>()) {}
    BinaryTree(const Compare& _cmp): root(nullptr), cmp(_cmp) {}

    BinaryTree(const BinaryTree& other) = delete;
    BinaryTree& operator=(const BinaryTree& other) = delete;

    ~BinaryTree();

    void add(T x);

    void traverse_dfs(std::function<void(const T&)> visit);

 private:
    struct TreeNode {
        T value;
        TreeNode *left;
        TreeNode *right;

        TreeNode(const T& _value): value(_value), left(nullptr), right(nullptr) {}
    };

    TreeNode *root;
    Compare cmp;

    std::stack<TreeNode*> traverse();
};


void run(std::istream& input, std::ostream& output) {
    BinaryTree<int> tree;
	int number;
    input >> number;

	for (int i = 0; i < number; ++i) {
        int elem;
        input >> elem;
        tree.add(elem);
    }
    tree.traverse_dfs([&](const int& key){output << key << " ";});
}

void run_test() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "10 7 2 10 8 5 3 6 4 1 9";
        run(input, output);

        std::cout << "***** Test 1: *****" << std::endl;
        std::cout << output.str() << std::endl;

        assert(output.str() == "1 4 3 6 5 2 9 8 10 7 ");

        std::cout << "***** Sucess *****\n" << std::endl;
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "10 7 10 2 6 1 9 4 5 8 3";
        run(input, output);

        std::cout << "***** Test 2: *****" << std::endl;
        std::cout << output.str() << std::endl;

        assert(output.str() == "1 3 5 4 6 2 8 9 10 7 ");

        std::cout << "***** Sucess *****\n" << std::endl;
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "10 4 3 9 5 6 8 7 2 10 1";
        run(input, output);

        std::cout << "***** Test 3: *****" << std::endl;
        std::cout << output.str() << std::endl;

        assert(output.str() == "1 2 3 7 8 6 5 10 9 4 ");

        std::cout << "***** Sucess *****\n" << std::endl;
    }
}


int main() {
    run(std::cin, std::cout);
    // run_test();
    return 0;
}


template <typename T, typename Compare>
BinaryTree<T, Compare>::~BinaryTree() {
    if (root == nullptr) {
        return;
    }
    std::stack<TreeNode*> stack = traverse();
    while (!stack.empty()) {
        TreeNode *temp = stack.top();
        stack.pop();
        delete temp;
    }
}

template <typename T, typename Compare>
void BinaryTree<T, Compare>::add(const T x) {
    if (root == nullptr) {
        root = new TreeNode(x);
        return;
    }

    TreeNode* temp = root;
    while (temp != nullptr) {
        if (cmp(x, temp->value)) {
            if (temp->left == nullptr) {
                temp->left = new TreeNode(x);
                return;
            }
            temp = temp->left;
        } else {
            if (temp->right == nullptr) {
                temp->right = new TreeNode(x);
                return;
            }
            temp = temp->right;
        }
    }
}

template <typename T, typename Compare>
void BinaryTree<T, Compare>::traverse_dfs(std::function<void(const T&)> visit) {
    if (root == nullptr) {
        return;
    }

    std::stack<TreeNode*> stack = traverse();
    while (!stack.empty()) {
        TreeNode *node = stack.top();
        stack.pop();
        visit(node->value);
    }
}

template <typename T, typename Compare>
std::stack<typename BinaryTree<T, Compare>::TreeNode*> BinaryTree<T, Compare>::traverse() {
    std::stack<TreeNode*> reverse, direct;
    reverse.push(root);

    while (!reverse.empty()) {
        TreeNode *node = reverse.top();
        reverse.pop();
        direct.push(node);
        if (node->left != nullptr) {
            reverse.push(node->left);
        }
        if (node->right != nullptr) {
            reverse.push(node->right);
        }
    }
    return direct;
}
