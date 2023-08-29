#pragma once

#include "stdafx.h"
#include "PowerPC Assembly Functions.h"

#include <filesystem>
#include <map>
#include <array>
#include "pugi/pugixml.hpp"

//active codes
extern int MENU_TITLE_CHECK_LOCATION; // No longer used!
extern int DI_DRAW_INDEX;
extern int DEBUG_MODE_INDEX;
extern int DISPLAY_HITBOXES_INDEX;
extern int DISPLAY_COLLISION_INDEX;
extern int DISPLAY_LEDGEGRAB_INDEX;
extern int HUD_DISPLAY_INDEX;
extern int CHARACTER_SELECT_P1_INDEX;
extern int CHARACTER_SELECT_P2_INDEX;
extern int CHARACTER_SELECT_P3_INDEX;
extern int CHARACTER_SELECT_P4_INDEX;
extern int COSTUME_SELECT_P1_INDEX;
extern int COSTUME_SELECT_P2_INDEX;
extern int COSTUME_SELECT_P3_INDEX;
extern int COSTUME_SELECT_P4_INDEX;
extern int INFINITE_SHIELDS_P1_INDEX;
extern int INFINITE_SHIELDS_P2_INDEX;
extern int INFINITE_SHIELDS_P3_INDEX;
extern int INFINITE_SHIELDS_P4_INDEX;
extern int CAMERA_LOCK_INDEX;
extern int ENDLESS_FRIENDLIES_MODE_INDEX;
extern int ENDLESS_FRIENDLIES_STAGE_SELECTION_INDEX;
extern int RANDOM_1_TO_1_INDEX;
extern int AUTO_SAVE_REPLAY_INDEX;
extern int SAVE_STATES_INDEX;
extern int SAVE_REPLAY_ANYWHERE_INDEX;
extern int AUTO_SKIP_TO_CSS_INDEX;
extern int CODE_MENU_ACTIVATION_SETTING_INDEX;
extern int PERCENT_SELECT_VALUE_P1_INDEX;
extern int PERCENT_SELECT_ACTIVATOR_P1_INDEX;
extern int PERCENT_SELECT_VALUE_P2_INDEX;
extern int PERCENT_SELECT_ACTIVATOR_P2_INDEX;
extern int PERCENT_SELECT_VALUE_P3_INDEX;
extern int PERCENT_SELECT_ACTIVATOR_P3_INDEX;
extern int PERCENT_SELECT_VALUE_P4_INDEX;
extern int PERCENT_SELECT_ACTIVATOR_P4_INDEX;
extern int DISABLE_DPAD_P1_INDEX;
extern int DISABLE_DPAD_P2_INDEX;
extern int DISABLE_DPAD_P3_INDEX;
extern int DISABLE_DPAD_P4_INDEX;
extern int FPS_DISPLAY_INDEX;
extern int DBZ_MODE_INDEX;
extern int DBZ_MODE_MAX_SPEED_X_INDEX;
extern int DBZ_MODE_ACCEL_X_INDEX;
extern int DBZ_MODE_MAX_SPEED_Y_INDEX;
extern int DBZ_MODE_ACCEL_Y_INDEX;
extern int ALT_STAGE_BEHAVIOR_INDEX;
extern int P1_TAG_STRING_INDEX;
extern int P2_TAG_STRING_INDEX;
extern int P3_TAG_STRING_INDEX;
extern int P4_TAG_STRING_INDEX;
extern int TAG_COSTUME_TOGGLE_INDEX;
extern int CROWD_CHEER_TOGGLE_INDEX;
extern int ALC_P1_INDEX;
extern int ALC_P2_INDEX;
extern int ALC_P3_INDEX;
extern int ALC_P4_INDEX;
extern int BIG_HEAD_INDEX;
extern int RANDOM_ANGLE_INDEX;
extern int WAR_MODE_INDEX;
extern int BUFFER_P1_INDEX;
extern int BUFFER_P2_INDEX;
extern int BUFFER_P3_INDEX;
extern int BUFFER_P4_INDEX;
extern int SCALE_INDEX;
extern int SPEED_INDEX;
extern int CSS_VERSION_SETTING_INDEX;
extern int THEME_SETTING_INDEX;
extern int DASH_ATTACK_ITEM_GRAB_INDEX;
extern int TRIP_TOGGLE_INDEX;
extern int TRIP_RATE_MULTIPLIER_INDEX;
extern int TRIP_INTERVAL_INDEX;
extern int BACKPLATE_COLOR_1_INDEX;
extern int BACKPLATE_COLOR_2_INDEX;
extern int BACKPLATE_COLOR_3_INDEX;
extern int BACKPLATE_COLOR_4_INDEX;
extern int BACKPLATE_COLOR_C_INDEX;
extern int BACKPLATE_COLOR_T_INDEX;


struct ConstantPair {
	int address;
	int* index;

	ConstantPair(int address, int &index) : address(address), index(&index) {}
};

//constant overrides
extern vector<ConstantPair> constantOverrides;
extern int SDI_DISTANCE_INDEX;
extern int HITSTUN_MULTIPLIER_INDEX;
extern int HITLAG_MAXIMUM_INDEX;
extern int HITLAG_MULTIPLIER_INDEX;
extern int ELECTRIC_HITLAG_MULTIPLIER_INDEX;
extern int ASDI_DISTANCE_INDEX;
extern int WALLJUMP_HORIZONTAL_MULTIPLIER_INDEX;
extern int MINIMUM_SHIELD_SIZE_SCALING_INDEX;
extern int SHIELD_DAMAGE_MULTIPLIER_INDEX;
extern int SHIELD_BASE_DAMAGE_INDEX;
extern int SHIELD_SIZE_MULTIPLIER_INDEX;
extern int SHIELD_TILT_MULTIPLIER_INDEX;
extern int KNOCKBACK_DECAY_MULTIPLIER_INDEX;
extern int WALL_BOUNCE_KNOCKBACK_MULTIPLIER_INDEX;
extern int STALING_TOGGLE_INDEX;

extern vector<int> Defaults;

