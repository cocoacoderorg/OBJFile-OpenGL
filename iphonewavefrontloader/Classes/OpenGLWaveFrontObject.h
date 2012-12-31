//
//  OpenGLWaveFrontObject.h
//  Wavefront OBJ Loader
//
//  Created by Jeff LaMarche on 12/14/08.
//  Copyright 2008 Jeff LaMarche. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import "OpengLWaveFrontCommon.h"
#import "OpenGLWaveFrontMaterial.h"

#include "obj_opengl.h"


// This line should be uncommented to use the famous Quake / invsqrt optimization
// If this line is commented out, normalization will happen using standard sqrtf()
#define USE_FAST_NORMALIZE 

@interface OpenGLWaveFrontObject : NSObject {
    
    int             numMesh;
    
    int*            vertCount;
    GLuint*         vertBuffer;
    NSMutableArray* materials;
    NSMutableArray* textures;
    
	Vertex3D			currentPosition;
	Rotation3D			currentRotation;
}
@property (nonatomic, retain) NSDictionary *materials;
@property Vertex3D currentPosition;
@property Rotation3D currentRotation;
- (id)initWithPath:(NSString *)path;
- (void)drawSelf;
@end
