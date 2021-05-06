// main.cpp
// PL_Project

// include libraries
# include <cctype>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h>
# include <cctype>
# include <iostream>
# include <sstream>
# include <iomanip>
# include <string>
# include <vector>

// definition
# define NOT !

// namespace
using namespace std;

// enum declaration
enum TokenType {
		LEFT_PAREN, RIGHT_PAREN, INT, STRING, DOT, FLOAT, NIL, T, QUOTE, SYMBOL
}; // token types

enum ErrorType {
		NOT_S_EXP, NO_CLOSING_QUOTE, UNEXPECTED_TOKEN_ATOM_LEFT_PAREN, UNEXPECTED_RIGHT_PAREN, NO_MORE_INPUT,
		INCORRECT_NUM_ARGUMENTS, INCORRECT_ARGUMENT_TYPE, DEFINE_UNBOUND, APPLY_NON_FUNCITON, NO_RETURN_VALUE,
		DIVISION_BY_ZERO, NOT_LIST
}; // error types

enum PrimitiveType {
		CONSTRUCTOR, QUOTE_BYPASSING, DEFINE_BINDING, PART_ACCESSOR, PRIMITIVE_PREDICATE, OPERATOR, EQUIVALENCE,
		BEGIN, CONDITIONAL, CLEAN_ENVIRONMENT
}; // primitive types

// define structures
struct TokenStruct {
		string content;
		TokenType tokenType;
		bool isPrimitive;
		PrimitiveType primitiveType;
}; // TokenStruct

struct TreeStruct {
		TokenStruct *leftToken;
		TokenStruct *rightToken;
		TreeStruct *previousNode;
		TreeStruct *leftNode;
		TreeStruct *rightNode;
		bool leftParenCreate;
}; // TreeStruct

struct ErrorStruct {
		ErrorType errorType;
		string errorMessage;
}; // ErrorStruct

struct ResultStruct {
		TokenStruct *stringResult;
		bool hasStringResult;
		TokenStruct *symbolResult;
		bool hasSymbolResult;
		TokenStruct *boolResult;
		bool hasBoolResult;
		TokenStruct *intResult;
		bool hasIntResult;
		TokenStruct *floatResult;
		bool hasFloatResult;
		TreeStruct *nodeResult;
		bool hasNodeResult;
}; // ResultStruct

// define global variable to track the cursor
int g_CursorLine = 1;
int g_CursorColumn = 0;
int g_uTestNum = 0;

class Project1Class {
private:
	vector<TokenStruct> m_LineOfTokens;
	TreeStruct *m_Root;
	TreeStruct *m_CurrentTreeLocation;
	ErrorStruct m_Error;

public:
	/*
		------------------ Class -------------------
		------------------ Getter ------------------
	*/
	
	vector<TokenStruct> GetSExp() {
		return m_LineOfTokens;
	} // GetSExp()
	
	TreeStruct *GetRoot() {
		return m_Root;
	} // GetRoot()
	
	/*
		------------------ Check -------------------
		------------------- exit -------------------
	*/
	
	bool CheckExit() {
		string exit = "\0";
		int tokenCount = 0;
		
		for ( int i = 0 ; i < m_LineOfTokens.size() ; i++ ) {
			if ( IsAtom( m_LineOfTokens[i] ) ||
					 m_LineOfTokens[i].tokenType == LEFT_PAREN ||
					 m_LineOfTokens[i].tokenType == RIGHT_PAREN ) {
				if ( m_LineOfTokens[i].tokenType == NIL ) {
					if ( m_LineOfTokens[i - 1].tokenType != DOT &&
							 m_LineOfTokens[i + 1].tokenType != DOT ) {
						exit += m_LineOfTokens[i].content;
						tokenCount++;
					} // if: nil with no dot
				} // if: nil
				
				else {
					exit += m_LineOfTokens[i].content;
					tokenCount++;
				} // else: not nil
			} // if: attach the symbol
		} // for: go through the tokens
		
		if ( ( exit == "(exit)" && tokenCount == 3 ) ||
				 m_Error.errorType == NO_MORE_INPUT ) {
			return true;
		} // if: exit case
		
		else {
			return false;
		} // else: not exit case
	} // CheckExit()
	
	/*
		-------------------- Is --------------------
		----------------- function -----------------
	*/
	
	bool IsFloat( TokenStruct newToken ) {
		for ( int i = 0 ; i < newToken.content.length() ; i++ ) {
			if ( newToken.content[i] == '.' ) {
				return true;
			} // if: if there's a dot in the inputTerm
		} // for: go through the inputTerm
		
		return false;
	} // IsFloat()
	
	bool IsAtom( TokenStruct currentToken ) {
		if ( currentToken.tokenType == SYMBOL ||
				 currentToken.tokenType == INT ||
				 currentToken.tokenType == FLOAT ||
				 currentToken.tokenType == STRING ||
				 currentToken.tokenType == NIL ||
				 currentToken.tokenType == T ) {
			return true;
		} // if: atom
		
		else {
			return false;
		} // else: not atom
	} // IsAtom()
	
	/*
		------------------- Token ------------------
		---------------- Processing ----------------
	*/
	
	bool ReadSExp() {
		m_LineOfTokens.clear();
		m_Root = NULL;
		m_Error.errorType = NOT_S_EXP;
		m_Error.errorMessage = "\0";
		
		if ( HasNextToken() == true ) {
			if ( CheckSExp() == false ) {
				return false;
			} // if: handle the error
			
			else {
				return true;
			} // else: print <S-exp>
		} // if: check the first input
		
		else {
			return false;
		} // else: first input error
	} // ReadSExp()
	
	char PeekCharAndGetRidOfComment() {
		char peekChar = cin.peek();
		
		while ( peekChar == ' ' || peekChar == '\n' || peekChar == '\r' ||
						peekChar == '\t' || peekChar == ';' || peekChar == '\0' ) {
			if ( peekChar == ';' ) {
				while ( peekChar != '\n' && peekChar != '\r' && peekChar != EOF ) {
					peekChar = cin.get();
					g_CursorColumn++;
				} // while: get the current line
				
				if ( peekChar == EOF ) {
					return EOF;
				} // if: eof
				
				g_CursorLine++;
				peekChar = cin.peek();
				g_CursorColumn = 0;
			} // if: check if there's any comment
			
			else {
				if ( peekChar == '\n' ) {
					g_CursorLine++;
					g_CursorColumn = 0;
				} // if: next line, modifiy cursor position
				
				else {
					g_CursorColumn++;
				} // else: cursor move right
				
				cin.get();
				peekChar = cin.peek();
			} // else: get next char
		} // while: get the first non-whitespace
		
		return peekChar;
	} // PeekCharAndGetRidOfComment()
	
	string GetString() {
		char currentChar = cin.get();
		
		if ( currentChar == '\n' ) {
			SetError( NO_CLOSING_QUOTE );
			return "\0";
		} // if: check closure error
		
		else if ( currentChar == EOF ) {
			SetError( NO_MORE_INPUT );
			return "\0";
		} // else if: eof
		
		g_CursorColumn++;
		string currentString = "\0";
		
		while ( currentChar != '\"' ) {
			char peekChar = cin.peek();
			
			if ( currentChar == '\\' && peekChar == '\"' ) {
				currentString.push_back( currentChar );
				currentString.push_back( cin.get() );
				g_CursorColumn++;
			} // if: "\"" case
			
			else {
				currentString.push_back( currentChar );
			} // else: normal string
			
			currentChar = cin.get();
			
			if ( currentChar == '\n' ) {
				SetError( NO_CLOSING_QUOTE );
				return "\0";
			} // if: check closure error
			
			else if ( currentChar == EOF ) {
				SetError( NO_MORE_INPUT );
				return "\0";
			} // if: eof
			
			g_CursorColumn++;
		} // while: get the string
		
		currentString.push_back( currentChar );
		// push the last " into the string
		return currentString;
	} // GetString()
	
	bool HasNextToken() {
		char peekChar = PeekCharAndGetRidOfComment();
		
		if ( peekChar == EOF ) {
			SetError( NO_MORE_INPUT );
			return false;
		} // if gets nothing
		
		TokenStruct newToken;
		newToken.content.push_back( cin.get() );
		g_CursorColumn++;
		// initialize new token
		
		if ( peekChar == '(' ) {
			if ( PeekCharAndGetRidOfComment() == ')' ) {
				newToken.content.push_back( cin.get() );
				g_CursorColumn++;
				newToken.tokenType = NIL;
			} // if: () case
			
			else {
				newToken.tokenType = LEFT_PAREN;
			} // else: left parenthesis
		} // if: left parenthesis
		
		else if ( peekChar == ')' ) {
			newToken.tokenType = RIGHT_PAREN;
		} // else if: right parenthesis
		
		else if ( peekChar == '"' ) {
			newToken.content.append( GetString() );
			
			if ( newToken.content == "\"" ) {
				return false;
			} // if: no closing quote error
			
			newToken.tokenType = STRING;
		} // else if: string
		
		else if ( peekChar == '\'' ) {
			newToken.tokenType = QUOTE;
		} // else if: quote
		
		else {
			while ( cin.peek() != '\n' && cin.peek() != ' ' &&
							cin.peek() != '\t' && cin.peek() != '\'' &&
							cin.peek() != '"' && cin.peek() != '(' &&
							cin.peek() != ')' && cin.peek() != ';' &&
							cin.peek() != '\r' && cin.peek() != '\0' &&
							cin.peek() != EOF ) {
				newToken.content.push_back( cin.get() );
				g_CursorColumn++;
			} // while: get the rest of the token
			
			newToken.tokenType = CheckTokenType( newToken );
		} // else: other types
		
		m_LineOfTokens.push_back( newToken ); // push the newToken to the vector
		return true;
	} // HasNextToken()
	
