/******************************************************
Author: KuovaneWu
2018-12-26

************************************************************/


#include "XGMacros.h"

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
#include <assert.h>
#include "XGXml.h"
#include "XGConvert.h"
#include <map>
#include "ImagePackage.h"





ImagePackage::ImagePackage()
{
	 m_pPackBuff = nullptr;
	 m_totalLen = 0;
}
void ImagePackage::readUnpackImageFiles()
{

	char temp[1024] = { 0 };
    #if CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM
        _getcwd(temp, 1024);
    #else
    // char path[512];
        unsigned size = 512;
        _NSGetExecutablePath(temp, &size);
    
    std::string path =temp;
    auto index = path.rfind("./");
    auto sCmd = path.substr(0,index);
    
    //temp =sCmd;
    memcpy(temp, sCmd.c_str(),index);
    temp[index] = 0;
    //printf("readUnpackImageFiles %s\n",temp);
    #endif

    
    std::string filePath = temp;
	filePath = filePath + XG_OBLIQUE_LINE+"unpackImages.xml";
	tinyxml2::XMLDocument sXmlDoc;
	sXmlDoc.LoadFile(filePath.c_str());


	xmlNodePtr pRoot = sXmlDoc.RootElement();

	if (pRoot)
	{
		m_isUnusePack = XGXml::getAttrBool(pRoot, "isUnusePack");
		
		VX_FOR_EACH_XML_NAMED_CHILDREN(pChannel, pRoot, "file")
		{
			std::string sFile = XG_OBLIQUE_LINE + XGXml::getAttrString(pChannel, "name");
			
			XGConvert::replace_str(sFile, "/", XG_OBLIQUE_LINE);


			sFile = s_sFilePathPre + sFile;
			s_unpackImageDataes[sFile] = UnpackImageData(sFile);
		}
	}
	else
	{
		printf("read unpackImages.xml fail!\n");
	}
}





void ImagePackage::getImageSize(std::string filepath,int &width,int &height)
{
	auto pos = filepath.find(".png");
	if (pos == std::string::npos)
	{
		return;
	}

	FILE *pFile = fopen(filepath.c_str(), "rb+");
	if (pFile)
	{
		char pBuf[64] = { 0 };
		fseek(pFile, 0, SEEK_SET);
		fread(pBuf, 32, 1, pFile);
		memcpy(&width, pBuf + 16, 4);
		memcpy(&height, pBuf + 20, 4);

		width = XGConvert::NF_NTOHL(width);
		height = XGConvert::NF_NTOHL(height);
		//printf(" %d,%d \n", width, height);
		fclose(pFile);
	}
}


void ImagePackage::getImageSize(char* pBuf, int &width, int &height)
{
	memcpy(&width, pBuf + 16, 4);
	memcpy(&height, pBuf + 20, 4);

	width = XGConvert::NF_NTOHL(width);
	height = XGConvert::NF_NTOHL(height);
}


std::string  ImagePackage::getRelativePath(std::string &path)
{
	//std::string sTemp = path; ;
	auto pos = path.find("creator/");
	if (pos == std::string::npos)
	{
		return path;
	}
	else
	{
		return path.substr(pos);
	}
}

bool ImagePackage::isSmallFile(std::string &path)
{
	if (path.find(".xml") != std::string::npos)
	{
		return true;
	}
	if (path.find(".plist") != std::string::npos)
	{
		return true;
	}
	if (path.find(".ccreator") != std::string::npos)
	{
		return true;
	}
	if (path.find(".atlas") != std::string::npos)
	{
		return true;
	}

	if (path.find(".json") != std::string::npos)
	{
		return true;
	}
	
	return false;
}

bool ImagePackage::isCreatorFile(std::string &path)
{
	if (path.find(".ccreator") != std::string::npos)
	{
		return true;
	}

	return false;
}



void ImagePackage::loadAllFiles()
{
    printf("loadAllFiles getFiles\n");
	XGConvert::getFiles(s_sFilePathPre, s_allFiles, s_allFileNames);
     printf("loadAllFiles readAllFiles\n");
	//readAllFiles();
    printf("loadAllFiles findPackageFiles\n");
	//findPackageFiles();
    

}


void ImagePackage::mymkdir(std::string sDir)
{
	const int cmdLen = 500;
	char sCmd[cmdLen] = { 0 };
#if CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM
    sprintf(sCmd, "if not exist %s mkdir %s", sDir.c_str(), sDir.c_str());
    system(sCmd);
#else
    sprintf(sCmd, "mkdir %s", sDir.c_str(), sDir.c_str());
    system(sCmd);
#endif
}

void ImagePackage::myrddir(std::string sDir)
{
	const int cmdLen = 500;
	char sCmd[cmdLen] = { 0 };
#if CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM
    sprintf(sCmd, "rd /s /q %s", sDir.c_str());
#else
    sprintf(sCmd, "rm -rf %s", sDir.c_str());
#endif
	
	system(sCmd);

}


