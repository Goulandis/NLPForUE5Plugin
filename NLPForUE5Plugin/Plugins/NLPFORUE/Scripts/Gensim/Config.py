import json
import os

ConfigFileName = "Config.json"

def LoadJsonFromFile(Path):
    with open(Path,'r',encoding='utf8') as File:
        Json = json.load(File)
        File.close()
        return Json

Config = LoadJsonFromFile(os.path.dirname(os.path.abspath(__file__)) + "\\" + ConfigFileName)
