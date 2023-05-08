#include "CXmlParser.h"
#include "../PathManager.h"
#include "rapidxml_print.hpp"
#include <fstream>
#include <sstream>

using namespace rapidxml;

CXmlParser::CXmlParser()
{
	m_XmlDoc = new rapidxml::xml_document<>();
	m_Xmlseg = new rapidxml::xml_document<>();
}

CXmlParser::~CXmlParser()
{
	SAFE_DELETE(m_XmlDoc)
	SAFE_DELETE(m_Xmlseg)
}

void CXmlParser::WriteFile(const std::string& str, const std::string& file)
{
	const PathInfo* path = CPathManager::GetInst()->FindPath(DIALOG_PATH);
	std::string strFilePath = path->pathMultibyte;
	strFilePath.append(file);

	xml_node<>* decl = m_XmlDoc->allocate_node(node_declaration);
	decl->append_attribute(m_XmlDoc->allocate_attribute("version", "1.0"));
	decl->append_attribute(m_XmlDoc->allocate_attribute("encoding", "utf-8"));
	m_XmlDoc->append_node(decl);

	xml_node<>* root = m_XmlDoc->allocate_node(node_element, "Dialogs");
	m_XmlDoc->append_node(root);

	xml_node<>* Dialog = m_XmlDoc->allocate_node(node_element, "Dialog");
	Dialog->append_attribute(m_XmlDoc->allocate_attribute("Name", "First_Contact"));
	root->append_node(Dialog);

	xml_node<>* Context = m_XmlDoc->allocate_node(node_element, "Context");
	Dialog->append_node(Context);

	xml_node<>* child = m_XmlDoc->allocate_node(node_element, "Talker");
	Context->append_node(child);
	child = m_XmlDoc->allocate_node(node_element, "Text");
	Context->append_node(child);
	child = m_XmlDoc->allocate_node(node_element, "Anim");
	Context->append_node(child);

	// Convert doc to string if needed
	//std::string xml_as_string;
	//rapidxml::print(std::back_inserter(xml_as_string), m_XmlDoc);

	// Save to file
	std::ofstream file_stored(strFilePath);
	if (!file_stored.is_open())
	{
		return; //the file didn't open
	}
	file_stored << *m_XmlDoc;
	file_stored.close();
	m_XmlDoc->clear();
}

void CXmlParser::AddFile(const std::string& str, const std::string& file)
{
	const PathInfo* Path = CPathManager::GetInst()->FindPath(DIALOG_PATH);
	std::string strFilePath = Path->pathMultibyte;
	strFilePath.append(file);

	std::basic_ifstream<char> xmlFile(strFilePath);
	xmlFile.seekg(0, std::ios::end);
	unsigned int size = (unsigned int)xmlFile.tellg();
	xmlFile.seekg(0);

	std::vector<char> xmlData(size + 1);
	xmlData[size] = 0;
	xmlFile.read(&xmlData.front(), (std::streamsize)size);
	m_XmlDoc->parse<parse_default>(&xmlData.front());
	xml_node<char>* pRootNode = m_XmlDoc->first_node();

	xml_node<>* Context = m_Xmlseg->allocate_node(node_element, "Context");
	m_Xmlseg->append_node(Context);
	xml_node<>* child = m_Xmlseg->allocate_node(node_element, "Talker");
	Context->append_node(child);
	child = m_Xmlseg->allocate_node(node_element, "Text");
	Context->append_node(child);
	child = m_Xmlseg->allocate_node(node_element, "Anim");
	Context->append_node(child);

	for (xml_node<char>* item = pRootNode->first_node(); item; item = item->next_sibling())
	{
		if (strcmp("Dialog", item->name()) == 0)
		{
			//DialogTextInfo Info;
			xml_node<>* a = m_Xmlseg->first_node(); /* Node to append */
			xml_node<>* node = m_XmlDoc->clone_node(a);
			item->append_node(node);
		}
	}

	std::ofstream file_stored(strFilePath);
	if (!file_stored.is_open())
	{
		return; //the file didn't open
	}
	file_stored << m_XmlDoc->value();
	//file_stored.write(*m_XmlDoc, 10);    //파일에 문자열 쓰기
	xmlFile.close();
	m_XmlDoc->clear();
	m_Xmlseg->clear();
}

