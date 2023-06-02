#include "stdafx.h"
#include "_CSSRosterChange.h"

const std::string codeVersion = "v1.0.0";


// Shield Color Notes:
// Goes through function: "GetResAnmClr/[nw4r3g3d7ResFileCFUl]/(g3d_resfile.o)" 0x8018dae8
//		Call Stack:
//			0x80064e30 (in make_model/[nw4r3g3d6ScnMdlRP16gfModelAnimatio]/ec_resour)
//			0x80064448 (in mk_model/[ecResource]/ec_resource.o)
// First param is pointer to I think the BRRES?
//	- Yep, pointer to Model Data [13]
//	- Whatever brres the CLR0 is in probs lol
// Second param is index of anim to use (eg. "_0", "_1", "_2", etc)
// Forcing second param to zero forces us to load the p1 shield and death plume
// It looks like after the call to _vc, r3 is the pointer to the CLR0s in the brres
// From there, it looks like it looks (0x10 * (index + 1)) + 0x14 past to get the data
// But if you go back 4 bytes from there, that's the string!
//
// Menu Notes:
// Coin, Character Outline, Tag Entry Button, and Tag Pane (setActionNo Elements, menSelChrElemntChange):
// As I've learned, these all get their colors set through the "setActionNo" function.
// The param_2 that gets passed in is an index, which gets appended to an object's base name
// to get the name to use when looking up any relevant animations (VISOs, PAT0s, CLR0s, etc.)
// To force a specific one of these to load, you'll wanna intercept the call to the relevant
// "GetResAnm___()" function call (in my case, I'm only interested in CLR0 calls).
// 
// Misc Elements (setFrameMatColor Elements, backplateColorChange):
// I'm gonna need to rewrite this one, it's currently *far* to broad in terms of what it *can* affect.
// The following are functions which call setFrameMatColor which are relevant to player slot colors:
// - setStockMarkColor/[IfPlayer]/(if_player.o): Various In-Battle Things (Franchise Icons)?
//	- Frachise Icon (First Call)
//	- BP Background (Second Call)
//	- Loupe (Third Call)
//	- Arrow (Fourth Call)
// - initMdlData/[ifVsResultTask]/(if_vsresult.o): Various Results Screen Stuff?
// At the point where we hook in this function, r31 is the original r3 value.
// Additionally, if we go *(*(r3 + 0x14) + 0x18), we find ourselves in what looks to be the 
// structure in memory which actually drives the assembled CLR0 data. I'm fairly certain that:
//	- 0x14 is flags, potentially?
//	- 0x18 is the current frame
//	- 0x1C is the frame advance rate
//	- 0x20 is maybe loop start frame?
//	- 0x24 into this data is the frame count
//	- 0x28 is a pointer to the playback policy (loop, don't loop, etc.)
//	- 0x2C IS THE CLR0 POINTER!
// 
// Hand Color:
// Goes through function: "updateTeamColor/[muSelCharHand]/mu_selchar_hand.o" 0x8069ca64
// Works a lot like the setFrameCol func, just need to intercept the frame being prescribed and overwrite it
//


