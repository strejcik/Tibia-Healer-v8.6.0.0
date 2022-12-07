#include "pch.h"
#include "Menu.h"
#include "Healer.h"

void Menu::HandleMsgs()
{
	while (GetMessage(&messages, NULL, 0, 0)) {
		TranslateMessage(&messages);
		DispatchMessage(&messages);
		Sleep(0.1f);
	}
}

HMENU Menu::CreateDLLWindowMenu()
{
	HMENU hMenu;
	hMenu = CreateMenu();
	if (hMenu == NULL)
		return FALSE;
	HMENU hMenuPopup = CreatePopupMenu();
	AppendMenuW(hMenuPopup, MF_STRING, CONFIG_SAVE, TEXT("Save config"));
	AppendMenuW(hMenuPopup, MF_STRING, CONFIG_LOAD, TEXT("Load config"));
	AppendMenuW(hMenuPopup, MF_STRING, AUTO_CONFIG_LOAD, TEXT("Auto load config"));
	AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hMenuPopup, TEXT("Config"));
	return hMenu;
}

void CALLBACK Menu::MainTimerLoop(HWND hwnd, UINT uMsg, int32_t timerId, DWORD dwTime)
{
	if (bHealer)
	{
		Healer::GetInstance().HealWithSpells(lightSpell, midSpell, heavySpell);
		Healer::GetInstance().HealWithItems(healthPotionHealer, manaPotionHealer);
	}

	if (bHealFriendSio || bHealFriendUH) Healer::GetInstance().HealPlayer(safeHpForSio, healFriendOnHp, friendList, bHealFriendSio, selectedHealFriendMode);
}

void ClickCheckBox(HWND windowHandle)
{
	SendMessage(windowHandle, BM_CLICK, BST_PUSHED, 0);
	SendMessage(windowHandle, BM_CLICK, BST_PUSHED, 0);
}

void Menu::LoadConfig(LPWSTR filePath)
{
	std::ifstream file(filePath);
	std::string content;

	if (file.is_open())
	{
		//Light spell input
		std::getline(file, content);
		lightSpell.spell[0] = '\0';
		strcat_s(lightSpell.spell, content.c_str());

		//Light spell Hp percentage
		std::getline(file, content);
		lightSpell.hpPercentage = NULL;
		lightSpell.hpPercentage = atoi(content.c_str());

		//Light spell Mana cost
		std::getline(file, content);
		lightSpell.manaCost = NULL;
		lightSpell.manaCost = atoi(content.c_str());

		//Mid spell input
		std::getline(file, content);
		midSpell.spell[0] = '\0';
		strcat_s(midSpell.spell, content.c_str());

		//Mid spell hp percentage
		std::getline(file, content);
		midSpell.hpPercentage = NULL;
		midSpell.hpPercentage = atoi(content.c_str());

		//Mid spell mana cost
		std::getline(file, content);
		midSpell.manaCost = NULL;
		midSpell.manaCost = atoi(content.c_str());

		//Heavy spell input
		std::getline(file, content);
		heavySpell.spell[0] = '\0';
		strcat_s(heavySpell.spell, content.c_str());

		//Heavy spell hp percentage
		std::getline(file, content);
		heavySpell.hpPercentage = NULL;
		heavySpell.hpPercentage = atoi(content.c_str());

		//Heavy spell mana cost
		std::getline(file, content);
		heavySpell.manaCost = NULL;
		heavySpell.manaCost = atoi(content.c_str());


		//Health Potion Healer
		std::getline(file, content);
		healthPotionHealer.itemId = atoi(content.c_str());
		std::getline(file, content);
		healthPotionHealer.hpPerc = atoi(content.c_str());


		//Mana Potion Healer
		std::getline(file, content);
		manaPotionHealer.itemId = atoi(content.c_str());
		std::getline(file, content);
		manaPotionHealer.manaPerc = atoi(content.c_str());


		ClickCheckBox(cBoxEnableHealer);
	}
	else
	{
		MessageBox(NULL, L"File you tried to open doesn't exist, is wrong format or opened by another process.", L"Error Opening file.", MB_OK);
	}
	file.close();
}

