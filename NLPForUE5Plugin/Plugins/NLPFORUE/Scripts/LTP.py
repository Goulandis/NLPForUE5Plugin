from ltp import LTP

ltp = LTP()

def cws(text):
    world = ltp.pipeline(text, tasks = ["cws"], return_dict = False)
    print('[ltp-cws]: ',world)
    return world