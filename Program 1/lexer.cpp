//*****************************************************************************
// purpose: CSE 4713 / 6713 Assignment 1: Lexical Analyzer
// created: 8-25-2015
//  author: Corey Henry cah835
//*****************************************************************************
#include "lexer.h"
#include <ctype.h>
#include <assert.h>
#include <string.h>

//*****************************************************************************
// Do the lexical parsing
char lexeme[MAX_LEXEME_LEN];  // Character buffer for lexeme
char *temp;
int type;
static char c;



//yylex function
int yylex()
{
    //read the first letter if needed
    if(!c)
    {
    c = -1;
    }
    // Prepare the buffer to hold the lexeme
    for( int i = 0; i < MAX_LEXEME_LEN; i++ )
        lexeme[i] = '\0';
    yytext = lexeme;
    yyleng = 0;
    
    // If the very first character has not yet been read, read it
    if( c < 0 )
        c = fgetc( yyin );
    
    // Test for end of file
    if( feof(yyin) )
    {
        // Ready for next time
        c = -1;
        // Done!
        return( TOK_EOF );
    }
    //check the type
    type = typechecker();
    //if type = whitespace skip it and start over
    while( type == 1 )
    {
        //call get and restart loop
        // Store current character and read the next
        c = fgetc( yyin );
        type = typechecker();
    }
    if( type == 2 ) //if type = puncuation go to puncuation branch
    {
        lexeme[yyleng++] = c;
        //get()
        c = fgetc( yyin );
        //check the new type
        type = typechecker();
        return( pun_checker() );
    }
    //if type = " go to string_lit branch
    else if( type == 3 )
    {
        lexeme[yyleng++] = c;
        //get()
        c = fgetc( yyin );
        //check the new type
        type = typechecker();
        while( type != 3 )
        {
            lexeme[yyleng++] = c;
            //get()
            c = fgetc( yyin );
            //check the new type
            type = typechecker();
        }
        if( type == 3 )
        {
            lexeme[yyleng++] = c;
            //get()
            c = fgetc( yyin );
            //check the new type
            type = typechecker();
            return( TOK_STRINGLIT );
        }
        else
        {
            return( TOK_EOF_SL );
        }
    }// end of string_lit branch
    else if(type == 4) //if type = letter go to identifier branch
    {
        // add()
        lexeme[yyleng++] = c;
        //get()
        c = fgetc( yyin );
        //check the new type
        type = typechecker();
        //while the new type is another letter or digit keep adding and getting
        while( type == 4 || type == 5 )
        {
            // add and get
            lexeme[yyleng++] = c;
            c = fgetc( yyin );
            type = typechecker();
        }//end of the while loop for letter or
        //call string checker to determine what the token is
        return( string_checker() );
    }//end of identifier branch if statement
    else if( type == 5 ) //if type = digit go to int lit branch
    {
        bool f = false;
        // add()
        lexeme[yyleng++] = c;
        //get()
        c = fgetc( yyin );
        //check the new type
        type = typechecker();
        //while loop that will run till the digit stops and will return a token or will keep an eye out to switch to float branch
        while( type == 5 )
        {
            // add()
            lexeme[yyleng++] = c;
            //get()
            c = fgetc( yyin );
            //check the new type
            type = typechecker();
        }//end of digit while
        if( type == 6 )// checking to see if it is a float
        {
            f = true;
            // add()
            lexeme[yyleng++] = c;
            //get()
            c = fgetc( yyin );
            //check the new type
            type = typechecker();
            //while loop to get the rest of the digits
            while( type == 5 )
            {
                // add()
                lexeme[yyleng++] = c;
                //get()
                c = fgetc( yyin );
                //check the new type
                type = typechecker();
            }//end of while for more digits to float
        }//end of if for float
        //if f is true return float int token if false return digit lit
        if( f == true )
            return( TOK_FLOATLIT);
        
        else
            return(TOK_INTLIT);
    
    }//end of int_lit if else statement
    else if( type == 7 )//if type = operator go to operator branch
    {
        lexeme[yyleng++] = c;
        //get()
        c = fgetc( yyin );
        //check the new type
        type = typechecker();
        
        //if there is another opperator loop again
        if( type == 7 )
        {
            lexeme[yyleng++] = c;
            //get()
            c = fgetc( yyin );
            //check the new type
            type = typechecker();
        }
        return( op_checker() );
    }//end of opperator else if branch
    else if ( type == 9)
    {
    // if EOF then return to prevent ?
    if( c== EOF)
    {
        return(TOK_EOF);
    }
        
    lexeme[yyleng++] = c;
    c = fgetc( yyin );
    // We don't yet know how to recognize any lexemes
    return( TOK_UNKNOWN );
    }
}


