#include <windows.h>
#include <stdio.h>
#include <limits.h>

#include <vector>
#include <algorithm>
#include <iterator>

#include "hlsdk_mini.h"
#include "hl2sdk_mini.h"

#define TOOL_NAME "SaveWarpTool"

PIMAGE_NT_HEADERS __ImageNtHeader(PVOID Base) {
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)Base;
	if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE) return NULL;
	PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)((BYTE*)Base + pDosHeader->e_lfanew);
	if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE) return NULL;
	return pNtHeaders;
}

//

const char* szBuildTime = __DATE__ " " __TIME__;

//

typedef struct _map_info_t {
	int fromMapIndex;
	int landmarkIndex;
	int toMapIndex;
} map_info_t;

typedef struct _landmark_info_t {
	int mapIndex;
	int landmarkIndex;
	const char* szOrigin;
} landmark_info_t;

using namespace std;

vector<const char*> MAP;
vector<const char*> LANDMARK;

vector<map_info_t> maps_info;
vector<landmark_info_t> landmarks_info;

vector<vector<vector<int>>> maps_landmark;

vector<vector<int>> maps_d;
vector<vector<int>> maps_p;

//

int GetMapIndex(const char* map_name, bool forceAdd = false) {
	for (int i = 0; i < (int)MAP.size(); i++)
		if (strcmp(map_name, MAP[i]) == 0)
			return i;
	if (forceAdd) {
		MAP.push_back(_strdup(map_name));
		return MAP.size() - 1;
	}
	return -1;
}

int GetLandmarkIndex(const char* landmark_name) {
	for (int i = 0; i < (int)LANDMARK.size(); i++)
		if (strcmp(landmark_name, LANDMARK[i]) == 0)
			return i;
	LANDMARK.push_back(_strdup(landmark_name));
	return LANDMARK.size() - 1;
}

BOOL UtilsCompareData(const BYTE* pData, const BYTE* bMask, const char* pszMask) {
	for (; *pszMask; ++pszMask, ++pData, ++bMask)
		if (*pszMask == 'x' && *pData != *bMask)
			return FALSE;
	return (*pszMask) == 0;
}

PVOID UtilsFindPattern(PVOID dwAddress, SIZE_T dwLen, BYTE *bMask, const char* pszMask) {
	for (SIZE_T i = 0; i < dwLen; i++)
		if (UtilsCompareData((BYTE*)dwAddress + i, bMask, pszMask))
			return (BYTE*)dwAddress + i;
	return NULL;
}

vector<int> intersection(vector<int> &v1, vector<int> &v2) {
	vector<int> v3;
	sort(v1.begin(), v1.end());
	sort(v2.begin(), v2.end());
	set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(v3));
	return v3;
}

BYTE* memmem(BYTE* bData, SIZE_T dwDataLen, const BYTE *bPattern, SIZE_T dwPatternLen) {
	for (SIZE_T i = 0; i < dwDataLen - dwPatternLen; i++)
		if (memcmp(bData + i, bPattern, dwPatternLen) == 0)
			return bData + i;
	return NULL;
}

char* GetFullBlock(char* pData, SIZE_T dataSize, char* pMatchInsideBlock, int matchInsideBlockLen) {
	const SIZE_T MAX_BYTES_TO_CHECK = 1024;
	//
	SIZE_T changelevelInfoSize = dataSize - (pMatchInsideBlock - pData);
	int flag = 0;
	//	
	char* pInfoBlockStart = pMatchInsideBlock;
	for (int i = 0; i < MAX_BYTES_TO_CHECK; i++) {
		if (*pInfoBlockStart == '{') {
			flag++;
			break;
		}
		pInfoBlockStart--;
		if (pInfoBlockStart >= pData + dataSize) break;
	}
	//
	char* pInfoBlockEnd = pMatchInsideBlock + matchInsideBlockLen;
	for (int i = 0; i < MAX_BYTES_TO_CHECK; i++) {
		if (*pInfoBlockEnd == '}') {
			flag++;
			break;
		}
		pInfoBlockEnd++;
		if (pInfoBlockEnd >= pData + dataSize) break;
	}
	if (flag == 2) {
		SIZE_T infoBlockLen = pInfoBlockEnd - pInfoBlockStart + 1;
		char* pInfoBlock = (char*)malloc(infoBlockLen + 1);
		memcpy(pInfoBlock, pInfoBlockStart, infoBlockLen);
		pInfoBlock[infoBlockLen] = '\0';
		return pInfoBlock;
	}
	return NULL;
}

const char pattern_ClassnameInfoLandmarkl[] = "\"classname\" \"info_landmark\"";
const char pattern_ClassnameTriggerChangelevel[] = "\"classname\" \"trigger_changelevel\"";
const char pattern_TriggerLandmark[] = "\"landmark\" \"";
const char pattern_TriggerMap[] = "\"map\" \"";
const char pattern_TriggerOrigin[] = "\"origin\" \"";
const char pattern_TriggerTargetname[] = "\"targetname\" \"";

void SaveTriggerChangelevelInfo(PVOID argument, char* infoBlock) {
	char* map = (char*)argument;
	char* p1 = strstr(infoBlock, pattern_TriggerLandmark);
	char* p2 = strstr(infoBlock, pattern_TriggerMap);
	if (p1 && p2) {
		p1 += strlen(pattern_TriggerLandmark);
		p2 += strlen(pattern_TriggerMap);
		char* p3 = strchr(p1, '"');
		char* p4 = strchr(p2, '"');
		if (p3 && p4) {
			*p3 = '\0';
			*p4 = '\0';
			char* p5 = strstr(map, ".bsp");
			if (p5) *p5 = '\0';
			char * from = map;
			char * to = p2;
			char * landmark = p1;
			map_info_t map_info;
			map_info.fromMapIndex = GetMapIndex(from, true);
			map_info.toMapIndex = GetMapIndex(to, true);
			map_info.landmarkIndex = GetLandmarkIndex(landmark);
			maps_info.push_back(map_info);
		}
	}
}

