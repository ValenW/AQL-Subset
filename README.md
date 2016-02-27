# AQL Subset
中山大学软件学院2013级编译原理Project

[AQL][1]，全称Annotation Query Language，用于文本分析，可从非结构化或半结构化的文本中提取结构化信息的语言，语法和SQL类似。
该程序实现了AQL语法的子集，包含其主要特点。

### 编译、运行环境：
由于需要用到dirent.h头文件，故只能在Unix环境下编译和使用。

### 使用方法：
进入src文件夹，执行命令：
```
$ make
$ ./AQL <path to .aql> <path to file or the dir include the files>
$ ...
$ make clean
```
`dataset`文件夹中已经有若干可供使用的样例，每个AQL文件对应一个同名的文件夹，内为适用于该AQL文件的若干文章，以`.input`或着`.txt`为后缀名。
另外该程序只能用于分析英文文章，{NUM1, NUM2}子句中要求NUM1 >= NUM2 > 0

以下以使用Perloc.aql为例:
```
$ cd src
$ make
$ ./AQL ../dataset/Perloc.aql ../dataset/perloc
$ make clean
```
需要输出到文件可自行使用重定向。


### TODO:
- [x] 基本功能
    - [x] lexer 词法分析
    - [x] tokenizer 文章分析与提取
    - [x] parser 语法分析
    - [x] operator 根据语法分析产生的结果生成view
    - [x] output 格式化输出
- [x] 支持不完整Token
    - [x] 更改tokenizer数据结构
    - [x] 更新对文章分析部分的实现
- [x] `Token`匹配算法优化
    - [x] 更改遍历为前后token的ID对比
    - [x] 支持出现在首部的`Token`
    - [x] 支持出现在尾部的`Token`
    - [ ] ~~支持连续的`Token`~~
    - [ ] ~~支持单独一个`Token`~~
- [x] AQL编写及数据集收集
- [ ] 支持中文
    - [ ] 更新AQL文件写法
    - [ ] 改造正则表达式引擎
    - [ ] 更新词法、语法分析和匹配规则

**v1.1**

- 重新实现了`Token`规则的匹配，提升了匹配的速度。
- 支持不完整Token的匹配。
- 能够提示一些基本的语义错误，比如from子句中出现了未曾创建的viewName。
- 我们认为连续的`Token`或者单独的`Token`规则是不可理喻的，故不予实现。

**v1.0**

- 实现了基本的AQL功能
- 能够提示词法，语法错误，及其出现的行列号。
- 对于`regex`匹配，只能匹配完整的Token，对于匹配到不完整Token的正则表达式，比如[0-9]匹配到数的一部分，则会报错。
- 对于`Token`规则的匹配使用遍历实现，有极大的改进空间 ;)。


  [1]: https://www-01.ibm.com/support/knowledgecenter/SSPT3X_3.0.0/com.ibm.swg.im.infosphere.biginsights.aqlref.doc/doc/aql-overview.html
