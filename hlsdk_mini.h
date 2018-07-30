// CONSTANTS
#define MAX_PHYSENTS 			600
#define MAX_MOVEENTS 			64
#define MAX_PHYSINFO_STRING		256
#define MAX_ENT_LEAFS			48
#define HISTORY_MAX 				64
#define MAX_INFO_STRING			256
#define MAX_SCOREBOARDNAME		32
#define MAX_QPATH 				64

// TYPE
typedef int	qboolean;
typedef int	func_t;
typedef int	string_t;
typedef float 	vec_t;

// FUNC
typedef int(*pfnUserMsgHook)	(const char *pszName, int iSize, void *pbuf);

// ENUM
typedef enum {
	TRI_FRONT = 0,
	TRI_NONE = 1,
} TRICULLSTYLE;

typedef enum {
	NA_UNUSED,
	NA_LOOPBACK,
	NA_BROADCAST,
	NA_IP,
	NA_IPX,
	NA_BROADCAST_IPX,
} netadrtype_t;

typedef enum {
	t_sound = 0,
	t_skin,
	t_model,
	t_decal,
	t_generic,
	t_eventscript,
	t_world,
} resourcetype_t;

typedef enum {
	MicrophoneVolume = 0,
	OtherSpeakerScale
} VoiceTweakControl;

// STRUCT
typedef struct rect_s {
	int	left, right, top, bottom;
} wrect_t;

typedef struct {
	unsigned r, g, b, a;
} colorVec;

typedef struct {
	vec_t x, y, z;
} vec3_t;

typedef struct {
	int		iSize;
	int		iWidth;
	int		iHeight;
	int		iFlags;
	int		iCharHeight;
	short	charWidths[256];
} SCREENINFO;

typedef struct client_data_s {
	vec3_t	origin;
	vec3_t	viewangles;
	int		iWeaponBits;
	float		fov;
} client_data_t;

typedef struct client_sprite_s {
	char		szName[64];
	char		szSprite[64];
	int		hspr;
	int		iRes;
	wrect_t	rc;
} client_sprite_t;

typedef struct {
	int			effect;
	byte			r1, g1, b1, a1;
	byte			r2, g2, b2, a2;
	float			x;
	float			y;
	float			fadein;
	float			fadeout;
	float			holdtime;
	float			fxtime;
	const char*	pName;
	const char*	pMessage;
} client_textmessage_t;

typedef struct {
	char*	name;
	short 	ping;
	byte 	thisplayer;
	byte 	spectator;
	byte 	packetloss;
	char*	model;
	short 	topcolor;
	short 	bottomcolor;
} hud_player_info_t;

typedef struct {
	float		animtime;
	vec3_t	origin;
	vec3_t	angles;
} position_history_t;

typedef struct {
	byte r, g, b;
} color24;

typedef struct cvar_s {
	char*	name;
	char*	string;
	int		flags;
	float		value;
	cvar_s*	next;
} cvar_t;

typedef struct link_s {
	link_s	*prev, *next;
} link_t;

typedef struct {
	byte	mouthopen;
	byte	sndcount;
	int 	sndavg;
} mouth_t;

typedef struct {
	vec3_t	normal;
	float		dist;
} pmplane_t;

typedef struct {
	float		prevanimtime;
	float		sequencetime;
	byte		prevseqblending[2];
	vec3_t	prevorigin;
	vec3_t	prevangles;
	int 		prevsequence;
	float		prevframe;
	byte		prevcontroller[4];
	byte		prevblending[2];
} latchedvars_t;

typedef struct usercmd_s {
	short			lerp_msec;
	byte				msec;
	vec3_t			viewangles;
	float				forwardmove;
	float				sidemove;
	float				upmove;
	byte				lightlevel;
	unsigned short  	buttons;
	byte   			impulse;
	byte				weaponselect;
	int				impact_index;
	vec3_t			impact_position;
} usercmd_t;

typedef struct {
	vec3_t	normal;
	float		dist;
} plane_t;

typedef struct weapon_data_s {
	int	m_iId;
	int	m_iClip;
	float	m_flNextPrimaryAttack;
	float	m_flNextSecondaryAttack;
	float	m_flTimeWeaponIdle;
	int	m_fInReload;
	int	m_fInSpecialReload;
	float	m_flNextReload;
	float	m_flPumpTime;
	float	m_fReloadTime;
	float	m_fAimedDamage;
	float	m_fNextAimBonus;
	int	m_fInZoom;
	int	m_iWeaponState;
	int	iuser1;
	int	iuser2;
	int	iuser3;
	int	iuser4;
	float	fuser1;
	float	fuser2;
	float	fuser3;
	float	fuser4;
} weapon_data_t;

typedef struct screenfade_s {
	float		fadeSpeed;
	float		fadeEnd;
	float		fadeTotalEnd;
	float		fadeReset;
	byte		fader, fadeg, fadeb, fadealpha;
	int		fadeFlags;
} screenfade_t;

typedef struct kbutton_s {
	int		down[2];		// key nums holding it down
	int		state;			// low bit is down state
} kbutton_t;

typedef struct con_nprint_s {
	int	index;
	float	time_to_live;
	float	color[3];
} con_nprint_t;

