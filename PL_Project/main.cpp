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
typedef struct CharStruct{
    char ch = '\0';
    string type = "\0";
    CharStruct* nextChar = NULL;
} CharStructStruct; // single char structure

typedef struct TokenStruct{
    CharStruct* token;
    TokenStruct* nextToken = NULL;
} TokenStruct; // single expression structure

// read all the expressions
TokenStruct* ReadSExp() {
    TokenStruct* head = NULL;
    TokenStruct* newToken = new TokenStruct;
    CharStruct* charCursor = NULL;
    head = newToken;
    charCursor = newToken->token;
    while (cin.peek() != '\n') {
        while (cin.peek() != ' ' && cin.peek() != '\n') {
            newToken->token->ch = cin.get();
            // move the cursor
            charCursor->nextChar = new CharStruct;
            charCursor = charCursor->nextChar;
        } // get one token
        if (cin.peek() == ' ') {
            // get the spaces
            while (cin.peek() == ' ') cin.get();
            // move the token
            newToken->nextToken = new TokenStruct;
            newToken = newToken->nextToken;
            charCursor = newToken->token;
        } // if the next char is space
        if (cin.peek() == '\n') {
            break;
        }
    }
    return head;
} // read the tokens

//string PrintSExpr(ExprStruct* head) {
//    ExprStruct* cursor = head;
//    string returnToken = "\0";
//    while (cursor != NULL) {
//        if (cursor->expr == "(exit)") {
//            return cursor->expr;
//        }
//        else {
//            cout << cursor->expr << endl;
//        }
//        cursor = cursor->next;
//    }
//    return returnToken;
//} // print our the tokens stored in the pointer

int main(int argc, const char * argv[]) {
//    int uTestNum = 0;
    bool end = false;
    cout << "Welcome to OurScheme!" << endl;
    cout << endl << "Thanks for using OurScheme!" << endl << endl;
    ReadSExp();
//    do {
//        cout << "> ";
//        if (PrintSExpr(ReadSExp()) == "(exit)") {
//            end = true;
//        }
//    } while (!end);
} // main screen
