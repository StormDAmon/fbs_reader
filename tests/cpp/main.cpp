//
//  mian.cpp
//
//  Created by Storm DAmon on 2018/11/7.
//

#include "flatbuffers/flatbuffers.h"
#include "test_reader.h"

#include <iostream>

int main()
{
	FR::Account acc;
	acc.lang = test_fbs::Language_CHT;

	FR::Pair extra;
	extra.first = "1";
	extra.second = "1";

	acc.extras.push_back(extra);
	acc.test_str = "sf";
	acc.test_bool = true;
	acc.test_float = 123.42;
	acc.test_short = 34;
	std::cout << acc.to_json() << std::endl;
	

	BUILD_FBS(fb, acc);
	std::string str(fb.GetSize(), 0);
	memcpy((void*)str.data(), fb.GetBufferPointer(), fb.GetSize());
	
	
	FR::Account acc_tmp(fb.GetBufferPointer(), fb.GetSize());
	std::cout << acc_tmp.to_json() << std::endl;

    return 0;
}