void Menu::OpenFileExplorer(HWND hwnd)
{
	OPENFILENAME ofn;

	char fileName[100];

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = LPWSTR(fileName);
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(fileName);
	ofn.lpstrFilter = L"config files\0*.bot\0";
	ofn.nFilterIndex = 1;

	if (GetOpenFileName(&ofn))
	{
		LoadConfig(ofn.lpstrFile);
	}
}

void Menu::SaveConfig()
{
	if (!MemReader::GetInstance().IsOnline())
	{
		MessageBox(NULL, L"You need to be online to save config with player name.", L"Error saving config.", MB_OK);
		return;
	}
	CSelfCharacter selfCharacter;
	MemReader::GetInstance().ReadSelfCharacter(&selfCharacter);
	char FilePathToBot[100];
	GetModuleFileNameA(inj_hModule, FilePathToBot, sizeof(FilePathToBot));
	if (strrchr(FilePathToBot, '\\') != NULL)
		*strrchr(FilePathToBot, '\\') = 0;
	strcat_s(FilePathToBot, 100, "\\");
	strcat_s(FilePathToBot, 100, selfCharacter.name);
	strcat_s(FilePathToBot, 100, ".bot");

	std::ofstream file(FilePathToBot);

	if (file.is_open())
	{
		// Light spell
		file << &lightSpell.spell[0] << std::endl;
		file << lightSpell.hpPercentage << std::endl;
		file << lightSpell.manaCost << std::endl;

		//Mid spell
		file << &midSpell.spell[0] << std::endl;
		file << midSpell.hpPercentage << std::endl;
		file << midSpell.manaCost << std::endl;

		//Heave spell
		file << &heavySpell.spell[0] << std::endl;
		file << heavySpell.hpPercentage << std::endl;
		file << heavySpell.manaCost << std::endl;

		//Health Potion Healer
		file << healthPotionHealer.itemId << std::endl;
		file << healthPotionHealer.hpPerc << std::endl;

		//Mana Potion Healer
		file << manaPotionHealer.itemId << std::endl;
		file << manaPotionHealer.manaPerc << std::endl;
	}
	else
	{
		MessageBox(NULL, L"Saving config failed. Try again or report issue to developers.", L"Error saving config.", MB_OK);
	}
	file.close();
}

void Menu::AutoLoadConfig()
{
	if (!MemReader::GetInstance().IsOnline())
	{
		MessageBox(NULL, L"You need to be online to load config with player name.", L"Error auto loading config.", MB_OK);
		return;
	}
	CSelfCharacter selfCharacter;
	MemReader::GetInstance().ReadSelfCharacter(&selfCharacter);
	char FilePathToBot[100];
	GetModuleFileNameA(inj_hModule, FilePathToBot, sizeof(FilePathToBot));
	if (strrchr(FilePathToBot, '\\') != NULL)
		*strrchr(FilePathToBot, '\\') = 0;
	strcat_s(FilePathToBot, 100, "\\");
	strcat_s(FilePathToBot, 100, selfCharacter.name);
	strcat_s(FilePathToBot, 100, ".bot");

	wchar_t wtext[100];
	size_t numberCharsConverted;
	mbstowcs_s(&numberCharsConverted, wtext, FilePathToBot, strlen(FilePathToBot) + 1);
	LPWSTR ptr = wtext;

	LoadConfig(ptr);
}

void Menu::ReadSioListTxt(std::array<std::string, 100>& friendList)
{
	char FilePathToBot[100];
	GetModuleFileNameA(inj_hModule, FilePathToBot, sizeof(FilePathToBot));
	if (strrchr(FilePathToBot, '\\') != NULL)
		*strrchr(FilePathToBot, '\\') = 0;
	strcat_s(FilePathToBot, 100, "\\sioList.txt");

	int32_t arrayIndex = 0;
	std::ifstream file(FilePathToBot);
	std::string content;
	if (file.is_open())
	{
		for (int32_t i = 0; i < friendList.size(); i++)
		{
			std::getline(file, content);
			if (!content.empty())
			{
				friendList[arrayIndex] = content;
				arrayIndex++;
			}
		}
	}
	else
	{
		MessageBox(NULL, L"Failed to open sioList.txt. Check if sioList.txt exists in bot directory.", L"Error opening sioList.txt.", MB_OK);
	}
	file.close();
}