//function to determine what we got for the lexeme
char typechecker()
{
    
    //checks for whitespace and calls get again
    if(isspace(c))
    {
        type = 1;
    }
    
    //is it a puncuation
    else if(c == ';' || c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}' || c == ',')
    {
        type = 2;
    }
    //check to see if it starts or ends a string
    else if(c == '\"')
    {
        type= 3;
    }
    
    //is it a letter
    else if(isalpha(c) == true)
    {
        type = 4;
    }
    
    //is it a didgit
    else if(isdigit(c) == true)
    {
        type = 5;
    }
    
    //is it a period for floats
    else if(c== '.')
    {
        type = 6;
    }
    
    //is it an opperator
    else if(c == '+' || c == '-' || c == '*' || c == '/' || c == '<' || c == '>' || c == '=' || c == ':' )
    {
        type = 7;
    }
    
    else if(c == '\"')
    {
        type = 8;
    }
    //everything else is unknown
    else
    {
        type = 9;
    }
    
    return(type);
}//end of typechecker





//function checking lexeme to see if it is a key word and returns that token, if it is not a keyword it just returns identifier token
int string_checker()
{
    temp = lexeme;
    
    //start of the keywords
    if(!strcmp(temp,"if"))
    {
        return TOK_IF;
    }
    
    else if(!strcmp(temp,"else"))
    {
        return TOK_ELSE;
    }
    
    else if(!strcmp(temp,"for"))
    {
        return TOK_FOR;
    }
    
    else if(!strcmp(temp,"while"))
    {
        return TOK_WHILE;
    }
    
    else if(!strcmp(temp,"print"))
    {
        return TOK_PRINT;
    }
    
    else if(!strcmp(temp,"for"))
    {
        return TOK_FOR;
    }
    
    else if(!strcmp(temp,"return"))
    {
        return TOK_RETURN;
    }
    
    else if(!strcmp(temp,"continue"))
    {
        return TOK_CONTINUE;
    }
    
    else if(!strcmp(temp,"break"))
    {
        return TOK_BREAK;
    }
    
    else if(!strcmp(temp,"debug"))
    {
        return TOK_DEBUG;
    }
    
    else if(!strcmp(temp,"read"))
    {
        return TOK_READ;
    }
    
    else if(!strcmp(temp,"let"))
    {
        return TOK_LET;
    }
    else if(!strcmp(temp,"and"))
    {
        return TOK_AND;
    }
    
    else if(!strcmp(temp,"or"))
    {
        return TOK_OR;
    }
    
    else if(!strcmp(temp,"not"))
    {
        return TOK_NOT;
    }
    
    else if(!strcmp(temp,"length"))
    {
        return TOK_LENGTH;
    }
    
    else if(!strcmp(temp,"int"))
    {
        return TOK_INT;
    }
    
    else if(!strcmp(temp,"float"))
    {
        return TOK_FLOAT;
    }
    
    else if(!strcmp(temp,"string"))
    {
        return TOK_STRING;
    }
    
    // if there is a string of letters and none of them match the above tokens it is just a stringlit
    else
    {
        return TOK_IDENTIFIER;
    }
    
}// end of string_checker function

//function to check what puncuation it is and returns the correct token
int pun_checker()
{
    temp = lexeme;
    
    if(!strcmp(temp,";"))
    {
        return TOK_SEMICOLON;
    }
    
    else if(!strcmp(temp,"("))
    {
        return TOK_OPENPAREN;
    }
    
    else if(!strcmp(temp,")"))
    {
        return TOK_CLOSEPAREN;
    }
    
    else if(!strcmp(temp,"["))
    {
        return TOK_OPENBRACKET;
    }
    
    else if(!strcmp(temp,"]"))
    {
        return TOK_CLOSEBRACKET;
    }
    
    else if(!strcmp(temp,"{"))
    {
        return TOK_OPENBRACE;
    }
    
    else if(!strcmp(temp,"}"))
    {
        return TOK_CLOSEBRACE;
    }
    
    else if(!strcmp(temp,","))
    {
        return TOK_COMMA;
    }
    
}//end of pun_checker function


//function to check what operator it is and returns the correct token
int op_checker()
{
    temp = lexeme;
    if(!strcmp(temp,"+"))
    {
        return TOK_PLUS;
    }
    
    else if(!strcmp(temp,"-"))
    {
        return TOK_MINUS;
    }
    
    else if(!strcmp(temp,"*"))
    {
        return TOK_MULTIPLY;
    }
    
    else if(!strcmp(temp,"/"))
    {
        return TOK_DIVIDE;
    }
    
    else if(!strcmp(temp,":="))
    {
        return TOK_ASSIGN;
    }
    
    else if(!strcmp(temp,"=="))
    {
        return TOK_EQUALTO;
    }
    
    else if(!strcmp(temp,"<"))
    {
        return TOK_LESSTHAN;
    }
    
    else if(!strcmp(temp,">"))
    {
        return TOK_GREATERTHAN;
    }
    
    else if(!strcmp(temp,"<>"))
    {
        return TOK_NOTQUALTO;
    }
    
}//end of op_checker function
