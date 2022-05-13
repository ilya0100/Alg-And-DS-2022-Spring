#include <iostream>
#include <cassert>
#include <sstream>
#include <unordered_set>


struct Node {
    std::string product_name;
    int color;
    int size;

    Node(): color(0), size(0) {}
};

template<>
struct std::hash<Node>{
    size_t operator()(const Node& node) const {
        size_t hash = 0;
        auto it = node.product_name.begin();
        for (; it != node.product_name.end(); ++it) {
            hash = hash * 137 + *it;
        }

        hash += (node.color % 137) ^ (node.size % 137);
        return hash;
    }
};

bool operator==(const Node& l, const Node& r) {
    if (l.product_name != r.product_name) {
        return false;
    }
    if (l.color != r.color) {
        return false;
    }
    if (l.size != r.size) {
        return false;
    }
    return true;
}


void run(std::istream& input, std::ostream& output) {
    std::unordered_set<Node> set;
	char operation;
	Node node;

	while(input >> operation >> node.product_name >> node.color >> node.size) {
		if(operation == '+') {
            if (set.find(node) == set.end()) {
                set.insert(node);
                output << "OK" << std::endl;
            } else {
                output << "FAIL" << std::endl;
            }
        }
		if(operation == '?') {
			if (set.find(node) != set.end()) {
                output << "OK" << std::endl;
            } else {
                output << "FAIL" << std::endl;
            }
        }
		if(operation == '-') {
            auto it = set.find(node);
			if (it != set.end()) {
                output << "OK" << std::endl;
                set.erase(it);
            } else {
                output << "FAIL" << std::endl;
            }
        }
	}
}

void run_test() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "+ a 1 2\n+ a 1 1\n+ a 2 1\n? a 1 1\n? b 2 1\n? a 1 3\n? b 1 1\n";
        run(input, output);
        std::cout << "***** Test 1: *****" << std::endl;
        std::cout << output.str() << std::endl;
        assert(output.str() == "OK\nOK\nOK\nOK\nFAIL\nFAIL\nFAIL\n");
        std::cout << "***** Sucess *****\n" << std::endl;
    }
}


int main() {
    run(std::cin, std::cout);
    // run_test();
    return 0;
}