void SaveLandmarkInfo(PVOID argument, char* infoBlock) {
	char* map = (char*)argument;
	char* p1 = strstr(infoBlock, pattern_TriggerOrigin);
	char* p2 = strstr(infoBlock, pattern_TriggerTargetname);
	if (p1 && p2) {
		p1 += strlen(pattern_TriggerOrigin);
		p2 += strlen(pattern_TriggerTargetname);
		char* p3 = strchr(p1, '"');
		char* p4 = strchr(p2, '"');
		if (p3 && p4) {
			*p3 = '\0';
			*p4 = '\0';
			char* p5 = strstr(map, ".bsp");
			if (p5) *p5 = '\0';
			char * from = map;
			char * landmark = p2;
			char * origin = p1;
			//
			landmark_info_t landmark_info;
			landmark_info.mapIndex = GetMapIndex(from, true);
			landmark_info.landmarkIndex = GetLandmarkIndex(landmark);
			landmark_info.szOrigin = _strdup(origin);
			landmarks_info.push_back(landmark_info);
		}
	}
}

typedef void(*fnBspParseBlock) (PVOID argument, char* infoBlock);

void ScanForPattern(char* pData, SIZE_T dataSize, const char* pattern, fnBspParseBlock callback, PVOID argument) {
	SIZE_T patternLen = strlen(pattern);
	char* pPointer = pData;
	char* pChangelevelInfo;
	do {
		SIZE_T bytesToCheck = dataSize - (pPointer - pData);
		pChangelevelInfo = (char*)memmem((BYTE*)pPointer, bytesToCheck, (BYTE*)pattern, patternLen);
		if (pChangelevelInfo) {
			char* pInfoBlock = GetFullBlock(pPointer, bytesToCheck, pChangelevelInfo, patternLen);
			if (pInfoBlock) {
				callback(argument, pInfoBlock);	
				pPointer = pChangelevelInfo + strlen(pInfoBlock);
				free(pInfoBlock);
			} else {
				pPointer = pChangelevelInfo + patternLen;
			}	
		}
	} while (pChangelevelInfo);
}

void ReadBspFile(const char* fileName, PVOID argument) {
	HANDLE hFile = CreateFileA(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return;
	}
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	HANDLE hMapping = CreateFileMappingA(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	if (hMapping == INVALID_HANDLE_VALUE) {
		CloseHandle(hFile);
		return;
	}
	char* pBspFile = (char*)MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, dwFileSize);
	if (pBspFile == NULL) {
		UnmapViewOfFile(hMapping);
		CloseHandle(hFile);
		return;
	}
	ScanForPattern(pBspFile, dwFileSize, pattern_ClassnameTriggerChangelevel, SaveTriggerChangelevelInfo, argument); // trigger_changelevel
	ScanForPattern(pBspFile, dwFileSize, pattern_ClassnameInfoLandmarkl, SaveLandmarkInfo, argument); // info_landmark
	UnmapViewOfFile(hMapping);
	CloseHandle(hFile);
}

char* ExtractMapName(const char* level) {
	if (!level) return NULL;
	const char* map = level;
	int map_len = strlen(level);
	const char* slash = strrchr(level, '/');
	if (slash) {
		const char* dot = strrchr(slash + 1, '.');
		map = slash + 1;
		map_len = dot - slash - 1;
	}
	char* out = (char*)malloc(map_len + 1);
	memcpy(out, map, map_len);
	out[map_len] = '\0';
	return out;
}

const char* GetLandmarkOrigin(int map, int landmark) {
	for (int i = 0; i < (int)landmarks_info.size(); i++) {
		if (landmarks_info[i].landmarkIndex == landmark && landmarks_info[i].mapIndex == map)
			return landmarks_info[i].szOrigin;
	}
	return "<unknown>";
}

typedef struct {
	int a, b, landmark;
} transition_ban_t;

vector<transition_ban_t> transition_bans;

bool IsBannedTransition(int a, int landmark, int b) {
	for (int j = 0; j < (int)transition_bans.size(); j++)
		if (transition_bans[j].a == a && 
			transition_bans[j].b == b && 
			transition_bans[j].landmark == landmark) 
			return true;
	return false;
}

vector<int> GetLandmarkConnection(int a, int b) {
	vector<int> result;
	for (int i = 0; i < (int)maps_landmark[a][b].size(); i++) {
		for (int j = 0; j < (int)maps_landmark[b][a].size(); j++) {
			if (maps_landmark[a][b][i] == maps_landmark[b][a][j]) {
				result.push_back(maps_landmark[a][b][i]);
			}
		}
	}
	return result;
}

vector<int> GetStrongLandmarkConnection(int a, int b) {
	vector<int> forwardConnection = GetLandmarkConnection(a, b);
	vector<int> backwardConnection = GetLandmarkConnection(b, a);
	return intersection(forwardConnection, backwardConnection);
}

int GetFirstStrongConnection(int a, int b) {
	vector<int> strongConnection = GetStrongLandmarkConnection(a, b);
	for (int i = 0; i < strongConnection.size(); i++) {
		if (!IsBannedTransition(a, strongConnection[i], b))
			return strongConnection[i];
	}
	return -1;
}

bool IsMapsStrongConnected(int a, int b) {
	return GetFirstStrongConnection(a, b) != -1;
}

void BuildWays() {
	int maps_count = (int)MAP.size();
	maps_landmark.resize(maps_count, vector<vector<int>>(maps_count));
	for (int i = 0; i < (int)maps_info.size(); i++)
		maps_landmark[maps_info[i].fromMapIndex][maps_info[i].toMapIndex].push_back(maps_info[i].landmarkIndex);
	//
	maps_d.resize(maps_count, vector<int>(maps_count, INT_MAX));
	maps_p.resize(maps_count, vector<int>(maps_count, -1));
	for (int i = 0; i < maps_count; i++)
		for (int j = 0; j < maps_count; j++) {
			if (i != j && IsMapsStrongConnected(i, j)) {
				maps_d[i][j] = 1;
				maps_p[i][j] = j;
			}
		}
	// Floyds
	for (int i = 0; i < maps_count; i++)
		for (int u = 0; u < maps_count; u++)
			for (int v = 0; v < maps_count; v++)
				if (u != v)
					if (maps_d[u][i] < INT_MAX && maps_d[i][v] < INT_MAX)
						if (maps_d[u][i] + maps_d[i][v] < maps_d[u][v]) {
							maps_d[u][v] = maps_d[u][i] + maps_d[i][v];
							maps_p[u][v] = maps_p[u][i];
						}
}