void Menu::OpenSioListTxt()
{
	char FilePathToBot[100];
	GetModuleFileNameA(inj_hModule, FilePathToBot, sizeof(FilePathToBot));
	if (strrchr(FilePathToBot, '\\') != NULL)
		*strrchr(FilePathToBot, '\\') = 0;
	strcat_s(FilePathToBot, sizeof(FilePathToBot), "\\sioList.txt");

	std::ifstream file(FilePathToBot);
	if (!file.is_open())
	{
		MessageBox(NULL, L"Failed to open sioList.txt. Check if sioList.txt exists in bot directory.", L"Error opening sioList.txt.", MB_OK);
	}
	else
	{
		file.close();
		ShellExecuteA(NULL, "open", FilePathToBot, "", FilePathToBot, SW_SHOW);
	}
}

LRESULT CALLBACK Menu::HealerMessageHandler(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	switch (uMessage)
	{
	case WM_CREATE:
		bHealerWindowOpen = true;
		CreateHealerMenu(hWindow);
		CheckDlgButton(hWindow, CLB_HEALER, bHealer);
		ToggleHealer();
		CheckDlgButton(hWindow, CLB_HEAL_FRIENDS_SIO, bHealFriendSio);
		ToggleSio();
		CheckDlgButton(hWindow, CLB_HEAL_FRIENDS_UH, bHealFriendUH);
		break;
	case WM_CLOSE:
		bHealerWindowOpen = false;
		break;
	case WM_DESTROY:
		//KillTimer(hWindow, 0);
		bHealerWindowOpen = false;
		//PostQuitMessage(0);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case CLB_HEALER:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_HEALER, BM_GETCHECK, 0, 0))
				{
					bHealer = !bHealer;
					CheckDlgButton(hWindow, CLB_HEALER, bHealer);
					ToggleHealer();
				}
				else
				{
					bHealer = !bHealer;
					CheckDlgButton(hWindow, CLB_HEALER, bHealer);
					ToggleHealer();
				}
				break;
			}
			break;
		case CLB_OPEN_SIO_LIST:
			OpenSioListTxt();
			break;
		case CLB_HEAL_FRIENDS_SIO:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_HEAL_FRIENDS_SIO, BM_GETCHECK, 0, 0))
				{
					bHealFriendSio = !bHealFriendSio;
					CheckDlgButton(hWindow, CLB_HEAL_FRIENDS_SIO, bHealFriendSio);
					ToggleSio();
				}
				else
				{
					bHealFriendSio = !bHealFriendSio;
					CheckDlgButton(hWindow, CLB_HEAL_FRIENDS_SIO, bHealFriendSio);
					ToggleSio();
				}
				break;
			}
			break;
		case CLB_HEAL_FRIENDS_UH:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_HEAL_FRIENDS_UH, BM_GETCHECK, 0, 0))
				{
					bHealFriendUH = !bHealFriendUH;
					CheckDlgButton(hWindow, CLB_HEAL_FRIENDS_UH, bHealFriendUH);
					ToggleSio();
				}
				else
				{
					bHealFriendUH = !bHealFriendUH;
					CheckDlgButton(hWindow, CLB_HEAL_FRIENDS_UH, bHealFriendUH);
					ToggleSio();
				}
				break;
			}
			break;
		case CLB_LIST_BOX_HEAL_MODES:
			switch (HIWORD(wParam))
			{
			case CBN_SELCHANGE:
				int32_t itemSelected = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
				if (selectedHealFriendMode == itemSelected)	break;
				selectedHealFriendMode = itemSelected;
				break;
			}
			break;
		}
		break;
	}
	return DefWindowProc(hWindow, uMessage, wParam, lParam);
}

