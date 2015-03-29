//
//  brigde_core.h
//  OliviaVideo
//
//  Created by Hao Xu on 15/1/9.
//  Copyright (c) 2015年 xuhao. All rights reserved.
//

#ifndef __OliviaVideo__brigde_core__
#define __OliviaVideo__brigde_core__

#include <stdio.h>
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

@interface rec_core : NSObject
{
    int width,height;
}
    - (char *) getCurrent;
    - (id) init :(int) port;
    - (NSImage *) getCurrentImg;
@end
#endif /* defined(__OliviaVideo__brigde_core__) */
