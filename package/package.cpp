/******************************************************
Author: KuovaneWu
2018-12-26

************************************************************/
#include "XGMacros.h"
#if CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM
#include <windows.h>
#include <direct.h>
#else
#include <mach-o/dyld.h>
#endif
#include <assert.h>
#include "XGXml.h"
#include "XGConvert.h"
#include <map>

#include "ImagePackage.h"




std::string s_projectPath;
std::string s_resPath;
std::string s_srcPath;
std::string s_desPath;
std::string s_tempPath;


static const std::string CMD_PROJECT_PATH = "-projectPath";
static const std::string CMD_PACK_IMAGE = "-packImage";
static const std::string CMD_PACK_LUA = "-packLua";
static const std::string CMD_ENCODE_LUA = "-encodeLua";
static const std::string CMD_PACK_ALL_TO_ONE = "-allToOne";

std::map<std::string, bool> s_cmdMap;



void cpyDirs()
{

	const int cmdLen = 500;
	char sCmd[cmdLen] = { 0 };

	//mymkdir(s_sFileTempPath);
#if CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM
	//memset(sCmd, 0, cmdLen);
	//sprintf(sCmd, "XCOPY %s %s /S /T /E", s_resPath.c_str(), s_desPath.c_str());
	//system(sCmd);

	//memset(sCmd, 0, cmdLen);
	//sprintf(sCmd, "XCOPY %s %s /S /T /E", s_resPath.c_str(), s_tempPath.c_str());
	//system(sCmd);
#else
   // memset(sCmd, 0, cmdLen);
   // sprintf(sCmd, "cp -p -R %s %s", s_resPath.c_str(), s_desPath.c_str());
  //  system(sCmd);
    
   // memset(sCmd, 0, cmdLen);
   // sprintf(sCmd, "cp -p -R %s %s", s_resPath.c_str(), s_tempPath.c_str());
    //system(sCmd);
#endif
}


void clearAndMakeDir()
{
	ImagePackage::myrddir(s_desPath);
	ImagePackage::myrddir(s_tempPath);
	ImagePackage::mymkdir(s_desPath);
	ImagePackage::mymkdir(s_tempPath);

	cpyDirs();
}

void initCmd(int argc, char *argv[])
{
	if (argc > 1)
	{
		for (int i = 1; i < argc ; i++)
		{
			std::string sCmd = argv[i];
			if (sCmd.find("-") != std::string::npos)
			{
				if (sCmd.compare(CMD_PROJECT_PATH) == 0 )
				{
					if (i+1 < argc)
					{
						s_projectPath = argv[i + 1];
					}
					s_cmdMap[CMD_PROJECT_PATH] = true;
					
				}
				else if (sCmd.compare(CMD_PACK_IMAGE) == 0)
				{
					s_cmdMap[CMD_PACK_IMAGE] = true;
				}
				else if (sCmd.compare(CMD_PACK_LUA) == 0)
				{
					s_cmdMap[CMD_PACK_LUA] = true;
				}
				else if (sCmd.compare(CMD_ENCODE_LUA) == 0)
				{
					s_cmdMap[CMD_ENCODE_LUA] = true;
				}
				else if (sCmd.compare(CMD_PACK_ALL_TO_ONE) == 0)
				{
					s_cmdMap[CMD_PACK_ALL_TO_ONE] = true;
				}

				
			}
		}
	}
}


int main(int argc,char *argv[])
{

	if (argc < 2)
	{
		printf("example: -projectPath d:\\projectRoot -packImage -packLua\n");
		return 0;
	}

	printf("init\n");
	initCmd(argc, argv);

	char temp[1024] = { 0 };
#if CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM
_getcwd(temp, 1024);
#else
   // char path[512];
    unsigned size = 512;
    _NSGetExecutablePath(temp, &size);
#endif
	


	s_resPath = s_projectPath+ XG_OBLIQUE_LINE+"res";
	s_desPath = s_projectPath + XG_OBLIQUE_LINE+ "out";
	s_srcPath = s_projectPath + XG_OBLIQUE_LINE+"src";
	s_tempPath = s_projectPath;
	s_tempPath = s_tempPath+XG_OBLIQUE_LINE+"packtemp";


	// clear
	printf("clear old\n");
	clearAndMakeDir();
	printf("make dir\n");

	{
		//pack res 
		auto it = s_cmdMap.find(CMD_PACK_IMAGE);
		if (it != s_cmdMap.end())
		{
			printf("pack img\n");
			auto pImagePackage = new ImagePackage();
			pImagePackage->main(s_resPath, s_desPath, s_tempPath);
			delete pImagePackage;
		}
		
	}

	
	printf("delete temp dir\n");
	ImagePackage::myrddir(s_tempPath);

	printf("pack end\n");
	
	//system("pause");
    
    return 0;
}