#pragma once
#include <vector>
#include <sstream>
#include "flatbuffers/flatbuffers.h"
#include "test_generated.h"


namespace FR
{

#ifndef GX1
#	define GX1(x, y) (!x ? (y) : x)
#endif
#ifndef GX2
#	define GX2(x1, x2, y) ((!x1 || !(x1->x2)) ? (y) : x1->x2)
#endif
#ifndef GX3
#	define GX3(x1, x2, x3, y) (((!x1 || !(x1->x2) || !(x1->x2->x3)) ? (y) : x1->x2->x3))
#endif
#ifndef GX4
#	define GX4(x1, x2, x3, x4, y) (((!x1 || !(x1->x2) || !(x1->x2->x3) || !(x1->x2->x3->x4))) ? (y) : x1->x2->x3->x4)
#endif
#ifndef GX5
#	define GX5(x1, x2, x3, x4, x5, y) (((!x1 || !(x1->x2) || !(x1->x2->x3) || !(x1->x2->x3->x4) || !(x1->x2->x3->x4->x5))) ? (y) : x1->x2->x3->x4->x5)

#endif
#ifndef fs
#	define fs(str) str.empty() ? NULL : fb.CreateString(str)
#endif
#ifndef fv
#	define fv(vct) vct.empty() ? NULL : fb.CreateVector(vct)
#endif
#ifndef FBS_MAX_SIZE
#	define FBS_MAX_SIZE 10 * 1024 * 1024
#endif
#ifndef READ_FBS
#	define READ_FBS(obj, cls, data, len)\
	flatbuffers::FlatBufferBuilder builder_##obj;\
	builder_##obj.PushFlatBuffer((const uint8_t*)data, len);\
	auto obj = flatbuffers::GetRoot<cls>(builder_##obj.GetBufferPointer())
#endif
#ifndef BUILD_FBS
#	define BUILD_FBS(fb, fr)\
	flatbuffers::FlatBufferBuilder fb;\
	fb.Finish(fr.to_fbs(fb))
#endif

static std::stringstream& ss_cut_back_c(std::stringstream &ss)
{
	std::string str = ss.str(); 
	if (1 != str.size()) str = str.substr(0, str.size() - 1); 
	ss.str(""); 
	ss << str; 
	return ss; 
}

template <typename T>
static bool fbs_verify_table(const void* ptr_fbs, const size_t n_len)
{
	if (!ptr_fbs || 0 == n_len || FBS_MAX_SIZE < n_len) return false;
	READ_FBS(obj, T, ptr_fbs, n_len);
	auto verify = flatbuffers::Verifier((const uint8_t*)builder_obj.GetBufferPointer(), builder_obj.GetSize());
	return obj->Verify(verify);
}


// Verify
////////////////////
// reader && builder


template <typename Tc, typename Tf>
static std::vector<Tc> read_vct(const flatbuffers::Vector<flatbuffers::Offset<Tf>> *ptr_fbs)
{
	std::vector<Tc> data;
	for (int i = 0; i < (int)GX2(ptr_fbs, size(), 0); i++)
	{
		auto iter = ptr_fbs->Get(i);
		data.push_back(iter);
	}
	return data;
}
template <typename Tc, typename Tf>
static std::vector<Tc> read_vct_enum(const flatbuffers::Vector<Tf> *ptr_fbs)
{
	std::vector<Tc> data;
	for (int i = 0; i < (int)GX2(ptr_fbs, size(), 0); i++)
	{
		auto iter = ptr_fbs->Get(i);
		data.push_back((Tc)iter);
	}
	return data;
}
template <typename Tc, typename Tf>
static std::vector<Tc> read_vct_str(const flatbuffers::Vector<flatbuffers::Offset<Tf>> *ptr_fbs)
{
	std::vector<Tc> data;
	for (int i = 0; i < (int)GX2(ptr_fbs, size(), 0); i++)
	{
		auto iter = ptr_fbs->Get(i);
		data.push_back(GX2(iter, c_str(), ""));
	}
	return data;
}
template <typename Tc>
static std::string read_vct_2_json(const std::vector<Tc> &data)
{
	std::stringstream ss;
	ss << "[";
	for (auto i : data) ss << i << ",";
	ss_cut_back_c(ss);
	ss << "]";
	return ss.str();
}
template <typename Tc, typename Tf>
static flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Tf>>> build_vct(flatbuffers::FlatBufferBuilder &fb, const std::vector<Tc> vct_data)
{
	std::vector<flatbuffers::Offset<Tf>> vct_fbs;
	for (auto i : vct_data)
	{
		vct_fbs.push_back(i.to_fbs(fb));
	}
	return fv(vct_fbs);
}
template <typename Tc, typename Tf>
static flatbuffers::Offset<flatbuffers::Vector<Tf>> build_vct_enum(flatbuffers::FlatBufferBuilder &fb, const std::vector<Tc> vct_data)
{
	std::vector<Tf> vct_fbs;
	for (auto i : vct_data)
	{
		vct_fbs.push_back(i);
	}
	return fv(vct_fbs);
}
template <typename Tc, typename Tf>
static flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Tf>>> build_vct_str(flatbuffers::FlatBufferBuilder &fb, const std::vector<Tc> vct_data)
{
	std::vector<flatbuffers::Offset<Tf>> vct_fbs;
	for (auto i : vct_data)
	{
		vct_fbs.push_back(fs(i));
	}
	return fv(vct_fbs);
}
static std::string EnumNameSafe(const char **arr, const int nValue)
{
	int nLen = 0;
	while (true) if (nullptr == arr[nLen++]) break;
	if (nValue > nLen - 2) return "";
	return arr[nValue];
}



struct Pair;
struct Account;


struct Pair
{
	std::string first;
	std::string second;

	Pair()
	{
	}
	Pair(const std::string& str)
	{
		Pair(str.data(), str.size());
	}
	Pair(const Pair &src)
	{
		first = src.first;
		second = src.second;
	}
	Pair(const void *ptr_data, const size_t n_len)
	{
		if (!verify(ptr_data, n_len))
		{
			*this = Pair();
			return;
		}
		READ_FBS(fbs_obj, test_fbs::Pair, ptr_data, n_len);
		*this = Pair(fbs_obj);
	}
	Pair(const test_fbs::Pair *ptr_fbs)
	{
		if (!ptr_fbs) return;
		first = GX3(ptr_fbs, first(), c_str(), "");
		second = GX3(ptr_fbs, second(), c_str(), "");
	}
	bool verify(const void* ptr_fbs, const size_t n_len)
	{
		return fbs_verify_table<test_fbs::Pair>(ptr_fbs, n_len);
	}
	flatbuffers::Offset<test_fbs::Pair> to_fbs(flatbuffers::FlatBufferBuilder &fb)
	{
		return test_fbs::CreatePair(fb, fs(first), fs(second));
	}
	std::string to_json(const bool b_simplify = false)
	{
		std::stringstream ss;
		ss << "{";
		if (!b_simplify || (b_simplify && first.size())) ss << "\"first\":" << "\"" << first << "\"" << ",";
		if (!b_simplify || (b_simplify && second.size())) ss << "\"second\":" << "\"" << second << "\"" << ",";
		ss_cut_back_c(ss);
		ss << "}";
		return ss.str();
	}
	friend std::ostream& operator << (std::ostream &ss, Pair &data)
	{
		ss << data.to_json(true);
		return ss;
	}
};

struct Account
{
	test_fbs::Language lang;
	std::vector<test_fbs::Language> langs;
	Pair extra;
	std::vector<Pair> extras;
	std::string test_str;
	std::vector<std::string> test_vct_str;
	int32_t test_int;
	std::vector<int32_t> test_vct_int;
	uint32_t test_uint;
	std::vector<uint32_t> test_vct_uint;
	int16_t test_short;
	std::vector<int16_t> test_vct_short;
	int64_t test_long;
	std::vector<int64_t> test_vct_long;
	uint64_t test_ulong;
	std::vector<uint64_t> test_vct_ulong;
	float test_float;
	std::vector<float> test_vct_float;
	bool test_bool;
	std::vector<bool> test_vct_bool;

	Account()
	{
		lang = test_fbs::Language_NONE;
		test_int = 0;
		test_uint = 0;
		test_short = 0;
		test_long = 0;
		test_ulong = 0;
		test_float = 0.0;
	}
	Account(const Account &src)
	{
		lang = src.lang;
		langs = src.langs;
		extra = src.extra;
		extras = src.extras;
		test_str = src.test_str;
		test_vct_str = src.test_vct_str;
		test_int = src.test_int;
		test_vct_int = src.test_vct_int;
		test_uint = src.test_uint;
		test_vct_uint = src.test_vct_uint;
		test_short = src.test_short;
		test_vct_short = src.test_vct_short;
		test_long = src.test_long;
		test_vct_long = src.test_vct_long;
		test_ulong = src.test_ulong;
		test_vct_ulong = src.test_vct_ulong;
		test_float = src.test_float;
		test_vct_float = src.test_vct_float;
		test_bool = src.test_bool;
		test_vct_bool = src.test_vct_bool;
	}
	Account(const void *ptr_data, const size_t n_len)
	{
		if (!verify(ptr_data, n_len))
		{
			*this = Account();
			return;
		}
		READ_FBS(fbs_obj, test_fbs::Account, ptr_data, n_len);
		*this = Account(fbs_obj);
	}
	Account(const test_fbs::Account *ptr_fbs)
	{
		if (!ptr_fbs) return;
		lang = ptr_fbs->lang();
		langs = read_vct_enum<test_fbs::Language, uint8_t>(ptr_fbs->langs());
		extra = ptr_fbs->extra();
		extras = read_vct<Pair, test_fbs::Pair>(ptr_fbs->extras());
		test_str = GX3(ptr_fbs, test_str(), c_str(), "");
		test_vct_str = read_vct_str<std::string, flatbuffers::String>(ptr_fbs->test_vct_str());
		test_int = ptr_fbs->test_int();
		test_vct_int = read_vct_enum<int32_t, int32_t>(ptr_fbs->test_vct_int());
		test_uint = ptr_fbs->test_uint();
		test_vct_uint = read_vct_enum<uint32_t, uint32_t>(ptr_fbs->test_vct_uint());
		test_short = ptr_fbs->test_short();
		test_vct_short = read_vct_enum<int16_t, int16_t>(ptr_fbs->test_vct_short());
		test_long = ptr_fbs->test_long();
		test_vct_long = read_vct_enum<int64_t, int64_t>(ptr_fbs->test_vct_long());
		test_ulong = ptr_fbs->test_ulong();
		test_vct_ulong = read_vct_enum<uint64_t, uint64_t>(ptr_fbs->test_vct_ulong());
		test_float = ptr_fbs->test_float();
		test_vct_float = read_vct_enum<float, float>(ptr_fbs->test_vct_float());
		test_bool = ptr_fbs->test_bool();
		test_vct_bool = read_vct_enum<bool, uint8_t>(ptr_fbs->test_vct_bool());
	}
	bool verify(const void* ptr_fbs, const size_t n_len)
	{
		return fbs_verify_table<test_fbs::Account>(ptr_fbs, n_len);
	}
	flatbuffers::Offset<test_fbs::Account> to_fbs(flatbuffers::FlatBufferBuilder &fb)
	{
		return test_fbs::CreateAccount(fb, lang, build_vct_enum<test_fbs::Language, uint8_t>(fb, langs), extra.to_fbs(fb), build_vct<Pair, test_fbs::Pair>(fb, extras), fs(test_str), build_vct_str<std::string, flatbuffers::String>(fb, test_vct_str), test_int, build_vct_enum<int32_t, int32_t>(fb, test_vct_int), test_uint, build_vct_enum<uint32_t, uint32_t>(fb, test_vct_uint), test_short, build_vct_enum<int16_t, int16_t>(fb, test_vct_short), test_long, build_vct_enum<int64_t, int64_t>(fb, test_vct_long), test_ulong, build_vct_enum<uint64_t, uint64_t>(fb, test_vct_ulong), test_float, build_vct_enum<float, float>(fb, test_vct_float), test_bool, build_vct_enum<bool, uint8_t>(fb, test_vct_bool));
	}
	std::string to_json(const bool b_simplify = false)
	{
		std::stringstream ss;
		ss << "{";
		ss << "\"lang\":" << "\"" << EnumNameLanguage(lang) << "\"" << ",";
		if (!b_simplify || (b_simplify && langs.size())) ss << "\"langs\":" << read_vct_2_json<test_fbs::Language>(langs) << ",";
		ss << "\"extra\":" << extra.to_json(b_simplify) << ",";
		if (!b_simplify || (b_simplify && extras.size())) ss << "\"extras\":" << read_vct_2_json<Pair>(extras) << ",";
		if (!b_simplify || (b_simplify && test_str.size())) ss << "\"test_str\":" << "\"" << test_str << "\"" << ",";
		if (!b_simplify || (b_simplify && test_vct_str.size())) ss << "\"test_vct_str\":" << read_vct_2_json<std::string>(test_vct_str) << ",";
		ss << "\"test_int\":" << (int32_t)test_int << ",";
		if (!b_simplify || (b_simplify && test_vct_int.size())) ss << "\"test_vct_int\":" << read_vct_2_json<int32_t>(test_vct_int) << ",";
		ss << "\"test_uint\":" << (uint32_t)test_uint << ",";
		if (!b_simplify || (b_simplify && test_vct_uint.size())) ss << "\"test_vct_uint\":" << read_vct_2_json<uint32_t>(test_vct_uint) << ",";
		ss << "\"test_short\":" << (int16_t)test_short << ",";
		if (!b_simplify || (b_simplify && test_vct_short.size())) ss << "\"test_vct_short\":" << read_vct_2_json<int16_t>(test_vct_short) << ",";
		ss << "\"test_long\":" << (int64_t)test_long << ",";
		if (!b_simplify || (b_simplify && test_vct_long.size())) ss << "\"test_vct_long\":" << read_vct_2_json<int64_t>(test_vct_long) << ",";
		ss << "\"test_ulong\":" << (uint64_t)test_ulong << ",";
		if (!b_simplify || (b_simplify && test_vct_ulong.size())) ss << "\"test_vct_ulong\":" << read_vct_2_json<uint64_t>(test_vct_ulong) << ",";
		ss << "\"test_float\":" << "\"" << (float)test_float << "\"" << ",";
		if (!b_simplify || (b_simplify && test_vct_float.size())) ss << "\"test_vct_float\":" << read_vct_2_json<float>(test_vct_float) << ",";
		ss << "\"test_bool\":" << "\"" << (bool)test_bool << "\"" << ",";
		if (!b_simplify || (b_simplify && test_vct_bool.size())) ss << "\"test_vct_bool\":" << read_vct_2_json<bool>(test_vct_bool) << ",";
		ss_cut_back_c(ss);
		ss << "}";
		return ss.str();
	}
	friend std::ostream& operator << (std::ostream &ss, Account &data)
	{
		ss << data.to_json(true);
		return ss;
	}
};

}
