
// data base to set the CYCLES PER LINE

g_NES.CYCLES_PER_LINE = CYCLES_DEFAULT_CYCLES_PER_LINE;//((float)((double)1364.0/(double)12.0));
//g_NES.CYCLES_PER_LINE = (float)113.6363492063492;
g_NES.CYCLES_BEFORE_NMI = 1 << CYCLES_SHIFT;
g_NES.BANKSWITCH_PER_TILE = 0;
g_NES.DPCM_IRQ = 1;

// Mapper 020 -----------------------------------------------------------------
if(NES_fds_id() == 0xb24b4e4d ||	// Kinnikuman - Kinnikusei Oui Soudatsusen
        NES_fds_id() == 0x014c4e4b ||	// Legend of Zelda 2 - Link no Bouken, The
        NES_fds_id() == 0xc1414c4e)	// Super Boy Allan
{
	g_NES.CYCLES_PER_LINE =117 << CYCLES_SHIFT;
}
else
	switch(crc){
	case 0x0b5667e9:		// 000 Excitebike (E)
	case 0x3a94fa0b:		// 000 Excitebike (JU)
	case 0x16342ad0:		// 000 baltron(J)
		g_NES.CYCLES_PER_LINE = 112 << CYCLES_SHIFT;
		break;
	case 0x900c7442:		// 001 Konamic Sports in Seoul (J)
		g_NES.BANKSWITCH_PER_TILE = 1;
		break;
	case 0xcf9cf7a2:		// 001 Romancia (J)
		g_NES.DPCM_IRQ = 1;
		break;
	case 0xdb564628:		// 001 Mario Open Golf (J)
		g_NES.CYCLES_PER_LINE = 105 << CYCLES_SHIFT;
		break;
	case 0x547ad451:	// Gekitou Stadium!! (J)
		g_NES.CYCLES_PER_LINE = 111 << CYCLES_SHIFT;
		break;
	case 0xaf16ee39:	// Hyokkori Hyoutan Jima - Nazo no Kaizokusen (J)
		g_NES.CYCLES_PER_LINE = 112 << CYCLES_SHIFT;
		break;
	case 0x9183054e:	// Dengeki - Big Bang! (J)
		g_NES.CYCLES_PER_LINE = 115 << CYCLES_SHIFT;
		break;
	case 0x7e5d2f1a:	// Itadaki Street - Watashi no Mise ni Yottette (J)
	case 0x37bf04d7:	// Marusa no Onna (J)
		g_NES.CYCLES_PER_LINE = 120 << CYCLES_SHIFT;
		break;
	case 0x11469ce3:	// Viva! Las Vegas (J)
		g_NES.CYCLES_PER_LINE = 130 << CYCLES_SHIFT;
		break;
	case 0x58507bc9:	// Famicom Top Management (J)
		g_NES.CYCLES_PER_LINE = 160 << CYCLES_SHIFT;
		break;
	case 0x7bd7b849:	// Nekketu koukou Dodgeballbu (J)
		g_NES.CYCLES_PER_LINE = 170 << CYCLES_SHIFT;
		break;
	case 0x95e4e594:	// Qix
//		g_NES.CYCLES_PER_LINE = ;
		break;
		// Mapper 002 -----------------------------------------------------------------
	case 0x7b0a41b9:	// Esper Bouken Tai (J)
		g_NES.CYCLES_PER_LINE = 128 << CYCLES_SHIFT;
		break;
	case 0xb59a7a29:	// Guardic Gaiden (J)
		g_NES.CYCLES_PER_LINE = 136 << CYCLES_SHIFT;
		break;
		// Mapper 003 -----------------------------------------------------------------
	case 0xca26a0f1:	// Family Trainer - Dai Undoukai (J)
		g_NES.CYCLES_PER_LINE = 113 << CYCLES_SHIFT;
		break;
	case 0xa30d8baf:	// Tokoro San no Mamorumo Semerumo (J)
		g_NES.CYCLES_PER_LINE = 115 << CYCLES_SHIFT;
		g_NES.CYCLES_BEFORE_NMI = 10 << CYCLES_SHIFT;
		g_NES.BANKSWITCH_PER_TILE = 1;
		break;
	case 0xd21da4f7:	// Drop Zone (E)
		g_NES.CYCLES_PER_LINE = 105 << CYCLES_SHIFT;
		break;
		// Mapper 004 -----------------------------------------------------------------
	case 0x1d8b2f59:	// Hyryuu no Ken 3 (J)
		g_NES.CYCLES_PER_LINE = 140 << CYCLES_SHIFT;
		break;
	case 0x14a01c70:	// Gun-Dec (J)
		g_NES.BANKSWITCH_PER_TILE = 1;
		break;
	case 0xb42feeb4:	// Beauty and the Beast (E)
		//			g_NES.CYCLES_PER_LINE = 108;
		break;
	case 0x0baf01d0:	// Juju Densetsu (J)
		g_NES.CYCLES_PER_LINE = 112 << CYCLES_SHIFT;
		break;
	case 0x5c707ac4:	// Mother (J)
		g_NES.CYCLES_PER_LINE = 114 << CYCLES_SHIFT;
		g_NES.BANKSWITCH_PER_TILE = 1;
		break;
	case 0xcb106f49:	// F-1 Sensation (J)
		g_NES.CYCLES_PER_LINE = 115 << CYCLES_SHIFT;
		break;
	case 0x8b59bac3:	// Radia Senki - Reimei Hen (J)
	case 0x00161afd:	// Ys 2 - Ancient Ys Vanished The Final Chapter (J)
	case 0x26ff3ea2:	// Yume Penguin Monogatari (J)
		g_NES.CYCLES_PER_LINE = 120 << CYCLES_SHIFT;
		break;
	case 0x459adfa8:	// Cattou Ninden Teyandee (J)
	case 0xbea682e5:	// Doki! Doki! Yuuenchi (J)
	case 0xf1081b1b:	// Heavy Barrel (J)
	case 0xb4d554d6:	// Heavy Barrel (U)
	case 0x9a172152:	// Jackie Chan (J)
	case 0x170739cf:	// Jigoku Gokuraku Maru (J)
	case 0xdeddd5e5:	// Kunio Kun no Nekketsu Soccer League (J)
	case 0x0c935dfe:	// Might and Magic (J)
	case 0x85c5b6b4:	// Nekketsu Kakutou Densetsu (J)
	case 0x6bbd9f11:	// Utsurun Desu (J)
	case 0x23f4b48f:	// Wily & Light no Rockboard - That's Paradise (J)
		g_NES.CYCLES_PER_LINE = 128 << CYCLES_SHIFT;
		break;
	case 0xd88d48d7:	// Kick master(U)
		g_NES.CYCLES_PER_LINE = 130 << CYCLES_SHIFT;
		break;
	case 0xa9a0d729:	// Dai Kaijuu - Deburas (J)
	case 0x346cd5d3:	// Ike Ike! Nekketsu Hockey Bu - Subette Koronde Dai Rantou (J)
	case 0x5fbd9178:	// Mitsume ga Tooru (English v1_01)
	case 0x390e0320:	// Mitsume ga Tooru (J)
	case 0x6055fe9b:	// Tecmo Super Bowl (J)
	case 0x8e5c2818:	// Tecmo Super Bowl (U)
	case 0xe3765667:	// Top Gun - The Second Mission (U)
		g_NES.CYCLES_PER_LINE = 136 << CYCLES_SHIFT;
		break;
	case 0x0ae5b57f:	// Super Donkey Kong 2 (Unk)
		g_NES.CYCLES_PER_LINE = 140 << CYCLES_SHIFT;
		break;
	case 0x336e2a6f:	// Asmik Kun Land (J)
	case 0x7ca52798:	// Chiki Chiki Machine Mou Race (J)
	case 0xc17ae2dc:	// God Slayer - Haruka Tenkuu no Sonata (J)
	case 0x8685f366:	// Matendouji (J)
	case 0xb55da544:	// Nekketsu Koukou - Dodgeball Bu - Soccer Hen (J)
	case 0x4fb460cd:	// Nekketsu! Street Basket - Ganbare Dunk Heroes (J)
	case 0xe19a2473:	// Sugoro Quest - Dice no Senshi Tachi (J)
	case 0x14a45522:	// Sugoro Quest - The Quest of Dice Heros (English vX.XX)
	case 0x80cd1919:	// Super Mario Bros 3 (E)
	case 0xe386da54:	// Super Mario Bros 3 (J)
	case 0xd852c2f7:	// Time Zone (J)
	case 0xade11141:	// Wanpaku Kokkun no Gourmet World (J)
		g_NES.CYCLES_PER_LINE = 144 << CYCLES_SHIFT;
		break;
	case 0x3c5ae54b:	// 1999 - Hore, Mitakotoka! Seikimatsu (J)
	case 0xb548fd2e:	// Bananan Ouji no Dai Bouken (J)
	case 0xeffeea40:	// Klax (J)
	case 0x2dc3817d:	// Meimon! Takonishi Ouendan - Kouha 6 Nin Shuu (J)
	case 0x5c1d053a:	// SD Battle Oozumou - Heisei Hero Basho (J)
	case 0x0bf31a3d:	// Soreike! Anpanman - Minna de Hiking Game! (J)
		g_NES.CYCLES_PER_LINE = 152 << CYCLES_SHIFT;
		break;
	case 0x6cc62c06:	// HoshinoKirby-YumenoIzuminoMonogatari(J)
	case 0xa67ea466:	// Arean 3
		g_NES.CYCLES_PER_LINE = 110 << CYCLES_SHIFT;
		break;
		// Mapper 007 -----------------------------------------------------------------
	case  0x524a5a32:	// Battletoads (E)
	case 0x9806cb84:	// Battletoads (J)
	case 0x279710dc:	// Battletoads (U)
		g_NES.CYCLES_PER_LINE = 112 << CYCLES_SHIFT;
		break;
	case 0x23d7d48f:	// Battletoads Double Dragon(E)
	case 0xceb65b06:	// Battletoads Double Dragon(U)
		g_NES.CYCLES_PER_LINE = 111 << CYCLES_SHIFT;
		break;
		// Mapper 016 -----------------------------------------------------------------
	case 0x6c6c2feb:	// Dragon Ball 3 - Gokuu Den (J)
	case 0x0c187747:	// Nishimura Kyoutarou Mystery - Blue Train Satsujin Jiken (J)
	case 0x09499f4d:	// DragonBallZ3-RessenJinzouNingen(J)
		g_NES.CYCLES_PER_LINE = 120 << CYCLES_SHIFT;
		break;
	case 0xab3062cf:	// sakigake
		g_NES.CYCLES_PER_LINE = 110 << CYCLES_SHIFT;
		break;
		//	case 0x73ac76db )  // SD Gundam Gaiden 2
		//  g_NES.CYCLES_PER_LINE = 120;
		// Mapper 018 -----------------------------------------------------------------
	case 0xa54d9086:	// Toukon Club (J)
		g_NES.CYCLES_PER_LINE = 114 << CYCLES_SHIFT;
		break;
		// Mapper 019 -----------------------------------------------------------------
	case 0x17421900:	// Youkai Douchuuki (J)
		g_NES.BANKSWITCH_PER_TILE = 1;
		break;
	case 0x6901346e:	// Sangokushi 2 - Haou no Tairiku (J)
		g_NES.CYCLES_PER_LINE = 105 << CYCLES_SHIFT;
		break;
	case 0x761ccfb5:	// Digital Devil Story - Megami Tensei 2 (J)
		g_NES.CYCLES_PER_LINE = 119 << CYCLES_SHIFT;
		break;
	case 0x9a2b0641:	// Namco Classic 2 (J)
		g_NES.CYCLES_PER_LINE = 120 << CYCLES_SHIFT;
		break;
	case 0x2b825ce1:	// Namco Classic (J)
		//	   	case 0x3deac303:	// Rolling Thunder (J)
	case 0xb62a7b71:	// Family Circuit '91
		g_NES.CYCLES_PER_LINE = 144 << CYCLES_SHIFT;
		break;
	case 0x96533999:	// Dokuganryuu Masamune (J)
		g_NES.CYCLES_BEFORE_NMI = 10 << CYCLES_SHIFT;
		break;
	case 0x3deac303:	// RollingThunder(J)
		g_NES.CYCLES_PER_LINE = 150 << CYCLES_SHIFT;
		break;
		// Mapper 021 -----------------------------------------------------------------
	case 0xb201b522:	// Wai Wai World 2 - SOS!! Paseri Jou (J)
		g_NES.CYCLES_PER_LINE = 117 << CYCLES_SHIFT;
		break;
		// Mapper 023 -----------------------------------------------------------------
	case 0x64818fc5:	// Wai Wai World (J)
		g_NES.CYCLES_PER_LINE = 115 << CYCLES_SHIFT;
		break;
		// Mapper 024 -----------------------------------------------------------------
	case 0xba6bdd6a:	// Akumajou Densetsu (J)
		g_NES.CYCLES_PER_LINE = 119 << CYCLES_SHIFT;
		break;
		// 25
		//		case 0xa2e68da8:   // Racer Mini Yonku - Japan Cup
		//			g_NES.CYCLES_PER_LINE = ;
		//			break;
		// Mapper 032 -----------------------------------------------------------------
	case 0x788bed9a:	// Meikyuu Jima (J)
		g_NES.CYCLES_PER_LINE = 114 << CYCLES_SHIFT;
		break;
		// Mapper 033 -----------------------------------------------------------------
	case 0xa71c3452:	// Insector X (J)
		g_NES.CYCLES_PER_LINE = 110 << CYCLES_SHIFT;
		break;
	case 0x63bb86b5:	// Jetsons-Cogswell'sCaper!,The(J)
		g_NES.CYCLES_PER_LINE = 117 << CYCLES_SHIFT;
		break;
		//45
		//		case 0xc082e6d3:	//
		//			g_NES.CYCLES_PER_LINE = 115;
		//			break;
		// Mapper 065 -----------------------------------------------------------------
	case 0xd202612b:	// Spartan X 2 (J)
		g_NES.CYCLES_PER_LINE = 125 << CYCLES_SHIFT;
		break;
		// Mapper 085 -----------------------------------------------------------------
//	case 0x33CE3FF0:	// Lagurange (J)
//		g_NES.CYCLES_PER_LINE = 125;
//		break;
		// Mapper 094 -----------------------------------------------------------------
	case 0x441aeae6:	// Senjou no Ookami (J)
		g_NES.CYCLES_PER_LINE = 117 << CYCLES_SHIFT;
		break;
		// Mapper 095 -----------------------------------------------------------------
	case 0xc7a79be2:	// Dragon Buster (J)
		g_NES.CYCLES_PER_LINE = 110 << CYCLES_SHIFT;
		break;
		// Mapper 246 -----------------------------------------------------------------
	case 0xea76fb00:	//
		g_NES.CYCLES_PER_LINE = 112 << CYCLES_SHIFT;
		break;
	}

