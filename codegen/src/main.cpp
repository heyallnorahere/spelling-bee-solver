#include "actions.h"
#include <iostream>
#include <string>
#include <cassert>
#include <map>
#include <functional>
std::map<std::string, std::function<int(int, char*[])>> actions;
void add_action(const std::string& action_name, std::function<int(int, char*[])> action) {
    actions[action_name] = action;
}
int main(int argc, char* argv[]) {
    add_action("embed", [](int c, char* v[]) {
        assert(c >= 4);
        return embed(v[2], v[3]);
    });
    if (argc < 2) {
        std::cout << "No action specified!" << std::endl;
        return -1;
    }
    std::string action = argv[1];
    if (actions.find(action) != actions.end()) {
        std::cout << "Executing action: " << action << std::endl;
        return actions[action](argc, argv);
    }
    std::cout << "No action found matching \"" << action << "\"" << std::endl;
    return -1;
}
