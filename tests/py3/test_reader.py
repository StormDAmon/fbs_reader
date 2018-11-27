import flatbuffers

def fs(fb, str):
	if 0 == len(str):
		return 0
	return fb.CreateString(str)

def read_vct(func, length):
	l = []
	for i in range(length):
		l.append(func(i))
	return l

def build_vct_cls(fb, list, func_start):
	length = len(list)
	l = []
	for i in list:
		l.append(i.to_fbs(fb))
	func_start(fb, length)
	for i in l:
		fb.PrependUOffsetTRelative(i)
	return fb.EndVector(length)



class Pair(object):
	first = str()
	second = str()

	def __init__(self, buf=None):
		if None == buf:
			return
		Pair = test_fbs.	}
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
		first = ptr_fbs->first();
		second = ptr_fbs->second();
	}
	bool verify(const void* ptr_fbs, const size_t n_len)
	{
		return fbs_verify_table<test_fbs::Pair>(ptr_fbs, n_len);
	}
	flatbuffers::Offset<test_fbs::Pair> to_fbs(flatbuffers::FlatBufferBuilder &fb)
	{
		return test_fbs::CreatePair(fb, first, second);
	}
	std::string to_json(const bool b_simplify = false)
	{
		std::stringstream ss;
		ss << "{";
		ss << "\"first\":" << "\"" << (str())first << "\"" << ",";
		ss << "\"second\":" << "\"" << (str())second << "\"" << ",";
		ss_cut_back_c(ss);
		ss << "}";
		return ss.str();
	}
	friend std::ostream& operator << (std::ostream &ss, Pair &data)
	{
		ss << data.to_json();
		return ss;
	}
};

class Account(object):
	lang = test_fbs::Language
	extras = list()
	test_str = str()
	test_vct_str = list()
	test_int = int()
	test_vct_int = list()
	test_uint = int()
	test_vct_uint = list()
	test_short = int()
	test_vct_short = list()
	test_long = int()
	test_vct_long = list()
	test_ulong = int()
	test_vct_ulong = list()
	test_float = float()
	test_vct_float = list()
	test_bool = bool()
	test_vct_bool = list()

	def __init__(self, buf=None):
		if None == buf:
			return
		Account = test_fbs.		lang = test_fbs::Language_NONE;
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
		extras = ptr_fbs->extras();
		test_str = ptr_fbs->test_str();
		test_vct_str = ptr_fbs->test_vct_str();
		test_int = ptr_fbs->test_int();
		test_vct_int = ptr_fbs->test_vct_int();
		test_uint = ptr_fbs->test_uint();
		test_vct_uint = ptr_fbs->test_vct_uint();
		test_short = ptr_fbs->test_short();
		test_vct_short = ptr_fbs->test_vct_short();
		test_long = ptr_fbs->test_long();
		test_vct_long = ptr_fbs->test_vct_long();
		test_ulong = ptr_fbs->test_ulong();
		test_vct_ulong = ptr_fbs->test_vct_ulong();
		test_float = ptr_fbs->test_float();
		test_vct_float = ptr_fbs->test_vct_float();
		test_bool = ptr_fbs->test_bool();
		test_vct_bool = ptr_fbs->test_vct_bool();
	}
	bool verify(const void* ptr_fbs, const size_t n_len)
	{
		return fbs_verify_table<test_fbs::Account>(ptr_fbs, n_len);
	}
	flatbuffers::Offset<test_fbs::Account> to_fbs(flatbuffers::FlatBufferBuilder &fb)
	{
		return test_fbs::CreateAccount(fb, lang, extras.to_fbs(fb), test_str, test_vct_str.to_fbs(fb), test_int, test_vct_int.to_fbs(fb), test_uint, test_vct_uint.to_fbs(fb), test_short, test_vct_short.to_fbs(fb), test_long, test_vct_long.to_fbs(fb), test_ulong, test_vct_ulong.to_fbs(fb), test_float, test_vct_float.to_fbs(fb), test_bool, test_vct_bool.to_fbs(fb));
	}
	std::string to_json(const bool b_simplify = false)
	{
		std::stringstream ss;
		ss << "{";
		ss << "\"lang\":" << "\"" << EnumNameLanguage(lang) << "\"" << ",";
		ss << "\"extras\":" << extras.to_json(b_simplify) << ",";
		ss << "\"test_str\":" << "\"" << (str())test_str << "\"" << ",";
		ss << "\"test_vct_str\":" << test_vct_str.to_json(b_simplify) << ",";
		ss << "\"test_int\":" << (int())test_int << ",";
		ss << "\"test_vct_int\":" << test_vct_int.to_json(b_simplify) << ",";
		ss << "\"test_uint\":" << (int())test_uint << ",";
		ss << "\"test_vct_uint\":" << test_vct_uint.to_json(b_simplify) << ",";
		ss << "\"test_short\":" << (int())test_short << ",";
		ss << "\"test_vct_short\":" << test_vct_short.to_json(b_simplify) << ",";
		ss << "\"test_long\":" << (int())test_long << ",";
		ss << "\"test_vct_long\":" << test_vct_long.to_json(b_simplify) << ",";
		ss << "\"test_ulong\":" << (int())test_ulong << ",";
		ss << "\"test_vct_ulong\":" << test_vct_ulong.to_json(b_simplify) << ",";
		ss << "\"test_float\":" << "\"" << (float())test_float << "\"" << ",";
		ss << "\"test_vct_float\":" << test_vct_float.to_json(b_simplify) << ",";
		ss << "\"test_bool\":" << "\"" << (bool())test_bool << "\"" << ",";
		ss << "\"test_vct_bool\":" << test_vct_bool.to_json(b_simplify) << ",";
		ss_cut_back_c(ss);
		ss << "}";
		return ss.str();
	}
	friend std::ostream& operator << (std::ostream &ss, Account &data)
	{
		ss << data.to_json();
		return ss;
	}
};

