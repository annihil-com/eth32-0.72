// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

// kobject: the idea is to put our own console commands in here, and 
// leave hooked engine/cgame commands in CEngine


// Member functions are from Mars, ordinary functions are from Venus.

#include "eth32.h"
#include "settings.h"

CXCommands CCommands;

void xcmd_Tracker()
{
	Portal.ConsoleCommand();
}

void xcmd_Namesteal()
{
	char buf[32];
	Syscall.Cvar_VariableStringBuffer("g_punkbuster", buf, 32);
	Tools.Namesteal(atoi(buf) != 0);
}

void xcmd_HackSpam()
{	// sol: added simple spam, feel free to modify however you like
	Gui.BufferConsoleCommand("say \"^2[ ^dETH32 ^ov" ETH32_VERSION " ^2by ^dCheatersUtopia^o.^dcom ^2& ^dnixCoders^o.^dorg ^2]\n\"");
}

void xcmd_MusicSpam()
{
	if (!Winamp.WinampInstance()){
		Syscall.UI_Print("^1Error: ^owinamp not running!\n");
		return;
	}

	Winamp.MakeSpam(true);
}

void xcmd_UpdateMusicSpamFmt()
{
	if (Syscall.UI_Argc() == 1){
		Syscall.UI_Print("^dSyntax:          ^o/eth_musicspamfmt ^2[^9\"format\"^2]\n\n");
		Syscall.UI_Print("^dSwitches:        ^2[^9w^2]  ^owhole unformatted title\n");
		Syscall.UI_Print("                 ^2[^9t^2]  ^osong title\n");
		Syscall.UI_Print("                 ^2[^9a^2]  ^oartist\n");
		Syscall.UI_Print("                 ^2[^9b^2]  ^obitrate\n");
		Syscall.UI_Print("                 ^2[^9c^2]  ^ochannels\n");
		Syscall.UI_Print("                 ^2[^9s^2]  ^osample rate\n");
		Syscall.UI_Print("                 ^2[^9l^2]  ^otrack length (in msec)\n");
		Syscall.UI_Print("                 ^2[^9L^2]  ^oformatted track length (mm:ss)\n");
		Syscall.UI_Print("                 ^2[^9p^2]  ^ocurrent position (in msec)\n");
		Syscall.UI_Print("                 ^2[^9P^2]  ^oformatted current position (mm:ss)\n\n");
		Syscall.UI_Print("^dCurrent Format:  ^7\"%s^7\"\n", Winamp.GetFormatString());
		return;
	}

	Winamp.UpdateSpamFormat((char *)Syscall.UI_Argv(1));
	Syscall.UI_Print("^o*** ^dMusic spam updated.\n");
}

//[n] - colored target name
//[N] - clean target name
//[d] - target deaths by me
//[D] - total target deaths
//[K] - my total kills
//[s] - my current spree
//[S] - my longest spree
//[w] - my current weapon

void xcmd_KillSpamFmt()
{
	if (Syscall.UI_Argc() == 1) {
		Syscall.UI_Print("^dSyntax:          ^o/eth_killspamfmt ^2[^9\"format\"^2]\n\n");
		Syscall.UI_Print("^dSwitches:        ^2[^9w^2]  ^owhole unformatted title\n");
		Syscall.UI_Print("                 ^2[^9n^2]  ^ocolored target name\n");
		Syscall.UI_Print("                 ^2[^9N^2]  ^oclean target name\n");
		Syscall.UI_Print("                 ^2[^9d^2]  ^otarget deaths by me\n");
		Syscall.UI_Print("                 ^2[^9D^2]  ^ototal target deaths\n");
		Syscall.UI_Print("                 ^2[^9K^2]  ^omy total kills\n");
		Syscall.UI_Print("                 ^2[^9s^2]  ^omy current spree\n");
		Syscall.UI_Print("                 ^2[^9S^2]  ^omy longest spree\n");
		Syscall.UI_Print("                 ^2[^9w^2]  ^omy current weapon\n\n");
		Syscall.UI_Print("^dCurrent Format:  ^7\"%s^7\"\n", Tools.GetKillSpamFormat());
		return;
	}
	Tools.UpdateKillSpamFormat((char *)Syscall.UI_Argv(1));
	Syscall.UI_Print("^o*** ^dKill spam updated.\n");
}

