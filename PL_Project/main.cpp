//  main.cpp
//  PL_Project

// project synatax
// <S-exp> ::= <ATOM> | LEFT-PAREN <S-exp> { <S-exp> } [ DOT <S-exp> ] RIGHT-PAREN | QUOTE <S-exp>
// <ATOM>  ::= SYMBOL | INT | FLOAT | STRING | NIL | T | LEFT-PAREN RIGHT-PAREN

// <BE> ::= <E> EQ <E>
// <E>  ::= <T> { ADD <T> | MINUS <T> }
// <T>  ::= <F> { * <F> | / <F> }
// <F>  ::= NUM | ID | ( <E> )

// Parser call Scanner with GetToken() and PeekToken()

// include libraries
#include <_ctype.h>
#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;

// define structures
typedef struct TokenStruct{
    string value = "\0";
} TokenStruct; // single char structure

typedef struct TreeStruct{
    TokenStruct* token;
    TreeStruct* left = NULL;
    TreeStruct* right = NULL;
} TreeStruct; // single expression structure

void CreateTree() {
    
} // create dot list tree

string GetToken( string atom ) {
    char next = '\0';
    char peek = '\0';
    next = cin.get();
    cout << next;
    switch ( int(next) ) {
        case 48 ... 57: // numbers
            atom += next;
        case 40: // '(' left bracket 
            CreateTree();
    }
    peek = cin.peek();
    if ( peek == ' ' )
        return atom;
    else
        GetToken(atom);
    return atom;
} // get token using recursive

string ReadSExp() {
    string exp = "\0";
    return exp;
    //call PeekToken
} // read and process the expression

// main function
int main(int argc, const char * argv[]) {
    //int uTestNum = 0;
    bool end = false;
    cout << "Welcome to OurScheme!" << endl;
    do {
        cout << "> ";
        GetToken("\0");
        //call GetToken and PeekToken
        //if ( ReadSExp() == "exit" ) {
            //end = true;
        //} // check exit
        //PrintSExp();
        cout << endl;
    } while (!end);
    cout << endl << "Thanks for using OurScheme!" << endl << endl;
} // main screen
