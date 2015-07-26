// 各マッパーの必要なメモリを設定
union {

	
// Mapper 
struct {
	uint8 xram[0x2000];
	uint8 *wram;
} Mapper;


// Mapper NSF
struct {
	uint8 wram1[0x2000];
	uint8 wram2[0x8000];
	uint8 chip_type;
} MapperNSF;

// Mapper 255
struct {
	uint8 regs[4];
} Mapper255;

// Mapper 254
struct {
	uint8 regs[1];
	uint32 irq_counter, irq_latch;
	uint8 irq_enabled;
} Mapper254;

// Mapper 253
struct {
	uint8 regs[9];
	uint8 rom_type;
	
	uint8	irq_enable;
	uint8	irq_counter;
	uint8	irq_latch ;	
	int 	irq_clock ; 
	uint8   cycles;
	uint8 	VRAM_switch;	       
	
} Mapper253;

// Mapper 251
struct {
	uint8 regs[11];
	uint8 bregs[4];
} Mapper251;


// Mapper 249
struct {
	uint8	spdata;
	uint8	regs[8];
	uint8	prg0, prg1;
	uint8	chr01, chr23, chr4, chr5, chr6, chr7;
	uint8	we_sram;
	uint8	patch;	
	uint8	irq_type;
	uint8	irq_enable;
	uint8	irq_counter;
	uint8	irq_latch;
	uint8	irq_request;

} Mapper249;

// Mapper 248
struct {
	uint8  regs[8];

	uint32 prg0,prg1;
	uint32 chr01,chr23,chr4,chr5,chr6,chr7;

	uint8 irq_enabled; // IRQs enabled
	uint8 irq_counter; // IRQ scanline counter, decreasing
	uint8 irq_latch;   // IRQ scanline counter latch
} Mapper248;

// Mapper 245
struct {
	uint8	reg[8];
	uint8	prg0, prg1;
	uint8	chr01, chr23, chr4, chr5, chr6, chr7;
	uint8	we_sram;
    
	uint8	irq_type;
	uint8	irq_enable;
	uint8	irq_counter;
	uint8	irq_latch;
	uint8	irq_request;
} Mapper245;

// Mapper 243
struct {
	uint8 regs[4];
} Mapper243;

// Mapper 236
struct {
	uint8 bank, mode;
} Mapper236;

// Mapper 237
struct {
	uint8 *wram;
} Mapper237;

// Mapper 235
struct {
	uint8 dummy[0x2000];
} Mapper235;

// Mapper 234
struct {
	uint8 regs[3];
} Mapper234;

// Mapper 232
struct {
	uint8 regs[2];
} Mapper232;

// Mapper 230
struct {
	uint8 rom_switch;
} Mapper230;

// Mapper 226
struct {
	uint8 regs[2];
} Mapper226;

// Mapper 217
struct {
	uint8 cmdin;
	uint8 cmd;
	uint8 regs[8];
	uint8 master,chrm;
	
	uint8 IRQCount;
} Mapper217;

// Mapper 215
struct {
	unsigned int dummy_mmc3[20];
	uint8 exRegs[4];
} Mapper215;


// Mapper 189
struct {
	uint8  *wram;

	uint8	patch;

	uint8	regs[2];
	uint8	chr01, chr23, chr4, chr5, chr6, chr7;

	uint8	irq_enabled;
	uint8	irq_counter;
	uint8	irq_latch;

