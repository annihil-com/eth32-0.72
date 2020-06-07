// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#include "eth32.h"
#include "cvars.h"
#include "settings.h"

#include <Psapi.h>

extern void __cdecl InvalidParameterHandler(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, uintptr_t pReserved);
extern LONG __stdcall Eth32ExceptionFilter( EXCEPTION_POINTERS* pep );


CTools Tools;

__forceinline void decode(void *in, void *_out, int c)
{
	unsigned int t,r,i;
	int key;
	memcpy(&key, dsKey+37, 4);

	for (i=0; i<c; i+=4){
		memcpy(&t, (void *)((DWORD)in+i), 4);
		_asm {
			mov eax, t			
			xor eax, key
			mov r, eax
		}
		memcpy((void *)((DWORD)_out+i), &r, 4);
	}	
}

__forceinline void encode(void *in, void *_out, int c)
{
	unsigned int t,r,i;
	int key;
	memcpy(&key, dsKey+37, 4);

	for (i=0; i<c; i+=4){
		memcpy(&t, (void *)((DWORD)in+i), 4);
		_asm {
			mov eax, t
			xor eax, key
			mov r, eax
		}
		memcpy((void *)((DWORD)_out+i), &r, 4);
	}	
}

void CTools::Init(HINSTANCE hInst)
{
	_invalid_parameter_handler oldHandler, newHandler;
	newHandler = InvalidParameterHandler;
	oldHandler = _set_invalid_parameter_handler(newHandler);
	_CrtSetReportMode(_CRT_ASSERT, 0);
	nThreads = NULL;
	sThreads = 0;
	memset(&eth32, 0, sizeof(eth32_t));

	eth32.hInst = hInst;

	int len = GetModuleFileNameA(hInst, eth32.path, sizeof(eth32.path));
	eth32.eth32CRC = Tools.crc32FromFile(eth32.path);
	if (len)
	{
		TRASH11
		TRASH12
		while (len && eth32.path[len] != '\\') {
			TRASH45
			len--;
			TRASH13
			TRASH28
		}
		if (len) {
			eth32.path[len + 1] = '\0';
		}
	}

	this->RegisterThread(GetCurrentThreadId(), "Init");
	/* install global exception handler for all threads
	   CRT tries to overwrites this, so we must prevent them 
	   
	   NOTE: make *sure* that whatever happens, we restore the patch once eth32
	   finishes execution, otherwise, no program can install an 
	   except handler until reboot (including later instances of eth32 itself) */
#ifdef NDEBUG
	SetUnhandledExceptionFilter(Eth32ExceptionFilter);
	bool retval = this->PatchMicrosoftCRTandDOTNETInstallGlobalExceptionFilterOverwritingRoutinesSoTheyDontWorkAgainUntilWeTellThemTo(true);
//#ifdef #ETH32_DEBUG
//	if (retval)
//		Debug.Log("Successfully installed exception handler");
//#endif
#endif

	Tools.LoadUserCvars();
	TRASH9
	TRASH39
	TRASH49
	eth32.authed = Portal.Auth();
	TRASH41
#ifdef ETH32_DEBUG
	Debug.Log("Auth status: %s", eth32.authed ? "Information validated" : "Failed");
#endif

	if (!loadOffsets()){
	#ifdef ETH32_DEBUG
		Debug.Log("%s", errorMessage);
	#endif
		return;
	} else {
	#ifdef ETH32_DEBUG
		Debug.Log("Offsets successfully loaded from ini file");
	#endif
	}

	// this must be done before et.exe code is run
	// Tools.ProtectPK3();

	TRASH11
	if (*(int *) ((DWORD) &eth32.hInst + 4)) {
		TRASH36
		Hook.hookSystem(true);
		Portal.Init();
		TRASH37
	}
	TRASH48
}

