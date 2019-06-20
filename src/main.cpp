//
//  mian.cpp
//
//  Created by Storm DAmon on 2018/10/18.
//
#include "reader.h"
#include "rl_path.h"

#include <iostream>


#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>


std::vector<std::string> getFilesInDir(const std::string& strDir)
{
    std::vector<std::string> vctData;
    if (strDir.empty()) return vctData;

    struct stat s;
    lstat(strDir.data(), &s);
    if(!S_ISDIR(s.st_mode)) return vctData;
    
    struct dirent* filename;
    DIR * dir;
    dir = opendir(strDir.data());
    if(!dir) return vctData;
    while((filename = readdir(dir)))
    {
        if( strcmp( filename->d_name , "." ) == 0 ||
            strcmp( filename->d_name , "..") == 0)
            continue;   
        vctData.push_back(filename->d_name);
    }   
    return vctData;
}


int main(int argc, char* argv[])
{
	bool bAllFile = false;
	if (2 != argc)
	{
		bAllFile = true;
	}
	std::vector<std::string> vctPath;
	std::string strRunPath = get_run_path();

	if (bAllFile)
	{
		auto vctFile = getFilesInDir(strRunPath);
		for (const auto& i : vctFile)
		{
			if (std::string::npos == i.find(".fbs")) continue;
			vctPath.push_back(strRunPath + PATH_L + i);
		}
	}
	else
	{
		std::string strPath = argv[1];
		if (std::string::npos == strPath.find(":") && '/' != strPath[0])
		{
			strPath = strRunPath + PATH_L + strPath;
		}
		vctPath.push_back(strPath);
	}



	for (const auto& i : vctPath)
	{
		if (!reader::ins()->load(i))
		{
			std::cout << "load [" << i << "] failed" << std::endl;
			continue;
		}
		std::cout << "load [" << i << "] success" << std::endl;
		if (!reader::ins()->save(CPP, reader::ins()->build(CPP)))
		{
			std::cout << "build [" << i << "] failed" << std::endl;
			continue;
		}
		std::cout << "build [" << i << "] success" << std::endl;

		//reader::ins()->save(Python3, reader::ins()->build(Python3));
	}


    return 0;
}

