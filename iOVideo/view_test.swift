//
//  view_test.swift
//  OliviaVideo
//
//  Created by Hao Xu on 15/2/19.
//  Copyright (c) 2015年 xuhao. All rights reserved.
//

import Foundation
import Cocoa

class Draw2D: NSView {

    
    // Only override drawRect: if you perform custom drawing.
    // An empty implementation adversely affects performance during animation.
    var cap_func :(()-> NSImage)?;
    var k :Int = 0
    
    override func drawRect(rect: CGRect) {
        if let img = cap_func
        {
            let myImage = img()
            myImage.drawInRect(self.bounds)
        }
    }
    
    func update_frame(timer:NSTimer)
    {
        self.needsDisplay = true
    }
    
    var timer:NSTimer?
    
    func setCapturefunc(f:()->NSImage)
    {
        cap_func = f;
        //timer = NSTimer(timeInterval: 0.033, target: self, selector: "update_frame:", userInfo: nil, repeats:true);
        timer = NSTimer.scheduledTimerWithTimeInterval(0.033, target: self, selector: "update_frame:", userInfo: nil, repeats:true);
        timer?.fire()
    }
    
}