#define MAX_SUBPAGE_DEPTH 20

//dpad same, + is start, 2 is A, 1 is B, A is Y, B is X, - is Z
const vector<u8> CODE_MENU_WIIMOTE_CONVERSION_TABLE = { 2, 3, 1, 0, 12, 31, 31, 31, 8, 9, 10, 11, 4, 31, 31, 31 };
//dpad same, + and - are start, A and B are A and B, C is Y, Z is start, 1 is X, 2 is Z
//dpad same, + is start, A and B are A and B, C is Y, Z is Z, - is X
const vector<u8> CODE_MENU_WIICHUCK_CONVERSION_TABLE = { 0, 1, 2, 3, 12, 31, 31, 31, 31, 31, 9, 8, 10, 4, 11, 31 };
const vector<u8> CODE_MENU_CLASSIC_CONVERSION_TABLE = { 3, 0, 4, 10, 8, 11, 9, 4, 31, 5, 12, 31, 12, 6, 2, 1 }; //1 to 1

// Enumeration of all predefined characters' Slot IDs.
enum LAVA_CHARA_SLOT_IDS
{
	LCSI_BOWSER = 0x0C,
	LCSI_CAPTAIN_FALCON = 0x0A,
	LCSI_CHARIZARD = 0x1E,
	LCSI_DEDEDE = 0x23,
	LCSI_DIDDY_KONG = 0x1C,
	LCSI_DONKEY_KONG = 0x01,
	LCSI_FALCO = 0x15,
	LCSI_FOX = 0x07,
	LCSI_GANONDORF = 0x16,
	LCSI_GIGA_BOWSER = 0x2C,
	LCSI_ICE_CLIMBERS = 0x10,
	LCSI_IKE = 0x25,
	LCSI_IVYSAUR = 0x22,
	LCSI_JIGGLYPUFF = 0x27,
	LCSI_KIRBY = 0x06,
	LCSI_LINK = 0x02,
	LCSI_LUCARIO = 0x24,
	LCSI_LUCAS = 0x1B,
	LCSI_LUIGI = 0x09,
	LCSI_MARIO = 0x00,
	LCSI_MARTH = 0x13,
	LCSI_META_KNIGHT = 0x18,
	LCSI_MR_GAME_AND_WATCH = 0x14,
	LCSI_NESS = 0x0B,
	LCSI_OLIMAR = 0x1A,
	LCSI_PEACH = 0x0D,
	LCSI_PIKACHU = 0x08,
	LCSI_PIT = 0x19,
	LCSI_POKETRAINER = 0x48,
	LCSI_POKETRAINER_CHARIZARD = 0x1D,
	LCSI_POKETRAINER_IVYSAUR = 0x21,
	LCSI_POKETRAINER_SQUIRTLE = 0x1F,
	LCSI_ROB = 0x26,
	LCSI_SAMUS = 0x03,
	LCSI_SHEIK = 0x0F,
	LCSI_SNAKE = 0x2A,
	LCSI_SONIC = 0x2B,
	LCSI_SOPO = 0x11,
	LCSI_SQUIRTLE = 0x20,
	LCSI_TOON_LINK = 0x28,
	LCSI_WARIO = 0x17,
	LCSI_WARIOMAN = 0x2D,
	LCSI_WOLF = 0x29,
	LCSI_YOSHI = 0x05,
	LCSI_ZELDA = 0x0E,
	LCSI_ZERO_SUIT_SAMUS = 0x04,
	LCSI_ROY = 0x32,
	LCSI_MEWTWO = 0x33,
	LCSI_KNUCKLES = 0x35,
	LCSI_RIDLEY = 0x38,
	LCSI_WALUIGI = 0x39,
	LCSI_DARK_SAMUS = 0x40,
	LCSI_SCEPTILE = 0x62,
	LCSI_ALLOY_RED = 0x6A,
	LCSI_ALLOY_BLUE = 0x6B,
	LCSI_ALLOY_YELLOW = 0x6C,
	LCSI_ALLOY_GREEN = 0x6D,
};

template <typename T1, typename T2>
bool zipVectorsToMap(const std::vector<T1>& vec1, const std::vector<T2>& vec2, std::map<T1, T2>& destinationMap)
{
	bool result = 0;

	if (vec1.size() == vec2.size())
	{
		result = 1;
		destinationMap.clear();
		for (std::size_t i = 0; i < vec1.size(); i++)
		{
			auto res = destinationMap.emplace(vec1[i], vec2[i]);
			if (res.second == 1)
			{
				res.first->second = vec2[i];
			}
		}
	}

	return result;
}
template <typename T1, typename T2>
void unzipMapToVectors(const std::map<T1, T2>& sourceMap, std::vector<T1>& vec1, std::vector<T2>& vec2)
{
	vec1.resize(sourceMap.size());
	vec2.resize(sourceMap.size());
	std::size_t index = 0;
	for (auto itr = sourceMap.begin(); itr != sourceMap.end(); itr++)
	{
		vec1[index] = itr->first;
		vec2[index] = itr->second;
		index++;
	}
}

// Declares existence of the two main character lists, populated by the function below.
// If COLLECT_EXTERNAL_EX_CHARACTERS (in "PowerPC Assembly Functions.h") is set to true,
// additional EX Character declarations will be collected from the file described by exCharInputFilename (see "Code Menu.cpp").
extern vector<string> CHARACTER_LIST;
extern vector<u16> CHARACTER_ID_LIST;
constexpr unsigned char CHARACTER_ID_MAX_VALUE = 0x7F;
void buildCharacterIDLists();
bool applyCharacterListVersion(unsigned long targetVersion);

// Declares existence of the two main roster lists, populated by the function below.
// If COLLECT_EXTERNAL_ROSTERS (in "PowerPC Assembly Functions.h") is set to true,
// additional Roster declarations will be collected from the file described by rosterInputFilename (see "Code Menu.cpp").
extern vector<string> ROSTER_LIST;
extern vector<string> ROSTER_FILENAME_LIST;
void buildRosterLists();

