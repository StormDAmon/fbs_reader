//
//  rl_string.h
//
//  Created by Storm DAmon on 2018/7/8.
//

#pragma once
#include <vector>
#include <string.h>


// Split
static std::vector<std::string> rl_split(const std::string& str_src, const std::string& str_split)
{
	std::vector<std::string> vct_out;
	if (str_src.empty() || str_split.empty()) return vct_out;
	std::string str_src_tmp = str_src;
	std::vector<char*> vct_out_tmp;

	char *_s = (char*)str_src_tmp.c_str();
	while (1)
	{
		char *p = strstr(_s, str_split.c_str());
		if (!p)
		{
			vct_out_tmp.push_back(_s);
			break;
		}
		*p = 0;
		vct_out_tmp.push_back(_s);
		_s = p + str_split.size();
	}

	for (auto iter : vct_out_tmp)
	{
		vct_out.push_back(iter);
	}
	return vct_out;
}

// Join
static std::string rl_join(std::vector<std::string>& vct_src, const std::string& str_separator)
{
	if (vct_src.empty()) return "";
	std::string str_out;
	for (auto i : vct_src) str_out += i + str_separator;
	return str_out.substr(0, str_out.size() - str_separator.size());
}

// Replace
static std::string rl_replace(std::string& str_src, const std::string& str_from, const std::string& str_to)
{
	if (str_src.empty() || str_from.empty() || std::string::npos == str_src.find(str_from)) return str_src;

	auto vct_tmp = rl_split(str_src, str_from);
	str_src.clear();
	for (auto item : vct_tmp)
	{
		str_src += item + str_to;
	}
	str_src = str_src.substr(0, str_src.size() - str_to.size());

	return str_src;
}


