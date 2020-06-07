// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#include "eth32.h"

CEngine Engine;

weapdef_t nullWeapon = { "Unknown", {0,0,0}, {0,0,0}, 0, WT_NONE, WA_NONE, 0, 0, 0, 0, false, false };

// There !!!HAS TO BE!!! a corresponding string for each cham type (except MAX_CHAMS)
const char *chamsText[MAX_CHAMS] =
{
	"Off",
	"Crystal",
	"Wave/Glow",
	"Solid",
	"Quad",
};

const char *autopushText[MAX_AUTOPUSH] =
{
	"Off",
	"Enemy",
	"Team",
	"Both",
};

CEngine::CEngine(void)
{
	forward = NULL;
	railtrailTime = NULL;
	pDrawPlayer = NULL;
	drawWeapon = false;
}

// actions to take when menu is closed
void CEngine::MenuCloseActions()
{
	SetConsoleShader();
}

void CEngine::SetConsoleShader()
{
	eth32.game.console[0] = eth32.game.console[0] ? eth32.game.console[0] : *(DWORD*)(eth32.et->consoleshader);
	eth32.game.console[1] = eth32.game.console[1] ? eth32.game.console[1] : *(DWORD*)(eth32.et->consoleshader+4);
	eth32.game.console[2] = eth32.game.console[2] ? eth32.game.console[2] : *(DWORD*)(eth32.et->consoleshader+8);

	if (eth32.settings.transparantConsole && eth32.cg.media.nullShader){ 
		*(DWORD*)(eth32.et->consoleshader) = eth32.cg.media.nullShader;
		*(DWORD*)(eth32.et->consoleshader+4) = eth32.cg.media.nullShader;
		*(DWORD*)(eth32.et->consoleshader+8) = eth32.cg.media.etlogoShader;
	} else {
		*(DWORD*)(eth32.et->consoleshader) = eth32.game.console[0];
		*(DWORD*)(eth32.et->consoleshader+4) = eth32.game.console[1];
		*(DWORD*)(eth32.et->consoleshader+8) = eth32.game.console[2];
	}
}

void CEngine::RegisterMedia()
{
	// font(s)
	Syscall.R_RegisterFont("ariblk", 27, &eth32.cg.media.fontArial);

	eth32.cg.media.etlogoShader = Syscall.R_RegisterShader("eth32logo");
	eth32.cg.media.nullShader = Syscall.R_RegisterShader("nullshader");
	eth32.cg.media.white = Syscall.R_RegisterShader("white");
	eth32.cg.media.cursor = Syscall.R_RegisterShader("ui/assets/3_cursor3");
	eth32.cg.media.binoc = Syscall.R_RegisterShader("gfx/misc/binocsimple");
	eth32.cg.media.smokePuff = Syscall.R_RegisterShader("smokePuff");
	eth32.cg.media.reticleSimple = Syscall.R_RegisterShader("gfx/misc/reticlesimple");
	eth32.cg.media.colorTable = Syscall.R_RegisterShader("gui/common/colortable");
	eth32.cg.media.statbar = Syscall.R_RegisterShader("statbar");
	eth32.cg.media.statbar2 = Syscall.R_RegisterShader("statbar2");
	eth32.cg.media.eth32Icon = Syscall.R_RegisterShaderNoMip("gfx/hud/ic_stamina");

	// chams
	eth32.cg.media.chams[CHAMS_CRYSTAL] = Syscall.R_RegisterShader("crystalChams");
	eth32.cg.media.chams[CHAMS_WAVE] = Syscall.R_RegisterShader("waveChams");
	eth32.cg.media.chams[CHAMS_SOLID] = Syscall.R_RegisterShader("solidChams");
	eth32.cg.media.chams[CHAMS_QUAD] = Syscall.R_RegisterShader("quadChams");
	
	// kobject: someone check this is also valid for TCE
	eth32.cg.media.railCore = Syscall.R_RegisterShader("railCore");

	eth32.cg.media.grenTarget = Syscall.R_RegisterShaderNoMip("target_arrow");

	// common GUI stuff
	eth32.cg.media.portalMarker = Syscall.R_RegisterShader("portal_marker");
	eth32.cg.media.combtnLeft = Syscall.R_RegisterShader("combtn_left");
	eth32.cg.media.combtnRight = Syscall.R_RegisterShader("combtn_right");
	eth32.cg.media.combtnCenter = Syscall.R_RegisterShader("combtn_center");
	eth32.cg.media.comselLeft = Syscall.R_RegisterShader("comsel_left");
	eth32.cg.media.comselRight = Syscall.R_RegisterShader("comsel_right");
	eth32.cg.media.comselCenter = Syscall.R_RegisterShader("comsel_center");

	// winamp
	eth32.cg.media.waPlay = Syscall.R_RegisterShader("wa_play");
	eth32.cg.media.waStop = Syscall.R_RegisterShader("wa_stop");
	eth32.cg.media.waPause = Syscall.R_RegisterShader("wa_pause");
	eth32.cg.media.waNext = Syscall.R_RegisterShader("wa_next");
	eth32.cg.media.waPrev = Syscall.R_RegisterShader("wa_prev");
	
	if(eth32.cgMod->type == MOD_TCE) 
	{
		//models
		eth32.cg.media.medpackTCE = Syscall.RegisterModel("models/multiplayer/medpack/medpack_pickup.md3");
		eth32.cg.media.flashGrenadeTCE = Syscall.RegisterModel("models/weapons2/grenade/m84.md3");
		eth32.cg.media.grenadeTCE = Syscall.RegisterModel("models/weapons2/mk32a/mk32a.md3");
		eth32.cg.media.dynamiteTCE = Syscall.RegisterModel("models/multiplayer/dynamite/dynamite.md3");
		eth32.cg.media.smokeGrenadeTCE = Syscall.RegisterModel("models/weapons2/grenade/m83.md3");

		//icons
		eth32.cg.media.flashGrenadeTCEIcon = Syscall.R_RegisterShaderNoMip("gfx/icons/iconw_pineapple_1_select.tga");
		eth32.cg.media.grenadeTCEIcon = Syscall.R_RegisterShaderNoMip("gfx/icons/iconw_pineapple_1_select.tga");
		eth32.cg.media.dynamiteTCEIcon = Syscall.R_RegisterShaderNoMip("gfx/icons/iconw_dynamite_1_select.tga");
		eth32.cg.media.smokeGrenadeTCEIcon = Syscall.R_RegisterShaderNoMip("gfx/icons/iconw_pineapple_1_select.tga");
	}
	else 
	{
		// world
		eth32.cg.media.medPack = Syscall.RegisterModel("models/multiplayer/medpack/medpack_pickup.md3");
		eth32.cg.media.ammoPack = Syscall.RegisterModel("models/multiplayer/ammopack/ammopack_pickup.md3");
		eth32.cg.media.supplyRackAmmo = Syscall.RegisterModel("models/mapobjects/supplystands/stand_ammo.md3");
		eth32.cg.media.supplyRackHealth = Syscall.RegisterModel("models/mapobjects/supplystands/stand_health.md3");
		eth32.cg.media.rackHealth = Syscall.RegisterModel("models/multiplayer/supplies/healthbox_wm.md3");
		eth32.cg.media.rackAmmo = Syscall.RegisterModel("models/multiplayer/supplies/ammobox_wm.md3");
		// projectiles/explosives
		eth32.cg.media.mortarModel = Syscall.RegisterModel("models/multiplayer/mortar/mortar_shell.md3");
		eth32.cg.media.grenadeAxisModel = Syscall.RegisterModel("models/ammo/grenade1.md3");
		eth32.cg.media.grenadeAlliedModel = Syscall.RegisterModel("models/weapons2/grenade/pineapple.md3");
		eth32.cg.media.satchelModel = Syscall.RegisterModel("models/multiplayer/satchel/satchel_world.md3");
		eth32.cg.media.dynamiteModel = Syscall.RegisterModel("models/multiplayer/dynamite/dynamite.md3");
		eth32.cg.media.rocketModel = Syscall.RegisterModel("models/ammo/rocket/rocket.md3");
		eth32.cg.media.rifleNadeAllied = Syscall.RegisterModel("models/multiplayer/m1_garand/m1_garand_prj.md3");
		eth32.cg.media.rifleNadeAxis = Syscall.RegisterModel("models/multiplayer/m1_garand/m1_garand_prj.md3"); /// haven't found it yet
		// weapon models 
		eth32.cg.media.mp40Model = Syscall.RegisterModel("models/weapons2/mp40/mp40.md3");
		eth32.cg.media.thompsonModel = Syscall.RegisterModel("models/weapons2/thompson/thompson.md3");
		eth32.cg.media.stenModel = Syscall.RegisterModel("models/weapons2/sten/sten.md3");
		//icons
		eth32.cg.media.grenadeAlliedIcon = Syscall.R_RegisterShaderNoMip("gfx/icons/iconw_pineapple_1_select.tga");
		eth32.cg.media.grenadeAxisIcon = Syscall.R_RegisterShaderNoMip("gfx/icons/iconw_grenade_1_select.tga");
		eth32.cg.media.landmineIcon = Syscall.R_RegisterShaderNoMip("gfx/icons/iconw_landmine_1_select.tga");
		eth32.cg.media.dynamiteIcon = Syscall.R_RegisterShaderNoMip("gfx/icons/iconw_dynamite_1_select.tga");
		eth32.cg.media.satchelIcon = Syscall.R_RegisterShaderNoMip("gfx/icons/iconw_satchel_1_select.tga");
	}
}