typedef struct event_args_s {
	int	flags;
	int	entindex;
	float	origin[3];
	float	angles[3];
	float	velocity[3];
	int	ducking;
	float	fparam1;
	float	fparam2;
	int	iparam1;
	int	iparam2;
	int	bparam1;
	int	bparam2;
} event_args_t;

typedef struct movevars_s {
	float		gravity;
	float		stopspeed;
	float		maxspeed;
	float		spectatormaxspeed;
	float		accelerate;
	float		airaccelerate;
	float		wateraccelerate;
	float		friction;
	float   	edgefriction;
	float		waterfriction;
	float		entgravity;
	float   	bounce;
	float   	stepsize;
	float  	maxvelocity;
	float		zmax;
	float		waveHeight;
	qboolean footsteps;
	char		skyName[32];
	float		rollangle;
	float		rollspeed;
	float		skycolor_r;
	float		skycolor_g;
	float		skycolor_b;
	float		skyvec_x;
	float		skyvec_y;
	float		skyvec_z;
} movevars_t;

typedef struct alight_s {
	int		ambientlight;
	int		shadelight;
	vec3_t	color;
	float*	plightvec;
} alight_t;

typedef struct mplane_s {
	vec3_t	normal;
	float		dist;
	byte		type;
	byte		signbits;
	byte		pad[2];
} mplane_t;

typedef struct mstudioevent_s {
	int 	frame;
	int	event;
	int	type;
	char	options[64];
} mstudioevent_t;

typedef struct demo_api_s {
	int(*IsRecording)	(void);
	int(*IsPlayingback)	(void);
	int(*IsTimeDemo)	(void);
	void(*WriteBuffer)	(int size, unsigned char *buffer);
} demo_api_t;

typedef struct cache_user_s {
	void *data;
} cache_user_t;




// структуры Возможны "пересечения"

