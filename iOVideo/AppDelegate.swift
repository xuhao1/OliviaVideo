//
//  AppDelegate.swift
//  iOVideo
//
//  Created by Hao Xu on 15/1/10.
//  Copyright (c) 2015年 xuhao. All rights reserved.
//

import Cocoa

@NSApplicationMain
class AppDelegate: NSObject, NSApplicationDelegate {

    @IBOutlet weak var window: NSWindow!

    @IBOutlet weak var video_view: Draw2D!
    @IBOutlet weak var port_num: NSTextField!
    @IBAction func start_listen(sender: AnyObject) {
        init_core()
    }
    var listen_thread : rec_core?;
    func init_core()
    {
        println("here\n")
        let text = port_num.stringValue as String
        let port : NSNumber = text.toInt()!
        listen_thread = rec_core(port.intValue)
        video_view?.setCapturefunc(){
            return (self.listen_thread?.getCurrentImg())!
            }
    }
    func applicationDidFinishLaunching(aNotification: NSNotification) {
        port_num.stringValue = "7890"
    }

    func applicationWillTerminate(aNotification: NSNotification) {
        // Insert code here to tear down your application
    }


}

