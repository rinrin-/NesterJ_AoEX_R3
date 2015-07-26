#ifndef MENU_H
#define MENU_H

extern int StateSlot;
extern u8 g_bEnableBitmap;
extern unsigned short g_bgBitmap[480*272];

void save_config(void);
void load_config(void);
void load_menu_bg(void);
void bgbright_change(void);

void setkeydefault(void);

void GetBatteryInfoString(char *msg, int *color);

int save_state(void);
int load_state(void);
long load_rom(const char *pszFile);
void menu_frame(const char *msg0, const char *msg1);
u8 nesterj_menu(void);

#endif
