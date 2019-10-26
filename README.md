# Compiler-Principle-Experiments
编译原理课程实验

## 实验一：词法分析
### 1.1 单词记数示例
1.1.1 编写wc.l，运行flex wc.l，生成lex.yy.c<br>
1.1.2 运行gcc lex.yy.c -lfl -o scanner<br>
1.1.3 运行 ./scanner test.c，得到想要的结果<br>
注：使用正则表达式对代码进行匹配时如果匹配成功则输出规则部分定义的匹配成功的操作，否则直接将原始代码输出。
### 1.2 词法分析
1.2.1 实验要求：识别非法字符，浮点数，八进制/十六进制，注释<br>
1.2.2 实现思路：先匹配合法的再匹配不合法的<br>


## 实验二：语法分析
### 2.1 使用Flex+Bison写计算器的例子 
参考代码：https://github.com/meyerd/flex-bison-example
### 2.2 正式实验：
2.2.1：设计包含全部生成式的测试c代码，使用写好的词法分析器(project2.l)和语法分析器(project2.y)对测试代码进行分析，要求无语法错误。
运行命令 `bison project2.y && flex project2.l && gcc project2.tab.c lex.yy.c && ./a.out test.c`<br>
2.2.2：

