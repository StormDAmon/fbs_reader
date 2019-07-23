#pragma once
#include <vector>
#include <sstream>
#include "flatbuffers/flatbuffers.h"
#include "mse_generated.h"


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
	fb.Finish(fr.toFbs(fb))
#endif


#ifndef FR_FUNC
#	define FR_FUNC
static std::stringstream& ssCutEndC(std::stringstream &ss)
{
	std::string str = ss.str(); 
	if (1 != str.size()) str = str.substr(0, str.size() - 1); 
	ss.str(""); 
	ss << str; 
	return ss; 
}

template <typename T>
static bool fbsVerifyTable(const void* pFbs, const size_t nLen)
{
	if (!pFbs || 0 == nLen || FBS_MAX_SIZE < nLen) return false;
	READ_FBS(obj, T, pFbs, nLen);
	auto verify = flatbuffers::Verifier((const uint8_t*)builder_obj.GetBufferPointer(), builder_obj.GetSize());
	return obj->Verify(verify);
}


// Verify
////////////////////
// reader && builder


template <typename Tc, typename Tf>
static std::vector<Tc> readVct(const flatbuffers::Vector<flatbuffers::Offset<Tf>> *pFbs)
{
	std::vector<Tc> data;
	for (int i = 0; i < (int)GX2(pFbs, size(), 0); i++)
	{
		auto iter = pFbs->Get(i);
		data.push_back(iter);
	}
	return data;
}
template <typename Tc, typename Tf>
static std::vector<Tc> readVctEnum(const flatbuffers::Vector<Tf> *pFbs)
{
	std::vector<Tc> data;
	for (int i = 0; i < (int)GX2(pFbs, size(), 0); i++)
	{
		auto iter = pFbs->Get(i);
		data.push_back((Tc)iter);
	}
	return data;
}
template <typename Tc, typename Tf>
static std::vector<Tc> readVctStr(const flatbuffers::Vector<flatbuffers::Offset<Tf>> *pFbs)
{
	std::vector<Tc> data;
	for (int i = 0; i < (int)GX2(pFbs, size(), 0); i++)
	{
		auto iter = pFbs->Get(i);
		data.push_back(GX2(iter, c_str(), ""));
	}
	return data;
}
template <typename Tc>
static std::string readVctToJson(const std::vector<Tc> &data)
{
	std::stringstream ss;
	ss << "[";
	for (auto i : data) ss << i << ",";
	ssCutEndC(ss);
	ss << "]";
	return ss.str();
}
template <typename Tc, typename Tf>
static flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Tf>>> buildVct(flatbuffers::FlatBufferBuilder &fb, const std::vector<Tc> vctData)
{
	std::vector<flatbuffers::Offset<Tf>> vctFbs;
	for (auto i : vctData)
	{
		vctFbs.push_back(i.toFbs(fb));
	}
	return fv(vctFbs);
}
template <typename Tc, typename Tf>
static flatbuffers::Offset<flatbuffers::Vector<Tf>> buildVctEnum(flatbuffers::FlatBufferBuilder &fb, const std::vector<Tc> vctData)
{
	std::vector<Tf> vctFbs;
	for (auto i : vctData)
	{
		vctFbs.push_back(i);
	}
	return fv(vctFbs);
}
template <typename Tc, typename Tf>
static flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Tf>>> buildVctStr(flatbuffers::FlatBufferBuilder &fb, const std::vector<Tc> vctData)
{
	std::vector<flatbuffers::Offset<Tf>> vctFbs;
	for (auto i : vctData)
	{
		vctFbs.push_back(fs(i));
	}
	return fv(vctFbs);
}
static std::string EnumNameSafe(const char* const *arr, const int nValue)
{
	int nLen = 0;
	while (true) if (nullptr == arr[nLen++]) break;
	if (nValue > nLen - 2) return "";
	return arr[nValue];
}
static int EnumNameSafe(const char* const *arr, const std::string &strName)
{
	int nValue = 0;
	int i = 0;
	while (nullptr != arr[i])
	{
		if (arr[i] == strName)
		{
			nValue = i;
			break;
		}
		i++;
	}
	return nValue;
}
#endif



struct engLoc;
struct serLoc;
struct MsgLoc;
struct MsgData;

#ifndef FR_engLoc
#define FR_engLoc
struct engLoc
{
	std::string id;
	std::string ip;
	int32_t port;

