###############################################################################
# purpose: Assignment 3 example recursive descent parser
#  author: J. Edward Swan II
###############################################################################
LEX		=	flex
.PRECIOUS 	=	example-parser.cpp example-rules.l lexer.h 

parse.exe: lex.yy.o my_parser.o
	g++ -o $@ $^

lex.yy.o: lex.yy.c lexer.h
	gcc -o $@ -c lex.yy.c

example-parser.o: my_parser.cpp lexer.h
	g++ -o $@ -c example-parser.cpp

lex.yy.c: rules.l
	$(LEX) $^

clean: 
	$(RM) *.o lex.yy.c parse.exe

