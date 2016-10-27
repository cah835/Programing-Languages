//
//  my_parser.cpp
//  assignment_3
//
//  Created by Corey Henry on 10/19/16.
//  Copyright © 2016 Corey Henry. All rights reserved.
//
#include <stdio.h>
#include <iostream>
#include <string>
#include <map>

using namespace std;
#include "lexer.h"


extern "C"
{
    // Instantiate global variables
    extern FILE *yyin;         // input stream
    extern FILE *yyout;        // output stream
    extern int   yyleng;       // length of current lexeme
    extern char *yytext;       // text of current lexeme
    extern int   yylex();      // the generated lexical analyzer
    extern int   yyLine;       // the current source code line
}

// Production functions
void  P( void );
void S( void );
void  A( void );
float E( void );
float B( void );
float R( void );
float F( void );
float T( void );
float F( void );
float U( void );
void G( void );
void O( void );
void C( void );
void W( void );

// Function declarations for checking whether the current token is
// in the first set of each production rule.
bool Check_P( void );
bool Check_S( void );
bool Check_A( void );
bool Check_E( void );
bool Check_B( void );
bool Check_R( void );
bool Check_F( void );
bool Check_T( void );
bool Check_F( void );
bool Check_U( void );
bool Check_G( void );
bool Check_O( void );
bool Check_C( void );
bool Check_W( void );

// Function to help print the tree nesting
string psp( int );

// Data type for the Symbol Table
typedef map<string, float> SymbolTableT;

// Needed global variables
int iTok;                  // The current token
SymbolTableT SymbolTable;  // The symbol table


//*****************************************************************************
// The main processing loop
int main( int argc, char* argv[] )
{
    int token;   // hold each token code
    
    // Set the input and output streams
    yyin = stdin;
    yyout = stdout;
    
    // Get the first token
    iTok = yylex();
    
    // Fire up the parser!
    try
    {
        if( !Check_P() ) // Check for {
            throw "missing '{' at beginning of program";
        
        // Process P Production
        P();
        
        if( iTok != TOK_EOF )
            throw "end of file expected, but there is more here!";
    }
    catch( char const *errmsg )
    {
        cout << endl << "***ERROR (line " << yyLine << "): "<< errmsg << endl;
        return 1;
    }
    
    // Tell the world about our success!!
    cout << endl
    << "=== GO BULLDOGS! Your parse was successful! ==="
    << endl << endl;;
    
    // Print out the symbol table
    SymbolTableT::iterator it;
    for( it = SymbolTable.begin(); it != SymbolTable.end(); ++it ) {
        cout << "symbol = " << it->first << ", value = " << it->second << endl;
    }
    
    return 0;
}


//*****************************************************************************
// P --> { A \{ A \} }
void P(void)
{
    static int Pcnt = 0; // Count the number of P's
    int CurPcnt = Pcnt++;
    char const *Perr =
    "assignment statement does not start with 'let'";
    
    cout << psp( CurPcnt ) << "enter P " << CurPcnt << endl;
    
    // We know the first token is '{'; talk about it and read next token
    cout << "-->found " << yytext << endl;
    iTok = yylex();
    
    // We next expect to see an S
    if( Check_S() )
        S();
    else
        throw Perr;
    
    // The A might be followed by a series of S's
    while( Check_S() )
        S();
    
    // Last should be a '}'
    //cout << "yytext is " << yytext << endl;
    //cout << "iTok is " << iTok << endl;
    if( iTok != TOK_CLOSEBRACE )
        throw "missing '}' at end of program";
    cout << "-->found " << yytext << endl;
    
    // Read the next token
    iTok = yylex();
    
    cout << psp( CurPcnt ) << "exit P " << CurPcnt << endl;
}

//*****************************************************************************
// S -->
void S(void)
{
    float rValue;        // Value returned from expression
    static int Scnt = 0; // Count the number of S's
    int CurScnt = Scnt++;
    
    cout << psp( CurScnt ) << "enter S " << CurScnt << endl;
    //determine what token case we have
    switch (iTok)
    {
        case TOK_LET:
            A();
            break;
            
        case TOK_READ:
            G();
            break;
        
        case TOK_PRINT:
            O();
            break;
            
        case TOK_IF:
            C();
            break;
            
        case TOK_WHILE:
            W();
            break;
        
        default:
            // If we made it to here, syntax error
            throw "factor does not start with 'LET' | 'READ' | 'PRINT' | 'IF' | 'WHILE'";
    }
    
    cout << psp( CurScnt ) << "exit S " << CurScnt << endl;
    return;
}

