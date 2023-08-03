import socket
from threading import Thread
import CmdDistribute as cd
import sys
import json

soc = None
bind = False
ip = '127.0.0.1'
port = 7214
recvbuff = 1024
conn = None
addr = None
data = None
recvthread = None

def readbuff():
    global conn,recvbuff
    endflag = '\x00'
    outdata = ''
    while True:
        indata = str(conn.recv(recvbuff),'utf-8','ignore')
        if endflag in indata:
            outdata += indata.strip()
            break
        else:
            outdata += indata
    return outdata


def socsend(indata):
    global conn
    senddata = indata.encode('utf-8')
    conn.send(senddata)
    print('[send msg to ue | ',sys.getsizeof(indata),']: ',indata)

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
