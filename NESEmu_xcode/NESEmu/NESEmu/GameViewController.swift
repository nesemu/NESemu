//
//  GameViewController.swift
//  NESEmu
//
//  Created by Ethan Williams on 5/8/19.
//  Copyright © 2019 NESEmu. All rights reserved.
//

import Cocoa
import SpriteKit

class GameViewController: NSViewController {

    override func viewDidLoad() {
        super.viewDidLoad()
        let skView = view as! SKView
        let scene = NESScene(size: view.bounds.size)
        skView.showsFPS = true
        skView.showsNodeCount = true
        skView.ignoresSiblingOrder = true
        skView.preferredFramesPerSecond = 60
        scene.scaleMode = .resizeFill
        skView.presentScene(scene)
    }
    
}