void ConsolePrintf(const char* format, ...);

bool EmulateSaveWarp(const char* currentMapName, const char* destinationMapName, const char** intermediateMap, const char** landmark) {
	if (!intermediateMap || !landmark) return false;
	int currentMapIndex = GetMapIndex(currentMapName);
	int destinationMapIndex = GetMapIndex(destinationMapName);
	if (currentMapIndex == -1 || destinationMapIndex == -1) return false;
	if (maps_d[currentMapIndex][destinationMapIndex] != INT_MAX) {
		int intermediateMapIndex = maps_p[currentMapIndex][destinationMapIndex];
		*intermediateMap = MAP[intermediateMapIndex];
		int lm = GetFirstStrongConnection(currentMapIndex, intermediateMapIndex);
		if (lm == -1) return false;
		*landmark = LANDMARK[GetFirstStrongConnection(currentMapIndex, intermediateMapIndex)];
		return true;
	}
	return false;
}

bool EmmulateWrongSaveWarp(const char* currentMapName, const char* startMapName, const char* finishMapName, const char** intermediateMap, const char** landmark, bool undo = false) {
	if (!intermediateMap || !landmark) return false;
	int currentMapIndex = GetMapIndex(currentMapName);
	int startMapIndex = GetMapIndex(startMapName);
	int finishMapIndex = GetMapIndex(finishMapName);
	if (currentMapIndex == -1 || startMapIndex == -1 || finishMapIndex == -1) return false;
	
		if (undo) {
			if (maps_d[currentMapIndex][finishMapIndex] == INT_MAX) return false;
			if (maps_d[currentMapIndex][finishMapIndex] >= 2) {
				int intermediateMapIndex = maps_p[currentMapIndex][finishMapIndex];
				*intermediateMap = MAP[intermediateMapIndex];
				//
				int nextIntermediateMapIndex = maps_p[intermediateMapIndex][finishMapIndex];
				int landmarkIndex = GetFirstStrongConnection(intermediateMapIndex, nextIntermediateMapIndex);
				*landmark = LANDMARK[landmarkIndex];
				return true;
			}
		} else {
			if (maps_d[startMapIndex][currentMapIndex] == INT_MAX) return false;
			if (maps_d[startMapIndex][currentMapIndex] >= 1) {
				int intermediateMapIndex = maps_p[currentMapIndex][startMapIndex];
				*intermediateMap = MAP[intermediateMapIndex];
				//
				int nextIntermediateMapIndex = maps_p[currentMapIndex][finishMapIndex];
				int landmarkIndex = GetFirstStrongConnection(currentMapIndex, nextIntermediateMapIndex);
				*landmark = LANDMARK[landmarkIndex];
				return true;
			}
		}
	
	return false;
}

int ReadMaps(const char* gameDirectory) {
	if (!gameDirectory) return -1;
	// Parse *.bsp
	char tmp_path[MAX_PATH];
	_sprintf_p(tmp_path, sizeof(tmp_path), "%s\\maps\\*.bsp", gameDirectory);
	WIN32_FIND_DATAA ffd;
	HANDLE hFind = FindFirstFileA(tmp_path, &ffd);
	if (hFind == INVALID_HANDLE_VALUE) return -1;
	do {
		_sprintf_p(tmp_path, sizeof(tmp_path), "%s\\maps\\%s", gameDirectory, ffd.cFileName);
		ReadBspFile(tmp_path, (PVOID)ffd.cFileName);
	} while (FindNextFileA(hFind, &ffd));
	// Make graph
	BuildWays();
	return 0;
}

//
// Forward
//

typedef void(*CommandHandler_t) (const char* const* argv, int arg);

void ConsolePrintf(const char* format, ...);
bool InPlayerReadyForChangelevel();
const char* GetCurrentMap();
void EngineCmd(char* cmd);
void RegisterCmd(const char* cmd, CommandHandler_t handler);
const char* GetGameDirectory();

//
// Commands
//

void Command_swarp(const char* const* argv, int argc) {
	if (argc < 2) {
		ConsolePrintf("Info: changelevel2 in <destination> direction.\n"
					  "Usage: %s <destination>\n", argv[0]);
		return;
	}
	const char* destinationMapName = argv[1];
	if (!InPlayerReadyForChangelevel()) {
		ConsolePrintf("Can't \"%s\", player not ready\n", argv[0]);
		return;
	}
	char* currentMapName = ExtractMapName(GetCurrentMap());
	if (!currentMapName) {
		ConsolePrintf("Can't get name of the current map\n");
		goto __free_return;
	}
	const char* intermediateMapName, *landmark;
	if (EmulateSaveWarp(currentMapName, destinationMapName, &intermediateMapName, &landmark)) {
		char tmp[256];
		_sprintf_p(tmp, sizeof(tmp), "changelevel2 \"%s\" \"%s\"", intermediateMapName, landmark);
		EngineCmd(tmp);
	} else {
		ConsolePrintf("Can't choose map for Save Warp!\n");
		goto __free_return;
	}
	//
__free_return:
	free(currentMapName);
}