void CEngine::CG_Init_Pre(int serverMessageNum, int serverCommandSequence, int clientNum)
{
	Tools.RegisterThread(GetCurrentThreadId(), "Main");
	Tools.SetUserCvars();

	#ifdef ETH32_DEBUG
		Debug.Log("CG_Init() Thread ID %x [ ClientNum: %i ]", GetCurrentThreadId(), clientNum);
	#endif

	Syscall.CG_SendConsoleCommand("unbind f12\n");
	Syscall.CG_SendConsoleCommand("unbind f11\n");
	Syscall.CG_SendConsoleCommand("unbind f10\n");
	Syscall.CG_SendConsoleCommand("unbind f9\n");
	Syscall.CG_SendConsoleCommand("unbind f8\n");
	Syscall.CG_SendConsoleCommand("unbind f7\n");
	Syscall.CG_SendConsoleCommand("unbind f6\n");

	// register all the medias we use
	RegisterMedia();

	// register all sounds
	sounds.registerSounds();

	eth32.cg.clientNum = clientNum;

	// reset snap to avoid trying to use an old one from prior map
	eth32.cg.snap = NULL;

	Engine.frameN = 0;

	// set rendering/drawing info
	eth32.cg.screenXScale = eth32.game.glconfig->vidWidth / 640.0f;
	eth32.cg.screenYScale = eth32.game.glconfig->vidHeight / 480.0f;

	// center of screen
	eth32.cg.centerX = (eth32.game.glconfig->vidWidth/2) / eth32.cg.screenXScale;
	eth32.cg.centerY = (eth32.game.glconfig->vidHeight/2) / eth32.cg.screenYScale;

	// must be called after glconfig, screenXScale, and screenYScale are valid
	Gui.Init();
	Tools.LoadSettings(true);

	// must be after GUI creates windows, so we can eventually set windows props
//	Tools.LoadSettings(true);

#ifdef _NDEBUG
	Punkbuster.RandomizeDummyFunc();
	DummyFunc();
#endif
}

void CEngine::CG_Init_Post(int serverMessageNum, int serverCommandSequence, int clientNum)
{
	// place some critical checks here, and abort if they fail 
	// so we dont have to continually check integrity elsewhere
	if (!eth32.et->inButtons)
		ExitProcess(0);
	if (!eth32.game.cg_fov)
		ExitProcess(0);
	if (!eth32.game.sensitivity)
		ExitProcess(0);

	//kobj: etpro workaround
	if (eth32.cgMod->type == MOD_ETPRO && eth32.cgMod->crc32 == 0xC54721AF){
		int i;
		for (i=0; i<MAX_CLIENTS; i++)
			eth32.cg.players[i].headModel = *(qhandle_t *)((DWORD)eth32.cg.module + 0x61E92F8);

#ifdef ETH32_DEBUG
		if (!eth32.cg.players[0].headModel)
			Debug.Log("Error: etpro head hModel not found!");
		else
			Debug.Log("Etpro head hModel is %i", eth32.cg.players[0].headModel);
#endif
	}

	Tools.SetUserCvars();

	// unreference our pk3 so we don't get kicked
	Hook.UnreferenceBadPk3();

	Aimbot.SetSelf(clientNum);
	Aimbot.SetUserCrouchStatus(false);

	eth32.cgameLoaded = true;

	sprintf_s(eth32.server, sizeof(eth32.server), "%i.%i.%i.%i:%i", ((et_netadr_t *)eth32.et->server_ip)->ip[0], ((et_netadr_t *)eth32.et->server_ip)->ip[1], ((et_netadr_t *)eth32.et->server_ip)->ip[2], ((et_netadr_t *)eth32.et->server_ip)->ip[3], htons(((et_netadr_t *)eth32.et->server_ip)->port)); 

	const char *info = ConfigString(CS_SERVERINFO);

	// retrieve server name
	strncpy_s( eth32.cg.serverName, 50, Tools.CleanString(Info_ValueForKey(info, "sv_hostname")), _TRUNCATE );
	// retrieve map name
	strncpy_s( eth32.cg.serverMapName, 25, Tools.CleanString(Info_ValueForKey(info, "mapname")), _TRUNCATE );

#ifdef ETH32_DEBUG
	Debug.Log("Server: %s IP: %s Map: %s", eth32.cg.serverName, eth32.server, eth32.cg.serverMapName);
#endif

	Portal.Init();

	eth32.cg.cgTime = 0;

	SetConsoleShader();
}

void CEngine::SetWeaponSpread()
{
	switch (eth32.cg.currentWeapon->type) {
		case WT_PISTOL:
			eth32.cg.spreadIndex = 600.0;
			break;
		case WT_SMG:
			if (eth32.cg.currentWeapon->attribs & WA_OVERHEAT)	// sten
				eth32.cg.spreadIndex = 200.0;
			else												// other smg
				eth32.cg.spreadIndex = 400.0;
			break;
		case WT_RIFLE:
			eth32.cg.spreadIndex = 250.0;
			break;
		case WT_SNIPER:
			eth32.cg.spreadIndex = 0.0;
			break;
		case WT_MG:
			eth32.cg.spreadIndex = 2500.0;
			break;
		case WT_SHOTGUN:
			eth32.cg.spreadIndex = 10000.0;		// guess
			break;
		default:
			eth32.cg.spreadIndex = 0.0;
	}

	// for simplicity's sake assume >lvl3 light weapons
	eth32.cg.spreadIndex *= 0.65;
}

int CEngine::CG_Shutdown(void)
{
	#ifdef ETH32_DEBUG
		Debug.Log("CG_Shutdown()");
	#endif

	Hook.hookCgame(false);
	eth32.cgameLoaded = false;

	Tools.SaveSettings();
	Gui.Shutdown();

	int r = orig_CG_vmMain(CG_SHUTDOWN);
	
	Hook.hookGame(true);

	eth32.cg.hasInit = false;

	forward = NULL;
	pDrawPlayer = NULL;
	drawWeapon = false;

	#ifdef ETH32_DEBUG
		Debug.Log("CG_Shutdown completed");
	#endif
	return r;
}

float CEngine::FrameCorrectionTime(int n)
{
	int frTime = 0;

	if (n > 0){
		for (int i=0; i<n; i++)
			frTime += frameTimes[(frameN-i) % MAX_FRAME_TIMES];	
		return 0.001f*(float)frTime;
	} else {
		float avg = 0;
		float std = 0;

		for(int i=0; i<MAX_FRAME_TIMES; i++)
			avg += frameTimes[i];
		avg = avg / (float)(MAX_FRAME_TIMES-1);
		
		for(int i=0; i<MAX_FRAME_TIMES; i++)
			std += pow(((float)frameTimes[i]-avg),2);
		std = sqrt(std/((float)(MAX_FRAME_TIMES-2)));

		return 0.002f*(avg+0.5*std);
	}	
}

void CEngine::PreFrame(int serverTime)
{
	// X    0 -> 180 -> -180 -> 0
	// Y	-90 -> 0 -> 90
	/*
	eth32.cg.ourViewAngleX = *eth32.game.viewAngleX + SHORT2ANGLE(eth32.cg.snap->ps.delta_angles[1]);
	eth32.cg.ourViewAngleY = *eth32.game.viewAngleY + SHORT2ANGLE(eth32.cg.snap->ps.delta_angles[0]);

	if (eth32.cg.ourViewAngleX > 180.0)
		eth32.cg.ourViewAngleX -= 360.0;
	if (eth32.cg.ourViewAngleX < -180.0)
		eth32.cg.ourViewAngleX += 360.0;
	*/

	eth32.cg.frametime = serverTime - eth32.cg.time;
	eth32.cg.time = serverTime;

	eth32.cg.xhairClientNum = Tools.ScanCrosshairEntity();

	// if spec, set our team to team allies for chams and other team dependent funcs
	eth32.cg.team = *eth32.cg.players[eth32.cg.clientNum].team;
	if (eth32.cg.team == TEAM_SPECTATOR)
		eth32.cg.team = TEAM_ALLIES;

	frameN++;
	frameTimes[frameN % MAX_FRAME_TIMES] = eth32.cg.frametime;

	numFramePlayers = 0;
	pDrawPlayer = NULL;
	playerDrawn = false;

#ifndef ETH32_PRELIM_OFFSETS // don't call if we are still offset searching
	this->GetSpawntimes();
#endif

	Gui.PreFrame();
	Aimbot.PreFrame();
	Engine.setMuzzle();
	Cam.resetInfo();

//	Draw.Text(5, 5, GUI_FONTSCALE, text, colorYellow, qtrue, qfalse, GUI_FONT, true);
}

