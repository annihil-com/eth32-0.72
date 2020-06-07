// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

typedef bool H_BOOL_A;
typedef H_BOOL_A A_BOOL_A;
typedef A_BOOL_A BOOLA_BOOLA;
typedef BOOLA_BOOLA BIG_BAD_BOOL;

#pragma once

char *stristr (const char * str1, const char * str2);

typedef struct threadInfo_st {
	char name[128];
	DWORD tID;
} threadInfo_t;

class CTools
{
public:
	void Init(HINSTANCE hInst);
	void Shutdown();
	DWORD crc32FromFile(const char *filename);
	unsigned int ClientNumForAddr(DWORD dwCentAddr);
	int ScanCrosshairEntity(void);	// Returns entNum if crosshair is over Ent, else returns -1 (even through walls)
	pack_t *getPack(char *pk3filename);

	// some utilities that may be used by Gui or Settings
	void CharToVec3(const char *txt, vec3_t v);
	void CharToVec4(const char *txt, vec4_t v);

	void SaveSettings(void);
	void LoadSettings(bool loadGui);
	void DataAsHex(char *out, void *in, size_t size);

	bool SkipCgameCvar(vmCvar_t *cvar, const char *cvarName);

	void SetUserCvars();
	void SaveUserCvars();
	void AddUserCvar(char *cvarName, char *cvarValue);
	void LoadUserCvars();

	// this seems out of place here
	void VectorAngles(const vec3_t forward, vec3_t angles);

	char *CleanString(char *string);
	void UrlString(char *string, int size);
	void RemoveUrlSpaces(char *string);

	const char *GetRandomName(void);
	void Namesteal(bool pbEnabled);

	char *GetEtVersionString(int version);
	char *GetModVersionString(int version);

	void KillSpam(entityState_t *es);
	void UpdateKillSpamFormat(char *newFmt);
	const char* GetKillSpamFormat(void) { return this->killspamFormatStr; }
	
	void RegisterThread(DWORD tID, char *name);
	threadInfo_t *nThreads;
	int sThreads;
	
	// damn... starting to feel like a real Microsoft Systems Programmer here ;)
	BIG_BAD_BOOL PatchMicrosoftCRTandDOTNETInstallGlobalExceptionFilterOverwritingRoutinesSoTheyDontWorkAgainUntilWeTellThemTo(BIG_BAD_BOOL pszftztsftBigImpressiveBoolPtrArg_DoWeWantToEnbalePatch);

	cvarInfo_t *userCvarList;

private:
	void getOffsetFromINI( char *key, int *offset );
	void getOffsetFromINI( char *key, DWORD *offset );
	bool loadOffsets();
	char offsetFile[MAX_PATH];
	char section[64];

	char killspamFormatStr[512];
	char killspamStr[1024];

	char errorMessage[1024];
};

extern CTools Tools;

