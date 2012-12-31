//
//  BOBJObject.h
//  OpenGL BOBJ Viewer
//
//  Created by Ian Bullard on 12/31/12.
//  Copyright (c) 2012 Ian Bullard. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <GLKit/GLKVector3.h>

@interface BOBJObject : NSObject {
    
    int             numMesh;
    
    int*            vertCount;
    GLuint*         vertArray;
    GLuint*         vertBuffer;
    int*            materialIndex;
    NSMutableArray* materials;
    NSMutableArray* textures;

    GLKVector3  currentPosition;
    GLKVector3  currentRotation;
}


@property GLKVector3 currentPosition;
@property GLKVector3 currentRotation;

- (id)initWithPath:(NSString *)path;
- (void)drawSelf:(GLKMatrix4)projection;

@end