void CXmlParser::ReadFile(const std::string& _strFile)
{
	const PathInfo* Path = CPathManager::GetInst()->FindPath(DIALOG_PATH);
	//std::wstring wPath = Path->path;
	
	std::string strFilePath = Path->pathMultibyte;
	//std::string strFilePath(wPath.begin(), wPath.end());
	strFilePath.append(_strFile);

	std::basic_ifstream<char> xmlFile(strFilePath);
	xmlFile.seekg(0, std::ios::end);
	unsigned int size = (unsigned int)xmlFile.tellg();
	xmlFile.seekg(0);

	std::vector<char> xmlData(size + 1);
	xmlData[size] = 0;
	xmlFile.read(&xmlData.front(), (std::streamsize)size);

	xml_document<>* m_XmlDoc = new rapidxml::xml_document<>();
	m_XmlDoc->parse<parse_default>(&xmlData.front());

	xml_node<char>* pRootNode = m_XmlDoc->first_node();
	xml_attribute<char>* attr;

	for (xml_node<char>* item = pRootNode->first_node(); item; item = item->next_sibling()) 
	{
		if (strcmp("Dialog", item->name()) == 0) 
		{
			DialogTextInfo Info;

			for (attr = item->first_attribute(); attr; attr = attr->next_attribute()) 
			{
				if (strcmp("Name", attr->name()) == 0) 
				{
					Info.DialogName = attr->value();
				}
			}

			for (xml_node<char>* subItem = item->first_node(); subItem; subItem = subItem->next_sibling())
			{
				for (xml_node<char>* contextItem = subItem->first_node(); contextItem; 
					contextItem = contextItem->next_sibling()) 
				{
					if (strcmp("Talker", contextItem->name()) == 0) 
					{
						char* value = contextItem->value();

						int rq_cch = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, value, 
							static_cast<int>(strlen(value)), nullptr , 0);

						std::wstring wValue;
						wValue.clear();
						wValue.resize(rq_cch);

						MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, value,
							static_cast<int>(strlen(value)),
							const_cast<wchar_t*>(wValue.c_str()),
							static_cast<int>(wValue.size()));

						Info.vecTalker.push_back(wValue);
					}

					if (strcmp("Text", contextItem->name()) == 0) {
						char* value = contextItem->value();

						int rq_cch = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, value, 
							static_cast<int>(strlen(value)), nullptr, 0);

						std::wstring wValue;
						wValue.clear();
						wValue.resize(rq_cch);

						MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, value,
							static_cast<int>(strlen(value)),
							const_cast<wchar_t*>(wValue.c_str()),
							static_cast<int>(wValue.size()));

						Info.vecText.push_back(wValue);
					}

					if (strcmp("Anim", contextItem->name()) == 0) {
						char* value = contextItem->value();

						int rq_cch = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, value, 
							static_cast<int>(strlen(value)), nullptr, 0);

						std::wstring wValue;
						wValue.clear();
						wValue.resize(rq_cch);

						MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, value,
							static_cast<int>(strlen(value)),
							const_cast<wchar_t*>(wValue.c_str()),
							static_cast<int>(wValue.size()));

						Info.vecAnim.push_back(wValue);
					}
				}
			}

			m_vecDialTextInfo.push_back(Info);
		}

	}

	xmlFile.close();
	m_XmlDoc->clear();
}

int CXmlParser::GetDialogCount()
{
	return static_cast<int>(m_vecDialTextInfo.size());
}

std::string CXmlParser::GetDialogInfoByIndex(int Idx, DialogInfo* Info) const
{
	size_t MaxSize = m_vecDialTextInfo[Idx].vecTalker.size();
	Info->TextIdx = 0;
	Info->TextMaxIdx = static_cast<int>(MaxSize);
	for (size_t size = 0; size < MaxSize; size++) 
	{
		Info->vecTalker.push_back(m_vecDialTextInfo[Idx].vecTalker[size]);
		Info->vecText.push_back(m_vecDialTextInfo[Idx].vecText[size]);
		Info->vecAnim.push_back(m_vecDialTextInfo[Idx].vecAnim[size]);
	}
	return m_vecDialTextInfo[Idx].DialogName;
}
