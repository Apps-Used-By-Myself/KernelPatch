
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "module.h"

int main()
{
    FILE *file = fopen("./hello/hello.kpm", "r");
    if (file == NULL) {
        perror("Failed to open the file");
        return -1;
    }
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    if (fileSize == -1) {
        perror("Failed to determine file size");
        fclose(file);
        return -1;
    }

    char *fileContent = (char *)malloc(fileSize);
    if (fileContent == NULL) {
        perror("Failed to allocate memory");
        fclose(file);
        return -1;
    }

    size_t bytesRead = fread(fileContent, 1, fileSize, file);

    if (bytesRead != fileSize) {
        perror("Failed to read file content");
        fclose(file);
        free(fileContent);
        return 1;
    }

    init_module(fileContent, bytesRead, 0);

    free(fileContent);
    fclose(file);

    return 0;
}