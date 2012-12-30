//
//  postprocess.c
//  ObjProcessor
//
//  Created by Ian Bullard on 12/30/12.
//  Copyright (c) 2012 Ian Bullard. All rights reserved.
//

#include "postprocess.h"


typedef struct MemAlloc
{
    char* mem;
    unsigned int top;
    unsigned int size;
};

DEFINE_LIST(OpenGL_Mesh, oglmesh_list);



void create_mesh(int i, ObjFileContents* obj, oglmesh_list* list)
{
    int start = obj->mesh.l[i].start;
    int end = obj->face.cur_size;
    
    if( i < obj->mesh.cur_size-1 )
    {
        end = obj->mesh.l[i].start;
    }
    
    for(int face = start; face < end; ++face)
    {
        // Assumptions: vertex winding is consistant
        //              all polygons are planar
        //              all polygons are convex
    }
}


void postprocess(ObjFileContents* obj, OpenGL_OBJ** final, int* size)
{
    // Create meshes from polygons
    oglmesh_list* meshes = malloc(sizeof(oglmesh_list) * obj->mesh.cur_size);

    for(int i = 0; i < obj->mesh.cur_size; ++i)
    {
        create_mesh(i, obj, &(meshes[i]));
    }
    
    // Create Materials from mats
    
    // Convert names to indices
    
    // Calculate memory block size
    
    // Allocate from memory block
    
    // Free temp allocs
    
    // Return results
}