BIG_BAD_BOOL CTools::PatchMicrosoftCRTandDOTNETInstallGlobalExceptionFilterOverwritingRoutinesSoTheyDontWorkAgainUntilWeTellThemTo(BIG_BAD_BOOL pszftztsftBigImpressiveBoolPtrArg_DoWeWantToEnbalePatch)
{
	static BYTE Original[5] = {0};
	const BYTE Patch[5] = {0x33,0xC0,0xC2,0x04,0x00};		// xor eax,eax  ;  ret  ;
	DWORD oldProtect;

	HMODULE hLib = GetModuleHandle("kernel32.dll");
	if (hLib == NULL){
#ifdef ETH32_DEBUG
		Debug.Log("Warning: could not install exception handler, NULL module");
#endif
		return false;
	}

	BYTE* patchTarget = (BYTE*)GetProcAddress(hLib, "SetUnhandledExceptionFilter");
	if (patchTarget == 0 || IsBadReadPtr(patchTarget, sizeof(Original))){
#ifdef ETH32_DEBUG
		Debug.Log("Warning: could not install exception handler, bad ptr %p", patchTarget);
#endif
		return false;
	}
	
	// patch it up
	if (pszftztsftBigImpressiveBoolPtrArg_DoWeWantToEnbalePatch){
		memcpy(Original, patchTarget, sizeof(Original));
		if (!VirtualProtect(patchTarget, sizeof(Patch), PAGE_EXECUTE_READWRITE, &oldProtect)){
#ifdef ETH32_DEBUG
			Debug.Log("Warning: could not install exception handler, could not change page access");
#endif
			return false;
		}
		memcpy(patchTarget, Patch, sizeof(Patch));

		DWORD t;
		if (!VirtualProtect(patchTarget, sizeof(Patch), oldProtect, &t)){
#ifdef ETH32_DEBUG
			Debug.Log("Warning: could not install exception handler, could not change page access");
#endif
			return false;
		}
	} else {
		if (!VirtualProtect(patchTarget, sizeof(Patch), PAGE_EXECUTE_READWRITE, &oldProtect)){
#ifdef ETH32_DEBUG
			Debug.Log("Warning: could not install exception handler, could not change page access");
#endif
			return false;
		}
		memcpy(patchTarget, Original, sizeof(Original));

		DWORD t;
		if (!VirtualProtect(patchTarget, sizeof(Patch), oldProtect, &t)){
#ifdef ETH32_DEBUG
			Debug.Log("Warning: could not install exception handler, could not change page access");
#endif
			return false;
		}
	}

	return true;
}

void CTools::Shutdown()
{
	TRASH45
	TRASH47
	Irc.Kill();
	Hook.hookGame(false);
	Hook.hookSystem(false);

	if (Tools.userCvarList)
		free(Tools.userCvarList);

	Irc.Kill();

	TRASH49
	// Added this because CG_Shutdown not called if exiting right out of ET
	SaveSettings();

	WSACleanup();
	TRASH30

#ifdef NDEBUG
	bool retval = this->PatchMicrosoftCRTandDOTNETInstallGlobalExceptionFilterOverwritingRoutinesSoTheyDontWorkAgainUntilWeTellThemTo(false);
	#ifdef ETH32_DEBUG
		if (retval)
			Debug.Log("Deregistered exception handler, un-patched system patcher");
	#endif
#endif

#ifdef ETH32_DEBUG
	Debug.Log("Unloading eth32, auf wiedersehen");
#endif
	//DeleteCriticalSection(&csdebug);
}

DWORD CTools::crc32FromFile(const char *filename)
{
	FILE *file;

	if (!filename)
		return 0;
	
	DWORD crc32;
	int err;

	if (fopen_s(&file, filename, "rb") != NO_ERROR)
		return 0;

	err = Crc32_ComputeFile(file, &crc32);

	fclose(file);

	if (err == -1)
		return 0;

	return crc32;
}

unsigned int CTools::ClientNumForAddr(DWORD dwCentAddr)
{
	// need to be sure centBase & centSize is defined for the mod
	// don't want to add a lot of checking here because it will probably be used quite a lot
	// TODO: maybe save &cg_entities[0] in cgame_t to avoid unneeded calcs
	return (dwCentAddr - eth32.cgMod->centBase - (DWORD)eth32.cg.module) / eth32.cgMod->centSize;
}

int CTools::ScanCrosshairEntity(void)
{
	vec3_t angle, end;
	trace_t		trace;
 
	VectorCopy( eth32.cg.refdef->vieworg, angle);
	VectorMA( angle, 8192, eth32.cg.refdef->viewaxis[0], end );
 
	orig_CG_Trace( &trace, angle, NULL, NULL, end, eth32.cg.clientNum, CONTENTS_BODY );
 
	if(trace.entityNum < MAX_CLIENTS && eth32.cg.players[trace.entityNum].currentState->eType == ET_PLAYER)
	{
		return trace.entityNum;
	}

	return -1;
}