void CEngine::PostFrame(void)
{
	int clientNum;
	player_t *player;

	/*if (Engine.sfxplay) {
		Syscall.S_StartLocalSound(sfxplay, CHAN_LOCAL_SOUND); 
		sfxplay = 0;
	}*/

	// autopush stuff
	if (eth32.settings.autoPush && Engine.forward){
		trace_t tr;
		vec3_t end;
		VectorMA(eth32.cg.muzzle, eth32.settings.autoPushDistance, Engine.forward, end);
		orig_CG_Trace(&tr, eth32.cg.muzzle, NULL, NULL, end, eth32.cg.clientNum, CONTENTS_BODY);
		if (tr.entityNum >= 0 && tr.entityNum < MAX_CLIENTS && !IS_DEAD(tr.entityNum) && IS_INFOVALID(tr.entityNum)) {
			if (eth32.settings.autoPush == AUTOPUSH_BOTH ||			
			   (eth32.settings.autoPush == AUTOPUSH_ENEMY && !IS_FRIENDLY(tr.entityNum)) ||
			   (eth32.settings.autoPush == AUTOPUSH_TEAM && IS_FRIENDLY(tr.entityNum))) {
				   this->CmdActivate();
			}
		}
	}

	// sol: setting currentWeapon based on snap info
	if (eth32.cg.weaponList && eth32.cg.snap && eth32.cg.snap->ps.weapon < eth32.cg.numWeapons)
		eth32.cg.currentWeapon = &eth32.cg.weaponList[eth32.cg.snap->ps.weapon];
	else
		eth32.cg.currentWeapon = &nullWeapon;
	
	// traverse captured players list
	for (int i=0 ; i<numFramePlayers ; i++) {
		clientNum = framePlayers[i];
		player = &eth32.cg.players[clientNum];

		if (eth32.settings.drawHeadHitbox)
			Aimbot.DrawHeadBox( player->clientNum, eth32.settings.drawHeadAxes );
		if (eth32.settings.drawBodyHitbox)
			Aimbot.DrawBodyBox( player->clientNum, false );

		if (eth32.settings.aimSort == SORT_TARGETING)
			player->targetingUs = Aimbot.TargetingUs( player->clientNum );

		PlayerEsp(player);

		// added distance here, it might be used for both radar and aimbot
		player->distance = VectorDistance(eth32.cg.refdef->vieworg, player->lerpOrigin);	

		// add this player to our radar
		Gui.AddRadarPlayer(player);

		//if (eth32.cg.currentWeapon->attribs & WA_USER_DEFINED)
		Aimbot.AddTarget(player);
	}

	if(eth32.cgMod->type != MOD_TCE)
		GentityESP();
	else
		GentityESPTCE();

	if (GetAsyncKeyState(VK_F6) & 1)
		Cam.dropCamSet(eth32.settings.dropCamCount); //temp
	if (GetAsyncKeyState(VK_F5) & 1)
		Cam.dropCamKill(eth32.settings.dropCamCount); //temp

	Cam.numFramePlayers = this->numFramePlayers;
	Cam.dropCamActive();

	Gui.PostFrame();
	
	Aimbot.DrawGrenadelauncherTrace();
	Aimbot.DrawGrenadeTrace();

#ifdef ETH32_DEBUG
	if (GetAsyncKeyState(VK_INSERT)) {
		char buf[256];
		refdef_t *rd = eth32.cg.refdef;
		sprintf_s(buf, 256, "eth32.cg.refdef: fwd = %.2f %2.f %.2f", rd->viewaxis[0][0], rd->viewaxis[0][1], rd->viewaxis[0][2]);
		Draw.Text(eth32.game.glconfig->vidWidth >> 1, 5, GUI_FONTSCALE, buf, colorWhite, qtrue, qfalse, GUI_FONT, true);
		sprintf_s(buf, 256, "rendering refdef: fwd = %.2f %2.f %.2f", eth32.cg.tempRenderVector[0], eth32.cg.tempRenderVector[1], eth32.cg.tempRenderVector[2]);
		Draw.Text(eth32.game.glconfig->vidWidth >> 1, 20, GUI_FONTSCALE, buf, colorWhite, qtrue, qfalse, GUI_FONT, true);
	}
#endif

	Aimbot.PostFrame();

	/*
	if (Engine.slerp){
	char buf[256];
	sprintf_s(buf,sizeof(buf), "%i %i %i %i %i", Engine.slerp->animation->duration, Engine.slerp->animation->numFrames, Engine.slerp->animation->firstFrame, Engine.slerp->animation->moveSpeed, Engine.slerp->animation->movetype);
	Draw.Text(5,5,GUI_FONTSCALE,buf,colorYellow, qtrue, qfalse, GUI_FONT, true);

	sprintf_s(buf,sizeof(buf),"%i %i %f %i %i %i",
			Engine.slerp->animationNumber, Engine.slerp->animationTime, Engine.slerp->backlerp,
	Engine.slerp->frame, Engine.slerp->frameTime, Engine.slerp->pitching);

	Draw.Text(5,15,GUI_FONTSCALE,buf,colorYellow, qtrue, qfalse, GUI_FONT, true);
	}
	//cent->currentState.legsAnim
/*
	char buf[256];
	sprintf_s(buf,256,"{%.1f %.1f %.1f} <-> {%.1f %.1f} | {%.1f %.1f %.1f}  => {%.2f, %.2f}", 
		eth32.cg.snap->ps.viewangles[0],
		eth32.cg.snap->ps.viewangles[1],
		eth32.cg.snap->ps.viewangles[2],

		*eth32.game.viewAngleX,
		*eth32.game.viewAngleY,

		SHORT2ANGLE(eth32.cg.snap->ps.delta_angles[0]),
		SHORT2ANGLE(eth32.cg.snap->ps.delta_angles[1]),
		SHORT2ANGLE(eth32.cg.snap->ps.delta_angles[2]),

		eth32.cg.ourViewAngleX,
		eth32.cg.ourViewAngleY		
		
		);
	Draw.Text(5,5,GUI_FONTSCALE,buf,colorYellow, qtrue, qfalse, GUI_FONT, true);
*/

	refEntities.clear( ); //end of frame time to clear vector
}

/*
 Handle server-generated garbage in the viewangles
 Every CG_DrawActiveFrame (or CL_Frame) the engine generates a usercmd
 and sends this to the server. Usercmd contains the ANGLE2SHORT of 
 cl.viewangles, or game.viewAngleX and game.viewAngleY.
 these values continously increase or decrease depending on mouse moventment
 (or other user input) only. 

 Server then calculates our viewangles. To handle the input correctly, 
 in case userinput != server viewangles, server also sends delta_angles
 which is the amount to be added to our viewangles to arrive at the server
 dictated ps.viewangles.

 mousemovent (viewAngleX,Y) -> ucmd -> server -> add delta_angles -> ps.viewangles -> send back -> refdef

*/

void CEngine::CorrectServerAngles()
{
	return;
	playerState_t *ps = &eth32.cg.snap->ps;
	
	// force roll to 0 to keep normal view orientation and send appropriate correction next frame :)
	// fixme: restore mouse roll input at appropriate time (i.e. respawn)
	if (ps->delta_angles[ROLL] != 0 && !(ps->eFlags & EF_MG42_ACTIVE || ps->eFlags & EF_AAGUN_ACTIVE || ps->eFlags & EF_MOUNTEDTANK) ){
		ps->delta_angles[ROLL] = 0;	
		// curiously, there is no way in Q3 to undo a roll with user input without a hax
		*(float *)(eth32.game.viewAngleX+4) = -ps->viewangles[ROLL];
	}
	
}

int CEngine::CG_DrawActiveFrame(int serverTime, int stereoView, int demoPlayback)
{
	// No need to mess with anything until game is ready (i.e. we have snapshot)
	if (!eth32.cg.snap)
		return orig_CG_vmMain(CG_DRAW_ACTIVE_FRAME, serverTime, stereoView, demoPlayback);

	eth32.cg.cgTime = serverTime;
	Engine.PreFrame(serverTime);
	int oldHealth = eth32.cg.snap->ps.stats[STAT_HEALTH];
	//if (eth32.settings.thirdPerson) eth32.cg.snap->ps.stats[STAT_HEALTH] = -1;
	int ret = orig_CG_vmMain(CG_DRAW_ACTIVE_FRAME, serverTime, stereoView, demoPlayback);
	//if (eth32.settings.thirdPerson) eth32.cg.snap->ps.stats[STAT_HEALTH] = oldHealth;
	Engine.PostFrame();

	return ret;
}

void CEngine::StartPlayerCapture(centity_t *cent)
{
	unsigned int clientNum = Tools.ClientNumForAddr((DWORD)cent);

	playerDrawn = false;
	pDrawPlayer = NULL;
	if (clientNum < 0 || clientNum >= MAX_CLIENTS || IS_DEAD(clientNum) || !IS_INFOVALID(clientNum))
		return;

	framePlayers[numFramePlayers] = clientNum;
	Cam.framePlayers[numFramePlayers] = clientNum;
	pDrawPlayer = &eth32.cg.players[clientNum];
	
	pDrawPlayer->friendly = IS_FRIENDLY(clientNum);
	pDrawPlayer->invulnerable = IS_INVULNERABLE(clientNum);

	// sol: added visibility check here so that we can use it for chams
	pDrawPlayer->visible = IsVisible(eth32.cg.refdef->vieworg, pDrawPlayer->lerpOrigin, pDrawPlayer->clientNum);
}

void CEngine::EndPlayerCapture(void)
{
	// will use next index if player was actually drawn... 
	// this was a fix for TCE, though if the nixCoders visibility solution works I will remove the drawn check
	if (playerDrawn)
		numFramePlayers++;

#ifdef ETH32_DEBUG
	if (pDrawPlayer && pDrawPlayer->name)
		if (VectorCompare(vec3_origin, pDrawPlayer->orHead.origin))
			Debug.Log("ERROR! ERROR! ERROR! %i %s did not receive a HEAD!", pDrawPlayer->clientNum, pDrawPlayer->name);
#endif

	pDrawPlayer = NULL;
}

void CEngine::CapturePlayerHead(refEntity_t *refent)
{
	// pDrawPlayer is only valid when we are dealing with a player that is not dead
	// kobj: etpro does not seem to set hModel for head here??? grab it directly from cgame instead!
	if (pDrawPlayer) {
		pDrawPlayer->headModel = refent->hModel;
	}
}

