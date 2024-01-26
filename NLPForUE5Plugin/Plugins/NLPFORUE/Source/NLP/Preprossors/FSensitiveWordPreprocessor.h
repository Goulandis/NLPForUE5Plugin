// 基于DFA算法实现的铭感词过滤预处理器
#pragma once
#include <unordered_set>
#include <map>
#include "FPreprocessor.h"
#include "CoreMinimal.h"

// 敏感词DFA节点
struct TreeNode
{
	using TreeMap = map<unsigned char,TreeNode*>;

	TreeNode()
	{
		c_ = '0';
		IsEnd = false;
	}

	TreeNode(unsigned char c,bool InIsEnd):c_(c),IsEnd(InIsEnd){}
	
	TreeNode* FindChildNode(const unsigned char NextChar) const
	{
		if(SubTreeMap.count(NextChar))
		{
			return SubTreeMap.at(NextChar);
		}
		return nullptr;
	}

	TreeNode* InsertChildNode(const unsigned char NextChar)
	{
		// 判断要插入的节点是否已存在于树中,如果存在则反复空指针，表示插入失败
		if(FindChildNode(NextChar))
		{
			return nullptr;
		}
		TreeNode* Child = new TreeNode(NextChar,false);
		// 当内存不足时有可能会创建节点失败
		if(Child == nullptr)
		{
			return nullptr;
		}
		SubTreeMap.insert(make_pair(NextChar,Child));
		return Child;
	}
	
	unsigned char c_;
	bool IsEnd;
	TreeMap SubTreeMap;
};

// 敏感词DFA树
class DFATree
{
public:
	DFATree(){}
	DFATree(const vector<string>& SensitiveWords);
	// 往词典中加入单个敏感词
	bool AddSensitiveWord(const string& SensitiveWord);
	// 查找一句话中所有的敏感词
	vector<string> FindAllSensitiveWords(const std::string& Text,const int MatchType = 2) const;
	bool HasSensitiveWord(const std::string Text,const int MatchType = 2) const;
	// 将敏感词替换成指定符号
	string ReplaceAllSensitiveWords(const string& Text,const bool UnixShellColored = true,const int MatchType = 2,const unsigned char ReplacedChar = '*') const;
private:
	// 向树中插入节点
	bool Insert(TreeNode* parent,const string& SensitiveWord);
	// 检查一个句话里面是否存在敏感词
	int CheckSensitiveWord(const TreeNode* Node,const string& Text,int BeginIndex,const int MatchType) const;

	TreeNode Root;
};

static const int kMinMatch = 1;
static const int kMaxMatch = 2;
static const int kBoldRedANSIColorCodeLen = 11;
static const string kBoldRedANSIColorCodePrefix = "\033[1;31m";
static const string kBoldRedANSIColorCodeSuffix = "\033[0m";

static int Utf8StringLen(const string& Word)
{
	const char* s = Word.c_str();
	int Len = 0;
	while(*s)
	{
		Len += (*s++ & 0xc0) != 0x80;
	}
	return Len;
}

class NLP_API FSensitiveWordPreprocessor : public FPreprocessor
{
public:
	static FSensitiveWordPreprocessor& Get();
	~FSensitiveWordPreprocessor();

	void LoadSensitiveWordDict(const std::string& Path);
	string SensitiveWordFiltering(const std::string& Text);
	vector<string> GetSensitiveWords(const std::string& Text);
	bool HasSensitiveWord(const std::string& Text);
private:
	FSensitiveWordPreprocessor();

	DFATree* DfaTree;
};