	TokenType CheckTokenType( TokenStruct newToken ) {
		bool isNumber = false;
		bool isSymbol = false;
		int plusSignBitCount = 0;
		int minusSignBitCount = 0;
		int decimalPointCount = 0;
		
		for ( int i = 0 ; i < newToken.content.length() ; i++ ) {
			if ( newToken.content[i] == '+' ) {
				plusSignBitCount++;
			} // if: check the number of plus sign bit
			
			else if ( newToken.content[i] == '-' ) {
				minusSignBitCount++;
			} // else if: check the number of minus sign bit
			
			else if ( newToken.content[i] == '.' ) {
				decimalPointCount++;
			} // else if: check the number of decimal point count
			
			else if ( isdigit( newToken.content[i] ) ) {
				isNumber = true;
			} // else if: there are digits in the token
		} // for: go through the token
		
		if ( ( plusSignBitCount > 0 && minusSignBitCount > 0 ) ||
				 plusSignBitCount > 1 || minusSignBitCount > 1 ||
				 decimalPointCount > 1 ) {
			isNumber = false;
		} // if: more than one sign bit
		
		for ( int i = 0 ; i < newToken.content.length() ; i++ ) {
			if ( isupper( newToken.content[i] ) || islower( newToken.content[i] ) ) {
				isSymbol = true;
			} // if: there are alphabetic characters in the token
		} // for: go through the token
		
		if ( isNumber && NOT isSymbol ) {
			if ( IsFloat( newToken ) ) {
				return FLOAT;
			} // if: float
			
			else {
				return INT;
			} // else: int
		} // if: check if the number is float
		
		else {
			if ( newToken.content == "#f" || newToken.content == "nil" || newToken.content == "()" ) {
				return NIL;
			} // if: nil
			
			else if ( newToken.content == "t" || newToken.content == "#t" ) {
				return T;
			} // else if: #t
			
			else if ( newToken.content == "." ) {
				return DOT;
			} // else if: dot
			
			else if ( newToken.content == "quote" ) {
				return QUOTE;
			} // else if: quote
			
			else {
				return SYMBOL;
			} // else: symbols
		} // else: check t, nil or dot
	} // CheckTokenType()
	
	/*
		------------------- Check ------------------
		------------------- Syntax -----------------
	*/
	
	bool CheckSExp() {
		if ( IsAtom( m_LineOfTokens.back() ) ) {
			if ( m_Root != NULL ) {
				if ( m_CurrentTreeLocation->leftToken != NULL ) {
					FindValidNodePosition();
					
					if ( m_LineOfTokens.at( m_LineOfTokens.size() - 2 ).tokenType != DOT ) {
						RightCreateNode();
						LeftInsertToken();
					} // if: without dot case
					
					else {
						RightInsertToken();
					} // else: with case
				} // if: left is empty, insert left
				
				else {
					if ( m_CurrentTreeLocation->leftNode != NULL ) {
						FindValidNodePosition();
						
						if ( m_LineOfTokens.at( m_LineOfTokens.size() - 2 ).tokenType != DOT ) {
							RightCreateNode();
							LeftInsertToken();
						} // if: without dot case
						
						else {
							RightInsertToken();
						} // else: with dot case
					} // if : left node is not NULL
					
					else {
						LeftInsertToken();
					} // else: insert at left
				} // else: insert at the right
			} // if: check if there's any tree
			
			return true;
		} // if: <ATOM>
		
		else if ( m_LineOfTokens.back().tokenType == LEFT_PAREN ) {
			// initialize or create node
			if ( m_Root == NULL ) {
				InitializeRoot();
				m_CurrentTreeLocation->leftParenCreate = true;
			} // if: initialize the root pointer
			
			else {
				if ( m_CurrentTreeLocation->leftToken != NULL ) {
					FindValidNodePosition();
					RightCreateNode();
					
					if ( m_LineOfTokens.at( m_LineOfTokens.size() - 2 ).tokenType != DOT ) {
						LeftCreateNode();
					} // if: with out dot
				} // if: right create case
				
				else {
					if ( m_CurrentTreeLocation->leftNode != NULL ) {
						FindValidNodePosition();
						RightCreateNode();
						
						if ( m_LineOfTokens.at( m_LineOfTokens.size() - 2 ).tokenType != DOT ) {
							LeftCreateNode();
						} // if: with out dot
					} // if: insert right
					
					else {
						FindValidNodePosition();
						LeftCreateNode();
					} // else: insert right
				} // else: right insert node
				
				m_CurrentTreeLocation->leftParenCreate = true;
			} // else: create a node
			
			if ( HasNextToken() == false ) {
				return false;
			} // if: check if there is any token left
			
			// LEFT-PAREN <S-exp>
			if ( CheckSExp() == true ) {
				if ( HasNextToken() == false ) {
					return false;
				} // if: check if there is any token left
				
				// LEFT-PAREN <S-exp> { <S-exp> }
				while ( CheckSExp() == true ) {
					if ( HasNextToken() == false ) {
						return false;
					} // if: check if there is any token left
				} // while: { <S-exp> }
				
				if ( m_Error.errorType != NOT_S_EXP ) {
					return false;
				} // if: there's a syntax error in the while loop
				
				// LEFT-PAREN <S-exp> { <S-exp> } [ DOT ]
				if ( m_LineOfTokens.back().tokenType == DOT ) {
					if ( HasNextToken() == false ) {
						return false;
					} // if: check if there is any token left
					
					// LEFT-PAREN <S-exp> { <S-exp> } [ DOT <S-exp> ]
					if ( CheckSExp() == true ) {
						if ( HasNextToken() == false ) {
							return false;
						} // if: check if there is any token left
					} // if: <S-exp>
					
					else {
						if ( m_Error.errorType == NOT_S_EXP ) {
							SetError( UNEXPECTED_TOKEN_ATOM_LEFT_PAREN );
						} // if: overwrite error
						
						return false;
					} // else: syntax error
				} // if: [ DOT <S-exp> ]
				
				// LEFT-PAREN <S-exp> { <S-exp> } [ DOT <S-exp> ] RIGHT-PAREN
				if ( m_LineOfTokens.back().tokenType == RIGHT_PAREN ) {
					m_CurrentTreeLocation = m_CurrentTreeLocation->previousNode;
					return true;
				} // if: RIGHT-PAREN
				
				else {
					SetError( UNEXPECTED_RIGHT_PAREN );
					return false;
				} // else: syntax error
			} // if: <S-exp>
			
			else {
				if ( m_Error.errorType == NOT_S_EXP ) {
					SetError( UNEXPECTED_TOKEN_ATOM_LEFT_PAREN );
				} // if: overwrite error
				
				return false;
			} // else: syntax error
		} // else if: LEFT-PAREN
		
		else if ( m_LineOfTokens.back().tokenType == QUOTE ) {
			bool isQuoteWordCase = false;
			
			if ( m_LineOfTokens.back().content == "'" ) {
				TokenStruct quoteLeftParen;
				quoteLeftParen.tokenType = LEFT_PAREN;
				quoteLeftParen.content = "(";
				TokenStruct quoteString;
				quoteString.tokenType = QUOTE;
				quoteString.content = "quote";
				m_LineOfTokens.push_back( quoteLeftParen );
				m_LineOfTokens.push_back( quoteString );
				isQuoteWordCase = true;
				
				if ( m_Root == NULL ) {
					InitializeRoot();
					m_CurrentTreeLocation->leftParenCreate = true;
					LeftInsertToken();
				} // if: initialize the root pointer
				
				else {
					if ( m_CurrentTreeLocation->leftToken != NULL ) {
						FindValidNodePosition();
						RightCreateNode();
						
						if ( m_LineOfTokens.at( m_LineOfTokens.size() - 2 ).tokenType != DOT ) {
							LeftCreateNode();
						} // if: without dot
						
						m_CurrentTreeLocation->leftParenCreate = true;
						LeftInsertToken();
					} // if: already has left token
					
					else {
						if ( m_CurrentTreeLocation->leftNode != NULL ) {
							FindValidNodePosition();
							RightCreateNode();
							
							if ( m_LineOfTokens.at( m_LineOfTokens.size() - 2 ).tokenType != DOT ) {
								LeftCreateNode();
							} // if: without dot
							
							m_CurrentTreeLocation->leftParenCreate = true;
							LeftInsertToken();
						} // if: insert right
						
						else {
							FindValidNodePosition();
							LeftCreateNode();
							m_CurrentTreeLocation->leftParenCreate = true;
							LeftInsertToken();
						} // else: insert right
					} // else: right insert node
				} // else: create a node
			} // if: is quote symbol case
			
			else {
				m_CurrentTreeLocation->leftParenCreate = true;
				LeftInsertToken();
			} // else: quote word case
			
			if ( HasNextToken() == false ) {
				return false;
			} // if: check if there is any token left
			
			// QUOTE <S-exp>
			if ( CheckSExp() == true ) {
				if ( isQuoteWordCase ) {
					TokenStruct quoteRightParen;
					quoteRightParen.tokenType = RIGHT_PAREN;
					quoteRightParen.content = ")";
					m_LineOfTokens.push_back( quoteRightParen );
					m_CurrentTreeLocation = m_CurrentTreeLocation->previousNode;
				} // if: is quote symbol case
				
				return true;
			} // if: <S-exp>
			
			else {
				SetError( UNEXPECTED_TOKEN_ATOM_LEFT_PAREN );
				return false;
			} // else: syntax error
		} // else if: QUOTE
		
		SetError( NOT_S_EXP );
		return false;
	} // CheckSExp()
	
