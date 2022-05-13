#include <iostream>
#include <cassert>
#include <sstream>


class BinaryTree {
 public:
    BinaryTree(): root(nullptr) {}
    BinaryTree(const BinaryTree& other) = delete;

    BinaryTree& operator=(const BinaryTree& other) = delete;

    ~BinaryTree();

    void add(int x);

    bool is_all_nodes_equal() const;

 private:
    struct TreeNode {
        int value;
        TreeNode *left;
        TreeNode *right;

        TreeNode(): value(0), left(nullptr), right(nullptr) {}
        TreeNode(int val): value(val), left(nullptr), right(nullptr) {}
    };

    TreeNode *root;

    void insert_node(int x, TreeNode *&node);
    void destroy_node(TreeNode *& node);
};


void run(std::istream& input, std::ostream& output) {
    BinaryTree tree;
	int number;

	while(input >> number) {
		tree.add(number);
	}
    output << (tree.is_all_nodes_equal() ? 1 : 0);
}

void run_test() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "2 3 1";
        run(input, output);

        std::cout << "***** Test 1: *****" << std::endl;
        std::cout << output.str() << std::endl;

        assert(output.str() == "0");

        std::cout << "***** Sucess *****\n" << std::endl;
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "1 1 1";
        run(input, output);

        std::cout << "***** Test 2: *****" << std::endl;
        std::cout << output.str() << std::endl;

        assert(output.str() == "1");

        std::cout << "***** Sucess *****\n" << std::endl;
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "1";
        run(input, output);

        std::cout << "***** Test 3: *****" << std::endl;
        std::cout << output.str() << std::endl;

        assert(output.str() == "1");

        std::cout << "***** Sucess *****\n" << std::endl;
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "4 3 3 3";
        run(input, output);

        std::cout << "***** Test 4: *****" << std::endl;
        std::cout << output.str() << std::endl;

        assert(output.str() == "0");

        std::cout << "***** Sucess *****\n" << std::endl;
    }
}


int main() {
    run(std::cin, std::cout);
    // run_test();
    return 0;
}


BinaryTree::~BinaryTree() {
    destroy_node(root);
}

void BinaryTree::add(const int x) {
    insert_node(x, root);
}

bool BinaryTree::is_all_nodes_equal() const {
    assert(root != nullptr);

    if (root->left != nullptr) {
        return false;
    }

    TreeNode* curr = root->right;
    TreeNode* prev = root;

    while (curr != nullptr) {
        if (curr->value != prev->value) {
            return false;
        }
        prev = curr;
        curr = curr->right;
    }
    return true;
}

void BinaryTree::insert_node(int x, TreeNode *&node) {
    if (node == nullptr) {
        node = new TreeNode(x);
        return;
    }
    if (x < node->value) {
        insert_node(x, node->left);
    } else if (x > node->value) {
        insert_node(x, node->right);
    }
}

void BinaryTree::destroy_node(TreeNode *& node) {
    if (node != nullptr) {
        destroy_node(node->left);
        destroy_node(node->right);
        delete node;
    }
}
