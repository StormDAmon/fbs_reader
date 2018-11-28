//
//  reader.h
//
//  Created by Storm DAmon on 2018/10/18.
//

#pragma once
#include <string>
#include <sstream>
#include <queue>
#include <map>
#include <vector>

#include "rl_string.h"


// 唯一实例
#ifndef DEF_INS
#define DEF_INS(cls)\
	static cls* ins()\
	{\
		if (!g_ptr_app) g_ptr_app = new cls;\
		return g_ptr_app;\
	};\
	static cls *g_ptr_app;
#define DEF_INS_BODY(cls) cls *cls::g_ptr_app = NULL
#endif


enum Cls_Type
{
	CT_Normal = 0,
	CT_Str,
	CT_Enum,
	CT_Table
};
struct Table_Item
{
	bool array;
	Cls_Type type;
	std::string cls;
	std::string label;
	Table_Item()
	{
		array = false;
		type = CT_Normal;
	}
};
struct Table
{
	std::string base;
	std::string reader;

	std::string name;
	std::vector<Table_Item> items;
};

enum Reader_Type
{
	CPP = 0,
	Python2,
	Python3,
	NodeJS
};

class reader
{
public:
	reader();
	~reader()
	{
	}

	// 生成 reader
	bool load(const std::string &str_path);
	void transTable();
	std::string build(const Reader_Type &type);
	std::string build_cpp();
	std::string build_py2() { return ""; };
	std::string build_py3();
	std::string build_nJS() { return ""; };

	// 存文件
	bool save(const Reader_Type &type, const std::string &str_src);

	// 唯一实例
	DEF_INS(reader);
	

private:
	std::string m_str_path;
	std::string m_str_file_name;
	std::string m_str_fbs;

	std::map<std::string, std::string> m_map_cls_name_cpp;		// 类名映射 C++
	std::map<std::string, std::string> m_map_cls_name_py2;		// 类名映射 Python2
	std::map<std::string, std::string> m_map_cls_name_py3;		// 类名映射 Python3
	std::map<std::string, std::string> m_map_cls_name_njs;		// 类名映射 NodeJS

	std::string m_str_namespace;	// 命名空间
	std::map<std::string, std::vector<std::string> > m_map_enum;	// 枚举映射
	std::vector<Table> m_vct_table;	// 列表

};


#define BOTH(head, end) (head == str_begin_flag && is_match(end, iter))
#define BOTH_2E(head, end_a, end_b) (head == str_begin_flag && (is_match(end_a, iter) || is_match(end_b, iter)))

static bool is_match(const std::string &str, std::string::iterator &iter)
{
	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] != iter[i]) return false;
	}
	return true;
}

#define isInMapK(map, i) map.end() != map.find(i)

static bool in_m_value(std::map<std::string, std::string> &map, const std::string &str)
{
	if (str.empty() || map.empty()) return false;
	for (auto i : map)
	{
		if (i.second == str) return true;
	}
	return false;
}