void swarp_wrong(const char* const* argv, int argc, bool reverse = false) {
	if (argc < 3) {
		ConsolePrintf("Info: changelevel2 with landmark from wrong trigger_changelevel.\n"
					  "Usage: %s <start> <finish>\n", argv[0]);
		return;
	}
	const char* startMapName = argv[1];
	const char* finishMapName = argv[2];
	//
	if (!InPlayerReadyForChangelevel()) {
		ConsolePrintf("Can't \"%s\", player not ready\n", argv[0]);
		return;
	}
	char* currentMapName = ExtractMapName(GetCurrentMap());
	if (!currentMapName) {
		ConsolePrintf("Can't get name of the current map\n");
		goto __free_return;
	}
	const char* intermediateMapName, *landmark;
	if (EmmulateWrongSaveWarp(currentMapName, startMapName, finishMapName, &intermediateMapName, &landmark, reverse)) {
		char tmp[256];
		_sprintf_p(tmp, sizeof(tmp), "changelevel2 \"%s\" \"%s\"", intermediateMapName, landmark);
		EngineCmd(tmp);
	} else {
		ConsolePrintf("Can't choose map for Wrong Warp!\n");
		goto __free_return;
	}
	//
__free_return:
	free(currentMapName);
}

void Command_swarp_wrong(const char* const* argv, int argc) {
	swarp_wrong(argv, argc);
}

void Command_swarp_wrong_rev(const char* const* argv, int argc) {
	const bool reverse = true;
	swarp_wrong(argv, argc, reverse);
}

void Command_swarp_dump(const char* const* argv, int argc) {
	for (int i = 0; i < (int)maps_info.size(); i++) {
		char line[256];
		_sprintf_p(line, sizeof(line), "\"%s\",\"%s\",\"%s\",\"%s\"\n", 
				   MAP[maps_info[i].fromMapIndex], 
				   LANDMARK[maps_info[i].landmarkIndex], 
				   MAP[maps_info[i].toMapIndex], 
				   GetLandmarkOrigin(maps_info[i].fromMapIndex, maps_info[i].landmarkIndex));
		ConsolePrintf(line);
	}
}

//
// Common
//

typedef enum {
	E_INVALID = 0,
	E_GOLDSOURCE,
	E_SOURCE,
} engine_type_t;

engine_type_t EngineType;

//
// GoldSource
//

cl_enginefuncs_s* pEngFuncs;

//
// Source
//

union {
	IVEngineClient006* if006;
	IVEngineClient011* if011;
	IVEngineClient012* if012;
	IVEngineClient013* if013;
} pEngineClient;
int ifverEngineClient = -1;

union {
	IVEngineCvar001* if001;
	IVEngineCvar002* if002;
	IVEngineCvar003* if003;
	IVEngineCvar004* if004;
} pEngineCvar;
int ifverEngineCvar = -1;

union {
	IVClientEntityList002* if002;
	IVClientEntityList003* if003;
} pClientEntityList;
int ifverClientEntityList = -1;

FnMsg Msg;

PVOID pConCommandBase_vtable;

//
// Info Command
//

void Command_swarp_info(const char* const* argv, int argc) {
	ConsolePrintf("Save Warp Tool by shar (https://github.com/sharkov63/SaveWarpTool/)\n"
				  "  SWT BuildTime: %s\n"
				  "  Engine: %s\n"
				  "  EngineClient Interface Version: %03i\n"
				  "  EngineCvar Interface Version: %03i\n"
				  "  ClientEntityList Interface Version: %03i\n"
				  "  Game Dir: %s\n",
				  szBuildTime, EngineType == E_GOLDSOURCE ? "GoldSource" : "Source", ifverEngineClient, ifverEngineCvar, ifverClientEntityList, GetGameDirectory());
}

typedef struct {
	int x, y, z;
	int map;
} waypoint_t;

vector<waypoint_t> waypoints;

void Command_swarp_wp_add(const char* const* argv, int argc) {
	if (EngineType == E_GOLDSOURCE) {
		cl_entity_s* localplayer = pEngFuncs->GetLocalPlayer();
		if ((DWORD_PTR)localplayer >= 0x1000) {// GetLocalPlayer may return sizeof(cl_entity_s) on error
			waypoint_t wp;
			wp.map = GetMapIndex(ExtractMapName(GetCurrentMap()));
			if (wp.map != -1) {
				wp.x = (int)localplayer->origin.x;
				wp.y = (int)localplayer->origin.y;
				wp.z = (int)localplayer->origin.z;
				waypoints.push_back(wp);
			} else {
				ConsolePrintf("[SWT] Can't get name of the current map\n");
			}
		}
	} else if (EngineType == E_SOURCE) {
		waypoint_t wp;
		wp.map = GetMapIndex(ExtractMapName(GetCurrentMap()));
		if (wp.map != -1) {
			int index = -1;
			if (ifverEngineClient <= 10) index = pEngineClient.if006->GetPlayer(); // not working
			else if (ifverEngineClient <= 11) index = pEngineClient.if011->GetLocalPlayer();
			else if (ifverEngineClient <= 12) index = pEngineClient.if012->GetLocalPlayer();
			else index = pEngineClient.if013->GetLocalPlayer();
			if (index != -1) {
				auto localplayer = pClientEntityList.if003->GetClientEntity(index);
				Vector origin = localplayer->GetAbsOrigin();
				wp.x = (int)origin.x;
				wp.y = (int)origin.y;
				wp.z = (int)origin.z;
				waypoints.push_back(wp);
			} else {
				ConsolePrintf("[SWT] Can't get player\n");
			}
		} else {
			ConsolePrintf("[SWT] Can't get name of the current map\n");
		}
	}
}

void Command_swarp_wp_clear(const char* const* argv, int argc) {
	waypoints.clear();
	ConsolePrintf("[SWT] All waypoints removed!\n");
}

void Command_swarp_wp_dump(const char* const* argv, int argc) {
	for (int i = 0; i < (int)waypoints.size(); i++)
		ConsolePrintf("%s | %i %i %i\n", MAP[waypoints[i].map], waypoints[i].x, waypoints[i].y, waypoints[i].z);
}


void Command_swarp_swarp_ban(const char* const* argv, int argc) {
	if (argc < 3) {
		ConsolePrintf("swarp_swarp_ban <map_from> <landmark> <map_to>\n");
		return;
	}

	int a = GetMapIndex(argv[1]);
	int landmark = GetLandmarkIndex(argv[2]);
	int b = GetMapIndex(argv[3]);
	
	if (a == -1) {
		ConsolePrintf("Invalid map name \"%s\"\n", argv[1]);
		return;
	}
	if (b == -1) {
		ConsolePrintf("Invalid map name \"%s\"\n", argv[3]);
		return;
	}
	if (landmark == -1) {
		ConsolePrintf("Invalid landmark name \"%s\"\n", argv[2]);
		return;
	}

	transition_ban_t ban;
	ban.a = a;
	ban.b = b;
	ban.landmark = landmark;

	transition_bans.push_back(ban);
}

