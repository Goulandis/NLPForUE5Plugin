import codecs
import os
import re
import jieba

class ChineseTagger(object):
    
    def __init__(self,language=None):
        self.stopword = []
        self.language = language
        cfp = codecs.open(os.path.dirname(__file__) + '/data/cn_stopwords.txt','r+','utf-8')
        for line in cfp:
            for word in line.split():
                self.stopword.append(word)
        cfp.close()
        jieba.load_userdict(os.path.dirname(__file__)+'/data/user_dict.txt')
    
    def get_bigram_pair_string(self,text):
        bigram_pairs = []
        text = re.sub(r'\s+', ' ', str(text))  # trans 多空格 to空格
        text = re.sub(r'\n+', ' ', str(text))  # trans 换行 to空格
        text = re.sub(r'\t+', ' ', str(text))  # trans Tab to空格
        text = re.sub("[\s+\.\!\/_,$%^*(+\"\']+|[+——；！，”。《》，。：“？、~@#￥%……&*（）1234567①②③④)]+".\
                          encode().decode("utf8"), "".encode().decode("utf8"), text)
        
        wordlist = list(jieba.cut(str(text)))
        for word in wordlist:
            if word not in self.stopword:
                if word != '\r\n' and word != ' ' and word != '\u3000'.encode().decode('unicode_escape') \
                        and word != '\xa0'.encode().decode('unicode_escape'):  # 词语的清洗：去全角空格
                    bigram_pairs.append(word)
        print(bigram_pairs)
        # 返回包含词性引理对的文本字符串  
        return ' '.join(bigram_pairs)
    
    def get_text_index_string(self, text):
        bigram_pairs = []
        text = re.sub(r'\s+', ' ', str(text))  # trans 多空格 to空格
        text = re.sub(r'\n+', ' ', str(text))  # trans 换行 to空格
        text = re.sub(r'\t+', ' ', str(text))  # trans Tab to空格
        text = re.sub("[\s+\.\!\/_,$%^*(+\"\']+|[+——；！，”。《》，。：“？、~@#￥%……&*（）1234567①②③④)]+".\
                          encode().decode("utf8"), "".encode().decode("utf8"), text)
        
        wordlist = list(jieba.cut(str(text)))
        for word in wordlist:
            if word not in self.stopword:
                if word != '\r\n' and word != ' ' and word != '\u3000'.encode().decode('unicode_escape') \
                        and word != '\xa0'.encode().decode('unicode_escape'):  # 词语的清洗：去全角空格
                    bigram_pairs.append(word)
        # 返回包含词性引理对的文本字符串  
        return ' '.join(bigram_pairs)
        #return text.lower()
