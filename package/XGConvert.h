
#ifndef __XG_CONVERT_H__
#define __XG_CONVERT_H__

#include "XGMacros.h"

class XGConvert
{
public:
	static int stringToInteger(const char* input);
	static int64 stringToInterger64(const char* input);
	static double stringToDouble(const char* input);
	static float stringToFloat(const char* input);
	static bool stringToBool(const char* input);
	static std::string integer64ToString(int64 input);
	static std::string integerToString(int input);
	static std::string floatToString(float input);
	static char* integer64ToChars(int64 input, char* buffer);
	static char* integerToChars(int input, char* buffer);
	static char* doubleToChars(double input, char* buffer);
	static char* floatToChars(float input, char* buffer);
    static int32_t NF_HTONL(int32_t nData);
    static int32_t NF_NTOHL(int32_t nData);
	static void getFiles(std::string path, std::vector<std::string>& files, std::vector<std::string> &ownname);
	static std::string& replace_str(std::string& str, const std::string& to_replaced, const std::string& newchars);

	static bool isDirExist(const char* pDir);


	static bool createDir(const char* pDir);
	
	static bool createDirEx(const char* pDir);
	static char getObliqueLine();
	//static bool XGConvert::repalaceObliqueLine(std::string );
};

#endif	// __XG_CONVERT_H__