typedef struct triangleapi_s {
	int	version;
	void(*RenderMode)	(int mode);
	void(*Begin)			(int primitiveCode);
	void(*End) 			(void);
	void(*Color4f)		(float r, float g, float b, float a);
	void(*Color4ub) 		(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void(*TexCoord2f) 	(float u, float v);
	void(*Vertex3fv)		(float *worldPnt);
	void(*Vertex3f) 		(float x, float y, float z);
	void(*Brightness)		(float brightness);
	void(*CullFace) 		(TRICULLSTYLE style);
	int(*SpriteTexture) 	(struct model_s *pSpriteModel, int frame);
	int(*WorldToScreen) 	(float *world, float *screen);
} triangleapi_t;

typedef struct entity_state_s {
	int 		entityType;
	int 		number;
	float		msg_time;
	int 		messagenum;
	vec3_t	origin;
	vec3_t	angles;
	int 		modelindex;
	int 		sequence;
	float		frame;
	int 		colormap;
	short	skin;
	short	solid;
	int 		effects;
	float		scale;
	byte		eflags;
	int 		rendermode;
	int 		renderamt;
	color24	rendercolor;
	int 		renderfx;
	int 		movetype;
	float		animtime;
	float		framerate;
	int 		body;
	byte		controller[4];
	byte		blending[4];
	vec3_t	velocity;
	vec3_t	mins;
	vec3_t	maxs;
	int 		aiment;
	int 		owner;
	float		friction;
	float		gravity;
	int 		team;
	int 		playerclass;
	int 		health;
	qboolean	spectator;
	int         	weaponmodel;
	int 		gaitsequence;
	vec3_t	basevelocity;
	int 		usehull;
	int 		oldbuttons;
	int 		onground;
	int 		iStepLeft;
	float		flFallVelocity;
	float		fov;
	int 		weaponanim;
	vec3_t	startpos;
	vec3_t	endpos;
	float		impacttime;
	float		starttime;
	int 		iuser1;
	int 		iuser2;
	int 		iuser3;
	int 		iuser4;
	float		fuser1;
	float		fuser2;
	float		fuser3;
	float		fuser4;
	vec3_t	vuser1;
	vec3_t	vuser2;
	vec3_t	vuser3;
	vec3_t	vuser4;
} entity_state_t;

typedef struct edict_s edict_t;

typedef struct entvars_s {
	string_t	classname;
	string_t	globalname;
	vec3_t	origin;
	vec3_t	oldorigin;
	vec3_t	velocity;
	vec3_t	basevelocity;
	vec3_t     clbasevelocity;
	vec3_t	movedir;
	vec3_t	angles;
	vec3_t	avelocity;
	vec3_t	punchangle;
	vec3_t	v_angle;
	vec3_t	endpos;
	vec3_t	startpos;
	float		impacttime;
	float		starttime;
	int 		fixangle;
	float		idealpitch;
	float		pitch_speed;
	float		ideal_yaw;
	float		yaw_speed;
	int 		modelindex;
	string_t	model;
	int 		viewmodel;
	int 		weaponmodel;
	vec3_t	absmin;
	vec3_t	absmax;
	vec3_t	mins;
	vec3_t	maxs;
	vec3_t	size;
	float		ltime;
	float		nextthink;
	int 		movetype;
	int 		solid;
	int 		skin;
	int 		body;
	int 		effects;
	float		gravity;
	float		friction;
	int 		light_level;
	int 		sequence;
	int 		gaitsequence;
	float		frame;
	float		animtime;
	float		framerate;
	byte		controller[4];
	byte		blending[2];
	float		scale;
	int 		rendermode;
	float		renderamt;
	vec3_t	rendercolor;
	int 		renderfx;
	float		health;
	float		frags;
	int 		weapons;
	float		takedamage;
	int 		deadflag;
	vec3_t	view_ofs;
	int 		button;
	int 		impulse;
	edict_t*	chain;
	edict_t*	dmg_inflictor;
	edict_t*	enemy;
	edict_t*	aiment;
	edict_t*	owner;
	edict_t*	groundentity;
	int 		spawnflags;
	int 		flags;
	int 		colormap;
	int 		team;
	float		max_health;
	float		teleport_time;
	float		armortype;
	float		armorvalue;
	int 		waterlevel;
	int 		watertype;
	string_t	target;
	string_t	targetname;
	string_t	netname;
	string_t	message;
	float		dmg_take;
	float		dmg_save;
	float		dmg;
	float		dmgtime;
	string_t	noise;
	string_t	noise1;
	string_t	noise2;
	string_t	noise3;
	float		speed;
	float		air_finished;
	float		pain_finished;
	float		radsuit_finished;
	edict_t*	pContainingEntity;
	int 		playerclass;
	float		maxspeed;
	float		fov;
	int 		weaponanim;
	int 		pushmsec;
	int 		bInDuck;
	int 		flTimeStepSound;
	int 		flSwimTime;
	int 		flDuckTime;
	int 		iStepLeft;
	float		flFallVelocity;
	int 		gamestate;
	int 		oldbuttons;
	int 		groupinfo;
	int 		iuser1;
	int 		iuser2;
	int 		iuser3;
	int 		iuser4;
	float		fuser1;
	float		fuser2;
	float		fuser3;
	float		fuser4;
	vec3_t	vuser1;
	vec3_t	vuser2;
	vec3_t	vuser3;
	vec3_t	vuser4;
	edict_t*	euser1;
	edict_t*	euser2;
	edict_t*	euser3;
	edict_t*	euser4;
} entvars_t;

typedef struct edict_s {
	qboolean	free;
	int 		serialnumber;
	link_t	area;
	int 		headnode;
	int 		num_leafs;
	short	leafnums[MAX_ENT_LEAFS];
	float		freetime;
	void*	pvPrivateData;
	entvars_t	v;
} edict_t;

typedef struct {
	qboolean	allsolid;
	qboolean	startsolid;
	qboolean	inopen, inwater;
	float		fraction;
	vec3_t	endpos;
	plane_t	plane;
	edict_t*	ent;
	int		hitgroup;
} trace_t;

typedef struct pmtrace_s {
	qboolean		allsolid;
	qboolean		startsolid;
	qboolean		inopen, inwater;
	float			fraction;
	vec3_t		endpos;
	pmplane_t	plane;
	int 			ent;
	vec3_t      	deltavelocity;
	int         		hitgroup;
} pmtrace_t;

typedef struct physent_s {
	char 	name[32];
	int		player;
	vec3_t 	origin;
	struct model_s*	model;
	struct model_s*	studiomodel;
	vec3_t 	mins, maxs;
	int		info;
	vec3_t 	angles;
	int		solid;
	int		skin;
	int		rendermode;
	float 	frame;
	int		sequence;
	byte 	controller[4];
	byte 	blending[2];
	int		movetype;
	int		takedamage;
	int		blooddecal;
	int		team;
	int		classnumber;
	int		iuser1;
	int		iuser2;
	int		iuser3;
	int		iuser4;
	float 	fuser1;
	float 	fuser2;
	float 	fuser3;
	float 	fuser4;
	vec3_t 	vuser1;
	vec3_t 	vuser2;
	vec3_t 	vuser3;
	vec3_t 	vuser4;
} physent_t;

typedef struct mnode_s {
	int				contents;
	int				visframe;
	short			minmaxs[6];
	mnode_s*		parent;
	mplane_t*		plane;
	mnode_s*		children[2];
	unsigned short	firstsurface;
	unsigned short	numsurfaces;
} mnode_t;

typedef struct cl_entity_s {
	int 				index;
	qboolean			player;
	entity_state_t		baseline;
	entity_state_t 		prevstate;
	entity_state_t 		curstate;
	int 				current_position;
	position_history_t	ph[HISTORY_MAX];
	mouth_t			mouth;
	latchedvars_t 		latched;
	float				lastmove;
	vec3_t			origin;
	vec3_t			angles;
	vec3_t			attachment[4];
	int 				trivial_accept;
	struct model_s*	model;
	struct efrag_s*		efrag;
	mnode_s*		topnode;
	float				syncbase;
	int 				visframe;
	colorVec			cvFloorColor;
} cl_entity_t;

typedef struct netadr_s {
	netadrtype_t		type;
	unsigned char		ip[4];
	unsigned char		ipx[10];
	unsigned short	port;
} netadr_t;

typedef HSPRITE(*PFNSPR_LOAD)					(const char *szPicName);
typedef int(*PFNSPR_FRAMES)				(HSPRITE hPic);
typedef int(*PFNSPR_HEIGHT)				(HSPRITE hPic, int frame);
typedef int(*PFNSPR_WIDTH)				(HSPRITE hPic, int frame);
typedef void(*PFNSPR_SET)					(HSPRITE hPic, int r, int g, int b);
typedef void(*PFNSPR_DRAW)					(int frame, int x, int y, const wrect_t *prc);
typedef void(*PFNSPR_DRAWHOLES)			(int frame, int x, int y, const wrect_t *prc);
typedef void(*PFNSPR_DRAWADDITIVE)			(int frame, int x, int y, const wrect_t *prc);
typedef void(*PFNSPR_ENABLESCISSOR)		(int x, int y, int width, int height);
typedef void(*PFNSPR_DISABLESCISSOR)		(void);
typedef client_sprite_t*			(*PFNSPR_GETLIST)				(char* psz, int* piCount);
typedef void(*PFNFILLRGBA)					(int x, int y, int width, int height, int r, int g, int b, int a);
typedef int(*PFNGETSCREENINFO)			(SCREENINFO *pscrinfo);
typedef void(*PFNSETCROSSHAIR)				(HSPRITE hspr, wrect_t rc, int r, int g, int b);
typedef cvar_s*				(*PFNREGISTERVARIABLE)			(char *szName, char *szValue, int flags);
typedef float(*PFNGETCVARFLOAT)				(char *szName);
typedef char*					(*PFNGETCVARSTRING)			(char *szName);
typedef int(*PFNADDCOMMAND)				(char *cmd_name, void(*function)(void));
typedef int(*PFNHOOKUSERMSG)				(char *szMsgName, pfnUserMsgHook pfn);
typedef int(*PFNSERVERCMD)				(char *szCmdString);
typedef int(*PFNCLIENTCMD)				(char *szCmdString);
typedef void(*PFNGETPLAYERINFO)			(int ent_num, hud_player_info_t *pinfo);
typedef void(*PFNPLAYSOUNDBYNAME)		(char *szSound, float volume);
typedef void(*PFNPLAYSOUNDBYINDEX)		(int iSound, float volume);
typedef void(*PFNANGLEVECTORS)				(const float * vecAngles, float * forward, float * right, float * up);
typedef client_textmessage_t*	(*PFNTEXTMESSAGEGET)			(const char *pName);
typedef int(*PFNDRAWCHARACTER)			(int x, int y, int number, int r, int g, int b);
typedef int(*PFNDRAWCONSOLESTRING)		(int x, int y, char *string);
typedef void(*PFNDRAWSETTEXTCOLOR)		(float r, float g, float b);
typedef void(*PFNDRAWCONSOLESTRINGLEN)	(const char *string, int *length, int *height);
typedef void(*PFNCONSOLEPRINT)				(const char *string);
typedef void(*PFNCENTERPRINT)				(const char *string);
typedef int(*GETWINDOWCENTERX)			(void);
typedef int(*GETWINDOWCENTERY)			(void);
typedef void(*GETVIEWANGLES)				(float *);
typedef void(*SETVIEWANGLES)				(float *);
typedef int(*GETMAXCLIENTS)				(void);
typedef void(*CVAR_SETVALUE)				(char *cvar, float value);
typedef int(*CMD_ARGC)					(void);
typedef char*					(*CMD_ARGV)					(int arg);
typedef void(*CON_PRINTF)					(char *fmt, ...);
typedef void(*CON_DPRINTF)					(char *fmt, ...);
typedef void(*CON_NPRINTF)					(int pos, char *fmt, ...);
typedef void(*CON_NXPRINTF)					(con_nprint_s *info, char *fmt, ...);
typedef const char*				(*PHYSINFO_VALUEFORKEY)		(const char *key);
typedef const char*				(*SERVERINFO_VALUEFORKEY)		(const char *key);
typedef float(*GETCLIENTMAXSPEED)			(void);
typedef int(*CHECKPARM)					(char *parm, char **ppnext);
typedef void(*KEY__EVENT)					(int key, int down);
typedef void(*GETMOUSEPOSITION)			(int *mx, int *my);
typedef int(*ISNOCLIPPING)					(void);
typedef cl_entity_s*			(*GETLOCALPLAYER)				(void);
typedef cl_entity_s*			(*GETVIEWMODEL)				(void);
typedef cl_entity_s*			(*GETENTITYBYINDEX)			(int idx);
typedef float(*GETCLIENTTIME)				(void);
typedef void(*V_CALCSHAKE)					(void);
typedef void(*V_APPLYSHAKE)					(float *origin, float *angles, float factor);
typedef int(*PM_POINTCONTENTS)			(float *point, int *truecontents);
typedef int(*PM_WATERENTITY)				(float *p);
typedef pmtrace_s*				(*PM_TRACELINE)					(float *start, float *end, int flags, int usehull, int ignore_pe);
typedef struct model_s*			(*CL_LOADMODEL)				(const char *modelname, int *index);
typedef int(*CL_CREATEVISIBLEENTITY)		(int type, cl_entity_s *ent);
typedef const struct model_s*	(*GETSPRITEPOINTER)				(HSPRITE hSprite);
typedef void(*PFNPLAYSOUNDBYNAMEATLOCATION)	(char *szSound, float volume, float *origin);
typedef unsigned short(*PFNPRECACHEEVENT)			(int type, const char* psz);
typedef void(*PFNPLAYBACKEVENT)			(int flags, const edict_s *pInvoker, unsigned short eventindex, float delay, float *origin, float *angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2);
typedef void(*PFNWEAPONANIM)				(int iAnim, int body);
typedef float(*PFNRANDOMFLOAT)				(float flLow, float flHigh);
typedef long(*PFNRANDOMLONG)				(long lLow, long lHigh);
typedef void(*PFNHOOKEVENT)				(char *name, void(*pfnEvent)(event_args_s* args));
typedef int(*CON_ISVISIBLE)					(void);
typedef const char*				(*PFNGETGAMEDIRECTORY)		(void);
typedef cvar_s*				(*PFNGETCVARPOINTER)			(const char *szName);
typedef const char*				(*KEY_LOOKUPBINDING)			(const char *pBinding);
typedef const char*				(*PFNGETLEVELNAME)				(void);
typedef void(*PFNGETSCREENFADE)			(screenfade_s *fade);
typedef void(*PFNSETSCREENFADE)			(screenfade_s *fade);
typedef void*					(*VGUI_GETPANEL)				(void);
typedef void(*VGUI_VIEWPORTPAINTBACKGROUND)	(int extents[4]);
typedef byte*					(*COM_LOADFILE)					(char *path, int usehunk, int *pLength);
typedef char*					(*COM_PARSEFILE)				(char *data, char *token);
typedef void(*COM_FREEFILE)					(void *buffer);
typedef int(*ISSPECTATEONLY)				(void);
typedef struct model_s*			(*LOADMAPSPRITE)				(const char *filename);
typedef void(*COM_ADDAPPDIRECTORYTOSEARCHPATH)	(const char *pszBaseDir, const char *appName);
typedef int(*COM_EXPANDFILENAME)			(const char *fileName, char *nameOutBuffer, int nameOutBufferSize);
typedef const char*				(*PLAYERINFO_VALUEFORKEY)		(int playerNum, const char *key);
typedef void(*PLAYERINFO_SETVALUEFORKEY)	(const char *key, const char *value);
typedef qboolean(*GETPLAYERUNIQUEID)			(int iPlayer, char playerID[16]);
typedef int(*GETTRACKERIDFORPLAYER)		(int playerSlot);
typedef int(*GETPLAYERFORTRACKERID)		(int trackerID);
typedef int(*PFNSERVERCMDUNRELIABLE)		(char *szCmdString);

typedef struct playermove_s {
	int				player_index;
	qboolean			server;
	qboolean			multiplayer;
	float 			time;
	float 			frametime;
	vec3_t 			forward, right, up;
	vec3_t 			origin;
	vec3_t 			angles;
	vec3_t 			oldangles;
	vec3_t 			velocity;
	vec3_t 			movedir;
	vec3_t 			basevelocity;
	vec3_t 			view_ofs;
	float 			flDuckTime;
	qboolean			bInDuck;
	int				flTimeStepSound;
	int				iStepLeft;
	float				flFallVelocity;
	vec3_t 			punchangle;
	float 			flSwimTime;
	float 			flNextPrimaryAttack;
	int				effects;
	int				flags;
	int				usehull;
	float 			gravity;
	float 			friction;
	int				oldbuttons;
	float 			waterjumptime;
	qboolean			dead;
	int				deadflag;
	int				spectator;
	int				movetype;
	int				onground;
	int				waterlevel;
	int				watertype;
	int				oldwaterlevel;
	char 			sztexturename[256];
	char 			chtexturetype;
	float				maxspeed;
	float 			clientmaxspeed;
	int				iuser1;
	int				iuser2;
	int				iuser3;
	int				iuser4;
	float 			fuser1;
	float 			fuser2;
	float 			fuser3;
	float 			fuser4;
	vec3_t			vuser1;
	vec3_t 			vuser2;
	vec3_t			vuser3;
	vec3_t 			vuser4;
	int				numphysent;
	physent_t		physents[MAX_PHYSENTS];
	int				nummoveent;
	physent_t		moveents[MAX_MOVEENTS];
	int				numvisent;
	physent_t		visents[MAX_PHYSENTS];
	usercmd_t		cmd;
	int				numtouch;
	pmtrace_t		touchindex[MAX_PHYSENTS];
	char 			physinfo[MAX_PHYSINFO_STRING];
	movevars_s*		movevars;
	vec3_t 			player_mins[4];
	vec3_t 			player_maxs[4];
	const char*		(*PM_Info_ValueForKey) 	(const char *s, const char *key);
	void(*PM_Particle)				(vec3_t origin, int color, float life, int zpos, int zvel);
	int(*PM_TestPlayerPosition)	(vec3_t pos, pmtrace_t *ptrace);
	void(*Con_NPrintf)				(int idx, char *fmt, ...);
	void(*Con_DPrintf)				(char *fmt, ...);
	void(*Con_Printf)				(char *fmt, ...);
	double(*Sys_FloatTime)			(void);
	void(*PM_StuckTouch)			(int hitent, pmtrace_t *ptraceresult);
	int(*PM_PointContents) 		(vec3_t p, int *truecontents /*filled in if this is non-null*/);
	int(*PM_TruePointContents)	(vec3_t p);
	int(*PM_HullPointContents) 	(struct hull_s *hull, int num, vec3_t p);
	pmtrace_t(*PM_PlayerTrace) 			(vec3_t start, vec3_t end, int traceFlags, int ignore_pe);
	PM_TRACELINE 	PM_TraceLine;
	long(*RandomLong)			(long lLow, long lHigh);
	float(*RandomFloat)			(float flLow, float flHigh);
	int(*PM_GetModelType)		(struct model_s *mod);
	void(*PM_GetModelBounds)		(struct model_s *mod, vec3_t mins, vec3_t maxs);
	void*			(*PM_HullForBsp)			(physent_t *pe, vec3_t offset);
	float(*PM_TraceModel)			(physent_t *pEnt, vec3_t start, vec3_t end, trace_t *trace);
	int(*COM_FileSize)			(char *filename);
	byte*			(*COM_LoadFile)			(char *path, int usehunk, int *pLength);
	void(*COM_FreeFile) 			(void *buffer);
	char*			(*memfgets)				(byte *pMemFile, int fileSize, int *pFilePos, char *pBuffer, int bufferSize);
	qboolean			runfuncs;
	void(*PM_PlaySound) 			(int channel, const char *sample, float volume, float attenuation, int fFlags, int pitch);
	const char*		(*PM_TraceTexture)		(int ground, vec3_t vstart, vec3_t vend);
	void(*PM_PlaybackEventFull) 	(int flags, int clientindex, unsigned short eventindex, float delay, float *origin, float *angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2);
} playermove_t;

typedef struct event_api_s {
	int			version;
	void(*EV_PlaySound) 				(int ent, float *origin, int channel, const char *sample, float volume, float attenuation, int fFlags, int pitch);
	void(*EV_StopSound)				(int ent, int channel, const char *sample);
	int(*EV_FindModelIndex)			(const char *pmodel);
	int(*EV_IsLocal) 				(int playernum);
	int(*EV_LocalPlayerDucking) 		(void);
	void(*EV_LocalPlayerViewheight) 	(float *);
	void(*EV_LocalPlayerBounds) 		(int hull, float *mins, float *maxs);
	int(*EV_IndexFromTrace) 			(pmtrace_s *pTrace);
	physent_s*	(*EV_GetPhysent)				(int idx);
	void(*EV_SetUpPlayerPrediction)	(int dopred, int bIncludeLocalClient);
	void(*EV_PushPMStates)			(void);
	void(*EV_PopPMStates) 			(void);
	void(*EV_SetSolidPlayers)			(int playernum);
	void(*EV_SetTraceHull) 			(int hull);
	void(*EV_PlayerTrace) 			(float *start, float *end, int traceFlags, int ignore_pe, pmtrace_s *tr);
	void(*EV_WeaponAnimation) 		(int sequence, int body);
	unsigned short(*EV_PrecacheEvent) 			(int type, const char* psz);
	void(*EV_PlaybackEvent) 			(int flags, const edict_s *pInvoker, unsigned short eventindex, float delay, float *origin, float *angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2);
	const char*	(*EV_TraceTexture) 			(int ground, float *vstart, float *vend);
	void(*EV_StopAllSounds) 			(int entnum, int entchannel);
	void(*EV_KillEvents) 				(int entnum, const char *eventname);
} event_api_t;

typedef struct net_response_s {
	int		error;
	int		context;
	int		type;
	netadr_t	remote_address;
	double	ping;
	void*	response;
} net_response_t;

typedef void(*net_api_response_func_t)	(net_response_s *response);

typedef struct net_adrlist_s {
	net_adrlist_s*	next;
	netadr_t		remote_address;
} net_adrlist_t;

typedef struct net_status_s {
	int		connected;
	netadr_t	local_address;
	netadr_t	remote_address;
	int		packet_loss;
	double	latency;
	double	connection_time;
	double	rate;
} net_status_t;

typedef struct net_api_s {
	void(*InitNetworking)		(void);
	void(*Status) 			(net_status_s *status);
	void(*SendRequest) 		(int context, int request, int flags, double timeout, netadr_s *remote_address, net_api_response_func_t response);
	void(*CancelRequest) 		(int context);
	void(*CancelAllRequests) 	(void);
	char*		(*AdrToString)		(netadr_s *a);
	int(*CompareAdr) 		(netadr_s *a, netadr_s *b);
	int(*StringToAdr) 		(char *s, netadr_s *a);
	const char*	(*ValueForKey)		(const char *s, const char *key);
	void(*RemoveKey) 		(char *s, const char *key);
	void(*SetValueForKey) 	(char *s, const char *key, const char *value, int maxsize);
} net_api_t;

typedef struct ref_params_s {
	float			vieworg[3];
	float			viewangles[3];
	float			forward[3];
	float			right[3];
	float			up[3];
	float			frametime;
	float			time;
	int			intermission;
	int			paused;
	int			spectator;
	int			onground;
	int			waterlevel;
	float			simvel[3];
	float			simorg[3];
	float			viewheight[3];
	float			idealpitch;
	float			cl_viewangles[3];
	int			health;
	float			crosshairangle[3];
	float			viewsize;
	float			punchangle[3];
	int			maxclients;
	int			viewentity;
	int			playernum;
	int			max_entities;
	int			demoplayback;
	int			hardware;
	int			smoothing;
	usercmd_s*	cmd;
	movevars_s*	movevars;
	int			viewport[4];
	int			nextView;
	int			onlyClientDraw;
} ref_params_t;

typedef struct clientdata_s {
	vec3_t	origin;
	vec3_t	velocity;
	int		viewmodel;
	vec3_t	punchangle;
	int		flags;
	int		waterlevel;
	int		watertype;
	vec3_t	view_ofs;
	float		health;
	int		bInDuck;
	int		weapons;
	int		flTimeStepSound;
	int		flDuckTime;
	int		flSwimTime;
	int		waterjumptime;
	float		maxspeed;
	float		fov;
	int		weaponanim;
	int		m_iId;
	int		ammo_shells;
	int		ammo_nails;
	int		ammo_cells;
	int		ammo_rockets;
	float		m_flNextAttack;
	int		tfstate;
	int		pushmsec;
	int		deadflag;
	char		physinfo[MAX_PHYSINFO_STRING];
	int		iuser1;
	int		iuser2;
	int		iuser3;
	int		iuser4;
	float		fuser1;
	float		fuser2;
	float		fuser3;
	float		fuser4;
	vec3_t	vuser1;
	vec3_t	vuser2;
	vec3_t	vuser3;
	vec3_t	vuser4;
} clientdata_t;

typedef struct local_state_s {
	entity_state_t		playerstate;
	clientdata_t		client;
	weapon_data_t	weapondata[32];
} local_state_t;

typedef struct resource_s {
	char              		szFileName[MAX_QPATH];
	resourcetype_t	type;
	int               		nIndex;
	int               		nDownloadSize;
	unsigned char		ucFlags;
	unsigned char		rgucMD5_hash[16];
	unsigned char		playernum;
	unsigned char		rguc_reserved[32];
	resource_s*		pNext;
	resource_s*		pPrev;
} resource_t;

typedef struct customization_s {
	qboolean 			bInUse;
	resource_t			resource;
	qboolean				bTranslated;
	int        				nUserData1;
	int        				nUserData2;
	void*				pInfo;
	void*				pBuffer;
	customization_s*		pNext;
} customization_t;

typedef struct player_info_s {
	int				userid;
	char				userinfo[MAX_INFO_STRING];
	char				name[MAX_SCOREBOARDNAME];
	int				spectator;
	int				ping;
	int				packet_loss;
	char				model[MAX_QPATH];
	int				topcolor;
	int				bottomcolor;
	int				renderframe;
	int				gaitsequence;
	float				gaitframe;
	float				gaityaw;
	vec3_t			prevgaitorigin;
	customization_t 	customdata;
} player_info_t;

typedef struct r_studio_interface_s {
	int	version;
	int(*StudioDrawModel)	(int flags);
	int(*StudioDrawPlayer)	(int flags, entity_state_s *pplayer);
} r_studio_interface_t;

typedef struct tempent_s	TEMPENTITY;
typedef struct tempent_s {
	int			flags;
	float			die;
	float			frameMax;
	float			x;
	float			y;
	float			z;
	float			fadeSpeed;
	float			bounceFactor;
	int			hitSound;
	void(*hitcallback)	(tempent_s *ent, pmtrace_s *ptr);
	void(*callback)	(tempent_s *ent, float frametime, float currenttime);
	TEMPENTITY*	next;
	int			priority;
	short		clientIndex;
	vec3_t		tentOffset;
	cl_entity_t	entity;
} TEMPENTITY;

typedef struct IVoiceTweak_s {
	int(*StartVoiceTweakMode)	(void);
	void(*EndVoiceTweakMode)	(void);
	void(*SetControlFloat)		(VoiceTweakControl iControl, float value);
	float(*GetControlFloat)		(VoiceTweakControl iControl);
} IVoiceTweak;

// EngFuncs
typedef struct cl_enginefuncs_s {
	PFNSPR_LOAD 				pfnSPR_Load;
	PFNSPR_FRAMES 				pfnSPR_Frames;
	PFNSPR_HEIGHT 				pfnSPR_Height;
	PFNSPR_WIDTH 				pfnSPR_Width;
	PFNSPR_SET 					pfnSPR_Set;
	PFNSPR_DRAW 				pfnSPR_Draw;
	PFNSPR_DRAWHOLES 			pfnSPR_DrawHoles;
	PFNSPR_DRAWADDITIVE 		pfnSPR_DrawAdditive;
	PFNSPR_ENABLESCISSOR 		pfnSPR_EnableScissor;
	PFNSPR_DISABLESCISSOR 		pfnSPR_DisableScissor;
	PFNSPR_GETLIST 				pfnSPR_GetList;
	PFNFILLRGBA 				pfnFillRGBA;
	PFNGETSCREENINFO 			pfnGetScreenInfo;
	PFNSETCROSSHAIR 			pfnSetCrosshair;
	PFNREGISTERVARIABLE 		pfnRegisterVariable;
	PFNGETCVARFLOAT 			pfnGetCvarFloat;
	PFNGETCVARSTRING 			pfnGetCvarString;
	PFNADDCOMMAND 			pfnAddCommand;
	PFNHOOKUSERMSG			pfnHookUserMsg;
	PFNSERVERCMD 				pfnServerCmd;
	PFNCLIENTCMD 				pfnClientCmd;
	PFNGETPLAYERINFO 			pfnGetPlayerInfo;
	PFNPLAYSOUNDBYNAME 		pfnPlaySoundByName;
	PFNPLAYSOUNDBYINDEX 		pfnPlaySoundByIndex;
	PFNANGLEVECTORS 			pfnAngleVectors;
	PFNTEXTMESSAGEGET 			pfnTextMessageGet;
	PFNDRAWCHARACTER 			pfnDrawCharacter;
	PFNDRAWCONSOLESTRING 		pfnDrawConsoleString;
	PFNDRAWSETTEXTCOLOR 		pfnDrawSetTextColor;
	PFNDRAWCONSOLESTRINGLEN 	pfnDrawConsoleStringLen;
	PFNCONSOLEPRINT 			pfnConsolePrint;
	PFNCENTERPRINT 				pfnCenterPrint;
	GETWINDOWCENTERX 			GetWindowCenterX;
	GETWINDOWCENTERY 			GetWindowCenterY;
	GETVIEWANGLES 				GetViewAngles;
	SETVIEWANGLES 				SetViewAngles;
	GETMAXCLIENTS 				GetMaxClients;
	CVAR_SETVALUE 				Cvar_SetValue;
	CMD_ARGC 					Cmd_Argc;
	CMD_ARGV 					Cmd_Argv;
	CON_PRINTF 					Con_Printf;
	CON_DPRINTF				Con_DPrintf;
	CON_NPRINTF				Con_NPrintf;
	CON_NXPRINTF 				Con_NXPrintf;
	PHYSINFO_VALUEFORKEY 		PhysInfo_ValueForKey;
	SERVERINFO_VALUEFORKEY 	ServerInfo_ValueForKey;
	GETCLIENTMAXSPEED 			GetClientMaxspeed;
	CHECKPARM 					CheckParm;
	KEY__EVENT 					Key_Event;
	GETMOUSEPOSITION 			GetMousePosition;
	ISNOCLIPPING 				IsNoClipping;
	GETLOCALPLAYER 			GetLocalPlayer;
	GETVIEWMODEL 				GetViewModel;
	GETENTITYBYINDEX 			GetEntityByIndex;
	GETCLIENTTIME 				GetClientTime;
	V_CALCSHAKE 				V_CalcShake;
	V_APPLYSHAKE 				V_ApplyShake;
	PM_POINTCONTENTS 			PM_PointContents;
	PM_WATERENTITY 			PM_WaterEntity;
	PM_TRACELINE 				PM_TraceLine;
	CL_LOADMODEL 				CL_LoadModel;
	CL_CREATEVISIBLEENTITY 		CL_CreateVisibleEntity;
	GETSPRITEPOINTER 			GetSpritePointer;
	PFNPLAYSOUNDBYNAMEATLOCATION pfnPlaySoundByNameAtLocation;
	PFNPRECACHEEVENT 			pfnPrecacheEvent;
	PFNPLAYBACKEVENT 			pfnPlaybackEvent;
	PFNWEAPONANIM 				pfnWeaponAnim;
	PFNRANDOMFLOAT 			pfnRandomFloat;
	PFNRANDOMLONG 			pfnRandomLong;
	PFNHOOKEVENT 				pfnHookEvent;
	CON_ISVISIBLE 				Con_IsVisible;
	PFNGETGAMEDIRECTORY 		pfnGetGameDirectory;
	PFNGETCVARPOINTER 			pfnGetCvarPointer;
	KEY_LOOKUPBINDING 			Key_LookupBinding;
	PFNGETLEVELNAME 			pfnGetLevelName;
	PFNGETSCREENFADE 			pfnGetScreenFade;
	PFNSETSCREENFADE 			pfnSetScreenFade;
	VGUI_GETPANEL 				VGui_GetPanel;
	VGUI_VIEWPORTPAINTBACKGROUND VGui_ViewportPaintBackground;
	COM_LOADFILE 				COM_LoadFile;
	COM_PARSEFILE 				COM_ParseFile;
	COM_FREEFILE 				COM_FreeFile;
	triangleapi_s*					pTriAPI;
	struct efx_api_s*				pEfxAPI;
	event_api_s*					pEventAPI;
	demo_api_s*				pDemoAPI;
	net_api_s*					pNetAPI;
	IVoiceTweak_s*			pVoiceTweak;
	ISSPECTATEONLY				IsSpectateOnly;
	LOADMAPSPRITE 				LoadMapSprite;
	COM_ADDAPPDIRECTORYTOSEARCHPATH COM_AddAppDirectoryToSearchPath;
	COM_EXPANDFILENAME 		COM_ExpandFilename;
	PLAYERINFO_VALUEFORKEY 	PlayerInfo_ValueForKey;
	PLAYERINFO_SETVALUEFORKEY PlayerInfo_SetValueForKey;
	GETPLAYERUNIQUEID 			GetPlayerUniqueID;
	GETTRACKERIDFORPLAYER 		GetTrackerIDForPlayer;
	GETPLAYERFORTRACKERID 		GetPlayerForTrackerID;
	PFNSERVERCMDUNRELIABLE	pfnServerCmdUnreliable;
} cl_enginefunc_t;