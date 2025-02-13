#pragma once

#include <sstream>

namespace stringUtils
{
	bool IsNumber(const std::string& s);

	inline bool  ParseBool(const std::string& s) { bool b; std::istringstream(s) >> std::boolalpha >> b; return b; }
	inline int   ParseInt(const std::string& s) { return std::atoi(s.c_str()); }
	inline float ParseFloat(const std::string& s) { return static_cast<float>(std::atof(s.c_str())); }

	std::vector<std::string> Split(const char* s, char c = ' ');
	std::vector<std::string> Split(const std::string& s, char c = ' ');
	std::vector<std::string> Split(std::string_view s, const std::vector<char>& delimiters);
	template<class... Args>
	std::vector<std::string> Split(std::string_view s, Args&&... args)
	{
		const std::vector<char> delimiters = { std::forward<Args>(args)... };
		return Split(s, delimiters);
	}

	std::string Trim(const std::string& s);

	void MakeLowercase(std::string& str);
	std::string GetLowercased(const std::string& str);

	std::string GetUppercased(const std::string& str);

	std::string CommaSeparatedNumber(const std::string& num);

	std::string UnicodeToASCII(const wchar_t* pwstr);
	inline std::string  UnicodeToASCII(const std::wstring& wstr) { return UnicodeToASCII(wstr.c_str()); }
	inline std::wstring ASCIIToUnicode(const std::string& str) { return std::wstring(str.begin(), str.end()); }
	inline std::wstring ASCIIToUnicode(const char* str) { return std::wstring(str, str + strlen(str)); }

	template<unsigned STR_SIZE>
	std::string UnicodeToASCII(const wchar_t wchars[STR_SIZE])
	{
		char ascii[STR_SIZE];
		size_t numCharsConverted = 0;
		wcstombs_s(&numCharsConverted, ascii, wchars, STR_SIZE);
		return std::string(ascii);
	}
} // namespace stringUtils

namespace directory
{
	enum SpecialFolder
	{
		PROGRAM_FILES,
		APPDATA,
		LOCALAPPDATA,
		USERPROFILE,
	};

	std::string	GetSpecialFolderPath(SpecialFolder folder);
	std::string	GetFileNameWithoutExtension(std::string_view fileName);
	std::string	GetFileNameFromPath(const std::string& filePath);
	std::string GetFileExtension(const std::string& filePath);
	std::string GetCurrentPath();

	std::vector<std::string> GetFilesInPath(const std::string& path);

	// removes separators from a path and returns all the folder names in a vector
	// e.g. "C:/Program Files/TestFolder" -> ["C:", "Program Files", "TestFolder"]
	std::vector<std::string> GetFlattenedFolderHierarchy(const std::string& path);

	// returns true of the given file exists.
	bool FileExists(const std::string& pathToFile);

	// returns true if folder exists, false otherwise after creating the folder
	bool CreateFolderIfItDoesntExist(const std::string& directoryPath);

	// returns the folder path given a file path
	// e.g.: @pathToFile="C:\\Folder\\File.txt" -> returns "C:\\Folder\\"
	std::string GetFolderPath(const std::string& pathToFile);

	// returns true if @file0 has been written into later than @file1 has.
	bool IsFileNewer(const std::string& file0, const std::string& file1);

	// returns a list of files, filtered by the extension if specified
	std::vector<std::string> ListFilesInDirectory(const std::string& Directory, const char* FileExtension = nullptr);
} // namespace directory

// returns current time in format "YYYY-MM-DD_HH-MM-SS"
std::string GetCurrentTimeAsString();
std::string GetCurrentTimeAsStringWithBrackets();