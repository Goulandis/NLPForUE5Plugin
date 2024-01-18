import socket
from threading import Thread
import CmdDistribute as cd
import sys
import json

# 存储Socket
Soc = None
Bind = False
IP = '127.0.0.1'
Port = 7214
RecvBuff = 1024
# 存储连接
Conn = None
# 存储连接地址信息
Addr = None
# 存储接收到的数据包
Data = None
# 消息接收线程引用
RecvThread = None
# 客户端异常状态
ClientException = False

def IsJson(JsonData):
    try:
        json.loads(JsonData)
    except ValueError:
        print("不是json格式")
        return False
    return True

# 读取缓冲区的数据并转换成字符串输出
def ReadBuff():
    global Conn,RecvBuff,ClientException
    RecvData = None
    while True:
        # 使用ignore参数，防止recv函数使用默认的严格编码导致编码错误
        try:
            InData = str(Conn.recv(RecvBuff),'utf-8','ignore')
            RecvDataList = InData.split("\\n")
            if len(RecvDataList) < 2:
                print("Packets are not splitted using \\n")
                return RecvData
            RecvData = RecvDataList[0]
        except:
            ClientException = True
            # print(InData)
        return RecvData

# 发送数据
def SocSend(InData):
    global Conn
    SendData = InData + "\\n"
    SendDataByte = SendData.encode('utf-8')
    Conn.send(SendDataByte)
    print('[send msg to ue | ',sys.getsizeof(InData),']: ',InData)
    # print('[send msg to ue | ',sys.getsizeof(SendData),']: ',SendData)

# 接收数据，并根据指令名调用CmdDistribute模块的对应名字的函数，管理客户端的断连
def SocRecv():
    global Data,Conn,Addr,ClientException
    while True:
        Data = ReadBuff()
        # print('[recv msg from ue |',sys.getsizeof(Data),']: ',Data)
        if ClientException==True:
            print(Addr,"exit unexpectedly")
            break
        print('[recv msg from ue |',sys.getsizeof(Data),']: ',Data)
        if IsJson(Data):
            JsonData = json.loads(Data)
            if JsonData['cmd'] == 'soc':
                if JsonData['type'] == 'close':        
                    SocSend(json.dumps({"cmd":"soc","type":"close","data":{}} ,ensure_ascii=False))
                    print(Addr,'exited')
                    break
            else:
                Fun = getattr(cd,JsonData['cmd'])
                Result = Fun(JsonData)
                if Result:
                    SocSend(Result)

def SocListen():
    global Soc,Bind,Conn,Addr,RecvThread,ClientException
    Soc=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    Soc.bind((IP,Port))
    Soc.listen(5)
    print('server listen...')
    Bind = True
    while True:
        Conn,Addr = Soc.accept()
        print(Addr,'connected')
        ClientException = False
        RecvThread = Thread(target=SocRecv)
        RecvThread.setDaemon(True)
        RecvThread.start()   

SocListen()
