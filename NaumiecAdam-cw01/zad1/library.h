//
// Created by Adam Naumiec on 28/02/2023.
//

#ifndef NAUMIECADAM_CW01_LIBRARY_H
#define NAUMIECADAM_CW01_LIBRARY_H

#define BUFFER_SIZE 1025

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

typedef struct {
    char** arr;
    int size;
    int numberOfActiveBlocks;
} libraryMemory;

// 0
libraryMemory libraryCreate(int size);

// 1
void libraryInit(libraryMemory* libraryMemory, int size);

// 2
void libraryCount(libraryMemory* libraryMemory, char* name);

// 3
char* libraryGetIndex(libraryMemory* libraryMemory, int index);

// 4
void libraryDeleteIndex(libraryMemory* libraryMemory, int index);

// 5
void libraryDeleteArray(libraryMemory* libraryMemory);


#endif //NAUMIECADAM_CW01_LIBRARY_H
