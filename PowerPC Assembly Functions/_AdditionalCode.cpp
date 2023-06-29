#include "stdafx.h"
#include "_AdditionalCode.h"

namespace lava
{
	int CMNUCopyOverride = INT_MAX;
	int ASMCopyOverride = INT_MAX;
	int GCTBuildOverride = INT_MAX;
	int CloseOnFinishBypass = INT_MAX;

	bool copyFile(std::string sourceFile, std::string targetFile, bool overwriteExistingFile)
	{
		// Record result
		bool result = 0;
		if (sourceFile != targetFile)
		{
			// Initialize in and out streams
			std::ifstream sourceFileStream;
			std::ofstream targetFileStream;
			// Open and test input stream
			sourceFileStream.open(sourceFile, std::ios_base::in | std::ios_base::binary);
			if (sourceFileStream.is_open())
			{
				if (overwriteExistingFile || !std::filesystem::is_regular_file(targetFile))
				{
					// If successful, open and test output stream
					targetFileStream.open(targetFile, std::ios_base::out | std::ios_base::binary);
					if (targetFileStream.is_open())
					{
						// If both streams are open and valid, copy over the file's contents and record the success in result
						targetFileStream << sourceFileStream.rdbuf();
						result = 1;
					}
					targetFileStream.close();
				}
			}
			sourceFileStream.close();
		}
		return result;
	}
	bool backupFile(std::string fileToBackup, std::string backupSuffix, bool overwriteExistingBackup)
	{
		return copyFile(fileToBackup, fileToBackup + backupSuffix, overwriteExistingBackup);
	}

