#ifndef PRRPDEFINE
#define PRRPDEFINE

//#define TOFSTR(Str) FString(UTF8_TO_TCHAR(Str.c_str())) 

// 词性宏
#define CX_M "m" // 数词
#define CX_T "t" // 时间词
#define CX_I "i" // 成语
#define CX_N "n" // 名词
#define CX_V "v" // 动词

enum class ELanguageType
{
	None,
	zh_CN,//简体中文
	en_US,//英文(美国)
	zh_HK//繁体中文(香港)
};

const char* const SOURCE_PATH = "NLPFORUE/Source/";
const char* const DICT_PATH = "cppjieba/dict/jieba.dict.utf8";
const char* const HMM_PATH = "cppjieba/dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "cppjieba/dict/user.dict.utf8";
const char* const IDF_PATH = "cppjieba/dict/idf.utf8";
const char* const STOP_WORD_PATH = "cppjieba/dict/stop_words.utf8";
const char* const SENSITIVE_WORD_PATH = "NLPFORUE/Resources/textfilter/keywords";

#endif