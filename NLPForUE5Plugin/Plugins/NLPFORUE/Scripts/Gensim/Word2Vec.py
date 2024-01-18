#coding:utf-8

from gensim.models import Word2Vec
from gensim.models import KeyedVectors
import jieba
import numpy as np
import sys
import os
import json

# CorpusDir = sys.argv[1]
# TextSource = sys.argv[0]
# TextTarget = sys.argv[1]
# print(TextSource+"\n"+TextTarget)
CorpusDir = "E:/Goulandis/NLPForUE5Plugin/NLPForUE5Plugin/Plugins/NLPFORUE/Resources/SQLite3/Corpus"
ModelSaveDir = "E:/Goulandis/NLPForUE5Plugin/NLPForUE5Plugin/Plugins/NLPFORUE/Scripts/Gensim"
ModelSaveName = "Word2Vec"
Train = False

# 遍历语料文件夹，读取文件夹及其子文件夹下所有的文件
def GetCorpusFiles(Path,FileList=[]):
    if os.path.isfile(Path):
        FileList.append(Path)
    elif os.path.isdir(Path):
        for SubPath in os.listdir(Path):
            GetCorpusFiles(os.path.join(Path,SubPath),FileList)
    return FileList

def ReadCorpus(Path):
    TextList = []
    with open(Path,'r',encoding='utf8') as File:
        for Line in File.readlines():
            TextList.append(Line)
    return TextList

def TrainWord2VecModel(TextList):
    ModelExists = False
    for SubPath in os.listdir(ModelSaveDir):
        if os.path.isfile(SubPath):
            SubPathName = os.path.basename(SubPath)
            if SubPath == ModelSaveName:
                ModelExists = True
    if (ModelExists and Train) or not ModelExists:
        '''
        sg=1 是 skip-gram 算法，对低频词敏感；默认 sg=0 为 CBOW 算法。
        size 是输出词向量的维数，值太小会导致词映射因为冲突而影响结果，值太大则会耗内存并使算法计算变慢，一般值取为100到200之间。
        window 是句子中当前词与目标词之间的最大距离，3表示在目标词前看3-b 个词，后面看 b 个词（b 在0-3之间随机）。
        min_count 是对词进行过滤，频率小于 min-count 的单词则会被忽视，默认值为5。
        negative 和 sample 可根据训练结果进行微调，sample 表示更高频率的词被随机下采样到所设置的阈值，默认值为 1e-3。
        hs=1 表示层级 softmax 将会被使用，默认 hs=0 且 negative 不为0，则负采样将会被选择使用。
        '''
        Model = Word2Vec(sentences=TextList,sg=1,min_count=2,negative=1,sample=0.001,workers=4)
        Model.save(os.path.join(ModelSaveDir,ModelSaveName))

def Similarity(JsonData):
    FileList = GetCorpusFiles(CorpusDir)
    TextList = []
    for File in FileList:
        TextList = ReadCorpus(File)  
    TrainWord2VecModel(TextList)
    Model = Word2Vec.load(os.path.join(ModelSaveDir,ModelSaveName))
    Data = JsonData['data']
    SourceText = Data['source']
    TargetText = Data['target']
    SourceTextList = jieba.lcut(SourceText,cut_all=True)
    TargetTextList = jieba.lcut(TargetText,cut_all=True)
    RelAvg = Model.wv.n_similarity(SourceTextList,TargetTextList)
    Rel = {"cmd":"soc","type":"response","data":{"defualt":str(RelAvg)}}
    JsonStr = json.dumps(Rel,ensure_ascii=False)
    return JsonStr
