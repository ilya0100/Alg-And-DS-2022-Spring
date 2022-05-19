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

	void write_bit(unsigned char bit);
	void write_byte(unsigned char byte);

	const std::vector<unsigned char>& get_buffer() const { return buffer; }
    size_t get_bits_count() const { return bits_count; }

 private:
	std::vector<unsigned char> buffer;
	size_t bits_count;
};

class InBitStream {
 public:
	InBitStream(std::vector<unsigned char>& _buffer): buffer(_buffer), bits_count(0) {}

	unsigned char read_bit();
	unsigned char read_byte();

	const std::vector<unsigned char>& get_buffer() const { return buffer; }
    size_t get_bits_count() const { return bits_count; }

 private:
	std::vector<unsigned char> buffer;
	size_t bits_count;
};


class HaffmanCode {
 public:
	HaffmanCode(): _root(nullptr) {}

	HaffmanCode(const HaffmanCode& other) = delete;
	HaffmanCode& operator=(const HaffmanCode& other) = delete;

	~HaffmanCode();

	void build_tree(const std::vector<size_t>& bytes_freq);
	std::vector<std::vector<unsigned char>> get_table();

	std::vector<unsigned char> serialize();
	void deserialize(std::vector<unsigned char>& buffer);

    std::vector<unsigned char> decode_data(std::vector<unsigned char>& data,
                                           unsigned char filling_size) const;

 private:
	struct Node {
		Node* left;
		Node* right;
		size_t p;
		unsigned char byte;

		Node(): left(nullptr), right(nullptr), p(0), byte(0) {}
	};

	struct IsMoreNode {
		bool operator()(const Node* l, const Node* r) const {
			return l->p > r->p;
		}
	};

	Node* _root;
	std::vector<std::vector<unsigned char>> table;

	void delete_tree(Node* node);
	void build_table(Node* node, std::vector<unsigned char>& code);

	void serialize_tree(Node* node, OutBitStream& stream);
};


void Encode(CInputStream& original, COutputStream& compressed) {
	std::vector<size_t> frequencies(256);
	std::vector<unsigned char> original_buffer;

    unsigned char byte;
	while (original.Read(byte)) {
		++frequencies[byte];
		original_buffer.push_back(byte);
	}

	HaffmanCode code;
	code.build_tree(frequencies);
	std::vector<unsigned char> tree = code.serialize();

    compressed.Write(tree.size());
	for (size_t i = 0; i < tree.size(); ++i) {
		compressed.Write(tree[i]);
	}

	std::vector<std::vector<unsigned char>> table = code.get_table();
	OutBitStream stream;
    for (size_t i = 0; i < original_buffer.size(); ++i) {
        for (size_t j = 0; j < table[original_buffer[i]].size(); ++j) {
            if (table[original_buffer[i]][j] == 1) {
                stream.write_bit(1);
            } else {
                stream.write_bit(0);
            }
        }
    }

    std::vector<unsigned char> compressed_buffer = stream.get_buffer();
    unsigned char filling_size = compressed_buffer.size() * 8 - stream.get_bits_count();

    compressed.Write(filling_size);
    for (size_t i = 0; i < compressed_buffer.size(); ++i) {
        compressed.Write(compressed_buffer[i]);
    }
    // tree_size | tree | filling_size | data
}

void Decode(CInputStream& compressed, COutputStream& original) {
    unsigned char tree_size = 0;
    compressed.Read(tree_size);

    std::vector<unsigned char> tree;
    for (unsigned char i = 0; i < tree_size; ++i) {
        unsigned char byte;
        compressed.Read(byte);
        tree.push_back(byte);
    }

    unsigned char filling_size = 0;
    compressed.Read(filling_size);

    std::vector<unsigned char> compressed_buffer;
    unsigned char byte;
    while (compressed.Read(byte)) {
        compressed_buffer.push_back(byte);
    }

    HaffmanCode code;
    code.deserialize(tree);
    std::vector<unsigned char> decoded = code.decode_data(compressed_buffer, filling_size);

    for (size_t i = 0; i < decoded.size(); ++i) {
        original.Write(decoded[i]);
    }
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
		build_table(_root, code);
	}
	return table;
}