	// SF4
	uint8	protect_dat[4];
	uint8	lwd;
} Mapper189;

// Mapper 188
struct {
	uint8 dummy[0x2000];
} Mapper188;

// Mapper 185
struct {
	uint8 patch;
	uint8 dummy_chr_rom[0x400];
} Mapper185;

// Mapper 183
struct {
	uint8 regs[8];
	uint8 irq_enabled;
	uint32 irq_counter;
} Mapper183;

// Mapper 182
struct {
	uint8 regs[1];
	uint8 irq_enabled;
	uint8 irq_counter;
} Mapper182;

// Mapper 178
struct {
	uint8 regs[4];
} Mapper178;

// Mapper 177
struct {
	uint8 HengGe_TYPE;
} Mapper177;

// Mapper 175
struct {
	uint8 reg_dat;
} Mapper175;


//mapper 167
struct {
	uint8	regs[4];
	uint8	rom_type;
}Mapper167;

//mapper 165
struct {
	uint8 regs[8];
	uint8 prg0,prg1;
	uint8 chr0,chr1,chr2,chr3;
	uint8 latch;
	uint8 we_sram;
}Mapper165;


// Mapper 164
struct {	
	unsigned int regs[2];
}Mapper164;


// Mapper 163
struct {	
	uint8 wram[0x2000];

	uint8 cmd, laststrobe, trigger;
	uint8 DRegs[8];
}Mapper163;


// Mapper 162
struct {
	uint8	reg5000;
	uint8	reg5100;
	uint8	reg5200;
	uint8	reg5300;
}Mapper162;

// Mapper 160
struct {
	uint8 irq_enabled;
	uint8 irq_counter;
	uint8 irq_latch;
	uint8 refresh_type;
} Mapper160;


// Mapper 142
struct {
	uint8	reg[5];
	uint8	irq_enable;
	uint32	irq_counter;
} Mapper142;

// Mapper 132
struct {
	uint8 regs[4];
	uint8 cmd, is172, is173;
} Mapper132;

// Mapper 122
struct {
	int patch;
} Mapper122;

// Mapper 119
struct {
	uint8 regs[8];
	uint32 prg0,prg1;
	uint32 chr01,chr23,chr4,chr5,chr6,chr7;
	uint8 irq_enabled; // IRQs enabled
	uint8 irq_counter; // IRQ scanline counter, decreasing
	uint8 irq_latch;   // IRQ scanline counter latch
} Mapper119;

// Mapper 118
struct {
	uint8  regs[8];
	uint32 prg0,prg1;
	uint32 chr0,chr1,chr2,chr3,chr4,chr5,chr6,chr7;
	uint8 irq_enabled; // IRQs enabled
	uint8 irq_counter; // IRQ scanline counter, decreasing
	uint8 irq_latch;   // IRQ scanline counter latch
} Mapper118;

// Mapper 117
struct {
	uint8 irq_line;
	uint8 irq_enabled1;
	uint8 irq_enabled2;
} Mapper117;

// Mapper 116
struct {
	unsigned int dummy_mmc3[20];
	uint32 mode;
	
	uint8 vrc2_chr[8];
	uint8 vrc2_prg[2];
	uint8 vrc2_nmt;
	uint8 vrc2_padding;
	
	uint8 mmc3_banks[10];
	uint8 mmc3_ctrl;
	uint8 mmc3_nmt;

	uint8 mmc1_regs[4];
	uint8 mmc1_buffer;
	uint8 mmc1_shifter;
	uint8 mmc1_padding[2];
	uint8 irq_enable ;	// Disable
	uint8 irq_counter ;
	uint8 irq_latch ;
	
	uint8 exPreg;
	
} Mapper116;

// Mapper 115
struct {
	uint8	reg[8];
	uint8	prg0, prg1, prg2, prg3;
	uint8	prg0L, prg1L;
	uint8	chr0, chr1, chr2, chr3, chr4, chr5, chr6, chr7;

	uint8	irq_enable;
	uint8	irq_counter;
	uint8	irq_latch;

	uint8	ExPrgSwitch;
	uint8	ExChrSwitch;
} Mapper115;

// Mapper 114
struct {
	uint8 regs[1];
} Mapper114;

// Mapper 112
struct {
	uint8  regs[8];
	uint32 prg0,prg1;
	uint32 chr01,chr23,chr4,chr5,chr6,chr7;
	uint8 irq_enabled; // IRQs enabled
	uint8 irq_counter; // IRQ scanline counter, decreasing
	uint8 irq_latch;   // IRQ scanline counter latch
} Mapper112;

// Mapper 105
struct {
	uint8  write_count;
	uint8  bits;
	uint8  regs[4];

