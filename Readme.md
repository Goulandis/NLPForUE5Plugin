python环境：

conda虚拟环境，python=3.9.7

依赖第三方python库：

ltp、chaterbot、chatterbot_corpus



![](https://github.com/Goulandis/NLPForUE5Plugin/blob/main/NLPForUE5Plugin/Plugins/NLPFORUE/Resources/ChatBotForUE.png?raw=true)

第三方分词库：

cppjieba ：[yanyiwu/cppjieba: "结巴"中文分词的C++版本 (github.com)](https://github.com/yanyiwu/cppjieba/tree/master)

cppjieba 依赖子库：[yanyiwu/limonp: C++ headers(hpp) library with Python style. --- yanyiwu/limonp：具有 Python 风格的C++ headers（hpp） 库。 (github.com)](https://github.com/yanyiwu/limonp)

依赖子库可以直接使用git命令安装：

```
git submodule init
git submodule update
```

敏感词库：[observerss/textfilter: 敏感词过滤的几种实现+某1w词敏感词库 (github.com)](https://github.com/observerss/textfilter)

敏感词库属于敏感资源，作者在上传分享后删除了最新版本的资源，可以通过回退操作恢复删除的资源。