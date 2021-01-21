//  main.cpp
//  PL_Project

// <ATOM>  ::= SYMBOL | INT | FLOAT | STRING | NIL | T | LEFT-PAREN RIGHT-PAREN
// <S-exp> ::= <ATOM> | LEFT-PAREN <S-exp> { <S-exp> } [ DOT <S-exp> ] RIGHT-PAREN | QUOTE <S-exp>

#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;

typedef struct ExprStruct{
    string expr = "\0";
    string type = "\0";
    ExprStruct* next = NULL;
} ExprStruct; // single expr structure

ExprStruct* ReadSExp() {
    ExprStruct* head = NULL;
    ExprStruct* newToken = new ExprStruct;
    head = newToken;
    while (cin.peek() != '\n') {
        while (cin.peek() != ' ' && cin.peek() != '\n') {
            newToken->expr += cin.get();
        } // get one token
        if (cin.peek() != '\n') {
            cin.get();
            newToken->next = new ExprStruct;
            newToken = newToken->next;
        } // get the spaces
    }
    return head;
} // read the tokens

void PrintSExpr(ExprStruct* head) {
    ExprStruct* cursor = head;
    while (cursor != NULL) {
        cout << cursor->expr << endl;
        cursor = cursor->next;
    }
} // print our the tokens stored in the pointer

int main(int argc, const char * argv[]) {
//    int uTestNum = 0;
//    bool end = false;
    cout << "Welcome to OurScheme!" << endl;
//    do {
    cout << "> ";
    PrintSExpr(ReadSExp());
//    } while (!end);
    cout << endl <<"Thanks for using OurScheme!";
} // main screen
