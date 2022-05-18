#include "Huffman.h"

#include <bitset>
#include <vector>
#include <algorithm>
#include <queue>



class HaffmanCode {
 public:
	HaffmanCode(): _root(nullptr) {}

	HaffmanCode(const HaffmanCode& other) = delete;
	HaffmanCode& operator=(const HaffmanCode& other) = delete;

	~HaffmanCode();

	void build_tree(const std::vector<size_t>& bytes_freq);
	std::vector<std::vector<unsigned char>> get_table();

	std::vector<unsigned char> serialize();
	void deserialize(const std::vector<unsigned char>& stream, unsigned char bits_count);

 private:
	struct Node {
		Node* left;
		Node* right;
		size_t p;
		unsigned char byte;

		Node(): left(nullptr), right(nullptr), p(0), byte(0) {}
	};

	struct IsMoreNode {
		bool operator()(const Node* l, const Node* r) {
			return l->p > r->p;
		}
	};

	Node* _root;
	std::vector<std::vector<unsigned char>> table;

	void delete_tree(Node* node);
	void traverse(Node* node, std::vector<unsigned char>& code);
};



void Encode(CInputStream& original, COutputStream& compressed) {
	std::vector<size_t> frequencies(256);
	std::vector<unsigned char> stream;

	unsigned char byte;
	while (original.Read(byte)) {
		++frequencies[byte];
		stream.push_back(byte);
	}
	HaffmanCode codes;
	codes.build_tree(frequencies);
}

void Decode(CInputStream& compressed, COutputStream& original) {

}



void HaffmanCode::build_tree(const std::vector<size_t>& bytes_freq) {
	std::priority_queue<Node*, std::vector<Node*>, IsMoreNode> tree_builder;

	for (size_t i = 0; i < bytes_freq.size(); ++i) {
		if (bytes_freq[i] > 0) {
			Node* node = new Node;
			node->byte = i;
			node->p = bytes_freq[i];

			tree_builder.push(node);
		}
	}

	while (tree_builder.size() > 1) {
		Node* first = tree_builder.top();
		tree_builder.pop();

		Node* second = tree_builder.top();
		tree_builder.pop();

		Node* new_node = new Node;
		new_node->left = first;
		new_node->right = second;
		new_node->p = first->p + second->p;

		tree_builder.push(new_node);
	}
	_root =  tree_builder.top();
}

std::vector<std::vector<unsigned char>> HaffmanCode::get_table() {
	if (table.size() == 0) {
		table = std::vector<std::vector<unsigned char>>(256);
		std::vector<unsigned char> code;
		traverse(_root, code);
	}
	return table;
}

void HaffmanCode::traverse(Node* node, std::vector<unsigned char>& code) {
	if (node->left == nullptr && node->right == nullptr) {
		table[node->byte] = code;
		code.pop_back();
		return;
	}
	code.push_back(0);
	traverse(node->left, code);

	code.push_back(1);
	traverse(node->right, code);
	code.pop_back();
}

std::vector<unsigned char> HaffmanCode::serialize() {

}


HaffmanCode::~HaffmanCode() {
	delete_tree(_root);
}

void HaffmanCode::delete_tree(Node* node) {
	if (node == nullptr) {
		return;
	}
	delete_tree(node->left);
	delete_tree(node->right);
	delete node;
}


class OutBitStream {
	OutBitStream(): bitsCount(0) {}

	void WriteBit(unsigned char bit);
	void WriteByte(unsigned char byte);

	const std::vector<unsigned char>& GetBuffer() const { return buffer; }

private:
	std::vector<unsigned char> buffer;
	size_t bitsCount;
};

void OutBitStream::WriteBit(unsigned char bit)
{
	if(bitsCount % 8 == 0) {
		buffer.push_back(0);
	}

	if(bit != 0) {
		size_t bitPos = bitsCount % 8;
		buffer[bitsCount / 8] |= 1 << (7 - bitPos);
	}

	++bitsCount;
}

void OutBitStream::WriteByte(unsigned char byte)
{
	if(bitsCount % 8 == 0) {
		buffer.push_back(byte);
	} else {
		size_t offset = bitsCount % 8;
		buffer[bitsCount / 8] |= byte >> offset;
		buffer.push_back(byte << (8 - offset));
	}

	bitsCount += 8;
}




int main() {
	{
		std::cout << "***** Tree and table test *****" << std::endl;
		std::vector<size_t> frequencies(8);
		for (unsigned char i = 1; i < 6; ++i) {
			frequencies[i] = i;
		}
		HaffmanCode codes;
		codes.build_tree(frequencies);

		std::vector<std::vector<unsigned char>> table = codes.get_table();
		for (size_t i = 0; i < table.size(); ++i) {
			if (table[i].size() != 0) {
				std::cout << i << ": ";
				for (size_t j = 0; j < table[i].size(); ++j) {
					std::cout << static_cast<int>(table[i][j]) << " ";
				}
				std::cout << std::endl;
			}
		}
		std::cout << "**********" << std::endl;
	}
}