void overrideSetFontColorRGBA(int red, int green, int blue, int alpha)
{
	SetRegister(5, red);
	SetRegister(6, green);
	SetRegister(7, blue);
	SetRegister(8, alpha);
}
void resultsScreenNameFix(int reg1, int reg2)
{
	overrideSetFontColorRGBA(0xFF, 0xFF, 0xFF, 0xA0);
	// Index to relevant message
	LWZ(reg1, 3, 0x0C);
	MULLI(reg2, 4, 0x48);
	ADD(reg1, reg1, reg2);
	// Load message's format byte
	LBZ(reg2, reg1, 0x00);
	// Disable text stroke (zero out 2's bit)
	ANDI(reg2, reg2, 0b11111101);
	// Store message's format byte
	STB(reg2, reg1, 0x00);
}
void transparentCSSandResultsScreenNames()
{
	// If Color Changer is enabled
	if (BACKPLATE_COLOR_1_INDEX != -1)
	{
		int reg1 = 11;
		int reg2 = 12;

		ASMStart(0x800ea73c, "[CM: _BackplateColors] Results Screen Player Names are Transparent " + codeVersion + " [QuickLava]"); // Hooks "initMdlData/[ifVsResultTask]/if_vsresult.o".
		resultsScreenNameFix(reg1, reg2);
		ASMEnd();

		ASMStart(0x800ea8c0); // Hooks same as above.
		resultsScreenNameFix(reg1, reg2);
		ASMEnd();

		ASMStart(0x8069b268, "[CM: _BackplateColors] CSS Player Names are Transparent " + codeVersion + " [QuickLava]"); // Hooks "dispName/[muSelCharPlayerArea]/mu_selchar_player_area_obj".
		overrideSetFontColorRGBA(0xFF, 0xFF, 0xFF, 0xB0);
		ASMEnd();
	}
}

void infoPacCPUTeamColorFix()
{
	int reg1 = 11;
	int reg2 = 12;
	int colorIDReg = 4;

	int skip = GetNextLabel();

	ASMStart(0x800e2108, "[CM: _BackplateColors] Fix CPU Team Colors, Cache Team Status");

	CMPLI(colorIDReg, 0x5, 0);
	JumpToLabel(skip, bCACB_LESSER);
	ADDI(colorIDReg, colorIDReg, -5);
	Label(skip);

	// Load GameGlobal Pointer
	ADDIS(reg1, 0, 0x805a);
	LWZ(reg1, reg1, 0x00e0);
	// Load GameMOdeMelee Pointer
	LWZ(reg1, reg1, 0x08);
	// Grab Team Status
	LWZ(reg1, reg1, 0x10);
	// Store in cache location
	ADDIS(reg2, 0, BACKPLATE_COLOR_TEAM_BATTLE_STORE_LOC >> 0x10);
	STB(reg1, reg2, BACKPLATE_COLOR_TEAM_BATTLE_STORE_LOC & 0xFFFF);

	ASMEnd();
}

void storeTeamBattleStatus()
{
	int reg1 = 12;

	ASMStart(0x8068eda8, "[CM: _BackplateColors] Cache SelChar Team Battle Status in Code Menu");

	// Store team battle status in our buffer word.
	ADDIS(reg1, 0, BACKPLATE_COLOR_TEAM_BATTLE_STORE_LOC >> 0x10);
	STB(4, reg1, BACKPLATE_COLOR_TEAM_BATTLE_STORE_LOC & 0xFFFF);

	ASMEnd(0x7c7c1b78); // Restore original instruction: mr	r28, r3

	// 806e0aec setSimpleSetting/[sqSingleSimple]/sq_single_simple.o
	// 800500a0 gmInitMeleeDataDefault/[gmMeleeInitData]/gm_lib.o
	// 806e0c30 setStageSetting/[sqSingleSimple]/sq_single_simple.o - triggers after match end in classic, r0 is 0 tho
	// 806e10cc setStageSetting/[sqSingleSimple]/sq_single_simple.o 
	// 806e10d8 setStageSetting/[sqSingleSimple]/sq_single_simple.o - triggers after match end, r0 is 1!
	//// 806e0bc8
	//ASMStart(0x806e10e0, "[CM: _BackplateColors] Cache globalModeMelee Team Battle Status in Code Menu");

	//// Store team battle status in our buffer word.
	//// Note, it's fine we overwrite the existing value here, as we always want the most up to date status.
	//ADDIS(reg1, 0, BACKPLATE_COLOR_TEAM_BATTLE_STORE_LOC >> 0x10);
	//STB(0, reg1, BACKPLATE_COLOR_TEAM_BATTLE_STORE_LOC & 0xFFFF);

	//ASMEnd(0x3b000001); // Restore original instruction: stb	r0, 0x0013 (r31)
}

