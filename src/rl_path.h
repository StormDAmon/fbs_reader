//
//  rl_path.h
//
//  Created by Storm DAmon on 2018/7/8.
//
#pragma once



#include <string>
#include <stdio.h> 

#define MAX_PATH 512
#ifdef _WIN32
#	include <direct.h>  
#	include <windows.h>
#	define PATH_L "\\"
#else
#	include <unistd.h>
#	define PATH_L "/"
#endif



static std::string get_run_path()
{
	char buffer[MAX_PATH];
#ifdef _WIN32
	_getcwd(buffer, MAX_PATH);
#else
	getcwd(buffer, MAX_PATH);
#endif

	return buffer;
}



