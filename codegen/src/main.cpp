#include "actions.h"
#include <iostream>
#include <string>
#include <cassert>
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
