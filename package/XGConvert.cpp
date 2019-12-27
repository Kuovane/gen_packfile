
#include "XGConvert.h"
#include <iostream>
#include <vector>
#include <string>

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#if CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM
#include <windows.h>
#include <direct.h>
#include  <io.h>
#include  <stdio.h>
#include  <stdlib.h>
#else
#include <mach-o/dyld.h>
#include <unistd.h>
//#include <direct.h>
#include <dirent.h>

#include <sys/types.h>

#include <sys/stat.h>
#endif


#define _WS2_32_WINSOCK_SWAP_LONGLONG(l)            \
            ( ( ((l) >> 56) & 0x00000000000000FFLL ) |       \
              ( ((l) >> 40) & 0x000000000000FF00LL ) |       \
              ( ((l) >> 24) & 0x0000000000FF0000LL ) |       \
              ( ((l) >>  8) & 0x00000000FF000000LL ) |       \
              ( ((l) <<  8) & 0x000000FF00000000LL ) |       \
              ( ((l) << 24) & 0x0000FF0000000000LL ) |       \
              ( ((l) << 40) & 0x00FF000000000000LL ) |       \
              ( ((l) << 56) & 0xFF00000000000000LL ) )

int XGConvert::stringToInteger(const char* input)
{
	return atoi(input);
}

int64 XGConvert::stringToInterger64(const char* input)
{
	int64 nNumber = 0;
	for(int i = strlen(input)-1 , j = 0; i >= 0; i--,j++)
	{
		int64 nTemp = input[i]-'0';
		//CCAssert(nTemp>=0 && nTemp < 10,"");
		for(int k = 0; k < j ; k++)
		{
			nTemp *= 10;
		}
		nNumber += nTemp;
	}
	return nNumber;
}

double XGConvert::stringToDouble(const char* input)
{
	return atof(input);
}

float XGConvert::stringToFloat(const char* input)
{
	return (float)XGConvert::stringToDouble(input);
}

bool XGConvert::stringToBool(const char* input)
{
	return (!strcmp(input, "true"));
}


std::string XGConvert::integer64ToString(int64 input)
{
	char buffer[64] = { 0 };
	XGConvert::integer64ToChars(input, buffer);
	return buffer;
}

std::string XGConvert::integerToString(int input)
{
	char buffer[32] = { 0 };
	XGConvert::integerToChars(input, buffer);
	return buffer;
}

std::string XGConvert::floatToString(float input)
{
	char buffer[32] = { 0 };
	XGConvert::floatToChars(input, buffer);
	return buffer;
}

char* XGConvert::integer64ToChars(int64 input, char* buffer)
{
	sprintf(buffer, "%lld", input);
	return buffer;
}

char* XGConvert::integerToChars(int input, char* buffer)
{
	sprintf(buffer, "%d", input);
	return buffer;
}

char* XGConvert::doubleToChars(double input, char* buffer)
{
	sprintf(buffer, "%f", input);
	return buffer;
}

char* XGConvert::floatToChars(float input, char* buffer)
{
	return XGConvert::doubleToChars(input, buffer);
}

int32_t XGConvert::NF_HTONL(int32_t nData)
{
    
    return htonl(nData);
    
}

int32_t XGConvert::NF_NTOHL(int32_t nData)
{
    return ntohl(nData);
}
#if CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM
void XGConvert::getFiles(std::string path, std::vector<std::string>& files, std::vector<std::string> &ownname)
{
    /*files存储文件的路径及名称(eg.   C:\Users\WUQP\Desktop\test_devided\data1.txt)
     ownname只存储文件的名称(eg.     data1.txt)*/
    
    //文件句柄
    long   hFile = 0;
    //文件信息
    struct _finddata_t fileinfo;
    std::string p;
    if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
    {
        do
        {
            //如果是目录,迭代之
            //如果不是,加入列表
            if ((fileinfo.attrib &  _A_SUBDIR))
            {
                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                    getFiles(p.assign(path).append("\\").append(fileinfo.name), files, ownname);
            }
            else
            {
                files.push_back(p.assign(path).append("\\").append(fileinfo.name));
                ownname.push_back(fileinfo.name);
            }
        } while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }
}
#else
void XGConvert::getFiles(std::string path, std::vector<std::string>& files, std::vector<std::string> &ownname)
{
    struct dirent *ptr = nullptr;
    DIR *dir = nullptr;
    std::string PATH = path;
    //printf("opendir %s\n",PATH.c_str());
    dir=opendir(PATH.c_str());
    std::string p;
    while((ptr=readdir(dir))!=NULL)
    {

        if(ptr->d_name[0] == '.')
            continue;
        if(ptr->d_type == DT_DIR)
        {
            getFiles(p.assign(path).append("/").append(ptr->d_name), files, ownname);
        }
        else
        {
            files.push_back(p.assign(path).append("/").append(ptr->d_name));
            ownname.push_back(ptr->d_name);
           // files.push_back(ptr->d_name);
        }
        
        
    }
    if(dir){
        closedir(dir);
        
    }
    else
    {
        printf("opendir fail\n");
    }
    
    
}
#endif


std::string& XGConvert::replace_str(std::string& str, const std::string& to_replaced, const std::string& newchars)
{
	for (std::string::size_type pos(0); pos != std::string::npos; pos += newchars.length())
	{
		pos = str.find(to_replaced, pos);
		if (pos != std::string::npos)
			str.replace(pos, to_replaced.length(), newchars);
		else
			break;
	}
	return   str;
}


bool XGConvert::isDirExist(const char* pDir)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	return 0 == _access(pDir, 0);
#else
	return 0 == access(pDir, 0);
#endif
}


bool XGConvert::createDir(const char* pDir)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	return 0 == _mkdir(pDir);
#else
	return 0 == mkdir(pDir, 0777);
#endif
}

bool XGConvert::createDirEx(const char* pDir)
{

	//VxLocalString sLocalT(&t, pDir);

	auto ch = getObliqueLine();

	int len = strlen(pDir);
	char* t = (char*)malloc(len + 1);
	char* p = t;


	memcpy(t, pDir, len);

	t[len] = 0;

	while (t = strchr(++t, ch))
	{
		*t = 0;
		if (isDirExist(p))
		{
			*t = ch;
			continue;
		}
		if (!createDir(p))
		{
			return false;
		}
		*t = ch;
	}
	free(p);
	return true;
}


char XGConvert::getObliqueLine()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	return '\\';
#else
	return '/';
#endif
}