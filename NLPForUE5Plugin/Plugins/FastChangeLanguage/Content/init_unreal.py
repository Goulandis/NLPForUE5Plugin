# -*- coding: utf-8 -*-
import os
import sys
import platform
from subprocess import PIPE,Popen
from queue import Queue, Empty
import threading
from functools import partial
import unreal

def main():
    dirPath = os.path.dirname(__file__)
    print(dirPath)
    # 添加sys.path
    #sys.path.append(os.path.dirname(__file__)+"/Other")
    print("插件初始化")

    # 根据系统获取Python exe路径
    os_config = {
        "Windows": "Win64",
        "Linux": "Linux",
        "Darwin": "Mac",
    }
    # 获取当前平台
    os_platform = os_config.get(platform.system())
    # 查看平台是否支持
    if not os_platform:
        raise OSError("不支持当前平台 '{}'".format(platform.system()))

    # 默认Python3
    PYTHON = "Python3"
    # 在UE4中运行会获取UE4Editor.exe
    ThirdParty = os.path.join(sys.executable, "..", "..", "ThirdParty")
    interpreter = os.path.join(ThirdParty,PYTHON,os_platform,"python.exe")
    # 转换为绝对路径
    interpreter = os.path.abspath(interpreter)
    # print("sys.executable {}".format(sys.executable))
    print(interpreter)

    # 键盘监听Python文件
    exec_file = dirPath + "/key_listener.py"
    # 开启Python子进程进行键盘监听
    # 查看posix是否在内部模块中
    ON_POSIX = "posix" in sys.builtin_module_names
    p = Popen(
        [interpreter,exec_file],
        shell=True,
        stdout=PIPE,
        bufsize=1,
        #close_fds=ON_POSIX,
    )
    print("子线程pid： " + str(p.pid))



    def enqueue_output(out, queue):
        for line in iter(out.readline, b""):
            queue.put(line)
        unreal.log_warning("队列结束")
        out.close()

    q = Queue()
    t = threading.Thread(target=enqueue_output, args=(p.stdout, q))
    t.daemon = True  # thread dies with the program
    t.start()

    def get_foreground_window_pid():
        from ctypes import wintypes, windll, byref

        # NOTE https://stackoverflow.com/a/56572696
        h_wnd = windll.user32.GetForegroundWindow()
        pid = wintypes.DWORD()
        windll.user32.GetWindowThreadProcessId(h_wnd, byref(pid))
        return pid.value

    def __hotkey_tick__(delta_seconds):
        try:
            line = q.get_nowait()
        except Empty:
            return
        unreal.log_warning(line)
        if not line or delta_seconds > 0.1:
            unreal.log_warning("not line or delta_seconds > 0.1")
            return

        if platform.system() == "Windows":
            # 获取Windows当前关注窗口的pid
            pid = get_foreground_window_pid()
            print("当前窗口pid: {}".format(pid))
            # 若当前窗口pid和unreal pid不同则放弃操作
            if pid != os.getpid():
                unreal.log_warning("not ue4 pid!!!")
                return
        # unreal控制台运行Py文件
        unreal.SystemLibrary.execute_console_command(None,"py "+ dirPath +"/ChangeLanguage.py")

    # 完全Kill进程树
    def kill():
        py_kill = Popen("TASKKILL /PID " + str(p.pid) + " /f /t")
        unreal.log_warning("删除进程树")
    # 注册到ue4 slate Tick
    unreal.register_slate_post_tick_callback(__hotkey_tick__)
    # 关闭Unreal的同时关闭子进程
    unreal.register_python_shutdown_callback(kill)

    print("插件初始化成功")

        
if __name__ == '__main__':
    main()
