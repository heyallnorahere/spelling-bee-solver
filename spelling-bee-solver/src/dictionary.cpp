#include "dictionary.h"
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <cassert>
dictionary::dictionary(const std::string& url) {
	CURL* curl = curl_easy_init();
	assert(curl);
	auto result = curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	assert(result == CURLE_OK);
	result = curl_easy_perform(curl);
	assert(result == CURLE_OK);
	curl_easy_cleanup(curl);
}