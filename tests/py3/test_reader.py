import flatbuffers

def read_str(fb):
	if None == fb:
		return ''
	return fb.decode('utf-8')
def read_vct(fb, name):
	l = []
	for i in range(eval('fb.%sLength()' % name)):
		l.append(eval('fb.%s(%d)' % (name, i)))
	return l
def read_vct_cls(fb, name, cls):
	l = []
	for i in range(eval('fb.%sLength()' % name)):
		l.append(eval('%s(fb.%s(%d))' % (cls, name, i)))
	return l
def fs(fb, str):
	if 0 == len(str):
		return 0
	return fb.CreateString(str)
def build_vct(fb, list, func_start, t):
	length = len(list)
	l = []
	for i in list:
		l.insert(0, i)
	func_start(fb, length)
	for i in l:
		eval('fb.Prepend%s(%d)' % (t, i))
	return fb.EndVector(length)
def build_vct_bool(fb, list, func_start):
	return build_vct(fb, list, func_start, 'Bool')
def build_vct_enum(fb, list, func_start):
	return build_vct(fb, list, func_start, 'Byte')
def build_vct_byte(fb, list, func_start):
	return build_vct(fb, list, func_start, 'Byte')
def build_vct_ubyte(fb, list, func_start):
	return build_vct(fb, list, func_start, 'Uint8')
def build_vct_short(fb, list, func_start):
	return build_vct(fb, list, func_start, 'Int16')
def build_vct_ushort(fb, list, func_start):
	return build_vct(fb, list, func_start, 'Uint16')
def build_vct_int(fb, list, func_start):
	return build_vct(fb, list, func_start, 'Int32')
def build_vct_uint(fb, list, func_start):
	return build_vct(fb, list, func_start, 'Uint32')
def build_vct_long(fb, list, func_start):
	return build_vct(fb, list, func_start, 'Int64')
def build_vct_ulong(fb, list, func_start):
	return build_vct(fb, list, func_start, 'Uint64')
def build_vct_float(fb, list, func_start):
	return build_vct(fb, list, func_start, 'Float64')
def build_vct_cls(fb, list, func_start):
	length = len(list)
	l = []
	for i in list:
		l.insert(0, i.to_fbs(fb))
	func_start(fb, length)
	for i in l:
		fb.PrependUOffsetTRelative(i)
	return fb.EndVector(length)
def build_vct_str(fb, list, func_start):
	length = len(list)
	l = []
	for i in list:
		l.insert(0, fs(fb, i))
	func_start(fb, length)
	for i in l:
		fb.PrependUOffsetTRelative(i)
	return fb.EndVector(length)



import test_fbs.Language




import test_fbs.Pair
class Pair(object):
	first = ''
	second = ''

	def __init__(self, buf=None):
		if None == buf:
			return
		fbs = buf
		if 'bytearray' == type(buf).__name__:
			fbs = test_fbs.Pair.Pair.GetRootAsPair(buf, 0)
		self.first = read_str(fbs.First())
		self.second = read_str(fbs.Second())

	def to_fbs(self, fb=None):
		is_finish = False
		if None == fb:
			is_finish = True
			fb = flatbuffers.Builder(0)
		_first = fs(fb, self.first)
		_second = fs(fb, self.second)

		test_fbs.Pair.PairStart(fb)
		test_fbs.Pair.PairAddFirst(fb, _first)
		test_fbs.Pair.PairAddSecond(fb, _second)
		if is_finish:
			fb.Finish(test_fbs.Pair.PairEnd(fb))
			return fb.Output()
		return test_fbs.Pair.PairEnd(fb)



