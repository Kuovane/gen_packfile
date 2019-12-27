#include "XGXml.h"
#include "XGConvert.h"

xmlNodePtr XGXml::getXMLNodeForKey(xmlNodePtr pParent, const char* pKey)
{
	xmlNodePtr pCurNode = NULL;
	do 
	{
		if(!pParent || !pKey)
		{
			break;
		}

		pCurNode = pParent->FirstChildElement();

		while (pCurNode)
		{
			//if (pCurNode->type == XML_ELEMENT_NODE
			if (!strcmp(pCurNode->Name(), pKey))
			{
				break;
			}
			pCurNode = pCurNode->NextSiblingElement();
		}

	} while (0);

	return pCurNode;
}

xmlNodePtr XGXml::getXMLFirstChildNode(xmlNodePtr pParent)
{
	xmlNodePtr pCurNode = NULL;
	do 
	{
		if(!pParent)
		{
			break;
		}

		pCurNode = pParent->FirstChildElement();
#if 0
		while (pCurNode)
		{
			//if (pCurNode->type == XML_ELEMENT_NODE)
			{
				//break;
			}
			//pCurNode = pCurNode->next;
		}
#endif

	} while (0);

	return pCurNode;
}

xmlNodePtr XGXml::getBrotherXMLNodeForKey(xmlNodePtr pNode, const char* pKey)
{
	xmlNodePtr pCurNode = NULL;
	do 
	{
		if(!pNode || !pKey)
		{
			break;
		}

		pCurNode = pNode;

		while(pCurNode)
		{
			//if (pCurNode->type == XML_ELEMENT_NODE
			if(!strcmp(pCurNode->Name(), pKey))
			{
				break;
			}
			pCurNode = pCurNode->NextSiblingElement();
		}

	} while (0);

	return pCurNode;
}

const char* XGXml::getValueForKey(xmlNodePtr pParent, const char* pKey)
{
	xmlNodePtr pNode = XGXml::getXMLNodeForKey(pParent, pKey);
	return XGXml::getValueForNode(pNode);
}

const char* XGXml::getValueForNode(xmlNodePtr pNode)
{
	const char* pRet = NULL;
	if(pNode)
	{
		pRet = (const char*)pNode->GetText();//xmlNodeGetContent(pNode);
	}
	return pRet;
}

const char* XGXml::getAttrForNode(xmlNodePtr pNode, const char* pAttr)
{
	const char* pRet = NULL;
	if(pNode)
	{
		pRet = (const char*)pNode->Attribute(pAttr);//xmlGetProp(pNode, BAD_CAST pAttr);
	}
	return pRet;
}

void XGXml::setValueForKey(xmlNodePtr pParent, const char* pKey, const char* value)
{
	xmlNodePtr pNode = XGXml::getXMLNodeForKey(pParent, pKey);
	if(pNode)
	{
		return XGXml::setValueForNode(pNode, value);
	}
	else if(pParent && pKey)
	{

		xmlNodePtr pNewNode = pParent->GetDocument()->NewElement(pKey);
		pNewNode->LinkEndChild(pParent->GetDocument()->NewText(value));
		//xmlNodePtr pNewNode = xmlNewNode(NULL, BAD_CAST pKey);
		//xmlNodePtr pNewNodeContent = xmlNewText(BAD_CAST value);
		//xmlAddChild(pParent, pNewNode);
		//xmlAddChild(pNewNode, pNewNodeContent);
	}
}

void XGXml::setValueForNode(xmlNodePtr pNode, const char* value)
{
	if(pNode)
	{
		//xmlNodeSetContent(pNode, BAD_CAST value);
		if(pNode->FirstChild())
		{
			pNode->FirstChild()->SetValue(value);
		}
		else
		{
			pNode->LinkEndChild(pNode->GetDocument()->NewText(value));
		}
		
	}
}

void XGXml::setAttrForNode(xmlNodePtr pNode, const char* pAttr, const char* value)
{
	if(pNode)
	{
		pNode->SetAttribute(pAttr,value);
#if 0
		if(pNode->Attribute(pAttr))//xmlHasProp(pNode, BAD_CAST pAttr))
		{
			//xmlSetProp(pNode, BAD_CAST pAttr, BAD_CAST value);
		}
		else
		{
			//xmlNewProp(pNode, BAD_CAST pAttr, BAD_CAST value);
		}
#endif
	}
}

bool XGXml::hasChildren(xmlNodePtr pNode)
{
	XG_RETURN_VALUE_IF(!pNode,false);

	xmlNodePtr pChild = pNode->FirstChildElement();
	while(pChild)
	{
		//if(pChild->type == XML_ELEMENT_NODE)
		//{
			return true;
		//}
		//pChild = pChild->next;
	}
	return false;
}