void Command_swarp_swarp_ban_clear(const char* const* argv, int argc) {
	transition_bans.clear();
}

void Command_swarp_rebuild_graph(const char* const* argv, int argc) {
	BuildWays();
}

void Command_swt_set_pos(const char* const* argv, int argc) {
	if (argc < 3) {
		ConsolePrintf("swt_set_pos <x> <y> <z>\n");
		return;
	}
	int x = atoi(argv[1]);
	int y = atoi(argv[2]);
	int z = atoi(argv[3]);

	if (EngineType == E_GOLDSOURCE) {
		cl_entity_s* localplayer = pEngFuncs->GetLocalPlayer();
		if ((DWORD_PTR)localplayer >= 0x1000) {// GetLocalPlayer may return sizeof(cl_entity_s) on error
			localplayer->origin.x = (float)x;
			localplayer->origin.y = (float)y;
			localplayer->origin.z = (float)z;
		}
	} else if (EngineType == E_SOURCE) {
		waypoint_t wp;
		wp.map = GetMapIndex(ExtractMapName(GetCurrentMap()));
		if (wp.map != -1) {
			int index = -1;
			if (ifverEngineClient <= 10) index = pEngineClient.if006->GetPlayer(); // not working
			else if (ifverEngineClient <= 11) index = pEngineClient.if011->GetLocalPlayer();
			else if (ifverEngineClient <= 12) index = pEngineClient.if012->GetLocalPlayer();
			else index = pEngineClient.if013->GetLocalPlayer();
			if (index != -1) {
				auto localplayer = pClientEntityList.if003->GetClientEntity(index);
				Vector* origin = (Vector*)&localplayer->GetAbsOrigin();
				origin->x = x;
				origin->y = y;
				origin->z = z;
			} else {
				ConsolePrintf("[SWT] Can't get player\n");
			}
		} else {
			ConsolePrintf("[SWT] Can't get name of the current map\n");
		}
	}
}


//
// Wrappers
//

void ConsolePrintf(const char* format, ...) {
	char line[1024];
	va_list valist;
	va_start(valist, format);
	vsnprintf_s(line, sizeof(line), (char*)format, valist);
	va_end(valist);
	if (EngineType == E_GOLDSOURCE) {
		pEngFuncs->pfnConsolePrint(line);
	} else if (EngineType == E_SOURCE) {
		Msg(line);
	} else MessageBoxA(NULL, line, TOOL_NAME, MB_OK | MB_ICONINFORMATION);
}

bool InPlayerReadyForChangelevel() {
	if (EngineType == E_GOLDSOURCE) {
		cl_entity_s* localplayer = pEngFuncs->GetLocalPlayer();
		return (DWORD_PTR)localplayer >= 0x1000 // GetLocalPlayer may return sizeof(cl_entity_s) on error
			&& (localplayer->origin.z != 0.0 || localplayer->origin.y != 0.0 || localplayer->origin.x != 0.0); // dirty way to detect unready player
	} else if (EngineType == E_SOURCE) {
		if (ifverEngineClient <= 10) return pEngineClient.if006->GetPlayer() != 0;
		else if (ifverEngineClient <= 11) return pEngineClient.if011->GetLocalPlayer() != -1;
		else if (ifverEngineClient <= 12) return pEngineClient.if012->GetLocalPlayer() != -1;
		else return pEngineClient.if013->GetLocalPlayer() != -1;
	}
	return false;
}

const char* GetGameDirectory() {
	if (EngineType == E_GOLDSOURCE) {
		return pEngFuncs->pfnGetGameDirectory();
	} else if (EngineType == E_SOURCE) {
		if (ifverEngineClient <= 10) return pEngineClient.if006->GetGameDirectory();
		else if (ifverEngineClient <= 11) return pEngineClient.if011->GetGameDirectory();
		else if (ifverEngineClient <= 12) return pEngineClient.if012->GetGameDirectory();
		else return pEngineClient.if013->GetGameDirectory();
	}
	return NULL;
}

const char* GetCurrentMap() {
	if (EngineType == E_GOLDSOURCE) {
		return pEngFuncs->pfnGetLevelName();
	} else if (EngineType == E_SOURCE) {
		if (ifverEngineClient <= 10) return pEngineClient.if006->GetLevelName();
		else if (ifverEngineClient <= 11) return pEngineClient.if011->GetLevelName();
		else if (ifverEngineClient <= 12) return pEngineClient.if012->GetLevelName();
		else return pEngineClient.if013->GetLevelName();
	}
	return NULL;
}

void EngineCmd(char* cmd) {
	if (EngineType == E_GOLDSOURCE) {
		pEngFuncs->pfnClientCmd(cmd);
	} else if (EngineType == E_SOURCE) {
		if (ifverEngineClient <= 10) pEngineClient.if006->ClientCmd(cmd);
		else if (ifverEngineClient <= 11) pEngineClient.if011->ClientCmd(cmd);
		else if (ifverEngineClient <= 12) pEngineClient.if012->ClientCmd(cmd);
		else pEngineClient.if013->ClientCmd(cmd);
	}
}

int EngineArgc() {
	if (EngineType == E_GOLDSOURCE) {
		return pEngFuncs->Cmd_Argc();
	} else if (EngineType == E_SOURCE) {
		if (ifverEngineClient <= 10) return pEngineClient.if006->Cmd_Argc();
		else if (ifverEngineClient <= 11) return pEngineClient.if011->Cmd_Argc();
		else if (ifverEngineClient <= 12) return pEngineClient.if012->Cmd_Argc();
		else return 0; // CCommand
	}
	return 0;
}