LRESULT CALLBACK Menu::MessageHandler(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	switch (uMessage)
	{
	case WM_CREATE:
		SetTimer(hWindow, 0, 0, (TIMERPROC)&MainTimerLoop);
		break;
	case WM_CLOSE: break;
	case WM_DESTROY:
		KillTimer(hWindow, 0);
		if (f) fclose(f);
		FreeConsole();
		PostQuitMessage(0);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case MYMENU_EXIT:
			KillTimer(hWindow, 0);
			if (f) fclose(f);
			FreeConsole();
			PostQuitMessage(0);
			return 0;
			break;

		case CONFIG_LOAD:
			OpenFileExplorer(hWindow);
			break;

		case AUTO_CONFIG_LOAD:
			AutoLoadConfig();
			break;

		case CONFIG_SAVE:
			SaveConfig();
			break;

		case CLB_OPEN_HEALER_WINDOW:
		{
			if (!bHealerWindowOpen)
			{
				RECT rc;
				RegisterDLLWindowHealerClass();
				healerHWND = CreateWindowExA(0, "HealerWindowClass", "Healer", WS_EX_LAYERED | WS_MINIMIZEBOX, 100, 100, 326, 321, NULL, NULL, inj_hModule, NULL);
				GetWindowRect(healerHWND, &rc);
				int xPos = (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2;
				int yPos = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;
				SetWindowPos(healerHWND, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
				ShowWindow(healerHWND, SW_SHOWNORMAL);
				SetForegroundWindow(healerHWND);
			}
			break;
		}

		}
		break;
	}
	return DefWindowProc(hWindow, uMessage, wParam, lParam);
}

BOOL Menu::RegisterDLLWindowClass(const wchar_t szClassName[])
{
	HBRUSH hb = ::CreateSolidBrush(RGB(30, 144, 255));
	WNDCLASSEX wc;
	wc.hInstance = inj_hModule;
	wc.lpszClassName = (LPCWSTR)szClassName;
	wc.lpfnWndProc = MessageHandler;
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.cbSize = sizeof(wc);
	wc.hIcon = LoadIcon(NULL, IDI_ERROR);
	wc.hIconSm = LoadIcon(wc.hInstance, MAKEINTRESOURCE(101));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszMenuName = NULL;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = hb;
	if (!RegisterClassEx(&wc))
		return 0;
	return 1;
}

void Menu::RegisterDLLWindowHealerClass()
{

	WNDCLASSEX wcex;


	wcex.hInstance = inj_hModule;
	wcex.lpszClassName = L"HealerWindowClass";
	wcex.lpfnWndProc = HealerMessageHandler;
	wcex.style = CS_VREDRAW | CS_HREDRAW;
	wcex.cbSize = sizeof(wcex);
	wcex.hIcon = LoadIcon(NULL, IDI_ERROR);
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(101));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszMenuName = NULL;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);

	RegisterClassEx(&wcex);
}