pack_t *CTools::getPack(char *pk3filename)
{
	searchpath_t *browse = *(searchpath_t **)eth32.et->fs_searchpaths;
	for (; browse; browse = browse->next)
		if (browse->pack && strstr(browse->pack->pakFilename, pk3filename))
			return browse->pack;
	return NULL;
}

void CTools::CharToVec3(const char *txt, vec3_t v)
{
	sscanf_s(txt, "%f %f %f", &v[0], &v[1], &v[2]);
}

void CTools::CharToVec4(const char *txt, vec4_t v)
{
	sscanf_s(txt, "%f %f %f %f", &v[0], &v[1], &v[2], &v[3]);
}

void CTools::DataAsHex(char *out, void *in, size_t size)
{
#define ENOUGH 1024
	unsigned char *buf = (unsigned char *)in;
	char tmp[32];
	int i = 0;
	out[0] = '\0';
	for (; i<size; i++){
		sprintf_s(tmp, 32, "%02X ", (DWORD) buf[i]);
		strcat_s(out, ENOUGH, tmp);
	}
}

void CTools::SaveSettings(void)
{
	if (!eth32.settings.loaded)
		return;

	char settingsFile[MAX_PATH];
	sprintf_s(settingsFile, MAX_PATH, "%s\\%s", eth32.path, ETH32_SETTINGS);

	WritePrivateProfileString("Main Settings", "killspamfmt", this->killspamFormatStr, settingsFile);

	Settings.Save(settingsFile);
	Hook.SaveWeapons(settingsFile);
	Irc.SaveSettings(settingsFile);
	Portal.SaveSettings(settingsFile);
	Winamp.SaveSettings(settingsFile);
	Gui.SaveSettings(settingsFile);
}

void CTools::LoadSettings(bool loadGui)
{
	char settingsFile[MAX_PATH];
	sprintf_s(settingsFile, MAX_PATH, "%s\\%s", eth32.path, ETH32_SETTINGS);

	char value[1024];

	GetPrivateProfileString("Main Settings", "killspamfmt", "^7[n] ^9has discovered ^dETH32 ^9...", value, sizeof(value), settingsFile);
	strcpy_s(this->killspamFormatStr, sizeof(this->killspamFormatStr), value);

	Settings.Load(settingsFile);	
	Hook.LoadWeapons(settingsFile);
	Portal.LoadSettings(settingsFile);
	Winamp.LoadSettings(settingsFile);

	if (loadGui)
		Gui.LoadSettings(settingsFile);

	eth32.settings.loaded = true;
}

void CTools::getOffsetFromINI( char *key, DWORD *offset )
{
	char value[64];
	int i;
	GetPrivateProfileString(section, key, "0", value, 64, offsetFile);

	for( i=0; i<(int)strlen(value); i++ )
		value[i] = tolower(value[i]);
	
	sscanf_s( value, "0x%x", offset );
}

void CTools::getOffsetFromINI( char *key, int *offset )
{
	char value[64];
	int i;
	GetPrivateProfileString(section, key, "0", value, 64, offsetFile);

	for( i=0; i<(int)strlen(value); i++ )
		value[i] = tolower(value[i]);
	
	sscanf_s( value, "%i", offset );
}