void ImagePackage::saveFile(std::string filePath,char* pBuffData,int len)
{
    
	auto index = filePath.rfind(XG_OBLIQUE_LINE);
	auto sTemp = filePath.substr(0, index + 1);
	XGConvert::createDirEx(sTemp.c_str());

    
	FILE *pFile = fopen(filePath.c_str(), "wb+");
	if (pFile)
	{

		fwrite(pBuffData, len, 1, pFile);

		fclose(pFile);
	}

}

std::string ImagePackage::getTempFilePath(std::string filePath)
{

	filePath = XGConvert::replace_str(filePath, s_sFilePathPre, s_sFileTempPath);
	return filePath;
}


std::string ImagePackage::getOutFilePath(std::string filePath)
{

	filePath = XGConvert::replace_str(filePath, s_sFilePathPre, s_sDesPath);
	return filePath;
}


static bool isIgnore(std::string &sName)
{
	if (sName.find("creator/") == std::string::npos || sName.empty())
	{
		return true;
	}
	return false;
}

void ImagePackage::pushSceneName(std::string &sName)
{
	for (auto it = m_sceneList.begin(); it != m_sceneList.end(); it++)
	{
		if (*it == sName)
		{
			m_sceneList.erase(it);
			break;
		}
	}
	m_sceneList.push_back(sName);
	m_sCurName = sName;
}

void ImagePackage::removeSceneName(std::string &sName)
{
	for (auto it = m_sceneList.begin(); it != m_sceneList.end(); it++)
	{
		if (*it == sName)
		{
			m_sceneList.erase(it);
			break;
		}
	}
	if (!m_sceneList.empty())
	{
		auto p = m_sceneList.back();
		m_sCurName = p;
	}
	else
	{
		m_sCurName = "";
	}
	
}


void ImagePackage::pushPopupName(std::string &sName)
{
	for (auto it = m_popupList.begin(); it != m_popupList.end(); it++)
	{
		if (*it == sName)
		{
			m_popupList.erase(it);
			break;
		}
	}
	m_popupList.push_back(sName);
	m_sCurName = sName;
}

void ImagePackage::removePopupName(std::string &sName)
{
	for (auto it = m_popupList.begin(); it != m_popupList.end(); it++)
	{
		if (*it == sName)
		{
			m_popupList.erase(it);
			break;
		}
	}
	if (!m_sceneList.empty())
	{
		auto p = m_sceneList.back();
		m_sCurName = p;
	}
	else
	{
		m_sCurName = "";
	}
}

void ImagePackage::pushName(std::string &sName)
{
	if (sName.find("*#Scene:ctor") != std::string::npos)
	{
		auto sceneName = sName.substr(13);
		pushSceneName(sceneName);
		//sName.substr()
	}
	else if (sName.find("*#Scene:onEnter") != std::string::npos)
	{
		auto sceneName = sName.substr(16);
		pushSceneName(sceneName);
	}
	else if (sName.find("*#Scene:onExit") != std::string::npos)
	{
		auto sceneName = sName.substr(15);
		removeSceneName(sName);
	}
	else if (sName.find("*#Scene:onCleanup") != std::string::npos)
	{
		auto sceneName = sName.substr(18);
		removeSceneName(sName);
	}
	else if (sName.find("*#Popup:ctor") != std::string::npos)
	{
		auto popName = sName.substr(13);
		pushPopupName(popName);
	}
	else if (sName.find("*#Popup:onEnter") != std::string::npos)
	{
		auto popName = sName.substr(16);
		pushPopupName(popName);
	}
	else if (sName.find("*#Popup:onExit") != std::string::npos)
	{
		auto popName = sName.substr(15);
		removePopupName(popName);
	}
	else if (sName.find("*#Popup:onCleanup") != std::string::npos)
	{
		auto popName = sName.substr(18);
		removePopupName(popName);
	}
	else
	{
		if (!isIgnore(sName))
		{
			if (!m_sCurName.empty())
			{
				auto it = m_scenesFile.find(m_sCurName);
				if (it == m_scenesFile.end())
				{
					std::map<std::string, MyFileData*> t;
					m_scenesFile[m_sCurName] = t;
				}


				auto itMap = s_mapFileDataes.find(sName);
				if (itMap == s_mapFileDataes.end())
				{
					s_mapFileDataes[sName] = new MyFileData(m_sCurName);// m_scenesFile[m_sCurName][sName];
				}
				s_mapFileDataes[sName]->m_sPackNames[m_sCurName] = true;


				auto itFile = m_scenesFile[m_sCurName].find(sName);
				if (itFile == m_scenesFile[m_sCurName].end())
				{
					m_scenesFile[m_sCurName][sName] = s_mapFileDataes[sName];
				}	
			}
			
		}
	}
}