//*****************************************************************************
// A --> let ID := E ;
void A(void)
{
    float rValue;        // Value returned from expression
    static int Acnt = 0; // Count the number of A's
    int CurAcnt = Acnt++;
    
    cout << psp( CurAcnt ) << "enter A " << CurAcnt << endl;
    
    // We know we have found a 'let' token
    cout << "-->found " << yytext << endl;
    
    // Next should be an identifier; save its name
    iTok = yylex();
    if( iTok != TOK_IDENTIFIER )
        throw "missing identifier in assignment statement";
    cout << "-->found ID: " << yytext << endl;
    string IDname = yytext;
    
    // Next shoud be an assignment statement
    iTok = yylex();
    if( iTok != TOK_ASSIGN )
        throw "missing ':=' symbol in assignment statement";
    cout << "-->found " << yytext << endl;
    
    // Next should be an expression
    iTok = yylex();
    if( Check_E() )
        rValue = E();
    else
        throw "missing expression in assignment statement";
    
    
    // If the identifier is not yet in the symbol table, store it there
    SymbolTableT::iterator it = SymbolTable.find( IDname );
    //cout << "symbol table says: " << (it == SymbolTable.end()) << endl;
    if( it == SymbolTable.end() ) {
        SymbolTable.insert( pair<string, float>(IDname, 1.0) );
    }
    
    // Update ID in symbol table with value from expression
    it = SymbolTable.find( IDname );
    it->second = rValue;
    //cout << "key: " << it->first << " val: " << it->second << endl;
    
    // Last should be a ';' token
    if( iTok != TOK_SEMICOLON )
        throw "missing ';' at end of assignment statement";
    cout << "-->found " << yytext << endl;
    
    // Read the next token
    iTok = yylex();
    
    
    cout << psp( CurAcnt ) << "exit A " << CurAcnt << endl;
    
}

//*****************************************************************************
// E --> T { ( + | - ) T }
float E(void)
{
    float rValue1 = 0;   // The value to return
    float rValue2;
    static int Ecnt = 0; // Count the number of E's
    int CurEcnt = Ecnt++;
    char const *Eerr = "term does not start with 'ID' | 'FLOATLIT' | '('";
    
    cout << psp( CurEcnt ) << "enter E " << CurEcnt << endl;

    // We next expect to see a R
    if( Check_B() )
        rValue1 = B();
    else
        throw "Term does not start with B";
    //check for the rest
    while( iTok == TOK_AND || iTok == TOK_OR)
    {
        if(iTok == TOK_AND)
        {
            cout << "-->found " << yytext << endl;
            iTok = yylex();
            
        }
        
        if (iTok == TOK_OR)
        {
            cout << "-->found " << yytext << endl;
            iTok = yylex();
            
        }
        
        if(!Check_B())
        {
            throw "B expected but not found";
        }
        //R has been found now go to it.
        rValue1 = B();
        
        
    }
    cout << psp( CurEcnt ) << "exit E " << CurEcnt << endl;
    
    return rValue1;
}

//*****************************************************************************
// B --> R [( < | > | == ) R ]
float B(void)
{
    
    float rValue1 = 0;   // The value to return
    float rValue2;
    static int Bcnt = 0; // Count the number of B's
    int CurBcnt = Bcnt++;
    char const *Berr = "factor does not start with 'ID' | 'FLOATLIT' | '('";
    
    cout << psp( CurBcnt ) << "enter B " << CurBcnt << endl;
  
    // We next expect to see a R
    if( Check_R() )
        rValue1 = R();
    else
        throw "term does not start with R";
    //check for the rest
    if( iTok == TOK_GREATERTHAN || iTok == TOK_LESSTHAN || iTok == TOK_EQUALTO)
    {
        if(iTok == TOK_LESSTHAN)
        {
            cout << "-->found " << yytext << endl;
        }
        
        if (iTok == TOK_GREATERTHAN)
        {
            cout << "-->found " << yytext << endl;
        }
        
        if(iTok == TOK_EQUALTO)
        {
            cout << "-->found " << yytext << endl;
        }
        iTok = yylex();
        // check for R
        if(!Check_R())
        {
            throw "R expected but not found";
        }
        //R has been found now go to it.
        rValue1 = R();
        
    }// end of if for < || > || ==
    
    
    cout << psp( CurBcnt ) << "exit B " << CurBcnt << endl;
    
    return rValue1;
}