// Declares existence of two main theme lists, populated by the function below.
// If COLLECT_EXTERNAL_THEMES (in "PowerPC Assembly Functions.h") is set to true,
// additional Theme declarations will be collected from the file described by themeInputFilename (see "Code Menu.cpp").
namespace themeConstants
{
	enum themePathIndices
	{
		tpi_MENUMAIN = 0,
		tpi_SELCHAR,
		tpi_SELCHAR2,
		tpi_SELMAP,
		tpi_SELEVENT,
		tpi_TITLE,
		tpi__PATH_COUNT
	};

	constexpr unsigned long prefixLength = 0x03;
	
	extern std::array<std::string, tpi__PATH_COUNT> filenames;
}
struct menuTheme
{
	std::string name = "";
	std::array<std::string, themeConstants::tpi__PATH_COUNT> prefixes{};
	menuTheme(std::string name = "");
};
void initializeThemeConstants();
void buildThemeLists();
std::string getThemeFileBaseName(themeConstants::themePathIndices fileIndex);
std::string getThemeFileDefaultPrefix(themeConstants::themePathIndices fileIndex);
extern vector<string> THEME_LIST;
extern std::vector<menuTheme> THEME_SPEC_LIST;
// Notes, for each theme-able file, whether or not any specified themes actually use a non-standard prefix for it.
// Used to determine whether or not we actually need to output the hook for a given theme-able file.
extern std::array<bool, themeConstants::tpi__PATH_COUNT> THEME_FILE_GOT_UNIQUE_PREFIX;

namespace backplateColorConstants
{
	enum playerSlotColorLevel
	{
		pSCL_NONE = 0,
		pSCL_SHIELDS_AND_PLUMES_ONLY,
		pSCL_SHIELDS_PLUMES_AND_IN_GAME_HUD,
		pSCL_MENUS_AND_IN_GAME_WITHOUT_CSS_INPUT,
		pSCL_MENUS_AND_IN_GAME_WITH_CSS_INPUT,
		pSCL__COUNT
	};
	extern const std::array<std::string, playerSlotColorLevel::pSCL__COUNT> modeNames;
}
// Denotes the total number colors available to the HUD Color Switcher.
// Used to ensure that if we add a mechanism for adding additional colors, they'll be accounted for, both
// in the actual generated ASM in _BackplateColors, and by the actual code menu lines themselves.
extern const unsigned long BACKPLATE_COLOR_TOTAL_COLOR_COUNT;

// Incoming Configuration XML Variables (See "Code Menu.cpp" for defaults, and "_AdditionalCode.cpp" for relevant Config Parsing code!)
extern std::vector<std::string> CONFIG_INCOMING_COMMENTS;
extern bool CONFIG_DELETE_CONTROLS_COMMENTS;
extern unsigned char CONFIG_BACKPLATE_COLOR_MODE;
extern bool CONFIG_DASH_ATTACK_ITEM_GRAB_ENABLED;



// The stream for the MenuFile.
// Path is no longer specified in this line, is instead controlled by the below paths and applied in initMenuFileStream().
static fstream MenuFile;
void initMenuFileStream();


// Logging and Input Constants
extern const std::string outputFolder;
extern const std::string menuConfigXMLFileName;
extern const std::string netMenuConfigXMLFileName;
extern const std::string symbolMapInputFileName;
extern const std::string changelogFileName;
extern const std::string optionsFilename;
// Code Menu Output Constants
extern const std::string asmFileName;
extern const std::string asmTextFileName;
extern const std::string cmnuFileName;
extern const std::string asmOutputFilePath;
extern const std::string cmnuOutputFilePath;
extern const std::string cmnuOptionsOutputFilePath;
extern const std::string asmTextOutputFilePath;
extern const std::string asmBuildLocationDirectory;
extern const std::string cmnuBuildLocationDirectory;
extern const std::string asmBuildLocationFilePath;
extern const std::string cmnuBuildLocationFilePath;
std::string getCMNUAbsolutePath();
// AutoGCTRM Constants
extern const std::string buildFolder;
extern const std::string GCTRMExePath;
extern const std::string GCTRMCommandBase;
extern const std::string mainGCTName;
extern const std::string mainGCTFile;
extern const std::string mainGCTTextFile;
extern const std::string boostGCTName;
extern const std::string boostGCTFile;
extern const std::string boostGCTTextFile;

// Options File Functions
namespace xmlTagConstants
{
	extern const std::string codeMenuTag;
	extern const std::string nameTag;
	extern const std::string indexTag;
	extern const std::string valueTag;
	extern const std::string valueMinTag;
	extern const std::string valueMaxTag;
	extern const std::string valueDefaultTag;
	extern const std::string editableTag;
	extern const std::string cmnuPathTag;
	extern const std::string pageTag;
	extern const std::string selectionTag;
	extern const std::string selectionDefaultTag;
	extern const std::string selectionOptionTag;
	extern const std::string intTag;
	extern const std::string floatTag;
}
class Page; // Page Class Forward Decl.

extern pugi::xml_document menuOptionsTree;

void recursivelyFindPages(Page& currBasePageIn, std::vector<Page*>& collectedPointers);
void findPagesInOptionsTree(const pugi::xml_document& optionsTree, std::map<std::string, pugi::xml_node>& collectedNodes);
void findLinesInPageNode(const pugi::xml_node& pageNode, std::map<std::string, pugi::xml_node>& collectedNodes);
bool buildMenuOptionsTreeFromMenu(Page& mainPageIn, std::string xmlPathOut);

void applyDefaultValuesFromMenuOptionsTree(Page& mainPageIn, const pugi::xml_document& xmlDocumentIn);
bool applyDefaultValuesFromMenuOptionsTree(Page& mainPageIn, std::string xmlPathIn);

bool loadMenuOptionsTree(std::string xmlPathIn, pugi::xml_document& destinationDocument);

std::vector<const char*> splitLineContentString(const std::string& joinedStringIn);



