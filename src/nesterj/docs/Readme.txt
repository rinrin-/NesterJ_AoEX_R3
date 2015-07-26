========
 nester
========

a NES emulator by Darren Ranalli

http://nester.homepage.com/


Versions
========

2000.11.03 - public beta 3
 - fixed bug in joystick button input settings

2000.10.13 - public beta 2
  - major bug fix: SRAM now saved on reset (thanks to TAKEDA, toshiya)

2000.08.17 - public beta


History
=======

I played Nintendo as a kid. Lots.
I grew up, and I didn't play Nintendo so much.
In 1997, I downloaded Nesticle.
I played Nesticle in 1997. Lots.
Then I wanted a decent NES emulator for Windows.
So I started writing one in early 1999,
And here it is. <@:)


Controls
========

nester supports DirectInput-compatible keyboards and joysticks.

Here are the default controls:

d-pad  = arrow keys
Select = A
Start  = S
B btn  = Z
A btn  = X

hold '+' (on the num pad) to disable speed throttling
Ctrl+D toggles double-size in windowed mode
Alt+Enter toggles between windowed mode and full-screen mode
Esc exits full-screen mode
F5 saves the game
F7 loads a saved game
number keys 0-9 select a savestate "slot"
F1 displays an About box

** Want to play nester with a real NES pad? **
Check out DirectPad Pro at http://www.ziplabel.com/dpadpro/index.html


Stuff
=====

Requirements:
-------------
Windows 95/98/2000/NT4
DirectX 3.0 or higher
Service Pack 3 or higher required in NT
display card/monitor capable of displaying 256 or more colors
keyboard and/or joystick (DirectX 5 or higher required for joystick)
DirectSound-compatible sound card (recommended)

Supported mappers:
------------------
0,1,2,3,4,7,9,40

Features:
---------

DirectInput support
SNSS savestate support, compatible with Nofrendo, LoopyNES, & others (http://www.nofrendo.org/snss/)
uses nes6502, the fast 6502 core by Matthew Conte
uses the Nosefart/Nofrendo APU core by Matthew Conte
fullscreen and windowed display modes
double-size windowed mode
recent ROMs list
black & white mode (for those of us who had to play our NES on a B&W TV -- what, am I the only one??)
shell integration - associate .NES files with nester! It's fun. Just do it. No, really. DO IT!
small executable - nester is compressed with UPX (http://wildsau.idv.uni-linz.ac.at/mfx/upx.html)

SNSS - Standard NES Save State
------------------------------

nester uses the nifty SNSS savestate format. SNSS allows NES emulators to share savestates, which
is a Good Thing. nester uses the libSNSS library for loading and saving SNSS savestates. All NES
emulator authors are encouraged to use SNSS. To find out more, visit http://www.nofrendo.org/snss/


Thanks
======

I would like to thank the following people:

big, big thanks to Matthew Conte for all his help with everything NES, and for being so dang shady
TAKEDA, toshiya for his work on nester
Jeremy Chadwick (and everyone who contributed information to nestech.txt)
Mike Melanson, for writing the initial versions of libSNSS
loopy, for his PPU info
Kevin Horton, for his palette generation algorithm
nyef (some of the mapper code is based on code from DarcNES)
\Firebug\ for his mappers.txt
sardu, for writing nesticle
FanWen
Marat Fayzullin
the rest of the big happy family that is nesdev
and anyone else I forgot


Contact
=======

nester@popstar.com

PLEASE DO NOT SEND EMAIL ABOUT:
1.where to get game ROMs
2.where to find other emulators
3.any other information that is readily available on the internet

PLEASE DO SEND EMAIL ABOUT:
1.monkeys
2.bugs (i.e., nester froze up when I did blah and blah)
  Please tell me what version of Windows you're running,
  what version of DirectX you have installed, your hardware
  configuration if you know it, what ROM you're trying to
  use, and a detailed description of the problem and how
  to reproduce it.
3.offers of money (or anything else you think I might like :])


Legal crep
==========

The author shall not be held responsible for any damage or loss caused by this
software. Use it at your own risk. nester may only be used with ROMs that are
legally owned by the user. The author does not condone piracy. nester may NOT be
distributed with ROMs of any kind. All of these conditions also apply to any
derivatives of nester. nester was written using 100% legally reverse-engineered
information. Any similarity between the name "nester" and the name of any other
piece of software is purely coincidental. Nintendo, Nintendo Entertainment System,
NES, and Nester are registered trademarks of Nintendo of America. Windows,
Windows 95, Windows 98, Windows 2000, Windows NT, DirectX, DirectSound, and
DirectInput are registered trademarks of Microsoft.

You may distribute modified binaries compiled from the source code or a derivation
thereof, as long as 1) it is visually obvious that the binary is based on nester,
and 2) it is visually obvious that the binary is not an official version of nester.
The recommended procedure is to display "nester (unofficial)" on the title bar
throughout the non-fullscreen lifetime of the program. The lines of code that
accomplish this are clearly labeled in the source (see "version.h"). The intention
of these guidelines is to reduce confusion and to protect nester and the author.

This file must accompany any distribution of source code and/or binaries based on
nester.

Copyright (C) 2000 Darren Ranalli
