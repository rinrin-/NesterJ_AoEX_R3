
// data base to correct the rom image g_ROM.header renamed by nestoy

g_ROM.size_SaveRAM = 8*1024;
if(g_ROM.mapper == 5)
{
	g_ROM.size_SaveRAM = 16*1024;
}

switch(g_ROM.crc){
	// Mapper 000 -----------------------------------------------------------------
case 0x57970078:	// F-1 Race (J)
	g_ROM.ROM_banks[0x3FE1] = 0xFF;
	g_ROM.ROM_banks[0x3FE6] = 0x00;
	break;
case 0xaf2bbcbc:	// Mach Rider (JU)
	g_ROM.ROM_banks[0x7FDF] = 0xFF;
	g_ROM.ROM_banks[0x7FE4] = 0x00;
	break;
case 0xe16bb5fe:   // Zippy Race (J)
	g_ROM.header.flags_1 &= 0xf6;
	break;
	// Mapper 001 -----------------------------------------------------------------
case 0x7831b2ff:	// America Daitouryou Senkyo (J)
case 0x190a3e11:	// Be-Bop-Highschool - Koukousei Gokuraku Densetsu (J)
case 0x52449508:	// Home Run Nighter - Pennant League!! (J)
case 0x0973f714:	// Jangou (J)
case 0x7172f3d4:	// Kabushiki Doujou (J)
case 0xa5781280:	// Kujaku Ou 2 (J)
case 0x8ce9c87b:	// Money Game, The (J)
case 0xec47296d:	// Morita Kazuo no Shougi (J)
case 0xcee5857b:	// Ninjara Hoi! (J)
case 0xe63d9193:	// Tanigawa Kouji no Shougi Shinan 3 (J)
case 0xd54f5da9:	// Tsuppari Wars (J)
	g_ROM.header.flags_1 |= MASK_HAS_SAVE_RAM;
	break;
case 0xb8747abf:   // Best Play - Pro Yakyuu Special (J)
	g_ROM.size_SaveRAM = 16*1024;
	g_ROM.header.flags_1 |= MASK_HAS_SAVE_RAM;
	break;
	// Mapper 003 -----------------------------------------------------------------
case 0x8218c637:   // Space Hunter (J)
	g_ROM.header.flags_1 &= 0xf6;
	g_ROM.header.flags_1 |= MASK_4SCREEN_MIRRORING;
	break;
case 0x2bb6a0f8:	// Sherlock Holmes - Hakushaku Reijou Yuukai Jiken (J)
case 0x28c11d24:	// Sukeban Deka 3 (J)
case 0x02863604:	// Sukeban Deka 3 (J) (Alt)
	g_ROM.header.flags_1 &= 0xf6;
	g_ROM.header.flags_1 |= MASK_VERTICAL_MIRRORING;
	break;
case 0x29401686:	// Minna no Taabou no Nakayoshi Dai Sakusen (J)
	g_ROM.ROM_banks[0x2B3E] = 0x60;
	break;
case 0x9b6d2cb5:	// Mickey Mousecapade (J) [a]
	g_ROM.ROM_banks[0x5B98] = 0x80;
	break;
case 0x932a077a:	// TwinBee (J)
	g_ROM.mapper = 87;
	break;
	// Mapper 004 -----------------------------------------------------------------
case 0x58581770:// Rasaaru Ishii no Childs Quest (J)
	g_ROM.header.flags_1 &= 0xf6;
	g_ROM.header.flags_1 |= MASK_VERTICAL_MIRRORING;
	break;
case 0xf3feb3ab:	// Kunio Kun no Jidaigeki Dayo Zenin Shuugou! (J)
case 0xa524ae9b:	// Otaku no Seiza - An Adventure in the Otaku Galaxy (J)
case 0x46dc6e57:	// SD Gundam - Gachapon Senshi 2 - Capsule Senki (J)
case 0x66b2dec7:	// SD Gundam - Gachapon Senshi 3 - Eiyuu Senki (J)
case 0x92b07fd9:	// SD Gundam - Gachapon Senshi 4 - New Type Story (J)
case 0x8ee6463a:	// SD Gundam - Gachapon Senshi 5 - Battle of Universal Century (J)
case 0xaf754426:	// Ultraman Club 3 (J)
case 0xfe4e5b11:	// Ushio to Tora - Shinen no Daiyou (J)
case 0x57c12c17:	// Yamamura Misa Suspense - Kyouto Zaiteku Satsujin Jiken (J)
	g_ROM.header.flags_1 |= MASK_HAS_SAVE_RAM;
	break;
case 0x42e03e4a:	// RPG Jinsei Game (J)
	g_ROM.header.flags_1 |= MASK_HAS_SAVE_RAM;
	g_ROM.mapper = 118;
	break;
	// Mapper 005 -----------------------------------------------------------------
case 0x2b548d75:	// Bandit Kings of Ancient China (J)
case 0xf4cd4998:	// Dai Koukai Jidai (J)
case 0x8fa95456:	// Ishin no Arashi (J)
case 0x98c8e090:	// Nobunaga no Yabou - Sengoku Gunyuu Den (J)
case 0x57e3218b:	// L'Empereur (J)
case 0x2f50bd38:	// L'Empereur (U)
case 0x8e9a5e2f:	// L'Empereur (Alt)(U)
case 0xb56958d1:	// Nobunaga's Ambition 2 (J)
case 0xe6c28c5f:	// Suikoden - Tenmei no Chikai (J)
case 0xcd35e2e9:	// Uncharted Waters (J)
	g_ROM.size_SaveRAM = 32*1024;
	break;
case 0xf4120e58:	// Aoki Ookami to Shiroki Mejika - Genchou Hishi (J)
case 0x286613d8:	// Nobunaga no Yabou - Bushou Fuuun Roku (J)
case 0x11eaad26:	// Romance of the 3 Kingdoms 2 (J)
case 0x95ba5733:	// Sangokushi 2 (J)
	g_ROM.size_SaveRAM = 64*1024;
	break;
case 0xe91548d8:	// Shin 4 Nin Uchi Mahjong - Yakuman Tengoku (J)
	g_ROM.header.flags_1 |= MASK_HAS_SAVE_RAM;
	break;
	// Mapper 010 -----------------------------------------------------------------
case 0xc9cce8f2:	// Famicom Wars (J)
	g_ROM.header.flags_1 |= MASK_HAS_SAVE_RAM;
	break;
	// Mapper 011 -----------------------------------------------------------------
case 0x6467a5c4:	// Maxi 15 (Unl) (U)
	g_ROM.mapper = 234;
	break;
	// Mapper 016 -----------------------------------------------------------------
case 0x983d8175:	// Datach - Battle Rush - Build Up Robot Tournament (J)
case 0x894efdbc:	// Datach - Crayon Shin Chan - Ora to Poi Poi (J)
case 0x19e81461:	// Datach - Dragon Ball Z - Gekitou Tenkaichi Budou Kai (J)
case 0xbe06853f:	// Datach - J League Super Top Players (J)
case 0x0be0a328:	// Datach - SD Gundam - Gundam Wars (J)
case 0x5b457641:	// Datach - Ultraman Club - Supokon Fight! (J)
case 0xf51a7f46:	// Datach - Yuu Yuu Hakusho - Bakutou Ankoku Bujutsu Kai (J)
case 0x31cd9903:	// Dragon Ball Z - Kyoushuu! Saiya Jin (J)
case 0xe49fc53e:	// Dragon Ball Z 2 - Gekishin Freeza!! (J)
case 0x09499f4d:	// Dragon Ball Z 3 - Ressen Jinzou Ningen (J)
case 0x2e991109:	// Dragon Ball Z Gaiden - Saiya Jin Zetsumetsu Keikaku (J)
	g_ROM.header.flags_1 |= MASK_HAS_SAVE_RAM;
	break;
	// Mapper 019 -----------------------------------------------------------------
case 0x3296ff7a:	// Battle Fleet (J)
case 0x429fd177:	// Famista '90 (J)
case 0xdd454208:	// Hydlide 3 - Yami Kara no Houmonsha (J)
case 0xb1b9e187:	// Kaijuu Monogatari (J)
case 0xaf15338f:	// Mindseeker (J)
	g_ROM.header.flags_1 |= MASK_HAS_SAVE_RAM;
	break;
	// Mapper 026 -----------------------------------------------------------------
case 0x836cc1ab:	// Mouryou Senki Madara (J)
	g_ROM.header.flags_1 |= MASK_HAS_SAVE_RAM;
	break;
	// Mapper 033 -----------------------------------------------------------------
case 0x547e6cc1:	// Flintstones - The Rescue of Dino & Hoppy, The (J)
	g_ROM.mapper = 48;
	break;
case 0xa71c3452:	// Insector X (J)
	g_ROM.header.flags_1 &= 0xf6;
	g_ROM.header.flags_1 |= MASK_VERTICAL_MIRRORING;
	break;
	// 42
case 0x4BBA5080:	// Ai sensi Nicol Pirate
	g_ROM.mapper = 254;
	break;
	// Mapper 043 -----------------------------------------------------------------
case 0x6175b9a0:	// 150-in-1 (Pirate Cart)
	g_ROM.mapper = 235;
	break;
	// Mapper 047 -----------------------------------------------------------------
case 0x7eef434c:	// Supare Mario Bros, Tetris, Nintendo World Cup
	g_ROM.mapper = 47;
	break;
	// Mapper 065 -----------------------------------------------------------------
case 0xfd3fc292:	// Ai Sensei no Oshiete - Watashi no Hoshi (J)
	g_ROM.mapper = 32;
	break;
	// Mapper 068 -----------------------------------------------------------------
case 0xfde79681:	// Maharaja (J)
	g_ROM.header.flags_1 |= MASK_HAS_SAVE_RAM;
	break;
	// Mapper 080 -----------------------------------------------------------------
case 0x95aaed34:	// Mirai Shinwa Jarvas (J)
case 0x17627d4b:	// Taito Grand Prix - Eikou heno License (J)
	g_ROM.header.flags_1 |= MASK_HAS_SAVE_RAM;
	break;
	// Mapper 082 -----------------------------------------------------------------
case 0x4819a595:	// Kyuukyoku Harikiri Stadium - Heisei Gannen Ban (J)
	g_ROM.header.flags_1 |= MASK_HAS_SAVE_RAM;
	break;
	// Mapper 086 -----------------------------------------------------------------
case 0xe63f7d0b:	// Urusei Yatsura - Lum no Wedding Bell (J)
	g_ROM.mapper = 101;
	break;
	// VS Unisystem ---------------------------------------------------------------
case 0xeb2dba63:	// VS TKO Boxing
	g_ROM.mapper = 4;
	break;
case 0xcbe85490:	// VS Excitebike
case 0x29155e0c:	// VS Excitebike (Alt)
case 0x17ae56be:	// VS Freedom Force
case 0xff5135a3:	// VS Hogan's Alley
	g_ROM.header.flags_1 &= 0xf6;
	g_ROM.header.flags_1 |= MASK_4SCREEN_MIRRORING;
	break;
case 0x0b65a917:	// VS Mach Rider
	g_ROM.ROM_banks[0x7FDF] = 0xFF;
	g_ROM.ROM_banks[0x7FE4] = 0x00;
	break;
}

