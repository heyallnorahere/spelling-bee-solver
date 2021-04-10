#include <curl/curl.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include <cassert>
size_t write_callback(char* pointer, size_t size, size_t nmemb, void* userdata) {
    std::stringstream& output = *(std::stringstream*)userdata;
    output << std::string(pointer, nmemb);
    return nmemb;
}
CURLcode pull(CURL* c, const std::string& url, std::string& output) {
    std::stringstream data;
    CURLcode code;
    code = curl_easy_setopt(c, CURLOPT_URL, url.c_str());
    code = curl_easy_setopt(c, CURLOPT_SSL_VERIFYPEER, false);
    code = curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, write_callback);
    code = curl_easy_setopt(c, CURLOPT_WRITEDATA, &data);
    code = curl_easy_perform(c);
    output = data.str();
    return code;
}
template<typename T> bool __equals__(const T& a, const T& b) {
    return a == b;
}
template<typename Container, typename Element> bool contains(const Container& container, const Element& element, std::function<bool(const Element&, const Element&)> equals = __equals__<Element>) {
    for (const Element& e : container) {
        if (equals(e, element)) return true;
    }
    return false;
}
int embed(const std::string& url, const std::string& filepath) {
    std::string data;
    CURL* c = curl_easy_init();
    CURLcode code = pull(c, url, data);
    if (code != CURLE_OK) return (int)code;
    curl_easy_cleanup(c);
    std::cout << "Successfully pulled data from: " << url << std::endl;
    std::cout << "Parsing dictionary data..." << std::endl;
    std::string double_endl = "\r\n\r\n";
    size_t pos = data.find(double_endl);
    if (pos != std::string::npos) {
        data = data.substr(pos + double_endl.length());
    }
    std::string word = "";
    std::vector<std::string> dict;
    for (size_t i = 0; i < data.length(); i++) {
        char c = data[i];
        if (c == '\n') {
            dict.push_back(word);
            word.clear();
        } else if (c != '\r') {
            word.push_back(data[i]);
        }
    }
    if (!contains(dict, word)) {
        dict.push_back(word);
    }
    std::cout << "Embedding data into C source/header file: " << filepath << std::endl;
    std::string footer = "const char* embedded_data[] = {\n";
    std::ofstream output(filepath);
    output << footer;
    for (const auto& w : dict) {
        output << "\t\"" << w << "\",\n";
    }
    output << "};" << std::flush;
    output.close();
    return 0;
}
int main(int argc, char* argv[]) {
    if (argc < 2) {
        return -1;
    }
    std::string action = argv[1];
    if (action == "embed") {
        assert(argc >= 4);
        return embed(argv[2], argv[3]);
    }
    std::cout << "No action found matching \"" << action << "\"" << std::endl;
    return -1;
}
