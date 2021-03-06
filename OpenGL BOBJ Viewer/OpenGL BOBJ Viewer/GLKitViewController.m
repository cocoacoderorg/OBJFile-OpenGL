//
//  GLKitViewController.m
//  OpenGL BOBJ Viewer
//
//  Created by Ian Bullard on 12/31/12.
//  Copyright (c) 2012 Ian Bullard. All rights reserved.
//

#import "GLKitViewController.h"

@interface GLKitViewController () {
}
@property (strong, nonatomic) EAGLContext *context;

@end

@implementation GLKitViewController
@synthesize context = _context;

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    
    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    view.context = self.context;
    
//    NSString *filePath = [[NSBundle mainBundle] pathForResource:@"uvcube2" ofType:@"bobj"];
    NSString *filePath = [[NSBundle mainBundle] pathForResource:@"plane3" ofType:@"bobj"];
//  Matchboxhouse is very, very small.  You need to change the near plane to 0.01, the far plane to 2.0
//      and the object offset to -0.5
//    NSString *filePath = [[NSBundle mainBundle] pathForResource:@"Matchboxhouse" ofType:@"bobj"];
    

    [EAGLContext setCurrentContext:self.context];
    obj = [[BOBJObject alloc] initWithPath:filePath];
    
    curRot = GLKVector3Make(0.0f, 0.0f, 0.0f);
    
    readToDraw = true;
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
    self.context = nil;
}

#pragma mark - GLKViewDelegate

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect {

    if(!readToDraw)
    {
        return;
    }
    
    [EAGLContext setCurrentContext:self.context];
    
    glClearColor(0.0f, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    GLKMatrix4 proj;
    
    proj = GLKMatrix4MakePerspective(90, self.view.bounds.size.width/self.view.bounds.size.height, 0.1f, 1000.0f);
    obj.currentPosition = GLKVector3Make(0, 0, -3);
    
    curRot.y += 0.1f;
    
    obj.currentRotation = curRot;
    
    [obj drawSelf:proj];
}

#pragma mark - GLKViewControllerDelegate

- (void)update {
}

@end