//*****************************************************************************
// R --> T {( * | / ) T }
float R(void)
{
    float rValue1 = 0;   // The value to return
    float rValue2;
    static int Rcnt = 0; // Count the number of R's
    int CurRcnt = Rcnt++;
    char const *Rerr = "factor does not start with 'ID' | 'FLOATLIT' | '('";
    
    cout << psp( CurRcnt ) << "enter R " << CurRcnt << endl;
    
    // We next expect to see a F
    if( Check_T() )
        rValue1 = T();
    else
        throw Rerr;
    
    // As long as the next token is * or /, keep parsing F's
    while( iTok == TOK_PLUS || iTok == TOK_MINUS )
    {
        cout << "-->found " << yytext << endl;
        int iTokLast = iTok;
        iTok = yylex();
        if( Check_T() )
            rValue2 = T();
        else
            throw Rerr;
        
        // Perform the operation to update rValue1 acording to rValue2
        switch( iTokLast )
        {
            case TOK_PLUS:
                rValue1 = rValue1 + rValue2;
                break;
                
            case TOK_MINUS:
                rValue1 = rValue1 - rValue2;
        }
    }
    
    cout << psp( CurRcnt ) << "exit R " << CurRcnt << endl;
    
    return rValue1;
    
}

//*****************************************************************************
// T --> F { ( * | / ) F }
float T(void)
{
    float rValue1 = 0;   // The value to return
    float rValue2;
    static int Tcnt = 0; // Count the number of T's
    int CurTcnt = Tcnt++;
    char const *Terr = "factor does not start with 'ID' | 'FLOATLIT' | '('";
    
    cout << psp( CurTcnt ) << "enter T " << CurTcnt << endl;
    
    // We next expect to see a F
    if( Check_F() )
        rValue1 = F();
    else
        throw Terr;
    
    // As long as the next token is * or /, keep parsing F's
    while( iTok == TOK_MULTIPLY || iTok == TOK_DIVIDE )
    {
        cout << "-->found " << yytext << endl;
        int iTokLast = iTok;
        iTok = yylex();
        if( Check_F() )
            rValue2 = F();
        else
            throw Terr;
        
        // Perform the operation to update rValue1 acording to rValue2
        switch( iTokLast )
        {
            case TOK_MULTIPLY:
                rValue1 = rValue1 * rValue2;
                break;
                
            case TOK_DIVIDE:
                rValue1 = rValue1 / rValue2;
        }    
    }
    
    cout << psp( CurTcnt ) << "exit T " << CurTcnt << endl;
    
    return rValue1;
}

//*****************************************************************************
// F --> [ not | - ] U
float F(void)
{
    float rValue = 0;           // the value to return
    SymbolTableT::iterator it;  // look up values in symbol table
    static int Fcnt = 0;        // Count the number of F's
    int CurFcnt = Fcnt++;
    
    cout << psp( CurFcnt ) << "enter F " << CurFcnt << endl;
    
    // Determine what token we have
    if(iTok == TOK_NOT || iTok == TOK_MINUS)
    {
        switch( iTok )
        {
            case TOK_NOT:
                cout << "-->found " << yytext << endl;
                // Read past what we have found
                iTok = yylex();
                break;
            
            case TOK_MINUS:
                cout << "-->found " << yytext << endl;
                iTok = yylex();
                break;
            
            default:
                // If we made it to here, syntax error
                throw "factor does not start with 'NOT' | '-'";
        }
    }
    //check for U
    if(!Check_U())
    {
        throw "factor does not  have U";
    }
    // go to U
    rValue = U();

    cout << psp( CurFcnt ) << "exit F " << CurFcnt << endl;
    
    return rValue;
}

