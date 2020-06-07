// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#pragma once

#include <vector>

#define MAX_FRAME_TIMES	11

#define SCREEN_HEIGHT_CENTER 480 / 2	// center screen used for possition
#define SCREEN_WIDTH_CENTER 640 / 2		// center screen used for possition
#define BASE_WIDTH 500					// size width of drawn circle display
#define BASE_HEIGHT 500					// size height of drawn circle display

class CEngine
{
public:
	CEngine(void);

	void RegisterMedia();
	void SetConsoleShader();
	void MenuCloseActions();
	void SetWeaponSpread();
	void CG_Init_Pre(int serverMessageNum, int serverCommandSequence, int clientNum);
	void CG_Init_Post(int serverMessageNum, int serverCommandSequence, int clientNum);
	int CG_Shutdown(void);
	int CG_DrawActiveFrame(int serverTime, int stereoView, int demoPlayback);

	int CalcViewValues(void);
	void CorrectServerAngles();

	void PreFrame(int serverTime);
	void PostFrame(void);
	
	void StartPlayerCapture(centity_t *cent);
	void EndPlayerCapture(void);
	void CapturePlayerHead(refEntity_t *refent);

	void SetDrawWeapon(bool state) { drawWeapon = state; }

	void CmdActivate();
	void RenderWeapon(int x, int y, int size, int weapon = 0);

	void AddRefEntity(refEntity_t *re);
	void AddPolyToScene(qhandle_t shader, polyVert_t *polyVert);

	void PlayerRadarIcon(player_t *player);
	void PlayerEsp(player_t *player);
	void GentityESP(void);
	void GentityESPTCE(void);
	void GetTag(int clientNum, char *tagName, orientation_t *or);

	void MakeRailTrail( vec3_t start, vec3_t end, bool box, vec3_t color, int lifeTime );
	void MakeRailTrail( vec3_t start, vec3_t end, bool box, float r, float g, float b, int lifeTime );
	void SetRailTrailTime( cvar_t *cv );

	// Event Handlers for CG_EntityEvent
	void EV_Obituary(entityState_t *es);
	void EV_Bullet(entityState_t *es, bool hitFlesh);

	void GetSpawntimes();
	float FrameCorrectionTime(int n);

	const char *ConfigString(int index);

	void setMuzzle(void);
	int findSatchel(void);

	bool IsVisible(vec3_t start, vec3_t pt, int skipEntity); // needed one outside of aim class

	// public members
	float *forward;		// moved here so we calculate this only *one* time per frame
	lerpFrame_t *slerp;






	// remove me!!!
	int sfxplay;
	bool played;




private:
	player_t	*pDrawPlayer;			// used to see if refent being added is part of a player
	bool		drawWeapon;

	int frameTimes[MAX_FRAME_TIMES-1];
	int frameN;

	int			framePlayers[MAX_CLIENTS];	// indices into our main players array, added through cg_player
	int			numFramePlayers;			// count of actual drawn/valid players this frame
	bool		playerDrawn;				// safety check, used to verify player was actually added to scene by cg_player
	cvar_t *	railtrailTime;				// cvar time for quick reference for our own railtrail
	
	void _CG_RailTrail( vec3_t start, vec3_t end, vec3_t color, int lifeTime );

	int aReinfOffset[TEAM_NUM_TEAMS];
};

extern CEngine Engine;

extern const char *chamsText[];
extern const char *autopushText[];
extern const char *playerEspText[];
extern const char *gentityEspText[];
extern const char *gentityEspTextTCE[];
