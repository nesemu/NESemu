//
//  AppDelegate.swift
//  NESEmu
//
//  Created by Ethan Williams on 5/7/19.
//  Copyright © 2019 NESEmu. All rights reserved.
//


import Cocoa
import NESKit

@NSApplicationMain
class AppDelegate: NSObject, NSApplicationDelegate {
    
    
    func applicationDidFinishLaunching(_ aNotification: Notification) {
        // Insert code here to initialize your application
        
        let dialog = NSOpenPanel();
        dialog.title                   = "Choose a rom file";
        dialog.showsResizeIndicator    = true;
        dialog.showsHiddenFiles        = false;
        dialog.canChooseDirectories    = true;
        dialog.canCreateDirectories    = true;
        dialog.allowsMultipleSelection = false;
        dialog.allowedFileTypes        = ["nes"];
        
        dialog.begin { (result) in
            guard result == NSApplication.ModalResponse.OK, dialog.urls.isEmpty == false, let url = dialog.urls.first else {
                print("Error")
                return
            }
            print(url)
            print(url.path)
            let gamepak = NESKitGamepak.init(file: url.path);
            gamepak.powerup();
            let joypad0 = NESKitInputDevice.init(num: 0);
            let joypad1 = NESKitInputDevice.init(num: 1);
            let ppu = NESKitPPU.init(gamepak: gamepak)
            let cpuMemory = NESKitMemory.init(ppu: ppu, gamepak: gamepak, inputDevice1: joypad0, inputDevice2: joypad1)
            let cpu = NESKitCPU.init(memory: cpuMemory);
            
            ppu.assign_cpu(cpu);
            cpu.powerup();
            ppu.powerup();
        }
        
    }
    
    func applicationWillTerminate(_ aNotification: Notification) {
        // Insert code here to tear down your application
    }
    
}