	/*
		------------------- Tree -------------------
		--------------- Constructing ---------------
	*/
	
	void InitializeRoot() {
		m_Root = new TreeStruct;
		m_Root->leftNode = NULL;
		m_Root->rightNode = NULL;
		m_Root->previousNode = NULL;
		m_Root->leftToken = NULL;
		m_Root->rightToken = NULL;
		m_CurrentTreeLocation = m_Root;
	} // InitializeRoot()
	
	void LeftCreateNode() {
		m_CurrentTreeLocation->leftNode = new TreeStruct;
		m_CurrentTreeLocation->leftNode->previousNode = m_CurrentTreeLocation;
		m_CurrentTreeLocation = m_CurrentTreeLocation->leftNode;
		m_CurrentTreeLocation->leftParenCreate = false;
		m_CurrentTreeLocation->leftNode = NULL;
		m_CurrentTreeLocation->rightNode = NULL;
		m_CurrentTreeLocation->leftToken = NULL;
		m_CurrentTreeLocation->rightToken = NULL;
	} // LeftCreateNode()
	
	void RightCreateNode() {
		m_CurrentTreeLocation->rightNode = new TreeStruct;
		m_CurrentTreeLocation->rightNode->previousNode = m_CurrentTreeLocation;
		m_CurrentTreeLocation = m_CurrentTreeLocation->rightNode;
		m_CurrentTreeLocation->leftParenCreate = false;
		m_CurrentTreeLocation->leftNode = NULL;
		m_CurrentTreeLocation->rightNode = NULL;
		m_CurrentTreeLocation->leftToken = NULL;
		m_CurrentTreeLocation->rightToken = NULL;
	} // RightCreateNode()
	
	void FindValidNodePosition() {
		if ( m_CurrentTreeLocation->previousNode ) {
			if ( m_CurrentTreeLocation->previousNode->leftToken ) {
				if ( m_CurrentTreeLocation->previousNode->leftToken->tokenType == QUOTE ) {
					m_CurrentTreeLocation = m_CurrentTreeLocation->previousNode;
				} // if: quote case
			} // if: previous->leftToken exist
		} // if: previousNode exist
		
		while ( m_CurrentTreeLocation->rightNode != NULL ) {
			m_CurrentTreeLocation = m_CurrentTreeLocation->previousNode;
		} // while: move currentTreeLocation to an available position
	} // FindValidNodePosition()
	
	void LeftInsertToken() {
		m_CurrentTreeLocation->leftToken = new TokenStruct;
		m_CurrentTreeLocation->leftToken->content = m_LineOfTokens.back().content;
		m_CurrentTreeLocation->leftToken->tokenType = m_LineOfTokens.back().tokenType;
	} // LeftInsertToken()
	
	void RightInsertToken() {
		m_CurrentTreeLocation->rightToken = new TokenStruct;
		m_CurrentTreeLocation->rightToken->content = m_LineOfTokens.back().content;
		m_CurrentTreeLocation->rightToken->tokenType = m_LineOfTokens.back().tokenType;
	} // RightInsertToken()
	
	/*
		------------------- Print ------------------
		------------------ Results -----------------
	*/
	
	bool NeedToPrint( int i, int leftParenCount ) {
		if ( m_LineOfTokens[i].tokenType == DOT ) {
			if ( m_LineOfTokens[i + 1].tokenType == LEFT_PAREN ) {
				return false;
			} // if: dot-leftParen case
			
			else if ( m_LineOfTokens[i + 1].tokenType == NIL ) {
				return false;
			} // else if: dot-nil
		} // if: current token is DOT
		
		else if ( m_LineOfTokens[i].tokenType == LEFT_PAREN ) {
			if ( i >= 1 ) {
				if ( m_LineOfTokens[i - 1].tokenType == DOT ) {
					return false;
				} // if: dot-leftParen case
			} // if: check index validility
		} // if: current token is left-paren
		
		else if ( m_LineOfTokens[i].tokenType == RIGHT_PAREN ) {
			if ( IsAtom( m_LineOfTokens[i - 1] ) ) {
				return true;
			} // if: printing after an atom
			
			else {
				if ( leftParenCount > 1 ) {
					return true;
				} // if: still have spaces for continious rightParen
				
				return false;
			} // else if: continious rightParen
		} // else if: currrent token is right-paren
		
		else if ( m_LineOfTokens[i].tokenType == NIL ) {
			if ( i >= 1 ) {
				if ( m_LineOfTokens[i - 1].tokenType == DOT &&
						 m_LineOfTokens[i + 1].tokenType == RIGHT_PAREN ) {
					return false;
				} // if: nil-rightParen case
			} // if: check index validility
		} // else if: current token is NIL
		
		else if ( m_LineOfTokens[i].content == "'" ) {
			return false;
		} // else if: ' quote case
		
		return true;
	} // NeedToPrint()
	
	void PrintIndentation( int leftParenCount ) {
		int spaces = leftParenCount*2;
		
		while ( spaces > 0 ) {
			cout << " ";
			spaces--;
		} // while: print spaces
	} // PrintIndentation()
	
	void PrintSExp() {
		int leftParenCount = 0;
		bool hasLineReturn = false;
		
		for ( int i = 0 ; i < m_LineOfTokens.size() ; i++ ) {
			if ( NeedToPrint( i, leftParenCount ) ) {
				if ( m_LineOfTokens[i].tokenType == RIGHT_PAREN ) {
					leftParenCount--;
				} // if: print right parenthesis, minus leftParenCount by one
				
				if ( hasLineReturn ) {
					PrintIndentation( leftParenCount );
				} // if: has a line return
				
				if ( IsAtom( m_LineOfTokens[i] ) ) {
					if ( m_LineOfTokens[i].tokenType == INT ) {
						cout << atoi( m_LineOfTokens[i].content.c_str() ) << endl;
					} // if: int case
					
					else if ( m_LineOfTokens[i].tokenType == FLOAT ) {
						cout << fixed << setprecision( 3 )
								 << round( atof( m_LineOfTokens[i].content.c_str() )*1000 )/1000
								 << endl;
					} // else if: float case with precision and round
					
					else if ( m_LineOfTokens[i].tokenType == NIL ) {
						cout << "nil" << endl;
					} // else if: nil
					
					else if ( m_LineOfTokens[i].tokenType == T ) {
						cout << "#t" << endl;
					} // else if: #t
					
					else if ( m_LineOfTokens[i].tokenType == STRING ) {
						PrintString( m_LineOfTokens[i].content );
					} // else if: string
					
					else if ( m_LineOfTokens[i].tokenType == SYMBOL ) {
						cout << m_LineOfTokens[i].content << endl;
					} // else if: symbol
					
					hasLineReturn = true;
				} // if: current token is an atom
				
				else if ( m_LineOfTokens[i].tokenType == LEFT_PAREN ) {
					leftParenCount++;
					hasLineReturn = false;
					cout << "( ";
				} // else if: left paren
				
				else if ( m_LineOfTokens[i].tokenType == RIGHT_PAREN ) {
					cout << ")" << endl;
					hasLineReturn = true;
				} // else if: right paren
				
				else if ( m_LineOfTokens[i].tokenType == DOT ) {
					cout << "." << endl;
					hasLineReturn = true;
				} // else if: dot
			} // if: token need to be printed
			
			if ( leftParenCount == 0 && m_LineOfTokens[i].tokenType != QUOTE ) {
				return;
			} // if: check print complete
			
			else if ( i == m_LineOfTokens.size() - 1 && leftParenCount > 0 ) {
				while ( leftParenCount > 0 ) {
					leftParenCount--;
					PrintIndentation( leftParenCount );
					cout << ")" << endl;
				} // while: loop print right-paren
			} // if: for loop is end but leftParenCount still greater than zero
		} // for: go through the vector
	} // PrintSExp()
	
	void PrintString( string stringContent ) {
		for ( int index = 0 ; index < stringContent.length() ; index++ ) {
			if ( stringContent[index] == '\\' ) {
				if ( stringContent[index + 1] == 'n' ) {
					cout << endl;
					index++;
				} // if: '\n'
				
				else if ( stringContent[index + 1] == 't' ) {
					cout << '\t';
					index++;
				} // else if: '\t'
				
				else if ( stringContent[index + 1] == '"' ) {
					cout << '"';
					index++;
				} // else if: '"'
				
				else if ( stringContent[index + 1] == '\\' ) {
					cout << '\\';
					index++;
				} // else if: '\\'
				
				else {
					cout << stringContent[index];
				} // else: normal '\\'
			} // if: escape
			
			else {
				cout << stringContent[index];
			} // else: normal character
		} // for: go through the string
		
		cout << endl;
	} // PrintString()
	
	/*
		------------------- Error ------------------
		------------------ Handling ----------------
	*/
	
