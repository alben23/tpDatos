#include "Sequential.h"
#include <stdlib.h>

void FileSequential::create() {
    pFile = fopen(filename, "wb");
    fclose(pFile);
    pFile = NULL;
}

void FileSequential::insert(Comparator * c, void * record) {
    open();
    
    if (!hasKey(c)) {
    
        fseek(pFile, 0, SEEK_END);
        fwrite(record, length, 1, pFile);
    }
}

bool FileSequential::hasKey(Comparator * c) {
    void * obj = find(c);
    return !!obj;
}


void FileSequential::update(Comparator * c, void * record) {
    void * found = find(c);
    
    if (found) {
        fseek(pFile, -length, SEEK_CUR);
        // TODO checkeo de unicidad
        fwrite(record, length, 1, pFile);
    }
};

void FileSequential::remove(Comparator * c) {

    fseek(pFile, -length, SEEK_END);
    void * last = next();
    void * record = find(c);

    if (record) {
        fseek(pFile, -length, SEEK_CUR);
        fwrite(last, length, 1, pFile);
        // TODO Truncate file
    }
    
};

void * FileSequential::find(Comparator * c) {
    reset();
    void * obj = next();
    
    while (obj != NULL) {
        if (c->compareTo(obj) == 0) {
            return obj;
        }
        
        obj = next();
    }
    
    return NULL;
};

void * FileSequential::next() {
    
    void * buffer = (void *) malloc(length);
    fread(buffer, length, 1, pFile);

    if (feof(pFile)) {
        return NULL;
    } else {
        return buffer;
    }
}
