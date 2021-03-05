//  main.cpp
//  PL_Project

// project synatax
// <S-exp> ::= <ATOM> | LEFT-PAREN <S-exp> { <S-exp> } [ DOT <S-exp> ] RIGHT-PAREN | QUOTE <S-exp>
// <ATOM>  ::= SYMBOL | INT | FLOAT | STRING | NIL | T | LEFT-PAREN RIGHT-PAREN

// <BE> ::= <E> EQ <E>
// <E>  ::= <T> { ADD <T> | MINUS <T> }
// <T>  ::= <F> { * <F> | / <F> }
// <F>  ::= NUM | ID | ( <E> )

// Parser call Scanner with getToken() and peekToken()

// include libraries
#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;

// define structures
typedef struct TokenStruct{
    string token = "\0";
} TokenStruct; // single char structure

typedef struct TreeStruct{
    TokenStruct* token;
    TreeStruct* left = NULL;
    TreeStruct* right = NULL;
} TreeStruct; // single expression structure

string GetToken() {
        
}

// main function
int main(int argc, const char * argv[]) {
    //int uTestNum = 0;
    bool end = false;
    cout << "Welcome to OurScheme!" << endl;
    do {
        cout << "> ";
        //call GetToken and PeekToken
        //ReadSExp();
    } while (!end);
    cout << endl << "Thanks for using OurScheme!" << endl << endl;
} // main screen