	uint8  irq_enabled;
	uint32 irq_counter;
	uint8  init_state;
} Mapper105;

// Mapper 104
struct {
	uint8  prg_bank0;
} Mapper104;

// Mapper 100
struct {
	uint8  regs[8];

	uint32 prg0,prg1,prg2,prg3;
	uint32 chr0,chr1,chr2,chr3,chr4,chr5,chr6,chr7;

	uint8 irq_enabled; // IRQs enabled
	uint8 irq_counter; // IRQ scanline counter, decreasing
	uint8 irq_latch;   // IRQ scanline counter latch
} Mapper100;

// Mapper 96
struct {
	uint8 vbank0,vbank1;
} Mapper96;

// Mapper 95
struct {
	uint8  regs[1];
	uint32 prg0,prg1;
	uint32 chr01,chr23,chr4,chr5,chr6,chr7;
} Mapper95;

// Mapper 91
struct {
	uint8 irq_counter;
	uint8 irq_enabled;
} Mapper91;

// Mapper 90
struct {
	uint8	patch;

	uint8	prg_reg[4];
	uint8	nth_reg[4], ntl_reg[4];
	uint8	chh_reg[8], chl_reg[8];

	uint8	irq_enable;
	uint8	irq_counter;
	uint8	irq_latch;
	uint8	irq_occur;
	uint8	irq_preset;
	uint8	irq_offset;
    
	uint8	prg_6000, prg_E000;
	uint8	prg_size, chr_size;
	uint8	mir_mode, mir_type;
    
	uint8	key_val;
	uint8	mul_val1, mul_val2;
	uint8	sw_val;
} Mapper90;

// Mapper 88
struct {
	uint8  regs[2];
} Mapper88;

// Mapper 85
struct {
	uint8 irq_enabled;
	uint8 irq_counter;
	uint8 irq_latch;
	int patch;
} Mapper85;

// Mapper 83
struct {
	uint8 regs[3];
	uint32 irq_counter;
	uint8 irq_enabled;
} Mapper83;

// Mapper 82
struct {
	uint8 regs[1];
} Mapper82;

// Mapper 80
struct {
	uint8 patch;
} Mapper80;

// Mapper 76
struct {
	uint8 regs[1];
} Mapper76;

// Mapper 75
struct {
	uint8 regs[2];
} Mapper75;

// Mapper 74
struct {
	uint8 *wram;
	
	uint8 regs[8];
	uint8 prg0,prg1,prg2,prg3;
	uint8 chr[8];
	
	uint8 we_sram, irq_enable , irq_counter, irq_latch, irq_request;
} Mapper074;

// Mapper 73
struct {
	uint8 irq_enabled;
	uint32 irq_counter;
} Mapper73;

// Mapper 70
struct {
	uint8 patch;
} Mapper70;

// Mapper 69
struct {
	uint8 patch;
	uint8 regs[1];
	uint8 irq_enabled;
	uint32 irq_counter;
} Mapper69;

// Mapper 68
struct {
	uint8 regs[4];
} Mapper68;

// Mapper 67
struct {
	uint8 irq_enabled;
	uint8 irq_counter;
	uint8 irq_latch;
} Mapper67;

// Mapper 65
struct {
	uint8 patch, patch2;
	uint8 irq_enabled;
	uint32 irq_counter;
	uint32 irq_latch;
} Mapper65;

// Mapper 64
struct {
	uint8 regs[3];
	uint8 irq_latch;
	uint8 irq_counter;
	uint8 irq_enabled;
} Mapper64;

// Mapper 60
struct {
	uint8	patch;
	uint8	game_sel;
} Mapper60;

// Mapper 57
struct {
	uint8 regs[1];
} Mapper57;

// Mapper 52
struct {
	uint8  regs[8];
	uint32 prg0,prg1;
	uint32 chr01,chr23,chr4,chr5,chr6,chr7;
	uint8 irq_enabled; // IRQs enabled
	uint8 irq_counter; // IRQ scanline counter, decreasing
	uint8 irq_latch;   // IRQ scanline counter latch
} Mapper52;

// Mapper 51
struct {
	uint8 bank, mode;
} Mapper51;

// Mapper 50
struct {
	uint8 irq_enabled;
} Mapper50;

// Mapper 49
struct {
	uint8  regs[3];
	uint32 prg0,prg1;
	uint32 chr01,chr23,chr4,chr5,chr6,chr7;

