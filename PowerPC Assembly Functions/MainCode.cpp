#include "stdafx.h"
#include "ReplayFix.h"
#include "Controls.h"
#include "StopStartAlts.h"
#include "Save States.h"
#include "Miscellaneous Code.h"
#include "StopDPadOnSSS (except Wiimote).h"
#include "DrawDI.h"
#include "Code Menu.h"
#include "IASA Overlay.h"
#include "Control Codes.h"
#include "Last Character Auto Select.h"
#include "Tag Based Costumes.h"
#include "Light Shield.h"
#include "IkeClimbers.h"
#include "AIDisplay.h"
#include "C++Injection.h"
#include "_AdditionalCode.h"
#include "_CSSRosterChange.h"
#include "_ThemeChange.h"
//#include "FPS Display.h"
using namespace std;

int main(int argc, char** argv)
{
	if (argc > 1)
	{
		for (unsigned long i = 1; i < argc && i < lava::argumentIDs::argumentCount; i++)
		{
			if (std::strcmp("-", argv[i]) != 0)
			{
				try
				{
					bool decisionVal = !(std::stoi(argv[i]) == 0);
					switch (i)
					{
						case lava::argumentIDs::aI_CMNU:
						{
							lava::CMNUCopyOverride = decisionVal;
							std::cout << "[C.ARG] Forcing CMNU decision to: ";
							break;
						}
						case lava::argumentIDs::aI_ASM:
						{
							lava::ASMCopyOverride = decisionVal;
							std::cout << "[C.ARG] Forcing ASM decision to: ";
							break;
						}
						case lava::argumentIDs::aI_GCT:
						{
							lava::GCTBuildOverride = decisionVal;
							std::cout << "[C.ARG] Forcing GCT decision to: ";
							break;
						}
						case lava::argumentIDs::aI_CLOSE:
						{
							lava::CloseOnFinishBypass = decisionVal;
							std::cout << "[C.ARG] Bypass push button to close?: ";
							break;
						}
						default:
						{
							break;
						}
					}
					if (decisionVal)
					{
						std::cout << "Yes\n";
					}
					else
					{
						std::cout << "No\n";
					}
				}
				catch (std::exception e)
				{
					std::cerr << "[ERROR] Invalid argument value (\"" << argv[i] << "\") provided. ";
					switch (i)
					{
						case lava::argumentIDs::aI_CMNU:
						{
							std::cerr << "CMNU";
							break;
						}
						case lava::argumentIDs::aI_ASM:
						{
							std::cerr << "ASM";
							break;
						}
						case lava::argumentIDs::aI_GCT:
						{
							std::cerr << "GCT";
							break;
						}
						case lava::argumentIDs::aI_CLOSE:
						{
							std::cerr << "Push button to close";
							break;
						}
						default:
						{
							break;
						}
					}
					std::cerr << " argument not processed!\n";
				}
			}
		}
	}

	std::cout << "PowerPC Assembly Functions (Code Menu Building Utility " << lava::version << ")\n";
	if (!std::filesystem::is_directory(outputFolder))
	{
		std::filesystem::create_directories(outputFolder);
	}
	if (std::filesystem::is_directory(outputFolder))
	{
		initMenuFileStream();
		string OutputTextPath = asmTextOutputFilePath;

		loadMenuOptionsTree(cmnuOptionsOutputFilePath, menuOptionsTree);
		applyCharacterListSettingFromMenuOptionsTree(menuOptionsTree);
		buildCharacterIDLists();
		buildRosterLists();
		buildThemeLists();

		std::ofstream codeMenuLogOutput;
		codeMenuLogOutput.open(outputFolder + changelogFileName);
		codeMenuLogOutput << "PowerPC Assembly Functions (Code Menu Building Utility " << lava::version << ")\n";
		codeMenuLogOutput << "Building \"" << cmnuFileName << "\" for ";
		std::cout << "Building \"" << cmnuFileName << "\" for ";
		switch (BUILD_TYPE)
		{
			case NORMAL:
			{
				codeMenuLogOutput << "LegacyTE";
				std::cout << "LegacyTE";
				break;
			}
			case PMEX:
			{
				codeMenuLogOutput << "Project M EX";
				std::cout << "Project M EX";
				break;
			}
			case PROJECT_PLUS:
			{
				if (PROJECT_PLUS_EX_BUILD == true)
				{
					codeMenuLogOutput << "Project+ EX";
					std::cout << "Project+ EX";
				}
				else
				{
					codeMenuLogOutput << "Project+";
					std::cout << "Project+";
				}
				break;
			}
			default:
			{
				codeMenuLogOutput << "Unknown";
				std::cout << "Unknown";
				break;
			}
		}
		if (BUILD_NETPLAY_FILES == true)
		{
			codeMenuLogOutput << " Netplay";
			std::cout << " Netplay";
		}
		if (DOLPHIN_BUILD == true)
		{
			codeMenuLogOutput << " (Dolphin)";
			std::cout << " (Dolphin)";
		}
		else
		{
			codeMenuLogOutput << " (Console)";
			std::cout << " (Console)";
		}
		codeMenuLogOutput << "\n";
		std::cout << "\n";
		if (DOLPHIN_BUILD == true)
		{
			codeMenuLogOutput << "Note: This code menu was configured for use with Dolphin only, and IS NOT COMPATIBLE with consoles!\n";
			codeMenuLogOutput << "\tAttempting to use this code menu on console can (and likely will) damage your system.\n";
			std::cout << "Note: This code menu was configured for use with Dolphin only, and IS NOT COMPATIBLE with consoles!\n";
			std::cout << "\tAttempting to use this code menu on console can (and likely will) damage your system.\n";
		}

		if (TOURNAMENT_ADDITION_BUILD == true)
		{
			codeMenuLogOutput << "Note: Tournament Addition Flag is ON!\n";
			std::cout << "Note: Tournament Addition Flag is ON!\n";
		}
		if (IS_DEBUGGING == true)
		{
			codeMenuLogOutput << "Note: General Debug Flag is ON!\n";
			std::cout << "Note: General Debug Flag is ON!\n";
		}
		if (EON_DEBUG_BUILD == true)
		{
			codeMenuLogOutput << "Note: Eon's Debug Flag is ON!\n";
			std::cout << "Note: Eon's Debug Flag is ON!\n";
		}

		codeMenuLogOutput << "\n";
		std::cout << "\n";

		if (COLLECT_EXTERNAL_ROSTERS == true)
		{
			codeMenuLogOutput << "Adding Rosters to Code Menu from \"" << rosterInputFileName << "\"...\n";
			std::cout << "Adding Rosters to Code Menu from \"" << rosterInputFileName << "\"...\n";

			bool rosterInputOpenedSuccessfully = 0;
			std::vector<std::pair<std::string, std::string>> rosterNameFileNamePairs = lava::collectedRosterNamePathPairs(rosterInputFileName, rosterInputOpenedSuccessfully);
			if (rosterInputOpenedSuccessfully)
			{
				if (rosterNameFileNamePairs.size())
				{
					std::vector<std::pair<std::string, std::string>> zippedRosterVec{};
					std::map<std::string, std::size_t> rosterNameToIndexMap{};
					for (std::size_t i = 0; i < ROSTER_LIST.size(); i++)
					{
						zippedRosterVec.push_back(std::make_pair(ROSTER_LIST[i], ROSTER_FILENAME_LIST[i]));
						rosterNameToIndexMap.insert(std::make_pair(ROSTER_LIST[i], i));
					}
					for (int i = 0; i < rosterNameFileNamePairs.size(); i++)
					{
						std::pair<std::string, std::string>* currPair = &rosterNameFileNamePairs[i];
						if (currPair->second.size())
						{
							auto itr = rosterNameToIndexMap.find(currPair->first);
							if (itr == rosterNameToIndexMap.end())
							{
								zippedRosterVec.push_back(*currPair);
								rosterNameToIndexMap.insert(std::make_pair(currPair->first, zippedRosterVec.size() - 1));
								std::cout << "[ADDED] " << currPair->first << " (Filename: " << currPair->second << ")\n";
								codeMenuLogOutput << "[ADDED] " << currPair->first << " (Filename: " << currPair->second << ")\n";
							}
							// Otherwise, announce what was changed.
							else
							{
								zippedRosterVec[itr->second].second = currPair->second;
								std::cout << "[CHANGED] " << itr->first << " (Filename: " << currPair->second << ")\n";
								codeMenuLogOutput << "[CHANGED] " << itr->first << " (Filename: " << currPair->second << ")\n";
							}
						}
						else
						{
							std::cerr << "[ERROR] Invalid Filename specified! The roster \"" << currPair->first << "\" will not be added to the Code Menu!\n";
							codeMenuLogOutput << "[ERROR] Invalid Filename specified! The roster \"" << currPair->first << "\" will not be added to the Code Menu!\n";
						}
					}

					// Write the newly edited list back into the list vectors
					ROSTER_LIST.clear();
					ROSTER_FILENAME_LIST.clear();
					for (auto itr = zippedRosterVec.cbegin(); itr != zippedRosterVec.cend(); itr++)
					{
						ROSTER_LIST.push_back(itr->first);
						ROSTER_FILENAME_LIST.push_back(itr->second);
					}
				}
				else
				{
					std::cout << "[WARNING] \"" << rosterInputFileName << "\" was opened successfully, but no valid roster entries could be found.\n";
					codeMenuLogOutput << "[WARNING] \"" << rosterInputFileName << "\" was opened successfully, but no valid roster entries could be found.\n";
				}
			}
			else
			{
				std::cout << "[ERROR] Couldn't open \"" << rosterInputFileName << "\"! Ensure that the file is present in this folder and try again!\n";
				codeMenuLogOutput << "[ERROR] Couldn't open \"" << rosterInputFileName << "\"! Ensure that the file is present in this folder and try again!\n";
			}
			//Print the results.
			std::cout << "\nFinal Roster List:\n";
			codeMenuLogOutput << "\nFinal Roster List:\n";
			for (std::size_t i = 0; i < ROSTER_LIST.size(); i++)
			{
				std::cout << "\t" << ROSTER_LIST[i] << " (Filename: " << ROSTER_FILENAME_LIST[i] << ")\n";
				codeMenuLogOutput << "\t" << ROSTER_LIST[i] << " (Filename: " << ROSTER_FILENAME_LIST[i] << ")\n";
			}

			std::cout << "\n";
			codeMenuLogOutput << "\n";
		}
		if (COLLECT_EXTERNAL_EX_CHARACTERS == true)
		{
			codeMenuLogOutput << "Adding Characters to Code Menu from \"" << exCharInputFileName << "\"...\n";
			std::cout << "Adding Characters to Code Menu from \"" << exCharInputFileName << "\"...\n";

			bool exCharInputOpenedSuccessfully = 0;
			std::vector<std::pair<std::string, u16>> nameIDPairs = lava::collectNameSlotIDPairs(exCharInputFileName, exCharInputOpenedSuccessfully);
			if (exCharInputOpenedSuccessfully)
			{
				if (nameIDPairs.size())
				{
					// Builds a map from the predefined character and character ID lists.
					// Doing it this way ensures that paired values stay together, and handles sorting automatically when we insert new entries.
					std::map<std::string, u16> zippedIDMap;
					for (int i = 0; i < CHARACTER_LIST.size(); i++)
					{
						zippedIDMap.insert(std::make_pair(CHARACTER_LIST[i], CHARACTER_ID_LIST[i]));
					}
					for (int i = 0; i < nameIDPairs.size(); i++)
					{
						std::pair<std::string, u16>* currPair = &nameIDPairs[i];
						if (currPair->second != SHRT_MAX)
						{
							auto itr = zippedIDMap.insert(*currPair);
							// If the entry was newly added to the list (ie. not overwriting existing data), announce it.
							if (itr.second)
							{
								std::cout << "[ADDED] " << itr.first->first << " (Slot ID = 0x" << lava::numToHexStringWithPadding(itr.first->second, 2) << ")\n";
								codeMenuLogOutput << "[ADDED] " << itr.first->first << " (Slot ID = 0x" << lava::numToHexStringWithPadding(itr.first->second, 2) << ")\n";
							}
							// Otherwise, announce what was changed.
							else if (itr.first != zippedIDMap.end())
							{
								itr.first->second = currPair->second;
								std::cout << "[CHANGED] " << itr.first->first << " (Slot ID = 0x" << lava::numToHexStringWithPadding(itr.first->second, 2) << ")\n";
								codeMenuLogOutput << "[CHANGED] " << itr.first->first << " (Slot ID = 0x" << lava::numToHexStringWithPadding(itr.first->second, 2) << ")\n";
							}
						}
						else
						{
							std::cerr << "[ERROR] Invalid Slot ID specified! The character \"" << currPair->first << "\" will not be added to the Code Menu!\n";
							codeMenuLogOutput << "[ERROR] Invalid Slot ID specified! The character \"" << currPair->first << "\" will not be added to the Code Menu!\n";
						}
					}

					// Write the newly edited list back into the list vectors
					CHARACTER_LIST.clear();
					CHARACTER_ID_LIST.clear();
					for (auto itr = zippedIDMap.begin(); itr != zippedIDMap.end(); itr++)
					{
						CHARACTER_LIST.push_back(itr->first);
						CHARACTER_ID_LIST.push_back(itr->second);
					}
				}
				else
				{
					std::cout << "[WARNING] \"" << exCharInputFileName << "\" was opened successfully, but no valid character entries could be found.\n";
					codeMenuLogOutput << "[WARNING] \"" << exCharInputFileName << "\" was opened successfully, but no valid character entries could be found.\n";
				}
			}
			else
			{
				std::cout << "[ERROR] Couldn't open \"" << exCharInputFileName << "\"! Ensure that the file is present in this folder and try again!\n";
				codeMenuLogOutput << "[ERROR] Couldn't open \"" << exCharInputFileName << "\"! Ensure that the file is present in this folder and try again!\n";
			}
			//Print the results.
			std::cout << "\nFinal Character List:\n";
			codeMenuLogOutput << "\nFinal Character List:\n";
			for (std::size_t i = 0; i < CHARACTER_LIST.size(); i++)
			{
				std::cout << "\t" << CHARACTER_LIST[i] << " (Slot ID = 0x" << lava::numToHexStringWithPadding(CHARACTER_ID_LIST[i], 2) << ")\n";
				codeMenuLogOutput << "\t" << CHARACTER_LIST[i] << " (Slot ID = 0x" << lava::numToHexStringWithPadding(CHARACTER_ID_LIST[i], 2) << ")\n";
			}

			std::cout << "\n";
			codeMenuLogOutput << "\n";
		}
		

		CodeStart(OutputTextPath);
		//place all ASM code here

		//ReplayFix();

		//NameIsFound();

		//MenuControlCodes();

		//StopStartAltFunctions();

		//StopPokemonTrainerSwitch();

		//StopDPadOnSSS();

		//ConvertButtons();

		//ItemSpawnControl();

		//ClearASLData();

		//SetTeamAttackTraining();

		//LXPGreenOverlayFix();

		CodeMenu(); tagBasedCostumes(); cssRosterChange(); themeChange();

		//musicPercentCode();

		//DoubleFighterTest();

		//UCF();

		//CStickSlowFix();

		//FixStickyRAlts();

		//SelectLastCharacter();

		//FixTr4shTeamToggle();

		//cstickTiltTest();

		//FPSDisplay();

		//CStickTiltFix();

		//DBZModeTest();

		//slipperyTechs();

		//lightShield();

		//IkeClimbers();

		//fixStanimaTextBug();

		//AIDisplay();

		//loadCppCodes(); writeInjectionsRepeat();

		CodeEnd();

		std::cout << "\n";

		if (std::filesystem::is_regular_file(cmnuBuildLocationFilePath))
		{
			if (lava::offerCopyOverAndBackup(cmnuOutputFilePath, cmnuBuildLocationFilePath, lava::CMNUCopyOverride))
			{
				codeMenuLogOutput << "Note: Backed up \"" << cmnuBuildLocationFilePath << "\" and overwrote it with the newly built Code Menu.\n";
			}
		}
		else if (std::filesystem::is_directory(buildFolder + cmnuBuildLocationDirectory))
		{
			if (lava::offerCopy(cmnuOutputFilePath, cmnuBuildLocationFilePath, lava::CMNUCopyOverride))
			{
				codeMenuLogOutput << "Note: Copied newly built Code Menu to \"" << cmnuBuildLocationFilePath << "\".\n";
			}
		}
		if (MakeASM(OutputTextPath, asmOutputFilePath))
		{
			if (std::filesystem::is_regular_file(asmBuildLocationFilePath))
			{
				if (lava::offerCopyOverAndBackup(asmOutputFilePath, asmBuildLocationFilePath, lava::ASMCopyOverride))
				{
					codeMenuLogOutput << "Note: Backed up \"" << asmBuildLocationFilePath << "\" and overwrote it with the newly built ASM.\n";
				}
			}
			else if (std::filesystem::is_directory(buildFolder + asmBuildLocationDirectory))
			{
				if (lava::offerCopy(asmOutputFilePath, asmBuildLocationFilePath, lava::ASMCopyOverride))
				{
					codeMenuLogOutput << "Note: Copied newly built ASM to \"" << asmBuildLocationFilePath << "\".\n";
				}
			}
			if (lava::handleAutoGCTRMProcess(codeMenuLogOutput, lava::GCTBuildOverride) && BUILD_NETPLAY_FILES)
			{
				codeMenuLogOutput << "Note: The built GCTs are configured for use in Dolphin Netplay only, and ARE NOT COMPATIBLE with consoles!\n";
				codeMenuLogOutput << "Attempting to use them on console can (and likely will) damage your system.\n\n";
			}
		}
	}
	else
	{
		std::cerr << "[ERROR] The expected output folder (\"" << outputFolder << "\") couldn't be found or created in this folder. Ensure that the specified folder exists in the same folder as this program and try again.\n\n";
	}
	if (lava::CloseOnFinishBypass == INT_MAX || lava::CloseOnFinishBypass == 0)
	{
		std::cout << "Press any key to exit.\n";
		_getch();
	}
	return 0;
}