bool CTools::loadOffsets()
{
	char allSections[1024];
	int	 cScOff = 0;
	int  numCG = 0;

	cgMod_t	*newCG;

	sprintf_s(offsetFile, MAX_PATH, "%s\\%s", eth32.path, ETH32_OFFSETS);

	GetPrivateProfileStringA(NULL,NULL,NULL,allSections, 1024, offsetFile);

	if (!allSections[0]){
		sprintf_s( errorMessage, 1024, "%s not found, aborting", offsetFile );
		return false;
	}

	while( allSections[cScOff]!='\0' ){
		strncpy_s(section,allSections+cScOff,sizeof(section));
	#ifdef ETH32_DEBUG
		Debug.Log("offsets.ini: found section %s", section);
	#endif
		if (section[0] == 'C' && section[1] == 'G'){
			//handle CG offsets 
			newCG = new cgMod_t;
			sscanf_s( section+3, "0x%X", &newCG->crc32 );
			GetPrivateProfileStringA(section, "modname", "unknown", newCG->name, 64, offsetFile); 
			GetPrivateProfileStringA(section, "modversion", "unknown", newCG->version, 64, offsetFile); 
			getOffsetFromINI( "modtype", &newCG->type );
			getOffsetFromINI( "refdef", &newCG->refdef );
			getOffsetFromINI( "refdefViewAngles", &newCG->refdefViewAngles  );
			getOffsetFromINI( "pmext", &newCG->pmext   );
			getOffsetFromINI( "commands", &newCG->commands );
			getOffsetFromINI( "numCommands", &newCG->numCommands  );
			getOffsetFromINI( "predictedPS", &newCG->predictedPS );
			getOffsetFromINI( "centityBase", &newCG->centBase  );
			getOffsetFromINI( "centitySize", &newCG->centSize  );
			getOffsetFromINI( "centCurrentState", &newCG->centCurrentStateOffset  );
			getOffsetFromINI( "centLerpOrigin", &newCG->centLerpOrigin );
			getOffsetFromINI( "clientInfoBase", &newCG->ciBase  );
			getOffsetFromINI( "clientInfoSize", &newCG->ciSize   );
			getOffsetFromINI( "cliInfoValid", &newCG->ciInfoValidOffset  );
			getOffsetFromINI( "cliName", &newCG->ciNameOffset  );
			getOffsetFromINI( "cliTeam", &newCG->ciTeamOffset  );
			getOffsetFromINI( "cliHealth", &newCG->ciHealthOffset  );
			getOffsetFromINI( "cliClass", &newCG->ciClassOffset  );
			getOffsetFromINI( "CG_Player", &newCG->CG_Player  );
			getOffsetFromINI( "CG_AddPlayerWeapon", &newCG->CG_AddPlayerWeapon  );
			getOffsetFromINI( "CG_PositionRotatedEntityOnTag", &newCG->CG_PositionRotatedEntityOnTag );
			getOffsetFromINI( "CG_SetLerpFrameAnimationRate", &newCG->CG_SetLerpFrameAnimationRate  );
			getOffsetFromINI( "CG_FinishWeaponChange", &newCG->CG_FinishWeaponChange  );
			getOffsetFromINI( "CG_PlayVoiceChat", &newCG->CG_PlayVoiceChat  );
			getOffsetFromINI( "CG_EntityEvent", &newCG->CG_EntityEvent  );
			getOffsetFromINI( "CG_Trace", &newCG->CG_Trace  );
			getOffsetFromINI( "CG_DamageFeedback", &newCG->CG_DamageFeedback  );
			getOffsetFromINI( "CG_WeaponFireRecoil", &newCG->CG_WeaponFireRecoil  );
			getOffsetFromINI( "CG_Respawn", &newCG->CG_Respawn  );
			getOffsetFromINI( "CG_CalculateReinfTime", &newCG->CG_CalculateReinfTime );
			getOffsetFromINI( "CG_AllocLocalEntity", &newCG->CG_AllocLocalEntity );
			getOffsetFromINI( "CG_AddPMItem", &newCG->CG_AddPMItem  );
			getOffsetFromINI( "CG_AddPMItem2", &newCG->CG_AddPMItem2  );
			getOffsetFromINI( "CG_DrawPlayerStatusHead", &newCG->CG_DrawPlayerStatusHead  );
			getOffsetFromINI( "CG_DrawPlayerStats", &newCG->CG_DrawPlayerStats  );
			getOffsetFromINI( "CG_DrawPlayerStatus", &newCG->CG_DrawPlayerStatus  );
			getOffsetFromINI( "CG_CenterPrint",	&newCG->CG_CenterPrint  );
			getOffsetFromINI( "CG_PriorityCenterPrint", &newCG->CG_PriorityCenterPrint  );
			getOffsetFromINI( "CG_ChargeTimesChanged", &newCG->CG_ChargeTimesChanged  );
			getOffsetFromINI( "BG_FindClipForWeapon", &newCG->BG_FindClipForWeapon  );
			getOffsetFromINI( "BG_FindAmmoForWeapon", &newCG->BG_FindAmmoForWeapon  );
			getOffsetFromINI( "CG_CalcViewValues", &newCG->CG_CalcViewValues );

			memcpy( (void *) &Hook.cglist[numCG], newCG, sizeof(cgMod_t) );	
			numCG++;
			
			if (numCG > (MAX_CG_MODS-1) ){
				delete newCG;
				sprintf_s( errorMessage, 1024, "too many CG versions defined" );
				return false;
			}

			delete newCG;
		}

		cScOff += strlen(section)+1;
	}

	return true;
}