	engLoc()
	{
		port = 0;
	}
	engLoc(const std::vector<int8_t>& vct)
	{
		std::string str;
		str.insert(str.begin(), vct.begin(), vct.end());
		*this = engLoc(str);
	}
	engLoc(const std::string& str)
	{
		*this = engLoc(str.data(), str.size());
	}
	engLoc(const engLoc &src)
	{
		id = src.id;
		ip = src.ip;
		port = src.port;
	}
	engLoc(const void *pData, const size_t nLen)
	{
		if (!verify(pData, nLen))
		{
			*this = engLoc();
			return;
		}
		READ_FBS(fbsObj, MSE::engLoc, pData, nLen);
		*this = engLoc(fbsObj);
	}
	engLoc(const MSE::engLoc *pFbs)
	{
		if (!pFbs) return;
		id = GX3(pFbs, id(), c_str(), "");
		ip = GX3(pFbs, ip(), c_str(), "");
		port = pFbs->port();
	}
	bool verify(const void* pFbs, const size_t nLen)
	{
		return fbsVerifyTable<MSE::engLoc>(pFbs, nLen);
	}
	flatbuffers::Offset<MSE::engLoc> toFbs(flatbuffers::FlatBufferBuilder &fb)
	{
		return MSE::CreateengLoc(fb,
			 fs(id),
			 fs(ip),
			 port);
	}
	std::string toStr()
	{
		flatbuffers::FlatBufferBuilder fb;
		fb.Finish(toFbs(fb));
		std::string str(fb.GetSize(), 0);
		memcpy((void*)str.data(), fb.GetBufferPointer(), fb.GetSize());
		return str;
	}
	std::string toJson(const bool bSimplify = false)
	{
		std::stringstream ss;
		ss << "{";
		if (!bSimplify || (bSimplify && id.size())) ss << "\"id\":" << "\"" << id << "\"" << ",";
		if (!bSimplify || (bSimplify && ip.size())) ss << "\"ip\":" << "\"" << ip << "\"" << ",";
		ss << "\"port\":" << (int32_t)port << ",";
		ssCutEndC(ss);
		ss << "}";
		return ss.str();
	}
	friend std::ostream& operator <<(std::ostream &ss, engLoc &data)
	{
		ss << data.toJson(true);
		return ss;
	}
	friend bool operator ==(const engLoc& a, const engLoc& b)
	{
		if (a.id != b.id) return false;
		if (a.ip != b.ip) return false;
		if (a.port != b.port) return false;
		return true;
	}
	friend bool operator !=(const engLoc& a, const engLoc& b)
	{
		return !(a == b);
	}
	friend bool operator <(const engLoc& a, const engLoc& b)
	{
		if (a == b) return false;
		if (a.id > b.id) return false;
		if (a.ip > b.ip) return false;
		if (a.port > b.port) return false;
		return true;
	}
	friend bool operator >(const engLoc& a, const engLoc& b)
	{
		if (a == b) return false;
		if (a.id < b.id) return false;
		if (a.ip < b.ip) return false;
		if (a.port < b.port) return false;
		return true;
	}
};
#endif


#ifndef FR_serLoc
#define FR_serLoc
struct serLoc
{
	std::string id;
	std::string hardID;

	serLoc()
	{
	}
	serLoc(const std::vector<int8_t>& vct)
	{
		std::string str;
		str.insert(str.begin(), vct.begin(), vct.end());
		*this = serLoc(str);
	}
	serLoc(const std::string& str)
	{
		*this = serLoc(str.data(), str.size());
	}
	serLoc(const serLoc &src)
	{
		id = src.id;
		hardID = src.hardID;
	}
	serLoc(const void *pData, const size_t nLen)
	{
		if (!verify(pData, nLen))
		{
			*this = serLoc();
			return;
		}
		READ_FBS(fbsObj, MSE::serLoc, pData, nLen);
		*this = serLoc(fbsObj);
	}
	serLoc(const MSE::serLoc *pFbs)
	{
		if (!pFbs) return;
		id = GX3(pFbs, id(), c_str(), "");
		hardID = GX3(pFbs, hardID(), c_str(), "");
	}
	bool verify(const void* pFbs, const size_t nLen)
	{
		return fbsVerifyTable<MSE::serLoc>(pFbs, nLen);
	}
	flatbuffers::Offset<MSE::serLoc> toFbs(flatbuffers::FlatBufferBuilder &fb)
	{
		return MSE::CreateserLoc(fb,
			 fs(id),
			 fs(hardID));
	}
	std::string toStr()
	{
		flatbuffers::FlatBufferBuilder fb;
		fb.Finish(toFbs(fb));
		std::string str(fb.GetSize(), 0);
		memcpy((void*)str.data(), fb.GetBufferPointer(), fb.GetSize());
		return str;
	}
	std::string toJson(const bool bSimplify = false)
	{
		std::stringstream ss;
		ss << "{";
		if (!bSimplify || (bSimplify && id.size())) ss << "\"id\":" << "\"" << id << "\"" << ",";
		if (!bSimplify || (bSimplify && hardID.size())) ss << "\"hardID\":" << "\"" << hardID << "\"" << ",";
		ssCutEndC(ss);
		ss << "}";
		return ss.str();
	}
	friend std::ostream& operator <<(std::ostream &ss, serLoc &data)
	{
		ss << data.toJson(true);
		return ss;
	}
	friend bool operator ==(const serLoc& a, const serLoc& b)
	{
		if (a.id != b.id) return false;
		if (a.hardID != b.hardID) return false;
		return true;
	}
	friend bool operator !=(const serLoc& a, const serLoc& b)
	{
		return !(a == b);
	}
	friend bool operator <(const serLoc& a, const serLoc& b)
	{
		if (a == b) return false;
		if (a.id > b.id) return false;
		if (a.hardID > b.hardID) return false;
		return true;
	}
	friend bool operator >(const serLoc& a, const serLoc& b)
	{
		if (a == b) return false;
		if (a.id < b.id) return false;
		if (a.hardID < b.hardID) return false;
		return true;
	}
};
#endif


