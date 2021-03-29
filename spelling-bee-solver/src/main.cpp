#include "dictionary.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include <memory>
#include <iostream>
// credit to the repo author
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
void make_words(int length, std::vector<std::string>& words) {
	make_words("", words, length);
}
int main(int argc, const char* argv[]) {
	constexpr int max_word_length = 8;
	constexpr int min_word_length = 4;
	_dictionary = std::shared_ptr<dictionary>(new dictionary("https://raw.githubusercontent.com/dwyl/english-words/master/words_dictionary.json", max_word_length, min_word_length));
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
	for (int length = min_word_length; length < max_word_length; length++) {
		make_words(length, words);
	}
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