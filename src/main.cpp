//
//  mian.cpp
//
//  Created by Storm DAmon on 2018/10/18.
//
#include "reader.h"
#include "rl_path.h"


int main(int argc, char* argv[])
{
	if (2 != argc) return -1;
	std::string str_path = argv[1];
	if (std::string::npos == str_path.find(":"))
	{
		str_path = get_run_path() + PATH_L + str_path;
	}

	if (!reader::ins()->load(str_path)) return -1;

	reader::ins()->save(CPP, reader::ins()->build(CPP));
	reader::ins()->save(Python3, reader::ins()->build(Python3));

    return 0;
}