	void SetError( ErrorType errorType ) {
		stringstream errorStream;
		
		if ( errorType == NO_CLOSING_QUOTE ) {
			errorStream << "ERROR (no closing quote) : "
									<< "END-OF-LINE encountered at "
									<< "Line " << g_CursorLine << " Column " << g_CursorColumn + 1;
			m_Error.errorType = NO_CLOSING_QUOTE;
			m_Error.errorMessage = errorStream.str();
		} // if: no closing quote
		
		else if ( errorType == UNEXPECTED_TOKEN_ATOM_LEFT_PAREN ) {
			errorStream << "ERROR (unexpected token) : "
									<< "atom or '(' expected when token at "
									<< "Line " << g_CursorLine
									<< " Column " << g_CursorColumn - ( m_LineOfTokens.back().content.size() - 1 )
									<< " is >>" << m_LineOfTokens.back().content << "<<";
			m_Error.errorType = UNEXPECTED_TOKEN_ATOM_LEFT_PAREN;
			m_Error.errorMessage = errorStream.str();
		} // else if: unexpected atom or left paren
		
		else if ( errorType == UNEXPECTED_RIGHT_PAREN ) {
			errorStream << "ERROR (unexpected token) : "
									<< "')' expected when token at "
									<< "Line " << g_CursorLine
									<< " Column " << g_CursorColumn - ( m_LineOfTokens.back().content.size() - 1 )
									<< " is >>" << m_LineOfTokens.back().content << "<<";
			m_Error.errorType = UNEXPECTED_RIGHT_PAREN;
			m_Error.errorMessage = errorStream.str();
		} // else if: unexpected right paren
		
		else if ( errorType == NO_MORE_INPUT ) {
			m_Error.errorType = NO_MORE_INPUT;
			m_Error.errorMessage = "ERROR (no more input) : END-OF-FILE encountered";
		} // else if: no more input
		
		else if ( errorType == NOT_S_EXP ) {
			errorStream << "ERROR (unexpected token) : "
									<< "atom or '(' expected when token at "
									<< "Line " << g_CursorLine
									<< " Column " << g_CursorColumn - ( m_LineOfTokens.back().content.size() - 1 )
									<< " is >>" << m_LineOfTokens.back().content << "<<";
			m_Error.errorType = NOT_S_EXP;
			m_Error.errorMessage = errorStream.str();
		} // else: none above
	} // SetError()
	
	void ErrorHandling() {
		if ( m_Error.errorType == NO_MORE_INPUT ) {
			cout << m_Error.errorMessage;
		} // if: eof
		
		else {
			cout << m_Error.errorMessage << endl;
			
			if ( m_Error.errorType != NO_CLOSING_QUOTE ) {
				char peekChar = cin.peek();
				
				while ( peekChar != '\n' ) {
					if ( cin.peek() == EOF ) {
						return;
					} // if: eof
					
					cin.get();
					peekChar = cin.peek();
				} // while: get the left overs
				
				cin.get();
			} // if: not no closing quote, get the leftovers
		} // else: not eof
	} // ErrorHandling()
	
	/*
		------------------- Clear ------------------
		-------------------- Line ------------------
	*/
	
	void ClearTheLine() {
		g_CursorLine = 1;
		g_CursorColumn = 0;
		
		char peekChar = cin.peek();
		
		while ( peekChar == ' ' ) {
			cin.get();
			g_CursorColumn++;
			peekChar = cin.peek();
		} // while: get space
		
		if ( peekChar == '\n' ) {
			cin.get();
			g_CursorLine = 1;
			g_CursorColumn = 0;
		} // if: get the useless line return
		
		else if ( peekChar == ';' ) {
			cin.get();
			peekChar = cin.peek();
			
			while ( peekChar != '\n' ) {
				if ( cin.peek() == EOF ) {
					return;
				} // if: eof
				
				cin.get();
				peekChar = cin.peek();
			} // while: get the rest off the comment
			
			cin.get();
			g_CursorLine = 1;
			g_CursorColumn = 0;
		} // else if: comment
		
		else {
			return;
		} // else: get valid character
	} // ClearTheLine()
}; // Project1Class

class Project2Class {
	Project1Class m_project1;
	vector<TreeStruct *> m_DefineBindingList;
	vector<ResultStruct *> m_ResultList;
	ErrorStruct m_Error;
	// TODO: add a vector that contains expression results

public:
	/*
		--------------------- Is -------------------
		------------------ Function ----------------
	*/
	
	bool IsPrimitive( TokenStruct *currentToken ) {
		if ( currentToken->content == "cons" || currentToken->content == "list" ) {
			currentToken->isPrimitive = true;
			currentToken->primitiveType = CONSTRUCTOR;
		} // if: constructor
		
		else if ( currentToken->content == "quote" || currentToken->content == "'" ) {
			currentToken->isPrimitive = true;
			currentToken->primitiveType = QUOTE_BYPASSING;
		} // else if: quote bypassing
		
		else if ( currentToken->content == "define" ) {
			currentToken->isPrimitive = true;
			currentToken->primitiveType = DEFINE_BINDING;
		} // else if: difine binding
		
		else if ( currentToken->content == "car" || currentToken->content == "cdr" ) {
			currentToken->isPrimitive = true;
			currentToken->primitiveType = PART_ACCESSOR;
		} // else if: part accessors
		
		else if ( currentToken->content == "atom?" || currentToken->content == "pair?" ||
							currentToken->content == "list?" || currentToken->content == "null?" ||
							currentToken->content == "integer?" || currentToken->content == "real?" ||
							currentToken->content == "number?" || currentToken->content == "string?" ||
							currentToken->content == "boolean?" || currentToken->content == "symbol?" ) {
			currentToken->isPrimitive = true;
			currentToken->primitiveType = PRIMITIVE_PREDICATE;
		} // else if: primitive predicate
		
		else if ( currentToken->content == "+" || currentToken->content == "-" ||
							currentToken->content == "*" || currentToken->content == "/" ||
							currentToken->content == "not" || currentToken->content == "and" ||
							currentToken->content == "or" || currentToken->content == ">" ||
							currentToken->content == ">=" || currentToken->content == "<" ||
							currentToken->content == "<=" || currentToken->content == "=" ||
							currentToken->content == "string-append" || currentToken->content == "string>?" ||
							currentToken->content == "string<?" || currentToken->content == "string=?" ) {
			currentToken->isPrimitive = true;
			currentToken->primitiveType = OPERATOR;
		} // else if: opertor
		
		else if ( currentToken->content == "eqv?" || currentToken->content == "equal?" ) {
			currentToken->isPrimitive = true;
			currentToken->primitiveType = EQUIVALENCE;
		} // else if: equivalence
		
		else if ( currentToken->content == "begin" ) {
			currentToken->isPrimitive = true;
			currentToken->primitiveType = BEGIN;
		} // else if: begin
		
		else if ( currentToken->content == "if" || currentToken->content == "cond" ) {
			currentToken->isPrimitive = true;
			currentToken->primitiveType = CONDITIONAL;
		} // else if: conditional
		
		else if ( currentToken->content == "clean-environment" ) {
			currentToken->isPrimitive = true;
			currentToken->primitiveType = CLEAN_ENVIRONMENT;
		} // else if: clean environment
		
		else {
			currentToken->isPrimitive = false;
		} // else: not primitive
		
		return currentToken->isPrimitive;
	} // IsFunction()
	
	/*
		-------------------- New -------------------
		----------------- Structures ---------------
	*/
	
	ResultStruct *NewResult() {
		ResultStruct *newResult = new ResultStruct;
		newResult->hasStringResult = false;
		newResult->hasBoolResult = false;
		newResult->hasIntResult = false;
		newResult->hasFloatResult = false;
		newResult->hasNodeResult = false;
		newResult->hasSymbolResult = false;
		newResult->stringResult = new TokenStruct;
		newResult->stringResult->tokenType = SYMBOL;
		newResult->boolResult = new TokenStruct;
		newResult->intResult = new TokenStruct;
		newResult->intResult->tokenType = INT;
		newResult->floatResult = new TokenStruct;
		newResult->floatResult->tokenType = FLOAT;
		newResult->nodeResult = new TreeStruct;
		newResult->symbolResult = new TokenStruct;
		newResult->symbolResult->tokenType = SYMBOL;
		// initialization
		newResult->nodeResult->leftNode = NULL;
		newResult->nodeResult->rightNode = NULL;
		newResult->nodeResult->previousNode = NULL;
		newResult->nodeResult->leftToken = NULL;
		newResult->nodeResult->rightToken = NULL;
		return newResult;
	} // NewResult()
	
	/*
		------------------- Error ------------------
		------------------ Handling ----------------
	*/
	
	void SetError( ErrorType errorType, string errorMessage ) {
		m_Error.errorType = errorType;
		m_Error.errorMessage = errorMessage;
	} // SetError()
	
	void ErrorHandling() {
		if ( g_uTestNum == 4 )
			cout << "Project 2 outputs:" << endl;
		
		cout << m_Error.errorMessage << endl;
	} // ErrorHandling()
	/*
		------------------- Start ------------------
		---------------- Evaluation ----------------
	*/
	
	bool EvalSExp( Project1Class project1Result ) {
		m_ResultList.clear();
		m_project1 = project1Result;
		
		if ( g_uTestNum == 4 )
			cout << endl << "------------- PROJECT 2 --------------" << endl;
		
		bool hasError = false;
		
		if ( ProcessSExp( m_project1.GetRoot(), hasError ) ) {
			if ( g_uTestNum == 4 )
				cout << "----------- DEBUG MESSAGES -----------" << endl << endl;
			return true;
		} // if: sucessfully process
		
		else {
			if ( g_uTestNum == 4 )
				cout << "----------- DEBUG MESSAGES -----------" << endl << endl;
			
			return false;
		} // else: error, handle it
	} // EvalSExp()
	
