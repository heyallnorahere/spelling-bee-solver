#pragma once
#include <string>
#include <vector>
#include <unordered_set>
class dictionary {
public:
	dictionary(const std::string& url, int max_length = 0, int min_length = 0);
	bool is_word(const std::string& word);
	size_t get_word_count();
	std::string get_word(size_t index);
private:
#ifdef ALGORITHM_SLOWER
	std::unordered_set<std::string> m;
#else
	std::vector<std::string> m;
#endif
};