const char* EngineArgv(int idx) {
	if (EngineType == E_GOLDSOURCE) {
		return pEngFuncs->Cmd_Argv(idx);
	} else if (EngineType == E_SOURCE) {
		if (ifverEngineClient <= 10) return pEngineClient.if006->Cmd_Argv(idx);
		else if (ifverEngineClient <= 11) return pEngineClient.if011->Cmd_Argv(idx);
		else if (ifverEngineClient <= 12) return pEngineClient.if012->Cmd_Argv(idx);
		else return NULL; // CCommand
	}
	return NULL;
}

void Source_RegisterConCommand(ConCommandBase *pCmd) {
	if (ifverEngineCvar <= 1) pEngineCvar.if001->RegisterConCommandBase(pCmd);
	else if (ifverEngineCvar <= 2) pEngineCvar.if002->RegisterConCommandBase(pCmd);
	else if (ifverEngineCvar <= 3) pEngineCvar.if003->RegisterConCommandBase(pCmd);
	else pEngineCvar.if004->RegisterConCommand(pCmd);
}

ConCommandBase* Source_GetCommands() {
	if (ifverEngineCvar <= 1) return pEngineCvar.if001->GetCommands();
	else if (ifverEngineCvar <= 2) return pEngineCvar.if002->GetCommands();
	else if (ifverEngineCvar <= 3) return pEngineCvar.if003->GetCommands();
	else return pEngineCvar.if004->GetCommands();
	return NULL;
}

vector<pair<char*, CommandHandler_t>> Commands;

CommandHandler_t FindCommand(const char* cmdName) {
	for (int i = 0; i < (int)Commands.size(); i++) {
		if (strcmp(cmdName, Commands[i].first) == 0) {
			return Commands[i].second;
		}
	}
	return NULL;
}

void Cmd_Trampoline_GlobalArgs() {
	const char* cmdName = EngineArgv(0);
	CommandHandler_t handler = FindCommand(cmdName);
	if (handler) {
		int argc = EngineArgc();
		const char** argv = (const char**)malloc(sizeof(char*)*argc);
		argv[0] = cmdName;
		for (int j = 1; j < argc; j++)
			argv[j] = EngineArgv(j);
		handler(argv, argc);
		free(argv);
	}
}

void Cmd_Trampoline_LocalArgs(const CCommand& command) {
	const char* cmdName = command.m_ppArgv[0];
	CommandHandler_t handler = FindCommand(cmdName);
	if (handler) handler(command.m_ppArgv, command.m_nArgc);
}

void RegisterCmd(const char* cmdName, CommandHandler_t handler) {
	if (EngineType == E_GOLDSOURCE) {
		char* pszCmdName = _strdup(cmdName);
		Commands.push_back(pair<char*, CommandHandler_t>(pszCmdName, handler));
		pEngFuncs->pfnAddCommand(pszCmdName, Cmd_Trampoline_GlobalArgs);
	} else if (EngineType == E_SOURCE) {
		if (!pConCommandBase_vtable) return;
		char* pszCmdName = _strdup(cmdName);
		Commands.push_back(pair<char*, CommandHandler_t>(pszCmdName, handler));
		ConCommand* pCmd = (ConCommand*)malloc(sizeof(ConCommand));
		memset(pCmd, 0, sizeof(*pCmd));
		pCmd->vtable = pConCommandBase_vtable;
		pCmd->name = pszCmdName;
		pCmd->help_string = NULL;
		if (ifverEngineClient <= 12) {
			pCmd->m_fnCommandCallbackV1 = Cmd_Trampoline_GlobalArgs;
			pCmd->m_bUsingCommandCallbackInterface = true;	
		} else {
			pCmd->m_fnCommandCallback = Cmd_Trampoline_LocalArgs;
			pCmd->m_bUsingNewCommandCallback = true;			
		}
		Source_RegisterConCommand(pCmd);
	}
}

engine_type_t GetEngine() {
	if (GetModuleHandleA("hl2.exe")) return E_SOURCE;
	if (GetModuleHandleA("hl.exe")) return E_GOLDSOURCE;
	return E_GOLDSOURCE; // default
}

//
// Initialization
//

typedef struct {
	HMODULE hModule;
	char szModulePath[MAX_PATH];
	SIZE_T ModulePathLen;
} init_ctx_t ;