HWND Menu::CreateGeneralCheckBox(const char* lpWindowName, int32_t x, int32_t y, int32_t iWidth, int32_t iHeight, int32_t hMenu, HWND hWnd)
{
	HWND hwnd = CreateWindowExA(0, "button", lpWindowName, WS_CHILD | WS_VISIBLE | BS_CHECKBOX, x, y, iWidth, iHeight, hWnd, (HMENU)hMenu, NULL, 0);
	return hwnd;
}
HWND Menu::CreateGeneralButton(const char* lpWindowName, int32_t x, int32_t y, int32_t iWidth, int32_t iHeight, int32_t hMenu, HWND hWnd)
{
	HWND hwnd = CreateWindowExA(0, "button", lpWindowName, WS_CHILD | WS_VISIBLE, x, y, iWidth, iHeight, hWnd, (HMENU)hMenu, NULL, 0);
	SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, true);
	return hwnd;
}
HWND Menu::CreateGeneralLabel(const char* lpWindowName, int32_t x, int32_t y, int32_t iWidth, int32_t iHeight, HWND hWnd)
{
	HWND hwnd = CreateWindowExA(0, "static", lpWindowName, WS_CHILD | WS_VISIBLE, x, y, iWidth, iHeight, hWnd, 0, NULL, 0);
	SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, true);
	return hwnd;
}
HWND Menu::CreateGeneralGroupBox(const char* lpWindowName, int32_t x, int32_t y, int32_t iWidth, int32_t iHeight, HWND hWnd)
{
	HWND hwnd = CreateWindowExA(0, "button", lpWindowName, WS_CHILD | WS_VISIBLE | BS_GROUPBOX, x, y, iWidth, iHeight, hWnd, 0, NULL, 0);
	return hwnd;
}
HWND Menu::CreateGeneralListBox(const char* lpWindowName, int32_t x, int32_t y, int32_t iWidth, int32_t iHeight, int32_t hMenu, HWND hWnd)
{
	HWND hwnd = CreateWindowExA(WS_EX_STATICEDGE, "listbox", lpWindowName, WS_VISIBLE | WS_CHILD | LBS_STANDARD | LBS_DISABLENOSCROLL, x, y, iWidth, iHeight, hWnd, (HMENU)hMenu, NULL, 0);
	return hwnd;
}
HWND Menu::CreateGeneralInputBox(const char* lpWindowName, int32_t x, int32_t y, int32_t iWidth, int32_t iHeight, HWND hWnd)
{
	HWND hwnd = CreateWindowExA(WS_EX_STATICEDGE, "edit", lpWindowName, WS_CHILD | WS_BORDER | WS_VISIBLE | ES_AUTOHSCROLL | ES_CENTER, x, y, iWidth, iHeight, hWnd, NULL, NULL, 0);
	SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, true);
	return hwnd;
}
HWND Menu::CreateGeneralComboBox(const char* lpWindowName, int32_t x, int32_t y, int32_t iWidth, int32_t iHeight, int32_t hMenu, HWND hWnd)
{
	HWND hwnd = CreateWindowExA(WS_EX_STATICEDGE, "combobox", lpWindowName, WS_VISIBLE | WS_CHILD | LBS_STANDARD | LBS_DISABLENOSCROLL, x, y, iWidth, iHeight, hWnd, (HMENU)hMenu, NULL, 0);
	return hwnd;
}

