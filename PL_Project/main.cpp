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
		NOT_S_EXP, NO_CLOSING_QUOTE, UNEXPECTED_TOKEN_ATOM_LEFT_PAREN, UNEXPECTED_RIGHT_PAREN, NO_MORE_INPUT
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
		string stringResult;
		bool hasStringResult;
		bool boolResult;
		bool hasBoolResult;
		int intResult;
		bool hasIntResult;
		float floatResult;
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
	
	void CreateNode() {
		if ( m_CurrentTreeLocation->leftToken == NULL &&
				 m_CurrentTreeLocation->leftNode == NULL ) {
			m_CurrentTreeLocation->leftNode = new TreeStruct;
			m_CurrentTreeLocation->leftNode->previousNode = m_CurrentTreeLocation;
			m_CurrentTreeLocation = m_CurrentTreeLocation->leftNode;
		} // if: check left or right
		
		else {
			if ( IsAtom( m_LineOfTokens.at( m_LineOfTokens.size() - 2 ) ) ) {
				m_CurrentTreeLocation->rightNode = new TreeStruct;
				m_CurrentTreeLocation->rightNode->previousNode = m_CurrentTreeLocation;
				m_CurrentTreeLocation = m_CurrentTreeLocation->rightNode;
				m_CurrentTreeLocation->leftNode = new TreeStruct;
				m_CurrentTreeLocation->leftNode->previousNode = m_CurrentTreeLocation;
				m_CurrentTreeLocation = m_CurrentTreeLocation->leftNode;
			} // if: without dot case
			
			else {
				m_CurrentTreeLocation->rightNode = new TreeStruct;
				m_CurrentTreeLocation->rightNode->previousNode = m_CurrentTreeLocation;
				m_CurrentTreeLocation = m_CurrentTreeLocation->rightNode;
			} // else: normal dot case
		} // else: create at right
		
		m_CurrentTreeLocation->leftNode = NULL;
		m_CurrentTreeLocation->rightNode = NULL;
		m_CurrentTreeLocation->leftToken = NULL;
		m_CurrentTreeLocation->rightToken = NULL;
	} // CreateNode()
	
	void LeftCreateNode() {
		m_CurrentTreeLocation->leftNode = new TreeStruct;
		m_CurrentTreeLocation->leftNode->previousNode = m_CurrentTreeLocation;
		m_CurrentTreeLocation = m_CurrentTreeLocation->leftNode;
		m_CurrentTreeLocation->leftNode = NULL;
		m_CurrentTreeLocation->rightNode = NULL;
		m_CurrentTreeLocation->leftToken = NULL;
		m_CurrentTreeLocation->rightToken = NULL;
	} // LeftCreateNode()
	
	void RightCreateNode() {
		m_CurrentTreeLocation->rightNode = new TreeStruct;
		m_CurrentTreeLocation->rightNode->previousNode = m_CurrentTreeLocation;
		m_CurrentTreeLocation = m_CurrentTreeLocation->rightNode;
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
	vector<TreeStruct *> defineBindingList;
	vector<ResultStruct> resultList;
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
		---------------- Preprocessing -------------
		-------------------- Tree ------------------
	*/
	
	/*
		------------------- Start ------------------
		---------------- Evaluation ----------------
	*/
	
	bool EvalSExp( Project1Class project1Result ) {
		resultList.clear();
		m_project1 = project1Result;
		cout << "Project 2 outputs:" << endl;
		
		if ( ProcessSExp( m_project1.GetRoot() ) ) {
			PrintResult();
		} // if: sucessfully process
		
		else {
			// ErrorHandling();
		} // else: error, handle it
		return true;
	} // EvalSExp()
	
	bool ProcessSExp( TreeStruct *walk ) {
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
			
			if ( walk->leftNode ) {
				ProcessSExp( walk->leftNode );
			} // if: has a left node, go left
			
			if ( walk->rightNode ) {
				ProcessSExp( walk->rightNode );
			} // if: has a right node, go right
			
			if ( walk->leftToken ) {
				if ( walk->leftParenCreate ) {
					if ( IsPrimitive( walk->leftToken ) ) {
						vector<TreeStruct *> argumentList;
						
						if ( CheckArguments( walk, argumentList ) ) {
							CallCorrespondingFunction( walk, argumentList );
						} // if: check the parameters
						
						else {
							cout << "<ERROR>: incorrect number of arguments." << endl;
							return false;
						} // else if: arguments error TODO: INCORRECT NUMBER OF ARGUMENTS ERROR
					} // if: if is primitive
					
					else {
						if ( underQuote == false ) {
							cout << "<ERROR>: " << walk->leftToken->content
									 << " is created by left-paren, but not a function." << endl;
							return false;
						} // if: not under quote function
					} // else: left token but not function TODO: NOT AN FUNCTION ERROR
				} // if: this node is created by left-paren
			} // if: has a left token
		} // if: walk != NULL
		
		else {
			bool findDefineBinding = false;
			TokenStruct currentWord = m_project1.GetSExp()[0];
			
			for ( int i = 0 ; i < defineBindingList.size() ; i++ ) {
				if ( currentWord.content == defineBindingList[i]->leftToken->content ) {
					cout << defineBindingList[i]->rightNode->leftToken->content << endl;
					findDefineBinding = true;
				} // if: find
			} // for: go through defineBindingList
			
			if ( findDefineBinding == false ) {
				cout << "<ERROR>: " << currentWord.content << " is not a defined term." << endl;
			} // if: doesn't find a binding
			
			return false;
		} // else: only inputs a word, check defineBindingList TODO: hasn't implement defining quote
		
		return true;
	} // ProcessSExp(): go through the data tree while using postorder traversal
	
	/*
		----------------- Parameters ---------------
		----------------- Processing ---------------
	*/
	
	bool CheckArguments( TreeStruct *current, vector<TreeStruct *> &argumentList ) {
		if ( current->leftToken->primitiveType == CONSTRUCTOR ) {
			if ( current->leftToken->content == "cons" ) {
				if ( current->rightNode ) {
					argumentList.push_back( current->rightNode );
					
					if ( current->rightNode->rightNode ) {
						argumentList.push_back( current->rightNode->rightNode );
						
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
				argumentList.push_back( current->rightNode );
				
				if ( current->rightNode->rightNode == NULL ) {
					return true;
				} // if: no second argument
			} // if: has one arguments
			
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
	
	/*
	------------------ Function ----------------
	------------------- Entry ------------------
	*/
	
	void CallCorrespondingFunction( TreeStruct *functionNode, vector<TreeStruct *> arguments ) {
		// ------------------------------------------- DEBUG COUT ------------------------------------------------
		if ( g_uTestNum == 4 ) {
			cout << "<FUNCTION>: " << functionNode->leftToken->content << "\t<ARGUMENTS>: ";
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
					
					while ( quoteWalk->rightNode ) {
						cout << quoteWalk->leftToken->content << " ";
						quoteWalk = quoteWalk->rightNode;
						
						if ( quoteWalk->rightNode == NULL ) {
							if ( quoteWalk->leftToken ) {
								cout << quoteWalk->leftToken->content << " ";
							} // if: last node leftToken
							
							if ( quoteWalk->rightToken ) {
								cout << quoteWalk->rightToken->content << " ";
							} // if: last node rightToken
						} // if: last
					} // while: go through the quote
				} // else: quote case
			} // for: print arguments
			
			cout << endl;
		} // if: add debug output
		// ------------------------------------------- DEBUG COUT ------------------------------------------------
		
		TokenStruct *function = functionNode->leftToken;
		ResultStruct result;
		
		if ( function->primitiveType == CONSTRUCTOR ) {
			if ( function->content == "cons" ) {
				// Cons( arguments );
			} // if: cons
			
			else {
				// List( arguments );
			} // else if: list
		} // if: constructor
		
		else if ( function->primitiveType == QUOTE_BYPASSING ) {
			// Quote( argumnets );
		} // else if: quote
		
		else if ( function->primitiveType == DEFINE_BINDING ) {
			Define( arguments, result );
		} // else if: define
		
		else if ( function->primitiveType == PART_ACCESSOR ) {
			if ( function->content == "car" ) {
				// Car( arguments );
			} // if: car
			
			else {
				// Cdr( arguments );
			} // else: cdr
		} // else if: part accessor
		
		else if ( function->primitiveType == PRIMITIVE_PREDICATE ) {
			if ( function->content == "atom?" ) {
				// IsAtom( arguments );
			} // if: atom?
			
			else if ( function->content == "pair?" ) {
				// IsPair( arguments );
			} // else if: pair?
			
			else if ( function->content == "list?" ) {
				// IsList( arguments );
			} // else if: list?
			
			else if ( function->content == "null?" ) {
				// IsNull( arguments );
			} // else if: null?
			
			else if ( function->content == "integer?" ) {
				// IsInteger( arguments );
			} // else if: integer?
			
			else if ( function->content == "real?" || function->content == "number?" ) {
				// IsRealOrNumber( arguments );
			} // else if: real? or number?
			
			else if ( function->content == "string?" ) {
				// IsString( arguments );
			} // else if: string?
			
			else if ( function->content == "boolean?" ) {
				// IsBoolean( arguments );
			} // else if: boolean?
			
			else if ( function->content == "symbol?" ) {
				// IsSymbol( arguments );
			} // else if: symbol?
		} // else if: primitive predicate
		
		else if ( function->primitiveType == OPERATOR ) {
			if ( function->content == "+" ) {
				// Addition( arguments );
			} // if: +
			
			else if ( function->content == "-" ) {
				// Subtraction( arguments );
			} // else if: -
			
			else if ( function->content == "*" ) {
				// Mutiplication( arguments );
			} // else if: *
			
			else if ( function->content == "/" ) {
				// Division( arguments );
			} // else if: /
			
			else if ( function->content == "not" ) {
				// Not( arguments );
			} // else if: not
			
			else if ( function->content == "and" ) {
				// And( arguments );
			} // else if: and
			
			else if ( function->content == "or" ) {
				// Or( arguments );
			} // else if: or
			
			else if ( function->content == ">" ) {
				// GreaterThan( arguments );
			} // else if: >
			
			else if ( function->content == ">=" ) {
				// GreaterThanOrEqualTo( arguments );
			} // else if: >=
			
			else if ( function->content == "<" ) {
				// LessThen( arguments );
			} // else if: <
			
			else if ( function->content == "<=" ) {
				// LessThenOrEaualTo( arguments );
			} // else if: <=
			
			else if ( function->content == "=" ) {
				// EqualTo( arguments );
			} // else if: =
			
			else if ( function->content == "string-append" ) {
				// StringAppend( arguments );
			} // else if: string-append
			
			else if ( function->content == "string>?" ) {
				// StringGreatenThan( arguments );
			} // else if: string>?
			
			else if ( function->content == "string<?" ) {
				// StringSmallerThan( arguments );
			} // else if: string<?
			
			else if ( function->content == "string=?" ) {
				// StringEqualTo( arguments );
			} // else if: string=?
		} // else if; operator
		
		else if ( function->primitiveType == EQUIVALENCE ) {
			if ( function->content == "eqv?" ) {
				// Eqv( arguments );
			} // if: eqv?
			
			else if ( function->content == "equal?" ) {
				// Equal( arguments );
			} // else if: equal?
		} // else if: equivalence
		
		else if ( function->primitiveType == BEGIN ) {
			// Begin( arguments );
		} // else if: begin
		
		else if ( function->primitiveType == CONDITIONAL ) {
			if ( function->content == "if" ) {
				// If( arguments );
			} // if: if
			
			else if ( function->content == "cond" ) {
				// Cond( arguments );
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
	
	void Cons( vector<TreeStruct *> arguments ) {
		;
	} // Cons()
	
	void List( vector<TreeStruct *> arguments ) {
		;
	} // List()
	
	void Quote( vector<TreeStruct *> arguments ) {
		;
	} // Quote()
	
	void Define( vector<TreeStruct *> arguments, ResultStruct result ) {
		result.stringResult = arguments.front()->leftToken->content + " defined";
		result.hasStringResult = true;
		defineBindingList.push_back( arguments.front() );
		resultList.push_back( result );
	} // Define()
	
	/*
	------------------- Print ------------------
	------------------ Result ------------------
	*/
	
	void PrintResult() {
		if ( resultList.front().hasStringResult ) {
			cout << resultList.front().stringResult << endl;
		} // if: string form result
		
		else if ( resultList.front().hasBoolResult ) {
			if ( resultList.front().boolResult == true ) {
				cout << "#t" << endl;
			} // if: boolean result is true, #t
			
			else if ( resultList.front().boolResult == false ) {
				cout << "nil" << endl;
			} // else if: boolean result is false, nil
		} // else if: boolean form result
		
		else if ( resultList.front().hasFloatResult ) {
			cout << fixed << setprecision( 3 )
					 << round( resultList.front().floatResult*1000 )/1000
					 << endl;
		} // else if: float form result
		
		else if ( resultList.front().hasIntResult ) {
			cout << resultList.front().intResult << endl;
		} // else if: int form result
		
		else if ( resultList.front().hasNodeResult ) {
			// PrintTree();
		} // else if: tree structre form result
	} // PrintResult()
}; // Project2Class

int main() {
	cout << "Enter 1 for standard outputs, 4 for debugging informations." << endl;
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
						cout << endl << "Project 1 outputs:" << endl;
						project1.PrintSExp();
					} // if: evaluate correct
					
					// else {
					//   project2.PrintEvaluationError();
					// } // else: evaluation error
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