	bool ProcessSExp( TreeStruct *walk, bool &hasError ) {
		static bool underQuote = false;
		
		if ( walk ) {
			if ( walk->leftToken ) {
				if ( walk->leftToken->tokenType == QUOTE ) {
					underQuote = true;
				} // if: under quote function
				
				else {
					if ( IsPrimitive( walk->leftToken ) ) {
						underQuote = false;
					} // if: check if the leftToken is primitive, then reset underQuote
				} // else: reset underQuote
			} // if: has leftToken
			
			if ( walk->rightNode ) {
				ProcessSExp( walk->rightNode, hasError );
			} // if: has a right node, go right
			
			if ( walk->leftToken && hasError == false ) {
				if ( walk->leftParenCreate ) {
					if ( walk->leftToken->tokenType == SYMBOL || IsPrimitive( walk->leftToken ) ) {
						vector<TreeStruct *> argumentList;
						
						if ( IsPrimitive( walk->leftToken ) == false ) {
							if ( FindDefineBindings( true, walk->leftToken->content ) ) {
								walk->leftToken = GetDefineBindings( walk->leftToken->content )->leftToken;
								
								if ( IsPrimitive( walk->leftToken ) == false ) {
									string errorMessage = "ERROR (attempt to apply non-funciton) : " + walk->leftToken->content;
									SetError( APPLY_NON_FUNCITON, errorMessage );
									hasError = true;
								} // if: find pre-defined symbol, but not a function
							} // if: check if there's a bounded symbol
							
							else {
								if ( hasError == false ) {
									string errorMessage = "ERROR (unbound symbol) : " + walk->leftToken->content;
									SetError( DEFINE_UNBOUND, errorMessage );
									hasError = true;
								} // if: no previous error
							} // else: not find bounded symbol
						} // if: current token is a symbol
						
						if ( hasError == false ) {
							if ( CheckArguments( walk, argumentList ) ) {
								CallCorrespondingFunction( walk, argumentList );
							} // if: arguments has no problem
							
							else {
								hasError = true;
							} // else: arguments has error
						} // if: check the parameters
					} // if: if is primitive or symbol
					
					else {
						if ( underQuote == false ) {
							if ( hasError == false ) {
								string errorMessage = "ERROR (attempt to apply non-funciton) : " + walk->leftToken->content;
								SetError( APPLY_NON_FUNCITON, errorMessage );
								hasError = true;
							} // if: there's no previous error
						} // if: not under quote function
					} // else: left token but not function TODO: NOT AN FUNCTION ERROR
				} // if: this node is created by left-paren
			} // if: has a left token
			
			if ( walk->leftNode ) {
				ProcessSExp( walk->leftNode, hasError );
			} // if: has a left node, go left
		} // if: walk != NULL
		
		else {
			if ( FindDefineBindings( false, m_project1.GetSExp()[0].content ) == false ) {
				string errorMessage = "ERROR (unbound symbol) : " + m_project1.GetSExp()[0].content;
				SetError( DEFINE_UNBOUND, errorMessage );
				hasError = true;
			} // if: doesn't find a binding
		} // else: only inputs a word, check m_DefineBindingList TODO: hasn't implement defining quote
		
		return !hasError;
	} // ProcessSExp(): go through the data tree while using postorder traversal
	
	/*
		------------------- Define -----------------
		------------------ Bindings ----------------
	*/
	
	bool FindDefineBindings( bool duringCheckArguments, string currentWord ) {
		for ( int i = 0 ; i < m_DefineBindingList.size() ; i++ ) {
			if ( currentWord == m_DefineBindingList[i]->leftToken->content ) {
				if ( NOT duringCheckArguments ) {
					ResultStruct *result = NewResult();
					
					if ( m_DefineBindingList[i]->rightNode->leftNode ) {
						result->hasNodeResult = true;
						result->nodeResult = m_DefineBindingList[i]->rightNode->leftNode;
					} // else if: node
					
					else if ( m_DefineBindingList[i]->rightNode->leftToken->tokenType == STRING ) {
						result->hasStringResult = true;
						result->stringResult = m_DefineBindingList[i]->rightNode->leftToken;
					} // else if: symbol & string
					
					else if ( m_DefineBindingList[i]->rightNode->leftToken->tokenType == SYMBOL ) {
						result->hasSymbolResult = true;
						result->symbolResult = m_DefineBindingList[i]->rightNode->leftToken;
					} // else if: symbol & string
					
					else if ( m_DefineBindingList[i]->rightNode->leftToken->tokenType == FLOAT ) {
						result->hasFloatResult = true;
						result->floatResult = m_DefineBindingList[i]->rightNode->leftToken;
					} // else if: float
					
					else if ( m_DefineBindingList[i]->rightNode->leftToken->tokenType == INT ) {
						result->hasIntResult = true;
						result->intResult = m_DefineBindingList[i]->rightNode->leftToken;
					} // else if: int
					
					else if ( m_DefineBindingList[i]->rightNode->leftToken->tokenType == T ) {
						result->hasBoolResult = true;
						result->boolResult->tokenType = T;
					} // else if: T
					
					else if ( m_DefineBindingList[i]->rightNode->leftToken->tokenType == NIL ) {
						result->hasBoolResult = true;
						result->boolResult->tokenType = NIL;
					} // else if: NIL
					
					m_ResultList.push_back( result );
				} // if: not checking
				
				return true;
			} // if: find
		} // for: go through m_DefineBindingList
		
		return false;
	} // FindDefineBindings()
	
	TreeStruct *GetDefineBindings( string currentWord ) {
		for ( int i = 0 ; i < m_DefineBindingList.size() ; i++ ) {
			if ( currentWord == m_DefineBindingList[i]->leftToken->content ) {
				return m_DefineBindingList[i]->rightNode;
			} // if: find
		} // for: go through m_DefineBindingList
		
		return m_DefineBindingList.back()->rightNode;
	} // GetDefineBindings()
	
	/*
		----------------- Parameters ---------------
		----------------- Processing ---------------
	*/
	
