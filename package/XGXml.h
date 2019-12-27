
#ifndef __XG_XML_H__
#define __XG_XML_H__

#include "XGMacros.h"

class XGXml
{
protected:
	static const char* getValueForKey(xmlNodePtr pParent, const char* pKey);		
	static const char* getValueForNode(xmlNodePtr pNode);							
	static const char* getAttrForNode(xmlNodePtr pNode, const char* pAttr);			
	static void setValueForKey(xmlNodePtr pParent, const char* pKey, const char* value = "");
	static void setValueForNode(xmlNodePtr pNode, const char* value = "");
	static void setAttrForNode(xmlNodePtr pNode, const char* pAttr, const char* value = "");
	
public:
	static xmlNodePtr getXMLNodeForKey(xmlNodePtr pParent, const char* pKey);
	static xmlNodePtr getXMLFirstChildNode(xmlNodePtr pParent);
	static xmlNodePtr getBrotherXMLNodeForKey(xmlNodePtr pNode, const char* pKey);
	static bool hasChildren(xmlNodePtr pNode);
	static bool hasAttr(xmlNodePtr pNode, const char* pAttr);
	static bool hasContent(xmlNodePtr pNode);

	static bool getValueBool(xmlNodePtr pParent, const char* pKey, bool defaultValue = false);
	static bool getValueBool(xmlNodePtr pNode, bool defaultValue = false);
	static void setValueBool(xmlNodePtr pParent, const char* pKey, bool value);
	static void setValueBool(xmlNodePtr pNode, bool value);
	static bool getAttrBool(xmlNodePtr pParent, const char* pKey, bool defaultValue = false);
	static void setAttrBool(xmlNodePtr pParent, const char* pKey, bool value);

	static int getValueInteger(xmlNodePtr pParent, const char* pKey, int defaultValue = 0);
	static int getValueInteger(xmlNodePtr pNode, int defaultValue = 0);
	static void setValueInteger(xmlNodePtr pParent, const char* pKey, int value);
	static void setValueInteger(xmlNodePtr pNode, int value);
	static int getAttrInteger(xmlNodePtr pParent, const char* pKey, int defaultValue = 0);
	static void setAttrInteger(xmlNodePtr pParent, const char* pKey, int value);

	static float getValueFloat(xmlNodePtr pParent, const char* pKey, float defaultValue = 0.0f);
	static float getValueFloat(xmlNodePtr pNode, float defaultValue = 0.0f);
	static void setValueFloat(xmlNodePtr pParent, const char* pKey, float value);
	static void setValueFloat(xmlNodePtr pNode, float value);
	static float getAttrFloat(xmlNodePtr pParent, const char* pKey, float defaultValue = 0.0f);
	static void setAttrFloat(xmlNodePtr pParent, const char* pKey, float value);

	static double getValueDouble(xmlNodePtr pParent, const char* pKey, double defaultValue = 0.0);
	static double getValueDouble(xmlNodePtr pNode, double defaultValue = 0.0);
	static void setValueDouble(xmlNodePtr pParent, const char* pKey, double value);
	static void setValueDouble(xmlNodePtr pNode, double value);
	static double getAttrDouble(xmlNodePtr pParent, const char* pKey, double defaultValue = 0.0);
	static void setAttrDouble(xmlNodePtr pParent, const char* pKey, double value);
	
	static std::string getValueString(xmlNodePtr pParent, const char* pKey, std::string defaultValue = "");
	static std::string getValueString(xmlNodePtr pNode, std::string defaultValue = "");
	static void setValueString(xmlNodePtr pParent, const char* pKey, std::string value);
	static void setValueString(xmlNodePtr pNode, std::string value);
	static std::string getAttrString(xmlNodePtr pParent, const char* pKey, std::string defaultValue = "");
	static void setAttrString(xmlNodePtr pParent, const char* pKey, std::string value);

	static std::vector<int> getValueVecInteger(xmlNodePtr pParent, const char* pKey);
};

#endif	// __VX_XML_H__