void loadCustomHitbox()
{
	if (Syscall.UI_Argc() == 1){
		Syscall.UI_Print("^dSyntax: ^o/command ^2[^9list^2/^9stand^2] ^1x^9,^1y^9,^1z\n");
		return;
	}

	char *cmd = _strdup(Syscall.UI_Argv(1));
	char *newValue;
	vec3_t vec;
	
	if (!strcmp(cmd,"list")){
		hitbox_t *hbox;
		if( eth32.settings.hitboxType == HITBOX_CUSTOM )			
			hbox = &Aimbot.customHitbox;
		else
			hbox = &head_hitboxes[eth32.settings.hitboxType];

		Syscall.UI_Print("^dstand          ^9{^2% .1f, % .1f, % .1f^9}\n", 
			hbox->stand_offset[0], hbox->stand_offset[1], hbox->stand_offset[2]);
		Syscall.UI_Print("^drun            ^9{^2% .1f, % .1f, % .1f^9}\n", 
			hbox->stand_offset_moving[0], hbox->stand_offset_moving[1], hbox->stand_offset_moving[2]);
		Syscall.UI_Print("^dcrouch         ^9{^2% .1f, % .1f, % .1f^9}\n", 
			hbox->crouch_offset[0], hbox->crouch_offset[1], hbox->crouch_offset[2]);
		Syscall.UI_Print("^dcrawl          ^9{^2% .1f, % .1f, % .1f^9}\n", 
			hbox->crouch_offset_moving[0], hbox->crouch_offset_moving[1], hbox->crouch_offset_moving[2]);
		Syscall.UI_Print("^dprone          ^9{^2% .1f, % .1f, % .1f^9}\n", 
			hbox->prone_offset[0], hbox->prone_offset[1], hbox->prone_offset[2]);
		Syscall.UI_Print("^dsize           ^9{^2% .1f, % .1f, % .1f^9}\n", 
			hbox->size[0], hbox->size[1], hbox->size[2]);

		free(cmd);
		return;
	}

	if (Syscall.UI_Argc() != 3 ){
		Syscall.UI_Print("^1Error: ^oinsufficient arguments\n");
		free(cmd);
		return;
	} else
		newValue = _strdup(Syscall.UI_Argv(2));

	VectorCopy(vec3_origin, vec);
	sscanf_s(newValue,"%f,%f,%f", &vec[0], &vec[1], &vec[2]);

	if (!strcmp(cmd, "stand")) {
		VectorCopy(vec,Aimbot.customHitbox.stand_offset);
	} else if (!strcmp(cmd, "run")) {
		VectorCopy(vec,Aimbot.customHitbox.stand_offset_moving);
	} else if (!strcmp(cmd, "crouch")) {
		VectorCopy(vec,Aimbot.customHitbox.crouch_offset);
	} else if (!strcmp(cmd, "crawl")) {
		VectorCopy(vec,Aimbot.customHitbox.crouch_offset_moving);
	} else if (!strcmp(cmd, "prone")) {
		VectorCopy(vec,Aimbot.customHitbox.prone_offset);
	} else if (!strcmp(cmd, "size")) {
		VectorCopy(vec,Aimbot.customHitbox.size);
	} else
		Syscall.UI_Print("^1Error: ^ounknown command\n");

	free(cmd);
	free(newValue);
}

void goHome()
{
	Syscall.UI_ExecuteText("connect 87.99.33.29:27962\n");
}

void xcmd_Crash()
{
	_asm 
	{
		mov eax, 0x123
		mov [eax], 1
	}
}

