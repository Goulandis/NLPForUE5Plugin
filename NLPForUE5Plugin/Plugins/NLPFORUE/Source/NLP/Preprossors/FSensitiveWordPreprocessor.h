// 基于DFA算法实现的铭感词过滤预处理器
#pragma once
#include <unordered_set>
#include <map>
#include "FPreprocessor.h"
#include "CoreMinimal.h"

using namespace std;

// 敏感词DFA节点
struct TreeNode
{
	using TreeMap = map<unsigned char,TreeNode*>;

	TreeNode()
	{
		c_ = '0';
		is_end_ = false;
	}

	TreeNode(unsigned char c,bool is_end):c_(c),is_end_(is_end){}
	
	TreeNode* FindChildNode(const unsigned char next_char) const
	{
		if(subtree_map_.count(next_char))
		{
			return subtree_map_.at(next_char);
		}
		return nullptr;
	}

	TreeNode* InsertChildNode(const unsigned char next_char)
	{
		// 判断要插入的节点是否已存在于树中,如果存在则反复空指针，表示插入失败
		if(FindChildNode(next_char))
		{
			return nullptr;
		}
		TreeNode* child = new TreeNode(next_char,false);
		// 当内存不足时有可能会创建节点失败
		if(child == nullptr)
		{
			return nullptr;
		}
		subtree_map_.insert(make_pair(next_char,child));
		return child;
	}
	
	unsigned char c_;
	bool is_end_;
	TreeMap subtree_map_;
};

// 敏感词DFA树
class DFATree
{
public:
	DFATree(){}
	DFATree(const vector<string>& sensitive_words);
	// 往词典中加入单个敏感词
	bool AddSensitiveWord(const string& sensitive_word);
	// 查找一句话中所有的敏感词
	vector<string> FindAllSensitiveWords(const string& text,const int match_type = 2,const unsigned char replaced_char = '*') const;
	// 将敏感词替换成指定符号
	string ReplaceAllSensitiveWords(const string& text,const bool unix_shell_colored = true,const int match_type = 2,const unsigned char replaced_char = '*') const;
private:
	TreeNode root_;

	// 向树中插入节点
	bool Insert(TreeNode* parent,const string& sensitive_word);
	// 检查一个句话里面是否存在敏感词
	int CheckSensitiveWord(const TreeNode* node,const string& text,int begin_index,const int match_type) const;
};

static const int kMinMatch = 1;
static const int kMaxMatch = 2;
static const int kBoldRedANSIColorCodeLen = 11;
static const string kBoldRedANSIColorCodePrefix = "\033[1;31m";
static const string kBoldRedANSIColorCodeSuffix = "\033[0m";

static int Utf8StringLen(const string& word)
{
	const char* s = word.c_str();
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

	void LoadSensitiveWordDict(const string& Path);
	string SensitiveWordFiltering(const string& Text);
	vector<string> GetSensitiveWords(const string& Text);
private:
	FSensitiveWordPreprocessor();

	DFATree* DfaTree;
};
