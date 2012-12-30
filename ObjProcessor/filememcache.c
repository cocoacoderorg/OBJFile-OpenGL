//
//  filememcache.c
//  ObjProcessor
//
//  Created by Ian Bullard on 12/29/12.
//  Copyright (c) 2012 Ian Bullard. All rights reserved.
//

#include "filememcache.h"

#include <stdlib.h>
#include <memory.h>

FileMemCache* filemem_open(char * path)
{
    FileMemCache* res = malloc(sizeof(FileMemCache));
    
    res->fp = fopen(path, "r");
    
    if(res->fp == NULL)
    {
        free(res);
        return NULL;
    }
    
    res->max_read = fread(res->cache, sizeof(char), FILE_MEM_CACHE_SIZE, res->fp);
    res->cur_off = 0;
    
    return res;
}

int filemem_readLine(FileMemCache* fp, char* dest, int maxlen)
{
    int char_read = 0;
    
    memset(dest, 0, maxlen);
    
    while(1)
    {
        // have we filled the buffer?
        if( char_read == maxlen )
        {
            // yes, leave.  It's the user's problem to deal with too long of a line
            break;
        }
        
        // is there a char in the buffer?
        if(fp->cur_off < fp->max_read)
        {
            // yes, check it for EOL
            
            // Consume first part of unix line ending just in case
            if(fp->cache[fp->cur_off] == '\r')
                fp->cur_off++;
            
            if(fp->cache[fp->cur_off] == '\n')
            {
                // we have EOL, exit
                fp->cur_off++;
                
                return char_read;
            }
            else
            {
                dest[char_read] = fp->cache[fp->cur_off];
                char_read++;
                fp->cur_off++;
            }
        }
        else
        {
            // we have to refill the buffer
            
            // Are we at EOF?
            if( fp->max_read < FILE_MEM_CACHE_SIZE && fp->cur_off == fp->max_read )
            {
                // we are, just stop
                return char_read;
            }
            else
            {
                fp->max_read = fread(fp->cache, sizeof(char), FILE_MEM_CACHE_SIZE, fp->fp);
                fp->cur_off = 0;
            }
        }
    }
    
    return char_read;
}

bool filemem_eol(FileMemCache* fp)
{
    return fp->max_read < FILE_MEM_CACHE_SIZE && fp->cur_off == fp->max_read;
}

void filemem_close(FileMemCache* fp)
{
    fclose(fp->fp);
    
    free(fp);
}
