//
//  GameWindowController.swift
//  NESEmu
//
//  Created by Kyle Miller on 5/8/19.
//  Copyright © 2019 NESEmu. All rights reserved.
//

import Cocoa
import SpriteKit

class GameWindowController: NSWindowController, NSWindowDelegate {
    var view: SKView {
        let gameViewController = window!.contentViewController as! ViewController
        return gameViewController.view as! SKView
    }
    
    override func awakeFromNib() {
        super.awakeFromNib()
        
        window?.delegate = self
    }
    
    func applicationShouldTerminateAfterLastWindowClosed(sender: NSApplication) -> Bool {
        return true
    }
}