static const int START_OF_CODE_MENU_HEADER = 0x804E0000;
static const int CURRENT_PAGE_PTR_LOC = START_OF_CODE_MENU_HEADER; //4
static const int MAIN_PAGE_PTR_LOC = CURRENT_PAGE_PTR_LOC + 4; //4
static const int SALTY_RUNBACK_BUTTON_COMBO_LOC = MAIN_PAGE_PTR_LOC + 4; //4
static const int SKIP_RESULTS_BUTTON_COMBO_LOC = SALTY_RUNBACK_BUTTON_COMBO_LOC + 4; //4
//colors
static const int COLOR_ARRAY_START = SKIP_RESULTS_BUTTON_COMBO_LOC + 4; //4 * num colors
static const u8 NORMAL_LINE_COLOR_OFFSET = 0;
static const u8 HIGHLIGHTED_LINE_COLOR_OFFSET = NORMAL_LINE_COLOR_OFFSET + 4;
static const u8 CHANGED_LINE_COLOR_OFFSET = HIGHLIGHTED_LINE_COLOR_OFFSET + 4;
static const u8 CHANGED_AND_HIGHLIGHTED_LINE_COLOR_OFFSET = CHANGED_LINE_COLOR_OFFSET + 4;
static const u8 COMMENT_LINE_COLOR_OFFSET = CHANGED_AND_HIGHLIGHTED_LINE_COLOR_OFFSET + 4;

static const int MOVE_FRAME_TIMER_LOC = COLOR_ARRAY_START + 0x14; //4
static const int INCREMENT_FRAME_TIMER_LOC = MOVE_FRAME_TIMER_LOC + 4; //4
static const int FRAME_ADVANCE_FRAME_TIMER = INCREMENT_FRAME_TIMER_LOC + 4; //4

static const int PREV_CODE_MENU_CONTROL_FLAG = FRAME_ADVANCE_FRAME_TIMER + 4; //4
static const int CODE_MENU_CONTROL_FLAG = PREV_CODE_MENU_CONTROL_FLAG + 4; //4
static const int INFINITE_FRIENDLIES_FLAG_LOC = CODE_MENU_CONTROL_FLAG + 4; //4
static const int AUTO_SAVE_REPLAY_FLAG_LOC = INFINITE_FRIENDLIES_FLAG_LOC + 4; //4
static const int ON_GROUP_RECORDS_FLAG_LOC = AUTO_SAVE_REPLAY_FLAG_LOC + 4; //4

static const int CODE_MENU_BUTTON_MASK_LOC = ON_GROUP_RECORDS_FLAG_LOC + 4; //4
static const int BUTTON_ACTIVATOR_MASK_LOC = CODE_MENU_BUTTON_MASK_LOC + 4; //4
static const int MAIN_BUTTON_MASK_LOC = BUTTON_ACTIVATOR_MASK_LOC + 4; //4 * 8

static const int OLD_DEBUG_STATE_LOC = MAIN_BUTTON_MASK_LOC + 4 * 8; //4
static const int OLD_CAMERA_LOCK_STATE_LOC = OLD_DEBUG_STATE_LOC + 4; //4

static const int OLD_CAMERA_POS_LOC = OLD_CAMERA_LOCK_STATE_LOC + 4; //4

static const int SAVE_STATE_BUFFER_PTR_LOC = OLD_CAMERA_POS_LOC + 4; //4
static const int SAVE_STATE_ARTICLE_LIST_PTR_LOC = SAVE_STATE_BUFFER_PTR_LOC + 4; //4
static const int SAVE_STATE_ARTICLE_ID_LIST_PTR_LOC = SAVE_STATE_ARTICLE_LIST_PTR_LOC + 4; //4
static const int SAVE_STATE_ARTICLE_SAVED_RESOURCE_LIST_PTR_LOC = SAVE_STATE_ARTICLE_ID_LIST_PTR_LOC + 4; //4
static const int SAVE_STATE_LOCATIONS_TO_UPDATE_PTR_LOC = SAVE_STATE_ARTICLE_SAVED_RESOURCE_LIST_PTR_LOC + 4; //4
static const int SAVE_STATE_LOCATIONS_TO_CLEAR_PTR_LOC = SAVE_STATE_LOCATIONS_TO_UPDATE_PTR_LOC + 4; //4
static const int SAVE_STATE_SAVED_ARTICLE_LIST_PTR_LOC = SAVE_STATE_LOCATIONS_TO_CLEAR_PTR_LOC + 4; //4

static const int RESET_LINES_STACK_LOC = SAVE_STATE_SAVED_ARTICLE_LIST_PTR_LOC + 4; // 4 * MAX_SUBPAGE_DEPTH + 8

static const int CHARACTER_SWITCHER_ARRAY_LOC = RESET_LINES_STACK_LOC + 4 * MAX_SUBPAGE_DEPTH + 8; //0x10
static const int INIFINITE_SHIELDS_ARRAY_LOC = CHARACTER_SWITCHER_ARRAY_LOC + 0x10; //0x10
static const int PERCENT_SELCTION_VALUE_ARRAY_LOC = INIFINITE_SHIELDS_ARRAY_LOC + 0x10; //0x10
static const int PERCENT_SELCTION_ACTIVATOR_ARRAY_LOC = PERCENT_SELCTION_VALUE_ARRAY_LOC + 0x10; //0x10
static const int DISABLE_DPAD_ACTIVATOR_ARRAY_LOC = PERCENT_SELCTION_ACTIVATOR_ARRAY_LOC + 0x10; //0x10

static const int ENDLESS_ROTATION_QUEUE_LOC = DISABLE_DPAD_ACTIVATOR_ARRAY_LOC + 0x10; //8
static const int ENDLESS_ROTATION_PLACEMENT_LIST_LOC = ENDLESS_ROTATION_QUEUE_LOC + 8; //4 * 4
static const int ENDLESS_ROTATION_COMP_FUNC_LOC = ENDLESS_ROTATION_PLACEMENT_LIST_LOC + 4 * 4; //4 * 4

