import TypeDistribute as td

def soc(jsondata):
    fun = getattr(td,jsondata['type'])
    fun(jsondata)

def ltp(jsondata):
    print('Call ltp')
    fun = getattr(td,jsondata['type'])
    return fun(jsondata)