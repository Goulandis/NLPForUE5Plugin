#include "FSensitiveWordPreprocessor.h"
#include <fstream>

DFATree::DFATree(const vector<string>& sensitive_words)
{
	for(const string& sensitive_word : sensitive_words)
	{
		Insert(&root_,sensitive_word);
	}
}

bool DFATree::AddSensitiveWord(const string& sensitive_word)
{
	return Insert(&root_,sensitive_word);
}

vector<string> DFATree::FindAllSensitiveWords(const string& text, const int match_type,
	const unsigned char replaced_char) const
{
	vector<string> matched_words;
	int begin_index = 0;
	const int n = text.size();
	while(begin_index < n)
	{
		int match_len = CheckSensitiveWord(&root_,text,begin_index,match_type);
		if(match_len == 0)
		{
			begin_index++;
		}
		else
		{
			matched_words.push_back(text.substr(begin_index,match_len));
			begin_index += match_len;
		}
	}
	return matched_words;
}

string DFATree::ReplaceAllSensitiveWords(const string& text, const bool unix_shell_colored, const int match_type,
	const unsigned char replaced_char) const
{
	string replaced_text = text;
	int begin_index = 0;
	int shift_len = 0;
	const int n = text.size();
	while(begin_index < n)
	{
		int match_len = CheckSensitiveWord(&root_,text,begin_index,match_type);
		if(match_len == 0)
		{
			begin_index++;
		}
		else
		{
			const string matched_word = text.substr(begin_index,match_len);
			int utf8Len = Utf8StringLen(matched_word);
			string replaced_str = string(utf8Len,replaced_char);
			if(unix_shell_colored)
			{
				replaced_str = kBoldRedANSIColorCodePrefix + replaced_str + kBoldRedANSIColorCodeSuffix;
			}
			replaced_text = replaced_text.substr(0,begin_index - shift_len) + replaced_str + replaced_text.substr(begin_index + match_len - shift_len);
			begin_index += match_len;
			shift_len += match_len - utf8Len;
			shift_len -= unix_shell_colored ? kBoldRedANSIColorCodeLen : 0;
		}
	}
	return replaced_text;
}

bool DFATree::Insert(TreeNode* parent, const string& sensitive_word)
{
	const int n = sensitive_word.size();
	// 遍历敏感词的所有字符
	for(int i=0;i<n;++i)
	{
		if(!parent)
		{
			return false;
		}
		const unsigned char c = sensitive_word[i];
		// 查找字符是否已存在树的节点中
		TreeNode* child = parent->FindChildNode(c);
		// 如果字符不在树中，则往树中加入一个新的子节点存储字符，并将父节点指针移动到当前层
		if(!child)
		{
			parent = parent->InsertChildNode(c);
		}
		// 如果字符存在树中，则将父节点指针向下移动一层
		else
		{
			parent = child;
		}
		// 如果字符为词语的最后一个字符，则设置节点结束标志
		if(i == n -1)
		{
			parent->is_end_ = true;
		}
	}
	return true;
}

int DFATree::CheckSensitiveWord(const TreeNode* node, const string& text, int begin_index,
	const int match_type) const
{
	bool flag = false;
	int match_len = 0;
	const int n = text.size();
	for(int i = begin_index;i < n;++i)
	{
		const unsigned char c = text[i];
		node = node->FindChildNode(c);
		if(!node)
		{
			break;
		}
		else
		{
			match_len++;
			if(node->is_end_)
			{
				flag = true;
				if(match_type == kMaxMatch)
				{
					break;
				}
			}
		}
	}
	if(match_type < 2 || !flag)
	{
		match_len = 0;
	}
	return match_len;
}

FSensitiveWordPreprocessor::FSensitiveWordPreprocessor()
{
	NLOG(LOGNLP,Log,TEXT("FSensitiveWordPreprocessor constructed"));
}

FSensitiveWordPreprocessor& FSensitiveWordPreprocessor::Get()
{
	static FSensitiveWordPreprocessor Instance;
	return Instance;
}

FSensitiveWordPreprocessor::~FSensitiveWordPreprocessor()
{
	NLOG(LOGNLP,Log,TEXT("FSensitiveWordPreprocessor destructed"));
}

void FSensitiveWordPreprocessor::LoadSensitiveWordDict(const string& Path)
{
	vector<string> SensitiveWords;
	ifstream Ifs(Path.c_str());
	check(Ifs.is_open())
	string Line;
	while(getline(Ifs,Line))
	{
		SensitiveWords.push_back(Line);
	}
	if(SensitiveWords.size() == 0)
	{
		NLOG(LOGNLP,Error,TEXT("There is no set dictionary of sensitive words"));
	}
	size_t Len = SensitiveWords.size();
	DfaTree = new DFATree(SensitiveWords);
	Ifs.close();
}

string FSensitiveWordPreprocessor::SensitiveWordFiltering(const string& Text)
{
	return DfaTree->ReplaceAllSensitiveWords(Text,false);
}

vector<string> FSensitiveWordPreprocessor::GetSensitiveWords(const string& Text)
{
	return DfaTree->FindAllSensitiveWords(Text);
}
