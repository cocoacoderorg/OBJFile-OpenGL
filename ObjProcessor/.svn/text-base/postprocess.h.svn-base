//
//  postprocess.h
//  ObjProcessor
//
//  Created by Ian Bullard on 12/30/12.
//  Copyright (c) 2012 Ian Bullard. All rights reserved.
//

#ifndef ObjProcessor_postprocess_h
#define ObjProcessor_postprocess_h

#include "obj_contents.h"
#include "obj_opengl.h"

// Takes the OBJ file contents in obj and packs it into one contiguous
//  chunk of memory starting at *final with a size of size bytes
//  The contents of final point to memory inside the chunk,
//  they need to be converted to offsets before writing to a file
void postprocess(ObjFileContents* obj, OpenGL_OBJ** final, int* size);

#endif