	uint8 irq_enabled;
	uint8 irq_counter;
	uint8 irq_latch;
} Mapper49;

// Mapper 48
struct {
	uint8 regs[1];
	uint8 irq_enabled;
	uint8 irq_counter;
} Mapper48;

// Mapper 47
struct {
	uint8  regs[8];
	uint8  patch;
	uint32 rom_bank;
	uint32 prg0,prg1;
	uint32 chr01,chr23,chr4,chr5,chr6,chr7;
	uint8  irq_enabled; // IRQs enabled
	uint8  irq_counter; // IRQ scanline counter, decreasing
	uint8  irq_latch;   // IRQ scanline counter latch
} Mapper47;

// Mapper 46
struct {
	uint8 regs[4];
} Mapper46;

// Mapper 45
struct {
	uint8 patch;

	uint8  regs[7];
	uint32 p[4],prg0,prg1,prg2,prg3;
	uint32 c[8],chr0,chr1,chr2,chr3,chr4,chr5,chr6,chr7;

	uint8 irq_enabled;
	uint8 irq_counter;
	uint8 irq_latch;
} Mapper45;

// Mapper 44
struct {
	uint8  regs[8];

	uint32 rom_bank;
	uint32 prg0,prg1;
	uint32 chr01,chr23,chr4,chr5,chr6,chr7;
	uint8 irq_enabled; // IRQs enabled
	uint8 irq_counter; // IRQ scanline counter, decreasing
	uint8 irq_latch;   // IRQ scanline counter latch
} Mapper44;

// Mapper 43
struct {
	uint8 irq_enabled;
	uint32 irq_counter;
} Mapper43;

// Mapper 42
struct {
	uint8 irq_counter;
	uint8 irq_enabled;
} Mapper42;

// Mapper 41
struct {
	uint8 regs[1];
} Mapper41;

// Mapper 40 (smb2j)
struct {
	uint8 irq_enabled;
	uint32 lines_to_irq;
} Mapper40;

// Mapper 33
struct {
	uint8 patch;
	uint8 patch2;
	uint8 irq_enabled;
	uint8 irq_counter;
} Mapper33;

// Mapper 32
struct {
	uint8 patch;
	uint8 regs[1];
} Mapper32;

// Mapper 27
struct {
	uint16 regs[9];
	uint8 irq_enable,irq_counter,irq_latch;
	uint32 irq_clock;
} Mapper27;

// Mapper 26
struct {
	uint8 irq_enabled;
	uint8 irq_counter;
	uint8 irq_latch;
} Mapper26;

// Mapper 25
struct {
	uint8 patch;
	uint8 regs[11];
	uint8 irq_enabled;
	uint8 irq_counter;
	uint8 irq_latch;
} Mapper25;

// Mapper 24
struct {
	uint8 patch;
	uint8 irq_enabled;
	uint8 irq_counter;
	uint8 irq_latch;
} Mapper24;

// Mapper 23
struct {
	uint8 regs[9];

	uint32 patch;
	uint8 irq_enabled;
	uint8 irq_counter;
	uint8 irq_latch;
} Mapper23;

// Mapper 21
struct {
	uint8 regs[9];
	uint8 irq_enabled;
	uint8 irq_counter;
	uint8 irq_latch;
} Mapper21;

// Mapper 20
struct {
	uint8 bios[0x2000];
	uint8 *wram;
	uint8 disk[0x40000];

	uint8 irq_enabled;
	uint32 irq_counter;
	uint32 irq_latch;
	uint8 irq_wait;

