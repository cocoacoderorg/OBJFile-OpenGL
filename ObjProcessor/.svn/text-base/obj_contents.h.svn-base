//
//  obj_contents.h
//  ObjProcessor
//
//  Created by Ian Bullard on 12/29/12.
//  Copyright (c) 2012 Ian Bullard. All rights reserved.
//

#ifndef ObjProcessor_obj_contents_h
#define ObjProcessor_obj_contents_h

#include "vector.h"
#include "expanding_list.h"
#include "constants.h"
#include "objtypes.h"

DEFINE_LIST(Vector, vector_list);
DEFINE_LIST(FaceType, face_list);
DEFINE_LIST(NameType, name_list);
DEFINE_LIST(MeshType, mesh_list);
DEFINE_LIST(MaterialType, material_list);
DEFINE_LIST(TextureType, texture_list);

typedef struct
{
    vector_list vertex;
    vector_list texture_coord;
    vector_list normal;

    face_list   face;
    
    name_list   material_lib;
    
    mesh_list   mesh;
    
    material_list material;
    
    // state machine to track groups
    LineType    last_line; // tracks verts, faces and materials (breaks in meshes)
    MeshType    cur_mesh;
    MaterialType cur_material;
    bool        first_material;
    
    texture_list    texture;
    
    // options
    int        max_verts;
} ObjFileContents;

void InitObjFileContents(ObjFileContents* file, int maxVertCount);
void FinishObjFile(ObjFileContents* file);

void EmitUnknown(ObjFileContents* file, int line_num, char* line);
void EmitEmpty(ObjFileContents* file, int line_num, char* line);
void EmitComment(ObjFileContents* file, int line_num, char* line);
void EmitVertex(ObjFileContents* file, int line_num, char* line);
void EmitTexCoord(ObjFileContents* file, int line_num, char* line);
void EmitNormal(ObjFileContents* file, int line_num, char* line);
void EmitVertParam(ObjFileContents* file, int line_num, char* line);
void EmitFace(ObjFileContents* file, int line_num, char* line);
void EmitMaterialLib(ObjFileContents* file, int line_num, char* line);
void EmitUseMaterial(ObjFileContents* file, int line_num, char* line);
void EmitObject(ObjFileContents* file, int line_num, char* line);
void EmitGroup(ObjFileContents* file, int line_num, char* line);
void EmitSmoothingGroup(ObjFileContents* file, int line_num, char* line);

// Material
void EmitMatNew(ObjFileContents* file, int line_num, char* line);
void EmitMatAmbient(ObjFileContents* file, int line_num, char* line);
void EmitMatDiffuse(ObjFileContents* file, int line_num, char* line);
void EmitMatSpecularColor(ObjFileContents* file, int line_num, char* line);
void EmitMatSpecularCoeff(ObjFileContents* file, int line_num, char* line);
void EmitMatTransparent(ObjFileContents* file, int line_num, char* line);
void EmitMatIllumination(ObjFileContents* file, int line_num, char* line);
void EmitMatRefraction(ObjFileContents* file, int line_num, char* line);
void EmitMatTransmissionFilter(ObjFileContents* file, int line_num, char* line);
void EmitMatTexAmbient(ObjFileContents* file, int line_num, char* line);
void EmitMatTexDiffuse(ObjFileContents* file, int line_num, char* line);
void EmitMatTexSpecularColor(ObjFileContents* file, int line_num, char* line);
void EmitMatTexSpecularCoeff(ObjFileContents* file, int line_num, char* line);
void EmitMatTexTransparent(ObjFileContents* file, int line_num, char* line);
void EmitMatTexIllumination(ObjFileContents* file, int line_num, char* line);
void EmitMatTexBump(ObjFileContents* file, int line_num, char* line);
void EmitMatTexDisplace(ObjFileContents* file, int line_num, char* line);
void EmitMatTexDecal(ObjFileContents* file, int line_num, char* line);
void EmitMatTexReflection(ObjFileContents* file, int line_num, char* line);

#endif
