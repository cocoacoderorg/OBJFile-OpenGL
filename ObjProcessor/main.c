//
//  main.c
//  ObjProcessor
//
//  Created by Ian Bullard on 12/29/12.
//  Copyright (c) 2012 Ian Bullard. All rights reserved.
//

#include <stdio.h>
#include <memory.h>
#include <unistd.h>
#include <stdlib.h>

#include "constants.h"
#include "unit_test.h"
#include "filememcache.h"
#include "stringutil.h"
#include "obj_contents.h"
#include "postprocess.h"

// May be called multiple times to parse material files
void read_file(FileMemCache* fp, ObjFileContents* obj)
{
    char linebuffer[4096];
    int line_num = 0;
    
    while(!filemem_eol(fp))
    {
        filemem_readLine(fp, linebuffer, 4096);
        
        // skip whitespace
        char* start = linebuffer;
        
        while((*start == ' ' ||
               *start == '\t' ||
               *start == '\n' ||
               *start == '\r') &&
              *start != 0)
        {
            start++;
        }
        
        
        LineType lt = line_type(start);
        
        switch(lt)
        {
            case UNKNOWN:
                EmitUnknown(obj, line_num, start);
                break;
            case EMPTY:
                EmitEmpty(obj, line_num, start);
                break;
            case COMMENT:
                EmitComment(obj, line_num, start);
                break;
            case VERTEX:
                EmitVertex(obj, line_num, start);
                break;
            case TEXTURE_COORD:
                EmitTexCoord(obj, line_num, start);
                break;
            case NORMAL:
                EmitNormal(obj, line_num, start);
                break;
            case VERTEX_PARAM:
                EmitVertParam(obj, line_num, start);
                break;
            case FACE:
                EmitFace(obj, line_num, start);
                break;
            case MATERIAL_LIB:
                EmitMaterialLib(obj, line_num, start);
                break;
            case USE_MATERIAL:
                EmitUseMaterial(obj, line_num, start);
                break;
            case OBJECT:
                EmitObject(obj, line_num, start);
                break;
            case GROUP:
                EmitGroup(obj, line_num, start);
                break;
            case SMOOTHING_GROUP:
                EmitSmoothingGroup(obj, line_num, start);
                break;
            case MAT_NEW:
                EmitMatNew(obj, line_num, start);
                break;
            case MAT_AMBIENT:
                EmitMatAmbient(obj, line_num, start);
                break;
            case MAT_DIFFUSE:
                EmitMatDiffuse(obj, line_num, start);
                break;
            case MAT_SPECULAR_COLOR:
                EmitMatSpecularColor(obj, line_num, start);
                break;
            case MAT_SPECULAR_COEFF:
                EmitMatSpecularCoeff(obj, line_num, start);
                break;
            case MAT_TRANSPARENT:
                EmitMatTransparent(obj, line_num, start);
                break;
            case MAT_ILLUMINATION:
                EmitMatIllumination(obj, line_num, start);
                break;
            case MAT_REFRACTION:
                EmitMatRefraction(obj, line_num, start);
                break;
            case MAT_TRANMISSION_FILTER:
                EmitMatTransmissionFilter(obj, line_num, start);
                break;
            case MAT_TEX_AMBIENT:
                EmitMatTexAmbient(obj, line_num, start);
                break;
            case MAT_TEX_DIFFUSE:
                EmitMatTexDiffuse(obj, line_num, start);
                break;
            case MAT_TEX_SPECULAR_COLOR:
                EmitMatTexSpecularColor(obj, line_num, start);
                break;
            case MAT_TEX_SPECULAR_COEFF:
                EmitMatTexSpecularCoeff(obj, line_num, start);
                break;
            case MAT_TEX_TRANSPARENT:
                EmitMatTexTransparent(obj, line_num, start);
                break;
            case MAT_TEX_ILLUMINATION:
                EmitMatTexIllumination(obj, line_num, start);
                break;
            case MAT_TEX_BUMP:
                EmitMatTexBump(obj, line_num, start);
                break;
            case MAT_TEX_DISPLACEMENT:
                EmitMatTexDisplace(obj, line_num, start);
                break;
            case MAT_TEX_DECAL:
                EmitMatTexDecal(obj, line_num, start);
                break;
            case MAT_TEX_REFLECTION:
                EmitMatTexReflection(obj, line_num, start);
                break;
            default:
                EmitUnknown(obj, line_num, start);
                break;
        }
        
        line_num++;
    }
}