	uint8 disk_enabled;
	uint32 head_position;
	uint8 write_skip;
	uint8 disk_status;
	uint8 write_reg;
	uint8 current_side;

	uint8 access_flag;
	uint8 last_side;
	uint8 insert_wait;

	uint8 patch;
} Mapper20;

// Mapper 19
struct {
	uint8 patch;

	uint8 regs[3];
	uint8 irq_enabled;
	uint32 irq_counter;
	uint32 irq_sn;
} Mapper19;

// Mapper 18
struct {
	uint8 patch;
	uint8 regs[11];
	uint8 irq_enabled;
	uint32 irq_latch;
	uint32 irq_counter;
} Mapper18;

// Mapper 17
struct {
	uint8 irq_enabled;
	uint32 irq_counter;
	uint32 irq_latch;
} Mapper17;

// Mapper 16
struct {
	uint8 patch, patch2;
	uint8 regs[3];

	uint8 serial_out[0x2000];

	uint8 eeprom_cmd[4];
	uint8 eeprom_status;
	uint8 eeprom_mode;
	uint8 eeprom_pset;
	uint8 eeprom_addr, eeprom_data;
	uint16 eeprom_wbit, eeprom_rbit;

	uint8 barcode[256];
	uint8 barcode_status;
	uint8 barcode_pt;
	uint8 barcode_pt_max;
	uint8 barcode_phase;
	uint32 barcode_wait;
	uint8 barcode_enabled;

	uint8 irq_enabled;
	uint32 irq_counter;
	uint32 irq_latch;
} Mapper16;

// Mapper 13
struct {
	uint8 prg_bank;
	uint8 chr_bank;
} Mapper13;

// Mapper 12
struct {
	uint32	vb0, vb1;
	uint8	reg[8];
	uint8	prg0, prg1;
	uint8	chr01, chr23, chr4, chr5, chr6, chr7;
	uint8	we_sram;

	uint8	irq_enable;
	uint8	irq_counter;
	uint8	irq_latch;
	uint8	irq_request;
	uint8	irq_preset;
	uint8	irq_preset_vbl;
} Mapper12;

// Mapper 10
struct {
	uint8 regs[6];
	uint8 latch_0000;
	uint8 latch_1000;
} Mapper10;

// Mapper 9
struct {
	uint8 regs[6];
	uint8 latch_0000;
	uint8 latch_1000;
} Mapper9;

// Mapper 6
struct {
	uint8 irq_enabled;
	uint32 irq_counter;
	uint8 chr_ram[4*0x2000];
} Mapper6;

// Mapper 5
struct {
	uint32 wb[8];
	uint8 *wram;
	uint8 wram_size;

	uint8 chr_reg[8][2];

	uint8 irq_enabled;
	uint8 irq_status;
	uint32 irq_line;

	uint32 value0;
	uint32 value1;

	uint8 wram_protect0;
	uint8 wram_protect1;
	uint8 prg_size;
	uint8 chr_size;
	uint8 gfx_mode;
	uint8 split_control;
	uint8 split_bank;
} Mapper5;

// Mapper 4
struct {
	uint8 *wram;
	uint8	reg[8];
	uint8	prg0, prg1;
	uint8	chr01, chr23, chr4, chr5, chr6, chr7;
	uint8	we_sram;

	uint8	irq_type;
	uint8	irq_enable;
	uint8	irq_counter;
	uint8	irq_latch;
	uint8	irq_request;
	uint8	irq_preset;
	uint8	irq_preset_vbl;

	uint8	vs_patch;
	uint8	vs_index;

	uint8	rom_type;
	uint8	security;
	
	} Mapper4;


// Mapper 2
struct {
	uint8	patch;
} Mapper2;


// Mapper 1
struct {
	uint32 write_count;
	uint8  bits;
	uint8  regs[4];
	uint32 last_write_addr;

	// Best Play - Pro Yakyuu Special
	uint8 patch;
	uint8 wram_bank, wram_flag, wram_count;
//	uint8 wram[0x4000];
	uint8 *wram;