void HaffmanCode::build_table(Node* node, std::vector<unsigned char>& code) {
	if (node->left == nullptr || node->right == nullptr) {
		table[node->byte] = code;
		code.pop_back();
		return;
	}
	code.push_back(0);
	build_table(node->left, code);

	code.push_back(1);
	build_table(node->right, code);
	code.pop_back();
}

std::vector<unsigned char> HaffmanCode::serialize() {
	OutBitStream output;
	serialize_tree(_root, output);
    output.write_bit(0);
	return std::move(output.get_buffer());
}

void HaffmanCode::serialize_tree(Node* node, OutBitStream& stream) {
	if (node->left == nullptr || node->right == nullptr) {
		stream.write_bit(1);
		stream.write_byte(node->byte);
		return;
	}
	serialize_tree(node->left, stream);
	serialize_tree(node->right, stream);
	stream.write_bit(0);
}

void HaffmanCode::deserialize(std::vector<unsigned char>& buffer) {
	InBitStream stream(buffer);
	std::stack<Node*> tree_builder;

	while (stream.get_bits_count() / 8 < buffer.size()) {
		if (stream.read_bit() == 1) {
			Node* new_node = new Node;
			new_node->byte = stream.read_byte();
			tree_builder.push(new_node);
		} else {
            if (tree_builder.size() == 1) {
                break;
            }
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
    _root = tree_builder.top();
}

std::vector<unsigned char> HaffmanCode::decode_data(std::vector<unsigned char>& data,
                                                    unsigned char filling_size) const {
    InBitStream stream(data);
    size_t data_size = data.size() - filling_size;
    std::vector<unsigned char> decoded;

    Node* node = _root;
    for (size_t i = 0; i < data_size; ++i) {
        if (stream.read_bit() == 0) {
            node = node->left;
        } else {
            node = node->right;
        }
        if (node->left == nullptr || node->right == nullptr) {
            decoded.push_back(node->byte);
            node = _root;
        }
    }
    return decoded;
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


void OutBitStream::write_bit(unsigned char bit) {
	if(bits_count % 8 == 0) {
		buffer.push_back(0);
	}

	if(bit != 0) {
		size_t bit_pos = bits_count % 8;
		buffer[bits_count / 8] |= 1 << (7 - bit_pos);
	}

	++bits_count;
}

void OutBitStream::write_byte(unsigned char byte) {
	if(bits_count % 8 == 0) {
		buffer.push_back(byte);
	} else {
		size_t offset = bits_count % 8;
		buffer[bits_count / 8] |= byte >> offset;
		buffer.push_back(byte << (8 - offset));
	}

	bits_count += 8;
}


unsigned char InBitStream::read_bit() {
	size_t bit_pos = bits_count % 8;
	unsigned char bit = 0;

	bit |= (buffer[bits_count / 8] >> (7 - bit_pos)) & 1;
	++bits_count;
	return bit;
}

unsigned char InBitStream::read_byte() {
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
		std::srand(42);
		for (unsigned char i = 0; i < 9; ++i) {
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


		std::cout << "***** Deserialize tree test *****" << std::endl;

		HaffmanCode decode;
		decode.deserialize(serialized_tree);

		std::vector<std::vector<unsigned char>> decoded_table = decode.get_table();
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
    {
        std::string test_text = "Just text to test Haffman algorithm";
        std::vector<unsigned char> buffer;
        for (size_t i = 0; i < test_text.size(); ++i) {
            buffer[i] = test_text[i];
        }

        CInputStream encode_in(buffer);
        std::vector<unsigned char> temp;
        COutputStream encode_out(temp);
        Encode(encode_in, encode_out);

        CInputStream decode_in(encode_out.get_body());
        COutputStream decode_out(temp);
        Decode(decode_in, decode_out);

        std::vector<unsigned char> decoded = decode_out.get_body();
        std::string decoded_str;
        for (size_t i = 0; i < decoded.size(); ++i) {
            decoded_str.push_back(decoded[i]);
        }
        std::cout << decoded_str;
    }
}