void randomColorChange()
{
	// If Color Changer is enabled
	if (BACKPLATE_COLOR_1_INDEX != -1)
	{
		int reg1 = 11;
		int reg2 = 12;
		int	playerKindReg = 23;
		int	fighterKindReg = 31;
		int	costumeIDReg = 24;

		ASMStart(0x80697554, "[CM: _BackplateColors] MenSelChr Random Color Override " + codeVersion + " [QuickLava]"); // Hooks "setCharPic/[muSelCharPlayerArea]/mu_selchar_player_area_o".

		// Where we're hooking, we guarantee that we're dealing with the Random portrait.
		// The costumeIDReg at this moment is guaranteed to range from 0 (Red) to 4 (CPU).
		// Calculate offset into Backplate Color LOC Entries
		SetRegister(reg2, 0x4);
		MULLW(reg2, reg2, costumeIDReg);
		// Add that to first entry's location
		ORIS(reg2, reg2, BACKPLATE_COLOR_1_LOC >> 16);
		ADDI(reg2, reg2, BACKPLATE_COLOR_1_LOC & 0x0000FFFF);
		// Load line INDEX value
		LWZ(reg2, reg2, 0x00);
		// Load buffered Team Battle Status
		ADDIS(reg1, 0, BACKPLATE_COLOR_TEAM_BATTLE_STORE_LOC >> 0x10);
		LBZ(reg1, reg1, BACKPLATE_COLOR_TEAM_BATTLE_STORE_LOC & 0xFFFF);
		// If team battle flag is set...
		If(reg1, NOT_EQUAL_I_L, 0x00);
		{
			// ... use the default value from the target line.
			LWZ(costumeIDReg, reg2, Line::DEFAULT);
		}
		// Otherwise...
		Else();
		{
			// .. get the line's selected index
			LWZ(costumeIDReg, reg2, Line::VALUE);
		}
		EndIf();

		ASMEnd(0x3b5b0004); // Restore Original Instruction: addi	r26, r27, 4
	}
}

