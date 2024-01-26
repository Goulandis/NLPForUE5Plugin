#coding:utf-8

from gensim.models import Word2Vec
import jieba
import Config
import os
import json

CorpusDir = Config.Config['Word2Vec']['CorpusDir']
ModelSaveDir = Config.Config['Word2Vec']['ModelSaveDir']
ModelSaveName = Config.Config['Word2Vec']['ModelSaveName']
Train = Config.Config['Word2Vec']['Train']

# 遍历语料文件夹，读取文件夹及其子文件夹下所有的文件
def GetCorpusFiles(Path,FileList=[]):
    if os.path.isfile(Path):
        FileList.append(Path)
    elif os.path.isdir(Path):
        for SubPath in os.listdir(Path):
            GetCorpusFiles(os.path.join(Path,SubPath),FileList)
    return FileList

def ReadCorpus(FileList):
    TextList = []
    for Path in FileList:
        with open(Path,'r',encoding='utf8') as File:
            print("Reading file : " + Path)
            for Line in File.readlines():
                TextList.append(Line)
            File.close()
            print("Readed file : " + Path)
    return TextList

def TrainWord2VecModel(TextList):
    ModelFile = ModelSaveDir+ModelSaveName
    if os.path.exists(ModelFile):
        os.remove(ModelFile)
    if Train:
        '''
        sg=1 是 skip-gram 算法，对低频词敏感；默认 sg=0 为 CBOW 算法。
        size 是输出词向量的维数，值太小会导致词映射因为冲突而影响结果，值太大则会耗内存并使算法计算变慢，一般值取为100到200之间。
        window 是句子中当前词与目标词之间的最大距离，3表示在目标词前看3-b 个词，后面看 b 个词（b 在0-3之间随机）。
        min_count 是对词进行过滤，频率小于 min-count 的单词则会被忽视，默认值为5。
        negative 和 sample 可根据训练结果进行微调，sample 表示更高频率的词被随机下采样到所设置的阈值，默认值为 1e-3。
        hs=1 表示层级 softmax 将会被使用，默认 hs=0 且 negative 不为0，则负采样将会被选择使用。
        '''
        print("Start training Word2Vec model")
        Model = Word2Vec(sentences=TextList,sg=1,min_count=2,negative=1,sample=0.001,workers=4)
        print("Completed Word2Vec model")
        Model.save(os.path.join(ModelSaveDir,ModelSaveName))
        print("Saved Word2Vec model to " + os.path.join(ModelSaveDir,ModelSaveName))

def Similarity(JsonData):
    Model = Word2Vec.load(os.path.join(ModelSaveDir,ModelSaveName))
    Data = JsonData['Data']
    SourceText = Data['Source']
    TargetTextList = Data['Target']
    Start = Data["Start"]
    JsonResponse = json.loads('[]')
    SourceTextWordList = jieba.lcut(SourceText,cut_all=True)
    if Start == -1 or SourceText == "" or TargetTextList == "" or len(SourceTextWordList) == 0:
        return json.dumps({"Cmd":"Word2Vec","Type":"Similarity","Data":{"Tag":10,"Similarity":0}},ensure_ascii=False)
    # if SourceText == "" or TargetTextList == "":
    #     return json.dumps({"Cmd":"Word2Vec","Type":"Similarity","Data":{"Tag":1,"Similarity":0}},ensure_ascii=False) 
    # if len(SourceTextWordList) == 0:
    #     return json.dumps({"Cmd":"Word2Vec","Type":"Similarity","Data":{"Tag":1,"Similarity":0}},ensure_ascii=False)
    for TargetText in TargetTextList:
        TargetTextWordList = jieba.lcut(TargetText,cut_all=True)
        if len(TargetTextWordList) == 0:
            continue
        Avg = Model.wv.n_similarity(SourceTextWordList,TargetTextWordList)
        if Avg >= 0.75:
            JsonResponse.append({"TargetText":TargetText,"Similarity":str(Avg)})
    return json.dumps({"Cmd":"Word2Vec","Type":"Similarity","Data":{"Tag":0,"Response":JsonResponse}},ensure_ascii=False)

def Train(JsonData):

    FileList = GetCorpusFiles(CorpusDir)
    TextList = ReadCorpus(FileList)  
    TrainWord2VecModel(TextList)
    Rel = {"Cmd":"Word2Vec","Type":"Train","Data":{"Response":True}}
    JsonRel = json.dumps(Rel,ensure_ascii=False)
    return JsonRel
