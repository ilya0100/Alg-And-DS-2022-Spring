/*
    Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
    Хранимые строки непустые и состоят из строчных латинских букв.
    Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
    Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в
    случае, когда коэффициент заполнения таблицы достигает 3/4.
    Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества и
    проверки принадлежности данной строки множеству.

    1_2. Для разрешения коллизий используйте двойное хеширование.
*/

#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <sstream>

#define INIT_SIZE 8


class StringHasher {
 public:
    StringHasher(unsigned int a): a(a) {}

    unsigned int operator()(const std::string& key) const {
        unsigned int hash = 0;
        auto it = key.begin();
        for (; it != key.end(); ++it) {
            hash = hash * a + *it;
        }
        return hash;
    }
 private:
    unsigned int a;
};


template <typename H>
class HashTable {
 public:
    HashTable(int init_size, H hasher): hasher(hasher), table(std::vector<TableCell>(init_size)),
                                        keys_count(0), dels_count(0) {}

    bool has(const std::string& key) const;
    bool add(const std::string& key);
    bool del(const std::string& key);

    // void print_table() const;

 private:
    enum class CellState {
        empty,
        key,
        deleted
    };

    struct TableCell {
        CellState state;
        unsigned int hash;
        std::string key;

        TableCell(): state(CellState::empty), hash(0) {}
    };

    H hasher;
    std::vector<TableCell> table;
    unsigned int keys_count;
    unsigned int dels_count;

    void rehash(size_t table_size);
    static unsigned int second_hasher(const std::string& key);
};



void run(std::istream& input, std::ostream& output) {
    StringHasher hasher(137);
    HashTable<StringHasher> table(INIT_SIZE, hasher);
	char operation;
	std::string word;

	while(input >> operation >> word) {
		if(operation == '+') {
			output << (table.add(word) ? "OK" : "FAIL") << std::endl;
        }
		if(operation == '?') {
			output << (table.has(word) ? "OK" : "FAIL") << std::endl;
        }
		if(operation == '-') {
			output << (table.del(word) ? "OK" : "FAIL") << std::endl;
        }
	}
}

void run_test() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "+ hello\n+ bye\n? bye\n+ bye\n- bye\n? bye\n? hello";
        run(input, output);
        std::cout << "***** Test 1: *****" << std::endl;
        std::cout << output.str() << std::endl;
        assert(output.str() == "OK\nOK\nOK\nFAIL\nOK\nFAIL\nOK\n");
        std::cout << "***** Sucess *****\n" << std::endl;
    }
    {
        std::stringstream input;
        std::stringstream output;
        for (int i = 0; i < 10; ++i) {
            input << "+ " << i << std::endl;
        }
        run(input, output);
        std::cout << "***** Test 2: *****" << std::endl;
        std::cout << output.str() << std::endl;
        assert(output.str() == "OK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\n");
        std::cout << "***** Sucess *****\n" << std::endl;
    }
}


int main() {
    // run(std::cin, std::cout);
    run_test();
    return 0;
}



template <typename H>
bool HashTable<H>::has(const std::string& key) const {
    unsigned int hash = hasher(key);
    unsigned int second_hash = second_hasher(key) * 2 + 1;

    for (unsigned int i = 0; i < table.size(); ++i) {
        unsigned int general_hash = (hash + second_hash * i) % table.size();
        if (table[general_hash].state == CellState::empty) {
            return false;
        }
        if (table[general_hash].state == CellState::key) {
            if (key == table[general_hash].key) {
                return true;
            }
        }
    }
    return false;
}

template <typename H>
bool HashTable<H>::add(const std::string& key) {
    if (keys_count > 3 * table.size() / 4) {
        rehash(table.size() * 2);
    }

    unsigned int hash = hasher(key);
    unsigned int second_hash = second_hasher(key) * 2 + 1;
    unsigned int first_del = hash % table.size();

    for (unsigned int i = 0; i < table.size(); ++i) {
        unsigned int general_hash = (hash + second_hash * i) % table.size();

        if (table[general_hash].state == CellState::empty) {
            if (table[first_del].state == CellState::deleted) {
                general_hash = first_del;
            }
            table[general_hash].state = CellState::key;
            table[general_hash].hash = hash;
            table[general_hash].key = key;

            ++keys_count;
            return true;
        }
        if (table[general_hash].state == CellState::key) {
            if (key == table[general_hash].key) {
                return false;
            }
        }
        if (table[general_hash].state == CellState::deleted &&
            table[first_del].state != CellState::deleted) {
                first_del = general_hash;
        }
    }
    return false;
}

template <typename H>
bool HashTable<H>::del(const std::string& key) {
    if (dels_count > table.size() / 4) {
        rehash(table.size());
    }

    unsigned int hash = hasher(key);
    unsigned int second_hash = second_hasher(key) * 2 + 1;

    for (unsigned int i = 0; i < table.size(); ++i) {
        unsigned int general_hash = (hash + second_hash * i) % table.size();
        if (table[general_hash].state == CellState::empty) {
            return false;
        }
        if (table[general_hash].state == CellState::key) {
            if (key == table[general_hash].key) {
                table[general_hash].state = CellState::deleted;
                --keys_count;
                ++dels_count;
                return true;
            }
        }
    }
    return false;
}

template <typename H>
void HashTable<H>::rehash(size_t table_size) {
    std::vector<TableCell> new_table(table_size);
    for (size_t i = 0; i < table.size(); ++i) {
        if (table[i].state == CellState::key) {
            new_table[table[i].hash % new_table.size()] = table[i];
        }
    }
    table = std::move(new_table);
}

template <typename H>
unsigned int HashTable<H>::second_hasher(const std::string& key) {
    unsigned int hash = 0;
    auto it = key.begin();
    for (int i = 0; i < 5 && it != key.end(); ++it, ++i) {
        hash = hash * 137 + *it;
    }
    return hash;
}

// template <typename H>
// void HashTable<H>::print_table() const {
//     for (size_t i = 0; i < table.size(); ++i) {
//         if (table[i].state == CellState::key) {
//             std::cout << table[i].key << std::endl;
//         }
//     }
// }
