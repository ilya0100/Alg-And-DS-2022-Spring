/*
    4_1. Солдаты. 
    В одной военной части решили построить в одну шеренгу по росту. Т.к. часть была далеко не образцовая,
    то солдаты часто приходили не вовремя, а то их и вовсе приходилось выгонять из шеренги за плохо
    начищенные сапоги. Однако солдаты в процессе прихода и ухода должны были всегда быть выстроены по
    росту – сначала самые высокие, а в конце – самые низкие. За расстановку солдат отвечал прапорщик,
    который заметил интересную особенность – все солдаты в части разного роста. Ваша задача состоит в том,
    чтобы помочь прапорщику правильно расставлять солдат, а именно для каждого приходящего солдата
    указывать, перед каким солдатом в строе он должен становится.
    Требуемая скорость выполнения команды - O(log n).
*/

#include <iostream>
#include <cassert>
#include <sstream>
#include <stack>
#include <functional>
#include <algorithm>


template <typename T>
class IsLessDefault {
 public:
    bool operator()(const T& l, const T& r) const { return l < r; }
};


template <typename T, typename Compare = IsLessDefault<T>>
class AVLTree {
 public:
    AVLTree(): _root(nullptr), _cmp(IsLessDefault<T>()) {}
    AVLTree(const Compare& cmp): _root(nullptr), _cmp(cmp) {}

    AVLTree(const AVLTree& other) = delete;
    AVLTree& operator=(const AVLTree& other) = delete;

    ~AVLTree();

    void add(const T& key);
    void del(const T& key);

    int find_pos(const T& key);

 private:
    struct TreeNode {
        T key;
        unsigned char height;
        int nodes_count;
        TreeNode *left;
        TreeNode *right;

        TreeNode(): key(T()), height(1), nodes_count(1), left(nullptr), right(nullptr) {}
        TreeNode(const T& value): key(value), height(1), nodes_count(1), left(nullptr), right(nullptr) {}
    };

    TreeNode *_root;
    Compare _cmp;

    std::stack<TreeNode*> traverse();

    TreeNode *insert_node(TreeNode *node, const T& key);
    TreeNode *delete_node(TreeNode *node, const T& key);
    TreeNode *delete_and_find_min(TreeNode *node, TreeNode *&min);

    void delete_tree(TreeNode *&node);

    static unsigned char height(TreeNode *node) { return node != nullptr ? node->height : 0; }
    static unsigned char nodes_count(TreeNode *node) { return node != nullptr ? node->nodes_count : 0; }
    static char b_factor(TreeNode* node) { return height(node->right) - height(node->left); }

    static void fix_height(TreeNode* node);
    static void fix_nodes_count(TreeNode *node);

    TreeNode *rotate_right(TreeNode *root);
    TreeNode *rotate_left(TreeNode *root);

    TreeNode *balance(TreeNode *node);
};



void run(std::istream& input, std::ostream& output) {
    AVLTree<int> tree;
	int number;
    input >> number;

	for (int i = 0; i < number; ++i) {
        int elem, operation;
        input >> operation >> elem;
        switch (operation) {
        case 1:
            output << tree.find_pos(elem) << " ";
            break;

        case 2:
            tree.del(elem);
            break;

        default:
            break;
        }
    }
}

void run_test() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "5 1 100 1 200 1 50 2 1 1 150";
        run(input, output);

        std::cout << "***** Test 1: *****" << std::endl;
        std::cout << output.str() << std::endl;

        assert(output.str() == "0 0 2 1 ");

        std::cout << "***** Sucess *****\n" << std::endl;
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "7 1 150 1 130 1 170 1 100 1 140 1 90 1 120";
        run(input, output);

        std::cout << "***** Test 2: *****" << std::endl;
        std::cout << output.str() << std::endl;

        assert(output.str() == "0 1 0 3 2 5 4 ");

        std::cout << "***** Sucess *****\n" << std::endl;
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "10 1 1 1 2 1 3 1 4 1 5 1 6 1 7 1 8 1 9 1 10";
        run(input, output);

        std::cout << "***** Test 3: *****" << std::endl;
        std::cout << output.str() << std::endl;

        assert(output.str() == "0 0 0 0 0 0 0 0 0 0 ");

        std::cout << "***** Sucess *****\n" << std::endl;
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "10 1 10 1 9 1 8 1 7 1 6 1 5 1 4 1 3 1 2 1 1";
        run(input, output);

        std::cout << "***** Test 4: *****" << std::endl;
        std::cout << output.str() << std::endl;

        assert(output.str() == "0 1 2 3 4 5 6 7 8 9 ");

        std::cout << "***** Sucess *****\n" << std::endl;
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "7 1 150 1 130 1 170 1 100 1 140 2 140 1 145";
        run(input, output);

        std::cout << "***** Test 5: *****" << std::endl;
        std::cout << output.str() << std::endl;

        assert(output.str() == "0 1 0 3 2 2 ");

        std::cout << "***** Sucess *****\n" << std::endl;
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "8 1 150 1 130 1 170 1 100 1 140 2 150 1 110 1 150";
        run(input, output);

        std::cout << "***** Test 6: *****" << std::endl;
        std::cout << output.str() << std::endl;

        assert(output.str() == "0 1 0 3 2 3 1 ");

        std::cout << "***** Sucess *****\n" << std::endl;
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "7 1 150 1 170 1 120 1 110 1 140 1 145 1 130";
        run(input, output);

        std::cout << "***** Test 7: *****" << std::endl;
        std::cout << output.str() << std::endl;

        assert(output.str() == "0 0 2 3 2 2 4 ");

        std::cout << "***** Sucess *****\n" << std::endl;
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "7 1 130 1 110 1 160 1 170 1 150 1 155 1 140";
        run(input, output);

        std::cout << "***** Test 8: *****" << std::endl;
        std::cout << output.str() << std::endl;

        assert(output.str() == "0 1 0 0 2 2 4 ");

        std::cout << "***** Sucess *****\n" << std::endl;
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "6 1 150 1 90 1 140 1 130 1 120 1 110";
        run(input, output);

        std::cout << "***** Test 9: *****" << std::endl;
        std::cout << output.str() << std::endl;

        assert(output.str() == "0 1 1 2 3 4 ");

        std::cout << "***** Sucess *****\n" << std::endl;
    }
}