void menSelChrElemntChange()
{
	// If Color Changer is enabled
	if (BACKPLATE_COLOR_1_INDEX != -1)
	{
		int reg1 = 11;
		int reg2 = 12;
		int reg3 = 10;

		const std::string activatorString = "lBC3";
		int applyChangeLabel = GetNextLabel();
		int exitLabel = GetNextLabel();

		// Hooks "GetResAnmClr/[nw4r3g3d7ResFileCFPCc]/g3d_resfile.o".
		ASMStart(0x8018da3c, "[CM: _BackplateColors] MenSelChr Element Override " + codeVersion + " [QuickLava]",
			"\nIntercepts calls to certain player-slot-specific Menu CLR0s, and redirects them according"
			"\nto the appropriate Code Menu line. Intended for use with:"
			"\n\t- MenSelchrCentry4_TopN__#\n\t- MenSelchrChuman4_TopN__#\n\t- MenSelchrCoin_TopN__#\n\t- MenSelchrCursorB_TopN__#"
			"\nTo trigger this code on a given CLR0, set its \"OriginalPath\" field to \"" + activatorString + "\" in BrawlBox!"
		);

		// If the previous search for the targeted CLR0 was successful...
		If(3, NOT_EQUAL_I_L, 0x00);
		{
			// ... check if returned CLR0 has the activator string set.
			LWZ(reg2, 3, 0x18);
			LWZX(reg2, reg2, 3);
			SetRegister(reg1, activatorString);

			// If the activator string isn't set, then we can exit.
			CMPL(reg2, reg1, 0);
			JumpToLabel(exitLabel, bCACB_NOT_EQUAL);

			// Otherwise, we need to overwrite the target string's requested index.
			// First, we need to use strlen to get the end of the string.
			// That'll require overwriting the current r3 though, so we'll store that in reg1.
			MR(reg3, 3);

			// Then, load the string address into r3...
			MR(3, 31);
			// ... then load the strlen's address...
			SetRegister(reg2, 0x803F0640);
			// ... and run it.
			MTCTR(reg2);
			BCTRL();

			// Now that r3 is the length of the string, we can Subtract one to get the index we'll actually be overwriting.
			ADDI(3, 3, -1);
			// Load the current index byte into r3...
			LBZX(4, 3, 31);
			// ... and subtract '0' from it so we get the index as a number. Keep this!
			ADDI(4, 4, -1 * (unsigned char)'0');

			// Calculate offset into Backplate Color LOC Entries
			SetRegister(reg2, 0x4);
			MULLW(reg2, reg2, 4);
			// Add that to first entry's location
			ORIS(reg2, reg2, BACKPLATE_COLOR_1_LOC >> 16);
			ADDI(reg2, reg2, BACKPLATE_COLOR_1_LOC & 0x0000FFFF);
			// Load line INDEX value
			LWZ(reg2, reg2, 0x00);
			// Load buffered Team Battle Status
			ADDIS(reg1, 0, BACKPLATE_COLOR_TEAM_BATTLE_STORE_LOC >> 0x10);
			LBZ(reg1, reg1, BACKPLATE_COLOR_TEAM_BATTLE_STORE_LOC & 0xFFFF);
			// If team battle flag is set...
			If(reg1, NOT_EQUAL_I_L, 0x00);
			{
				// ... use the default value from the target line.
				LWZ(reg2, reg2, Line::DEFAULT);
			}
			// Otherwise...
			Else();
			{
				// .. get the line's selected index
				LWZ(reg2, reg2, Line::VALUE);
			}
			EndIf();

			// If the retrieved line value isn't the same as the original value...
			If(reg2, NOT_EQUAL_L, 4);
			{
				// ... add '0' to it to get the ASCII hex for the index...
				ADDI(reg2, reg2, '0');
				// ... and store it at the destination location in the string, as calculated before.
				STBX(reg2, 3, 31);

				// Lastly, prepare to run _vc.
				// Restore the params to what they were when we last ran _vC...
				ADDI(3, 1, 0x08);
				MR(4, 31);
				// ... load _vc's address...
				SetRegister(reg2, 0x8018CF30);
				// ... and run it.
				MTCTR(reg2);
				BCTRL();
			}
			Else();
			{
				MR(3, reg3);
			}
			EndIf();
		}
		EndIf();

		Label(exitLabel);

		ASMEnd(0x80010024); // Restore Original Instruction: lwz	r0, 0x0024 (sp)
	}
}

void shieldColorChange()
{
	// If Color Changer is enabled
	if (BACKPLATE_COLOR_1_INDEX != -1)
	{
		int reg1 = 11;
		int reg2 = 12;
		int reg3 = 4;

		const std::string activatorString = "lBC2";
		int exitLabel = GetNextLabel();

		// Hooks "GetResAnmClr/[nw4r3g3d7ResFileCFUl]/g3d_resfile.o".
		ASMStart(0x8018db38, "[CM: _BackplateColors] Shield Color + Death Plume Override " + codeVersion + " [QuickLava]",
			"\nIntercepts calls to the player-slot-specific \"ef_common\" CLR0s in common3.pac, and redirects them according"
			"\nto the appropriate Code Menu line. Intended for use with:"
			"\n\t- EffCommonShield_#\n\t- EffCommonDead_#"
			"\nTo trigger this code on a given CLR0, set its \"OriginalPath\" field to \"" + activatorString + "\" in BrawlBox!"
		);

		// Get data offset for the first CLR0 in the list
		LWZ(reg2, 3, 0x24);
		// Get the location for that entry's actual CLR0
		ADD(reg1, reg2, 3);



		// Grab the offset for the "Original Path" Value
		LWZ(reg2, reg1, 0x18);

		// Grab the first 4 bytes of the Orig Path
		LWZX(reg2, reg2, reg1);
		// Set reg3 to our Activation String
		SetRegister(reg3, activatorString);
		// Compare the 4 bytes we loaded to the target string...
		CMPL(reg2, reg3, 0);
		// ... and exit if they're not equal.
		JumpToLabel(exitLabel, bCACB_NOT_EQUAL);


		// Calculate offset into Backplate Color LOC Entries
		SetRegister(reg2, 0x4);
		MULLW(reg2, reg2, 31);
		// Add that to first entry's location
		ORIS(reg2, reg2, BACKPLATE_COLOR_1_LOC >> 16);
		ADDI(reg2, reg2, BACKPLATE_COLOR_1_LOC & 0x0000FFFF);
		// Load line INDEX value
		LWZ(reg2, reg2, 0x00);
		// Load buffered Team Battle Status
		ADDIS(reg1, 0, BACKPLATE_COLOR_TEAM_BATTLE_STORE_LOC >> 0x10);
		LBZ(reg1, reg1, BACKPLATE_COLOR_TEAM_BATTLE_STORE_LOC & 0xFFFF);
		// If team battle flag is set...
		If(reg1, NOT_EQUAL_I_L, 0x00);
		{
			// ... use the default value from the target line.
			LWZ(31, reg2, Line::DEFAULT);
		}
		// Otherwise...
		Else();
		{
			// .. get the line's selected index
			LWZ(31, reg2, Line::VALUE);
		}
		EndIf();

		Label(exitLabel);


		ASMEnd(0x381f0001); // Restore original instruction: addi r0, r31, 1
	}
}

