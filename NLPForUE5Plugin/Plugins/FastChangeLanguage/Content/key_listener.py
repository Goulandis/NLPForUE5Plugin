# -*- coding: utf-8 -*-
import os
import sys
import time
# 添加sys.path
sys.path.append(os.path.dirname(__file__)+"/Python")
try:
    from pynput.keyboard import Key, KeyCode, Listener
except Exception as err:
    print(err)



def main():
    def execute():
        print("Detected hotkey")

    combination_to_function = {
        frozenset([KeyCode(vk=0x75)]): execute,  # F6
    }

    pressed_vks = set()


    def get_vk(key):
        """
        Get the virtual key code from a key.
        These are used so case/shift modifications are ignored.
        """
        return key.vk if hasattr(key, 'vk') else key.value.vk

    def on_press(key):
        
        """ When a key is pressed """
        vk = get_vk(key)  # Get the key's vk
        if vk == 0x75:
            execute()

        # 保持stdout刷新确保主进程可以持续获取数据
        sys.stdout.flush()


    def on_release(key):
        """ When a key is released """
        vk = get_vk(key)
        # pressed_vks.remove(vk)  # Remove it from the set of currently pressed keys
        pressed_vks.clear()

    with Listener(on_press=on_press, on_release=on_release) as listener:
        listener.join()
        

if __name__ == "__main__":
    main()
