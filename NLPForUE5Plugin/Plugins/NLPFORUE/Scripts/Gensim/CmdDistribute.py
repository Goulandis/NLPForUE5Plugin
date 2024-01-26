import Word2Vec as w2v


def Soc(jsondata):
    fun = getattr(w2v,jsondata['Type'])
    fun(jsondata)

def Word2Vec(jsondata):
    fun = getattr(w2v,jsondata['Type'])
    return fun(jsondata)