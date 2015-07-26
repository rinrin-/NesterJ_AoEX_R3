
Unofficial nester,
patched version to support added mappers and features
=====================================================


*** PLEASE SEND ME TECH INFO ABOUT NON SUPPORTED MAPPERS !!! ***


nester(c) is NES Emulator created by Mr. Darren Ranalli.
i think, the number of supported mapper is not enough,
but it works very good. :-)


this is unofficial nester (c) pathed version added support
some mappers and expand controllers.

and this archive includes patched source files.

if you wanna try source files, please get original source file
at Mr. Darren Ranalli's home page (http://nester.homepage.com/),
over write source files included this archive, and compile it.
(* i use Visual C++ 6.0 Prof.Ed. Japanese Version.)

patched source files are based on "public beta 3"

the newest version is always uploaded at:
http://www1.interq.or.jp/~t-takeda/bin/nester_mapper_support.zip

----------

mappers added by this patch are (see also mappers.txt):

  MMC           5, 10, 47
  FFE           6, 8, 17
  ColorDream    11, 46
  Others        13, 100, 118, 240, 242, 246
  Nin1          15, 41, 44, 45, 49,225, 226, 227, 228, 229, 230, 231,
                232, 233, 234**, 235, 241, 255
  Bandai        16, 66, 70, 96, 188
  Jaleco        18, 72, 75, 78, 86, 92, 101
  Namco         19*, 76, 88, 95
  DiskSystem    20
  Konami        21, 22, 23, 24, 25, 26, 73, 85, 87
  Irem          32, 65, 77, 97
  Taito         33, 48, 80, 82
  Nina          34, 79
  Pirates       42, (43), 50, 90, 91**, 112, 113, (114), 117, 160, 182,
                (187), 189*, 243
  Tengen        64
  Sunsoft       67, 68, 69, 89, 93, 122, 184
  Camerica      71
  Capcom        94
  VS Unisystem  99, 151
  Nichibutsu    180
  Tecmo         185

  NSF           NESAudioRippingData
  PCT           PlayChoice-10 ROM Image

  ?  not tested.
  *  it has problem that sometimes screen is not correct.
  ** it has serious problem that ALWAYS screen is not correct.
  () not work.

----------

mapper note:

  mapper 100 Nesticle MMC3

  mapper 230 switch 'Contra' and '22 Games' in every reset.

  mapper 255 some games not work.


----------

about Sound Setting

  * support change sample bits (8/16 bits).

  * support change triangle wave setting (use ideal triangle or not).


----------

