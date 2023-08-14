# -*- coding: utf-8 -*-
import unreal

def main():
    # 获取当前所用语言
    current_language = unreal.InternationalizationLibrary.get_current_language()
    if(current_language == "zh-Hans"):
        unreal.InternationalizationLibrary.set_current_language("English")
        print("换成英文")
    else:
        unreal.InternationalizationLibrary.set_current_language("zh-Hans")
        print("换成中文")


if __name__ == "__main__":
    main()