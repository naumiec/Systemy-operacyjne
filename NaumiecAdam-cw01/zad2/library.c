//
// Created by Adam Naumiec on 28/02/2023.
//
#include "library.h"

char command[BUFFER_SIZE] = "";


// 0
libraryMemory libraryCreate(int size) {
    libraryMemory libraryMemory;
    libraryMemory.arr = calloc(size, sizeof(char*));
    libraryMemory.size = size;
    libraryMemory.numberOfActiveBlocks = 0;

    return libraryMemory;
}

// 1
void libraryInit(libraryMemory* libraryMemory, int size) {
    libraryMemory->arr = calloc(size, sizeof(char*));
    libraryMemory->size = size;
    libraryMemory->numberOfActiveBlocks = 0;
}

// 3
char* libraryGetIndex(libraryMemory* libraryMemory, int index) {
    if (index < libraryMemory->numberOfActiveBlocks) {
        return libraryMemory->arr[index];
    } else {
        fprintf(stderr, "OUT OF RANGE\n");
        return "ERROR\n";
    }
}

// 4
void libraryDeleteIndex(libraryMemory* libraryMemory, int index) {
    if (index < libraryMemory->numberOfActiveBlocks) {
        free(libraryMemory->arr[index]);
        libraryMemory->arr[index] = NULL;
    } else {
        fprintf(stderr, "OUT OF RANGE\n");
    }
}

// 5
void libraryDeleteArray(libraryMemory* libraryMemory) {
    for (int i = 0; i < libraryMemory->numberOfActiveBlocks; i++) {
        free(libraryMemory->arr[i]);
    }

    free(libraryMemory->arr);
}


long getFileSize(FILE* file) {
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    return size;
}


char* getFile(char* filename) {
    FILE* file = fopen(filename, "r");
    long size = getFileSize(file);

    if (file == NULL) {
        fprintf(stderr, "CANNOT OPEN FILE: %s\n", filename);
        return "ERROR\n";
    }

    char* buffer = calloc(size, sizeof(char));
    fread(buffer, sizeof(char), size, file);
    fclose(file);

    return buffer;
}

// 2
void libraryCount(libraryMemory* libraryMemory, char* filename) {
    char* tmpFilename = "/tmp/tmp_XXXXXX.txt";
    int tmp_file = mkstemp(tmpFilename);

    if (tmp_file == -1 || tmp_file == 0 || tmpFilename == NULL) {
        fprintf(stderr, "CANNOT CREATE TEMPORARY FILE\n");
    }


    FILE* tmpFileHandler = fopen(tmpFilename, "rw");

    if (tmpFileHandler == NULL  || tmpFilename == NULL) {
        fprintf(stderr, "CANNOT OPEN TEMPORARY FILE\n");
    }


    sprintf(command, "wc -l '%s' > '%s'", filename, tmpFilename);
    int systemReturn = system(command);

    if (systemReturn == -1 || systemReturn == 0) {
        fprintf(stderr, "CANNOT EXECUTE SYSTEM COMMAND\n");
    }


    char* fileReturn = getFile(tmpFilename);

    if (fileReturn == NULL) {
        fprintf(stderr, "CANNOT OPEN FILE '%s'\n", tmpFilename);
    }


    fseek(tmpFileHandler, 0, SEEK_END);
    long size = ftell(tmpFileHandler);
    fseek(tmpFileHandler, 0, SEEK_SET);


    if (libraryMemory->numberOfActiveBlocks >= libraryMemory->size) {
        fprintf(stderr, "OUT OF MEMORY\n");
        return;
    } else {
        libraryMemory->arr[libraryMemory->numberOfActiveBlocks] = calloc(size, sizeof(char));
        fread(libraryMemory->arr[libraryMemory->numberOfActiveBlocks], sizeof(char), size, tmpFileHandler);
        libraryMemory->numberOfActiveBlocks++;
    }

    remove(tmpFilename);
    fclose(tmpFileHandler);
    system("rm -f /tmp/tmp_*.txt");
}