#ifndef FR_MsgLoc
#define FR_MsgLoc
struct MsgLoc
{
	MSE::MsgType type;
	MSE::MsgMode mode;
	engLoc engine;
	serLoc service;

	MsgLoc()
	{
		type = MSE::MsgType_Once;
		mode = MSE::MsgMode_Asy;
	}
	MsgLoc(const std::vector<int8_t>& vct)
	{
		std::string str;
		str.insert(str.begin(), vct.begin(), vct.end());
		*this = MsgLoc(str);
	}
	MsgLoc(const std::string& str)
	{
		*this = MsgLoc(str.data(), str.size());
	}
	MsgLoc(const MsgLoc &src)
	{
		type = src.type;
		mode = src.mode;
		engine = src.engine;
		service = src.service;
	}
	MsgLoc(const void *pData, const size_t nLen)
	{
		if (!verify(pData, nLen))
		{
			*this = MsgLoc();
			return;
		}
		READ_FBS(fbsObj, MSE::MsgLoc, pData, nLen);
		*this = MsgLoc(fbsObj);
	}
	MsgLoc(const MSE::MsgLoc *pFbs)
	{
		if (!pFbs) return;
		type = pFbs->type();
		mode = pFbs->mode();
		engine = pFbs->engine();
		service = pFbs->service();
	}
	bool verify(const void* pFbs, const size_t nLen)
	{
		return fbsVerifyTable<MSE::MsgLoc>(pFbs, nLen);
	}
	flatbuffers::Offset<MSE::MsgLoc> toFbs(flatbuffers::FlatBufferBuilder &fb)
	{
		return MSE::CreateMsgLoc(fb,
			 type,
			 mode,
			 engine.toFbs(fb),
			 service.toFbs(fb));
	}
	std::string toStr()
	{
		flatbuffers::FlatBufferBuilder fb;
		fb.Finish(toFbs(fb));
		std::string str(fb.GetSize(), 0);
		memcpy((void*)str.data(), fb.GetBufferPointer(), fb.GetSize());
		return str;
	}
	std::string toJson(const bool bSimplify = false)
	{
		std::stringstream ss;
		ss << "{";
		ss << "\"type\":" << "\"" << EnumNameMsgType(type) << "\"" << ",";
		ss << "\"mode\":" << "\"" << EnumNameMsgMode(mode) << "\"" << ",";
		ss << "\"engine\":" << engine.toJson(bSimplify) << ",";
		ss << "\"service\":" << service.toJson(bSimplify) << ",";
		ssCutEndC(ss);
		ss << "}";
		return ss.str();
	}
	friend std::ostream& operator <<(std::ostream &ss, MsgLoc &data)
	{
		ss << data.toJson(true);
		return ss;
	}
	friend bool operator ==(const MsgLoc& a, const MsgLoc& b)
	{
		if (a.type != b.type) return false;
		if (a.mode != b.mode) return false;
		if (a.engine != b.engine) return false;
		if (a.service != b.service) return false;
		return true;
	}
	friend bool operator !=(const MsgLoc& a, const MsgLoc& b)
	{
		return !(a == b);
	}
	friend bool operator <(const MsgLoc& a, const MsgLoc& b)
	{
		if (a == b) return false;
		if (a.type > b.type) return false;
		if (a.mode > b.mode) return false;
		if (a.engine > b.engine) return false;
		if (a.service > b.service) return false;
		return true;
	}
	friend bool operator >(const MsgLoc& a, const MsgLoc& b)
	{
		if (a == b) return false;
		if (a.type < b.type) return false;
		if (a.mode < b.mode) return false;
		if (a.engine < b.engine) return false;
		if (a.service < b.service) return false;
		return true;
	}
};
#endif


