#pragma once
#include "../../Engine.h"
#include "rapidxml.hpp"

struct DialogTextInfo 
{
	std::string DialogName;
	std::vector<std::wstring> vecTalker;
	std::vector<std::wstring> vecText;
	std::vector<std::wstring> vecAnim;
};

struct DialogWriteInfo
{
	std::string talker;
	std::string text;
	std::string anim;
};

class CXmlParser
{
private :
	rapidxml::xml_document<>* m_XmlDoc;
	rapidxml::xml_document<>* m_Xmlseg;
	std::vector<DialogTextInfo> m_vecDialTextInfo;
public :
	CXmlParser();
	~CXmlParser();
	void WriteFile(const std::string& str,const std::string& file= "Chat.xml");
	void AddFile(const std::string& str, const std::string& file = "Chat.xml");
	void ReadFile(const std::string& _strFile);
	int GetDialogCount();
	std::string GetDialogInfoByIndex(int Idx, DialogInfo* Info) const;
};