	bool yesNoDecision(char yesKey, char noKey)
	{
		char keyIn = ' ';
		yesKey = std::tolower(yesKey);
		noKey = std::tolower(noKey);
		while (keyIn != yesKey && keyIn != noKey)
		{
			keyIn = _getch();
			keyIn = std::tolower(keyIn);
		}
		return (keyIn == yesKey);
	}
	bool offerCopyOverAndBackup(std::string fileToCopy, std::string fileToOverwrite, int decisionOverride)
	{
		bool backupSucceeded = 0;
		bool copySucceeded = 0;

		if (std::filesystem::is_regular_file(fileToCopy) && std::filesystem::is_regular_file(fileToOverwrite))
		{
			std::cout << "Detected \"" << fileToOverwrite << "\".\n" <<
				"Would you like to copy \"" << fileToCopy << "\" over it? " <<
				"A backup will be made of the existing file.\n";
			std::cout << "[Press 'Y' for Yes, 'N' for No]\n";
			if ((decisionOverride == INT_MAX && yesNoDecision('y', 'n')) || (decisionOverride != INT_MAX && decisionOverride != 0))
			{
				std::cout << "Making backup... ";
				if (lava::backupFile(fileToOverwrite, ".bak", 1))
				{
					backupSucceeded = 1;
					std::cout << "Success!\nCopying over \"" << fileToCopy << "\"... ";
					if (lava::copyFile(fileToCopy, fileToOverwrite, 1))
					{
						copySucceeded = 1;
						std::cout << "Success!\n";
					}
					else
					{
						std::cerr << "Failure! Please ensure that the destination file is able to be written to!\n";
					}
				}
				else
				{
					std::cerr << "Backup failed! Please ensure that " << fileToOverwrite << ".bak is able to be written to!\n";
				}
			}
			else
			{
				std::cout << "Skipping copy.\n";
			}
			std::cout << "\n";
		}

		return backupSucceeded && copySucceeded;
	}
	bool offerCopy(std::string fileToCopy, std::string fileToOverwrite, int decisionOverride)
	{
		bool copySucceeded = 0;

		if (std::filesystem::is_regular_file(fileToCopy) && !std::filesystem::is_regular_file(fileToOverwrite))
		{
			std::cout << "Couldn't detect \"" << fileToOverwrite << "\".\n" << "Would you like to copy \"" << fileToCopy << "\" to that location?\n";
			std::cout << "[Press 'Y' for Yes, 'N' for No]\n";
			if ((decisionOverride == INT_MAX && yesNoDecision('y', 'n')) || (decisionOverride != INT_MAX && decisionOverride != 0))
			{
				std::cout << "Copying over \"" << fileToCopy << "\"... ";
				if (lava::copyFile(fileToCopy, fileToOverwrite, 1))
				{
					copySucceeded = 1;
					std::cout << "Success!\n";
				}
				else
				{
					std::cerr << "Failure! Please ensure that the destination file is able to be written to!\n";
				}
			}
			else
			{
				std::cout << "Skipping copy.\n";
			}
			std::cout << "\n";
		}

		return copySucceeded;
	}
	bool handleAutoGCTRMProcess(std::ostream& logOutput, int decisionOverride)
	{
		bool result = 0;

		if (std::filesystem::is_regular_file(GCTRMExePath) && std::filesystem::is_regular_file(mainGCTTextFile) && std::filesystem::is_regular_file(boostGCTTextFile))
		{
			std::cout << "Detected \"" << GCTRMExePath << "\".\nWould you like to build \"" << mainGCTFile << "\" and \"" << boostGCTFile << "\"? Backups will be made of any existing files.\n";

			bool mainGCTBackupNeeded = std::filesystem::is_regular_file(mainGCTFile);
			// If no backup is needed, we can consider the backup resolved. If one is, we cannot.
			bool mainGCTBackupResolved = !mainGCTBackupNeeded;
			// Same as above.
			bool boostGCTBackupNeeded = std::filesystem::is_regular_file(boostGCTFile);
			bool boostGCTBackupResolved = !boostGCTBackupNeeded;

			std::cout << "[Press 'Y' for Yes, 'N' for No]\n";
			if ((decisionOverride == INT_MAX && yesNoDecision('y', 'n')) || (decisionOverride != INT_MAX && decisionOverride != 0))
			{
				if (mainGCTBackupNeeded || boostGCTBackupNeeded)
				{
					std::cout << "Backing up files... ";
					if (mainGCTBackupNeeded)
					{
						mainGCTBackupResolved = lava::backupFile(mainGCTFile, ".bak", 1);
					}
					if (boostGCTBackupNeeded)
					{
						boostGCTBackupResolved = lava::backupFile(boostGCTFile, ".bak", 1);
					}
				}
				if (mainGCTBackupResolved && boostGCTBackupResolved)
				{
					std::cout << "Success! Running GCTRM.\n";
					result = 1;
					std::string commandFull = "\"" + GCTRMCommandBase + "\"" + mainGCTTextFile + "\"\"";
					std::cout << "\n" << commandFull << "\n";
					system(commandFull.c_str());
					if (mainGCTBackupNeeded)
					{
						logOutput << "Note: Backed up and rebuilt \"" << mainGCTFile << "\".\n";
					}
					else
					{
						logOutput << "Note: Built \"" << mainGCTFile << "\".\n";
					}

					commandFull = "\"" + GCTRMCommandBase + "\"" + boostGCTTextFile + "\"\"";
					std::cout << "\n" << commandFull << "\n";
					system(commandFull.c_str());
					if (boostGCTBackupNeeded)
					{
						logOutput << "Note: Backed up and rebuilt \"" << boostGCTFile << "\".\n";
					}
					else
					{
						logOutput << "Note: Built \"" << boostGCTFile << "\".";
					}
					std::cout << "\n";
				}
				else
				{
					std::cerr << "Something went wrong while backing up the files. Skipping GCTRM.\n";
				}
			}
			else
			{
				std::cout << "Skipping GCTRM.\n";
			}
		}
		return result;
	}