static const int REPLAY_NTE_DATA_BUFFER_LOC = ENDLESS_ROTATION_COMP_FUNC_LOC + 4 * 4; //0x14
static const int REPLAY_CREATE_SECTION_BUFFER_LOC = REPLAY_NTE_DATA_BUFFER_LOC + 0x14; //8
static const int REPLAY_CRYPTO_BUFFER_LOC = REPLAY_CREATE_SECTION_BUFFER_LOC + 8; //0x30

static const int CODE_MENU_WIIMOTE_CONVERSION_TABLE_LOC = REPLAY_CRYPTO_BUFFER_LOC + 0x30; //0x10 * 3

static const int P1_TAG_HEX_LOC = CODE_MENU_WIIMOTE_CONVERSION_TABLE_LOC + 0x10 * 3; //0x18
static const int P2_TAG_HEX_LOC = P1_TAG_HEX_LOC + 0x18; //0x18
static const int P3_TAG_HEX_LOC = P2_TAG_HEX_LOC + 0x18; //0x18
static const int P4_TAG_HEX_LOC = P3_TAG_HEX_LOC + 0x18; //0x18

static const int P1_STOP_LOAD_FLAG_PTR_LOC = P4_TAG_HEX_LOC + 0x18; //4
static const int P2_STOP_LOAD_FLAG_PTR_LOC = P1_STOP_LOAD_FLAG_PTR_LOC + 4; //4
static const int P3_STOP_LOAD_FLAG_PTR_LOC = P2_STOP_LOAD_FLAG_PTR_LOC + 4; //4
static const int P4_STOP_LOAD_FLAG_PTR_LOC = P3_STOP_LOAD_FLAG_PTR_LOC + 4; //4

static const int RANDOM_ALTS_RNG = P4_STOP_LOAD_FLAG_PTR_LOC + 4; //4
static const int RANDOM_ALTS_MATCH_START_FLAG = RANDOM_ALTS_RNG + 4; //4

static const int TEAM_SETTINGS_LOC = RANDOM_ALTS_MATCH_START_FLAG + 4; //4
static const int TAG_LOAD_FLAGS_LOC = TEAM_SETTINGS_LOC + 4; //4

static const int PREV_TAG_COSTUMES_SETTING_LOC = TAG_LOAD_FLAGS_LOC + 4; //4

static const int DOLPHIN_MOUNT_VF_LOC = PREV_TAG_COSTUMES_SETTING_LOC + 4; //4

static const int CODE_MENU_OLD_CAMERA_MATRIX_LOC = DOLPHIN_MOUNT_VF_LOC + 4; //4 * 12 = 0x30
static const int CODE_MENU_NEED_TO_SAVE_CAMERA_MATRIX_FLAG_LOC = CODE_MENU_OLD_CAMERA_MATRIX_LOC + 0x30; //4

static const int SHOULD_DISPLAY_HUD_FLAG_LOC = CODE_MENU_NEED_TO_SAVE_CAMERA_MATRIX_FLAG_LOC + 4; //4

static const int SHOULD_RESET_HITBOX_DISPLAY_FLAG_LOC = SHOULD_DISPLAY_HUD_FLAG_LOC + 4; //4
static const int SHOULD_RESET_STAGE_COLLISIONS_FLAG_LOC = SHOULD_RESET_HITBOX_DISPLAY_FLAG_LOC + 4; //4

static const int ALC_P1_LOC = SHOULD_RESET_STAGE_COLLISIONS_FLAG_LOC + 4; //4
static const int ALC_P2_LOC = ALC_P1_LOC + 4; //4
static const int ALC_P3_LOC = ALC_P2_LOC + 4; //4
static const int ALC_P4_LOC = ALC_P3_LOC + 4; //4

static const int BIG_HEAD_LOC = ALC_P4_LOC + 4; //4

static const int RANDOM_ANGLE_LOC = BIG_HEAD_LOC + 4; //4

static const int WAR_MODE_LOC = RANDOM_ANGLE_LOC + 4; //4

static const int BUFFER_P1_LOC = WAR_MODE_LOC + 4; //4
static const int BUFFER_P2_LOC = BUFFER_P1_LOC + 4; //4
static const int BUFFER_P3_LOC = BUFFER_P2_LOC + 4; //4
static const int BUFFER_P4_LOC = BUFFER_P3_LOC + 4; //4

static const int SCALE_LOC = BUFFER_P4_LOC + 4; //4

static const int SPEED_LOC = SCALE_LOC + 4; //4

static const int CSS_VER_LOC = SPEED_LOC + 4; //4

static const int THEME_LOC = CSS_VER_LOC + 4; //4

static const int DASH_ATTACK_ITEM_GRAB_LOC = THEME_LOC + 4; //4

static const int TRIP_TOGGLE_LOC = DASH_ATTACK_ITEM_GRAB_LOC + 4; //4
static const int TRIP_RATE_MULTIPLIER_LOC = TRIP_TOGGLE_LOC + 4; //4
static const int TRIP_INTERVAL_LOC = TRIP_RATE_MULTIPLIER_LOC + 4; //4

static const int BACKPLATE_COLOR_1_LOC = TRIP_INTERVAL_LOC + 4; //4
static const int BACKPLATE_COLOR_2_LOC = BACKPLATE_COLOR_1_LOC + 4; //4
static const int BACKPLATE_COLOR_3_LOC = BACKPLATE_COLOR_2_LOC + 4; //4
static const int BACKPLATE_COLOR_4_LOC = BACKPLATE_COLOR_3_LOC + 4; //4
static const int BACKPLATE_COLOR_C_LOC = BACKPLATE_COLOR_4_LOC + 4; //4
static const int BACKPLATE_COLOR_T_LOC = BACKPLATE_COLOR_C_LOC + 4; //4
static const int BACKPLATE_COLOR_TEAM_BATTLE_STORE_LOC = BACKPLATE_COLOR_T_LOC + 4; //4

static const int DRAW_SETTINGS_BUFFER_LOC = BACKPLATE_COLOR_TEAM_BATTLE_STORE_LOC + 4; //0x200

