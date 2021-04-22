#include "actions.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <map>
#include <functional>
#include <nlohmann/json.hpp>
struct action {
    std::function<int(int, char*[], void*)> ptr;
    void* userdata;
};
struct json_action {
    std::string trigger, assemblypath, entrypoint;
    int argcount;
};
void from_json(const nlohmann::json& j, json_action& ja) {
    j["trigger"].get_to(ja.trigger);
    j["assemblypath"].get_to(ja.assemblypath);
    j["entrypoint"].get_to(ja.entrypoint);
    j["argcount"].get_to(ja.argcount);
}
std::map<std::string, action> actions;
void add_action(const std::string& action_name, std::function<int(int, char*[], void*)> action, void* userdata = NULL) {
    actions[action_name] = { action, userdata };
}
bool file_exists(const std::string& path) {
    FILE* f = fopen(path.c_str(), "rb");
    bool exists = f;
    if (f) {
        fclose(f);
    }
    return exists;
}
int main(int argc, char* argv[]) {
    add_action("embed", [](int c, char* v[], void*) {
        assert(c >= 4);
        return embed(v[2], v[3]);
    });
    std::vector<json_action> json_actions;
    if (file_exists("actions.json")) {
        std::ifstream file("actions.json");
        nlohmann::json j;
        file >> j;
        file.close();
        j.get_to(json_actions);
    }
    if (argc < 2) {
        std::cout << "No action specified!" << std::endl;
        return -1;
    }
    std::string action = argv[1];
    if (actions.find(action) != actions.end()) {
        std::cout << "Executing action: " << action << std::endl;
        return actions[action].ptr(argc, argv, actions[action].userdata);
    }
    std::cout << "No action found matching \"" << action << "\"" << std::endl;
    return -1;
}
