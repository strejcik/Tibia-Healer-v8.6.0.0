#pragma once


#define AUTO_CONFIG_LOAD (97)
#define CONFIG_SAVE (98)
#define CONFIG_LOAD (99)
#define MYMENU_EXIT (100)
#define CLB_HEALER (101)
#define CLB_SIO (115)
#define CLB_HEAL_FRIENDS (128)
#define CLB_HEAL_FRIENDS_SIO (129)
#define CLB_HEAL_FRIENDS_UH (130)
#define CLB_HEAL_FRIENDS_PARTY (131)
#define CLB_HEAL_FRIENDS_WAR (132)
#define CLB_OPEN_SIO_LIST (138)
#define CLB_LIST_BOX_HEAL_MODES (143)

#define CLB_OPEN_HEALER_WINDOW (1000)

struct InputSpellLabel
{
	HWND inputSpell;
	HWND inputMana;
	HWND inputHpPerc;
};

struct InputItemLabel
{
	HWND inputItemId;
	HWND inputManaPerc;
	HWND inputHpPerc;
	HWND boolEquipDelay;
};

struct InputTimerLabel
{
	HWND inputTime;
};


struct InputItemHealer
{
	HWND inputItemId;
	HWND inputMana;
	HWND inputHpPerc;
};

struct InputLowInfo
{
	HWND inputBox;
	int32_t lowInfo;
};

namespace Menu
{

	static CSelfCharacter selfCharacter;
	static uintptr_t moduleBase = (uintptr_t)GetModuleHandle(NULL);
	static Self* localPlayer = (Self*)(moduleBase + Offsets::selfPlayer);
	static Self selfPlayer;


	static FILE* f;

	BOOL RegisterDLLWindowClass(const wchar_t szClassName[]);
	void RegisterDLLWindowHealerClass();
	HMENU CreateDLLWindowMenu();
	LRESULT CALLBACK MessageHandler(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam);
	void CALLBACK MainTimerLoop(HWND hwnd, UINT uMsg, int32_t timerId, DWORD dwTime);

	void Init(HMODULE hModule);
	void HandleMsgs();
	void ToggleHealer();
	void ToggleSio();
	

	
	HWND CreateButton(const char* lpWindowName, int32_t x, int32_t y, int32_t iWidth, int32_t iHeight, int32_t hMenu);
	

	HWND CreateGeneralCheckBox(const char* lpWindowName, int32_t x, int32_t y, int32_t iWidth, int32_t iHeight, int32_t hMenu, HWND hWnd);
	HWND CreateGeneralButton(const char* lpWindowName, int32_t x, int32_t y, int32_t iWidth, int32_t iHeight, int32_t hMenu, HWND hWnd);
	HWND CreateGeneralLabel(const char* lpWindowName, int32_t x, int32_t y, int32_t iWidth, int32_t iHeight, HWND hWnd);
	HWND CreateGeneralGroupBox(const char* lpWindowName, int32_t x, int32_t y, int32_t iWidth, int32_t iHeight, HWND hWnd);
	HWND CreateGeneralListBox(const char* lpWindowName, int32_t x, int32_t y, int32_t iWidth, int32_t iHeight, int32_t hMenu, HWND hWnd);
	HWND CreateGeneralInputBox(const char* lpWindowName, int32_t x, int32_t y, int32_t iWidth, int32_t iHeight, HWND hWnd);
	HWND CreateGeneralComboBox(const char* lpWindowName, int32_t x, int32_t y, int32_t iWidth, int32_t iHeight, int32_t hMenu, HWND hWnd);

	void CreateMainMenu();
	void CreateHealerMenu(HWND hWindow);
	void OpenFileExplorer(HWND hwnd);
	void LoadConfig(LPWSTR filePath);
	void AutoLoadConfig();
	void SaveConfig();
	void OpenSioListTxt();
	void ReadSioListTxt(std::array<std::string, 100>& friendList);
	LRESULT CALLBACK HealerMessageHandler(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam);

	static HWND lBoxAmulets;
	static HWND lBoxRings;

	// Global variables
	const int32_t mainWindowWidth = 456;
	const int32_t mainWindowHeight = 115;

	static HMODULE inj_hModule;
	static HWND mainHWND;
	static HWND healerHWND;
	static MSG messages;
	const HFONT hFont = CreateFontA(16,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		("Arial"));                 // lpszFacename


	
	static HWND cBoxEnableHealer;


	static HWND cBoxKillTarget;
	static InputSpellLabel inputTargetSpell;

	
	static HWND buttonSioList;
	static HWND buttonHealer;
	
	
	static HWND cBoxHealFriendsSio;
	static HWND cBoxHealFriendsUH;
	static HWND lBoxHealFriendsOptions;

	


	static InputLowInfo inputLowMana;
	static InputLowInfo inputLowHealth;

	static InputSpellLabel inputLightSpell;
	static InputSpellLabel inputMidSpell;
	static InputSpellLabel inputHeavySpell;

	static InputItemHealer inputHealthPotion;
	static InputItemHealer inputManaPotion;

	static HealthPotionItemHealer healthPotionHealer;
	static ManaPotionItemHealer manaPotionHealer;


	
	static InputSpellLabel inputSioHp;
	static InputSpellLabel inputSioMyHp;

	
	static char buf[30];

	static int32_t safeHpForSio;
	static int32_t healFriendOnHp;

	
	
	static LightSpell lightSpell;
	static MidSpell midSpell;
	static HeavySpell heavySpell;
	static SioUhPercent sioUhPercent;


	

	

	static InputTimerLabel inputTimerHealingSpells;
	static InputTimerLabel inputTimerHealingItems;



	static bool bHealerWindowOpen = false;


	
	static bool bHealer = false;
	static bool bHealSio = false;
	static bool bHealFriendSio = false;
	static bool bHealFriendUH = false;
	


	static int32_t selectedHealFriendMode = 0;

	static BalanceItemOptions healFriendOptions[] = { {"List", 0}, {"Guild", 1}, {"Party", 2}, {"Hearts", 3} };

	static int32_t selectedPartyInviteMode = 0;

	static BalanceItemOptions partyInviteOptions[] = { {"Guild", 0}, {"Hearts", 1}, {"Message", 2} };

	static std::array<std::string, 100> friendList = {};
};
