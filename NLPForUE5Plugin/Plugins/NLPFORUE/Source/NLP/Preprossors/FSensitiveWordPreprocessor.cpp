#include "FSensitiveWordPreprocessor.h"
#include <fstream>

DFATree::DFATree(const vector<string>& SensitiveWords)
{
	for(const string& SensitiveWord : SensitiveWords)
	{
		Insert(&Root,SensitiveWord);
	}
}

bool DFATree::AddSensitiveWord(const string& SensitiveWord)
{
	return Insert(&Root,SensitiveWord);
}

vector<string> DFATree::FindAllSensitiveWords(const string& Text, const int MatchType) const
{
	vector<string> MatchedWords;
	int BeginIndex = 0;
	const int N = Text.size();
	while(BeginIndex < N)
	{
		int MatchLen = CheckSensitiveWord(&Root,Text,BeginIndex,MatchType);
		if(MatchLen == 0)
		{
			BeginIndex++;
		}
		else
		{
			MatchedWords.push_back(Text.substr(BeginIndex,MatchLen));
			BeginIndex += MatchLen;
		}
	}
	return MatchedWords;
}

bool DFATree::HasSensitiveWord(const std::string Text, const int MatchType) const
{
	int BeginIndex = 0;
	if(CheckSensitiveWord(&Root,Text,BeginIndex,MatchType) == 0)
	{
		return true;
	}
	return false;
}


string DFATree::ReplaceAllSensitiveWords(const string& Text, const bool UnixShellColored, const int MatchType,const unsigned char ReplacedChar) const
{
	string replaced_text = Text;
	int begin_index = 0;
	int shift_len = 0;
	const int n = Text.size();
	while(begin_index < n)
	{
		int match_len = CheckSensitiveWord(&Root,Text,begin_index,MatchType);
		if(match_len == 0)
		{
			begin_index++;
		}
		else
		{
			const string matched_word = Text.substr(begin_index,match_len);
			int utf8Len = Utf8StringLen(matched_word);
			string replaced_str = string(utf8Len,ReplacedChar);
			if(UnixShellColored)
			{
				replaced_str = kBoldRedANSIColorCodePrefix + replaced_str + kBoldRedANSIColorCodeSuffix;
			}
			replaced_text = replaced_text.substr(0,begin_index - shift_len) + replaced_str + replaced_text.substr(begin_index + match_len - shift_len);
			begin_index += match_len;
			shift_len += match_len - utf8Len;
			shift_len -= UnixShellColored ? kBoldRedANSIColorCodeLen : 0;
		}
	}
	return replaced_text;
}

bool DFATree::Insert(TreeNode* parent, const string& SensitiveWord)
{
	const int n = SensitiveWord.size();
	// 遍历敏感词的所有字符
	for(int i=0;i<n;++i)
	{
		if(!parent)
		{
			return false;
		}
		const unsigned char c = SensitiveWord[i];
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
			parent->IsEnd = true;
		}
	}
	return true;
}

int DFATree::CheckSensitiveWord(const TreeNode* Node, const string& Text, int BeginIndex,
	const int MatchType) const
{
	bool Flag = false;
	int MatchLen = 0;
	const int N = Text.size();
	for(int i = BeginIndex;i < N;++i)
	{
		const unsigned char c = Text[i];
		Node = Node->FindChildNode(c);
		if(!Node)
		{
			break;
		}
		else
		{
			MatchLen++;
			if(Node->IsEnd)
			{
				Flag = true;
				if(MatchType == kMaxMatch)
				{
					break;
				}
			}
		}
	}
	if(MatchType < 2 || !Flag)
	{
		MatchLen = 0;
	}
	return MatchLen;
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

bool FSensitiveWordPreprocessor::HasSensitiveWord(const std::string& Text)
{
	return DfaTree->HasSensitiveWord(Text);
}