bool CTools::SkipCgameCvar(vmCvar_t *cvar, const char *cvarName)
{
	int numCvars = sizeof(cvarList) / sizeof(cvarInfo_t);

	for (int i=0 ; i<numCvars ; i++) {
		if (!_strcmpi(cvarName, "cg_teamchatheight"))
			eth32.cg.cg_teamchatheight = cvar;

		if (!_strcmpi(cvarName, cvarList[i].name)) {
			strcpy_s(cvar->string, 256, cvarList[i].ourValue);
			cvar->integer = atoi(cvar->string);
			cvar->value = atof(cvar->string);
			return true;	// tell syscall to not register this
		}
	}
	return false;	// tell syscall to register this cvar as normal
}



void CTools::LoadUserCvars() 
{
	char path[MAX_STRING_CHARS];
	char cvar[MAX_STRING_CHARS];
	char cvarval[MAX_STRING_CHARS];
	char val[MAX_STRING_CHARS];
	FILE *cvarFile;
	char line[MAX_STRING_CHARS];
	sprintf_s(path, sizeof(path), "%s\\cvarforce.txt", eth32.path);
	userCvarList = NULL;

	if (fopen_s(&cvarFile, path, "r"))
		return;

	int j;
	bool start = false;
	while (fgets(line, MAX_STRING_CHARS-1, cvarFile))
	{
		char *end = strchr(line, ' ');
		if (!end || strlen(line)<4)
			continue;

		strncpy_s(cvar, sizeof(cvar), line, end-line);
		strncpy_s(val, sizeof(val), end+1, _TRUNCATE);

		// strip leading spaces and the quotes around the value
		j = 0; start = false;
		for(int i=0; i<MAX_STRING_CHARS;i++){
			if (!start && val[i]==' ')
				continue;
			if (!start && val[i]=='\"'){
				start = true;
				continue;
			}
			if ( (start && val[i]=='\"' && (val[i+1]=='\0' || val[i+1]==0xd || val[i+1]==0xa)) || val[i]=='\0' || val[i+1]==0xd || val[i+1]==0xa){
				cvarval[j++] = '\0';
				break;
			}
			cvarval[j++] = val[i];
		}

		AddUserCvar(cvar, cvarval);
	}

	fclose(cvarFile);
}

void CTools::AddUserCvar(char *cvarName, char *cvarValue)
{
	int cnt = 0;
	cvarInfo_t *c = userCvarList;
	while (c) {
		cnt++;
		c = c->next;
	}

	DWORD oldBase = (DWORD)userCvarList;
	userCvarList = (cvarInfo_t *)realloc(userCvarList, (cnt+1)*sizeof(cvarInfo_t));
	userCvarList[cnt].next = NULL;
	// realloc can also relocate the memory block, so we need to adjust the ptrs accordingly
	// (it does not fragment)
	c = userCvarList;
	while (c) {
		if (c->next)
			c->next = (cvarInfo_t *)((DWORD)c->next+(DWORD)userCvarList-oldBase);
		c = c->next;
	}	

	strncpy_s(userCvarList[cnt].name,MAX_STRING_CHARS, cvarName, MAX_STRING_CHARS);
	strncpy_s(userCvarList[cnt].ourValue,MAX_STRING_CHARS, cvarValue, MAX_STRING_CHARS);
	if (cnt>0)
		userCvarList[cnt-1].next = &userCvarList[cnt];	
	userCvarList[cnt].isSet = false;
}

