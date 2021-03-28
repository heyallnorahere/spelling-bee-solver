#include "actions.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
int action_embed_data(const char* path1, const char* path2) {
	FILE* file = fopen(path1, "r");
	if (!file) {
		printf("File %s not found!\n", path1);
		return -1;
	}
	fseek(file, 0, SEEK_END);
	size_t size = (size_t)ftell(file);
	rewind(file);
	size_t buffer_size = size * sizeof(char);
	void* buffer = malloc(buffer_size);
	if (!buffer) {
		printf("Could not allocate %d bytes!\n", (int)buffer_size);
		return -1;
	}
	size_t read = fread(buffer, sizeof(char), size, file);
	if (read != size) {
		printf("Could not read %d bytes!\n", (int)buffer_size);
		free(buffer);
		return -1;
	}
	fclose(file);
	file = fopen(path2, "w");
	if (!file) {
		printf("File %s not found!\n", path2);
		free(buffer);
		return -1;
	}
	read = fwrite(buffer, sizeof(char), size, file);
	if (read != size) {
		printf("Could not write %d bytes!\n", (int)buffer_size);
		free(buffer);
		return -1;
	}
	fclose(file);
	free(buffer);
	return 0;
}