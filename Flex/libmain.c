/* libmain - flex run-time support library "main" function */

/* $Header: /tmp/cvsroot/Cola/Flex/libmain.c,v 2.3.8.1 1997-07-09 23:19:45 distler Exp $ */

extern int yylex();

int main( argc, argv )
int argc;
char *argv[];

    {
    return yylex();
    }