void backplateColorChange()
{
	// 00 = Clear
	// 01 = P1
	// 02 = P2
	// 03 = P3
	// 04 = P4
	// 05 = Pink
	// 06 = Purple
	// 07 = Orange
	// 08 = Teal
	// 09 = CPU Grey

	if (BACKPLATE_COLOR_1_INDEX != -1)
	{
		int reg1 = 11;
		int reg2 = 12;

		int exitLabel = GetNextLabel();

		const std::string activatorString = "lBC1";

		CodeRaw("[CM: _BackplateColors] Hand Color Fix " + codeVersion + " [QuickLava]",
			"Fixes a conflict with Eon's Roster-Size-Based Hand Resizing code, which could"
			"\nin some cases cause CSS hands to be wind up the wrong color."
			, 
			{ 
				0x0469CA2C, 0xC0031014, // op	lfs	f0, 0x1014 (r3) @ 8069CA2C
				0x0469CAE0, 0xC0031014	// op	lfs	f0, 0x1014 (r3) @ 8069CAE0
			}
		);

		// Hooks "SetFrame/[nw4r3g3d15AnmObjMatClrResFf]/g3d_anmclr.o".
		ASMStart(0x80197fac, "[CM: _BackplateColors] HUD Color Changer " + codeVersion + " [QuickLava]",
			"\nIntercepts the setFrameMatCol calls used to color certain Menu elements by player slot, and redirects them according"
			"\nto the appropriate Code Menu lines. Intended for use with:"
			"\n\tIn sc_selcharacter.pac:"
			"\n\t\t- MenSelchrCbase4_TopN__0\n\t\t- MenSelchrCursorA_TopN__0\n\t\t- MenSelchrCmark4_TopN__0"
			"\n\tIn info.pac (and its variants, eg. info_training.pac):"
			"\n\t\t- InfArrow_TopN__0\n\t\t- InfFace_TopN__0\n\t\t- InfLoupe0_TopN__0\n\t\t- InfMark_TopN__0\n\t\t- InfPlynm_TopN__0"
			"\n\tIn stgresult.pac:"
			"\n\t\t- InfResultRank#_TopN__0\n\t\t- InfResultMark##_TopN"
			"\nTo trigger this code on a given CLR0, set its \"OriginalPath\" field to \"" + activatorString + "\" in BrawlBox!"
		);

		// Grab 0x2C past the pointer in r3, and we've got the original CLR0 data now.
		LWZ(reg1, 3, 0x2C);

		// Grab the offset for the "Original Path" Value
		LWZ(reg2, reg1, 0x18);

		// Grab the first 4 bytes of the Orig Path
		LWZX(reg2, reg2, reg1);
		// Set reg1 to our Activation String
		SetRegister(reg1, activatorString);
		// Compare the 4 bytes we loaded to the target string...
		CMPL(reg2, reg1, 0);
		// ... and exit if they're not equal.
		JumpToLabel(exitLabel, bCACB_NOT_EQUAL);

		// Convert target frame to an integer, and copy it into reg1
		SetRegister(reg1, SET_FLOAT_REG_TEMP_MEM);
		FCTIWZ(3, 1);
		STFD(3, reg1, 0x00);
		LWZ(reg1, reg1, 0x04);

		// This is only really here for the Results Screen Franchise icons.
		// Those ask for normal frame + 10 as you click through the stats.
		// This prevents that from happening.
		If(reg1, GREATER_OR_EQUAL_I_L, 10);
		{
			ADDI(reg1, reg1, -10);
		}
		EndIf();

		//Shuffle around the requested frames to line up with the Code Menu Lines
		// If we ask for frame 0, point to 1 above Transparent Line
		If(reg1, EQUAL_I, 0x00);
		{
			SetRegister(reg1, 0x06);
		}
		EndIf();
		// If we ask for frame 9, point to 1 above CPU Line
		If(reg1, EQUAL_I, 0x9);
		{
			SetRegister(reg1, 0x05);
		}
		EndIf();
		// Then subtract 1, ultimately correcting everything.
		ADDI(reg1, reg1, -0x1);

		// Calculate which code menu line we should be looking at
		SetRegister(reg2, 0x4);
		MULLW(reg2, reg2, reg1);
		// Add that to first entry's location
		ORIS(reg2, reg2, BACKPLATE_COLOR_1_LOC >> 16);
		ADDI(reg2, reg2, BACKPLATE_COLOR_1_LOC & 0x0000FFFF);
		// Load line INDEX value
		LWZ(reg2, reg2, 0x00);
		// Load buffered Team Battle Status
		ADDIS(reg1, 0, BACKPLATE_COLOR_TEAM_BATTLE_STORE_LOC >> 0x10);
		LBZ(reg1, reg1, BACKPLATE_COLOR_TEAM_BATTLE_STORE_LOC & 0xFFFF);
		// If team battle flag is set...
		If(reg1, NOT_EQUAL_I_L, 0x00);
		{
			// ... use the default value from the target line.
			LWZ(reg2, reg2, Line::DEFAULT);
	}
		// Otherwise...
		Else();
		{
			// .. get the line's selected index
			LWZ(reg2, reg2, Line::VALUE);
		}
		EndIf();

		// And now, to perform black magic int-to-float conversion, as pilfered from the Brawl game code lol.
		// Set reg1 to our staging location
		SetRegister(reg1, SET_FLOAT_REG_TEMP_MEM);

		// Store the integer to convert at the tail end of our Float staging area
		STW(reg2, reg1, 0x04);
		// Set reg2 equal to 0x4330, then store it at the head of our staging area
		ADDIS(reg2, 0, 0x4330);
		STW(reg2, reg1, 0x00);
		// Load that value into fr3 now
		LFD(3, reg1, 0x00);
		// Load the global constant 0x4330000000000000 float from 0x805A36EA
		ADDIS(reg2, 0, 0x805A);
		LFD(1, reg2, 0x36E8);
		// Subtract that constant float from our constructed float...
		FSUB(1, 3, 1);

		// ... and voila! conversion done. Not entirely sure why this works lol, though my intuition is that
		// it's setting the exponent part of the float such that the mantissa ends up essentially in plain decimal format.
		// Exponent ends up being 2^52, and a double's mantissa is 52 bits long, so seems like that's what's up.
		// Genius stuff lol.

		Label(exitLabel);

		ASMEnd(0xfc600890); // Restore original instruction: fmr	f3, f1
	}

}