//*****************************************************************************
// U --> id | FLOATLIT | (E)
float U(void)
{
    float rValue = 0;           // the value to return
    SymbolTableT::iterator it;  // look up values in symbol table
    static int Ucnt = 0;        // Count the number of U's
    int CurUcnt = Ucnt++;
    
    cout << psp( CurUcnt ) << "enter U " << CurUcnt << endl;
    
    // Determine what token we have
    switch( iTok )
    {
        case TOK_IDENTIFIER:
            cout << "-->found ID: " << yytext << endl;
            
            // Look up value of identifier in symbol table
            it = SymbolTable.find( yytext );
            // If the symbol is not in the table, uninitialized identifier error
            if( it == SymbolTable.end() )
                throw "uninitialized identifier used in expression";
            // Return the value of the identifier
            rValue = it->second;
            
            // Read past what we have found
            iTok = yylex();
            break;
            
        case TOK_FLOATLIT:
            cout << "-->found FLOATLIT: " << yytext << endl;
            
            // Capture the value of this literal
            rValue = (float)atof( yytext );
            
            iTok = yylex();
            break;
            
        case TOK_OPENPAREN:
            // We expect (E); parse it
            cout << "-->found (" << endl;
            iTok = yylex();
            rValue = E();
            if( iTok == TOK_CLOSEPAREN ) {
                cout << "-->found )" << endl;
                iTok = yylex();
            }
            else
                throw "E does not end with )";
            break;
            
        default:
            // If we made it to here, syntax error
            throw "factor does not start with 'ID' | 'FLOATLIT' | '('";
    }
    
    cout << psp( CurUcnt ) << "exit U " << CurUcnt << endl;
    
    
    return rValue;
}



//*****************************************************************************
// G --> read [ stringlit ] id
void G(void)
{
    static int Gcnt = 0; // Count the number of O's
    int CurGcnt = Gcnt++;
    char const *Gerr = "assignment statement does not start with 'let'";
    
    cout << psp( CurGcnt ) << "enter G " << CurGcnt << endl;
    
    // We know the first token is '{'; talk about it and read next token
    cout << "-->found " << yytext << endl;
    
    //get next token
    iTok = yylex();
    
    if(iTok == TOK_STRINGLIT)
    {
        cout << "-->found string: " << yytext << endl;
        //get a new iTok
        iTok = yylex();
    }
    
    if(iTok != TOK_IDENTIFIER)
    {
        throw "factor does not have IDENTIFIER";
    }
    cout << "-->found ID: " << yytext << endl;
    // If the identifier is not yet in the symbol table, store it there
    SymbolTableT::iterator it = SymbolTable.find( yytext );
    //cout << "symbol table says: " << (it == SymbolTable.end()) << endl;
    if( it == SymbolTable.end() ) {
        SymbolTable.insert( pair<string, float>(yytext, 1.0) );
    }
    //get a new iTok
    iTok = yylex();
    //check for semicolon
    if(iTok != TOK_SEMICOLON)
    {
        throw " ERROR : missing semicolon";
    }
    cout << "-->found " << yytext << endl;
    
    // get a new itok
    iTok = yylex();
    
    cout << psp( CurGcnt ) << "exit G " << CurGcnt << endl;
    return;

}

//*****************************************************************************
// O --> print [ Stringlit] [id]
void O(void)
{
    static int Ocnt = 0; // Count the number of O's
    int CurOcnt = Ocnt++;
    char const *Oerr = "assignment statement does not start with 'let'";
    
    cout << psp( CurOcnt ) << "enter O " << CurOcnt << endl;
    
    // We know the first token is '{'; talk about it and read next token
    cout << "-->found " << yytext << endl;
    iTok = yylex();
    if(iTok == TOK_STRINGLIT)
    {
        cout << "-->found string: " << yytext << endl;
        //get a new iTok
        iTok = yylex();
    }
    
    if(iTok == TOK_IDENTIFIER)
    {
        cout << "-->found ID: " << yytext << endl;
        //get a new iTok
        iTok = yylex();
    }
    //check for semicolon
    if(iTok != TOK_SEMICOLON)
    {
        throw " ERROR : missing semicolon";
    }
    cout << "-->found " << yytext << endl;
    
    // get a new itok
    iTok = yylex();
    
    cout << psp( CurOcnt ) << "exit O " << CurOcnt << endl;
    return;

}