	void WriteByteVec(const unsigned char* Bytes, u32 Address, unsigned char addressReg, unsigned char manipReg, std::size_t numToWrite, bool appendNullTerminator)
	{
		if (Address != ULONG_MAX)
		{
			SetRegister(addressReg, Address); // Load destination address into register
		}

		unsigned long fullWordCount = numToWrite / 4; // Get the number of 4 byte words we can make from the given vec.
		unsigned long currWord = 0;
		unsigned long offsetIntoBytes = 0;

		// For each word we can make...
		for (unsigned long i = 0; i < fullWordCount; i++)
		{
			// ... grab it, then store it in our manip register.
			currWord = lava::bytesToFundamental<unsigned long>(Bytes + offsetIntoBytes);
			SetRegister(manipReg, currWord);
			// Then write a command which stores the packed word into the desired location.
			STW(manipReg, addressReg, offsetIntoBytes);
			offsetIntoBytes += 0x04;
		}
		// For the remaining few bytes...
		for (offsetIntoBytes; offsetIntoBytes < numToWrite; offsetIntoBytes++)
		{
			// ... just write them one by one, no other choice really.
			SetRegister(manipReg, Bytes[offsetIntoBytes]);
			STB(manipReg, addressReg, offsetIntoBytes);
		}
		if (appendNullTerminator)
		{
			SetRegister(manipReg, 0x00);
			STB(manipReg, addressReg, offsetIntoBytes);
		}
	}
	void WriteByteVec(std::vector<unsigned char> Bytes, u32 Address, unsigned char addressReg, unsigned char manipReg, std::size_t numToWrite, bool appendNullTerminator)
	{
		WriteByteVec((const unsigned char*)Bytes.data(), Address, addressReg, manipReg, std::min<std::size_t>(Bytes.size(), numToWrite), appendNullTerminator);
	}
	void WriteByteVec(std::string Bytes, u32 Address, unsigned char addressReg, unsigned char manipReg, std::size_t numToWrite, bool appendNullTerminator)
	{
		WriteByteVec((const unsigned char*)Bytes.data(), Address, addressReg, manipReg, std::min<std::size_t>(Bytes.size(), numToWrite), appendNullTerminator);
	}

	std::vector<std::pair<std::string, u16>> collectNameSlotIDPairs(std::string exCharInputFilePath, bool& fileOpened)
	{
		std::vector<std::pair<std::string, u16>> result{};
		fileOpened = 0;

		// Read from character file
		std::ifstream exCharInputStream;
		exCharInputStream.open(exCharInputFilePath);

		if (exCharInputStream.is_open())
		{
			fileOpened = 1;
			std::string currentLine = "";
			std::string manipStr = "";
			while (std::getline(exCharInputStream, currentLine))
			{
				// Disregard the current line if it's empty, or is marked as a comment
				if (!currentLine.empty() && currentLine[0] != '#' && currentLine[0] != '/')
				{
					// Clean the string
					// Removes any space characters from outside of quotes. Note, quotes can be escaped with \\.
					manipStr = "";
					bool inQuote = 0;
					bool doEscapeChar = 0;
					for (std::size_t i = 0; i < currentLine.size(); i++)
					{
						if (currentLine[i] == '\"' && !doEscapeChar)
						{
							inQuote = !inQuote;
						}
						else if (currentLine[i] == '\\')
						{
							doEscapeChar = 1;
						}
						else if (inQuote || !std::isspace(currentLine[i]))
						{
							doEscapeChar = 0;
							manipStr += currentLine[i];
						}
					}
					// Determines the location of the delimiter, and ensures that there's something before and something after it.
					// Line is obviously invalid if that fails, so we skip it.
					std::size_t delimLoc = manipStr.find('=');
					if (delimLoc != std::string::npos && delimLoc > 0 && delimLoc < (manipStr.size() - 1))
					{
						// Store character name portion of string
						std::string characterNameIn = manipStr.substr(0, delimLoc);
						// Initialize var for character id portion of string
						u16 characterSlotIDIn = SHRT_MAX;
						// Handles hex input for character id
						characterSlotIDIn = lava::stringToNum(manipStr.substr(delimLoc + 1, std::string::npos), 1, SHRT_MAX);
						// Insert new entry into list.
						result.push_back(std::make_pair(characterNameIn, characterSlotIDIn));
					}
				}
			}
		}

		return result;
	}
	std::vector<std::pair<std::string, std::string>> collectedRosterNamePathPairs(std::string exRosterInputFilePath, bool& fileOpened)
	{
		std::vector<std::pair<std::string, std::string>> result{};
		fileOpened = 0;

		// Read from character file
		std::ifstream exCharInputStream;
		exCharInputStream.open(exRosterInputFilePath);

		if (exCharInputStream.is_open())
		{
			fileOpened = 1;
			std::string currentLine = "";
			std::string manipStr = "";
			while (std::getline(exCharInputStream, currentLine))
			{
				// Disregard the current line if it's empty, or is marked as a comment
				if (!currentLine.empty() && currentLine[0] != '#' && currentLine[0] != '/')
				{
					// Clean the string
					// Removes any space characters from outside of quotes. Note, quotes can be escaped with \\.
					manipStr = "";
					bool inQuote = 0;
					bool doEscapeChar = 0;
					for (std::size_t i = 0; i < currentLine.size(); i++)
					{
						if (currentLine[i] == '\"' && !doEscapeChar)
						{
							inQuote = !inQuote;
						}
						else if (currentLine[i] == '\\')
						{
							doEscapeChar = 1;
						}
						else if (inQuote || !std::isspace(currentLine[i]))
						{
							doEscapeChar = 0;
							manipStr += currentLine[i];
						}
					}
					// Determines the location of the delimiter, and ensures that there's something before and something after it.
					// Line is obviously invalid if that fails, so we skip it.
					std::size_t delimLoc = manipStr.find('=');
					if (delimLoc != std::string::npos && delimLoc > 0 && delimLoc < (manipStr.size() - 1))
					{
						// Store roster name portion of string
						std::string rosterNameIn = manipStr.substr(0, delimLoc);
						// Store roster filename portion of string
						std::string fileNameIn = manipStr.substr(delimLoc + 1, std::string::npos);
						// Insert new entry into list.
						result.push_back(std::make_pair(rosterNameIn, fileNameIn));
					}
				}
			}
		}

		return result;
	}