#ifndef FR_MsgData
#define FR_MsgData
struct MsgData
{
	MsgLoc from;
	MsgLoc to;
	MSE::ErrCode res;
	std::string bizName;
	std::vector<int8_t> data;
	uint64_t args;

	MsgData()
	{
		res = MSE::ErrCode_Success;
		args = 0;
	}
	MsgData(const std::vector<int8_t>& vct)
	{
		std::string str;
		str.insert(str.begin(), vct.begin(), vct.end());
		*this = MsgData(str);
	}
	MsgData(const std::string& str)
	{
		*this = MsgData(str.data(), str.size());
	}
	MsgData(const MsgData &src)
	{
		from = src.from;
		to = src.to;
		res = src.res;
		bizName = src.bizName;
		data = src.data;
		args = src.args;
	}
	MsgData(const void *pData, const size_t nLen)
	{
		if (!verify(pData, nLen))
		{
			*this = MsgData();
			return;
		}
		READ_FBS(fbsObj, MSE::MsgData, pData, nLen);
		*this = MsgData(fbsObj);
	}
	MsgData(const MSE::MsgData *pFbs)
	{
		if (!pFbs) return;
		from = pFbs->from();
		to = pFbs->to();
		res = pFbs->res();
		bizName = GX3(pFbs, bizName(), c_str(), "");
		data = readVctEnum<int8_t, int8_t>(pFbs->data());
		args = pFbs->args();
	}
	bool verify(const void* pFbs, const size_t nLen)
	{
		return fbsVerifyTable<MSE::MsgData>(pFbs, nLen);
	}
	flatbuffers::Offset<MSE::MsgData> toFbs(flatbuffers::FlatBufferBuilder &fb)
	{
		return MSE::CreateMsgData(fb,
			 from.toFbs(fb),
			 to.toFbs(fb),
			 res,
			 fs(bizName),
			 buildVctEnum<int8_t, int8_t>(fb, data),
			 args);
	}
	std::string toStr()
	{
		flatbuffers::FlatBufferBuilder fb;
		fb.Finish(toFbs(fb));
		std::string str(fb.GetSize(), 0);
		memcpy((void*)str.data(), fb.GetBufferPointer(), fb.GetSize());
		return str;
	}
	std::string toJson(const bool bSimplify = false)
	{
		std::stringstream ss;
		ss << "{";
		ss << "\"from\":" << from.toJson(bSimplify) << ",";
		ss << "\"to\":" << to.toJson(bSimplify) << ",";
		ss << "\"res\":" << "\"" << EnumNameErrCode(res) << "\"" << ",";
		if (!bSimplify || (bSimplify && bizName.size())) ss << "\"bizName\":" << "\"" << bizName << "\"" << ",";
		if (!bSimplify || (bSimplify && data.size())) ss << "\"data\":" << readVctToJson<int8_t>(data) << ",";
		ss << "\"args\":" << (uint64_t)args << ",";
		ssCutEndC(ss);
		ss << "}";
		return ss.str();
	}
	friend std::ostream& operator <<(std::ostream &ss, MsgData &data)
	{
		ss << data.toJson(true);
		return ss;
	}
	friend bool operator ==(const MsgData& a, const MsgData& b)
	{
		if (a.from != b.from) return false;
		if (a.to != b.to) return false;
		if (a.res != b.res) return false;
		if (a.bizName != b.bizName) return false;
		if (a.data != b.data) return false;
		if (a.args != b.args) return false;
		return true;
	}
	friend bool operator !=(const MsgData& a, const MsgData& b)
	{
		return !(a == b);
	}
	friend bool operator <(const MsgData& a, const MsgData& b)
	{
		if (a == b) return false;
		if (a.from > b.from) return false;
		if (a.to > b.to) return false;
		if (a.res > b.res) return false;
		if (a.bizName > b.bizName) return false;
		if (a.data > b.data) return false;
		if (a.args > b.args) return false;
		return true;
	}
	friend bool operator >(const MsgData& a, const MsgData& b)
	{
		if (a == b) return false;
		if (a.from < b.from) return false;
		if (a.to < b.to) return false;
		if (a.res < b.res) return false;
		if (a.bizName < b.bizName) return false;
		if (a.data < b.data) return false;
		if (a.args < b.args) return false;
		return true;
	}
};
#endif


}