// called once
void read_obj_file(char* file, int maxVertCount, ObjFileContents* obj)
{
    FileMemCache*   fp = NULL;
    
    printf("Processing file %s.\n", file);
    
    fp = filemem_open(file);
    
    if( fp == NULL )
    {
        printf("Failed to open file\n");
        return;
    }
    
    InitObjFileContents(obj, maxVertCount);
    
    // read main file
    read_file(fp, obj);

    filemem_close(fp);
    
    // read material files
    for(int i = 0; i < obj->material_lib.cur_size; ++i)
    {
        printf("Processing material file %s.\n", obj->material_lib.l[i].name);
        
        fp = filemem_open(obj->material_lib.l[i].name);
        
        if( fp == NULL )
        {
            printf("Failed to open material file %s\n", obj->material_lib.l[i].name);
            continue;
        }
        
        read_file(fp, obj);
        
        filemem_close(fp);
    }
    FinishObjFile(obj);    
}

int main(int argc, const char * argv[])
{
    bool test = true;//false; // TODO - reset to default
    char* path = "/Volumes/User/Users/ibullard/projects/thirdparty/OBJFile-OpenGL/OpenGL BOBJ Viewer/Models";//NULL; // TODO - reset to default
    char* file = "plane3.obj";//NULL;
    char* output = "plane3.bobj"; //NULL;
    int maxVerts = 65536;
    bool report = true;
    
    // process arguments
    for(int a = 1; a < argc; ++a)
    {
        if(strcmp(argv[a], "-t") == 0 ||
           strcmp(argv[a], "--test") == 0)
        {
            test = true;
        }
        
        if(strcmp(argv[a], "-p") == 0 ||
           strcmp(argv[a], "--path") == 0)
        {
            path = malloc(strlen(argv[a+1])+1);
            strcpy(path, argv[a]);
            ++a;
        }
        
        if(strcmp(argv[a], "-i") == 0 ||
           strcmp(argv[a], "--input") == 0)
        {
            file = malloc(strlen(argv[a+1])+1);
            strcpy(file, argv[a]);
            ++a;
        }

        if(strcmp(argv[a], "-o") == 0 ||
           strcmp(argv[a], "--output") == 0)
        {
            output = malloc(strlen(argv[a+1])+1);
            strcpy(output, argv[a]);
            ++a;
        }
        
        if(strcmp(argv[a], "-q") == 0 ||
           strcmp(argv[a], "--force-quads") == 0)
        {
            maxVerts = 4;
        }
        
        if(strcmp(argv[a], "-f") == 0 ||
           strcmp(argv[a], "--force-tris") == 0)
        {
            maxVerts = 3;
        }
        
        if(strcmp(argv[a], "-r") == 0 ||
           strcmp(argv[a], "--report") == 0)
        {
            report = true;
        }
    }
    
    if(path == NULL)
    {
        path = malloc(4096);
        if (getcwd(path, 4096) == NULL)
        {
            printf("Could not get local path, try using -p <path> to pass one in");
            goto exit;
        }
    }
    
    if(file == NULL)
    {
        printf("No file, use -f <file> to pass one in");
        goto exit;
    }
    
    if(output == NULL)
    {
        printf("No output, use -o <file> to pass one in");
        goto exit;
    }
    
    chdir(path);

    char temp[4096];
    if (getcwd(temp, 4096) == NULL)
    {
        goto exit;
    }
    
    if(test)
    {
        if(!unit_test())
        {
            printf("Unit tests failed, exiting...\n");
            return 255;
        }
        printf("Unit tests passed.\n");
    }
    

    ObjFileContents obj;
    read_obj_file(file, maxVerts, &obj);

    if(report)
    {
        for(int i = 0; i < obj.mesh.cur_size; ++i)
        {
            int faces;
            if( (i + 1) < obj.mesh.cur_size )
            {
                faces = obj.mesh.l[i+1].start - obj.mesh.l[i].start;
            }
            else
            {
                faces = obj.face.cur_size - obj.mesh.l[i].start;
            }
            
            printf("Mesh #%d:\n", i+1);
            printf("    Object: %s\n", obj.mesh.l[i].object);
            printf("    Group: %s\n", obj.mesh.l[i].group);
            printf("    Material: %s\n", obj.mesh.l[i].material);
            printf("    Num Faces: %d\n\n", faces);
        }
    }

    OpenGL_OBJ* outData;
    int outputSize;
    
    // finish out the processing
    postprocess(&obj, &outData, &outputSize);
    
    // save the file
    FILE* fout = fopen(output, "wb");
    
    if(fout != NULL)
    {
        fwrite(outData, outputSize, 1, fout);
        fclose(fout);
    }
    else
    {
        printf("Failed to open output file.\n");
    }
    
exit:
    // clean up
    // TODO: free path
    //       free file

    return 0;
}

