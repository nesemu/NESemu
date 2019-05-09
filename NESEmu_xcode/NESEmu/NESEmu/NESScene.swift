//
//  NESScene.swift
//  NESEmu
//
//  Created by Kyle Miller on 5/8/19.
//  Copyright © 2019 NESEmu. All rights reserved.
//

import SpriteKit

class NESScene: SKScene {
    private let node: SKSpriteNode
    
    static let screenSize = CGSize(width: 256, height: 240)
    
    override init(size: CGSize) {
        node = SKSpriteNode()
        node.size = size
        node.yScale = -1
        
        super.init(size: size)
        self.scaleMode = SKSceneScaleMode.aspectFit
        self.anchorPoint = CGPoint(x: 0.5, y: 0.5)
        
        isPaused = true
        
        addChild(node)
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    override func update(_ currentTime: TimeInterval) {
        let texture = NESRun()
        node.texture = texture
    }
    
    func NESRun() -> SKTexture {
        var imageReady : Bool = false
        var generatedImage : Bool = false
        var texture : SKTexture = SKTexture(imageNamed: "nes")
        if (NESSystem.ROMPath == "") {
            return texture
        }
        while (!generatedImage) {
            NESSystem.cpucounter += NESSystem.shared.cpu.step()
            while (NESSystem.ppucounter < NESSystem.cpucounter*3) {
                imageReady = NESSystem.shared.ppu.step()
                if (imageReady) {
                    generatedImage = true
                    let data = Data(bytesNoCopy: NESSystem.shared.ppu.getFramebuffer(), count: NESSystem.screenx*NESSystem.screeny*32, deallocator: Data.Deallocator.none)
                    texture = SKTexture(data: data, size: CGSize(width: 256, height: 240), rowLength: 256, alignment: 0)
                    texture.filteringMode = .nearest
                }
                NESSystem.ppucounter += 1
            }
        }
        return texture
    }
}
