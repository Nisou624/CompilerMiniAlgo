flex Lex.l
bison -d bis.y
gcc lex.yy.c bis.tab.c -lfl -ly -o suu.exe