//*****************************************************************************
// C --> if (E) P [ else P ]
void C(void)
{
    static int Ccnt = 0; // Count the number of C's
    int CurCcnt = Ccnt++;
    char const *Cerr =
    "assignment statement does not start with 'let'";
    
    cout << psp( CurCcnt ) << "enter C " << CurCcnt << endl;
    
    // We know the first token is '{'; talk about it and read next token
    cout << "-->found " << yytext << endl;
    iTok = yylex();
    //check to make sure you get E
    if(iTok != TOK_OPENPAREN)
    {
        throw "factor is missing (";
    }
    cout << "-->found " << yytext << endl;
    //get the next itok
    iTok = yylex();
    if(!Check_E())
    {
        throw "factor does not have (E) as next statement";
    }
    //E was found go to E function
    E();
    if(iTok != TOK_CLOSEPAREN)
    {
        throw "factor does not have a close brace";
    }
    cout << "-->found " << yytext << endl;
    // get the next iTok
    iTok = yylex();
    // returned from E now check for P
    if(!Check_P())
    {
        throw "Factor does not have (P) as next statement";
    }
    // P was found go to P function
    P();
    // check for [ else P ]
    if(iTok == TOK_ELSE)
    {
        cout << "-->found " << yytext << endl;
        //get a new iTok
        iTok = yylex();
        //check for P
        if(!Check_P())
        {
            throw "Factor does not have (P) as next statement";
        }
        // P was found go to P function
        P();
    }
    
    cout << psp( CurCcnt ) << "exit C " << CurCcnt << endl;
    return;
}

//*****************************************************************************
// W --> while (E) P
void W(void)
{
    static int Wcnt = 0; // Count the number of W's
    int CurWcnt = Wcnt++;
    char const *Werr = "assignment statement does not start with 'let'";
    
    cout << psp( CurWcnt ) << "enter W " << CurWcnt << endl;
    
    // We know the first token is '{'; talk about it and read next token
    cout << "-->found " << yytext << endl;
    iTok = yylex();
    //check to make sure you get E
    if(iTok != TOK_OPENPAREN)
    {
        throw "factor is missing (";
    }
    cout << "-->found " << yytext << endl;
    //get the next itok
    iTok = yylex();
    if(!Check_E())
    {
        throw "factor does not have (E) as next statement";
    }
    //E was found go to E function
    E();
    //check for )
    if(iTok != TOK_CLOSEPAREN)
    {
        throw "factor does not have a close brace";
    }
    cout << "-->found " << yytext << endl;
    // get the next iTok
    iTok = yylex();
    
    //check and run P
    if(!Check_P())
    {
        throw "Factor does not have (P) as next statement";
    }
    // P was found go to P function
    P();
    cout << psp( CurWcnt ) << "exit W " << CurWcnt << endl;
    return;
}

//declare all the check functions


//function for check P
bool Check_P(void)
{
    return iTok == TOK_OPENBRACE;
}

//function for check S
bool Check_S(void)
{
    return iTok == TOK_LET || iTok == TOK_READ || iTok == TOK_PRINT || iTok == TOK_IF || iTok == TOK_WHILE;
}

//function for check A
bool Check_A(void)
{
    return iTok == TOK_LET;
}

//function for check E
bool Check_E(void)
{
    return iTok == TOK_NOT || iTok == TOK_MINUS || iTok == TOK_IDENTIFIER || iTok == TOK_FLOATLIT || iTok == TOK_OPENPAREN;
}

//function for check B
bool Check_B(void)
{
    return iTok == TOK_NOT || iTok == TOK_MINUS || iTok == TOK_IDENTIFIER || iTok == TOK_FLOATLIT || iTok == TOK_OPENPAREN;
}

//function for check R
bool Check_R(void)
{
    return iTok == TOK_NOT || iTok == TOK_MINUS || iTok == TOK_IDENTIFIER || iTok == TOK_FLOATLIT || iTok == TOK_OPENPAREN;
}

//function for check T
bool Check_T(void)
{
    return iTok == TOK_NOT || iTok == TOK_MINUS || iTok == TOK_IDENTIFIER || iTok == TOK_FLOATLIT || iTok == TOK_OPENPAREN;
}

//function for check F
bool Check_F(void)
{
    return iTok == TOK_NOT || iTok == TOK_MINUS || iTok == TOK_IDENTIFIER || iTok == TOK_FLOATLIT || iTok == TOK_OPENPAREN;
}

//function for check U
bool Check_U(void)
{
    return iTok == TOK_IDENTIFIER || iTok == TOK_FLOATLIT || iTok == TOK_OPENPAREN;
}

//function for check G
bool Check_G(void)
{
    return iTok == TOK_READ;
}

//function for check O
bool Check_O(void)
{
    return iTok == TOK_PRINT;
}

//function for check C
bool Check_C(void)
{
    return iTok == TOK_IF;
}

//function for check W
bool Check_W(void)
{
    return iTok == TOK_WHILE;
}

//*****************************************************************************
string psp( int n ) // Stands for p-space, but I want the name short
{
    string str( n, ' ' );
    return str;
}

