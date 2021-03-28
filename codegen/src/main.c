#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "actions.h"
void usage() {
    printf("Usage: codegen <action> <path1> <path2>\n");
}
int main(int argc, const char* argv[]) {
    if (argc < 4) {
        usage();
        return -1;
    }
    const char* action = argv[1];
    const char* path1 = argv[2];
    const char* path2 = argv[3];
    if (strcmp(action, "embed") == 0) {
        return action_embed_data(path1, path2);
    }
    printf("No action found that matches \"%s\"!\n", action);
    return -1;
}