	bool CheckArguments( TreeStruct *current, vector<TreeStruct *> &argumentList ) {
		if ( current->leftToken->primitiveType == CONSTRUCTOR ) {
			if ( current->leftToken->content == "cons" ) {
				if ( current->rightNode ) {
					if ( current->rightNode->leftToken ) {
						if ( current->rightNode->leftToken->tokenType == SYMBOL ) {
							if ( FindDefineBindings( true, current->rightNode->leftToken->content ) ) {
								argumentList.push_back( current->rightNode );
							} // if: find defineList
							
							else {
								string errorMessage = "ERROR (unbound symbol) : " + current->rightNode->leftToken->content;
								SetError( DEFINE_UNBOUND, errorMessage );
								return false;
							} // else if: find no pre-defined bindings for arguments
						} // if: current is symbol
							
							// else if ( current->rightNode->leftToken->tokenType == ) {
							// 	string errorMessage =
							// 					"ERROR (" + current->leftToken->content + " with incorrect argument type) : " +
							// 					current->rightNode->leftToken->content;
							// 	SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
							// 	return false;
							// } // else: false argument type
						
						else {
							argumentList.push_back( current->rightNode );
						} // else: current is not a symbol
					} // if: leftToken exist
					
					else {
						argumentList.push_back( current->rightNode );
					} // else: push rightNode in
					
					if ( current->rightNode->rightNode ) {
						if ( current->rightNode->rightNode->leftToken ) {
							if ( current->rightNode->rightNode->leftToken->tokenType == SYMBOL ) {
								if ( FindDefineBindings( true, current->rightNode->rightNode->leftToken->content ) ) {
									argumentList.push_back( current->rightNode->rightNode );
								} // if: find defineList
								
								else {
									string errorMessage =
													"ERROR (unbound symbol) : " + current->rightNode->rightNode->leftToken->content;
									SetError( DEFINE_UNBOUND, errorMessage );
									return false;
								} // else if: find no pre-defined bindings for arguments
							} // if: current is symbol
								
								// else if ( current->rightNode->leftToken->tokenType == ) {
								// 	string errorMessage =
								// 					"ERROR (" + current->leftToken->content + " with incorrect argument type) : " +
								// 					current->rightNode->leftToken->content;
								// 	SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
								// 	return false;
								// } // else: false argument type
							
							else {
								argumentList.push_back( current->rightNode->rightNode );
							} // else: not symbol, push into arguments
						} // if: current rightToken exist
						
						else {
							argumentList.push_back( current->rightNode->rightNode );
						} // else: not symbol
						
						if ( current->rightNode->rightNode->rightNode == NULL ) {
							return true;
						} // if: no third argument
					} // if: has two arguments
				} // if: has one argument
			} // if: cons
			
			else if ( current->leftToken->content == "list" ) {
				argumentList.push_back( current->rightNode );
				return true;
			} // else if: list
			
			string errorMessage = "ERROR (incorrect number of arguments) : " + current->leftToken->content;
			SetError( INCORRECT_NUM_ARGUMENTS, errorMessage );
			return false;
		} // if: constuctor
		
		else if ( current->leftToken->primitiveType == QUOTE_BYPASSING ) {
			if ( current->rightNode ) {
				argumentList.push_back( current->rightNode );
				
				if ( current->rightNode->rightNode == NULL ) {
					return true;
				} // if: no second argument
			} // if: has one argument
			
			return false;
		} // else if: quote
		
		else if ( current->leftToken->primitiveType == DEFINE_BINDING ) {
			if ( current->rightNode ) {
				argumentList.push_back( current->rightNode );
				
				if ( current->rightNode->rightNode ) {
					argumentList.push_back( current->rightNode->rightNode );
					
					if ( current->rightNode->rightNode->rightNode == NULL ) {
						return true;
					} // if: no third argument
				} // if: has two arguments
			} // if: has one argument
			
			return false;
		} // else if: define binding
		
		else if ( current->leftToken->primitiveType == PART_ACCESSOR ) {
			if ( current->rightNode ) {
				if ( current->rightNode->leftToken ) {
					if ( current->rightNode->leftToken->tokenType == SYMBOL ) {
						if ( FindDefineBindings( true, current->rightNode->leftToken->content ) ) {
							argumentList.push_back( current->rightNode );
						} // if: find bindings
						
						else {
							string errorMessage = "ERROR (unbound symbol) : " + current->rightNode->leftToken->content;
							SetError( DEFINE_UNBOUND, errorMessage );
							return false;
						} // else if: find no pre-defined bindings for arguments
						
						// else {
						// 	string errorMessage =
						// 					"ERROR (" + current->leftToken->content + " with incorrect argument type) : " +
						// 					current->rightNode->leftToken->content;
						// 	SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
						// 	return false;
						// } // else: false argument type
					} // if: current token is symbol
					
					else {
						argumentList.push_back( current->rightNode );
					} // else: current token not symbol
				} // if: leftToken exist
				
				else {
					argumentList.push_back( current->rightNode );
				} // else: push rightNode in
				
				if ( current->rightNode->rightNode == NULL ) {
					return true;
				} // if: no second argument
			} // if: has one arguments
			
			string errorMessage = "ERROR (incorrect number of arguments) : " + current->leftToken->content;
			SetError( INCORRECT_NUM_ARGUMENTS, errorMessage );
			return false;
		} // else if: part accessors
		
		else if ( current->leftToken->primitiveType == PRIMITIVE_PREDICATE ) {
			if ( current->rightNode ) {
				argumentList.push_back( current->rightNode );
				
				if ( current->rightNode->rightNode == NULL ) {
					return true;
				} // if: no second argument
			} // if: has one arguments
			
			return false;
		} // else if: primitive predicate
		
		else if ( current->leftToken->primitiveType == OPERATOR ) {
			if ( current->leftToken->content == "not" ) {
				if ( current->rightNode ) {
					argumentList.push_back( current->rightNode );
					
					if ( current->rightNode->rightNode == NULL ) {
						return true;
					} // if: no second argument
				} // if: hos one argument
			} // if: not
			
			else {
				if ( current->rightNode ) {
					argumentList.push_back( current->rightNode );
					
					if ( current->rightNode->rightNode ) {
						argumentList.push_back( current->rightNode->rightNode );
						return true;
					} // if: has greater than two arguments
				} // if: has one arguments
			} // else: other operators
		} // else if: operators
		
		else if ( current->leftToken->primitiveType == EQUIVALENCE ) {
			if ( current->rightNode ) {
				argumentList.push_back( current->rightNode );
				
				if ( current->rightNode->rightNode ) {
					argumentList.push_back( current->rightNode->rightNode );
					
					if ( current->rightNode->rightNode->rightNode == NULL ) {
						return true;
					} // if: no third argument
				} // if: has two arguments
			} // if: has one argument
			
			return false;
		} // else if: equivalence
		
		else if ( current->leftToken->primitiveType == BEGIN ) {
			if ( current->rightNode ) {
				argumentList.push_back( current->rightNode );
				return true;
			} // if: has more than one arguments
			
			return false;
		} // else if: begin
		
		else if ( current->leftToken->primitiveType == CONDITIONAL ) {
			if ( current->leftToken->content == "if" ) {
				if ( current->rightNode ) {
					argumentList.push_back( current->rightNode );
					
					if ( current->rightNode->rightNode ) {
						argumentList.push_back( current->rightNode->rightNode );
						
						if ( current->rightNode->rightNode->rightNode == NULL ) {
							return true;
						} // if: no third argument
						
						else if ( current->rightNode->rightNode->rightNode ) {
							argumentList.push_back( current->rightNode->rightNode->rightNode );
							
							if ( current->rightNode->rightNode->rightNode->rightNode == NULL ) {
								return true;
							} // if: no forth argument
						} // if: has three arguments
					} // if: has two arguments
				} // if: has one argument
			} // if: if
			
			else if ( current->leftToken->content == "cond" ) {
				if ( current->rightNode ) {
					argumentList.push_back( current->rightNode );
					return true;
				} // if: has more than one argument
			} // else if: cond
			
			return false;
		} // else if: conditional
		
		return false;
	} // CheckParameters()
	
	void PrintQuoteArguments( TreeStruct *quoteWalk ) {
		if ( quoteWalk ) {
			if ( quoteWalk->leftToken ) {
				cout << quoteWalk->leftToken->content << " ";
			} // if: has leftToken
			if ( quoteWalk->rightToken ) {
				cout << quoteWalk->rightToken->content << " ";
			} // if: has rightToken
			if ( quoteWalk->leftNode ) {
				PrintQuoteArguments( quoteWalk->leftNode );
			} // if: has leftNode
			if ( quoteWalk->rightNode ) {
				PrintQuoteArguments( quoteWalk->rightNode );
			} // if: has rightNode
		} // if: quoteWalk is not NULL
	} // PrintQuoteArguments()
	
	TokenType CheckResultType( ResultStruct *result ) {
		if ( result->hasNodeResult ) {
			return LEFT_PAREN;
		} // if: node
		
		else if ( result->hasStringResult ) {
			return STRING;
		} // if: string
		
		else if ( result->hasSymbolResult ) {
			return SYMBOL;
		} // else if: symbol
		
		else if ( result->hasIntResult ) {
			return INT;
		} // else if: int
		
		else if ( result->hasFloatResult ) {
			return FLOAT;
		} // else if: float
		
		else if ( result->hasBoolResult ) {
			if ( result->boolResult->tokenType == T ) {
				return T;
			} // if: #t
			
			else {
				return NIL;
			} // else: nil
		} // else if: boolean
		
		return NIL;
	} // CheckResultType()
	
	/*
	------------------ Function ----------------
	------------------- Entry ------------------
	*/
	
	void CallCorrespondingFunction( TreeStruct *functionNode, vector<TreeStruct *> arguments ) {
		// ------------------------------------------- DEBUG COUT ------------------------------------------------
		if ( g_uTestNum == 4 ) {
			cout << "<FUNCTION>: " << functionNode->leftToken->content;
			
			if ( functionNode->leftToken->content.length() < 4 ) {
				cout << "\t";
			} // if: just to pretty print debug message
			
			cout << "\t<ARGUMENTS>: ";
			
			for ( int i = 0 ; i < arguments.size() ; i++ ) {
				if ( functionNode->leftToken->primitiveType != QUOTE_BYPASSING ) {
					if ( arguments[i]->leftToken ) {
						cout << arguments[i]->leftToken->content << " ";
					} // if: simple arguments
					
					else {
						cout << arguments[i]->leftNode->leftToken->content << " ";
					} // else: quote case
				} // if: not quote case
				
				else {
					TreeStruct *quoteWalk = arguments[i]->leftNode;
					PrintQuoteArguments( quoteWalk );
				} // else: quote case
			} // for: print arguments
			
			cout << endl;
		} // if: add debug output
		// ------------------------------------------- DEBUG COUT --------------------------------------------- */
		
		TokenStruct *function = functionNode->leftToken;
		ResultStruct *result = NewResult();
		
		if ( function->primitiveType == CONSTRUCTOR ) {
			if ( function->content == "cons" ) {
				Cons( arguments, result );
			} // if: cons
			
			else {
				List( arguments, result );
			} // else if: list
		} // if: constructor
		
		else if ( function->primitiveType == QUOTE_BYPASSING ) {
			Quote( arguments, result );
		} // else if: quote
		
		else if ( function->primitiveType == DEFINE_BINDING ) {
			Define( arguments, result );
		} // else if: define
		
		else if ( function->primitiveType == PART_ACCESSOR ) {
			if ( function->content == "car" ) {
				Car( arguments, result );
			} // if: car
			
			else {
				// Cdr( arguments, result );
			} // else: cdr
		} // else if: part accessor
		
		else if ( function->primitiveType == PRIMITIVE_PREDICATE ) {
			if ( function->content == "atom?" ) {
				// IsAtom( arguments, result );
			} // if: atom?
			
			else if ( function->content == "pair?" ) {
				// IsPair( arguments, result );
			} // else if: pair?
			
			else if ( function->content == "list?" ) {
				// IsList( arguments, result );
			} // else if: list?
			
			else if ( function->content == "null?" ) {
				// IsNull( arguments, result );
			} // else if: null?
			
			else if ( function->content == "integer?" ) {
				// IsInteger( arguments, result );
			} // else if: integer?
			
			else if ( function->content == "real?" || function->content == "number?" ) {
				// IsRealOrNumber( arguments, result );
			} // else if: real? or number?
			
			else if ( function->content == "string?" ) {
				// IsString( arguments, result );
			} // else if: string?
			
			else if ( function->content == "boolean?" ) {
				// IsBoolean( arguments, result );
			} // else if: boolean?
			
			else if ( function->content == "symbol?" ) {
				// IsSymbol( arguments, result );
			} // else if: symbol?
		} // else if: primitive predicate
		
		else if ( function->primitiveType == OPERATOR ) {
			if ( function->content == "+" ) {
				// Addition( arguments, result );
			} // if: +
			
			else if ( function->content == "-" ) {
				// Subtraction( arguments, result );
			} // else if: -
			
			else if ( function->content == "*" ) {
				// Mutiplication( arguments, result );
			} // else if: *
			
			else if ( function->content == "/" ) {
				// Division( arguments, result );
			} // else if: /
			
			else if ( function->content == "not" ) {
				// Not( arguments, result );
			} // else if: not
			
			else if ( function->content == "and" ) {
				// And( arguments, result );
			} // else if: and
			
			else if ( function->content == "or" ) {
				// Or( arguments, result );
			} // else if: or
			
			else if ( function->content == ">" ) {
				// GreaterThan( arguments, result );
			} // else if: >
			
			else if ( function->content == ">=" ) {
				// GreaterThanOrEqualTo( arguments, result );
			} // else if: >=
			
			else if ( function->content == "<" ) {
				// LessThen( arguments, result );
			} // else if: <
			
			else if ( function->content == "<=" ) {
				// LessThenOrEaualTo( arguments, result );
			} // else if: <=
			
			else if ( function->content == "=" ) {
				// EqualTo( arguments, result );
			} // else if: =
			
			else if ( function->content == "string-append" ) {
				// StringAppend( arguments, result );
			} // else if: string-append
			
			else if ( function->content == "string>?" ) {
				// StringGreatenThan( arguments, result );
			} // else if: string>?
			
			else if ( function->content == "string<?" ) {
				// StringSmallerThan( arguments );
			} // else if: string<?
			
			else if ( function->content == "string=?" ) {
				// StringEqualTo( arguments, result );
			} // else if: string=?
		} // else if; operator
		
		else if ( function->primitiveType == EQUIVALENCE ) {
			if ( function->content == "eqv?" ) {
				// Eqv( arguments, result );
			} // if: eqv?
			
			else if ( function->content == "equal?" ) {
				// Equal( arguments, result );
			} // else if: equal?
		}// else if: equivalence
		
		else if ( function->primitiveType == BEGIN ) {
			// Begin( arguments, result );
		} // else if: begin
		
		else if ( function->primitiveType == CONDITIONAL ) {
			if ( function->content == "if" ) {
				// If( arguments, result );
			} // if: if
			
			else if ( function->content == "cond" ) {
				// Cond( arguments, result );
			} // else if: cond
		} // else if: conditional
		
		else if ( function->primitiveType == CLEAN_ENVIRONMENT ) {
			// CleanEnvironment();
		} // else if: clean-environment
	} // CallCorrespondingFunction()
	
