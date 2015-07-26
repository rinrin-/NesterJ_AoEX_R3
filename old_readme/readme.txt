NesterJ AoEX R3
2nd June 2012
By ShimaFlareX
shimaflare.wordpress.com
shimaflarex at gmail dot com



Based on NesterJ 1.12 Plus 0.61 RM, so it includes features like rewind mode, cheat codes support, rotated/mirrored screen, sepia palette, support to rare mappers (the pirate bootleg FF7 works on it), etc. See NesterJ 1.12 Plus 0.61 RM changelog(included in old_readme folder) for more info.

Changelog:
R3:
+Fixed some crashes (i.e. Mario Bros Classic (E) is fixed now, possibly other games too)
+Fixed minor cosmetic issues on rewind submenu

R2:
+Changed the menu CPU clock to 200Mhz
+Fixed: CPU Modes 211Mhz and 200Mhz settings weren't being properly loaded from config file.
+Removed Wifi option from menu (it doesn't work).
+Added slim extra RAM support. At the default save rate, you can rewind more than one and a half minute if you are using a custom firmware and a PSP that supports it.
+You can now see the number of maximum rewind states on the REWIND CONFIG menu.
+Minor code optimization.


R1:
+Improved vsync speed a LOT.
And I really mean it. There’s no noticeable speed difference when vsync is enabled now, while the original code took a 50% speed hit…
As result, VSync is now enabled by default.

+Added new VSYNC option (SOMETIMES). This only synchronizes when there’s no frame drops occurring. I did this when I was still using the original Vsync code. As there’s no noticeable speed hit with vsync always on, this isn’t the default option.

+Added a new option Battery Power Save. Default is ON.
The original NesterJ code used a loop to determine when the next cycle should be emulated.
Enabling this options attempts to sleep the thread when it`s still too early to execute the next cycle. 
There shouldn't be any speed decrease with this, so you should always keep it enabled (unless you want to compare the CPU Usage using PSP-HUD or any similar plugin).
I don’t know if ruka did the same thing I did on his latest version, as he didn’t distribute the source.

Some test results:
ROM:Pogo Cats (Public Domain)
Screen Mode GPU 4:3, test taken on “PUSH START” screen. The FPS was always at NTSC maximum (60FPS).

CPU @ 222Mhz:
Battery Power Save Off: CPU Usage 80%
Battery Power Save On: CPU Usage 74%

At higher clock frequencies, the CPU Usage saving is more noticeable:
CPU @ 333Mhz:
Battery Power Save Off: CPU Usage 87%
Battery Power Save On: CPU Usage 52%

+Changed some default settings: Show FPS is now on, and the default screen mode is "GPU 4:3".

+Added new CPU Speed modes: 211Mhz and 200Mhz

-Removed Chinese language code: Whoever coded this duplicated the entire menu code changing all strings to Chinese. This results in a harder to maintain menu code (you need to change everything twice…). Also, bloated code is bloated.


Legal Stuff:
The author shall not be held responsible for any damage or loss caused by this software. Use it at your own risk. NesterJ AoEX may only be used with ROMs that are legally owned by the user. The author does not condone piracy. NesterJ AoEX may NOT be distributed with ROMs of any kind. All of these conditions also apply to any
derivatives of NesterJ AoEX.
You may distribute modified binaries compiled from the source code or a derivation thereof, as long as you agree with the license terms on readme_nester.txt located in the old_readme folder.