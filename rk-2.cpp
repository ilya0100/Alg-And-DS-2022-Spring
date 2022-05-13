#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <sstream>


class BinaryTree {
 public:
    BinaryTree(): root(nullptr) {}
    BinaryTree(const BinaryTree& other) = delete;

    BinaryTree& operator=(const BinaryTree& other) = delete;

    ~BinaryTree();

    void add(int x);

    bool is_all_nodes_equal() const;

    // void print() {
    //     print_node(root);
    // }

    // void mirror() {
    //     mirror_internal(root);
    // }

    // int max_depth() {
    //     return depth(root);
    // }

 private:
    struct TreeNode {
        int value;
        TreeNode *left;
        TreeNode *right;

        TreeNode(): value(0), left(nullptr), right(nullptr) {}
        TreeNode(int val): value(val), left(nullptr), right(nullptr) {}
        // TreeNode(int val, TreeNode *left, TreeNode *right): value(0), left(left), right(right) {}
    };

    TreeNode *root;

    void insert_node(int x, TreeNode *&node);

    // void print_node(TreeNode *node) {
    //     if (node != nullptr) {
    //         print_node(node->left);
    //         std::cout << node->value << " ";
    //         print_node(node->right);
    //     }
    // }

    void destroy_node(TreeNode *& node);

    // void mirror_internal(TreeNode *& node) {
    //     if (node == nullptr) {
    //         return;
    //     }
    //     TreeNode *temp = node->left;
    //     node->left = node->right;
    //     node->right = temp;
    //     if (node->left != nullptr) {
    //         mirror_internal(node->left);
    //     }
    //     if (node->right != nullptr) {
    //         mirror_internal(node->right);
    //     }
    //     return;
    // }

    // int depth(TreeNode * node) {
    //     if (node == nullptr) { return 0; }
    //     int left = depth(node->left);
    //     int right = depth(node->right);
    //     return 1 + (left > right ? left : right);
    // }
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
}


int main() {
    // run(std::cin, std::cout);
    run_test();
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