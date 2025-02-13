#ifndef UTILS_FILE_H
#define UTILS_FILE_H
#include <string>
#include <fstream>
#include "Utils/Text/Encoding/Wstring/Utils.Encoding.Wstring.h"

bool isFileExist(const std::string& PathFile);
bool isFileExist(const std::ifstream& Fstream_File);
bool isFileNotEmpty(std::ifstream& Fstream_File);


#endif