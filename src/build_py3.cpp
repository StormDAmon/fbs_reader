//
//  build_py3.cpp
//
//  Created by Storm DAmon on 2018/11/14.
//

#include <vector>
#include <algorithm>
#include "reader.h"

// 转换 Python 变量名
std::string namePy(const std::string &str)
{
    if (str.empty()) return "";
	std::string str_;
	bool bTurnBig = true;
	for (auto c : str)
	{
		if (bTurnBig)
		{
			if ('a' <= c && c <= 'z') c += 'A' - 'a';
			bTurnBig = false;
		}

		if (('a' > c || c > 'z') && 
			('A' > c || c > 'Z'))
		{
			bTurnBig = true;
			if ('0' > c || c > '9') continue;
		}
		str_ += c;
	}
    return str_;
}

std::string strLower(const std::string &str)
{
	std::string str_ = str;
	transform(str_.begin(), str_.end(), str_.begin(), ::tolower);
	return str_;
}

std::string reader::build_py3()
{
	std::vector<Table> vct_table = m_vct_table;

	// 翻译块
	for (auto &t : vct_table)
	{
		std::stringstream out;
		// 开始生成 Table
		out << "import " << m_str_namespace << "." << t.name << "\n";
		out << "class " << t.name << "(object):\n";
		for (auto i : t.items)
		{
			out << "\t" << i.label << " = ";
			if (i.array) out << "[]";
			else
			{
				switch(i.type)
				{
					case CT_Str:
					case CT_Normal: out << m_map_cls_name_py3[i.cls]; break;
					case CT_Table: 	out << i.cls << "()"; break;
					case CT_Enum:
					{
						out << m_str_namespace << "." << i.cls << "." << i.cls << "." << *m_map_enum[i.cls].begin();
						break;
					}
					default: break;
				}
			}
			out << "\n";
		}
		out << "\n";

        // __init__()
		out << "\tdef __init__(self, buf=None):\n";
		out << "\t\tif None == buf:\n";
		out << "\t\t\treturn\n";
		out << "\t\tfbs = buf\n";
		out << "\t\tif 'bytearray' == type(buf).__name__:\n";
		out << "\t\t\tfbs = " << m_str_namespace << "." << namePy(t.name) << "." << namePy(t.name) << ".GetRootAs" << namePy(t.name) << "(buf, 0)\n";
		for (auto i : t.items)
		{
			out << "\t\tself." << i.label << " = ";
			if (i.array)
			{
				if (CT_Table == i.type) 
				{
					out << "read_vct_cls(fbs, '" << namePy(i.label) << "', '" << namePy(i.cls) << "')";
				}
				else out << "read_vct(fbs, '" << namePy(i.label) << "')";
			}
			else
			{
				if (CT_Table == i.type) out << i.cls << "(";
				else if (CT_Str == i.type) out << "read_str(";

				if (CT_Table == i.type) out << "fbs." << i.label << "()";
				else out << "fbs." << namePy(i.label) << "()";
				switch(i.type)
				{
					case CT_Str:
					case CT_Table: out << ")"; break;
					case CT_Normal: 
					case CT_Enum:
					default: break;
				}
			}
			out << "\n";
		}
		out << "\n";

		// to_fbs()
		out << "\tdef to_fbs(self, fb=None):\n";
		out << "\t\tis_finish = False\n";
		out << "\t\tif None == fb:\n";
		out << "\t\t\tis_finish = True\n";
		out << "\t\t\tfb = flatbuffers.Builder(0)\n";
		for (auto i : t.items)
		{
			if (i.array)
			{
				std::string strFuncName = "";
				switch(i.type)
				{
					case CT_Str: strFuncName = "build_vct_str"; break;
					case CT_Table: strFuncName = "build_vct_cls"; break;
					case CT_Normal: 
					{
						if ("byte" == i.cls) 		strFuncName = "build_vct_byte";
						else if ("ubyte" == i.cls) 	strFuncName = "build_vct_ubyte";
						else if ("short" == i.cls) 	strFuncName = "build_vct_short";
						else if ("ushort" == i.cls) strFuncName = "build_vct_ushort";
						else if ("int" == i.cls) 	strFuncName = "build_vct_int";
						else if ("uint" == i.cls) 	strFuncName = "build_vct_uint";
						else if ("long" == i.cls) 	strFuncName = "build_vct_long";
						else if ("ulong" == i.cls) 	strFuncName = "build_vct_ulong";
						else if ("float" == i.cls) 	strFuncName = "build_vct_float";
						else if ("bool" == i.cls) 	strFuncName = "build_vct_bool";
						break;
					}
					case CT_Enum: strFuncName = "build_vct_enum"; break;
					default: break;
				}
				out << "\t\t_" << i.label << " = " << strFuncName << "(fb, self." << i.label << ", " << m_str_namespace << "." << namePy(t.name) << "." << namePy(t.name) << "Start" << namePy(i.label) << "Vector)\n";
			}
			else if (CT_Str == i.type)
			{
				out << "\t\t_" << i.label << " = fs(fb, self." << i.label << ")\n";
			}
			else if (CT_Table == i.type)
			{
				out << "\t\t_" << i.label << " = self." << i.label << ".to_fbs(fb)\n";
			}
		}
		out << "\n";

		out << "\t\t" << m_str_namespace << "." << namePy(t.name) << "." << namePy(t.name) << "Start(fb)\n";
		for (auto i : t.items)
		{
			out << "\t\t" << m_str_namespace << "." << namePy(t.name) << "." << namePy(t.name) << "Add" << namePy(i.label) << "(fb, ";
			if (i.array || CT_Str == i.type || CT_Table == i.type) out << "_";
			else out << "self.";
			out << i.label << ")\n";
		}
		out << "\t\tif is_finish:\n";
		out << "\t\t\tfb.Finish(" << m_str_namespace << "." << namePy(t.name) << "." << namePy(t.name) << "End(fb))\n";
		out << "\t\t\treturn fb.Output()\n";
		out << "\t\treturn " << m_str_namespace << "." << namePy(t.name) << "." << namePy(t.name) << "End(fb)\n";
		out << "\n";

		t.reader = out.str();
	}


	// 填充
	std::stringstream out;
	out << "import flatbuffers\n\n";
	out << "def read_str(fb):\n\
\tif None == fb:\n\
\t\treturn ''\n\
\treturn fb.decode('utf-8')\n";
	out << "def read_vct(fb, name):\n\
\tl = []\n\
\tfor i in range(eval('fb.%sLength()' % name)):\n\
\t\tl.append(eval('fb.%s(%d)' % (name, i)))\n\
\treturn l\n";
	out << "def read_vct_cls(fb, name, cls):\n\
\tl = []\n\
\tfor i in range(eval('fb.%sLength()' % name)):\n\
\t\tl.append(eval('%s(fb.%s(%d))' % (cls, name, i)))\n\
\treturn l\n";
	out << "def fs(fb, str):\n\
\tif 0 == len(str):\n\
\t\treturn 0\n\
\treturn fb.CreateString(str)\n";
	out << "def build_vct(fb, list, func_start, t):\n";
	out << "\tlength = len(list)\n";
	out << "\tl = []\n";
	out << "\tfor i in list:\n";
	out << "\t\tl.insert(0, i)\n";
	out << "\tfunc_start(fb, length)\n";
	out << "\tfor i in l:\n";
	out << "\t\teval('fb.Prepend%s(%d)' % (t, i))\n";
	out << "\treturn fb.EndVector(length)\n";
	out << "def build_vct_bool(fb, list, func_start):\n";
	out << "\treturn build_vct(fb, list, func_start, 'Bool')\n";
	out << "def build_vct_enum(fb, list, func_start):\n";
	out << "\treturn build_vct(fb, list, func_start, 'Byte')\n";
	out << "def build_vct_byte(fb, list, func_start):\n";
	out << "\treturn build_vct(fb, list, func_start, 'Byte')\n";
	out << "def build_vct_ubyte(fb, list, func_start):\n";
	out << "\treturn build_vct(fb, list, func_start, 'Uint8')\n";
	out << "def build_vct_short(fb, list, func_start):\n";
	out << "\treturn build_vct(fb, list, func_start, 'Int16')\n";
	out << "def build_vct_ushort(fb, list, func_start):\n";
	out << "\treturn build_vct(fb, list, func_start, 'Uint16')\n";
	out << "def build_vct_int(fb, list, func_start):\n";
	out << "\treturn build_vct(fb, list, func_start, 'Int32')\n";
	out << "def build_vct_uint(fb, list, func_start):\n";
	out << "\treturn build_vct(fb, list, func_start, 'Uint32')\n";
	out << "def build_vct_long(fb, list, func_start):\n";
	out << "\treturn build_vct(fb, list, func_start, 'Int64')\n";
	out << "def build_vct_ulong(fb, list, func_start):\n";
	out << "\treturn build_vct(fb, list, func_start, 'Uint64')\n";
	out << "def build_vct_float(fb, list, func_start):\n";
	out << "\treturn build_vct(fb, list, func_start, 'Float64')\n";
	out << "def build_vct_cls(fb, list, func_start):\n\
\tlength = len(list)\n\
\tl = []\n\
\tfor i in list:\n\
\t\tl.insert(0, i.to_fbs(fb))\n\
\tfunc_start(fb, length)\n\
\tfor i in l:\n\
\t\tfb.PrependUOffsetTRelative(i)\n\
\treturn fb.EndVector(length)";
	out << "\n";
	out << "def build_vct_str(fb, list, func_start):\n\
\tlength = len(list)\n\
\tl = []\n\
\tfor i in list:\n\
\t\tl.insert(0, fs(fb, i))\n\
\tfunc_start(fb, length)\n\
\tfor i in l:\n\
\t\tfb.PrependUOffsetTRelative(i)\n\
\treturn fb.EndVector(length)";
	out << "\n\n\n\n";

	for (auto e : m_map_enum)
	{
		out << "import " + m_str_namespace + "." + e.first + "\n";
	}
	out << "\n\n\n\n";

	// 填充结构体
	std::vector<Table> vct_table_tmp;
	for (auto table : vct_table)
	{
		bool b_insert = false;
		for (auto iter = vct_table_tmp.begin(); iter != vct_table_tmp.end(); iter++)
		{
			if (std::string::npos != iter->reader.find(" = " + table.name + "("))
			{
				vct_table_tmp.insert(iter, table);
				b_insert = true;
				break;
			}
		}
		if (!b_insert) vct_table_tmp.push_back(table);
	}
	
	for (auto t : vct_table_tmp)
	{
		out << t.reader + "\n\n";
	}

	return out.str();
}

