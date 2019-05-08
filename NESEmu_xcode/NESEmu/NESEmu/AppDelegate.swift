//
//  AppDelegate.swift
//  NESEmu
//
//  Created by Ethan Williams on 5/7/19.
//  Copyright © 2019 NESEmu. All rights reserved.
//


import Cocoa
import NESKit

struct NESSystem {
    static var filename = "/Users/kyle/NESemu/ROMS/Super_Mario_Bros.nes"
    static var gamepack = NESKitGamepak(file: filename)
    static var joypad1 = NESKitInputDevice(num: -1)
    static var joypad2 = NESKitInputDevice(num: -2)
    static var ppu = NESKitPPU(gamepak: gamepack)
    static var memory = NESKitMemory(ppu: ppu, gamepak: gamepack, inputDevice1: joypad1, inputDevice2: joypad2)
    static var cpu = NESKitCPU(memory: memory)
}

@NSApplicationMain
class AppDelegate: NSObject, NSApplicationDelegate {
    
    func applicationDidFinishLaunching(_ aNotification: Notification) {
        NESSystem.gamepack.powerup();
        NESSystem.ppu.assign_cpu(NESSystem.cpu)
        NESSystem.cpu.powerup()
        NESSystem.ppu.powerup()
    }
    
    func applicationWillTerminate(_ aNotification: Notification) {
        // Insert code here to tear down your application
    }
    
    
}
