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



结巴分词词性对照表

| 标注 | 解释     | 标注 | 解释           | 标注 | 解释                 |
| ---- | -------- | ---- | -------------- | ---- | -------------------- |
| a    | 形容词   | mq   | 数量词         | tg   | 时语素               |
| ad   | 副形词   | n    | 名词           | u    | 助词                 |
| ag   | 形语素   | ng   | 例：义，乳，亭 | ud   | 例：得               |
| an   | 名形词   | nr   | 人名           | ug   | 例：过               |
| b    | 区别词   | nrfg | 也是人名       | uj   | 例：的               |
| c    | 连词     | nrt  | 也是人名       | ul   | 例：了               |
| d    | 副词     | ns   | 地名           | uv   | 例：地               |
| df   | 例：不要 | nt   | 机构团体       | uz   | 例：着               |
| dg   | 副语素   | nz   | 其他专名       | v    | 动词                 |
| e    | 叹词     | o    | 拟声词         | vd   | 副动词               |
| f    | 方位词   | p    | 介词           | vg   | 动语素               |
| g    | 语素     | q    | 量词           | vi   | 例：沉溺于，等同于   |
| h    | 前接成分 | r    | 代词           | vn   | 动名词               |
| i    | 成语     | rg   | 例：兹         | vq   | 例：去浄，去过，唸过 |
| j    | 简称略语 | rr   | 人称代词       | x    | 非语素词             |
| k    | 分接成分 | rz   | 例：这位       | y    | 语气词               |
| l    | 习用语   | s    | 处所词         | z    | 状态词               |
| m    | 数词     | t    | 时间词         | zg   | 例：且，丗，丢       |

## UE与Python的通信指令

### UE退出连接

```json
{
    "cmd":"soc",
    "type":"close",
    "data":{}
}
```

### 文本相似度比较

```json
{
    "cmd":"Word2Vec",
    "type":"Similarity",
    "data":
    {
        "source":"源文本",
        "target":"比较文本"
    }
}
```



## 使用Windows.h的API FindWindow( LPCTSTR lpClassName,

         LPCTSTR lpWindowName )是传递窗口名字符串报错

这是因为使用不同的编译器，编译器会根据编码方式来选定使用LPCWSTR还是LPCTSTR，LPCWSTR对应32-bit指针, 指向一个unicode字符串常量的指针,每个字符占2字节；LPCTSTR对应32-bit指针 指向一个常字符串,每字符可能占1字节或2字节，取决于Unicode是否定义。于是就会出现参数类型不匹配的问题

由于他们的底层都是char指针，所以可以进行强制转换，使用TEXT()宏或者_T()宏进行强制转换：

```C++
FindWindow(NULL,_T("xxx"));
FindWindow(NULL,TEXT("xxx"));
```

由于TEXT宏和UE的TEXT宏冲突，所以可以选择使用_T宏。