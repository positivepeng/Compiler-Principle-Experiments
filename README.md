# Compiler-Principle-Experiments
编译原理课程实验

## 实验一：词法分析
### 1.1 单词记数示例
1.1.1 编写wc.l，运行flex wc.l，生成lex.yy.c<br>
1.1.2 运行gcc lex.yy.c -lfl -o scanner<br>
1.1.3 运行 ./scanner test.c，得到想要的结果<br>

### 1.2 词法分析
1.2.1 实验要求：识别非法字符，浮点数，八进制/十六进制，注释<br>
1.2.2 实现思路：先匹配合法的再匹配不合法的<br>