	/*
	------------------ Function ----------------
	----------------- Definition ---------------
	*/
	
	void Cons( vector<TreeStruct *> arguments, ResultStruct *result ) {
		TreeStruct *resultRoot = new TreeStruct;
		// initialization
		resultRoot->leftNode = NULL;
		resultRoot->rightNode = NULL;
		resultRoot->previousNode = NULL;
		resultRoot->leftToken = NULL;
		resultRoot->rightToken = NULL;
		
		if ( arguments[0]->leftToken ) {
			if ( arguments[0]->leftToken->tokenType == SYMBOL ) {
				if ( GetDefineBindings( arguments[0]->leftToken->content )->leftNode ) {
					resultRoot->leftNode = GetDefineBindings( arguments[0]->leftToken->content )->leftNode;
				} // if: return a node
				
				else {
					resultRoot->leftToken = GetDefineBindings( arguments[0]->leftToken->content )->leftToken;
				} // else: return a token
			} // if: defined bindings
			
			else {
				resultRoot->leftToken = arguments[0]->leftToken;
			} // else: not defined simple data
		} // if: token form, get leftToken
		
		else {
			if ( CheckResultType( m_ResultList.back() ) == LEFT_PAREN ) {
				resultRoot->leftNode = m_ResultList.back()->nodeResult;
				m_ResultList.pop_back();
			} // if: previous result is node
			
			else if ( CheckResultType( m_ResultList.back() ) == INT ||
								CheckResultType( m_ResultList.back() ) == FLOAT ) {
				resultRoot->leftToken = m_ResultList.back()->intResult;
				m_ResultList.pop_back();
			} // else if: previous result is int
			
			else {
				string errorMessage = "ERROR (cons with incorrect argument type) : ";
				
				if ( m_ResultList.back()->hasStringResult ) {
					errorMessage += m_ResultList.back()->stringResult->content;
					SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
					return;
				} // if: previous result has wrong error type
			} // else: wrong result type
		} // else: get from previous result
		
		if ( m_ResultList.empty() ) {
			if ( arguments[1]->leftToken ) {
				if ( arguments[1]->leftToken->tokenType == SYMBOL ) {
					if ( GetDefineBindings( arguments[1]->leftToken->content )->leftNode ) {
						resultRoot->rightNode = GetDefineBindings( arguments[1]->leftToken->content )->leftNode;
					} // if: return a node
					
					else {
						resultRoot->rightToken = GetDefineBindings( arguments[1]->leftToken->content )->leftToken;
					} // else: return a token
				} // if: defined bindings
				
				else {
					resultRoot->rightToken = arguments[1]->leftToken;
				} // else: not defined binding
			} // if: token form
			
			else {
				resultRoot->rightNode = arguments[1]->leftNode;
			} // else: node form
		} // if: no previous result
		
		else {
			if ( CheckResultType( m_ResultList.back() ) == LEFT_PAREN ) {
				resultRoot->rightNode = m_ResultList.back()->nodeResult;
				m_ResultList.pop_back();
			} // if: previous result is node
			
			else if ( CheckResultType( m_ResultList.back() ) == INT ||
								CheckResultType( m_ResultList.back() ) == FLOAT ) {
				resultRoot->rightToken = m_ResultList.back()->intResult;
				m_ResultList.pop_back();
			} // else if: previous result is int
			
			else {
				string errorMessage = "ERROR (cons with incorrect argument type) : ";
				
				if ( m_ResultList.back()->hasStringResult ) {
					errorMessage += m_ResultList.back()->stringResult->content;
					SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
					return;
				} // if: previous result has wrong error type
			} // else: wrong result type
		} // else: has previous result
		
		result->hasNodeResult = true;
		result->nodeResult = resultRoot;
		m_ResultList.push_back( result );
	} // Cons()
	
	void List( vector<TreeStruct *> arguments, ResultStruct *result ) {
		TreeStruct *resultRoot = new TreeStruct;
		// initialization
		resultRoot->leftNode = NULL;
		resultRoot->rightNode = NULL;
		resultRoot->previousNode = NULL;
		resultRoot->leftToken = NULL;
		resultRoot->rightToken = NULL;
		
		TreeStruct *resultWalk = resultRoot;
		TreeStruct *listWalk = arguments[0];
		
		if ( listWalk->rightNode == NULL ) {
			if ( listWalk->leftToken ) {
				if ( listWalk->leftToken->tokenType == SYMBOL ) {
					if ( GetDefineBindings( listWalk->leftToken->content )->leftNode ) {
						resultWalk->leftNode = GetDefineBindings( listWalk->leftToken->content )->leftNode;
					} // if: return a node
					
					else {
						resultWalk->leftToken = GetDefineBindings( listWalk->leftToken->content )->leftToken;
					} // else: return a token
				} // if: defined bindings
				
				else {
					resultWalk->leftToken = listWalk->leftToken;
				} // else: not defined
			} // if: leftToken exist
			
			else {
				if ( CheckResultType( m_ResultList.back() ) == LEFT_PAREN ) {
					resultRoot->leftNode = m_ResultList.back()->nodeResult;
					m_ResultList.pop_back();
				} // if: previous result is node
				
				else if ( CheckResultType( m_ResultList.back() ) == INT ||
									CheckResultType( m_ResultList.back() ) == FLOAT ) {
					resultRoot->leftToken = m_ResultList.back()->intResult;
					m_ResultList.pop_back();
				} // else if: previous result is int
				
				else {
					string errorMessage = "ERROR (cons with incorrect argument type) : ";
					
					if ( m_ResultList.back()->hasStringResult ) {
						errorMessage += m_ResultList.back()->stringResult->content;
						SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
						return;
					} // if: previous result has wrong error type
				} // else: wrong result type
			} // else: get previous result
		} // if: only one data
		
		else {
			while ( listWalk->rightNode ) {
				if ( listWalk->leftToken ) {
					if ( listWalk->leftToken->tokenType == SYMBOL ) {
						if ( GetDefineBindings( listWalk->leftToken->content )->leftNode ) {
							resultWalk->leftNode = GetDefineBindings( listWalk->leftToken->content )->leftNode;
						} // if: return a node
						
						else {
							resultWalk->leftToken = GetDefineBindings( listWalk->leftToken->content )->leftToken;
						} // else: return a token
					} // if: defined bindings
					
					else {
						resultWalk->leftToken = listWalk->leftToken;
					} // else: not defined
				} // if: leftToken exist
				
				else {
					if ( CheckResultType( m_ResultList.back() ) == LEFT_PAREN ) {
						resultWalk->leftNode = m_ResultList.back()->nodeResult;
						m_ResultList.pop_back();
					} // if: previous result is node
					
					else if ( CheckResultType( m_ResultList.back() ) == INT ||
										CheckResultType( m_ResultList.back() ) == FLOAT ) {
						resultWalk->leftToken = m_ResultList.back()->intResult;
						m_ResultList.pop_back();
					} // else if: previous result is int
					
					else {
						string errorMessage = "ERROR (cons with incorrect argument type) : ";
						
						if ( m_ResultList.back()->hasStringResult ) {
							errorMessage += m_ResultList.back()->stringResult->content;
							SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
							return;
						} // if: previous result has wrong error type
					} // else: wrong result type
				} // else: get previous result
				
				listWalk = listWalk->rightNode;
				resultWalk->rightNode = new TreeStruct;
				resultWalk->rightNode->previousNode = resultWalk;
				resultWalk = resultWalk->rightNode;
				// initialization
				resultWalk->leftNode = NULL;
				resultWalk->rightNode = NULL;
				resultWalk->leftToken = NULL;
				resultWalk->rightToken = NULL;
				
				if ( listWalk->rightNode == NULL ) {
					if ( listWalk->leftToken ) {
						if ( listWalk->leftToken->tokenType == SYMBOL ) {
							if ( GetDefineBindings( listWalk->leftToken->content )->leftNode ) {
								resultWalk->leftNode = GetDefineBindings( listWalk->leftToken->content )->leftNode;
							} // if: return a node
							
							else {
								resultWalk->leftToken = GetDefineBindings( listWalk->leftToken->content )->leftToken;
							} // else: return a token
						} // if: defined bindings
						
						else {
							resultWalk->leftToken = listWalk->leftToken;
						} // else: not defined
					} // if: leftToken exist
					
					else {
						if ( CheckResultType( m_ResultList.back() ) == LEFT_PAREN ) {
							resultWalk->leftNode = m_ResultList.back()->nodeResult;
							m_ResultList.pop_back();
						} // if: previous result is node
						
						else if ( CheckResultType( m_ResultList.back() ) == INT ||
											CheckResultType( m_ResultList.back() ) == FLOAT ) {
							resultWalk->leftToken = m_ResultList.back()->intResult;
							m_ResultList.pop_back();
						} // else if: previous result is int
						
						else {
							string errorMessage = "ERROR (cons with incorrect argument type) : ";
							
							if ( m_ResultList.back()->hasStringResult ) {
								errorMessage += m_ResultList.back()->stringResult->content;
								SetError( INCORRECT_ARGUMENT_TYPE, errorMessage );
								return;
							} // if: previous result has wrong error type
						} // else: wrong result type
					} // else: get previous result
				} // if: last node
			} // while: go through all the nodes on the right side
		} // else: more than one data
		
		result->hasNodeResult = true;
		result->nodeResult = resultRoot;
		m_ResultList.push_back( result );
	} // List()
	
