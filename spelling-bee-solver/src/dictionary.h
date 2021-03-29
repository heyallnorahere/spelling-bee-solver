#pragma once
#include <string>
#include <vector>
#include <unordered_set>
class dictionary {
public:
	dictionary(const std::string& url, int max_length = 0, int min_length = 0);
	bool is_word(const std::string& word);
private:
	std::unordered_set<std::string> m;
};