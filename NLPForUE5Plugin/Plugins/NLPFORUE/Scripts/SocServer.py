import socket
from threading import Thread
import CmdDistribute as cd
import sys
import json

# 存储Socket
soc = None
bind = False
ip = '127.0.0.1'
port = 7214
recvbuff = 1024
# 存储连接
conn = None
# 存储连接地址信息
addr = None
# 存储接收到的数据包
data = None
# 消息接收线程引用
recvthread = None

# 读取缓冲区的数据并转换成字符串输出
def readbuff():
    global conn,recvbuff
    # endflag = '\x00'
    # outdata = ''
    while True:
        # 使用ignore参数，防止recv函数使用默认的严格编码导致编码错误
        indata = str(conn.recv(recvbuff),'utf-8','ignore')
        # if endflag in indata:
        #     outdata += indata.strip()
        #     break
        # else:
        #     outdata += indata
        return indata

# 发送数据
def socsend(indata):
    global conn
    senddata = indata.encode('utf-8')
    conn.send(senddata)
    print('[send msg to ue | ',sys.getsizeof(indata),']: ',indata)

# 接收数据，并根据指令名调用CmdDistribute模块的对应名字的函数，管理客户端的断连
def socrecv():
    global data,conn,addr
    while True:
        data = readbuff()
        print('[recv msg from ue |',sys.getsizeof(data),']: ',data)
        jsondata = json.loads(data[0:len(data)-1])
        if jsondata['cmd'] == 'soc':
            if jsondata['type'] == 'close':
                print(addr,'已退出')
                break
        else:
            fun = getattr(cd,jsondata['cmd'])
            result = fun(jsondata)
            if result:
                socsend(result)


def soclisten():
    global soc,bind,conn,addr,recvthread
    soc=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    soc.bind((ip,port))
    soc.listen(5)
    print('server listen...')
    bind = True
    while True:
        conn,addr = soc.accept()
        print(addr,'已接入')
        recvthread = Thread(target=socrecv)
        recvthread.setDaemon(True)
        recvthread.start()   

soclisten()