void ImagePackage::readOrderFilePathes()
{
	bool isIgnoreSize = false;
	auto fileName = "filePath.txt";
	FILE *pFile = fopen(fileName, "r");
	if (pFile)
	{
		fseek(pFile, 0, SEEK_END);
		int len = ftell(pFile);



		fseek(pFile, 0, SEEK_SET);

		char* pBuf = new char[len + 4];

		fread(pBuf, len, 1, pFile);
		std::string sFileInfo = pBuf;
		//sFileInfo = sFileInfo + "\n";

		for (int nStart = 0; nStart < sFileInfo.length(); )
		{
			int nPos = sFileInfo.find('\n', nStart);
			if (nPos == std::string::npos)
			{
				nPos = sFileInfo.length() - 1;
				//break;
			}

			//char pTmp[100] = { 0 };
			std::string sTmp;
			int nLen = 0;

			auto name = sFileInfo.substr(nStart, nPos - nStart);
			//XGConvert::replace_str(name, "/", XG_OBLIQUE_LINE);
			name = getRelativePath(name);
			
			pushName(name);
			

			nStart = nPos + 1;
		}
		fclose(pFile);
	}
	{
		std::map<std::string,bool> sCommonFiles;
		auto path = "pathOut.txt";
		FILE *pFile = fopen(path, "w");
		if (pFile)
		{
			for (auto it = m_scenesFile.begin(); it != m_scenesFile.end(); ++it)
			{

				std::string temp = "#*pack -ignoreSize\n";

				fwrite(temp.c_str(), temp.length(), 1, pFile);


				for (auto itPack = it->second.begin(); itPack != it->second.end(); ++itPack)
				{
					if (itPack->second->m_sPackNames.size() > 1)
					{
						sCommonFiles[itPack->first]=true;
					}
					else
					{
						auto temp = itPack->first + "\n";
						fwrite(temp.c_str(), temp.length(), 1, pFile);
					}
					
				}
				temp = "#*pack -name " + it->first+"/"+ it->first +"\n";
				fwrite(temp.c_str(), temp.length(), 1, pFile);
			
			}

			std::string temp = "#*pack -ignoreSize\n";
			fwrite(temp.c_str(), temp.length(), 1, pFile);

			for (auto it = sCommonFiles.begin(); it != sCommonFiles.end(); it++)
			{
				temp = it->first + "\n";
				fwrite(temp.c_str(), temp.length(), 1, pFile);
			}
			temp = "#*pack -name common\n";
			fwrite(temp.c_str(), temp.length(), 1, pFile);

			fclose(pFile);
		}
	}

}

void ImagePackage::doPackageOrderPng(int count, std::string sName)
{
	//texture pacakge
	auto name = "order" + XGConvert::integerToString(count);
	
	const int cmdLen = 500;
	char sCmd[cmdLen] = { 0 };


#if CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM
	if (!sName.empty())
	{
	//	auto desPath  = s_sDesPath
	//	auto index = filePath.rfind(XG_OBLIQUE_LINE);
	//	auto sTemp = filePath.substr(0, index + 1);
	//	XGConvert::createDirEx(sTemp.c_str());

		name = sName;
		sprintf(sCmd, "TexturePacker --trim-mode None --max-size 2048 --shape-padding 2 --disable-rotation --multipack  --data  %s\\%s{n}.plist --format cocos2d --sheet %s\\%s{n}.png %s", s_sDesPath.c_str(), name.c_str(), s_sDesPath.c_str(), name.c_str(), s_sFileTempPath.c_str());
	}
	else{
		sprintf(sCmd, "TexturePacker --trim-mode None --max-size 2048 --shape-padding 2 --disable-rotation --multipack  --data  %s\\%s.plist --format cocos2d --sheet %s\\%s.png %s", s_sDesPath.c_str(), name.c_str(), s_sDesPath.c_str(), name.c_str(), s_sFileTempPath.c_str());
		
	}
	
#else
	sprintf(sCmd, "/usr/local/bin/TexturePacker --trim-mode None --shape-padding 2 --disable-rotation --multipack --data  %s/tex{n}.plist --format cocos2d --sheet %s/tex{n}.png %s", s_sDesPath.c_str(), s_sDesPath.c_str(), s_sFileTempPath.c_str());
#endif
	system(sCmd);

	ImagePackage::myrddir(s_sFileTempPath);
	ImagePackage::mymkdir(s_sFileTempPath);
}

void ImagePackage::packagePng()
{
	readOrderFilePathes();
}


void ImagePackage::initPath(std::string &srcDir, std::string &desDir, std::string &tempDir)
{



	s_sFilePathPre = srcDir;
	s_sDesPath = desDir;
	s_sFileTempPath = tempDir;


}



void ImagePackage::saveFilesData()
{
	auto path = s_sDesPath + XG_OBLIQUE_LINE+"resdata";
	FILE *pFile = fopen(path.c_str(), "wb");
	if (pFile)
	{

		fwrite(m_pPackBuff, m_totalLen, 1, pFile);

		fclose(pFile);
	}
}



void ImagePackage::dealAllRes()
{

    printf("ImagePackage packagePng\n");
	packagePng();
    printf("ImagePackage saveUnpackFiles\n");
}




void ImagePackage::main(std::string srcDir, std::string desDir, std::string tempDir)
{

	initPath(srcDir,desDir,tempDir);
	//debug end

	dealAllRes();
}