void CEngine::CmdActivate()
{
	kbutton_t *aBtn = (kbutton_t *) ((DWORD)eth32.et->inButtons + 6*sizeof(kbutton_t));
	aBtn->wasPressed = 1;
}

void CEngine::RenderWeapon(int x, int y, int size, int weapon)
{
	if (!orig_CG_AddPlayerWeapon || !eth32.cg.snap) {	// invalid func or snap
#ifdef ETH32_DEBUG
		Draw.Text(x, y, GUI_FONTSCALE, "Error", colorWhite, qtrue, qfalse, GUI_FONT, true);
#endif
		return;
	}
	
	static vec3_t viewangle = { 0, 0, 0 };
	viewangle[1] += 2.5;
	if (viewangle[1] > 360.f)
		viewangle[1] -= 360.f;

	refdef_t rd;
	refEntity_t re;

	memset(&rd, 0, sizeof(refdef_t));
	memset(&re, 0, sizeof(refEntity_t));

	entityState_t *es = eth32.cg.players[eth32.cg.snap->ps.clientNum].currentState;

	// modify for the weapon we want to render	
	int oldweap = es->weapon;
	if (weapon) {
		if (IS_WEAPATTRIB(weapon, WA_SCOPED)) // for scoped weapons, keeping normal weapon in otherAkimbo
			es->weapon = eth32.cg.weaponList[weapon].otherAkimbo; 
		else
			es->weapon = weapon;
	}
	else if (IS_WEAPATTRIB(oldweap, WA_SCOPED)) {
		es->weapon = eth32.cg.weaponList[oldweap].otherAkimbo;
	}

	// sol: Used for testing and setting weapon render parms
	//AnglesToAxis(eth32.settings.weapAngle, re.axis);
	//VectorCopy(eth32.settings.weapOrigin, re.origin);

	weapdef_t *weapdef = &eth32.cg.weaponList[es->weapon];

	if (weapdef->r_dist == 0.f) {
#ifdef ETH32_DEBUG
		Draw.Text(x, y, GUI_FONTSCALE, "No def", colorWhite, qtrue, qfalse, GUI_FONT, true);
#endif
		es->weapon = oldweap;
		return;
	}

	AnglesToAxis(weapdef->r_angle, re.axis);
	VectorCopy(weapdef->r_origin, re.origin);

	rd.x = x;
	rd.y = y;
	rd.width = rd.height = size;
	rd.fov_x = rd.fov_y = 30.f;
	rd.rdflags = RDF_NOWORLDMODEL;

	AnglesToAxis(viewangle, rd.viewaxis);
	
	// sol: Used for testing and setting weapon render parms
	//VectorScale(rd.viewaxis[0], -eth32.settings.weapDist, rd.vieworg); 

	VectorScale(rd.viewaxis[0], -weapdef->r_dist, rd.vieworg); 	

	// setup and render
	orig_syscall(CG_R_SAVEVIEWPARMS);
	orig_syscall(CG_R_CLEARSCENE);
	orig_CG_AddPlayerWeapon(&re, NULL, eth32.cg.players[eth32.cg.snap->ps.clientNum].cent);
	orig_syscall(CG_R_RENDERSCENE, &rd);
	orig_syscall(CG_R_RESTOREVIEWPARMS);

	// restore proper weapon
	es->weapon = oldweap;
}

void CEngine::AddRefEntity(refEntity_t *re)
{
	if(eth32.cgMod->type == MOD_TCE 
		&& re->hModel == eth32.cg.media.flashGrenadeTCE && eth32.settings.gentInfo[ESP_GENT_TCE_FLASH].chams
		|| re->hModel == eth32.cg.media.smokeGrenadeTCE && eth32.settings.gentInfo[ESP_GENT_TCE_SMOKE].chams
		|| re->hModel == eth32.cg.media.grenadeTCE && eth32.settings.gentInfo[ESP_GENT_TCE_GRENADE].chams
		|| re->hModel == eth32.cg.media.dynamiteTCE && eth32.settings.gentInfo[ESP_GENT_TCE_DYNAMITE].chams
		|| re->hModel == eth32.cg.media.medpackTCE
		)
	{
		if(re->hModel == 0)
			return;
		if (eth32.settings.wallhack)
			re->renderfx |= RF_DEPTHHACK;
		if(re->hModel == eth32.cg.media.medpackTCE)
		{
			re->customShader = eth32.cg.media.chams[eth32.settings.chamsPlayer];
			memcpy(re->shaderRGBA, eth32.settings.colorHealth, sizeof(BYTE[3]));
		}
		else
		{
			re->customShader = eth32.cg.media.chams[eth32.settings.chamsPlayer];
			memcpy(re->shaderRGBA, eth32.settings.colorAmmo, sizeof(BYTE[3]));
		}

	}

	if(re->hModel == eth32.cg.media.grenadeAxisModel && eth32.settings.gentInfo[ESP_GENT_GRENADE].chams
		|| re->hModel == eth32.cg.media.grenadeAlliedModel && eth32.settings.gentInfo[ESP_GENT_GRENADEP].chams
		|| re->hModel == eth32.cg.media.dynamiteModel && eth32.settings.gentInfo[ESP_GENT_DYNAMITE].chams
		|| re->hModel == eth32.cg.media.mortarModel && eth32.settings.gentInfo[ESP_GENT_MORTAR].chams
		|| re->hModel == eth32.cg.media.satchelModel && eth32.settings.gentInfo[ESP_GENT_SATCHEL].chams
		|| re->hModel == eth32.cg.media.rifleNadeAllied && eth32.settings.gentInfo[ESP_GENT_RIFLENADE].chams
		|| re->hModel == eth32.cg.media.rifleNadeAxis && eth32.settings.gentInfo[ESP_GENT_RIFLENADE].chams)
	{
		if(re->hModel == 0)
			return;
		if (eth32.settings.wallhack)
			re->renderfx |= RF_DEPTHHACK;
		
		re->customShader = eth32.cg.media.chams[eth32.settings.chamsPlayer];
		memcpy(re->shaderRGBA, eth32.settings.colorAmmo, sizeof(BYTE[3]));
	}

	if(re->hModel == eth32.cg.media.medPack || re->hModel == eth32.cg.media.ammoPack) // maybe move?
	{
		if(re->hModel == 0)
			return;

		if (eth32.settings.wallhack)
			re->renderfx |= RF_DEPTHHACK;

		if (eth32.settings.chamsPlayer)
		{
			// sol: todo, add chams type for items
			re->customShader = eth32.cg.media.chams[eth32.settings.chamsPlayer];
			// set to health or ammo color respectively
			memcpy(re->shaderRGBA, (re->hModel == eth32.cg.media.medPack)?eth32.settings.colorHealth:eth32.settings.colorAmmo, sizeof(BYTE[3]));
		}
	}

	if(re->hModel == eth32.cg.media.mortarModel)
	{
		vec3_t axis, view;
		AxisToAngles(re->axis, axis);
		AxisToAngles(eth32.cg.refdef->viewaxis, view);

		if((int)view[1] == (int)axis[1])
		{
			VectorCopy( re->origin, Cam.gentityOrigin );
			memcpy( Cam.gentityAxis, re->axis , 3*sizeof(vec3_t) );
		}
		//Debug.Log("viewaxis {%f,%f,%f} re axis{%f,%f,%f}", view[0], view[1], view[2], axis[0], axis[1], axis[2]);
	}

	if (pDrawPlayer && (re->reType != RT_SPRITE)) { // this is part of a player and isn't an icon above it's head
		if (re->hModel == pDrawPlayer->headModel) {			
			VectorCopy( re->origin, pDrawPlayer->orHead.origin );
			memcpy( pDrawPlayer->orHead.axis,re->axis , 3*sizeof(vec3_t) );
		}

		playerDrawn = true;
		
		if (eth32.settings.wallhack)
			re->renderfx |= RF_DEPTHHACK;

		if (drawWeapon) {
			if (eth32.settings.chamsWeapon) {
				re->customShader = eth32.cg.media.chams[eth32.settings.chamsWeapon];
				memcpy(re->shaderRGBA, (pDrawPlayer->friendly)?eth32.settings.colorTeamWeapon:eth32.settings.colorEnemyWeapon, sizeof(BYTE[3]));
				re->shaderRGBA[3] = 0xff;
			}
		}
		else if (eth32.settings.chamsPlayer) {
			re->customShader = eth32.cg.media.chams[eth32.settings.chamsPlayer];
			// invulnerable
			if (pDrawPlayer->invulnerable) {
				memcpy(re->shaderRGBA, eth32.settings.colorInvulnerable, sizeof(BYTE[3]));
			}
			// teammate
			else if (pDrawPlayer->friendly) {
				memcpy(re->shaderRGBA, (pDrawPlayer->visible)?eth32.settings.colorTeam:eth32.settings.colorTeamHidden, sizeof(BYTE[3]));
			}
			// enemy
			else {
				memcpy(re->shaderRGBA, (pDrawPlayer->visible)?eth32.settings.colorEnemy:eth32.settings.colorEnemyHidden, sizeof(BYTE[3]));
			}
			re->shaderRGBA[3] = 0xff;
		}
	}
	else if (drawWeapon && eth32.settings.chamsOurWeapon && eth32.settings.chamsWeapon) {
		re->customShader = eth32.cg.media.chams[eth32.settings.chamsWeapon];
		memcpy(re->shaderRGBA, eth32.settings.colorTeamWeapon, sizeof(BYTE[3]));
		re->shaderRGBA[3] = 0xff;
	}		

	if (drawingCam)
		refEntities.push_back(*re);
}

