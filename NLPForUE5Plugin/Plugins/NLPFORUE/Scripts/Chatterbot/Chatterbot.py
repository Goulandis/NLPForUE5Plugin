# -*- coding: utf-8 -*-
from chatterbot import ChatBot
from chatterbot.trainers import ListTrainer
from chatterbot.trainers import ChatterBotCorpusTrainer
from chatterbot.comparisons import JaccardSimilarity
import os
from chiadapter.storage_adapter import ChiStorageAdapter


# 是否需要训练
btrian = True
# 训练语料数据目录
directory_to_search = '../../../../DataSets/chinese_chatbot_corpus/clean_chat_corpus/customresponses'
# 支持的训练语料文件类型
file_extensions = ['.txt', '.yml', '.tsv']
chatbot = None
# 名字
chatbotname = '菠菜'
# 加载的适配器
# logic_adapters = [
#     'chatterbot.logic.MathematicalEvaluation',
#     'chatterbot.logic.TimeLogicAdapter'
# ]

# 搜寻目录及其子目录下所有的语料数据
def FindCorpusFiles(directory, extensions):
    file_list = []

    for root, _, files in os.walk(directory):
        for file in files:
            if any(file.endswith(ext) for ext in extensions):
                file_list.append(os.path.join(root, file))

    return file_list

# 使用语料库训练
def InitBot():
    print(chatbotname,'开始初始化...')
    global chatbot,directory_to_search,file_extensions
    conversation = []
    # 创建实体
    chatbot = ChatBot(
        chatbotname,
        # storage_adapter = 'chiadapter.storage_adapter.ChiStorageAdapter',
        storage_adapter='chatterbot.storage.SQLStorageAdapter',
        logic_adapters=[
        'chatterbot.logic.BestMatch',
         'chatterbot.logic.MathematicalEvaluation',
         'chatterbot.logic.TimeLogicAdapter'  
        ],
        preprocessors=[
            'chatterbot.preprocessors.clean_whitespace'
        ],
        trainer='chatterbot.trainers.ListTrainer'
    )
    print('适配器配置完成')
    if btrian:
        pcount = 0
        filepathlist = FindCorpusFiles(directory_to_search, file_extensions)
        print('已搜索到的语料库语料：')
        for tmp in filepathlist:
            print(tmp)
        print('开始读取语料库')
        for filepath in filepathlist:
            with open(filepath,encoding='utf-8') as file:
                print('读取语料库：',filepath)
                content = file.readlines()
                for singletalk in content:
                    dialogue = singletalk.split('\t')
                    question = dialogue[0]
                    answer = dialogue[1]
                    conversation.append(question)
                    conversation.append(answer)
                    pcount += 1
            print(filepath,'语料库读取完毕,总共',pcount,'条对话')
            pcount = 0
        print('所有语料库读取完毕，开始训练',chatbotname)
        trainer = ListTrainer(chatbot)
        trainer.train(conversation)
    print(chatbotname,'完成初始化，可以开始聊天了')

# 启动
def ChatbotSetup():
    global chatbot
    InitBot()
    while True:
        print('我:')
        inp = input()
        response = chatbot.get_response(inp)
        print('菠菜:')
        print(response)

ChatbotSetup()







