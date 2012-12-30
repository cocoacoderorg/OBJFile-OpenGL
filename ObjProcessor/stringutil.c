//
//  stringutil.c
//  ObjProcessor
//
//  Created by Ian Bullard on 12/29/12.
//  Copyright (c) 2012 Ian Bullard. All rights reserved.
//

#include "stringutil.h"
#include "constants.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

// Cheap check for type, full check happens in obj_contents.c
LineType line_type(char* line)
{
    LineType result = UNKNOWN;
    
    char* start = line;
    
    switch(*start)
    {
        case 0:
            result = EMPTY;
            break;
        case '#':
            result = COMMENT;
            break;
        case 'v':
            start++;
            switch(*start)
            {
                case 't':
                    result = TEXTURE_COORD;
                    break;
                case 'n':
                    result = NORMAL;
                    break;
                case 'p':
                    result = VERTEX_PARAM;
                    break;
                default:
                    result = VERTEX;
                    break;
            }
            break;
        case 'f':
            result = FACE;
            break;
        case 'm':
            if(start[1] == 'a' && start[2] == 'p')
            {
                switch(start[4])
                {
                    case 'K':
                        switch(start[5])
                        {
                            case 'a':
                                result = MAT_TEX_AMBIENT;
                                break;
                            case 'd':
                                result = MAT_TEX_DIFFUSE;
                                break;
                            case 's':
                                result = MAT_TEX_SPECULAR_COLOR;
                                break;
                        }
                        break;
                    case 'N':
                        result = MAT_TEX_SPECULAR_COEFF;
                        break;
                    case 'd':
                        result = MAT_TEX_TRANSPARENT;
                        break;
                    case 'b':
                        result = MAT_TEX_BUMP;
                        break;
                    default:
                        break;
                }
            }
            else
            {
                result = MATERIAL_LIB;
            }
            break;
        case 'u':
            result = USE_MATERIAL;
            break;
        case 'o':
            result = OBJECT;
            break;
        case 'g':
            result = GROUP;
            break;
        case 's':
            result = SMOOTHING_GROUP;
            break;
        case 'n':
            result = MAT_NEW;
            break;
        case 'K':
            start++;
            switch(*start)
            {
                case 'a':
                    result = MAT_AMBIENT;
                    break;
                case 'd':
                    result = MAT_DIFFUSE;
                    break;
                case 's':
                    result = MAT_SPECULAR_COLOR;
                    break;
                default:
                    break;
            }
            break;
        case 'N':
            start++;
            switch(*start)
            {
                case 'i':
                    result = MAT_REFRACTION;
                    break;
                case 's':
                    result = MAT_SPECULAR_COEFF;
                    break;
                default:
                    break;
            }
            break;
        case 'd':
            start++;
            switch(*start)
            {
                case 'i':
                    result = MAT_TEX_DISPLACEMENT;
                    break;
                case 'e':
                    result = MAT_TEX_DECAL;
                    break;
                default:
                    result = MAT_TRANSPARENT;
                    break;
            }
        case 'T':
            start++;
            switch(*start)
            {
                case 'f':
                    result = MAT_TRANMISSION_FILTER;
                    break;
                case 'r':
                    result = MAT_TRANSPARENT;
                    break;
                default:
                    break;
            }
            break;
        case 'b':
            result = MAT_TEX_BUMP;
            break;
        case 'r':
            result = MAT_TEX_REFLECTION;
            break;
        case 'i':
            result = MAT_ILLUMINATION;
            break;
        default:
            break;
    }
    
    return result;
}

Vector parse_vector(char* line)
{
    Vector result;
    VEC_ASSIGN(result, NAN, NAN, NAN, NAN);
    char* delim = " ";
    
    char* next = strtok(line, delim);
    int index = 0;
    while(next != NULL && index < 4)
    {
        result.v[index++] = atof(next);
        next = strtok(NULL, delim);
    }
    
    return result;
}

FaceType parse_face(char* line)
{
    // Valid permutations for a face:
    //  v1 v2 v3 ...
    //  v1/vt1 v2/vt2 v3/vt3 ...
    //  v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ...
    //  v1//vn1 v2//vn2 v3//vn3 ...
    
    FaceType result;
    char* delim = " ";
    
    // init
    for(int i = 0; i < MAX_FACE_VERTS; ++i)
    {
        result.vert[i] = result.texcoord[i] = result.normal[i] = -1;
    }
    
    char* s= strtok(line, delim);
    char* end = s;
    int state, index = 0;
    
    while(*end != '\0') end++;
    
    while(s != NULL)
    {
        char* start = s;
        char* set = s;
        state = 0;
        bool exit = false;
        
        while (!exit)
        {
            if(*set == '/' || *set == '\0')
            {
                *set = '\0';
                
                if(state == 0)
                {
                    result.vert[index] = atoi(start);
                }
                else if(state == 1)
                {
                    // texture coord may be nothing (v1//n1)
                    if(start != set)
                        result.texcoord[index] = atoi(start);
                }
                else if(state == 2)
                {
                    result.normal[index] = atoi(start);
                }
                state++;
                start = set + 1;
                
                exit = set == end;
            }
            set++;
        }
        
        s = strtok(NULL, delim);
        if(s != NULL)
        {
            end = s;
            while(*end != '\0') end++;
        }
        index++;
    }
    
    return result;
}
