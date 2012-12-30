//
//  expanding_list.h
//  ObjProcessor
//
//  Created by Ian Bullard on 12/29/12.
//  Copyright (c) 2012 Ian Bullard. All rights reserved.
//

#ifndef ObjProcessor_expanding_list_h
#define ObjProcessor_expanding_list_h

#include <stdlib.h>
#include <memory.h>

#define DEFINE_LIST(type, typename)   \
    typedef struct  \
    {   \
        type* l;    \
        int cur_size;   \
        int max_size;   \
        size_t type_size;   \
    } typename;

#define CREATE_LIST(list, type, init_size)    \
    {   \
        list.l = malloc(sizeof(type) * init_size);   \
        list.cur_size = 0;  \
        list.max_size = init_size;  \
        list.type_size = sizeof(type);   \
    }

#define FREE_LIST(list)   \
    free(list.l);

#define ADD_LIST(list, value) \
    {   \
        if( list.cur_size == list.max_size )    \
        {   \
            void* newlist = malloc(list.type_size * list.max_size * 2);   \
            memcpy(newlist, list.l, list.type_size * list.cur_size);  \
            free(list.l);   \
            list.l = newlist;   \
            list.max_size = list.max_size * 2; \
        }   \
        list.l[list.cur_size++] = value;    \
    }

#endif
