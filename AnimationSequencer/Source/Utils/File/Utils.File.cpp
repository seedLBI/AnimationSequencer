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



/// <summary>
/// Filter формат: "Название\0*.file1;*.file2\0"
/// </summary>
/// <param name="TitleWindow - название окна"></param>
/// <param name="Filter - фильтр для файлов"></param>
/// <returns></returns>
std::string Save_FileDialog(const std::string& TitleWindow, const std::string& Filter) {
    OPENFILENAME ofn;       // common dialog box structure
    wchar_t szFile[460]{};        // buffer for file name
    HWND hwnd = GetConsoleWindow();              // owner window
    HANDLE hf;              // file handle
    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    // Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
    // use the contents of szFile to initialize itself.

    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);

    std::wstring w_Filter = std::wstring(Filter.begin(), Filter.end());
    ofn.lpstrFilter = w_Filter.c_str();


    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    // Display the Open dialog box. 

    std::wstring w_TitleWindow = std::wstring(TitleWindow.begin(), TitleWindow.end());

    ofn.lpstrTitle = w_TitleWindow.c_str();

    if (GetSaveFileName(&ofn) == TRUE) {

    }




    //std::wstring ws(szFile);
    //std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;

    //return myconv.to_bytes(ws);


    return wstring_to_stringUTF8(szFile);
}


/// <summary>
/// Filter формат: "Название\0*.file1;*.file2\0"
/// </summary>
/// <param name="TitleWindow - название окна"></param>
/// <param name="Filter - фильтр для файлов"></param>
/// <returns></returns>
std::string Open_FileDialog(const std::string& TitleWindow, const std::string& Filter ) {
    OPENFILENAME ofn;       // common dialog box structure
    wchar_t szFile[460]{};        // buffer for file name
    HWND hwnd = GetConsoleWindow();              // owner window
    HANDLE hf;              // file handle
    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    // Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
    // use the contents of szFile to initialize itself.

    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);

    std::wstring w_Filter = std::wstring(Filter.begin(), Filter.end());
    ofn.lpstrFilter = w_Filter.c_str();


    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    // Display the Open dialog box. 

    std::wstring w_TitleWindow = std::wstring(TitleWindow.begin(), TitleWindow.end());

    ofn.lpstrTitle = w_TitleWindow.c_str();

    if (GetOpenFileName(&ofn) == TRUE) {

    }




    //std::wstring ws(szFile);
    //std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;

    //return myconv.to_bytes(ws);


    return wstring_to_stringUTF8(szFile);
}