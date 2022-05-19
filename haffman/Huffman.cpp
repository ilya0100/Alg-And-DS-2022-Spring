#include "Huffman.h"

#include <bitset>
#include <vector>
#include <algorithm>
#include <queue>
#include <stack>
#include <ctime>



class OutBitStream {
 public:
	OutBitStream(): bits_count(0) {}

	void WriteBit(unsigned char bit);
	void WriteByte(unsigned char byte);

	const std::vector<unsigned char>& GetBuffer() const { return buffer; }

private:
	std::vector<unsigned char> buffer;
	size_t bits_count;
};

class InBitStream {
 public:
	InBitStream(std::vector<unsigned char>& _buffer): buffer(_buffer), bits_count(0) {}

	unsigned char ReadBit();
	unsigned char ReadByte();

	const std::vector<unsigned char>& GetBuffer() const { return buffer; }

private:
	std::vector<unsigned char> buffer;
	size_t bits_count;
};


class HaffmanCode {
 public:
	HaffmanCode(): _root(nullptr), char_count(0) {}

	HaffmanCode(const HaffmanCode& other) = delete;
	HaffmanCode& operator=(const HaffmanCode& other) = delete;

	~HaffmanCode();

	void build_tree(const std::vector<size_t>& bytes_freq);
	std::vector<std::vector<unsigned char>> get_table();

	std::vector<unsigned char> serialize();
	void deserialize(std::vector<unsigned char>& buffer);

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
	unsigned char char_count;

	void delete_tree(Node* node);
	void traverse(Node* node, std::vector<unsigned char>& code);

	void serialize_tree(Node* node, OutBitStream& stream);
	void deserialize_tree(Node* node, InBitStream& stream);
};


void Encode(CInputStream& original, COutputStream& compressed) {
	std::vector<size_t> frequencies(256);
	std::vector<unsigned char> stream;

	unsigned char byte;
	while (original.Read(byte)) {
		++frequencies[byte];
		stream.push_back(byte);
	}
	HaffmanCode code;
	code.build_tree(frequencies);
	std::vector<unsigned char> tree = code.serialize();
	for (size_t i = 0; i < tree.size(); ++i) {
		compressed.Write(tree[i]);
	}

	std::vector<std::vector<unsigned char>> table = code.get_table();
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
			++char_count;
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
	OutBitStream output;
	output.WriteByte(char_count);
	serialize_tree(_root, output);
	return output.GetBuffer();
}

void HaffmanCode::serialize_tree(Node* node, OutBitStream& stream) {
	if (node->left == nullptr && node->right == nullptr) {
		stream.WriteBit(1);
		stream.WriteByte(node->byte);
		return;
	}
	serialize_tree(node->left, stream);
	serialize_tree(node->right, stream);
	stream.WriteBit(0);
}

void HaffmanCode::deserialize(std::vector<unsigned char>& buffer) {
	InBitStream stream(buffer);
	deserialize_tree(_root, stream);
}

void HaffmanCode::deserialize_tree(Node* node, InBitStream& stream) {
	char_count = stream.ReadByte();
	std::stack<Node*> tree_builder;

	for (size_t read_count = 0; read_count < char_count;) {
		if (stream.ReadBit() == 1) {
			Node* new_node = new Node;
			new_node->byte = stream.ReadByte();
			tree_builder.push(new_node);
			++read_count;
		} else {
			Node* first = tree_builder.top();
			tree_builder.pop();

			Node* second = tree_builder.top();
			tree_builder.pop();

			Node* new_node = new Node;
			new_node->right = first;
			new_node->left = second;
			tree_builder.push(new_node);
		}
	}
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


void OutBitStream::WriteBit(unsigned char bit)
{
	if(bits_count % 8 == 0) {
		buffer.push_back(0);
	}

	if(bit != 0) {
		size_t bit_pos = bits_count % 8;
		buffer[bits_count / 8] |= 1 << (7 - bit_pos);
	}

	++bits_count;
}

void OutBitStream::WriteByte(unsigned char byte)
{
	if(bits_count % 8 == 0) {
		buffer.push_back(byte);
	} else {
		size_t offset = bits_count % 8;
		buffer[bits_count / 8] |= byte >> offset;
		buffer.push_back(byte << (8 - offset));
	}

	bits_count += 8;
}


unsigned char InBitStream::ReadBit() {
	size_t bit_pos = bits_count % 8;
	unsigned char bit = 0;
	
	bit |= (buffer[bits_count / 8] >> (7 - bit_pos)) & 1; 
	++bits_count;
	return bit;
}

unsigned char InBitStream::ReadByte() {
	if (bits_count % 8 == 0) {
		unsigned char byte = buffer[bits_count / 8];
		bits_count += 8;
		return byte;
	}

	size_t offset = bits_count % 8;
	unsigned char byte = 0;

	byte |= buffer[bits_count / 8] << offset;
	bits_count += 8 - offset;
	byte |= buffer[bits_count / 8] >> (8 - offset);
	bits_count += offset;
	return byte;
}




int main() {
	{
		std::cout << "***** Tree and table test *****" << std::endl;
		
		std::vector<size_t> frequencies(10);
		std::srand(std::time(nullptr));
		for (unsigned char i = 1; i < 8; ++i) {
			frequencies[i] = std::rand() % 10;
		}
		HaffmanCode encode;
		encode.build_tree(frequencies);

		std::vector<std::vector<unsigned char>> table = encode.get_table();
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
		std::cout << std::endl;

		std::cout << "***** Serialize tree test *****" << std::endl;

		std::vector<unsigned char> serialized_tree = encode.serialize();
		for (size_t i = 0; i < serialized_tree.size(); ++i) {
			std::bitset<8> bits(serialized_tree[i]);
			std::cout << bits << "|";
		}
		std::cout << std::endl;

		std::cout << "**********" << std::endl;
		std::cout << std::endl;

		std::cout << "***** Read bit stream test *****" << std::endl;
		InBitStream input(serialized_tree);
		unsigned char count = input.ReadByte();
		for (size_t ai = 0; ai < count;) {
			if (input.ReadBit() == 1) {
				unsigned char byte = input.ReadByte();
				++ai;
				std::cout << 1 << "|" << static_cast<int>(byte) << "|";
			} else {
				std::cout << 0 << "|";
			}
		}
		std::cout << std::endl;

		std::cout << "**********" << std::endl;
		std::cout << std::endl;

		std::cout << "***** Deserialize tree test *****" << std::endl;

		HaffmanCode decode;
		decode.deserialize(serialized_tree);

		std::vector<std::vector<unsigned char>> decoded_table = encode.get_table();
		for (size_t i = 0; i < decoded_table.size(); ++i) {
			if (decoded_table[i].size() != 0) {
				std::cout << i << ": ";
				for (size_t j = 0; j < decoded_table[i].size(); ++j) {
					std::cout << static_cast<int>(decoded_table[i][j]) << " ";
				}
				std::cout << std::endl;
			}
		}

		std::cout << "**********" << std::endl;
		std::cout << std::endl;
	}
}