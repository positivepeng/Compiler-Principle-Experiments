# Compiler-Principle-Experiments
编译原理课程实验

## 实验一：词法分析
### 1.1 单词记数示例
1.1.1 编写wc.l，运行flex wc.l，生成lex.yy.c
1.1.2 运行gcc lex.yy.c -lfl -o scanner
1.1.3 运行 ./scanner test.c，得到想要的结果
