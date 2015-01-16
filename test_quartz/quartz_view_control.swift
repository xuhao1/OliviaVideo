//
//  quartz_view_control.swift
//  OliviaVideo
//
//  Created by Hao Xu on 15/1/11.
//  Copyright (c) 2015年 xuhao. All rights reserved.
//

import Foundation
import Cocoa
import Quartz

class quartz_view : QCView
{
    override func drawRect(dirtyRect: NSRect) {
        var context = NSGraphicsContext.currentContext()?.CGContext
        CGContextSetRGBFillColor (context, 1, 0, 0, 1);
        CGContextFillRect(context, CGRectMake(0, 0, 200, 100));
        CGContextSetRGBFillColor (context, 0, 0, 1, 0.5);// 5
        CGContextFillRect (context, CGRectMake (0, 0, 100, 200));
        
        CGContextSetLineWidth(context, 3.0);
        CGContextSetRGBStrokeColor(context, 1.0, 0.0, 0.0, 1.0);
        CGContextStrokeRect(context, CGContextGetClipBoundingBox(context));
    }
    
    override init(frame frameRect: NSRect) {
        super.init(frame: frameRect)
    }

    required init?(coder: NSCoder) {
        super.init(coder: coder)
        init_color()
    }
    
    func init_color()
    {
        setEraseColor(NSColor(calibratedRed: 0.8, green: 0.5, blue: 0.5, alpha: 1))
    }

}