void CTools::SaveUserCvars()
{
	char path[MAX_STRING_CHARS];
	FILE *cvarFile;
	sprintf_s(path, sizeof(path), "%s\\cvarforce.txt", eth32.path);	

	if (fopen_s(&cvarFile, path, "w"))
		return;

	cvarInfo_t *c = Tools.userCvarList;
	while (c) {
		fprintf_s(cvarFile, "%s \"%s\"\n", c->name, c->ourValue);
		c = c->next;
	}
	fclose(cvarFile);
}

void CTools::SetUserCvars() 
{
	if (!userCvarList)
		return;
	
	cvarInfo_t *c = userCvarList;
	while (c){
		Syscall.Cvar_Set(c->name, c->ourValue);
		c = c->next;
	}
}

char *CTools::CleanString(char *string)
{
	char*	d;
	char*	s;
	int		c;

	s = string;
	d = string;

	// get rid of leading whitespace
	while (*s == ' ')
		s++;

	while ((c = *s) != 0 ) {
		if ( Q_IsColorString( s ) ) {
			s++;
		}		
		else if ( c >= 0x20 && c <= 0x7E ) {
			*d++ = c;
		}
		s++;
	}
	*d = '\0';

	return string;
}

void CTools::UrlString(char *string, int size)
{
	if (!string || !*string || size < 2)
		return;

	int cnt = 0;
	char *s = string;
	char *d = new char[size];

	while (*s && cnt < size-1)
	{
		if (*s == ' ') {
			if (cnt > size-4)
				break;
			strcpy_s(&d[cnt], 4, "%20");
			cnt += 3;
		}
		else {
			d[cnt] = *s;
			cnt++;
		}
		s++;
	}
	d[cnt] = 0;
	strcpy_s(string, size, d);
	delete [] d;
}

#define IS_URL_SPACE(p)		( p && *(p) == '%' && *((p)+1) == '2' && *((p)+2) == '0' )
void CTools::RemoveUrlSpaces(char *string)
{
	char*	d;
	char*	s;
	int		c;

	s = string;
	d = string;
	while ((c = *s) != 0 ) {
		if ( IS_URL_SPACE( s ) ) {
			s += 2;
			*d++ = ' ';
		}		
		else {
			*d++ = c;
		}
		s++;
	}
	*d = '\0';
}

char *CTools::GetEtVersionString(int version)
{
	if (version == 0)
		return "2.55";
	else if (version == 1)
		return "2.56";
	else if (version == 2)
		return "2.60";
	else if (version == 3)
		return "2.60b";
	else 
		return "unknown";
}

char *CTools::GetModVersionString(int version)
{
	if (version == 0)
		return "etmain";
	else if (version == 1)
		return "etpub";
	else if (version == 2)
		return "jaymod 1.x";
	else if (version == 3)
		return "jaymod 2.x";
	else if (version == 4)
		return "noquarter";
	else if (version == 5)
		return "etpro";
	else if (version == 6)
		return "tce";
	else
		return "unknown";
}

const char* CTools::GetRandomName(void)
{
	const char* names[MAX_CLIENTS];
	int cnt = 0;

	for (int i=0 ; i<MAX_CLIENTS ; i++) {
		if (IS_INFOVALID(i) && i != eth32.cg.clientNum) {
			if (!strcmp(eth32.cg.players[i].name, eth32.cg.players[eth32.cg.clientNum].name))
				continue;

			names[cnt] = eth32.cg.players[i].name;
			cnt++;
		}
	}

	if (!cnt)
		return NULL;

	srand(eth32.cg.time);
	return names[rand() % cnt];
}

void CTools::Namesteal(bool pbEnabled)
{
	const char *playerName = GetRandomName();
	char cmd[256];
	char msg[1024];
	
	if (!playerName || !*playerName) {
		Syscall.UI_Print("^1Error: ^ono valid players to namesteal from!\n");
		return;
	}
	
	if (!pbEnabled) {
		sprintf_s(cmd, sizeof(cmd), "name \"%s\"\n", playerName);
		Gui.BufferConsoleCommand(cmd);

		sprintf_s(msg, sizeof(msg), "^oName stolen^d: ^7%s", playerName);
		Gui.CenterPrint(msg, 1);
	}
	else {
		// TODO: add pb proof namesteal
	}
}

//[n] - colored target name
//[N] - clean target name
//[d] - target deaths by me
//[D] - total target deaths
//[K] - my total kills
//[s] - my current spree
//[S] - my longest spree
//[w] - my current weapon