void CEngine::AddPolyToScene(qhandle_t shader, polyVert_t *polyVert)
{
	// check is already done in syscall, but checking again in case we use this func for something else
	if (shader == eth32.cg.media.smokePuff) {
		// apply some transparency to the smoke
		int c = 0;
		for (; c < 4; c++)
			polyVert[c].modulate[3] = eth32.settings.smoketrnsp;
	}
}

void CEngine::PlayerRadarIcon(player_t *player)
{

	vec4_t colorTeam, colorEnemy;
	if(!IsVisible(eth32.cg.refdef->vieworg, player->lerpOrigin, 0))
	{
		Vector4Set(colorTeam, eth32.settings.colorTeam[0]/255.f, eth32.settings.colorTeam[1]/255.f, eth32.settings.colorTeam[2]/255.f, 1);
		Vector4Set(colorEnemy, eth32.settings.colorEnemy[0]/255.f, eth32.settings.colorEnemy[1]/255.f, eth32.settings.colorEnemy[2]/255.f, 1);

		if(player->eth32user)
		{
			Syscall.R_SetColor(colorGreen);
			Draw.DrawCompassIcon(SCREEN_WIDTH_CENTER - (BASE_WIDTH / 2), SCREEN_HEIGHT_CENTER - (BASE_HEIGHT / 2), BASE_WIDTH, BASE_HEIGHT, eth32.cg.refdef->vieworg, player->lerpOrigin, NULL, eth32.cg.media.eth32Icon);
			Syscall.R_SetColor(NULL);
		}
		else
		{
			Syscall.R_SetColor((player->friendly) ? colorTeam : colorEnemy);
			Draw.DrawCompassIcon(SCREEN_WIDTH_CENTER - (BASE_WIDTH / 2), SCREEN_HEIGHT_CENTER - (BASE_HEIGHT / 2), BASE_WIDTH, BASE_HEIGHT, eth32.cg.refdef->vieworg, player->lerpOrigin, NULL, eth32.cg.media.eth32Icon);
			Syscall.R_SetColor(NULL);
		}
	}
}

const char *playerEspText[ESP_PLAYER_MAX] =
{
	"Middle",
	"Side",
	"Stagered",
};

void CEngine::PlayerEsp(player_t *player)
{
	static vec4_t colorHidden = { 0.5, 0.5, 0.5, 0.5 };
	vec3_t espOrigin;

	VectorCopy(player->orHead.origin, espOrigin);
	espOrigin[2] += 15.f;	// move to above head in world

	if (Draw.worldToScreen(espOrigin, &player->screenX, &player->screenY)) {
		float *textColor = (player->visible) ? colorWhite : colorHidden;
		player->screenY -= 13;

		if (player->eth32user) {
			float *trackerColor;
				
			if (eth32.settings.portalCoopMode == COOP_ALL || (eth32.settings.portalCoopMode == COOP_FRIENDS && player->eth32friend))
				trackerColor = colorGreen;
			else
				trackerColor = colorRed;

			if (eth32.settings.iconTracker) {
				Syscall.R_SetColor(trackerColor);
				Draw.Pic(player->screenX - 5, player->screenY, 10, 10, eth32.cg.media.eth32Icon);
				Syscall.R_SetColor(NULL);
				player->screenY -= 11;
			}

			if (eth32.settings.espTracker) {
				Draw.TextCenter(player->screenX, player->screenY, 0.12, player->portalName, trackerColor, qtrue, qfalse, GUI_FONT);
				player->screenY -= 11;
			}
		}

		if (eth32.settings.espName) {
			Draw.TextCenter(player->screenX, player->screenY, 0.15, player->name, textColor, (player->visible) ? qfalse : qtrue, qtrue, GUI_FONT);
		}

		if (Aimbot.grenadeTarget == player && 
			((eth32.settings.grenadeBot && eth32.cg.currentWeapon->attribs & WA_GRENADE)
			|| (eth32.settings.rifleBot && eth32.cg.currentWeapon->attribs & WA_RIFLE_GRENADE))) {
			player->screenY -= 21;
			
			if (Aimbot.GrenadeFireOK)
				Syscall.R_SetColor(colorGreen);
			else
				Syscall.R_SetColor(colorRed);

			Draw.Pic(player->screenX-12, player->screenY, 24, 24, eth32.cg.media.grenTarget);
			Syscall.R_SetColor(NULL);
		}
	}
}

#define	ICONS_SIZE 14					// for scaling	
#define D_ICON_SIZE 24					// used without scaling

const char *gentityEspText[ESP_GENT_MAX] =
{
	"Axis Grenade",
	"Allied Grenade",
	"Dynamite",
	"Landmine",
	"Mortar",
	"Panzer",
	"Rifle Grenade",
	"Satchel",
	"Smoke",
};

const char *gentityEspTextTCE[ESP_GENT_TCE_MAX] =
{
	"Grenade",
	"Flash",
	"Smoke",
	"Dynamite",
};

void CEngine::GentityESPTCE(void)
{
	int			distance, d;
	float		iconScale;
	vec4_t		colorSet;
	gentity_t	*gentity;
	byte		color[4];
	
	for( int i =0 ; i < MAX_GENTITIES ; i++)
	{
		gentity = &eth32.cg.gentity[i];
		
		if(gentity->cent->currentValid && gentity->currentState->eType == ET_MISSILE) 
		{
			gentity->lerpOrigin[ROLL] += 0.5f; // buffalo: move height just a tad so gentity doesn't show as non visible when on the ground

			iconScale	=	d    =	VectorDistance(eth32.cg.refdef->vieworg, gentity->lerpOrigin);
			distance	=	(int) (iconScale / (64.0f / 2.0f)); //meters
			float fadeScale   =  1.0f;

			//char buf[16];
			fadeScale = 400.0f / iconScale;
			iconScale	=	1.0 - min( 1.f, iconScale / 2000.f );
			fadeScale = max(fadeScale, 0.2f);	// Max fade
			fadeScale = min(fadeScale, 1.0f);	// Min fade

			memcpy(color, (IsVisible(eth32.cg.refdef->vieworg, gentity->lerpOrigin, 0))?eth32.settings.colorHealth:eth32.settings.colorAmmo, sizeof(BYTE[3]));
			VectorCopy(color, colorSet);

			colorSet[3] = fadeScale;
			Syscall.R_SetColor(colorSet);
			
		switch( gentity->currentState->weapon )
		{
			case WP_GRENADE_PINEAPPLE:	
			{	
				gentityInfo_t *gent = &eth32.settings.gentInfo[ESP_GENT_TCE_GRENADE];

				if(Draw.worldToScreen(gentity->lerpOrigin, &gentity->screenX, &gentity->screenY))
				{
					if(gent->icon)
						Draw.Pic(gentity->screenX - (D_ICON_SIZE - 4) / 2, gentity->screenY - (D_ICON_SIZE - 4) / 2, D_ICON_SIZE - 4, D_ICON_SIZE - 4, eth32.cg.media.grenadeTCEIcon);
				
					if(gent->text)
						Draw.TextCenter(gentity->screenX, gentity->screenY + 2, 0.15, "[G]", colorSet, qfalse, qtrue, &eth32.cg.media.fontArial);
				}
					
				if(gent->radarIcon && d < gent->distance)
					Draw.DrawCompassIcon(SCREEN_WIDTH_CENTER - (BASE_WIDTH / 2), SCREEN_HEIGHT_CENTER - (BASE_HEIGHT / 2), BASE_WIDTH, BASE_HEIGHT, eth32.cg.refdef->vieworg, gentity->lerpOrigin, NULL, eth32.cg.media.grenadeTCEIcon);
				
				break;
			}

			case WP_GRENADE_LAUNCHER://TCE M84 - Flash grenade
			{
				gentityInfo_t *gent = &eth32.settings.gentInfo[ESP_GENT_TCE_FLASH];

				if(Draw.worldToScreen(gentity->lerpOrigin, &gentity->screenX, &gentity->screenY) && d < gent->distance)
				{
					if(gent->icon)
						Draw.Pic(gentity->screenX - (D_ICON_SIZE - 4) / 2, gentity->screenY - (D_ICON_SIZE - 4) / 2, D_ICON_SIZE - 4, D_ICON_SIZE - 4, eth32.cg.media.grenadeTCEIcon);
				
					if(gent->text)
						Draw.TextCenter(gentity->screenX, gentity->screenY + 2, 0.15, "[FG]", colorSet, qfalse, qtrue, &eth32.cg.media.fontArial);
								
				}
	
				if(gent->radarIcon && d < gent->distance)	
					Draw.DrawCompassIcon(SCREEN_WIDTH_CENTER - (BASE_WIDTH / 2), SCREEN_HEIGHT_CENTER - (BASE_HEIGHT / 2), BASE_WIDTH, BASE_HEIGHT, eth32.cg.refdef->vieworg, gentity->lerpOrigin, NULL, eth32.cg.media.grenadeTCEIcon);		
			
				break;
			}

			case WP_SMOKE_BOMB:	//TCE Same as ET
			{
				gentityInfo_t *gent = &eth32.settings.gentInfo[ESP_GENT_TCE_SMOKE];
				if(Draw.worldToScreen(gentity->lerpOrigin, &gentity->screenX, &gentity->screenY) && d < gent->distance)
				{
					if(gent->icon)
						Draw.Pic(gentity->screenX - (D_ICON_SIZE - 4) / 2, gentity->screenY - (D_ICON_SIZE - 4) / 2, D_ICON_SIZE - 4, D_ICON_SIZE - 4, eth32.cg.media.grenadeTCEIcon);
					
					if(gent->text)
						Draw.TextCenter(gentity->screenX, gentity->screenY + 2, 0.15, "[SG]", colorSet, qfalse, qtrue, &eth32.cg.media.fontArial);
					
				}
				if(gent->radarIcon && d < gent->distance)	
					Draw.DrawCompassIcon(SCREEN_WIDTH_CENTER - (BASE_WIDTH / 2), SCREEN_HEIGHT_CENTER - (BASE_HEIGHT / 2), BASE_WIDTH, BASE_HEIGHT, eth32.cg.refdef->vieworg, gentity->lerpOrigin, NULL, eth32.cg.media.grenadeTCEIcon);		
			
				break;
			}
			default:
				break;
		}
		Syscall.R_SetColor(NULL);	// clear color
		}
	}
}