void formatSetting(char *out, size_t o_s, const settingdef_t *setting)
{
	switch (setting->type) {
		case SETTING_INT:
			sprintf_s(out, o_s, "^9%i", *(int *)setting->target);
			break;
		case SETTING_FLOAT:
			sprintf_s(out, o_s, "^9%.3f", *(float *)setting->target);
			break;
		case SETTING_BOOL:
			(*(bool *)setting->target) ? strcpy_s(out, o_s, "^2true") : strcpy_s(out, o_s, "^1false");
			break;
		case SETTING_VEC3:
		{
			float *v = (float*)setting->target;
			sprintf_s(out, o_s, "^2{^9%.3f^2, ^9%.3f^2, ^9%.3f^2}", v[0], v[1], v[2]);
			break;
		}
		case SETTING_VEC4:
		{
			float *v = (float*)setting->target;
			sprintf_s(out, o_s, "^2{^9%.3f^2, ^9%.3f^2, ^9%.3f^2, ^9%.3f^2}", v[0], v[1], v[2], v[3]);
			break;
		}
		case SETTING_BYTE3:
		{
			BYTE *b = (BYTE*)setting->target;
			sprintf_s(out, o_s, "^9%3i %3i %3i", b[0], b[1], b[2]);
			break;
		}
		case SETTING_STRING:
		{
			sprintf_s(out, o_s, "^o%s", (char *)setting->target);
			break;
		}
		default:
			strcpy_s(out, o_s, "^3unknown type");
			break;
	}
}

// handles main settings only for now
void xcmd_Settings()
{
	Settings.ConsoleCommand();
}

void xcmd_Toggle()
{
	Settings.Toggle();
}

void xcmd_CvarForce()
{
	if (Syscall.UI_Argc() == 1){
		Syscall.UI_Print("^dCvar Force List\n");
		cvarInfo_t *c = Tools.userCvarList;
		int i = 0; 
		while (c) {
			Syscall.UI_Print("^3%03i  ^2%16s   ^9\"^o%s^9\"\n",i, c->name, c->ourValue);
			c = c->next;
			i++;
		}

		return;
	} else if (Syscall.UI_Argc() == 2){
		const char *s = Syscall.UI_Argv(1);
		cvarInfo_t *c = Tools.userCvarList;
		while (c) {
			if (!_stricmp(s, c->name))
				Syscall.UI_Print("^2%16s   ^9\"^o%s^9\"\n", c->name, c->ourValue);
			c = c->next;
		}
		return;
	}

	char *cvarName = _strdup(Syscall.UI_Argv(1));
	char *val = _strdup(Syscall.UI_Argv(2));

	// delete a cvar (( FIX the 0th cvar)
	if (!_stricmp(cvarName, "del")){
		int num = atoi(val);
		if (num > 0){
			if (!IsBadReadPtr(&Tools.userCvarList[num], sizeof(cvarInfo_t))){
				if (Tools.userCvarList[num].next)
					Tools.userCvarList[num-1].next = &Tools.userCvarList[num+1];
				else
					Tools.userCvarList[num-1].next = NULL;

				Syscall.UI_Print("^dDeleted ^2%s\n", Tools.userCvarList[num].name);
			}
		}		
	}

	// see if we should update an already present cvar
	bool found = false;
	cvarInfo_t *c = Tools.userCvarList;
	while (c) {
		if (!_stricmp(cvarName, c->name)){
			strncpy_s(c->ourValue, MAX_STRING_CHARS, val, _TRUNCATE);
			Syscall.UI_Print("^dUpdated:\n^2%16s   ^9\"^o%s^9\"\n", c->name, c->ourValue);
			found = true;
			break;
		}				
		c = c->next;
	}	

	if (!found && _stricmp(cvarName, "del")){
		Syscall.UI_Print("^dAdded new entry:\n^2%16s   ^9\"^o%s^9\"\n", cvarName, val);
		Tools.AddUserCvar(cvarName, val);
	}

	Tools.SaveUserCvars();

	free(val);
	free(cvarName);
}

