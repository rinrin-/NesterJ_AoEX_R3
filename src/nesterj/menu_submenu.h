#ifndef MENU_SUBMENU_H
#define MENU_SUBMENU_H

extern int StateSlot;

void submenu_diskchange(void);

void submenu_preferencesconfig(void);
void submenu_graphicsconfig(void);
void submenu_soundconfig(void);
void submenu_colorconfig(void);
void submenu_keyconfig(void);
void submenu_menucolorconfig(void);
void submenu_configcheat(void);
void submenu_rewindconfig(void); //davex

// Msgbox types
#define MB_OK		0x0001L
#define MB_OKCANCEL	0x0002L
#define MB_YESNO	0x0003L

// Msgbox ret val
#define IDOK                0x01
#define IDCANCEL            0x02
#define IDYES               0x01
#define IDNO                0x02
u32 MessageBox(char *pszMsg, u32 color, u32 ulTypes);
u32 MessageBoxGbk12(char*pszMsg, u32 color, u32 ulTypes);

#define STATE_SLOT_MAX 10	// 0-9


#endif