void CEngine::GentityESP(void)
{
	int			distance, d;
	float		iconScale;
	vec4_t		colorSet;
	gentity_t	*gentity;
	byte		color[4];
	
	for( int i =0 ; i < MAX_GENTITIES ; i++)
	{
		gentity = &eth32.cg.gentity[i];
		
		if(gentity->cent->currentValid && gentity->currentState->eType == ET_MISSILE) 
		{
			gentity->lerpOrigin[ROLL] += 0.5f; // buffalo: move height just a tad so gentity doesn't show as non visible when on the ground

			iconScale	=	d    =	VectorDistance(eth32.cg.refdef->vieworg, gentity->lerpOrigin);
			distance	=	(int) (iconScale / (64.0f / 2.0f)); //meters
			float fadeScale   =  1.0f;

			char buf[16];
			fadeScale = 400.0f / iconScale;
			iconScale	=	1.0 - min( 1.f, iconScale / 2000.f );
			fadeScale = max(fadeScale, 0.2f);	// Max fade
			fadeScale = min(fadeScale, 1.0f);	// Min fade

			memcpy(color, (IsVisible(eth32.cg.refdef->vieworg, gentity->lerpOrigin, 0))?eth32.settings.colorHealth:eth32.settings.colorAmmo, sizeof(BYTE[3]));
			VectorCopy(color, colorSet);

			colorSet[3] = fadeScale;
			Syscall.R_SetColor(colorSet);
			
		switch( gentity->currentState->weapon )
		{
			
			case WP_M7:
			{
				gentityInfo_t *gent = &eth32.settings.gentInfo[ESP_GENT_RIFLENADE];

				if(Draw.worldToScreen(gentity->lerpOrigin, &gentity->screenX, &gentity->screenY) && d < gent->distance)
				{
					if(gent->icon)
						Draw.Pic(gentity->screenX - (ICONS_SIZE * iconScale + 4)/2, gentity->screenY - (ICONS_SIZE * iconScale + 4)/2, (ICONS_SIZE * iconScale + 8), (ICONS_SIZE * iconScale + 8), eth32.cg.media.grenadeAlliedIcon);
						
					if(gent->text)
							Draw.TextCenter(gentity->screenX, gentity->screenY + 2, 0.15, "[G]", colorSet, qfalse, qtrue, &eth32.cg.media.fontArial);
				}
				break;
			}
			case WP_GPG40:
			{
				gentityInfo_t *gent = &eth32.settings.gentInfo[ESP_GENT_RIFLENADE];

				if(Draw.worldToScreen(gentity->lerpOrigin, &gentity->screenX, &gentity->screenY) && d < gent->distance)
				{
					if(gent->icon)
						Draw.Pic(gentity->screenX - (ICONS_SIZE * iconScale + 4)/2, gentity->screenY - (ICONS_SIZE * iconScale + 4)/2, (ICONS_SIZE * iconScale + 8), (ICONS_SIZE * iconScale + 8), eth32.cg.media.grenadeAxisIcon);
						
					if(gent->text)
							Draw.TextCenter(gentity->screenX, gentity->screenY + 2, 0.15, "[G]", colorSet, qfalse, qtrue, &eth32.cg.media.fontArial);
				}
				break;
			}
			case WP_SATCHEL:
			{
				gentityInfo_t *gent = &eth32.settings.gentInfo[ESP_GENT_SATCHEL];

				if(Draw.worldToScreen(gentity->lerpOrigin, &gentity->screenX, &gentity->screenY) && d < gent->distance)
				{
					if(gent->icon)
						Draw.Pic(gentity->screenX - (ICONS_SIZE * iconScale + 4)/2, gentity->screenY - (ICONS_SIZE * iconScale + 4)/2, (ICONS_SIZE * iconScale + 8), (ICONS_SIZE * iconScale + 8), eth32.cg.media.satchelIcon);
						
					if(gent->text)
						Draw.TextCenter(gentity->screenX, gentity->screenY + 2, 0.15, "[S]", colorSet, qfalse, qtrue, &eth32.cg.media.fontArial);
				}
				if(gent->radarIcon && d < gent->distance)
					Draw.DrawCompassIcon(SCREEN_WIDTH_CENTER - (BASE_WIDTH / 2), SCREEN_HEIGHT_CENTER - (BASE_HEIGHT / 2), BASE_WIDTH, BASE_HEIGHT, eth32.cg.refdef->vieworg, gentity->lerpOrigin, NULL, eth32.cg.media.satchelIcon);

				break;
			}
			
			case WP_GRENADE_PINEAPPLE:	
			{							
				gentityInfo_t *gent = &eth32.settings.gentInfo[ESP_GENT_GRENADEP];

				if(Draw.worldToScreen(gentity->lerpOrigin, &gentity->screenX, &gentity->screenY) && d < gent->distance)
				{
					if(gent->icon)
						Draw.Pic(gentity->screenX - (D_ICON_SIZE - 4) / 2, gentity->screenY - (D_ICON_SIZE - 4) / 2, D_ICON_SIZE - 4, D_ICON_SIZE - 4, eth32.cg.media.grenadeAlliedIcon);
						
					if(gent->text)
						Draw.TextCenter(gentity->screenX, gentity->screenY + 2, 0.15, "[G]", colorSet, qfalse, qtrue, &eth32.cg.media.fontArial);
					}
				if(gent->radarIcon && d < gent->distance)
					Draw.DrawCompassIcon(SCREEN_WIDTH_CENTER - (BASE_WIDTH / 2), SCREEN_HEIGHT_CENTER - (BASE_HEIGHT / 2), BASE_WIDTH, BASE_HEIGHT, eth32.cg.refdef->vieworg, gentity->lerpOrigin, NULL, eth32.cg.media.grenadeAlliedIcon);
		
				break;
			}

			case WP_GRENADE_LAUNCHER://TCE M84 - Flash grenade
			{
				gentityInfo_t *gent = &eth32.settings.gentInfo[ESP_GENT_GRENADE];

				if(Draw.worldToScreen(gentity->lerpOrigin, &gentity->screenX, &gentity->screenY) && d < gent->distance)
				{
					gentityInfo_t *gent = &eth32.settings.gentInfo[ESP_GENT_GRENADE];

						if(gent->icon)
						Draw.Pic(gentity->screenX - (D_ICON_SIZE - 4) / 2, gentity->screenY - (D_ICON_SIZE - 4) / 2, D_ICON_SIZE - 4, D_ICON_SIZE - 4, eth32.cg.media.grenadeAxisIcon);
							
						if(gent->text)
							Draw.TextCenter(gentity->screenX, gentity->screenY + 2, 0.15, "[G]", colorSet, qfalse, qtrue, &eth32.cg.media.fontArial);
					
				}
	
				if(gent->radarIcon && d < gent->distance)	
					Draw.DrawCompassIcon(SCREEN_WIDTH_CENTER - (BASE_WIDTH / 2), SCREEN_HEIGHT_CENTER - (BASE_HEIGHT / 2), BASE_WIDTH, BASE_HEIGHT, eth32.cg.refdef->vieworg, gentity->lerpOrigin, NULL, eth32.cg.media.grenadeAxisIcon);		
			
				break;
			}
		
			case WP_LANDMINE:
			{
				gentityInfo_t *gent = &eth32.settings.gentInfo[ESP_GENT_LANDMINE];
				if(Draw.worldToScreen(gentity->lerpOrigin, &gentity->screenX, &gentity->screenY) && d < gent->distance)
				{
					if(gent->icon)
						Draw.Pic(gentity->screenX - (ICONS_SIZE * iconScale + 4)/2, gentity->screenY - (ICONS_SIZE * iconScale + 4)/2, (ICONS_SIZE * iconScale + 8), (ICONS_SIZE * iconScale + 8), eth32.cg.media.landmineIcon);
						
					if(gent->text)
						Draw.TextCenter(gentity->screenX, gentity->screenY + 2, 0.15, "[L]", colorSet, qfalse, qtrue, &eth32.cg.media.fontArial);
				}
				if(gent->radarIcon && d < gent->distance)
					Draw.DrawCompassIcon(SCREEN_WIDTH_CENTER - (BASE_WIDTH / 2), SCREEN_HEIGHT_CENTER - (BASE_HEIGHT / 2), BASE_WIDTH, BASE_HEIGHT, eth32.cg.refdef->vieworg, gentity->lerpOrigin, NULL, eth32.cg.media.landmineIcon);
		
				break;
			}
			
			case WP_SMOKE_BOMB:	//TCE Same as ET
			{
				gentityInfo_t *gent = &eth32.settings.gentInfo[ESP_GENT_SMOKE];
				if(Draw.worldToScreen(gentity->lerpOrigin, &gentity->screenX, &gentity->screenY) && d < gent->distance)
				{
					if(gent->icon)
						Draw.Pic(gentity->screenX - (ICONS_SIZE * iconScale + 4)/2, gentity->screenY - (ICONS_SIZE * iconScale + 4)/2, (ICONS_SIZE * iconScale + 8), (ICONS_SIZE * iconScale + 8), eth32.cg.media.grenadeAlliedIcon);
						
					if(gent->text)
						Draw.TextCenter(gentity->screenX, gentity->screenY + 2, 0.15, "[SG]", colorSet, qfalse, qtrue, &eth32.cg.media.fontArial);
				}
				if(gent->radarIcon && d < gent->distance)
					Draw.DrawCompassIcon(SCREEN_WIDTH_CENTER - (BASE_WIDTH / 2), SCREEN_HEIGHT_CENTER - (BASE_HEIGHT / 2), BASE_WIDTH, BASE_HEIGHT, eth32.cg.refdef->vieworg, gentity->lerpOrigin, NULL, eth32.cg.media.grenadeAlliedIcon);
		
				break;
			}
			case WP_MORTAR_SET:
			{
				gentityInfo_t *gent = &eth32.settings.gentInfo[ESP_GENT_MORTAR];
				if(Draw.worldToScreen(gentity->lerpOrigin, &gentity->screenX, &gentity->screenY) && d < gent->distance)
				{
					if(gent->text)
						Draw.TextCenter(gentity->screenX, gentity->screenY + 2, 0.15, "[M]", colorSet, qfalse, qtrue, &eth32.cg.media.fontArial);
				}
				break;
			}
			case WP_PANZERFAUST:
			{
				gentityInfo_t *gent = &eth32.settings.gentInfo[ESP_GENT_PANZER];
				if(Draw.worldToScreen(gentity->lerpOrigin, &gentity->screenX, &gentity->screenY) && d < gent->distance)
				{
					if(gent->text)
						Draw.TextCenter(gentity->screenX, gentity->screenY + 2, 0.15, "[P]", colorSet, qfalse, qtrue, &eth32.cg.media.fontArial);
				}
				break;
			}
			case WP_DYNAMITE: 
			{
				gentityInfo_t *gent = &eth32.settings.gentInfo[ESP_GENT_DYNAMITE];
				if(gentity->currentState->teamNum < 4 )
				{
					sprintf_s(buf,16,"%i",30-(eth32.cg.time - gentity->currentState->effect1Time)/1000);
					if(Draw.worldToScreen(gentity->lerpOrigin, &gentity->screenX, &gentity->screenY) && d < gent->distance)
					{
						if(gent->icon)
							Draw.Pic(gentity->screenX - (D_ICON_SIZE * iconScale + 4)/2, gentity->screenY - (D_ICON_SIZE * iconScale + 4)/2, (D_ICON_SIZE * iconScale + 8), (D_ICON_SIZE * iconScale + 8), eth32.cg.media.dynamiteIcon);
						if(gent->text)
							Draw.TextCenter(gentity->screenX, gentity->screenY + 2, 0.15, buf, colorSet, qfalse, qtrue, &eth32.cg.media.fontArial);
					}
					if(gent->radarIcon&& d < gent->distance)	
					Draw.DrawCompassIcon(SCREEN_WIDTH_CENTER - (BASE_WIDTH / 2), SCREEN_HEIGHT_CENTER - (BASE_HEIGHT / 2), BASE_WIDTH, BASE_HEIGHT, eth32.cg.refdef->vieworg, gentity->lerpOrigin, buf, eth32.cg.media.dynamiteIcon);
				}
				break;
			}
			default:
				break;
		}
		Syscall.R_SetColor(NULL);	// clear color
		}
	}
}