void WINAPI InitSWT(init_ctx_t* ctx) {
	Sleep(5000); // HLS OOE needs time to load client.dll
	//
	char* pos = ctx->szModulePath + ctx->ModulePathLen;
	while (pos >= ctx->szModulePath && *pos != '\\') --pos;
	pos[1] = '\0';
	//
	if (EngineType == E_SOURCE) {
		//
		HMODULE hTier0 = (HMODULE)GetModuleHandleA((LPCSTR)"tier0");
		if (!hTier0) {
			MessageBoxA(0, "ModuleNotFound: tier0", TOOL_NAME, MB_OK | MB_ICONERROR);
			goto __terminate;
		}
		Msg = (FnMsg)GetProcAddress(hTier0, "Msg");
		if (!Msg) {
			MessageBoxA(0, "FunctionNotFound: tier0.Msg", TOOL_NAME, MB_OK | MB_ICONERROR);
			goto __terminate;
		}
		HMODULE hClient = (HMODULE)GetModuleHandleA((LPCSTR)"client");
		if (!hClient) {
			MessageBoxA(0, "ModuleNotFound: client", TOOL_NAME, MB_OK | MB_ICONERROR);
			goto __terminate;
		}
		HMODULE hEngine = (HMODULE)GetModuleHandleA((LPCSTR)"engine");
		if (!hEngine) {
			MessageBoxA(0, "ModuleNotFound: engine", TOOL_NAME, MB_OK | MB_ICONERROR);
			goto __terminate;
		}
		PIMAGE_NT_HEADERS pNtHeader = __ImageNtHeader(hEngine);
		SIZE_T Size;
		if (!pNtHeader) goto __terminate;

		Size = pNtHeader->OptionalHeader.SizeOfImage;
		CreateInterfaceFn engine_CreateInterface = (CreateInterfaceFn)GetProcAddress(hEngine, "CreateInterface");
		// VEngineClient
		const int MAX_ENGINECLIENT_IF_VERSION = 20;
		for (int i = 1; i < MAX_ENGINECLIENT_IF_VERSION; i++) {
			char szInterfaceVersion[32];
			_sprintf_p(szInterfaceVersion, sizeof(szInterfaceVersion), "VEngineClient%03i", i);
			int retval;
			PVOID pInterface = engine_CreateInterface(szInterfaceVersion, &retval);
			if (pInterface) {
				if (i <= 10) pEngineClient.if006 = reinterpret_cast<IVEngineClient006*>(pInterface);
				else if (i <= 11) pEngineClient.if011 = reinterpret_cast<IVEngineClient011*>(pInterface);
				else if (i <= 12) pEngineClient.if012 = reinterpret_cast<IVEngineClient012*>(pInterface);
				else pEngineClient.if013 = reinterpret_cast<IVEngineClient013*>(pInterface);
				ifverEngineClient = i;
				break;
			}
		}
		if (ifverEngineClient == -1) {
			MessageBoxA(0, "InterfaceVersionNotSupported: VEngineClient", TOOL_NAME, MB_OK | MB_ICONERROR);
			goto __terminate;
		}
		//
		HMODULE hVstdlib = (HMODULE)GetModuleHandleA((LPCSTR)"vstdlib");
		CreateInterfaceFn vstdlib_CreateInterface = (CreateInterfaceFn)GetProcAddress(hVstdlib, "CreateInterface");
		// VEngineCvar
		const int MAX_ENGINECVAR_IF_VERSION = 10;
		for (int i = 1; i < MAX_ENGINECVAR_IF_VERSION; i++) {
			char szInterfaceVersion[32];
			_sprintf_p(szInterfaceVersion, sizeof(szInterfaceVersion), "VEngineCvar%03i", i);
			int retval; 
			PVOID pInterface = engine_CreateInterface(szInterfaceVersion, &retval);
			if (!pInterface) pInterface = vstdlib_CreateInterface(szInterfaceVersion, &retval);
			if (pInterface) {
				if (i <= 1) pEngineCvar.if001 = reinterpret_cast<IVEngineCvar001*>(pInterface);
				else if (i <= 2) pEngineCvar.if002 = reinterpret_cast<IVEngineCvar002*>(pInterface);
				else if (i <= 3) pEngineCvar.if003 = reinterpret_cast<IVEngineCvar003*>(pInterface);
				else pEngineCvar.if004 = reinterpret_cast<IVEngineCvar004*>(pInterface);
				ifverEngineCvar = i;
				break;
			}
		}
		if (ifverEngineCvar == -1) {
			MessageBoxA(0, "InterfaceVersionNotSupported: VEngineCvar", TOOL_NAME, MB_OK | MB_ICONERROR);
			goto __terminate;
		}
		//	
		ConCommandBase* pCmd = Source_GetCommands();
		while (pCmd) {
			// random cmd for stealing vtable
			if (strcmp(pCmd->name, "changelevel") == 0)
				break;
			pCmd = pCmd->next;
		}
		if (pCmd) pConCommandBase_vtable = pCmd->vtable;
		//
		CreateInterfaceFn client_CreateInterface = (CreateInterfaceFn)GetProcAddress(hClient, "CreateInterface");
		// VClientEntityList
		const int MAX_CLIENTENTITYLIST_IF_VERSION = 10;
		for (int i = 1; i < MAX_CLIENTENTITYLIST_IF_VERSION; i++) {
			char szInterfaceVersion[32];
			_sprintf_p(szInterfaceVersion, sizeof(szInterfaceVersion), "VClientEntityList%03i", i);
			int retval;
			PVOID pInterface = client_CreateInterface(szInterfaceVersion, &retval);
			if (pInterface) {
				if (i <= 2) pClientEntityList.if002 = reinterpret_cast<IVClientEntityList002*>(pInterface);
				else pClientEntityList.if003 = reinterpret_cast<IVClientEntityList003*>(pInterface);
				ifverClientEntityList = i;
				break;
			}
		}
		if (ifverClientEntityList == -1) {
			MessageBoxA(0, "InterfaceVersionNotSupported: VClientEntityList", TOOL_NAME, MB_OK | MB_ICONERROR);
			goto __terminate;
		}
	} else if (EngineType == E_GOLDSOURCE){
		// Detect Protocol and Main module
		PVOID Base;
		SIZE_T Size;

		PVOID HlBase = (PVOID)GetModuleHandleA(NULL);
		PVOID HwBase = (PVOID)GetModuleHandleA((LPCSTR)"hw");
		PVOID SwBase = (PVOID)GetModuleHandleA((LPCSTR)"sw");
		if (HwBase == NULL && SwBase == NULL) {
			// 47 protocol
			Base = HlBase;
		} else {
			// 48 protocol
			if (HwBase == 0) Base = SwBase; // Software
			else Base = HwBase; // OpenGL D3D
		}

		PIMAGE_NT_HEADERS pNtHeader = __ImageNtHeader(Base);
		if (!pNtHeader) goto __terminate;

		Size = pNtHeader->OptionalHeader.SizeOfImage;
		// Find EngineFunctionsTable
		PVOID enginefunctions = UtilsFindPattern(Base, Size, (BYTE*)"invalid sound %i\x0A", "xxxxxxxxxxxxxxxxx");
		if (!enginefunctions) {
			MessageBoxA(0, "PatternNotFound: pEngFuncs", TOOL_NAME, MB_OK | MB_ICONERROR);
			goto __terminate;
		}
		pEngFuncs = (cl_enginefuncs_s*)((BYTE*)enginefunctions + 24);

		// Force draw distance to 65535
		pEngFuncs->Cvar_SetValue("sv_zmax", 65535);
		cvar_t* cvar_sv_zmax = pEngFuncs->pfnGetCvarPointer("sv_zmax");
		if (cvar_sv_zmax) cvar_sv_zmax->name = "swarp_zmax";
	}
	//
	ReadMaps(GetGameDirectory());

	RegisterCmd("swarp", Command_swarp);
	RegisterCmd("swarp_wrong", Command_swarp_wrong);
	RegisterCmd("swarp_wrong_rev", Command_swarp_wrong_rev);
	RegisterCmd("swarp_dump", Command_swarp_dump);
	RegisterCmd("swarp_info", Command_swarp_info);
	RegisterCmd("swarp_wp_add", Command_swarp_wp_add);
	RegisterCmd("swarp_wp_clear", Command_swarp_wp_clear);
	RegisterCmd("swarp_wp_dump", Command_swarp_wp_dump);

	RegisterCmd("swarp_ban", Command_swarp_swarp_ban);
	RegisterCmd("swarp_ban_clear", Command_swarp_swarp_ban_clear);
	RegisterCmd("swarp_rebuild_graph", Command_swarp_rebuild_graph);

	RegisterCmd("swt_set_pos", Command_swt_set_pos);

	//
__terminate:
	free(ctx);
	ExitThread(0);
}