void CTools::KillSpam(entityState_t *es)
{
	int target = es->otherEntityNum;

	if (!IS_INFOVALID(target))
		return;

	player_t *player = &eth32.cg.players[target];
	
	char *s = killspamFormatStr;

	if (!(*s)) {
		killspamStr[0] = '\0';
		return;
	}

	int i;

	strcpy_s(killspamStr, sizeof(killspamStr), "say \"");

	for( i=0; i<strlen(s); i++){
		if (s[i]=='[' && s[i+2] == ']'){
			switch (s[i+1]){
				case 'n':
					strcat_s(killspamStr, sizeof(killspamStr), player->name);
					break;
				case 'N':
					char cleanname[MAX_QPATH];
					strcpy_s(cleanname, MAX_QPATH, player->name);
					strcat_s(killspamStr, sizeof(killspamStr), CleanString(cleanname));
					break;
				case 'd':
					sprintf_s(killspamStr+strlen(killspamStr), sizeof(killspamStr)-strlen(killspamStr), "%i", player->stats.my_kills);
					break;
				case 'D':
					sprintf_s(killspamStr+strlen(killspamStr), sizeof(killspamStr)-strlen(killspamStr), "%i", player->stats.deaths);
					break;
				case 'K':
					sprintf_s(killspamStr+strlen(killspamStr), sizeof(killspamStr)-strlen(killspamStr), "%i", eth32.stats.kills);
					break;
				case 's':
					sprintf_s(killspamStr+strlen(killspamStr), sizeof(killspamStr)-strlen(killspamStr), "%i", eth32.stats.spree);
					break;
				case 'S':
					sprintf_s(killspamStr+strlen(killspamStr), sizeof(killspamStr)-strlen(killspamStr), "%i", eth32.stats.longestSpree);
					break;
				case 'w':
					if (eth32.cg.currentWeapon) 
						strcat_s(killspamStr, sizeof(killspamStr), eth32.cg.currentWeapon->name);
					break;
				default:
					i+=3;
					break;
			}
			i+=2;
			continue;
		}
		strncat_s(killspamStr, sizeof(killspamStr), s+i, 1);
	}

	strcat_s(killspamStr, sizeof(killspamStr), "\"\n");
	Syscall.CG_SendConsoleCommand(killspamStr);
}

void CTools::UpdateKillSpamFormat(char *newFmt)
{
	strncpy_s(this->killspamFormatStr, sizeof(this->killspamFormatStr), newFmt, _TRUNCATE);
}

void CTools::RegisterThread(DWORD tID, char *name)
{
	if (!this->nThreads)
		nThreads = (threadInfo_t *)malloc(sizeof(threadInfo_t));

	sThreads++;
	nThreads = (threadInfo_t *) realloc(nThreads,(sizeof(threadInfo_t)*sThreads));
	
	strncpy_s(nThreads[sThreads-1].name, 127, name, _TRUNCATE);
	nThreads[sThreads-1].tID = tID;

#ifdef ETH32_DEBUG
	Debug.Log("Register Thread 0x%X %s", tID, name);
#endif
}

void CTools::VectorAngles(const vec3_t forward, vec3_t angles)
{
    float tmp, yaw, pitch;

    if (forward[1] == 0 && forward[0] == 0) {
	yaw = 0;
	if (forward[2] > 0)
	    pitch = 90;
	else
	    pitch = 270;
    } else {
	yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
	if (yaw < 0)
	    yaw += 360;

	tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
	pitch = (atan2(forward[2], tmp) * 180 / M_PI);
	if (pitch < 0)
	    pitch += 360;
    }

    angles[0] = pitch;
    angles[1] = yaw;
    angles[2] = 0;
}

// case insensitive strstr
char *stristr (const char * str1, const char * str2)
{
        char *cp = (char *) str1;
        char *s1, *s2;

        if ( !*str2 )
            return((char *)str1);

        while (*cp)
        {
                s1 = cp;
                s2 = (char *) str2;
				
                while ( *s1 && *s2 && !(tolower(*s1)-tolower(*s2)) )
                        s1++, s2++;

                if (!*s2)
                        return(cp);

                cp++;
        }

        return NULL;
}