	// Menu Config Parsing and Constants
	namespace configXMLConstants
	{
		// General
		const std::string menuConfigTag = "codeMenuConfig";
		const std::string disabledTag = "disabled";
		const std::string nameTag = "name";
		const std::string filenameTag = "filename";

		// EX Characters
		const std::string characterDeclsTag = "characterDeclarations";
		const std::string slotIDTag = "slotID";

		// EX Rosters
		const std::string roseterDeclsTag = "rosterDeclarations";

		// Themes
		const std::string themeDeclsTag = "themeDeclarations";
		const std::string themeTag = "menuTheme";
		const std::string themeFileTag = "themeFile";
		const std::string prefixTag = "replacementPrefix";

		// Colors
		const std::string slotColorDeclsTag = "slotColorDeclarations";
		const std::string slotColorTag = "slotColor";
	}
	bool declNodeIsDisabled(const pugi::xml_node_iterator& declNodeItr)
	{
		return declNodeItr->attribute(configXMLConstants::disabledTag.c_str()).as_bool() == 1;
	}
	std::vector<menuTheme> collectThemesFromXML(const pugi::xml_node_iterator& themeDeclNodeItr)
	{
		std::vector<menuTheme> result;

		for (pugi::xml_node_iterator themeItr = themeDeclNodeItr->begin(); themeItr != themeDeclNodeItr->end(); themeItr++)
		{
			if (themeItr->name() == configXMLConstants::themeTag)
			{
				menuTheme tempTheme;
				tempTheme.name = themeItr->attribute(configXMLConstants::nameTag.c_str()).as_string(tempTheme.name);
				// If the entry has no name, skip to next node.
				if (tempTheme.name.empty()) continue;

				for (pugi::xml_node_iterator themeFileItr = themeItr->begin(); themeFileItr != themeItr->end(); themeFileItr++)
				{
					if (themeFileItr->name() == configXMLConstants::themeFileTag)
					{
						std::size_t filenameIndex = SIZE_MAX;

						for (pugi::xml_attribute_iterator themeFileAttrItr = themeFileItr->attributes_begin(); themeFileAttrItr != themeFileItr->attributes_end(); themeFileAttrItr++)
						{
							if (themeFileAttrItr->name() == configXMLConstants::nameTag)
							{
								auto filenameItr = std::find(themeConstants::filenames.begin(), themeConstants::filenames.end(), themeFileAttrItr->as_string());
								if (filenameItr != themeConstants::filenames.end())
								{
									filenameIndex = filenameItr - themeConstants::filenames.begin();
								}
							}
							else if (filenameIndex != SIZE_MAX && themeFileAttrItr->name() == configXMLConstants::prefixTag)
							{
								tempTheme.prefixes[filenameIndex] = themeFileAttrItr->as_string().substr(0, themeConstants::prefixLength);
								THEME_FILE_GOT_UNIQUE_PREFIX[filenameIndex] |= themeConstants::filenames[filenameIndex].find(tempTheme.prefixes[filenameIndex]) != 0x00;
							}
						}
					}
				}
				result.push_back(tempTheme);
			}
		}

		return result;
	}
	bool parseAndApplyConfigXML(std::string configFilePath, lava::outputSplitter& logOutput)
	{
		bool result = 0;

		pugi::xml_document configDoc;
		if (std::filesystem::is_regular_file(configFilePath))
		{
			pugi::xml_parse_result res = configDoc.load_file(configFilePath.c_str());
			if (res.status == pugi::xml_parse_status::status_ok || res.status == pugi::xml_parse_status::status_no_document_element)
			{
				pugi::xml_node_iterator configRoot = configDoc.end();
				for (pugi::xml_node_iterator topLevelNodeItr = configDoc.begin(); configRoot == configDoc.end() && topLevelNodeItr != configDoc.end(); topLevelNodeItr++)
				{
					if (topLevelNodeItr->name() == configXMLConstants::menuConfigTag)
					{
						configRoot = topLevelNodeItr;
					}
				}

				if (configRoot != configDoc.end())
				{
					result = 1;
					for (pugi::xml_node_iterator declNodeItr = configRoot->begin(); declNodeItr != configRoot->end(); declNodeItr++)
					{
						// If we're set to collect EX Characters...
						if (COLLECT_EXTERNAL_EX_CHARACTERS && !declNodeIsDisabled(declNodeItr) && declNodeItr->name() == configXMLConstants::characterDeclsTag)
						{
							// ... pull them from the XML and apply the changes to the menu lists.
						}
						// If we're set to collect EX Rosters...
						else if (COLLECT_EXTERNAL_ROSTERS && !declNodeIsDisabled(declNodeItr) && declNodeItr->name() == configXMLConstants::roseterDeclsTag)
						{
						}
						// If we're set to collect Themes...
						else if (COLLECT_EXTERNAL_THEMES && !declNodeIsDisabled(declNodeItr) && declNodeItr->name() == configXMLConstants::themeDeclsTag)
						{
							std::vector<menuTheme> tempThemeList = collectThemesFromXML(declNodeItr);
							// If we actually retrieved any valid themes from the file, process them!
							if (!tempThemeList.empty())
							{
								// For each newly collected theme...
								for (int i = 0; i < tempThemeList.size(); i++)
								{
									menuTheme* currTheme = &tempThemeList[i];

									// ... check to see if a theme of the same name already exists in our map.
									auto itr = std::find(THEME_LIST.begin(), THEME_LIST.end(), currTheme->name);

									// If one by that name doesn't already exist...
									if (itr == THEME_LIST.end())
									{
										// Add it to our list...
										THEME_LIST.push_back(currTheme->name);
										THEME_SPEC_LIST.push_back(*currTheme);
										// ... and announce that a theme has been successfully collected.
										logOutput << "[ADDED]";
									}
									// Otherwise, if a theme by that name *does* already exist...
									else
									{
										// ... overwrite the theme currently associated with that name...
										THEME_SPEC_LIST[itr - THEME_LIST.begin()] = *currTheme;
										// ... and announce that a theme has been changed.
										logOutput << "[CHANGED]";
									}
									// Describe the processed theme.
									logOutput << " \"" << currTheme->name << "\", Replacement Prefixes Are:\n";
									for (std::size_t u = 0; u < currTheme->prefixes.size(); u++)
									{
										logOutput << "\t\"" << themeConstants::filenames[u] << "\": \"" << currTheme->prefixes[u] << "\"\n";
									}
								}
							}
							// Otherwise, note that nothing was found.
							else
							{
								logOutput << "[WARNING] Theme Declaration block parsed, but no valid entries were found!\n";
							}

							// Do final theme list summary.
							logOutput << "\nFinal Theme List:\n";
							for (std::size_t i = 0; i < THEME_LIST.size(); i++)
							{
								logOutput << "\t\"" << THEME_LIST[i] << "\", Replacement Prefixes Are:\n";
								for (std::size_t u = 0; u < THEME_SPEC_LIST[i].prefixes.size(); u++)
								{
									logOutput << "\t\t\"" << themeConstants::filenames[u] << "\": \"" << THEME_SPEC_LIST[i].prefixes[u] << "\"\n";
								}
							}
							logOutput << "\n";
						}
						// If we're set to collect Slot Colors...
						else if (COLLECT_EXTERNAL_SLOT_COLORS && !declNodeIsDisabled(declNodeItr) && declNodeItr->name() == configXMLConstants::slotColorDeclsTag)
						{
							
						}
					}
				}
			}
		}

		return result;
	}
}