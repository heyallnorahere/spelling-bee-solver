#pragma once
#include <string>
#include <vector>
class dictionary {
public:
	dictionary(const std::string& url);
	bool is_word(const std::string& word);
private:
	std::vector<std::string> m;
};