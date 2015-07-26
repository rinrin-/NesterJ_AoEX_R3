#ifndef CHEAT_H
#define CHEAT_H


#define CHEAT_MAX_ADDRESS ( 64)
#define CHEAT_MAX_OPTIONS (192)
#define CHEAT_MAX_NAME	  (128)

#define QUOTE_MAX (128)	

#define CHEAT_MAX_LINE (1000)//最多读入100项金手指


struct CheatOption {
	char szOptionName[CHEAT_MAX_NAME];
	unsigned long AddressInfo[CHEAT_MAX_ADDRESS + 1];
};
typedef struct CheatOption CHEATOPTION;

struct CheatInfo {
	struct CheatInfo* pNext;
	struct CheatInfo* pPrevious;
	int nStatus;								// 0 = 关闭，1 = 打开
	int nCurrent;								// Currently selected option //指定选择的项目
	int nDefault;								// Default option
	char szCheatName[CHEAT_MAX_NAME];
	struct CheatOption* pOption[CHEAT_MAX_OPTIONS];
};
typedef struct CheatInfo CHEATINFO;

#endif
