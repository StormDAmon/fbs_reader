//
//  build_cpp.cpp
//
//  Created by Storm DAmon on 2018/11/7.
//
#include "reader.h"



std::string reader::build_cpp()
{
	std::vector<Table> vct_table = m_vct_table;

	// 翻译块
	for (auto &table : vct_table)
	{
		// items
		auto vct_tmp = rl_split(table.base, "{");
		std::string str_data = vct_tmp[1];
		rl_replace(str_data, "}", "");

		std::string str_tmp = str_data;
		str_data.clear();
		std::string str_begin_flag;
		for (auto iter = str_tmp.begin(); iter != str_tmp.end(); iter++)
		{
			if (str_begin_flag.empty())
			{
				if (is_match("/*", iter)) str_begin_flag = "/*";
				else if (is_match("//", iter)) str_begin_flag = "//";
			}
			if (str_begin_flag.size())
			{
				if (BOTH("/*", "*/") || BOTH_2E("//", "\n", "\r\n"))
				{
					if (BOTH("/*", "*/")) iter++;
					str_begin_flag.clear();
				}
				continue;
			}
			str_data += *iter;
		}

		rl_replace(str_data, "\r\n", "");
		rl_replace(str_data, "\n", "");
		rl_replace(str_data, "\t", "");
		rl_replace(str_data, " ", "");

		auto vct_items = rl_split(str_data, ";");
		std::vector<Table_Item> vct_items_fix;
		for (auto item : vct_items)
		{
			if (std::string::npos == item.find(":")) continue;
			auto vct_feilds = rl_split(item, ":");
			Table_Item item_new;
			item_new.cls = vct_feilds[1];
			item_new.label = vct_feilds[0];
			if (std::string::npos != item_new.cls.find("="))
			{
				item_new.cls = item_new.cls.substr(0, item_new.cls.find("="));
			}
			if (isInMapK(m_map_cls_name_cpp, item_new.cls)) item_new.cls = m_map_cls_name_cpp[item_new.cls];
			else if (m_map_enum.end() != m_map_enum.find(item_new.cls)) item_new.cls = m_str_namespace + "::" + item_new.cls;

			vct_items_fix.push_back(item_new);
		}

		// 开始生成 struct
		table.reader.clear();
		table.reader = "struct " + table.name + "\n";
		table.reader += "{\n";
		for (auto &item : vct_items_fix)
		{
			if (std::string::npos != item.cls.find("["))
			{
				rl_replace(item.cls, "[", "");
				rl_replace(item.cls, "]", "");
				if (isInMapK(m_map_cls_name_cpp, item.cls)) item.cls = m_map_cls_name_cpp[item.cls];
				if (m_map_enum.end() != m_map_enum.find(item.cls)) item.cls = m_str_namespace + "::" + item.cls;
				item.cls = "std::vector<" + item.cls + ">";
			}
			table.reader += "\t" + item.cls + " " + item.label + ";\n";
		}
		table.reader += "\n";

		table.reader += "\t" + table.name + "()\n";
		table.reader += "\t{\n";
		for (auto item : vct_items_fix)
		{
			if ((std::string::npos != item.cls.find("int") ||
				std::string::npos != item.cls.find("INT") ||
				std::string::npos != item.cls.find("long")) && 
				std::string::npos == item.cls.find("std::vector"))
			{
				table.reader += "\t\t" + item.label + " = 0;\n";
			}
			else if (std::string::npos != item.cls.find("float") &&
				std::string::npos == item.cls.find("std::vector"))
			{
				table.reader += "\t\t" + item.label + " = 0.0;\n";
			}
			else if (std::string::npos != item.cls.find(m_str_namespace + "::") &&
				std::string::npos == item.cls.find("std::vector"))
			{
				std::string str_value = m_map_enum[item.cls.substr(item.cls.rfind(":") + 1, item.cls.size() - m_str_namespace.size() - 2)][0];
				rl_replace(str_value, "\r\n", "");
				rl_replace(str_value, "\n", "");
				rl_replace(str_value, "\t", "");
				table.reader += "\t\t" + item.label + " = " + item.cls + "_" + str_value + ";\n";
			}
		}
		table.reader += "\t}\n";

		table.reader += "\t" + table.name + "(const " + table.name + " &src)\n\
\t{\n";
		for (auto item : vct_items_fix)
		{
			table.reader += "\t\t" + item.label + " = src." + item.label + ";\n";
		}
		table.reader += "\t}\n";
		table.reader += "\
\t" + table.name + "(const void *pData, const size_t nLen)\n\
\t{\n\
\t\tif (!verify(pData, nLen))\n\
\t\t{\n\
\t\t\t*this = " + table.name + "();\n\
\t\t\treturn;\n\
\t\t}\n\
\t\tREAD_FBS(fbsObj, " + m_str_namespace + "::" + table.name + ", pData, nLen);\n\
\t\t*this = " + table.name + "(fbsObj);\n\
\t}\n";
		table.reader += "\t" + table.name + "(const " + m_str_namespace + "::" + table.name + " *pFbs)\n";
		table.reader += "\t{\n";
		table.reader += "\t\tif (!pFbs) return;\n";
		for (auto item : vct_items_fix)
		{
			table.reader += "\t\t";
			if ("std::string" == item.cls)
			{
				table.reader += item.label + " = GX3(pFbs, " + item.label + "(), c_str(), \"\")";
			}
			else if (std::string::npos != item.cls.find("std::vector"))
			{
				std::string str_cls = item.cls.substr(12, item.cls.size() - 12 - 1);
				if (std::string::npos != str_cls.find(m_str_namespace))
				{
					str_cls = str_cls.substr(str_cls.rfind(":") + 1, str_cls.size() - str_cls.rfind(":"));
				}
				if ("std::string" == str_cls)
				{
					table.reader += item.label + " = readVctStr<" + str_cls + ", flatbuffers::String>(pFbs->" + item.label + "())";
				}
				else if (m_map_enum.end() != m_map_enum.find(str_cls))
				{
					table.reader += item.label + " = readVctEnum<" + m_str_namespace + "::" + str_cls + ", " + "uint8_t>(pFbs->" + item.label + "())";
				}
				else if (!in_m_value(m_map_cls_name_cpp, str_cls))
				{
					table.reader += item.label + " = readVct<" + str_cls + ", " + m_str_namespace + "::" + str_cls + ">(pFbs->" + item.label + "())";
				}
				else
				{
					std::string str_cls_tmp = str_cls;
					if ("bool" == str_cls_tmp) str_cls_tmp = "uint8_t";
					table.reader += item.label + " = readVctEnum<" + str_cls + ", " + str_cls_tmp + ">(pFbs->" + item.label + "())";
				}
			}
			else
			{
				table.reader += item.label + " = pFbs->" + item.label + "()";
			}
			table.reader += ";\n";
		}
		table.reader += "\t}\n";
		table.reader += "\tbool verify(const void* pFbs, const size_t nLen)\n\
\t{\n\
\t\treturn fbsVerifyTable<" + m_str_namespace + "::" + table.name + ">(pFbs, nLen);\n\
\t}\n";
		table.reader += "\tflatbuffers::Offset<" + m_str_namespace + "::" + table.name + "> toFbs(flatbuffers::FlatBufferBuilder &fb)\n";
		table.reader += "\t{\n";
		table.reader += "\t\treturn " + m_str_namespace + "::Create" + table.name + "(fb,";
		std::vector<std::string> vct_params;
		for (auto item : vct_items_fix)
		{
			if ("std::string" == item.cls) vct_params.push_back(" fs(" + item.label + ")");
			else if (std::string::npos != item.cls.find("std::vector"))
			{
				std::string str_cls = item.cls.substr(12, item.cls.size() - 12 - 1);
				if (std::string::npos != str_cls.find(m_str_namespace))
				{
					str_cls = str_cls.substr(str_cls.rfind(":") + 1, str_cls.size() - str_cls.rfind(":"));
				}
				if ("std::string" == str_cls)
				{
					vct_params.push_back(" buildVctStr<" + str_cls + ", flatbuffers::String>(fb, " + item.label + ")");
				}
				else if (m_map_enum.end() != m_map_enum.find(str_cls))
				{
					vct_params.push_back(" buildVctEnum<" + m_str_namespace + "::" + str_cls + ", uint8_t>(fb, " + item.label + ")");
				}
				else if (!in_m_value(m_map_cls_name_cpp, str_cls))
				{
					vct_params.push_back(" buildVct<" + str_cls + ", " + m_str_namespace + "::" + str_cls + ">(fb, " + item.label + ")");
				}
				else
				{
					std::string str_cls_tmp = str_cls;
					if ("bool" == str_cls_tmp) str_cls_tmp = "uint8_t";
					vct_params.push_back(" buildVctEnum<" + str_cls + ", " + str_cls_tmp + ">(fb, " + item.label + ")");
				}
			}
			else if (in_m_value(m_map_cls_name_cpp, item.cls) ||
				std::string::npos != item.cls.find(m_str_namespace + "::")) vct_params.push_back(" " + item.label);
			else if (m_map_enum.end() != m_map_enum.find(item.cls)) vct_params.push_back(" " + item.label);
			else vct_params.push_back(" " + item.label + ".toFbs(fb)");
		}
		table.reader += "\n\t\t\t";
		table.reader += rl_join(vct_params, ",\n\t\t\t");
		table.reader += ");\n";
		table.reader += "\t}\n";
		table.reader += "\tstd::string toJson(const bool bSimplify = false)\n";
		table.reader += "\t{\n";
		table.reader += "\t\tstd::stringstream ss;\n";
		table.reader += "\t\tss << \"{\";\n";
		for (auto iter = vct_items_fix.begin(); iter != vct_items_fix.end(); iter++)
		{
			std::string str_value;
			std::string str_cls = iter->cls;
			std::string str_simplify;
			if (std::string::npos != str_cls.find("std::vector"))
			{
				str_cls = str_cls.substr(12, str_cls.size() - 12 - 1);
				str_value = " << readVctToJson<" + str_cls + ">(" + iter->label + ")";
			}
			else if (m_map_enum.end() != m_map_enum.find(str_cls.substr(str_cls.rfind(":") + 1, str_cls.size() - str_cls.rfind(":"))))
			{
				str_value = " << \"\\\"\" << EnumName" + str_cls.substr(str_cls.rfind(":") + 1, str_cls.size() - str_cls.rfind(":")) + "(" + iter->label + ") << \"\\\"\"";
			}
			else if (!in_m_value(m_map_cls_name_cpp, str_cls) && std::string::npos == str_cls.find("::") ||
				std::string::npos != str_cls.find("std::string"))
			{
				str_value = " << " + iter->label + ".toJson(bSimplify)";
				if (std::string::npos != str_cls.find("std::string"))
				{
					str_value = " << \"\\\"\" << " + iter->label + " << \"\\\"\"";
				}
			}
			else if (std::string::npos != str_cls.find("int") ||
				std::string::npos != str_cls.find("INT") ||
				std::string::npos != str_cls.find("long"))
			{
				str_value = " << (" + str_cls + ")" + iter->label;
			}
			else str_value = " << \"\\\"\" << (" + str_cls + ")" + iter->label + " << \"\\\"\"";

			if (std::string::npos != iter->cls.find("std::vector") ||
				std::string::npos != iter->cls.find("std::string"))
			{
				str_simplify = iter->label + ".size()";
			}
			if (str_simplify.size()) str_simplify = "if (!bSimplify || (bSimplify && " + str_simplify + ")) ";

			table.reader += "\t\t" + str_simplify + "ss << \"\\\"" + iter->label + "\\\":\"" + str_value;
			table.reader += " << \",\";\n";
		}
		table.reader += "\t\tssCutEndC(ss);\n";
		table.reader += "\t\tss << \"}\";\n";
		table.reader += "\t\treturn ss.str();\n";
		table.reader += "\t}\n";
		table.reader += "\
\tfriend std::ostream& operator <<(std::ostream &ss, " + table.name + " &data)\n\
\t{\n\
\t\tss << data.toJson(true);\n\
\t\treturn ss;\n\
\t}\n";
		table.reader += "};";
	}

	// 填充宏
	std::string str_out = "#pragma once\n\
#include <vector>\n\
#include <sstream>\n\
#include \"flatbuffers/flatbuffers.h\"\n\
#include \"" + m_str_file_name + "_generated.h\"\n\
\n\n\
namespace FR\n\
{\n\
\n\
#ifndef GX1\n\
#	define GX1(x, y) (!x ? (y) : x)\n\
#endif\n\
#ifndef GX2\n\
#	define GX2(x1, x2, y) ((!x1 || !(x1->x2)) ? (y) : x1->x2)\n\
#endif\n\
#ifndef GX3\n\
#	define GX3(x1, x2, x3, y) (((!x1 || !(x1->x2) || !(x1->x2->x3)) ? (y) : x1->x2->x3))\n\
#endif\n\
#ifndef GX4\n\
#	define GX4(x1, x2, x3, x4, y) (((!x1 || !(x1->x2) || !(x1->x2->x3) || !(x1->x2->x3->x4))) ? (y) : x1->x2->x3->x4)\n\
#endif\n\
#ifndef GX5\n\
#	define GX5(x1, x2, x3, x4, x5, y) (((!x1 || !(x1->x2) || !(x1->x2->x3) || !(x1->x2->x3->x4) || !(x1->x2->x3->x4->x5))) ? (y) : x1->x2->x3->x4->x5)\n\n\
#endif\n\
#ifndef fs\n\
#	define fs(str) str.empty() ? NULL : fb.CreateString(str)\n\
#endif\n\
#ifndef fv\n\
#	define fv(vct) vct.empty() ? NULL : fb.CreateVector(vct)\n\
#endif\n\
#ifndef FBS_MAX_SIZE\n\
#	define FBS_MAX_SIZE 10 * 1024 * 1024\n\
#endif\n\
#ifndef READ_FBS\n\
#	define READ_FBS(obj, cls, data, len)\\\n\
	flatbuffers::FlatBufferBuilder builder_##obj;\\\n\
	builder_##obj.PushFlatBuffer((const uint8_t*)data, len);\\\n\
	auto obj = flatbuffers::GetRoot<cls>(builder_##obj.GetBufferPointer())\n\
#endif\n\
#ifndef BUILD_FBS\n\
#	define BUILD_FBS(fb, fr)\\\n\
	flatbuffers::FlatBufferBuilder fb;\\\n\
	fb.Finish(fr.toFbs(fb))\n\
#endif\n\
\n\
\n\
#ifndef FR_FUNC\n\
#	define FR_FUNC\n\
static std::stringstream& ssCutEndC(std::stringstream &ss)\n\
{\n\
\tstd::string str = ss.str(); \n\
\tif (1 != str.size()) str = str.substr(0, str.size() - 1); \n\
\tss.str(\"\"); \n\
\tss << str; \n\
\treturn ss; \n\
}\n\
\n\
template <typename T>\n\
static bool fbsVerifyTable(const void* pFbs, const size_t nLen)\n\
{\n\
	if (!pFbs || 0 == nLen || FBS_MAX_SIZE < nLen) return false;\n\
	READ_FBS(obj, T, pFbs, nLen);\n\
	auto verify = flatbuffers::Verifier((const uint8_t*)builder_obj.GetBufferPointer(), builder_obj.GetSize());\n\
	return obj->Verify(verify);\n\
}\n\
\n\
\n\
// Verify\n\
////////////////////\n\
// reader && builder\n\
\n\
\n\
template <typename Tc, typename Tf>\n\
static std::vector<Tc> readVct(const flatbuffers::Vector<flatbuffers::Offset<Tf>> *pFbs)\n\
{\n\
	std::vector<Tc> data;\n\
	for (int i = 0; i < (int)GX2(pFbs, size(), 0); i++)\n\
	{\n\
		auto iter = pFbs->Get(i);\n\
		data.push_back(iter);\n\
	}\n\
	return data;\n\
}\n\
template <typename Tc, typename Tf>\n\
static std::vector<Tc> readVctEnum(const flatbuffers::Vector<Tf> *pFbs)\n\
{\n\
	std::vector<Tc> data;\n\
	for (int i = 0; i < (int)GX2(pFbs, size(), 0); i++)\n\
	{\n\
		auto iter = pFbs->Get(i);\n\
		data.push_back((Tc)iter);\n\
	}\n\
	return data;\n\
}\n\
template <typename Tc, typename Tf>\n\
static std::vector<Tc> readVctStr(const flatbuffers::Vector<flatbuffers::Offset<Tf>> *pFbs)\n\
{\n\
	std::vector<Tc> data;\n\
	for (int i = 0; i < (int)GX2(pFbs, size(), 0); i++)\n\
	{\n\
		auto iter = pFbs->Get(i);\n\
		data.push_back(GX2(iter, c_str(), \"\"));\n\
	}\n\
	return data;\n\
}\n\
template <typename Tc>\n\
static std::string readVctToJson(const std::vector<Tc> &data)\n\
{\n\
	std::stringstream ss;\n\
	ss << \"[\";\n\
	for (auto i : data) ss << i << \",\";\n\
	ssCutEndC(ss);\n\
	ss << \"]\";\n\
	return ss.str();\n\
}\n\
template <typename Tc, typename Tf>\n\
static flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Tf>>> buildVct(flatbuffers::FlatBufferBuilder &fb, const std::vector<Tc> vctData)\n\
{\n\
	std::vector<flatbuffers::Offset<Tf>> vctFbs;\n\
	for (auto i : vctData)\n\
	{\n\
		vctFbs.push_back(i.toFbs(fb));\n\
	}\n\
	return fv(vctFbs);\n\
}\n\
template <typename Tc, typename Tf>\n\
static flatbuffers::Offset<flatbuffers::Vector<Tf>> buildVctEnum(flatbuffers::FlatBufferBuilder &fb, const std::vector<Tc> vctData)\n\
{\n\
	std::vector<Tf> vctFbs;\n\
	for (auto i : vctData)\n\
	{\n\
		vctFbs.push_back(i);\n\
	}\n\
	return fv(vctFbs);\n\
}\n\
template <typename Tc, typename Tf>\n\
static flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Tf>>> buildVctStr(flatbuffers::FlatBufferBuilder &fb, const std::vector<Tc> vctData)\n\
{\n\
	std::vector<flatbuffers::Offset<Tf>> vctFbs;\n\
	for (auto i : vctData)\n\
	{\n\
		vctFbs.push_back(fs(i));\n\
	}\n\
	return fv(vctFbs);\n\
}\n\
static std::string EnumNameSafe(const char **arr, const int nValue)\n\
{\n\
	int nLen = 0;\n\
	while (true) if (nullptr == arr[nLen++]) break;\n\
	if (nValue > nLen - 2) return \"\";\n\
	return arr[nValue];\n\
}\n\
static int EnumNameSafe(const char **arr, const std::string &strName)\n\
{\n\
	int nValue = 0;\n\
	int i = 0;\n\
	while (nullptr != arr[i])\n\
	{\n\
		if (arr[i] == strName)\n\
		{\n\
			nValue = i;\n\
			break;\n\
		}\n\
		i++;\n\
	}\n\
	return nValue;\n\
}\n\
#endif\n\
\n\n\n";

	// 填充结构体声明
	for (auto table : vct_table)
	{
		str_out += "struct " + table.name + ";\n";

	}
	str_out += "\n";

	// 填充结构体
	std::vector<Table> vct_table_tmp;
	for (auto table : vct_table)
	{
		bool b_insert = false;
		for (auto iter = vct_table_tmp.begin(); iter != vct_table_tmp.end(); iter++)
		{
			if (std::string::npos != iter->reader.find(table.name + " "))
			{
				vct_table_tmp.insert(iter, table);
				b_insert = true;
				break;
			}
		}
		if (!b_insert) vct_table_tmp.push_back(table);
	}

	for (auto table : vct_table_tmp)
	{
		str_out += table.reader + "\n\n";
	}
	str_out += "}\n";


	return str_out;
}

