#include "XGMacros.h"

#include <assert.h>
#include "XGXml.h"
#include "XGConvert.h"
#include <map>
#include <list>

#if CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM
#include <windows.h>
#endif

class UnpackImageData
{
public:
	UnpackImageData(std::string sImageName = "")
	{
		m_sImage = sImageName;
	}
	std::string m_sImage;

};


class MyFileData {
public:
	MyFileData(std::string& sName) {
		m_sPackNames[sName] = true;
	}
	std::map<std::string, bool> m_sPackNames;
};


class ImagePackage
{
public:
	ImagePackage();
	void readUnpackImageFiles();
	void getImageSize(std::string filepath, int &width, int &height);




	void getImageSize(char* pBuf, int &width, int &height);
	std::string  getRelativePath(std::string &path);
	bool isSmallFile(std::string &path);
	bool isCreatorFile(std::string &path);

	void readAllFiles();

	void findPackageFiles();

	void loadAllFiles();

	static void mymkdir(std::string sDir);

	static void myrddir(std::string sDir);
	static void saveFile(std::string filePath, char* pBuffData, int len);

	std::string getTempFilePath(std::string filePath);

	std::string getOutFilePath(std::string filePath);


	void pushSceneName(std::string &sName);
	void removeSceneName(std::string &sName);

	void pushPopupName(std::string &sName);
	void removePopupName(std::string &sName);

	void pushName(std::string &sName);
	void readOrderFilePathes();

	void doPackageOrderPng(int count, std::string sName = "");

	void packagePng();


	void initPath(std::string &srcDir, std::string &desDir, std::string &tempDir);



	void saveFilesData();


	void dealAllRes();
	void main(std::string srcDir, std::string desDir,std::string tempDir);

public :
	 std::string s_sFilePathPre;
	 std::string s_sDesPath;
	 std::string s_sFileTempPath;
	 std::string s_sCurPath;

	 std::list<std::string> m_sceneList;
	 std::list<std::string> m_popupList;
	 std::map<std::string, std::map<std::string , MyFileData*> > m_scenesFile;
	 std::string m_sCurSceneName;
	 std::string m_sCurPopupName;
	 std::string m_sCurName;

	 std::vector<std::string> s_allFiles;
	 std::vector<std::string> s_allFileNames;

	 std::vector<MyFileData*> s_fileOrderFileDataes;

	 std::map<std::string,MyFileData *> s_mapFileDataes;
	 std::map<std::string, UnpackImageData> s_unpackImageDataes;

	 char* m_pPackBuff;
	 int m_totalLen;
	 bool m_isUnusePack = false;
};