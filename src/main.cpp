//
//  mian.cpp
//
//  Created by Storm DAmon on 2018/10/18.
//
#include "reader.h"
#include "rl_path.h"

#include <iostream>

int main(int argc, char* argv[])
{
	if (2 != argc) return -1;
	std::string strPath = argv[1];
	std::cout << "argv: " << strPath << std::endl;
	if (std::string::npos == strPath.find(":") && '/' != strPath[0])
	{
		strPath = get_run_path() + PATH_L + strPath;
	}
	std::cout << "will build file: " << strPath << std::endl;

	if (!reader::ins()->load(strPath)) return -1;

	reader::ins()->save(CPP, reader::ins()->build(CPP));
	reader::ins()->save(Python3, reader::ins()->build(Python3));

    return 0;
}

