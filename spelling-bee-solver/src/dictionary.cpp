#include "dictionary.h"
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <cassert>
#include <iostream>
#include <sstream>
#include <map>
size_t write_callback(char* pointer, size_t size, size_t nmemb, void* userdata) {
	std::stringstream& output = *(std::stringstream*)userdata;
	output << std::string(pointer, nmemb * size);
	return nmemb;
}
dictionary::dictionary(const std::string& url) {
	CURL* curl = curl_easy_init();
	assert(curl);
	assert(!curl_easy_setopt(curl, CURLOPT_URL, url.c_str()));
	assert(!curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false));
	std::stringstream data;
	assert(!curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback));
	assert(!curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data));
	assert(!curl_easy_perform(curl));
	curl_easy_cleanup(curl);
	nlohmann::json j;
	data >> j;
	std::map<std::string, int> map;
	j.get_to(map);
	for (auto p : map) {
		this->m.push_back(p.first);
	}
}
bool dictionary::is_word(const std::string& word) {
	for (const auto& w : this->m) {
		if (w == word) {
			return true;
		}
	}
	return false;
}