int main() {
    run(std::cin, std::cout);
    // run_test();
    return 0;
}


template <typename T, typename Compare>
AVLTree<T, Compare>::~AVLTree() {
    delete_tree(_root);
}

template <typename T, typename Compare>
void AVLTree<T, Compare>::add(const T& key) {
    _root = insert_node(_root, key);
}

template <typename T, typename Compare>
void AVLTree<T, Compare>::del(const T& key) {
    _root = delete_node(_root, key);
}


template <typename T, typename Compare>
void AVLTree<T, Compare>::delete_tree(TreeNode *&node) {
    if (node == nullptr) {
        return;
    }
    delete_tree(node->left);
    delete_tree(node->right);
    delete node;
    node = nullptr;
}

template <typename T, typename Compare>
void AVLTree<T, Compare>::fix_height(TreeNode *node) {
    unsigned char hl =  height(node->left);
    unsigned char hr =  height(node->right);
    node->height = std::max(hl, hr) + 1;
}

template <typename T, typename Compare>
void AVLTree<T, Compare>::fix_nodes_count(TreeNode *node) {
    node->nodes_count = nodes_count(node->left) + nodes_count(node->right) + 1;
}

template <typename T, typename Compare>
typename AVLTree<T, Compare>::TreeNode *AVLTree<T, Compare>::rotate_right(TreeNode *root) {
    TreeNode* new_root = root->left;
    root->left = new_root->right;
    new_root->right = root;

    fix_height(root);
    fix_height(new_root);

    fix_nodes_count(root);
    fix_nodes_count(new_root);
    return new_root;
}

template <typename T, typename Compare>
typename AVLTree<T, Compare>::TreeNode *AVLTree<T, Compare>::rotate_left(TreeNode *root) {
    TreeNode* new_root = root->right;
    root->right = new_root->left;
    new_root->left = root;

    fix_height(root);
    fix_height(new_root);

    fix_nodes_count(root);
    fix_nodes_count(new_root);
    return new_root;
}

template <typename T, typename Compare>
typename AVLTree<T, Compare>::TreeNode *AVLTree<T, Compare>::balance(TreeNode *node) {
    fix_height(node);
    fix_nodes_count(node);
    if (b_factor(node) == 2) {
        if (b_factor(node->right) < 0) {
            node->right = rotate_right(node->right);
        }
        return rotate_left(node);
    }
    if (b_factor(node) == -2) {
        if (b_factor(node->left) > 0) {
            node->left = rotate_left(node->left);
        }
        return rotate_right(node);
    }
    return node;
}

template <typename T, typename Compare>
typename AVLTree<T, Compare>::TreeNode *AVLTree<T, Compare>::insert_node(TreeNode *node, const T& key) {
    if (node == nullptr) {
        return new TreeNode(key);
    }
    if (_cmp(key, node->key)) {
        node->left = insert_node(node->left, key);
    } else {
        node->right = insert_node(node->right, key);
    }
    return balance(node);
}

template <typename T, typename Compare>
typename AVLTree<T, Compare>::TreeNode *AVLTree<T, Compare>::delete_node(TreeNode *node, const T& key) {
    if (node == nullptr) {
        return nullptr;
    }
	if (_cmp(key, node->key)) {
		node->left = delete_node(node->left, key);
    } else if (_cmp(node->key, key)) {
		node->right = delete_node(node->right, key);
    } else {
		TreeNode *left = node->left;
		TreeNode *right = node->right;
		delete node;

		if(right == nullptr) {
            return left;
        }

        TreeNode *min;
        TreeNode *temp = delete_and_find_min(right, min);
        min->right = temp;
        min->left = left;
		return balance(min);
	}
	return balance(node);
}

template <typename T, typename Compare>
typename AVLTree<T, Compare>::TreeNode *AVLTree<T, Compare>::delete_and_find_min(TreeNode *node, TreeNode *&min) {
    if (node->left == nullptr) {
        min = node;
        return node->right;
    }
    node->left = delete_and_find_min(node->left, min);
    return balance(node);
}

template <typename T, typename Compare>
int AVLTree<T, Compare>::find_pos(const T& key) {
    add(key);
    int pos = _root->nodes_count;

    TreeNode *temp = _root;
    while (temp != nullptr) {
        if (_cmp(key, temp->key)) {
            temp = temp->left;
        } else {
            if (temp->left != nullptr) {
                pos -= temp->left->nodes_count;
            }
            --pos;
            temp = temp->right;
        }
    }
    return pos;
}