//
// *.ASI loader
//

const char szName_RIB_Main_8[] = "_RIB_Main@8";
signed int __stdcall RIB_Main_8(PVOID a1, PVOID a2) { // Source Engine 
	//MessageBoxA(0, __FUNCTION__, TOOL_NAME, MB_OK | MB_ICONINFORMATION);
	return 1;
}

const char szName_RIB_Main_20[] = "_RIB_Main@20";
signed int __stdcall RIB_Main_20(PVOID a1, PVOID a2, PVOID a3, PVOID a4, PVOID a5) { // GoldSource Engine 
	//MessageBoxA(0, __FUNCTION__, TOOL_NAME, MB_OK | MB_ICONINFORMATION);
	return 1;
}

const char szName_SWT_asi[] = "SWT.asi";

struct {
	IMAGE_EXPORT_DIRECTORY ExportDirectory;
	DWORD Functions[1];
	DWORD Names[1];
	WORD NameOrdinals[1];
} ASI_EXPORT;

DWORD GetRVA(PVOID p, PVOID ImageBase) {
	return (DWORD_PTR)p - (DWORD_PTR)ImageBase;
}

void FakeAsiExport(PVOID ImageBase) {
	if (EngineType == E_SOURCE) {
		ASI_EXPORT.NameOrdinals[0] = 0;
		ASI_EXPORT.Functions[0] = GetRVA(&RIB_Main_8, ImageBase);
		ASI_EXPORT.Names[0] = GetRVA((PVOID)szName_RIB_Main_8, ImageBase);
	} else if (EngineType == E_GOLDSOURCE) {
		ASI_EXPORT.NameOrdinals[0] = 0;
		ASI_EXPORT.Functions[0] = GetRVA(&RIB_Main_20, ImageBase);
		ASI_EXPORT.Names[0] = GetRVA((PVOID)szName_RIB_Main_20, ImageBase);
	}
	ASI_EXPORT.ExportDirectory.AddressOfNameOrdinals = GetRVA(&ASI_EXPORT.NameOrdinals, ImageBase);
	ASI_EXPORT.ExportDirectory.AddressOfFunctions = GetRVA(&ASI_EXPORT.Functions, ImageBase);
	ASI_EXPORT.ExportDirectory.AddressOfNames = GetRVA(&ASI_EXPORT.Names, ImageBase);
	ASI_EXPORT.ExportDirectory.NumberOfFunctions = sizeof(ASI_EXPORT.Functions) / sizeof(ASI_EXPORT.Functions[0]);
	ASI_EXPORT.ExportDirectory.NumberOfNames = sizeof(ASI_EXPORT.Names) / sizeof(ASI_EXPORT.Names[0]);
	ASI_EXPORT.ExportDirectory.Name = GetRVA((PVOID)szName_SWT_asi, ImageBase);
	ASI_EXPORT.ExportDirectory.Base = 1;
	//
	PIMAGE_NT_HEADERS pNTHeaders = __ImageNtHeader(ImageBase);
	PIMAGE_DATA_DIRECTORY pExportDir = &pNTHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
	DWORD oldProtect;
	VirtualProtect(pExportDir, sizeof(*pExportDir), PAGE_READWRITE, &oldProtect);
	pExportDir->VirtualAddress = GetRVA(&ASI_EXPORT.ExportDirectory, ImageBase);
	pExportDir->Size = sizeof(ASI_EXPORT);
	VirtualProtect(pExportDir, sizeof(*pExportDir), oldProtect, &oldProtect);
}

//
// Entry
//

int WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
    Sleep(6000); // give HL2 some time to load
	if (fdwReason == DLL_PROCESS_ATTACH) {
		EngineType = GetEngine();
        //MessageBoxA(0, "DLL_PROCESS_ATTACH", TOOL_NAME, MB_OK | MB_ICONINFORMATION);
		//MessageBoxA(0, (EngineType == E_SOURCE ? "SOURCE" : "GOLDSRC"), TOOL_NAME, MB_OK | MB_ICONINFORMATION);
		DisableThreadLibraryCalls(hinstDLL);
		
		init_ctx_t* ctx = (init_ctx_t*)malloc(sizeof(init_ctx_t));
		ctx->ModulePathLen = GetModuleFileNameA(hinstDLL, ctx->szModulePath, sizeof(ctx->szModulePath) - 1);
		if (ctx->ModulePathLen >= 4
			&& ctx->szModulePath[ctx->ModulePathLen - 4] == '.'
			&& ctx->szModulePath[ctx->ModulePathLen - 3] == 'a'
			&& ctx->szModulePath[ctx->ModulePathLen - 2] == 's'
			&& ctx->szModulePath[ctx->ModulePathLen - 1] == 'i')
			FakeAsiExport(hinstDLL);
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)InitSWT, ctx, 0, NULL);/**/
	} else if (fdwReason == DLL_PROCESS_DETACH) {
		//MessageBoxA(0, "DLL_PROCESS_DETACH", TOOL_NAME, MB_OK | MB_ICONINFORMATION);
		//
	}
	return TRUE;
}