void CEngine::GetTag(int clientNum, char *tagName, orientation_t *or)
{
	orig_CG_vmMain(CG_GET_TAG, clientNum, tagName, or);
}

void CEngine::EV_Obituary(entityState_t *es)
{
	int target;
	int attacker;
	if (eth32.cgMod->type != MOD_ETPRO) {
		target = es->otherEntityNum;
		attacker = es->otherEntityNum2;
	}
	else {	// etpro's struct is modified. entitynums are in place of time and time2 members when using SDK struct
		target = es->time;
		attacker = es->time2;
	}

	// reset the aimbot when we die
	if (target == eth32.cg.clientNum) {
		Aimbot.Respawn();
		if (eth32.settings.autoTapout)
			Syscall.SendClientCommand("forcetapout");
	}

	if (target == attacker) {
		eth32.cg.players[target].stats.spree = 0;
		eth32.cg.players[target].stats.suicides++;
	}
	else {
		eth32.cg.players[target].stats.spree = 0;
		eth32.cg.players[target].stats.deaths++;
		eth32.cg.players[target].stats.kdRatio = (float)eth32.cg.players[target].stats.kills / eth32.cg.players[target].stats.deaths;

		if (attacker >= 0 && attacker < MAX_CLIENTS) {	// make sure attacker wasn't world or tank etc
			eth32.cg.players[attacker].stats.spree++;
			if (eth32.cg.players[attacker].stats.spree > eth32.cg.players[attacker].stats.longestSpree)
				eth32.cg.players[attacker].stats.longestSpree = eth32.cg.players[attacker].stats.spree;
			eth32.cg.players[attacker].stats.kills++;
			eth32.cg.players[attacker].stats.kdRatio = (float)eth32.cg.players[attacker].stats.kills / (eth32.cg.players[attacker].stats.deaths > 0 ? eth32.cg.players[attacker].stats.deaths : 1);

			if (attacker == eth32.cg.clientNum) {
				eth32.cg.players[target].stats.my_deaths++;		

				stats_t *targ = &eth32.cg.players[target].stats;
				targ->threat = targ->my_deaths == 0.f ? 999.0 : (float)targ->my_kills / ((float)targ->my_deaths);

				memcpy(&eth32.stats, &eth32.cg.players[eth32.cg.clientNum].stats, sizeof stats_t);	
				if (eth32.settings.killSpam)
					Tools.KillSpam(es);

				if(eth32.settings.killSounds)
					sounds.processSounds();
				
			}
			else if (target == eth32.cg.clientNum) {
				eth32.cg.players[attacker].stats.my_kills++;

				stats_t *targ = &eth32.cg.players[attacker].stats;
				if (targ->my_deaths == 0)
					targ->threat = 100.0; // 100 k/d is plenty high to start I think
				else 
					targ->threat = (float)targ->my_kills / ((float)targ->my_deaths);

				memcpy(&eth32.stats, &eth32.cg.players[eth32.cg.clientNum].stats, sizeof stats_t);
			}
		}	
	}
}

void CEngine::EV_Bullet(entityState_t *es, bool hitFlesh)
{
	int source = es->otherEntityNum;
	int fleshtarget = es->eventParm;

	if (source >= 0 && source < MAX_CLIENTS)
	{
		player_t *player = &eth32.cg.players[source];

		player->stats.bulletsFired++;
		if (hitFlesh && !IS_SAMETEAM(source,fleshtarget))  // don't want to log hits on teammates
			player->stats.bulletsHit++;

		player->stats.accuracy = ((float)player->stats.bulletsHit / player->stats.bulletsFired) * 100.f;

		/* sol: temp removed
		if (eth32.settings.hitsounds && hitFlesh && source == eth32.cg.snap->ps.clientNum) {
			sfxHandle_t sound = IS_SAMETEAM(source, fleshtarget) ? eth32.cg.media.sounds.hitsoundtm : eth32.cg.media.sounds.hitsound;
#ifdef ETH32_DEBUG
			Debug.Log("Sound: %i",  IS_SAMETEAM(source, fleshtarget));
#endif
			Syscall.S_StartLocalSound(sound, CHAN_LOCAL_SOUND);
		}*/
	}
}

void CEngine::GetSpawntimes()
{
	if (!eth32.cg.bools.spawntimerReady && (orig_CG_CalculateReinfTime != NULL))
		return;

	int temp = *eth32.cg.players[eth32.cg.snap->ps.clientNum].team;

	eth32.cg.spawnTimes[TEAM_FRIEND] = orig_CG_CalculateReinfTime(qfalse);
	*eth32.cg.players[eth32.cg.snap->ps.clientNum].team = (temp == TEAM_AXIS) ? TEAM_ALLIES : TEAM_AXIS;
	eth32.cg.spawnTimes[TEAM_ENEMY] = orig_CG_CalculateReinfTime(qfalse);
	*eth32.cg.players[eth32.cg.snap->ps.clientNum].team = temp;
}

const char *CEngine::ConfigString(int index) {
	if ( index < 0 || index >= MAX_CONFIGSTRINGS || !eth32.cg.gameState) 
		return NULL;

	return eth32.cg.gameState->stringData + eth32.cg.gameState->stringOffsets[index];
}

void CEngine::SetRailTrailTime( cvar_t *cv ){ railtrailTime = cv; }

void CEngine::_CG_RailTrail( vec3_t start, vec3_t end, vec3_t color, int lifeTime )
{
	localEntity_t	*le;
	refEntity_t		*re;

	le = orig_CG_AllocLocalEntity();
	re = &le->refEntity;

	le->leType = LE_FADE_RGB;
	le->startTime = eth32.cg.time;
	le->endTime = eth32.cg.time + lifeTime;
	le->lifeRate = 1.0 / ( le->endTime - le->startTime );

	re->shaderTime = eth32.cg.time / 1000.0f;
	re->reType = RT_RAIL_CORE;
	re->customShader = eth32.cg.media.railCore;

	VectorCopy( start, re->origin );
	VectorCopy( end, re->oldorigin );

	if (eth32.settings.railWallhack)
		re->renderfx |= RF_DEPTHHACK;

	le->color[0] = color[0]; //R
	le->color[1] = color[1]; //G
	le->color[2] = color[2]; //B
	le->color[3] = 1.0f;

	AxisClear( re->axis );
}