import test_fbs.Account
class Account(object):
	lang = test_fbs.Language.Language.NONE
	langs = []
	extra = Pair()
	extras = []
	test_str = ''
	test_vct_str = []
	test_int = 0
	test_vct_int = []
	test_uint = 0
	test_vct_uint = []
	test_short = 0
	test_vct_short = []
	test_long = 0
	test_vct_long = []
	test_ulong = 0
	test_vct_ulong = []
	test_float = 0.0
	test_vct_float = []
	test_bool = False
	test_vct_bool = []

	def __init__(self, buf=None):
		if None == buf:
			return
		fbs = buf
		if 'bytearray' == type(buf).__name__:
			fbs = test_fbs.Account.Account.GetRootAsAccount(buf, 0)
		self.lang = fbs.Lang()
		self.langs = read_vct(fbs, 'Langs')
		self.extra = Pair(fbs.Extra())
		self.extras = read_vct_cls(fbs, 'Extras', 'Pair')
		self.test_str = read_str(fbs.TestStr())
		self.test_vct_str = read_vct(fbs, 'TestVctStr')
		self.test_int = fbs.TestInt()
		self.test_vct_int = read_vct(fbs, 'TestVctInt')
		self.test_uint = fbs.TestUint()
		self.test_vct_uint = read_vct(fbs, 'TestVctUint')
		self.test_short = fbs.TestShort()
		self.test_vct_short = read_vct(fbs, 'TestVctShort')
		self.test_long = fbs.TestLong()
		self.test_vct_long = read_vct(fbs, 'TestVctLong')
		self.test_ulong = fbs.TestUlong()
		self.test_vct_ulong = read_vct(fbs, 'TestVctUlong')
		self.test_float = fbs.TestFloat()
		self.test_vct_float = read_vct(fbs, 'TestVctFloat')
		self.test_bool = fbs.TestBool()
		self.test_vct_bool = read_vct(fbs, 'TestVctBool')

	def to_fbs(self, fb=None):
		is_finish = False
		if None == fb:
			is_finish = True
			fb = flatbuffers.Builder(0)
		_langs = build_vct_enum(fb, self.langs, test_fbs.Account.AccountStartLangsVector)
		_extra = self.extra.to_fbs(fb)
		_extras = build_vct_cls(fb, self.extras, test_fbs.Account.AccountStartExtrasVector)
		_test_str = fs(fb, self.test_str)
		_test_vct_str = build_vct_str(fb, self.test_vct_str, test_fbs.Account.AccountStartTestVctStrVector)
		_test_vct_int = build_vct_int(fb, self.test_vct_int, test_fbs.Account.AccountStartTestVctIntVector)
		_test_vct_uint = build_vct_uint(fb, self.test_vct_uint, test_fbs.Account.AccountStartTestVctUintVector)
		_test_vct_short = build_vct_short(fb, self.test_vct_short, test_fbs.Account.AccountStartTestVctShortVector)
		_test_vct_long = build_vct_long(fb, self.test_vct_long, test_fbs.Account.AccountStartTestVctLongVector)
		_test_vct_ulong = build_vct_ulong(fb, self.test_vct_ulong, test_fbs.Account.AccountStartTestVctUlongVector)
		_test_vct_float = build_vct_float(fb, self.test_vct_float, test_fbs.Account.AccountStartTestVctFloatVector)
		_test_vct_bool = build_vct_bool(fb, self.test_vct_bool, test_fbs.Account.AccountStartTestVctBoolVector)

		test_fbs.Account.AccountStart(fb)
		test_fbs.Account.AccountAddLang(fb, self.lang)
		test_fbs.Account.AccountAddLangs(fb, _langs)
		test_fbs.Account.AccountAddExtra(fb, _extra)
		test_fbs.Account.AccountAddExtras(fb, _extras)
		test_fbs.Account.AccountAddTestStr(fb, _test_str)
		test_fbs.Account.AccountAddTestVctStr(fb, _test_vct_str)
		test_fbs.Account.AccountAddTestInt(fb, self.test_int)
		test_fbs.Account.AccountAddTestVctInt(fb, _test_vct_int)
		test_fbs.Account.AccountAddTestUint(fb, self.test_uint)
		test_fbs.Account.AccountAddTestVctUint(fb, _test_vct_uint)
		test_fbs.Account.AccountAddTestShort(fb, self.test_short)
		test_fbs.Account.AccountAddTestVctShort(fb, _test_vct_short)
		test_fbs.Account.AccountAddTestLong(fb, self.test_long)
		test_fbs.Account.AccountAddTestVctLong(fb, _test_vct_long)
		test_fbs.Account.AccountAddTestUlong(fb, self.test_ulong)
		test_fbs.Account.AccountAddTestVctUlong(fb, _test_vct_ulong)
		test_fbs.Account.AccountAddTestFloat(fb, self.test_float)
		test_fbs.Account.AccountAddTestVctFloat(fb, _test_vct_float)
		test_fbs.Account.AccountAddTestBool(fb, self.test_bool)
		test_fbs.Account.AccountAddTestVctBool(fb, _test_vct_bool)
		if is_finish:
			fb.Finish(test_fbs.Account.AccountEnd(fb))
			return fb.Output()
		return test_fbs.Account.AccountEnd(fb)



