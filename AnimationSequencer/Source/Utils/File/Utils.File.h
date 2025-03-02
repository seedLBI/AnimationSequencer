#ifndef UTILS_FILE_H
#define UTILS_FILE_H
#include <string>
#include <fstream>
#include <windows.h>
#include <algorithm>
#include <codecvt>

#include "Utils/Text/Encoding/Wstring/Utils.Encoding.Wstring.h"

bool isFileExist(const std::string& PathFile);
bool isFileExist(const std::ifstream& Fstream_File);
bool isFileNotEmpty(std::ifstream& Fstream_File);

std::string Open_FileDialog(const std::string& TitleWindow, const std::string& Filter);
std::string Save_FileDialog(const std::string& TitleWindow, const std::string& Filter);

#endif