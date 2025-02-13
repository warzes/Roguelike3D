#include "stdafx.h"
#include "CuteUtility.h"
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <shlobj.h>
#include <algorithm>
#include <filesystem>
#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>
//=============================================================================
bool stringUtils::IsNumber(const std::string& s)
{
	// https://stackoverflow.com/a/4654718/2034041
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}
//=============================================================================
std::vector<std::string> stringUtils::Split(const char* s, char c)
{
	std::vector<std::string> result;
	do
	{
		const char* begin = s;

		if (*begin == c || *begin == '\0')
			continue;	// skip delimiter character

		while (*s != c && *s)
			s++;	// iterate until delimiter is found

		result.push_back(std::string(begin, s));

	} while (*s++);
	return result;
}
//=============================================================================
std::vector<std::string> stringUtils::Split(const std::string& str, char c)
{
	return Split(str.c_str(), c);
}
//=============================================================================
std::vector<std::string> stringUtils::Split(std::string_view s, const std::vector<char>& delimiters)
{
	std::vector<std::string> result;
	const char* ps = s.data();
	auto IsDelimiter = [&delimiters](const char c)
		{
			return std::find(delimiters.begin(), delimiters.end(), c) != delimiters.end();
		};

	do
	{
		const char* begin = ps;

		if (IsDelimiter(*begin) || (*begin == '\0'))
			continue; // skip delimiter characters

		while (!IsDelimiter(*ps) && *ps)
			ps++; // iterate until delimiter is found or string has ended

		result.push_back(std::string(begin, ps));

	} while (*ps++);
	return result;
}
//=============================================================================
std::string stringUtils::Trim(const std::string& s)
{
	size_t end = s.find_last_not_of(" \n\r\t\f\v");
	size_t begin = s.find_first_not_of(" \n\r\t\f\v");
	size_t range = end + 1 - begin;
	return (end == std::string::npos) ? "" : s.substr(begin, range);
}
//=============================================================================
void stringUtils::MakeLowercase(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
}
//=============================================================================
std::string stringUtils::GetLowercased(const std::string& str)
{
	std::string lowercased(str.size(), '\0');
	std::transform(str.begin(), str.end(), lowercased.begin(), [](unsigned char c) { return std::tolower(c); });
	return lowercased;
}
//=============================================================================
std::string stringUtils::GetUppercased(const std::string& str)
{
	std::string uppercased(str.size(), '\0');
	std::transform(str.begin(), str.end(), uppercased.begin(), [](unsigned char c) { return std::toupper(c); });
	return uppercased;
}
//=============================================================================
std::string stringUtils::CommaSeparatedNumber(const std::string& num)
{
	std::string _num = "";
	int i = 0;
	for (auto it = num.rbegin(); it != num.rend(); ++it)
	{
		if (i % 3 == 0 && i != 0)
		{
			_num += ",";
		}
		_num += *it;
		++i;
	}
	return std::string(_num.rbegin(), _num.rend());
}
//=============================================================================
std::string stringUtils::UnicodeToASCII(const wchar_t* pwstr)
{
	const std::wstring wstr(pwstr);
#if _WIN32
	// https://codingtidbit.com/2020/02/09/c17-codecvt_utf8-is-deprecated/
	if (wstr.empty()) return std::string();
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
#else
	return std::string(wstr.begin(), wstr.end());  // results in warning in C++17
#endif
}
//=============================================================================
std::string directory::GetSpecialFolderPath(SpecialFolder folder)
{
#if _WIN32
	PWSTR retPath = {};
	REFKNOWNFOLDERID folder_id = [&]()
		{
			switch (folder)
			{
			case PROGRAM_FILES:	return FOLDERID_ProgramFiles;
			case APPDATA:		return FOLDERID_RoamingAppData;
			case LOCALAPPDATA:	return FOLDERID_LocalAppData;
			case USERPROFILE:	return FOLDERID_UserProfiles;
			}
			return FOLDERID_RoamingAppData;
		}();

	HRESULT hr = SHGetKnownFolderPath(folder_id, 0, NULL, &retPath);
	if (hr != S_OK)
	{
		// Log::Error("SHGetKnownFolderPath() returned %s.", hr == E_FAIL ? "E_FAIL" : "E_INVALIDARG");
		return "";
	}
	return stringUtils::UnicodeToASCII(retPath);
#else
	assert(false);	// IMPLEMENT: platform-specific logic for other platforms
	return "";
#endif
}
//=============================================================================
std::string directory::GetFileNameWithoutExtension(std::string_view path)
{
	// example: path: "Archetypes/player.txt" | return val: "player"
	const std::vector<std::string> pathTokens = stringUtils::Split(path.data(), '.');
#if _DEBUG
	if (pathTokens.size() == 0)
	{
		//Warning("Empty tokens: path=%s", path);
	}
#endif
	assert(pathTokens.size() > 0);
	const std::string& no_extension = pathTokens[0];
	return stringUtils::Split(no_extension.c_str(), '/').back();
}
//=============================================================================
std::string directory::GetFileNameFromPath(const std::string& filePath)
{
	return stringUtils::Split(filePath.c_str(), { '/', '\\' }).back();
}
//=============================================================================
std::string directory::GetFileExtension(const std::string& filePath)
{
	auto v = stringUtils::Split(filePath, '.');
	return v.empty() ? "" : v.back();
}
//=============================================================================
std::string directory::GetCurrentPath()
{
	char path[MAX_PATH];
	GetModuleFileNameA(NULL, path, MAX_PATH);
	std::string::size_type pos = std::string(path).find_last_of("\\/");
	return std::string(path).substr(0, pos + 1);
}
//=============================================================================
std::vector<std::string> directory::GetFilesInPath(const std::string& path)
{
	constexpr size_t MAX_FILE_PATH_LENGTH = 300;

	std::vector<std::string> files;
	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		std::string filePath = stringUtils::UnicodeToASCII<MAX_FILE_PATH_LENGTH>(entry.path().c_str());
		files.push_back(filePath);
	}
	return files;
}
//=============================================================================
std::vector<std::string> directory::GetFlattenedFolderHierarchy(const std::string& path)
{
	std::string sanitizedPath = path;
	std::replace(sanitizedPath.begin(), sanitizedPath.end(), '/', '\\');
	return stringUtils::Split(sanitizedPath, '\\');
}
//=============================================================================
bool directory::FileExists(const std::string& pathToFile)
{
	return std::filesystem::exists(pathToFile);
}
//=============================================================================
bool directory::CreateFolderIfItDoesntExist(const std::string& directoryPath)
{
	bool bSuccess = true;
	std::vector<std::string> FolderNames = GetFlattenedFolderHierarchy(directoryPath);
	std::string parentFolder = "";
	for (const std::string& FolderName : FolderNames)
	{
		const std::string FolderPath = parentFolder.empty() ? FolderName : (parentFolder + "/" + FolderName);
		if (CreateDirectoryA(FolderPath.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError()) // note: this fails to detect a failure in case "/FolderName" 
		{
			;// directory either successfully created or already exists: NOP
			bSuccess &= true;
		}
		else
		{
			bSuccess = false;
			std::string errMsg = "Failed to create directory: " + FolderPath;
			MessageBoxA(NULL, errMsg.c_str(), "Error Creating Folder", MB_OK);
			return false;
		}
		parentFolder = FolderPath;
	}
	return bSuccess;
}
//=============================================================================
std::string directory::GetFolderPath(const std::string& pathToFile)
{
	const auto tokens = stringUtils::Split(pathToFile, '\\', '/');
	std::string path = "";
	if (!tokens.empty())
		for (int i = 0; i < tokens.size() - 1; ++i)
			path += tokens[i] + "/";
	return path;
}
//=============================================================================
bool directory::IsFileNewer(const std::string& file0, const std::string& file1)
{
	if (!FileExists(file0)) { /*Warning("DirectoryUtils::IsFileNewer(): File %s doesn't exist", file0.c_str());*/ return false; }
	if (!FileExists(file1)) { /*Warning("DirectoryUtils::IsFileNewer(): File %s doesn't exist", file1.c_str());*/ return true; }

	return std::filesystem::last_write_time(file0) > std::filesystem::last_write_time(file1);
}
//=============================================================================
std::vector<std::string> directory::ListFilesInDirectory(const std::string& Directory, const char* FileExtension)
{
	if (FileExtension)
	{
		assert(strlen(FileExtension) > 0);
		if (FileExtension[0] == '.')
			FileExtension = &FileExtension[1];
	}
	std::vector<std::string> files;
	for (const auto& entry : std::filesystem::directory_iterator(Directory))
	{
		if (entry.is_directory()) continue;
		if (FileExtension)
		{
			if (GetFileExtension(entry.path().string()) == FileExtension)
			{
				files.push_back(entry.path().string());
			}
		}
		else
		{
			files.push_back("");
		}
	}
	return files;
}
//=============================================================================
std::string GetCurrentTimeAsString()
{
	const std::time_t now = std::time(0);
	std::tm tmNow;	// current time
	localtime_s(&tmNow, &now);

	// YYYY-MM-DD_HH-MM-SS
	std::stringstream ss;
	ss << (tmNow.tm_year + 1900) << "_"
		<< std::setfill('0') << std::setw(2) << tmNow.tm_mon + 1 << "_"
		<< std::setfill('0') << std::setw(2) << tmNow.tm_mday << "-"
		<< std::setfill('0') << std::setw(2) << tmNow.tm_hour << ":"
		<< std::setfill('0') << std::setw(2) << tmNow.tm_min << ":"
		<< std::setfill('0') << std::setw(2) << tmNow.tm_sec;
	return ss.str();
}
//=============================================================================
std::string GetCurrentTimeAsStringWithBrackets() { return "[" + GetCurrentTimeAsString() + "]"; }
//=============================================================================