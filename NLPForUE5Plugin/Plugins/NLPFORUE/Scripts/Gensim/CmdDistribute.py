import Word2Vec as w2v


def soc(jsondata):
    fun = getattr(w2v,jsondata['type'])
    fun(jsondata)

def Word2Vec(jsondata):
    fun = getattr(w2v,jsondata['type'])
    return fun(jsondata)