void Menu::Init(HMODULE hModule)
{
	inj_hModule = hModule;
	HMENU hMenu = CreateDLLWindowMenu();

	// Main Window
	RegisterDLLWindowClass(L"DLLWindowClass");
	mainHWND = CreateWindowExA(0, "DLLWindowClass", "Tibia v8.6.0.0", WS_EX_LAYERED | WS_MINIMIZEBOX, 0, 0, mainWindowWidth, mainWindowHeight, NULL, hMenu, inj_hModule, NULL);



	RECT rc;
	//retrieves the dimensions of the bounding rectangle
	GetWindowRect(mainHWND, &rc);

	//get center position for main window
	int xPos = (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2;
	int yPos = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;

	//set position of main window
	SetWindowPos(mainHWND, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

	//show main window
	ShowWindow(mainHWND, SW_SHOWNORMAL);

	//display main window on top
	SetForegroundWindow(mainHWND);



	CreateMainMenu();
}
HWND Menu::CreateButton(const char* lpWindowName, int32_t x, int32_t y, int32_t iWidth, int32_t iHeight, int32_t hMenu)
{
	HWND hwnd = CreateWindowExA(0, "button", lpWindowName, WS_CHILD | WS_VISIBLE, x, y, iWidth, iHeight, mainHWND, (HMENU)hMenu, inj_hModule, 0);
	SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, true);
	return hwnd;
}

void Menu::CreateMainMenu()
{
	buttonHealer = CreateButton("Healer", 10, 10, 100, 20, CLB_OPEN_HEALER_WINDOW);
}

void Menu::CreateHealerMenu(HWND hWindow)
{

	// REGION HEALING

	std::string LIGHT_HEAL_SPELL_PERC = std::to_string(lightSpell.hpPercentage);
	char const* LHPERC = LIGHT_HEAL_SPELL_PERC.c_str();
	std::string MID_HEAL_SPELL_PERC = std::to_string(midSpell.hpPercentage);
	char const* MHPERC = MID_HEAL_SPELL_PERC.c_str();
	std::string HEAVY_HEAL_SPELL_PERC = std::to_string(heavySpell.hpPercentage);
	char const* HHPERC = HEAVY_HEAL_SPELL_PERC.c_str();


	std::string LIGHT_HEAL_SPELL_MP = std::to_string(lightSpell.manaCost);
	char const* LHSMP = LIGHT_HEAL_SPELL_MP.c_str();
	std::string MID_HEAL_SPELL_MP = std::to_string(midSpell.manaCost);
	char const* MHSMP = MID_HEAL_SPELL_MP.c_str();
	std::string HEAVY_HEAL_SPELL_MP = std::to_string(heavySpell.manaCost);
	char const* HHSMP = HEAVY_HEAL_SPELL_MP.c_str();


	std::string HEALTH_POTION_ID = std::to_string(healthPotionHealer.itemId);
	char const* HPID = HEALTH_POTION_ID.c_str();
	std::string HEALTH_POTION_HPPERC = std::to_string(healthPotionHealer.hpPerc);
	char const* HPPERC = HEALTH_POTION_HPPERC.c_str();


	std::string MANA_POTION_ID = std::to_string(manaPotionHealer.itemId);
	char const* MPID = MANA_POTION_ID.c_str();
	std::string MANA_POTION_HPPERC = std::to_string(manaPotionHealer.manaPerc);
	char const* MPPERC = MANA_POTION_HPPERC.c_str();

	std::string FRIEND_HP_HPPERC = std::to_string(sioUhPercent.friendHpPercentage);
	char const* FHPPERC = FRIEND_HP_HPPERC.c_str();
	std::string MY_HP_HPPERC = std::to_string(sioUhPercent.myHpPercentage);
	char const* MYHPPERC = MY_HP_HPPERC.c_str();

	CreateGeneralGroupBox("Healing Spells / Potions", 5, 0, 310, 188, hWindow);

	cBoxEnableHealer = CreateGeneralCheckBox("", 172, 0, 15, 20, CLB_HEALER, hWindow);

	CreateGeneralLabel("Light:", 10, 25, 40, 20, hWindow);
	CreateGeneralLabel("Mid:", 10, 55, 25, 20, hWindow);
	CreateGeneralLabel("Heavy:", 10, 85, 40, 20, hWindow);

	inputLightSpell.inputSpell = CreateGeneralInputBox(&lightSpell.spell[0], 60, 25, 100, 20, hWindow);
	inputMidSpell.inputSpell = CreateGeneralInputBox(&midSpell.spell[0], 60, 55, 100, 20, hWindow);
	inputHeavySpell.inputSpell = CreateGeneralInputBox(&heavySpell.spell[0], 60, 85, 100, 20, hWindow);

	CreateGeneralLabel("%", 170, 25, 20, 20, hWindow);
	CreateGeneralLabel("%", 170, 55, 20, 20, hWindow);
	CreateGeneralLabel("%", 170, 85, 20, 20, hWindow);

	inputLightSpell.inputHpPerc = CreateGeneralInputBox(LHPERC, 190, 25, 30, 20, hWindow);
	inputMidSpell.inputHpPerc = CreateGeneralInputBox(MHPERC, 190, 55, 30, 20, hWindow);
	inputHeavySpell.inputHpPerc = CreateGeneralInputBox(HHPERC, 190, 85, 30, 20, hWindow);

	CreateGeneralLabel("MP:", 230, 25, 30, 20, hWindow);
	CreateGeneralLabel("MP:", 230, 55, 30, 20, hWindow);
	CreateGeneralLabel("MP:", 230, 85, 30, 20, hWindow);

	inputLightSpell.inputMana = CreateGeneralInputBox(LHSMP, 260, 25, 45, 20, hWindow);
	inputMidSpell.inputMana = CreateGeneralInputBox(MHSMP, 260, 55, 45, 20, hWindow);
	inputHeavySpell.inputMana = CreateGeneralInputBox(HHSMP, 260, 85, 45, 20, hWindow);

	CreateGeneralLabel("Health Potion:", 10, 120, 100, 20, hWindow);
	inputHealthPotion.inputItemId = CreateGeneralInputBox(HPID, 100, 120, 60, 20, hWindow);
	CreateGeneralLabel("HP", 170, 120, 50, 20, hWindow);
	inputHealthPotion.inputHpPerc = CreateGeneralInputBox(HPPERC, 200, 120, 30, 20, hWindow);

	CreateGeneralLabel("Mana Potion:", 10, 150, 100, 20, hWindow);
	inputManaPotion.inputItemId = CreateGeneralInputBox(MPID, 100, 150, 60, 20, hWindow);
	CreateGeneralLabel("MP", 170, 150, 50, 20, hWindow);
	inputManaPotion.inputMana = CreateGeneralInputBox(MPPERC, 200, 150, 30, 20, hWindow);


	CreateGeneralGroupBox("Heal Friends", 5, 188, 310, 100, hWindow);

	buttonSioList = CreateGeneralButton("Sio List", 10, 208, 100, 20, CLB_OPEN_SIO_LIST, hWindow);

	CreateGeneralLabel("FriendHP:", 10, 233, 80, 20, hWindow);
	inputSioHp.inputHpPerc = CreateGeneralInputBox(FHPPERC, 75, 233, 30, 20, hWindow);
	CreateGeneralLabel("%", 110, 233, 20, 20, hWindow);

	CreateGeneralLabel("Sio:", 140, 233, 80, 20, hWindow);
	cBoxHealFriendsSio = CreateGeneralCheckBox("", 170, 233, 10, 20, CLB_HEAL_FRIENDS_SIO, hWindow);

	CreateGeneralLabel("MyHP:", 10, 258, 80, 20, hWindow);
	inputSioMyHp.inputHpPerc = CreateGeneralInputBox(MYHPPERC, 75, 258, 30, 20, hWindow);
	CreateGeneralLabel("%", 110, 258, 20, 20, hWindow);

	CreateGeneralLabel("UH:", 140, 258, 80, 20, hWindow);
	cBoxHealFriendsUH = CreateGeneralCheckBox("", 170, 258, 10, 20, CLB_HEAL_FRIENDS_UH, hWindow);

	lBoxHealFriendsOptions = CreateGeneralComboBox("", 210, 208, 100, 70, CLB_LIST_BOX_HEAL_MODES, hWindow);

	for (int32_t i = 0; i < sizeof(healFriendOptions) / sizeof(healFriendOptions[0]); i++)
	{
		SendMessageA(lBoxHealFriendsOptions, CB_ADDSTRING, 0, (LPARAM)healFriendOptions[i].name);
	}

	SendMessage(lBoxHealFriendsOptions, CB_SETCURSEL, selectedHealFriendMode, (LPARAM)0);

}

void Menu::ToggleSio()
{
	if (bHealFriendSio || bHealFriendUH)
	{
		if (GetWindowTextA(inputSioHp.inputHpPerc, &buf[0], sizeof(buf)))
		{
			healFriendOnHp = atoi(buf);
			sioUhPercent.friendHpPercentage = atoi(buf);
			if (GetWindowTextA(inputSioMyHp.inputHpPerc, &buf[0], sizeof(buf)))
			{
				safeHpForSio = atoi(buf);
				sioUhPercent.myHpPercentage = atoi(buf);
			}
			else
			{
				safeHpForSio = NULL;
			}
		}
		else
		{
			safeHpForSio = NULL;
			healFriendOnHp = NULL;
			sioUhPercent.myHpPercentage = NULL;
			sioUhPercent.friendHpPercentage = NULL;
		}
		if (selectedHealFriendMode == 0)
		{
			ReadSioListTxt(friendList);
		}
	}
	if (bHealFriendSio)
	{
		EnableWindow(inputSioHp.inputHpPerc, !bHealFriendSio);
		EnableWindow(inputSioMyHp.inputHpPerc, !bHealFriendSio);
		EnableWindow(cBoxHealFriendsUH, !bHealFriendSio);
		EnableWindow(buttonSioList, !bHealFriendSio);
		EnableWindow(lBoxHealFriendsOptions, !bHealFriendSio);
	}
	else if (bHealFriendUH)
	{
		EnableWindow(inputSioHp.inputHpPerc, !bHealFriendUH);
		EnableWindow(inputSioMyHp.inputHpPerc, !bHealFriendUH);
		EnableWindow(cBoxHealFriendsSio, !bHealFriendUH);
		EnableWindow(buttonSioList, !bHealFriendUH);
		EnableWindow(lBoxHealFriendsOptions, !bHealFriendUH);
	}
	else
	{
		EnableWindow(inputSioHp.inputHpPerc, true);
		EnableWindow(inputSioMyHp.inputHpPerc, true);
		EnableWindow(cBoxHealFriendsUH, true);
		EnableWindow(cBoxHealFriendsSio, true);
		EnableWindow(buttonSioList, true);
		EnableWindow(lBoxHealFriendsOptions, true);
	}
}

void Menu::ToggleHealer()
{
	if (bHealer)
	{
		lightSpell.spellLenght = GetWindowTextA(inputLightSpell.inputSpell, &lightSpell.spell[0], sizeof(lightSpell.spell));
		if (lightSpell.spellLenght)
		{
			GetWindowTextA(inputLightSpell.inputMana, &buf[0], sizeof(buf));
			lightSpell.manaCost = atoi(buf);
			GetWindowTextA(inputLightSpell.inputHpPerc, &buf[0], sizeof(buf));
			lightSpell.hpPercentage = atoi(buf);
		}
		else
		{
			lightSpell.manaCost = 0;
			lightSpell.hpPercentage = 0;
		}

		midSpell.spellLenght = GetWindowTextA(inputMidSpell.inputSpell, &midSpell.spell[0], sizeof(midSpell.spell));
		if (midSpell.spellLenght)
		{
			GetWindowTextA(inputMidSpell.inputMana, &buf[0], sizeof(buf));
			midSpell.manaCost = atoi(buf);
			GetWindowTextA(inputMidSpell.inputHpPerc, &buf[0], sizeof(buf));
			midSpell.hpPercentage = atoi(buf);
		}
		else
		{
			midSpell.manaCost = 0;
			midSpell.hpPercentage = 0;
		}

		heavySpell.spellLenght = GetWindowTextA(inputHeavySpell.inputSpell, &heavySpell.spell[0], sizeof(heavySpell.spell));
		if (heavySpell.spellLenght)
		{
			GetWindowTextA(inputHeavySpell.inputMana, &buf[0], sizeof(buf));
			heavySpell.manaCost = atoi(buf);
			GetWindowTextA(inputHeavySpell.inputHpPerc, &buf[0], sizeof(buf));
			heavySpell.hpPercentage = atoi(buf);
		}
		else
		{
			heavySpell.manaCost = 0;
			heavySpell.hpPercentage = 0;
		}

		if (GetWindowTextA(inputHealthPotion.inputItemId, &buf[0], sizeof(buf)))
		{
			healthPotionHealer.itemId = atoi(buf);

			if (GetWindowTextA(inputHealthPotion.inputHpPerc, &buf[0], sizeof(buf)))
			{
				healthPotionHealer.hpPerc = atoi(buf);
			}
			else
			{
				healthPotionHealer.hpPerc = NULL;
			}
		}
		else
		{
			healthPotionHealer.itemId = NULL;
		}

		if (GetWindowTextA(inputManaPotion.inputItemId, &buf[0], sizeof(buf)))
		{
			manaPotionHealer.itemId = atoi(buf);
			if (GetWindowTextA(inputManaPotion.inputMana, &buf[0], sizeof(buf)))
			{
				manaPotionHealer.manaPerc = atoi(buf);
			}
			else
			{
				manaPotionHealer.manaPerc = NULL;
			}
		}
		else
		{
			manaPotionHealer.itemId = NULL;
		}
	}
	EnableWindow(inputLightSpell.inputSpell, !bHealer);
	EnableWindow(inputMidSpell.inputSpell, !bHealer);
	EnableWindow(inputHeavySpell.inputSpell, !bHealer);

	EnableWindow(inputLightSpell.inputMana, !bHealer);
	EnableWindow(inputMidSpell.inputMana, !bHealer);
	EnableWindow(inputHeavySpell.inputMana, !bHealer);

	EnableWindow(inputLightSpell.inputHpPerc, !bHealer);
	EnableWindow(inputMidSpell.inputHpPerc, !bHealer);
	EnableWindow(inputHeavySpell.inputHpPerc, !bHealer);

	EnableWindow(inputHealthPotion.inputItemId, !bHealer);
	EnableWindow(inputHealthPotion.inputHpPerc, !bHealer);

	EnableWindow(inputManaPotion.inputItemId, !bHealer);
	EnableWindow(inputManaPotion.inputMana, !bHealer);
}
