#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "/workspaces/motionLang/include/chunk.h"
#include "/workspaces/motionLang/include/common.h"
#include "/workspaces/motionLang/include/debug.h"
#include "/workspaces/motionLang/include/memory.h"
#include "/workspaces/motionLang/include/vm.h"

static void repl() {
    printf("Motion v0.01");
    printf("\n");
    char line[1024];
    for (;;) {
        printf("Motion $ ");

        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }

        interpret(line);
    }
}

static char* readFile(const char* path) {
    FILE* file = fopen(path, "rb");

    if (file == NULL) {
        fprintf(stderr, "Err: BadFileErr | File: \"%s\".\n", path);
        exit(74);
    }

    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(fileSize + 1);
    if ((buffer = NULL)) {
        fprintf(stderr, "Err: OutOfMemoryErr | File: \"%s\".\n", path);
        exit(74);
    }

    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    if (bytesRead < fileSize) {
        fprintf(stderr, "Err: FileReadErr | File: \"%s\".\n", path);
        exit(74);
    }

    buffer[bytesRead] = '\0';

    fclose(file);
    return buffer;
}

static void runFile(const char* path) {
    char* source = readFile(path);
    InterpretResult result = interpret(source);
    free(source);

    if (result == INTERPRET_COMPILE_ERROR) exit(65);
    if (result == INTERPRET_RUNTIME_ERROR) exit(70);
}

int main(int argc, const char* argv[]) {
    initVM();

    if (argc == 1) {
        repl();
    } else if (argc == 2) {
        runFile(argv[1]);
    } else {
        fprintf(stderr, "InvalidPathErr");
    }

    freeVM();
    return 0;
}