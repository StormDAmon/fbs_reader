//
//  rl_string.h
//
//  Created by Storm DAmon on 2018/7/8.
//

#pragma once
#include <vector>
#include <string.h>


// Split
static std::vector<std::string> rl_split(const std::string &strSrc, const std::string &strSplit)
{
	std::vector<std::string> vctOut;
    std::string strTmp = strSrc;
    while (strTmp.size())
	{
        int nIndex = strTmp.find(strSplit);
        if (std::string::npos == nIndex)
		{
            vctOut.push_back(strTmp);
            strTmp.clear();
        }
        else
		{
            vctOut.push_back(strTmp.substr(0, nIndex));
            strTmp = strTmp.substr(nIndex + strSplit.size(), strTmp.size() - nIndex - strSplit.size());
        }
    }
	return vctOut;
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


