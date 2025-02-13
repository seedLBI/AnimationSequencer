#include "Utils.File.h"


bool isFileExist(const std::string& PathFile) {
    std::ifstream ifn(stringUTF8_to_wstring(PathFile));
    bool result = ifn.good();
    ifn.close();
    return result;
}

bool isFileExist(const std::ifstream& Fstream_File) {
    return Fstream_File.good();
}

bool isFileNotEmpty(std::ifstream& Fstream_File) {
    return !(Fstream_File.peek() == std::ifstream::traits_type::eof());
}