void CEngine::MakeRailTrail( vec3_t start, vec3_t end, bool box, float r, float g, float b, int lifeTime )
{
	vec3_t color;
	color[0] = r; color[1] = g; color[2] = b;
	MakeRailTrail(start,end,box,color,lifeTime);
}

void CEngine::MakeRailTrail( vec3_t start, vec3_t end, bool box, vec3_t color, int lifeTime )
{
	if (!orig_CG_AllocLocalEntity || !eth32.cg.media.railCore)
		return;

	// use cg_railTrailTime cvar
	if ( lifeTime < 0 )
		lifeTime = eth32.game.cg_railTrailTime->integer;

	// just a line
	if (!box) {	
		_CG_RailTrail( start, end, color, lifeTime );
		return;
	}
	vec3_t	diff, v1, v2, v3, v4, v5, v6;

	VectorSubtract(start, end, diff);

	VectorCopy(start, v1);
	VectorCopy(start, v2);
	VectorCopy(start, v3);
	v1[0] -= diff[0];
	v2[1] -= diff[1];
	v3[2] -= diff[2];
	_CG_RailTrail( start, v1, color, lifeTime);
	_CG_RailTrail( start, v2, color, lifeTime);
	_CG_RailTrail( start, v3, color, lifeTime);

	VectorCopy(end, v4);
	VectorCopy(end, v5);
	VectorCopy(end, v6);
	v4[0] += diff[0];
	v5[1] += diff[1];
	v6[2] += diff[2];
	_CG_RailTrail( end, v4, color, lifeTime);
	_CG_RailTrail( end, v5, color, lifeTime);
	_CG_RailTrail( end, v6, color, lifeTime);

	_CG_RailTrail( v2, v6, color, lifeTime);
	_CG_RailTrail( v6, v1, color, lifeTime);
	_CG_RailTrail( v1, v5, color, lifeTime);

	_CG_RailTrail( v2, v4, color, lifeTime);
	_CG_RailTrail( v4, v3, color, lifeTime);
	_CG_RailTrail( v3, v5, color, lifeTime);
}


int __cdecl eth32_CG_CalculateReinfTime(qboolean menu)
{
	eth32.cg.bools.spawntimerReady = true;
	UNDETFUNC(CG_CalculateReinfTime)
	orig_CG_CalculateReinfTime = (CG_CalculateReinfTime_t) ((DWORD) eth32.cg.module + eth32.cgMod->CG_CalculateReinfTime);
	return orig_CG_CalculateReinfTime(menu);
}


// kobject: CG_Player directly sets up head/body refents and adds them, so its safe to use these checks in there
void __cdecl eth32_CG_Player(centity_t *cent)
{
	// Using CG_Player to filter valid players for us
	Engine.StartPlayerCapture(cent);
	orig_CG_Player(cent);
	Engine.EndPlayerCapture();
}

void __cdecl eth32_CG_AddPlayerWeapon(refEntity_t *parent, playerState_t *ps, centity_t *cent)
{
	Engine.SetDrawWeapon(true);
	orig_CG_AddPlayerWeapon(parent, ps, cent);
	Engine.SetDrawWeapon(false);
}

void __cdecl eth32_CG_PositionRotatedEntityOnTag(refEntity_t *entity, const refEntity_t *parent, const char *tagName)
{
	orig_CG_PositionRotatedEntityOnTag(entity, parent, tagName);

	if (!strcmp(tagName, "tag_head")){
		Engine.CapturePlayerHead(entity);
	}
}

void __cdecl eth32_CG_EntityEvent(centity_t *cent, vec3_t position)
{
	entityState_t *es;
	es = &cent->currentState;
	
	int event;
	event = es->event & ~EV_EVENT_BITS;

	switch(event)
	{	
	case EV_SHAKE:
		return;
	case EV_OBITUARY:
		Engine.EV_Obituary(es);
		break;
	case EV_BULLET_HIT_WALL:
		Engine.EV_Bullet(es, false);
		break;
	case EV_BULLET_HIT_FLESH:
		Engine.EV_Bullet(es, true);
		break;
	case EV_POPUPMESSAGE:
		if (es->effect1Time == PM_TEAM && es->density == 1) { // client disconnected
			int num = Tools.ClientNumForAddr((DWORD)cent);
			if (num >= 0 && num < MAX_CLIENTS) // sanity check
				memset(&eth32.cg.players[num].stats, 0, sizeof(stats_t)); // clear stats on disconnecting client
		}
		break;		
	default:
		break;	
	}

	orig_CG_EntityEvent(cent, position); 
}

void __cdecl eth32_CG_Trace(trace_t *result, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int skipNumber, int mask)
{
	orig_CG_Trace(result, start, mins, maxs, end, skipNumber, mask);

	// TC:E checks if a player is visible to avoid drawing invisible ones
	// so we make it think it's visible
	if (eth32.cg.snap && skipNumber == eth32.cg.snap->ps.clientNum)
		result->fraction = 1.0f;
}

void __cdecl eth32_CG_DamageFeedback(int yawByte, int pitchByte, int damage)
{
	// Hmm... did we forget to kick the player's view when hit? oops...
	if (eth32.settings.dmgFeedback)
		orig_CG_DamageFeedback(yawByte, pitchByte, damage);
}

void __cdecl eth32_CG_WeaponFireRecoil(centity_t *cent)
{
	// TODO: Add recoil when we fire a weapon...oh wait, nm ;)
}

__forceinline bool CEngine::IsVisible(vec3_t start, vec3_t pt, int skipEntity)// added to CEngine for now 
{
	trace_t t;

	orig_CG_Trace(&t, start, NULL, NULL, pt, skipEntity, MASK_SHOT);

	return (t.fraction == 1.f);
}

// kobject: fixed setMuzzle() check SDK, uses muzzleTrace, not muzzlePoint
__forceinline void CEngine::setMuzzle(void) 
{
	VectorCopy( eth32.cg.refdef->vieworg, eth32.cg.muzzle );

	if(eth32.cg.snap->ps.viewangles[PITCH] > 30.0 && (eth32.cg.snap->ps.eFlags & EF_CROUCHING))
		eth32.cg.muzzle[2] += 30.0 - eth32.cg.snap->ps.viewheight;

	SnapVector(eth32.cg.muzzle);
}

int CEngine::findSatchel(void)
{
	int entityNum = 0;
	for (; entityNum < MAX_GENTITIES; entityNum++) {
		if ((eth32.cg.gentity[entityNum].currentState->weapon == WP_SATCHEL)
				&& (eth32.cg.gentity[entityNum].currentState->clientNum == eth32.cg.snap->ps.clientNum)
				&& (!VectorCompare(eth32.cg.gentity[entityNum].lerpOrigin, vec3_origin)))
			return entityNum;
	}

	return -1;
}

int CEngine::CalcViewValues()
{
	playerState_t *ps = eth32.settings.interpolatedPs ? eth32.cg.predictedps : &eth32.cg.snap->ps;

	if (ps->eFlags & EF_MG42_ACTIVE || ps->eFlags & EF_AAGUN_ACTIVE || ps->eFlags & EF_MOUNTEDTANK ||
		(eth32.cg.snap->ps.pm_flags & PMF_LIMBO) || eth32.cg.snap->ps.stats[STAT_HEALTH] <= 0 )
		return orig_CG_CalcViewValues();

	// set refdef size
	eth32.cg.refdef->width = eth32.game.glconfig->vidWidth;
	eth32.cg.refdef->width &= ~1;

	eth32.cg.refdef->height = eth32.game.glconfig->vidHeight;
	eth32.cg.refdef->height &= ~1;

	eth32.cg.refdef->x = (eth32.game.glconfig->vidWidth - eth32.cg.refdef->width)/2;
	eth32.cg.refdef->y = (eth32.game.glconfig->vidHeight - eth32.cg.refdef->height)/2;
	
	if (eth32.cg.snap->ps.pm_type == PM_INTERMISSION){
		VectorCopy( ps->origin, eth32.cg.refdef->vieworg );
		VectorCopy( ps->viewangles, eth32.cg.refdefViewAngles );
		AnglesToAxis( eth32.cg.refdefViewAngles, eth32.cg.refdef->viewaxis );

		eth32.cg.refdef->fov_x = eth32.game.cg_fov->value;
		float x = eth32.cg.refdef->width / tan( eth32.cg.refdef->fov_x / 360 * M_PI );
		eth32.cg.refdef->fov_y = atan2( eth32.cg.refdef->height, x );
		eth32.cg.refdef->fov_y = eth32.cg.refdef->fov_y * 360 / M_PI;
		return 0;
	}

	VectorCopy( ps->origin, eth32.cg.refdef->vieworg );
	VectorCopy( ps->viewangles, eth32.cg.refdefViewAngles );

	eth32.cg.refdef->vieworg[2] += ps->viewheight;

	AnglesToAxis( eth32.cg.refdefViewAngles, eth32.cg.refdef->viewaxis );

	eth32.cg.refdef->fov_x = eth32.game.cg_fov->value;
	float x = eth32.cg.refdef->width / tan( eth32.cg.refdef->fov_x / 360 * M_PI );
	eth32.cg.refdef->fov_y = atan2( eth32.cg.refdef->height, x );
	eth32.cg.refdef->fov_y = eth32.cg.refdef->fov_y * 360 / M_PI;

	return 0;
}