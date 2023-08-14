from chatterbot import languages
from chatterbot.storage import SQLStorageAdapter
from chiadapter.tagging import ChineseTagger

class ChiStorageAdapter(SQLStorageAdapter):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.tagger = ChineseTagger(language=kwargs.get('tagger_language',languages.CHI))


#test()