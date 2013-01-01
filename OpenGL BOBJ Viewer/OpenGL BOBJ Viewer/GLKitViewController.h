//
//  GLKitViewController.h
//  OpenGL BOBJ Viewer
//
//  Created by Ian Bullard on 12/31/12.
//  Copyright (c) 2012 Ian Bullard. All rights reserved.
//

#import <GLKit/GLKit.h>

#import "BOBJObject.h"

@interface GLKitViewController : GLKViewController {
    BOBJObject* obj;
    
    GLKVector3 curRot;
    
    bool readToDraw;
}
@end