// HOOK Vtable
// Provides a table to store HOOKS in, allowing them to be called repeatedly from different locations!
// Registering a HOOK:
//		1) Add a new entry to the struct, then open a HOOK using ASMStart(), passing your entry as the "BranchAddress" argument.
//		2) Write the meat of your hook, **ENSURING THAT YOU END YOUR HOOK WITH A BLR INSTRUCTION**. This is how you'll get back afterwards!
//		3) Close the hook using ASMEnd().
// Calling a Registered HOOK from Your Own:
//		0) If necessary, backup the Link Register's current value (along with any other registers you need to keep) to the stack.
//		1) Call SetRegister(), passing your entry as the "value" argument.
//		2) Use MCTR to load the address into the Count Register.
//		3) Use BCTRL to branch and link to that address, from which you'll be branched into the body of the targeted HOOK. The BLR instruction
//			at the end of that HOOK's body should then send you back to where you originally BCTRL'd from, allowing execution to continue from there!
//		4) Pull any registers you backed up back off of the stack.
static struct
{
	constexpr unsigned int table_start() { return DRAW_SETTINGS_BUFFER_LOC + 0x200; };

	//const int THEME_CHANGE_APPLY_PREFIXES = table_start();

	constexpr unsigned int table_size() { return (sizeof(*this) > 1) ? (sizeof(*this)) : 0; };
	constexpr unsigned int table_end() { return table_start() + table_size(); };
} HOOK_VTABLE;

static const int START_OF_CODE_MENU = HOOK_VTABLE.table_end();


static int CurrentOffset = START_OF_CODE_MENU;

