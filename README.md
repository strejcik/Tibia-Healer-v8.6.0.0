# Tibia Healer for Tibia 8.6

### Features

- Healer: light / mid / heavy
- Auto potioner: health potion / mana potion
- Heal friends: 
    + List
    + Guild
    + Party
	+ Hearts

![](https://i.imgur.com/gUZxk23.png)
### Usage
- Download project and compile as x86 (Release).

Or

- There is "Tibia-Bot.exe" and "Tibia-Bot.dll" inside Release folder which is already compiled and ready to go.

`Run Tibia 8.6`

`Run Tibia-Bot.exe`

### Information

- If you want to use potions like SMP, SHP, UMP, UHP then you need to know the id's of those items:
[Item Id](https://tibia.fandom.com/wiki/Item_IDs "Item id")

 - Healing
    + [List]:  create sioList.txt in same folder with "Tibia-Bot.exe" and "Tibia-Bot.dll". Add nicknames to that list so the players can be healed.
    + [Guild]: every player which is from your guild and is on your screen will be healed.
    + [Party] :every player which is from your party and is on your screen will be healed.
	+ [Hearts] :every player which is from your vip list, has heart icon and is on your screen will be healed.

- "Tibia-Bot.exe" and "Tibia-Bot.dll" are 100% clean and free of viruses. Anti-virus might claim that those files are malicious/dangerous it's because there is reading/writing in Tibia memory. It's false positive. Also if you have your anti virus running it might block "Tibia-Bot.exe" or "Tibia-Bot.dll", so you can add those 2 files for exceptions in anti-virus settings.

- Healing with Sio / UH rune is not perfect, because there is no any check in code about position of both players and check if casted spell / used rune is not against any obstacle between players. Meaning, if there is any obstacle like closed doors / wall between two players then healer will cast spell / try to uh player without result. But self healer and auto potioner works just fine.
