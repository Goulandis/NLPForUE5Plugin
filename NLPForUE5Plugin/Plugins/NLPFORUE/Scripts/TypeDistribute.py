import LTP
import json

def cws(jsondata):
    data = jsondata['data']
    text = data['defualt']
    result = LTP.cws(text)
    cwslist = result[0]
    cwsobj = {"defualt":cwslist}
    jsonstr = json.dumps(cwsobj,ensure_ascii=False)
    return jsonstr


    
        