#define CODE_MENU_GECKO_IF(MenuIndex) if(MenuIndex != -1) {\
									GeckoIf(MenuIndex, EQUAL, 1); {
#define CODE_MENU_GECKO_ENDIF }GeckoEndIf(); }

//code menu flags
#define CODE_MENU_CLOSED 0
#define CODE_MENU_PRIMED 1
#define CODE_MENU_TRIGGERED 2
#define CODE_MENU_CLOSING 3
#define CODE_MENU_OPEN 4

//line types
#define SELECTION_LINE 0
#define INTEGER_LINE 1
#define FLOATING_LINE 2
#define HAS_VALUE_LIMIT 2
#define SUB_MENU_LINE 3
#define COMMENT_LINE 4
#define PRINT_LINE 5

//default code menu settings
#define INITIAL_XPOS -200
#define INITIAL_YPOS -150
#define LINE_HEIGHT 18
#define MOVE_THRESHHOLD 65
#define INCREMENT_THRESHHOLD 50
#define MOVE_NUM_WAIT_FRAMES 5
#define FIRST_MOVE_NUM_WAIT_FRAMES 15
#define INCREMENT_NUM_WAIT_FRAMES 5
#define FIRST_INCREMENT_NUM_WAIT_FRAMES 15
#define FRAME_ADVANCE_NUM_WAIT_FRAMES 4
#define FIRST_FRAME_ADVANCE_NUM_WAIT_FRAMES 16
#define TRIGGER_ENTER_SUB_MENU_BUTTON BUTTON_A
#define TRIGGER_LEAVE_SUB_MENU_BUTTON BUTTON_B
#define TRIGGER_RESET_LINE_BUTTON BUTTON_X
#define TRIGGER_RESET_PAGE_BUTTON BUTTON_Y
#define SCROLL_DISTANCE 15

//action types
#define NO_ACTION 0
#define MOVE_UP 1
#define MOVE_DOWN 2
#define ENTER_SUB_MENU 3
#define LEAVE_SUB_MENU 4
#define INCREMENT 5
#define DECREMENT 6
#define EXIT_MENU 7
#define RESET_LINE 8
#define RESET_PAGE 9

#define FRAMES_UNTIL_SLOW_MOTION 12
#define FRAMES_WAITED_DURING_SLOW_MOTION 3

static vector<int> Defaults;



class Page;

class Line
{
public:
	Line() {}

	Line(string Text, u16 TextOffset, u8 type, u8 flags, u8 ColorOffset, int* Index = nullptr) {
		this->Text = Text + "\0"s;
		this->type = type;
		this->Flags = flags;
		this->Color = ColorOffset;
		this->TextOffset = TextOffset;
		this->Index = Index;
		Size = Text.size() + TextOffset + 1;

		Padding = (4 - Size % 4) % 4;
		Size += Padding;
	}

	virtual void WriteLineData()
	{
		WriteLineData({});
	}

	void WriteLineData(vector<u8> SelectionOffsets)
	{
		vector<u8> output;
		AddValueToByteArray(Size, output);
		if (Size == 0) {
			cout << Text << endl;
		}
		AddValueToByteArray(type, output);
		AddValueToByteArray(Flags, output);
		AddValueToByteArray(Color, output);
		AddValueToByteArray(TextOffset, output);
		AddValueToByteArray(lineNum, output);
		//AddValueToByteArray((u8) 0, output);
		AddValueToByteArray(Value, output);
		if (type == PRINT_LINE) {
			AddValueToByteArray(numArgs, output);
		}
		else if (type != COMMENT_LINE) {
			AddValueToByteArray(UpOffset, output);
			AddValueToByteArray(DownOffset, output);
			if (type == SUB_MENU_LINE) {
				AddValueToByteArray(SubMenuOffset, output);
			}
			else {
				AddValueToByteArray(Default, output);
				AddValueToByteArray(Max, output);
				if (type == INTEGER_LINE || type == FLOATING_LINE) {
					AddValueToByteArray(Min, output);
					AddValueToByteArray(Speed, output);
				}
			}
		}
		copy(output.begin(), output.end(), ostreambuf_iterator<char>(MenuFile));
		copy(SelectionOffsets.begin(), SelectionOffsets.end(), ostreambuf_iterator<char>(MenuFile));
		MenuFile << Text;
		WritePadding();
	}

	void WritePadding() {
		for (int i = 0; i < Padding; i++) {
			MenuFile << '\0';
		}
	}

	int* Index = nullptr;
	u32 numArgs;
	u32 Value = 0xFFFFFFFF;
	u32 Default;
	u32 Max;
	u32 Min;
	u32 Speed;
	Page *SubMenuPtr;
	u16 SubMenuOffset;
	int PageOffset;
	u8 type;
	u8 Color;
	u8 Flags = 0;
	u16 TextOffset;
	u16 DownOffset;
	u16 UpOffset;
	string Text;
	u16 Size;
	u8 lineNum;
	int Padding;
	vector<int*> args;
	//offsets
	static const int SIZE = 0; //2
	static const int TYPE = SIZE + 2; //1
	static const int FLAGS = TYPE + 1; //1
	static const int COLOR = FLAGS + 1; //1
	static const int TEXT_OFFSET = COLOR + 1; //2
	static const int LINE_NUM = TEXT_OFFSET + 2; //1
	static const int VALUE = LINE_NUM + 1; //4
	static const int PRINT_LINE_NUM_ARGS = VALUE + 4; //4
	static const int PRINT_LINE_TEXT_START = PRINT_LINE_NUM_ARGS + 4;
	static const int COMMENT_LINE_TEXT_START = VALUE + 4;
	static const int UP = VALUE + 4; //2
	static const int DOWN = UP + 2; //2
	static const int SUB_MENU = DOWN + 2; //2
	static const int SUB_MENU_LINE_TEXT_START = SUB_MENU + 2;
	static const int DEFAULT = DOWN + 2; //4
	static const int MAX = DEFAULT + 4; //4
	static const int SELECTION_LINE_OFFSETS_START = MAX + 4;
	static const int MIN = MAX + 4; //4
	static const int SPEED = MIN + 4; //4
	static const int NUMBER_LINE_TEXT_START = SPEED + 4;
};

class Comment : public Line
{
public:
	Comment(string Text, int* Index = nullptr)
	: Line(Text, COMMENT_LINE_TEXT_START, COMMENT_LINE, 0, COMMENT_LINE_COLOR_OFFSET, Index) {}

	void WriteLineData()
	{
		Line::WriteLineData();
	}
};

class Print : public Line {
public:
	Print(string Text, vector<int*> args = {})
		: Line(Text, PRINT_LINE_TEXT_START, PRINT_LINE, 0, COMMENT_LINE_COLOR_OFFSET) {
		this->args = args;
		this->numArgs = args.size();
		Size += args.size() * 4;
		for (auto x : args) {
			argValues.push_back(*x);
		}
		//cout << Size << endl;
	}

	void WriteLineData()
	{
		Line::WriteLineData();
		for (auto x : argValues) {
			sprintf(OpHexBuffer, "%08X", x);
			cout << Text << ": " << OpHexBuffer << endl;
			x = _byteswap_ulong(x);
			
			MenuFile.write((const char*)& x, 4);
			//sprintf(OpHexBuffer, "%08X", x);
			//cout << Text << ": " << OpHexBuffer << endl;
			//MenuFile << OpHexBuffer;
		}
	}

private:
	vector<int> argValues;
};

class Selection : public Line
{
public:
	Selection(string Text, vector<string> Options, vector<u16> Values, int Default, int &Index)
	: Line(CreateSelectionString(Text + ":  %s", Options), SELECTION_LINE_OFFSETS_START + Options.size() * 4, SELECTION_LINE, 0, NORMAL_LINE_COLOR_OFFSET, &Index)
	{
		if (Options.size() != Values.size()) {
			cout << "Mismatched values" << endl;
			exit(-1);
		}
		u16 offset = Text.size() + 5 + 1 + SELECTION_LINE_OFFSETS_START + Options.size() * 4;
		for (int i = 0; i < Options.size(); i++) {
			AddValueToByteArray(offset, OptionOffsets);
			AddValueToByteArray(Values[i], OptionOffsets);
			offset += Options[i].size() + 1;
		}
		Value = Default;
		this->Default = Default;
		Defaults.push_back(Default);
		Defaults.push_back(Values[Default]);
		this->Max = Options.size() - 1;
	}

	Selection(string Text, vector<string> Options, int Default, int &Index)
		: Selection(Text, Options, CreateVector(Options), Default, Index) {}

	Selection(string Text, vector<string> Options, vector<u16> Values, string Default, int &Index)
		: Selection(Text, Options, Values, distance(Options.begin(), find(Options.begin(), Options.end(), Default)), Index) {}

	Selection(string Text, vector<string> Options, string Default, int &Index)
		: Selection(Text, Options, distance(Options.begin(), find(Options.begin(), Options.end(), Default)), Index) {}

	string CreateSelectionString(string Text, vector<string> Options)
	{
		for (string x : Options) {
			Text += "\0"s + x;
		}
		return Text;
	}

	vector<u16> CreateVector(vector<string> x) 
	{
		vector<u16> Values;
		for (u16 i = 0; i < x.size(); i++) {
			Values.push_back(i);
		}
		return Values;
	}

	void WriteLineData()
	{
		Line::WriteLineData(OptionOffsets);
	}

	vector<u8> OptionOffsets;
};

class Toggle : public Selection
{
public:
	Toggle(string Text, bool Default, int &Index)
		: Selection(Text,  { "OFF", "ON" }, Default, Index) {}
};

class SubMenu : public Line
{
public:
	SubMenu() {}

	SubMenu(string Text, Page* SubMenuPtr)
	: Line(Text + " >", SUB_MENU_LINE_TEXT_START, SUB_MENU_LINE, 0, NORMAL_LINE_COLOR_OFFSET)
	{
		this->SubMenuPtr = SubMenuPtr;
	}
};

class Integer : public Line
{
public:
	Integer(string Text, int Min, int Max, int Default, int Speed, int &Index, std::string format = "%d")
	: Line(Text + ":  " + format, NUMBER_LINE_TEXT_START, INTEGER_LINE, 0, NORMAL_LINE_COLOR_OFFSET, &Index)
	{
		this->Min = Min;
		this->Max = Max;
		Value = Default;
		this->Default = Default;
		Defaults.push_back(Default);
		this->Speed = Speed;
	}
};

class Floating : public Line
{
public:
	Floating(string Text, float Min, float Max, float Default, float Speed, int &Index, string format = "%f")
	: Line(Text + ":  " + format, NUMBER_LINE_TEXT_START, FLOATING_LINE, 0, NORMAL_LINE_COLOR_OFFSET, &Index)
	{
		this->Min = GetHexFromFloat(Min);
		this->Max = GetHexFromFloat(Max);
		Value = GetHexFromFloat(Default);
		this->Default = GetHexFromFloat(Default);
		Defaults.push_back(GetHexFromFloat(Default));
		this->Speed = GetHexFromFloat(Speed);
	}
};

class Page
{
public:
	string PageName = "";
	u32 CurrentLineOffset;
	u32 Size;
	u32 PrevPageOffset = 0;
	u32 NumChangedLines = 0;
	u32 PrintLowHold = 0;
	vector<Line*> Lines;
	SubMenu CalledFromLine;
	static const int NUM_WORD_ELEMS = 4;
	static const int CURRENT_LINE_OFFSET = 0;
	static const int PREV_PAGE = CURRENT_LINE_OFFSET + 4;
	static const int NUM_CHANGED_LINES = PREV_PAGE + 4;
	static const int PRINT_LOW_HOLD = NUM_CHANGED_LINES + 4;
	static const int FIRST_LINE_OFFSET = NUM_WORD_ELEMS * 4;

	Page(string Name, vector<Line*> Lines) {
		CalledFromLine = SubMenu(Name, this);
		PageName = Name;
		this->Lines = Lines;
		Size = NUM_WORD_ELEMS * 4;
		for (auto x : Lines) {
			x->PageOffset = Size;
			Size += x->Size;
		}
		for(int i = 0; i < Lines.size(); i++) {
			Lines[i]->lineNum = i;
		}
		//Lines.back()->Size = 0;
		ConnectSelectableLines();
	}
	
	void WritePage()
	{
		vector<u8> output;
		AddValueToByteArray(CurrentLineOffset, output);
		AddValueToByteArray(PrevPageOffset, output);
		AddValueToByteArray(NumChangedLines, output);
		AddValueToByteArray(PrintLowHold, output);
		copy(output.begin(), output.end(), ostreambuf_iterator<char>(MenuFile));
		for (auto x : Lines) {
			x->WriteLineData();
		}
	}

	void ConnectSelectableLines()
	{
		vector<int> SelectableLines;
		GetSelectableLines(SelectableLines);
		if (SelectableLines.size() > 0) {
			SelectableLines.insert(SelectableLines.begin(), SelectableLines.back());
			SelectableLines.push_back(SelectableLines[1]);

			for (int i = 1; i < SelectableLines.size() - 1; i++) {
				Lines[SelectableLines[i]]->UpOffset = Lines[SelectableLines[i - 1]]->PageOffset;
				Lines[SelectableLines[i]]->DownOffset = Lines[SelectableLines[i + 1]]->PageOffset;
			}
		
			CurrentLineOffset = Lines[SelectableLines[1]]->PageOffset;
			Lines[SelectableLines[1]]->Color = HIGHLIGHTED_LINE_COLOR_OFFSET;
		}
		else {
			CurrentLineOffset = NUM_WORD_ELEMS * 4;
		}
	}

	void GetSelectableLines(vector<int> &SelectableLines)
	{
		for (int i = 0; i < Lines.size(); i++) {
			if (Lines[i]->type != COMMENT_LINE && Lines[i]->type != PRINT_LINE) {
				SelectableLines.push_back(i);
			}
		}
	}
};

void PrintChar(int SettingsPtrReg, int CharReg);
void PrintString(int StringPtrReg, int NumCharsReg, int SettingsPtrReg);
void DrawBlackBackground();
void PrintPage(int PageReg, int SettingPtrReg, int Reg1, int Reg2, int Reg3, int Reg4, int Reg5, int Reg6);
void PrintCodeMenuLine(int LinePtrReg, int SettingsPtrReg, int ColorArrayPtrReg, int TempReg1, int TempReg2);
void SetTextColor(int ColorReg, int SettingsPtrReg);
void SetTextSize(int FPSizeReg, int SettingsPtrReg);
void CodeMenu();
void ActualCodes();
void ControlCodeMenu();
void PrintCodeMenu();
void PrimeCodeMenu();
void CreateMenu(Page MainPage);
void ExecuteAction(int ActionReg);
void ResetLine(int LineReg, int PageReg, int StackReg, int TypeReg, int TempReg1, int TempReg2, int TempReg3);
void ResetPage(int StackReg, int TempReg1, int TempReg2, int TempReg3, int TempReg4, int TempReg5, int TempReg6);
void ExitMenu();
void EnterMenu(int LineReg, int PageReg, int TypeReg, int TempReg1, int TempReg2);
void LeaveMenu(int PageReg, int TempReg1, int TempReg2, int TempReg3, int TempReg4, int TempReg5, int TempReg6, int ActionReg);
void DecreaseValue(int LineReg, int PageReg, int TypeReg, int TempReg1, int TempReg2, int TempReg3, int TempReg4, int TempReg5);
void IncreaseValue(int LineReg, int PageReg, int TypeReg, int TempReg1, int TempReg2, int TempReg3, int TempReg4, int TempReg5);
void Move(int LineReg, int PageReg, int NextLineOffset, int TempReg1, int TempReg2);
void GetActionFromInputs(int ButtonReg, int ControlStickXReg, int ControlStickYReg, int ResultReg);
void SetControlStickAction(int StickValReg, int TimerLoc, int NumWaitFrames, int FirstTimeNumWaitFrames, int Threshhold, int PositiveAction, int NegativeAction, int ResultReg);
void ApplyMenuSetting(int Index, int Destination, int reg1, int reg2, int size = 4);
void GetArrayValueFromIndex(int ArrayLoc, int IndexReg, int min, int max, int ResultReg = 3);
void RunIfPortToggle(int ARRAY_LOC, int PortReg);
void SaveReplay();
void SetupPrintText(int SettingPtrReg);
void SetTextPos(int XPosReg, int YPosReg, int SettingPtrReg);
void constantOverride();