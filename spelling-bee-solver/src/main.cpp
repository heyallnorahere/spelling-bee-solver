#include "dictionary.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include <memory>
#include <iostream>
#include <array>
std::shared_ptr<dictionary> _dictionary;
struct char_t {
	char data;
	operator char() {
		return this->data;
	}
};
void from_json(const nlohmann::json& j, char_t& c) {
	auto temp = j.get<std::string>();
	c.data = temp[0];
}
char main_character;
std::vector<char> other_characters;
template<typename T, typename U> bool contains(const T& container, const U& value) {
	for (const U& val : container) {
		if (val == value) {
			return true;
		}
	}
	return false;
}
#ifdef ALGORITHM_SLOWER
void make_words(const std::string& prompt, std::vector<std::string>& words, int length, int index = 0) {
	std::vector<char> compounded_list = other_characters;
	compounded_list.push_back(main_character);
	for (char c : compounded_list) {
		std::string word = prompt + c;
		if (index >= length - 1) {
			if (_dictionary->is_word(word) && contains(word, main_character)) {
				words.push_back(word);
			}
		}
		else {
			make_words(word, words, length, index + 1);
		}
	}
}
void make_words(int min_word_length, int max_word_length, std::vector<std::string>& words) {
	for (int length = min_word_length; length < max_word_length + 1; length++) {
		make_words("", words, length);
	}
}
#endif
#ifdef ALGORITHM_DAWG
std::vector<char> get_compounded_list() {
	std::vector<char> list = other_characters;
	list.push_back(main_character);
	return list;
}
struct node {
	std::array<node*, 26> children;
	bool eow;
	char character;
	node* parent;
	node() {
		for (size_t i = 0; i < this->children.size(); i++) {
			this->children[i] = NULL;
		}
		this->eow = false;
		this->character = '\0';
		this->parent = NULL;
	}
	~node() {
		for (size_t i = 0; i < this->children.size(); i++) {
			delete this->children[i];
		}
	}
};
node root;
void insert_word(const std::string& word, node& n, int index = 0) {
	if (!contains(get_compounded_list(), word[index])) {
		return;
	}
	auto& ptr = n.children[word[index] - 'a'];
	if (!ptr) {
		ptr = new node;
		ptr->parent = &n;
		ptr->character = word[index];
	}
	if (index >= word.length() - 1) {
		ptr->eow = true;
	} else {
		insert_word(word, *ptr, index + 1);
	}
}
void assemble(node& n, std::string& word) {
	if (n.character != '\0') {
		assemble(*n.parent, word);
		word.push_back(n.character);
	}
}
void walk_node(node& n, std::vector<std::string>& words) {
	if (n.eow) {
		std::string word;
		assemble(n, word);
		if (contains(word, main_character)) words.push_back(word);
	}
	for (auto child : n.children) {
		if (child) {
			walk_node(*child, words);
		}
	}
}
void make_words(int min_word_length, int max_word_length, std::vector<std::string>& words) {
	for (size_t i = 0; i < _dictionary->get_word_count(); i++) {
		insert_word(_dictionary->get_word(i), root);
	}
	walk_node(root, words);
}
#endif
int main(int argc, const char* argv[]) {
	constexpr int max_word_length = 10;
	constexpr int min_word_length = 4;
	// credit to the repo author
	_dictionary = std::shared_ptr<dictionary>(new dictionary(max_word_length, min_word_length));
	std::cout << "pulled and loaded dictionary!" << std::endl;
	std::string spelling_bee_path = "spelling-bee.json";
	if (argc >= 2) {
		spelling_bee_path = argv[1];
	}
	std::ifstream file(spelling_bee_path);
	nlohmann::json spelling_bee_data;
	file >> spelling_bee_data;
	file.close();
	std::cout << "loaded spelling bee data!" << std::endl;
	std::vector<std::string> words;
	main_character = spelling_bee_data["main character"].get<char_t>();
	auto _other_characters = spelling_bee_data["other characters"];
	assert(_other_characters.size() >= 6 && _other_characters.is_array());
	for (auto c : _other_characters) {
		other_characters.push_back(c.get<char_t>());
	}
	make_words(min_word_length, max_word_length, words);
	nlohmann::json answers;
	for (const auto& word : words) {
		answers.push_back(word);
	}
	std::string answers_file_path = "answers.json";
	std::ofstream answers_file(answers_file_path);
	answers_file << answers.dump(4);
	answers_file.close();
	std::cout << "wrote answers to: " + answers_file_path << std::endl;
	return 0;
}