void xcmd_setEtproGuid()
{
	if (Syscall.UI_Argc() == 1)
	{
		if (!eth32.settings.etproGuid || strlen(eth32.settings.etproGuid) < 2)
			Syscall.UI_Print("^dEtpro Guid: ^2original");
		else
			Syscall.UI_Print("^dEtpro Guid: ^7%s", eth32.settings.etproGuid);
		return;
	}

	strncpy_s(eth32.settings.etproGuid, sizeof(eth32.settings.etproGuid), Syscall.UI_Argv(1), _TRUNCATE);
	Syscall.UI_Print("^dNew etpro guid: ^7%s\n^dSetting won't take effect until next map (or do /reconnect)!", eth32.settings.etproGuid);
}

void xmcd_ircMsg()
{
	Gui.SetChatTarget(4); // irc msg type == 4
}

void xcmd_replyMsg()
{
	if (!eth32.cg.replyPlayer[0]) {
		Gui.CenterPrint("^1Error: ^7No target for reply!", 1);
		return;
	}

	Gui.ReplyMessage();
}

void xcmd_AimKeyDown()
{
	Aimbot.SetAimkeyStatus(true);
}

void xcmd_AimKeyUp()
{
	Aimbot.SetAimkeyStatus(false);
}

/*
void setEtproOS()
{
	if (Syscall.UI_Argc() == 1){
		Syscall.UI_Print("^2%s", eth32.settings.etproOs ? "win" : "linux");
		return;
	}

	char *cmd = _strdup(Syscall.UI_Argv(1));

	if (!strcmp(cmd, "win"))
		eth32.settings.etproOs = true;
	else
		eth32.settings.etproOs = false;

	Syscall.UI_Print("^2new etpro OS set to %s", eth32.settings.etproOs ? "win" : "linux");
	free(cmd);
}*/

CXCommands::CXCommands(void)
{
	cmdsRegistered = false;
}

void CXCommands::RegisterCommands()
{
	if (cmdsRegistered || !orig_Cmd_AddCommand)
		return;

	orig_Cmd_AddCommand("eth_vec", &loadCustomHitbox);
	orig_Cmd_AddCommand("eth_home", &goHome);
	orig_Cmd_AddCommand("eth_musicspam", &xcmd_MusicSpam);
	orig_Cmd_AddCommand("eth_musicspamfmt", &xcmd_UpdateMusicSpamFmt);
	orig_Cmd_AddCommand("eth_hackspam", &xcmd_HackSpam);
	orig_Cmd_AddCommand("eth_namesteal", &xcmd_Namesteal);
	orig_Cmd_AddCommand("eth_killspamfmt", &xcmd_KillSpamFmt);
	orig_Cmd_AddCommand("eth_crashme", &xcmd_Crash);
	orig_Cmd_AddCommand("eth_cvar", &xcmd_CvarForce);
	orig_Cmd_AddCommand("eth_etproguid", &xcmd_setEtproGuid);
	orig_Cmd_AddCommand("eth_ircmsg", &xmcd_ircMsg);
	orig_Cmd_AddCommand("eth_tracker", &xcmd_Tracker);
	orig_Cmd_AddCommand("eth_set", &xcmd_Settings);
	orig_Cmd_AddCommand("eth_toggle", &xcmd_Toggle);
	orig_Cmd_AddCommand("eth_reply", &xcmd_replyMsg);

	orig_Cmd_AddCommand("+aim", &xcmd_AimKeyDown);
	orig_Cmd_AddCommand("-aim", &xcmd_AimKeyUp);
//	orig_Cmd_AddCommand("eth_spoof", &xcmd_spoofIp);
	//orig_Cmd_AddCommand("eth_etproos", &setEtproOS);

	cmdsRegistered = true;
}
