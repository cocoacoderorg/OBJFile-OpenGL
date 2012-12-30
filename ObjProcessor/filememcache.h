//
//  filememcache.h
//  ObjProcessor
//
//  Created by Ian Bullard on 12/29/12.
//  Copyright (c) 2012 Ian Bullard. All rights reserved.
//

#ifndef ObjProcessor_filememcache_h
#define ObjProcessor_filememcache_h

#include <stdio.h>

#include "constants.h"

#define FILE_MEM_CACHE_SIZE 4098

typedef struct
{
    FILE* fp;
    char cache[FILE_MEM_CACHE_SIZE];
    size_t max_read;
    size_t cur_off;
} FileMemCache;

FileMemCache* filemem_open(char * path);

int filemem_readLine(FileMemCache* fp, char* dest, int maxlen);

bool filemem_eol(FileMemCache* fp);

void filemem_close(FileMemCache* fp);


#endif
