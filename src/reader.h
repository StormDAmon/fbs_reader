//
//  reader.h
//
//  Created by Storm DAmon on 2018/10/18.
//

#pragma once
#include <string>
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


struct TABLE_ITEM
{
	std::string cls;
	std::string label;
};
struct TABLE
{
	std::string name;
	std::vector<TABLE_ITEM> items;
	std::string base;
	std::string reader;
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
	std::vector<TABLE> m_vct_table;	// 列表

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


static bool in_m_key(std::map<std::string, std::string> &map, const std::string &str)
{
	if (map.end() != map.find(str)) return true;
	return false;
}

static bool in_m_value(std::map<std::string, std::string> &map, const std::string &str)
{
	if (str.empty() || map.empty()) return false;
	for (auto i : map)
	{
		if (i.second == str) return true;
	}
	return false;
}