about Nintendo Disk System (mapper #20)

  * need 'DISKSYS.ROM' in the same directory of nester.exe
    support both pure BIOS ROM image and .NES format ROM image

  * support NES, FDS, and FAM format disk images.

  * support save datas writed to disk, saved as "*.sdk"

  * support ExtraSound.

  * shortcut key to change disk:

    'A' ... change disk 1st-sideA
    'B' ... change disk 1st-sideB
    'C' ... change disk 2nd-sideA
    'D' ... change disk 2nd-sideB
    'E' ... eject inserted disk

   also you can change disk in menu.


----------

about VS Unisystem (mapper #99, #151)

  * push 'm' to insert coin.

  * not support dip switch.

  * VS Unisystem PPUs have unique palettes, so some games output wrog colors.
    games supported auto palette change are:

    #0  .... 'Slalom'
    #1  .... 'Dr.Mario'
    #2  .... 'CastleVania'
    #4  .... 'Atari RBI Baseball', 'Super Sky Kid', 'TKO Boxing'
    #68 .... 'Platoon'
    #99 .... 'Battle City', 'Clu Clu Land', 'Excite Bike', 'Golf',
             'Hogan's Alley', 'Ice Climber', 'Ladies Golf', 'Mach Ride',
             'Pin Ball', 'Soccer', 'Super Mario Brothers'
    #151 ... 'Goonies', 'Gradius'

  * VS Unisystem flag bit in NES header must be set.


----------

about PlayChoice-10

  * load *.pct file included in this archive.

  * rom image files must be in the same directory of *.pct files.

  * you must rename rom image files with pc10ren.exe, it searches all files
    in the current directory and copy to 'pc10\' as the fixed name.

  * supported games are:

    '1942', 'Balloon Fight', 'Baseball', 'Captain Sky Hawk', 'Castlevania',
    'Contra', 'Double Dragon', 'Dr Mario', 'Duck Hunt', 'Excitebike',
    'Fester's Quest', 'Gauntlet', 'Golf', 'Gradius', 'Hogan's Alley',
    'Kung Fu', 'Mario Open Golf', 'Mega Man 3', 'Mike Tyson's Punch-Out!!',
    'Ninja Gaiden', 'Ninja Gaiden 3 - The Ancient Ship of Doom',
    'Nintendo World Cup', 'Power Blade', 'Pro Wrestling', 'Rad Racer',
    'Rad Racer 2', 'RC Pro-Am', 'Rockin' Kats', 'Rush'n Attack', 'Rygar',
    'Super C', 'Super Mario Bros', 'Super Mario Bros 2', 'Super Mario Bros 3',
    'Teenage Mutant Ninja Turtles', 'Teenage Mutant Ninja Turtles 2',
    'Tennis', 'The Goonies', 'Track & Field', 'Trojan', 'Volleyball',
    'Wild Gunman', 'Yo! noid'

  (PlayChoice-10 support is based on JoinPC10, thanx D-Tox Software)


----------

about NSF Player

  * push start button to play next song.

  * not support Jaleco 86 ExtraSound.

  (i used 6502 ASM Player source code, thanx Chris Covell)


----------

about network play

  see "netplay.txt" in this archive.


----------

about Rec/Play Movie

  * movie file is normaly saved as 'movie\(ROM name).mov'.
    (you can change file name)

  * control start rec/play or stop in menu "FILE" - "Movie Rec/Play".

  * you can stop playing and replay the game at the part of movie.

  * not support some games that state save is incorrect (e.g. DiskSystem)
    since movie file include state save.
    and also you may not be able to use the old movie file if state file
    format is changed.


----------

about Game Genie Codes

  * support both 6 and 8 characters' codes.

  * support MAX 255 codes for each games.

  * nester searches text file 'genie\(ROM Name).gen' when load ROM.
    write codes at the top of line in the text file.

  * see the sample codes files in the directory 'genie\'


----------

about External Devices

  * MIC device on 2P Controller

       push 'm' to input voice.


  * VS Unisystem

       push 'm' to insert coin.


  * Arkanoid Paddle

       Paddle ... Mouse Cursor (move left or right)
       Button ... Mouse Left Crick
       Select ... Mouse Right Crick

       disenable Arkanoid Paddle with menu if you want to use joypad.


  * Clazy Climber Controller

             up                up                   [w]            [y]
              |                 |                    |              |
       left --+-- right  left --+-- right  ->  [a] --+-- [d]  [g] --+-- [j]
              |                 |                    |              |
            down              down                  [s]            [h]


  * Datach Barcode Battler

       support JAN (normal/short) code.

       input 8 or 13 digits in the menu
         "Options" - "Control Device" - "Datach Barcode Battler".


  * Doremikko Keyboard

        +-+-+ +-+-+-+ +-+-+ +-+-+-+ +-+-+ +-+-+-+
        |B|B| |B|B|B| |B|B| |B|B|B| |B|B| |B|B|B|    Black Key
       +++++++++++++++++++++++++++++++++++++++++++
       | | | | | | | | | | | | | | | | | | | | | |
       |W|W|W|W|W|W|W|W|W|W|W|W|W|W|W|W|W|W|W|W|W|   White Key
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

                           |

        +-+-+ +-+-+-+ +-+-+ +-+-+-+ +-+-+ +-+-+-+
        |S|D| |G|H|J| |L|;| |2|3|4| |6|7| |9|0|-|
       +++++++++++++++++++++++++++++++++++++++++++
       | | | | | | | | | | | | | | | | | | | | | |
       |Z|X|C|V|B|N|M|,|.|/|Q|W|E|R|T|Y|U|I|O|P|@|
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

       disable all shortcut keys(e.g. F5 to state save) with [BackSpace]
        (note: also disable all menu items)
       and enable them with hitting [BackSpace] again.


  * Exciting Boxing Controller

       [left HOOK] [left JABB] [rightJABB] [rightHOOK]

       [left MOVE] [   BODY  ] [ STRAIGHT] [rightMOVE]

                              |

       [    q    ] [    w    ] [    e    ] [    r    ]

       [    a    ] [    s    ] [    d    ] [    f    ]


  * Family Basic Keyboard with Data Recorder

     Family Basic Keyboard:

       [STOP]         ->  [End]
       [CLR/HOME]     ->  [Home]
       [GRAPH](down)  ->  [PageDown]
              ( up )  ->  [PageUp]
       [KANA]         ->  [F12]
       other keys     ->  same keys

       disable all shortcut keys(e.g. F5 to state save) with [BackSpace]
        (note: also disable all menu items)
       and enable them with hitting [BackSpace] again.

     Data Recorder:

       control it at the menu 'Options' - 'Control Device' - 'Data Recorder'

       data recorder file is normaly saved as 'tape\(ROM name).tpr'.
        (you can change file name)

       start 'Rec Recorder' before start 'SAVE'
        (note: you must stop recording when SAVE is finished.)

       and start 'Play Recorder' after start 'LOAD'
        (note: stop automaticaly when finished playing whole tape)

       need 27.3 KB per 1 sec to save datas.


  * Family Trainer, Pokkun Moguraa, Power Pad

       [ 1] [ 2] [ 3] [ 4]      [ q] [ w] [ e] [ r]

       [ 5] [ 6] [ 7] [ 8]  ->  [ a] [ s] [ d] [ f]   Start  ... [ t]

       [ 9] [10] [11] [12]      [ z] [ x] [ c] [ v]   Select ... [ g]


  * Hyper Shot (Hyper Olympic)

       Run  ... JoyPad's 'A' button
       Jump ... JoyPad's 'B' button


  * Mahjong Controller (Ide Yousuke Meijin No Jissen Mahjong)

       [A] [B] [C] [D] [E] [F] [G] [H] [I] [J] [K] [L] [M] [N]
             [Start] [Select] [Kan] [Pon] [Chii] [Reach] [Ron]

                                  |

       [q] [w] [e] [r] [t] [y] [u] [a] [s] [d] [f] [g] [h] [j]
             [  z  ] [   x  ] [ c ] [ v ] [  b ] [  n  ] [ m ]


  * Oeka Kids Tablet

       Push Bottun  ... Mouse Left Button
       Pen Position ... Mouse Cursor


  * Optical Gun / VS Unisystem Zapper

       Shoot ... Mouse Left Crick
       Sight ... Mouse Cursor

       note: not support in Full Screen Mode


  * Space Shadow Gun

       Shoot  ... Mouse Left Crick (full auto)
       Sight  ... Mouse Cursor
       Bomb   ... Mouse Right Crick
       Others ... Pad1 (except A,B)

       note: not support in Full Screen Mode


  * Turbo File (External 8KB Strage Device)

       the data is saved as 'tb_file.dat' in the same directry of nester.exe


  * the games that controller is selected automaticaly are:

    * Arkanoid (J)
    * Arkanoid 2

    * Crazy Climber

    * Datach - Battle Rush - Build Up Robot Tournament
    * Datach - Crayon Shin Chan - Ora to Poi Poi
    * Datach - Dragon Ball Z - Gekitou Tenkaichi Budou Kai
    * Datach - J League Super Top Players
    * Datach - SD Gundam - Gundam Wars
    * Datach - Ultraman Club - Supokon Fight!
    * Datach - Yuu Yuu Hakusho - Bakutou Ankoku Bujutsu Kai

    * Doremikko

    * Exciting Boxing

    * Family Basic (Ver 1.0)
    * Family Basic (Ver 2.0)
    * Family Basic (Ver 2.1a)
    * Family Basic (Ver 3.0)
    * Family Basic (Ver 3.0) (Alt)

    * Family Trainer - Aerobics Studio
    * Family Trainer - Athletic World
    * Family Trainer - Daiundoukai
    * Family Trainer - Fuuun!! Takeshi Jou 2 (not work)
    * Family Trainer - Jogging Race
    * Family Trainer - Manhattan Police
    * Family Trainer - Meiro Daisakusen
    * Family Trainer - Rairai! Kyounshiizu
    * Family Trainer - Running Stadium
    * Family Trainer - Totsugeki Fuuun Takeshi Jou

    * Hyper Olympic
    * Hyper Olympic Tonosama Version
    * Hyper Sports

    * Ide Yousuke Meijin No Jissen Mahjong
    * Ide Yousuke Meijin No Jissen Mahjong 2

    * Oeka Kids - Anpanman no Hiragana Daisuki
    * Oeka Kids - Anpanman to Oekaki Shiyou!!

    * Baby Boomer
    * Barker Bill's Trick Shooting
    * Chiller
    * Duck Hunt
    * Freedom Force
    * Gotcha!
    * Gumshoe
    * Gun Sight
    * Hogan's Alley
    * Laser Invation
    * Mad City
    * Mechanized Attack
    * Shooting Range
    * The Adventures of Bayou Billy
    * Wild Gunman

    * Pokkun Moguraa

    * Athletic World
    * Dance Aerobics
    * Short Order - Eggsplode (support only Eggsplode)
    * Street Cop

    * Space Shadow

    * Best Play - Pro Yakyuu
    * Best Play - Pro Yakyuu 2
    * Best Play - Pro Yakyuu '90
    * Best Play - Pro Yakyuu Special
    * Castle Excellent
    * Derby Stallion - Zenkoku Ban
    * Downtown - Nekketsu Monogatari
    * Dungeon Kid
    * Fleet Commander
    * Haja No Fuuin
    * Itadaki Street - Watashi no Mise ni Yottette
    * Ninjara Hoi!
    * Wizardry - Proving Grounds of the Mad Overlord (J)
    * Wizardry - Legacy of Llylgamyn (J)
    * Wizardry - The Knight of Diamonds (J)

    * VS Duck Hunt
    * VS Hogan's Alley

----------

Thanks:

thanks to Darren Ranalli, the author of original nester.

Mikami Kana and Screw, the authors of nesterJ.

Mamiya, the author of NEZplug.

Chris Covell, the author of 6502 ASM Player for NSF.

xodnizel, the author of FCE Ultra.

mm-mm and Quietust, the authors of G-NES / NESten mapper plug in.

rinao always give me good advice for mapper error.

stun and YANO give me the fixed sound codes.

GIGO(G-NES) and Nori(Famtasia) give me many advices and codes.

Buddha, Kinoshita, Marimo, MUMU, Nazo-X, nez nez, parm, Takecchi,
Takeuchi, Scott Wu(swNES), and all nesdev members for tech-info.

D-Tox Software, the author of JoinPC10.

Mr.navarone gave me the message board for development in his website.

and thanks to everyone who try unofficial nester.


----------

Contact:

TAKEDA, toshiya
t-takeda@m1.interq.or.jp