bool XGXml::hasAttr(xmlNodePtr pNode, const char* pAttr)
{
	return (pNode && NULL != pNode->Attribute(pAttr));//xmlHasProp(pNode, BAD_CAST pAttr));
}

bool XGXml::hasContent(xmlNodePtr pNode)
{
	XG_RETURN_VALUE_IF(!pNode, false);
	//xmlNodePtr pChild = pNode->FirstChildElement();
	while(pNode->GetText())
	{
		return true;
	}
	return false;

}

bool XGXml::getValueBool(xmlNodePtr pParent, const char* pKey, bool defaultValue)
{
	xmlNodePtr pNode = XGXml::getXMLNodeForKey(pParent, pKey);
	return XGXml::getValueBool(pNode, defaultValue);
}

bool XGXml::getValueBool(xmlNodePtr pNode, bool defaultValue)
{
	const char* value = XGXml::getValueForNode(pNode);
	bool ret = defaultValue;
	if (value)
	{
		ret = XGConvert::stringToBool(value);
		//xmlFree((void*)value);
	}
	return ret;
}

void XGXml::setValueBool(xmlNodePtr pParent, const char* pKey, bool value)
{
	if(value)
	{
		XGXml::setValueForKey(pParent, pKey, "true");
	}
	else
	{
		XGXml::setValueForKey(pParent, pKey, "false");
	}
}

void XGXml::setValueBool(xmlNodePtr pNode, bool value)
{
	if(value)
	{
		XGXml::setValueForNode(pNode, "true");
	}
	else
	{
		XGXml::setValueForNode(pNode, "false");
	}
}

int XGXml::getValueInteger(xmlNodePtr pParent, const char* pKey, int defaultValue)
{
	xmlNodePtr pNode = XGXml::getXMLNodeForKey(pParent, pKey);
	return XGXml::getValueInteger(pNode, defaultValue);
}

int XGXml::getValueInteger(xmlNodePtr pNode, int defaultValue)
{
	const char* value = XGXml::getValueForNode(pNode);
	int ret = defaultValue;
	if (value)
	{
		ret = XGConvert::stringToInteger(value);
		//xmlFree((void*)value);
	}
	return ret;
}

void XGXml::setValueInteger(xmlNodePtr pParent, const char* pKey, int value)
{
	char pTmp[32];
	XGConvert::integerToChars(value, pTmp);
	XGXml::setValueForKey(pParent, pKey, pTmp);
}

void XGXml::setValueInteger(xmlNodePtr pNode, int value)
{
	char pTmp[32];
	XGConvert::integerToChars(value, pTmp);
	XGXml::setValueForNode(pNode, pTmp);
}

float XGXml::getValueFloat(xmlNodePtr pParent, const char* pKey, float defaultValue)
{
	xmlNodePtr pNode = XGXml::getXMLNodeForKey(pParent, pKey);
	return XGXml::getValueFloat(pNode, defaultValue);
}

float XGXml::getValueFloat(xmlNodePtr pNode, float defaultValue)
{
	const char* value = XGXml::getValueForNode(pNode);
	float ret = defaultValue;
	if (value)
	{
		ret = XGConvert::stringToFloat(value);
	//	xmlFree((void*)value);
	}
	return ret;
}

void XGXml::setValueFloat(xmlNodePtr pParent, const char* pKey, float value)
{
	char pTmp[32];
	XGConvert::floatToChars(value, pTmp);
	XGXml::setValueForKey(pParent, pKey, pTmp);
}

void XGXml::setValueFloat(xmlNodePtr pNode, float value)
{
	char pTmp[32];
	XGConvert::floatToChars(value, pTmp);
	XGXml::setValueForNode(pNode, pTmp);
}

double XGXml::getValueDouble(xmlNodePtr pParent, const char* pKey, double defaultValue)
{
	xmlNodePtr pNode = XGXml::getXMLNodeForKey(pParent, pKey);
	return XGXml::getValueDouble(pNode, defaultValue);
}

double XGXml::getValueDouble(xmlNodePtr pNode, double defaultValue)
{
	const char* value = XGXml::getValueForNode(pNode);
	double ret = defaultValue;
	if (value)
	{
		ret = XGConvert::stringToDouble(value);
		//xmlFree((void*)value);
	}
	return ret;
}

void XGXml::setValueDouble(xmlNodePtr pParent, const char* pKey, double value)
{
	char pTmp[32];
	XGConvert::doubleToChars(value, pTmp);
	XGXml::setValueForKey(pParent, pKey, pTmp);
}

