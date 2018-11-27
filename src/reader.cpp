//
//  reader.cpp
//
//  Created by Storm DAmon on 2018/10/18.
//
#include <iostream>
#include <fstream>
#include "reader.h"

DEF_INS_BODY(reader);

reader::reader()
{
	m_map_cls_name_cpp["byte"] = "int8_t";
	m_map_cls_name_cpp["ubyte"] = "uint8_t";
	m_map_cls_name_cpp["short"] = "int16_t";
	m_map_cls_name_cpp["ushort"] = "uint16_t";
	m_map_cls_name_cpp["int"] = "int32_t";
	m_map_cls_name_cpp["uint"] = "uint32_t";
	m_map_cls_name_cpp["long"] = "int64_t";
	m_map_cls_name_cpp["ulong"] = "uint64_t";
	m_map_cls_name_cpp["float"] = "float";
	m_map_cls_name_cpp["bool"] = "bool";
	m_map_cls_name_cpp["string"] = "std::string";

	m_map_cls_name_py3["byte"] = "int()";
	m_map_cls_name_py3["ubyte"] = "int()";
	m_map_cls_name_py3["short"] = "int()";
	m_map_cls_name_py3["ushort"] = "int()";
	m_map_cls_name_py3["int"] = "int()";
	m_map_cls_name_py3["uint"] = "int()";
	m_map_cls_name_py3["long"] = "int()";
	m_map_cls_name_py3["ulong"] = "int()";
	m_map_cls_name_py3["float"] = "float()";
	m_map_cls_name_py3["bool"] = "bool()";
	m_map_cls_name_py3["string"] = "str()";
}



bool reader::load(const std::string &str_path)
{
	// 读文件
	std::string str_reader;
	std::ifstream file(str_path, std::ios::in);
	if (!file) return false;
	char c;
	while ((c = file.get()) != EOF) m_str_fbs += c;
	file.close();
	if (m_str_fbs.empty()) return false;

	// 文件名
	m_str_file_name = str_path.substr(str_path.rfind("/") + 1, str_path.size() - str_path.rfind("/"));
	m_str_file_name = m_str_file_name.substr(m_str_file_name.rfind("\\") + 1, m_str_file_name.size() - m_str_file_name.rfind("\\"));
	m_str_file_name = m_str_file_name.substr(0,  m_str_file_name.find("."));
	// 文件路径
	m_str_path = str_path.substr(0, str_path.rfind(m_str_file_name));

	// 块分割
	std::string str_begin_flag;
	std::string str_cur_tmp;
	for (auto iter = m_str_fbs.begin(); iter != m_str_fbs.end(); iter++)
	{
		// Head
		if (str_begin_flag.empty())
		{
			bool b_match = true;

#define MATCH(head)\
		else if (is_match(head, iter))\
		{\
			str_begin_flag = head;\
		}
			if (is_match("/*", iter)) str_begin_flag = "/*";
			MATCH("//")
			MATCH("namespace")
			MATCH("enum")
			MATCH("table")
			MATCH("root_type")
		}

		// Data
		if (str_begin_flag.size()) str_cur_tmp += *iter;

		// End
		if (str_begin_flag.size())
		{
			if (BOTH("/*", "*/") ||
				BOTH_2E("//", "\n", "\r\n") ||
				BOTH("namespace", ";") ||
				BOTH("enum", "}") ||
				BOTH("table", "}") ||
				BOTH("root_type", ";"))
			{
				// 填充缓存
				if (BOTH("namespace", ";"))
				{
					m_str_namespace = str_cur_tmp.substr(10, str_cur_tmp.size() - 10 - 1);
				}
				else if (BOTH("enum", "}"))
				{
					std::string str_name = str_cur_tmp.substr(5, str_cur_tmp.find(":") - 5);
					std::string str_data = str_cur_tmp.substr(str_cur_tmp.find("{") + 1, str_cur_tmp.size() - str_cur_tmp.find("{") - 2);
					rl_replace(str_data, " ", "");
					std::vector<std::string> vct_items;
					vct_items = rl_split(str_data, ",");
					for (auto &i : vct_items)
					{
						if (std::string::npos != i.find("="))
						{
							i = i.substr(0, i.find("="));
						}
					}
					m_map_enum[str_name] = vct_items;
				}
				else if (BOTH("table", "}"))
				{
					// name
					std::string str_table_name = str_cur_tmp.substr(6, str_cur_tmp.find("{") - 6);
					rl_replace(str_table_name, "\r\n", "");
					rl_replace(str_table_name, "\n", "");
					rl_replace(str_table_name, " ", "");
					TABLE table;
					table.name = str_table_name;
					table.base = str_cur_tmp;
					m_vct_table.push_back(table);
				}

				if (BOTH("/*", "*/") ||
					BOTH("//", "\r\n")) iter++;
				str_begin_flag.clear();
				str_cur_tmp.clear();
			}
		}
	}
	return true;
}


std::string reader::build(const Reader_Type &type)
{
	switch (type)
	{
		case CPP: return build_cpp();
		case Python2: return build_py2();
		case Python3: return build_py3();
		case NodeJS: return build_nJS();
		default: break;
	}
	return "";
}

// 存文件
bool reader::save(const Reader_Type &type, const std::string &str_src)
{
	if (str_src.empty()) return false;

	std::string str_file_type;
	switch (type)
	{
		case CPP: str_file_type = ".h"; break;
		case Python2: 
		case Python3: str_file_type = ".py"; break;
		case NodeJS: str_file_type = ".js"; break;
		default: break;
	}

	std::string str_to_path = m_str_path + "/" + m_str_file_name + "_reader" + str_file_type;
	std::fstream file(str_to_path, std::ios::out);
	if (!file) return false;
	file << str_src;
	file.close();
	return true;
}