	MMC1_Size_t MMC1_Size;
	uint32 MMC1_256K_base;
	uint32 MMC1_swap;

	// these are the 4 ROM banks currently selected
	uint32 MMC1_bank1;
	uint32 MMC1_bank2;
	uint32 MMC1_bank3;
	uint32 MMC1_bank4;

	uint32 MMC1_HI1;
	uint32 MMC1_HI2;
} Mapper1;


// Mapper 1
struct {	
	uint16	last_addr;

	uint8	patch;
	uint8	wram_patch;
	uint8	wram_bank;
	uint8	wram_count;

	uint8	reg[4];
	uint8	shift, regbuf;
} vnes01;




// Mapper 252
struct {
	uint8 regs[9];
	
	uint8	irq_enable;
	uint8	irq_counter;
	uint8	irq_latch;
	uint8	irq_occur;
	int 	irq_clock;
} Mapper252;

struct {
	unsigned int ctrl;
	uint8 cmd,dip;
	uint8 type;
	uint8 latch[8];
} S74x374;
//Mmc3
struct {
	unsigned int count;
	unsigned int latch;
	unsigned int reload;
	unsigned int enabled;				

	unsigned int ctrl0;
	unsigned int ctrl1;

	unsigned int chr[8];
	unsigned int prg[4];	
	
} Mmc3;


// Mapper 121
struct {
	unsigned int dummy[20];
	unsigned int exRegs[3];
} Mapper121;

// Mapper 134
struct {
	unsigned int dummy[20];
	unsigned int exReg;
} Mapper134;

// Mapper 205
struct {
	unsigned int dummy[20];
	unsigned int exReg;
} Mapper205;


struct {
	unsigned int dummy_mmc3[20];
	uint8 wram[0x4000];
	uint8 exPrg[4];
	uint8 exChr[8];
} waixing;


// Unl6035052
struct {
	unsigned int dummy_mmc3[20];
	uint8 exRegs;
} Unl6035052;

//UnlSHeroes
struct{
	unsigned int dummy_mmc3[20];
	uint8 exRegs;
	uint8 tekker;
}UnlSHeroes;

// Mapper 187
struct {
	unsigned int dummy_mmc3[20];
	uint8  exRegs[4];
	uint8  *wram;
} Mapper187;

// BMC-SuperHIK8in1
struct {
	unsigned int dummy_mmc3[20];
	uint8  exRegs[5];
	uint8  *wram;
} BmcHero;

// BMC-BmcSuper24in1
struct {
	unsigned int dummy_mmc3[20];
	uint8  exRegs[3];
} BmcSuper24in1;

//fk23c
struct {
	unsigned int dummy_mmc3[20];
	uint8  exRegs[8];
	unsigned int unromChr;
	uint8  mode;
} fk23c;

//BenshengBs5
struct {
	uint8  mode;
} BenshengBs5;

//BmcT262
struct {
	uint16 addrreg;
	uint8 datareg;
	uint8 busy;
} BmcT262;


//Bmc-8157
struct {
	uint16 cmdreg;
	uint8 invalid_data;
	uint8 bit;	
	unsigned int trash;
	unsigned int mode;
} Bmc8157;


struct {
	unsigned int reg[2];
	unsigned int bank;
} BmcCtc65;

struct {
	unsigned int regs[2];
	uint8 epromFirst;
	uint8  *wram;
} BmcSV16in1;

struct {
	unsigned int reg;
} BmcWs;

struct {
	uint16 cartMode;
	uint8 dipswitch;
} BmcD1038;

//BMC-64IN1NOREPEAT
struct {
	uint8 regs[4];
} y2k;

// UNL_N625092
struct {
	unsigned int regs[2];
} n625092;

// Game800in1
struct {
	unsigned int mode;
	unsigned int prg_bank4;
	unsigned int prg_bank6;
} Game800in1;

//Unl
struct {
	unsigned int dummy_mmc3[20];
	unsigned int exRegs[3];
	uint8 *wram;
} Unl;

}; // union