void XGXml::setValueDouble(xmlNodePtr pNode, double value)
{
	char pTmp[32];
	XGConvert::doubleToChars(value, pTmp);
	XGXml::setValueForNode(pNode, pTmp);
}

std::string XGXml::getValueString(xmlNodePtr pParent, const char* pKey, std::string defaultValue)
{
	xmlNodePtr pNode = XGXml::getXMLNodeForKey(pParent, pKey);
	return XGXml::getValueString(pNode, defaultValue);
}

std::string XGXml::getValueString(xmlNodePtr pNode, std::string defaultValue)
{
	const char* value = XGXml::getValueForNode(pNode);
	std::string ret = defaultValue;
	if (value)
	{
		ret = value;
		//xmlFree((void*)value);
	}
	return ret;
}

void XGXml::setValueString(xmlNodePtr pParent, const char* pKey, std::string value)
{
	XGXml::setValueForKey(pParent, pKey, value.c_str());
}

void XGXml::setValueString(xmlNodePtr pNode, std::string value)
{
	XGXml::setValueForNode(pNode, value.c_str());
}


bool XGXml::getAttrBool(xmlNodePtr pParent, const char* pKey, bool defaultValue)
{
	const char* value = getAttrForNode(pParent, pKey);
	bool ret = defaultValue;
	if(value)
	{
		ret = XGConvert::stringToBool(value);
		//xmlFree((void*)value);
	}
	return ret;
}

void XGXml::setAttrBool(xmlNodePtr pParent, const char* pKey, bool value)
{
	if(value)
	{
		setAttrForNode(pParent, pKey, "true");
	}
	else
	{
		setAttrForNode(pParent, pKey, "false");
	}
}

int XGXml::getAttrInteger(xmlNodePtr pParent, const char* pKey, int defaultValue)
{
	const char* value = (const char*)pParent?pParent->Attribute(pKey):NULL;//xmlGetProp(pParent, BAD_CAST pKey);
	int ret = defaultValue;
	if(value)
	{
		ret = XGConvert::stringToInteger(value);
		//xmlFree((void*)value);
	}
	return ret;
}

void XGXml::setAttrInteger(xmlNodePtr pParent, const char* pKey, int value)
{
	char tmp[32];
	XGConvert::integerToChars(value, tmp);
	setAttrForNode(pParent, pKey, tmp);
}

float XGXml::getAttrFloat(xmlNodePtr pParent, const char* pKey, float defaultValue)
{
	const char* value = getAttrForNode(pParent, pKey);
	float ret = defaultValue;
	if(value)
	{
		ret = XGConvert::stringToFloat(value);
	//	xmlFree((void*)value);
	}
	return ret;
}

void XGXml::setAttrFloat(xmlNodePtr pParent, const char* pKey, float value)
{
	char tmp[32];
	XGConvert::floatToChars(value, tmp);
	setAttrForNode(pParent, pKey, tmp);
}

double XGXml::getAttrDouble(xmlNodePtr pParent, const char* pKey, double defaultValue)
{
	const char* value = getAttrForNode(pParent, pKey);
	double ret = defaultValue;
	if(value)
	{
		ret = XGConvert::stringToDouble(value);
	//	xmlFree((void*)value);
	}
	return ret;
}

void XGXml::setAttrDouble(xmlNodePtr pParent, const char* pKey, double value)
{
	char tmp[32];
	XGConvert::doubleToChars(value, tmp);
	setAttrForNode(pParent, pKey, tmp);
}

std::string XGXml::getAttrString(xmlNodePtr pParent, const char* pKey, std::string defaultValue)
{
	const char* value = getAttrForNode(pParent, pKey);
	if(value)
	{
		std::string ret = value;
	//	xmlFree((void*)value);
		return ret;
	}
	else
	{
		return defaultValue;
	}
}

void XGXml::setAttrString(xmlNodePtr pParent, const char* pKey, std::string value)
{
	setAttrForNode(pParent, pKey, value.c_str());
}


std::vector<int> XGXml::getValueVecInteger(xmlNodePtr pParent, const char* pKey)
{
	std::vector<int> sRet;

	xmlNodePtr pCurNode = NULL;
	do 
	{
		if(!pParent || !pKey)
		{
			break;
		}

		pCurNode = pParent->FirstChildElement();

		while (pCurNode)
		{
			//if (pCurNode->type == XML_ELEMENT_NODE
			if(!strcmp(pCurNode->Name(), pKey))
			{
				sRet.push_back(XGXml::getValueInteger(pCurNode));
			}
			pCurNode = pCurNode->NextSiblingElement();
		}

	} while (0);

	return sRet;
}