	void Quote( vector<TreeStruct *> arguments, ResultStruct *result ) {
		result->nodeResult = arguments.front()->leftNode;
		result->hasNodeResult = true;
		m_ResultList.push_back( result );
	} // Quote()
	
	void Define( vector<TreeStruct *> arguments, ResultStruct *result ) {
		result->stringResult->content = arguments.front()->leftToken->content + " defined";
		result->hasStringResult = true;
		
		for ( int i = 0 ; i < m_DefineBindingList.size() ; i++ ) {
			if ( arguments.front()->leftToken->content == m_DefineBindingList[i]->leftToken->content ) {
				m_DefineBindingList.erase( m_DefineBindingList.begin() + i );
			} // if: find pre-defined , delete it
		} // for: find any pre-defined
		
		if ( arguments.back()->leftNode ) {
			arguments.back()->leftNode = m_ResultList.back()->nodeResult;
			m_ResultList.pop_back();
		} // if: define a function value
		
		m_DefineBindingList.push_back( arguments.front() );
		m_ResultList.push_back( result );
	} // Define()
	
	void Car( vector<TreeStruct *> arguments, ResultStruct *result ) {
	
	} // Car()
	/*
	------------------- Print ------------------
	------------------ Result ------------------
	*/
	
	void PrintResult() {
		if ( g_uTestNum == 4 )
			cout << "Project 2 outputs:" << endl;
		
		if ( m_ResultList.back()->hasStringResult ) {
			PrintString( m_ResultList.back()->stringResult->content );
		} // if: string form result
		
		else if ( m_ResultList.back()->hasSymbolResult ) {
			cout << m_ResultList.back()->symbolResult->content;
		} // else if: symbol
		
		else if ( m_ResultList.back()->hasBoolResult ) {
			if ( m_ResultList.back()->boolResult->tokenType == T ) {
				cout << "#t" << endl;
			} // if: boolean result is true, #t
			
			else if ( m_ResultList.back()->boolResult->tokenType == NIL ) {
				cout << "nil" << endl;
			} // else if: boolean result is false, nil
		} // else if: boolean form result
		
		else if ( m_ResultList.back()->hasFloatResult ) {
			cout << fixed << setprecision( 3 )
					 << round( atof( m_ResultList.back()->floatResult->content.c_str() )*1000 )/1000
					 << endl;
		} // else if: float form result
		
		else if ( m_ResultList.back()->hasIntResult ) {
			cout << m_ResultList.back()->intResult->content << endl;
		} // else if: int form result
		
		else if ( m_ResultList.back()->hasNodeResult ) {
			int layer = 0;
			bool isRightNode = false;
			PrintTree( m_ResultList.back()->nodeResult, layer, isRightNode );
			
			if ( layer > 0 ) {
				while ( layer > 0 ) {
					layer--;
					cout << string( layer, ' ' ) << ')' << endl;
				} // while: loop print right-paren
			} // if: layer still greater than zero
		} // else if: tree structre form result
	} // PrintResult()
	
	void PrintTree( TreeStruct *currentRoot, int &layer, bool isRightNode ) {
		static bool hasLineReturn = false;
		
		if ( isRightNode == false ) {
			if ( hasLineReturn ) {
				cout << string( layer*2, ' ' );
			} // if: has lineReturn
			
			cout << "( ";
			hasLineReturn = false;
			layer++;
		} // if: currentNode is the leftNode of the previousNode
		
		if ( currentRoot->leftNode ) {
			isRightNode = false;
			PrintTree( currentRoot->leftNode, layer, isRightNode );
		} // if: has leftNode
		
		if ( currentRoot->leftToken ) {
			if ( hasLineReturn ) {
				cout << string( layer*2, ' ' );
			} // if: has line return
			
			PrintData( currentRoot->leftToken );
			isRightNode = false;
			hasLineReturn = true;
			
			if ( currentRoot->rightToken && currentRoot->rightToken->tokenType != NIL ) {
				cout << string( layer*2, ' ' ) << '.' << endl << string( layer*2, ' ' );
				PrintData( currentRoot->rightToken );
			} // if: has data in rightToken
		} // if: has data in leftToken
		
		else if ( currentRoot->rightToken ) {
			cout << string( layer*2, ' ' ) << '.' << endl << string( layer*2, ' ' );
			PrintData( currentRoot->rightToken );
		} // else if: has rightToken
		
		if ( currentRoot->rightNode ) {
			isRightNode = true;
			PrintTree( currentRoot->rightNode, layer, isRightNode );
		} // if: has isRightNode
		
		if ( layer > 1 ) {
			layer--;
			cout << string( layer*2, ' ' ) << ')' << endl;
		} // if: still space print right paren
	} // PrintTree()
	
	void PrintData( TokenStruct *currentToken ) {
		if ( currentToken->tokenType == INT ) {
			cout << atoi( currentToken->content.c_str() ) << endl;
		} // if: int case
		
		else if ( currentToken->tokenType == FLOAT ) {
			cout << fixed << setprecision( 3 )
					 << round( atof( currentToken->content.c_str() )*1000 )/1000
					 << endl;
		} // else if: float case with precision and round
		
		else if ( currentToken->tokenType == NIL ) {
			cout << "nil" << endl;
		} // else if: nil
		
		else if ( currentToken->tokenType == T ) {
			cout << "#t" << endl;
		} // else if: #t
		
		else if ( currentToken->tokenType == STRING ) {
			PrintString( currentToken->content );
		} // else if: string
		
		else if ( currentToken->tokenType == SYMBOL ) {
			cout << currentToken->content << endl;
		} // else if: symbol
	} // PrintData()
	
	void PrintString( string stringContent ) {
		for ( int index = 0 ; index < stringContent.length() ; index++ ) {
			if ( stringContent[index] == '\\' ) {
				if ( stringContent[index + 1] == 'n' ) {
					cout << endl;
					index++;
				} // if: '\n'
				
				else if ( stringContent[index + 1] == 't' ) {
					cout << '\t';
					index++;
				} // else if: '\t'
				
				else if ( stringContent[index + 1] == '"' ) {
					cout << '"';
					index++;
				} // else if: '"'
				
				else if ( stringContent[index + 1] == '\\' ) {
					cout << '\\';
					index++;
				} // else if: '\\'
				
				else {
					cout << stringContent[index];
				} // else: normal '\\'
			} // if: escape
			
			else {
				cout << stringContent[index];
			} // else: normal character
		} // for: go through the string
		
		cout << endl;
	} // PrintString()
}; // Project2Class

int main() {
	cout << endl << "Enter 1 for standard outputs, 4 for debugging informations." << endl;
	cin >> g_uTestNum;
	bool end = false;
	cout << "Welcome to OurScheme!" << endl << endl;
	
	if ( g_uTestNum == 5 )
		cout << "hehe";
	
	else {
		Project1Class project1;
		Project2Class project2;
		
		do {
			cout << "> ";
			
			if ( project1.ReadSExp() == true ) {
				if ( project1.CheckExit() == true ) {
					end = true;
				} // if: check exit
				
				else {
					if ( project2.EvalSExp( project1 ) == true ) {
						if ( g_uTestNum == 4 ) {
							cout << endl << "Project 1 outputs:" << endl;
							project1.PrintSExp();
							cout << endl;
						} // if: debug
						
						project2.PrintResult();
					} // if: evaluate correct
					
					else {
						project2.ErrorHandling();
					} // else: evaluation error
					project1.ClearTheLine();
				} // else: not exit case
			} // if: valid syntax
			
			else {
				project1.ErrorHandling();
				g_CursorLine = 1;
				g_CursorColumn = 0;
				
				if ( project1.CheckExit() == true ) {
					end = true;
				} // if: check exit
			} // else: syntax error
			
			cout << endl;
		} while ( NOT end );
	} // else
	
	cout << "Thanks for using OurScheme!" << endl << endl;
} // main(): main function
