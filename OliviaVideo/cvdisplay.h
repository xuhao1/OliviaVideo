//
//  cvdisplay.h
//  OliviaVideo
//
//  Created by xuhao on 14/12/5.
//  Copyright (c) 2014年 xuhao. All rights reserved.
//

#ifndef __OliviaVideo__cvdisplay__
#define __OliviaVideo__cvdisplay__

#include <stdio.h>

typedef unsigned char uchar;

int cv_show_frame(uchar * data[],int height,int width);

int init_cv();
#endif /* defined(__OliviaVideo__cvdisplay__) */
