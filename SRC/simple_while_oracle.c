#include "ARVADA.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Token types
typedef enum {
    TOK_WHILE,
    TOK_IF,
    TOK_ELSE,
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_LBRACE,
    TOK_RBRACE,
    TOK_SEMICOLON,
    TOK_ASSIGN,
    TOK_EQ,
    TOK_NE,
    TOK_LT,
    TOK_GT,
    TOK_LE,
    TOK_GE,
    TOK_PLUS,
    TOK_MINUS,
    TOK_MULT,
    TOK_DIV,
    TOK_NUMBER,
    TOK_IDENTIFIER,
    TOK_EOF,
    TOK_ERROR
} TokenType;

typedef struct {
    TokenType type;
    char value[256];
    int pos;
} Token;

// Global variables for parsing
char* input;
int pos;
int len;
Token current_token;

// Function declarations
void advance();
Token get_next_token();
int parse_program();
int parse_statement();
int parse_while_statement();
int parse_if_statement();
int parse_assignment();
int parse_expression();
int parse_term();
int parse_factor();
int parse_condition();
void skip_whitespace();
int is_keyword(char* str);

// Skip whitespace
void skip_whitespace() {
    while (pos < len && isspace(input[pos])) {
        pos++;
    }
}

// Check if string is a keyword
int is_keyword(char* str) {
    return (strcmp(str, "while") == 0 ||
            strcmp(str, "if") == 0 ||
            strcmp(str, "else") == 0);
}

// Get next token
Token get_next_token() {
    Token token;
    token.pos = pos;

    skip_whitespace();

    if (pos >= len) {
        token.type = TOK_EOF;
        strcpy(token.value, "EOF");
        return token;
    }

    char ch = input[pos];

    // Single character tokens
    switch (ch) {
        case '(':
            token.type = TOK_LPAREN;
            strcpy(token.value, "(");
            pos++;
            return token;
        case ')':
            token.type = TOK_RPAREN;
            strcpy(token.value, ")");
            pos++;
            return token;
        case '{':
            token.type = TOK_LBRACE;
            strcpy(token.value, "{");
            pos++;
            return token;
        case '}':
            token.type = TOK_RBRACE;
            strcpy(token.value, "}");
            pos++;
            return token;
        case ';':
            token.type = TOK_SEMICOLON;
            strcpy(token.value, ";");
            pos++;
            return token;
        case '+':
            token.type = TOK_PLUS;
            strcpy(token.value, "+");
            pos++;
            return token;
        case '-':
            token.type = TOK_MINUS;
            strcpy(token.value, "-");
            pos++;
            return token;
        case '*':
            token.type = TOK_MULT;
            strcpy(token.value, "*");
            pos++;
            return token;
        case '/':
            token.type = TOK_DIV;
            strcpy(token.value, "/");
            pos++;
            return token;
    }

    // Two character tokens
    if (ch == '=' && pos + 1 < len) {
        if (input[pos + 1] == '=') {
            token.type = TOK_EQ;
            strcpy(token.value, "==");
            pos += 2;
            return token;
        } else {
            token.type = TOK_ASSIGN;
            strcpy(token.value, "=");
            pos++;
            return token;
        }
    }

    if (ch == '!' && pos + 1 < len && input[pos + 1] == '=') {
        token.type = TOK_NE;
        strcpy(token.value, "!=");
        pos += 2;
        return token;
    }

    if (ch == '<') {
        if (pos + 1 < len && input[pos + 1] == '=') {
            token.type = TOK_LE;
            strcpy(token.value, "<=");
            pos += 2;
        } else {
            token.type = TOK_LT;
            strcpy(token.value, "<");
            pos++;
        }
        return token;
    }

    if (ch == '>') {
        if (pos + 1 < len && input[pos + 1] == '=') {
            token.type = TOK_GE;
            strcpy(token.value, ">=");
            pos += 2;
        } else {
            token.type = TOK_GT;
            strcpy(token.value, ">");
            pos++;
        }
        return token;
    }

    // Numbers
    if (isdigit(ch)) {
        int start = pos;
        while (pos < len && isdigit(input[pos])) {
            pos++;
        }
        token.type = TOK_NUMBER;
        strncpy(token.value, input + start, pos - start);
        token.value[pos - start] = '\0';
        return token;
    }

    // Identifiers and keywords
    if (isalpha(ch) || ch == '_') {
        int start = pos;
        while (pos < len && (isalnum(input[pos]) || input[pos] == '_')) {
            pos++;
        }
        strncpy(token.value, input + start, pos - start);
        token.value[pos - start] = '\0';

        if (strcmp(token.value, "while") == 0) {
            token.type = TOK_WHILE;
        } else if (strcmp(token.value, "if") == 0) {
            token.type = TOK_IF;
        } else if (strcmp(token.value, "else") == 0) {
            token.type = TOK_ELSE;
        } else {
            token.type = TOK_IDENTIFIER;
        }
        return token;
    }

    // Error case
    token.type = TOK_ERROR;
    sprintf(token.value, "Unexpected character: %c", ch);
    pos++;
    return token;
}

// Advance to next token
void advance() {
    current_token = get_next_token();
}

// Parse factor: NUMBER | IDENTIFIER | '(' expression ')'
int parse_factor() {
    if (current_token.type == TOK_NUMBER || current_token.type == TOK_IDENTIFIER) {
        advance();
        return 1;
    } else if (current_token.type == TOK_LPAREN) {
        advance();
        if (!parse_expression()) return 0;
        if (current_token.type != TOK_RPAREN) return 0;
        advance();
        return 1;
    }
    return 0;
}

// Parse term: factor (('+' | '-' | '*' | '/') factor)*
int parse_term() {
    if (!parse_factor()) return 0;

    while (current_token.type == TOK_PLUS || current_token.type == TOK_MINUS ||
           current_token.type == TOK_MULT || current_token.type == TOK_DIV) {
        advance();
        if (!parse_factor()) return 0;
    }
    return 1;
}

// Parse expression: term
int parse_expression() {
    return parse_term();
}

// Parse condition: expression ('==' | '!=' | '<' | '>' | '<=' | '>=') expression
int parse_condition() {
    if (!parse_expression()) return 0;

    if (current_token.type == TOK_EQ || current_token.type == TOK_NE ||
        current_token.type == TOK_LT || current_token.type == TOK_GT ||
        current_token.type == TOK_LE || current_token.type == TOK_GE) {
        advance();
        if (!parse_expression()) return 0;
    }

    return 1;
}

// Parse assignment: IDENTIFIER '=' expression ';'
int parse_assignment() {
    if (current_token.type != TOK_IDENTIFIER) return 0;
    advance();

    if (current_token.type != TOK_ASSIGN) return 0;
    advance();

    if (!parse_expression()) return 0;

    if (current_token.type != TOK_SEMICOLON) return 0;
    advance();

    return 1;
}

// Parse while statement: 'while' '(' condition ')' '{' statement* '}'
int parse_while_statement() {
    if (current_token.type != TOK_WHILE) return 0;
    advance();

    if (current_token.type != TOK_LPAREN) return 0;
    advance();

    if (!parse_condition()) return 0;

    if (current_token.type != TOK_RPAREN) return 0;
    advance();

    if (current_token.type != TOK_LBRACE) return 0;
    advance();

    while (current_token.type != TOK_RBRACE && current_token.type != TOK_EOF) {
        if (!parse_statement()) return 0;
    }

    if (current_token.type != TOK_RBRACE) return 0;
    advance();

    return 1;
}

// Parse if statement: 'if' '(' condition ')' '{' statement* '}' ('else' '{' statement* '}')?
int parse_if_statement() {
    if (current_token.type != TOK_IF) return 0;
    advance();

    if (current_token.type != TOK_LPAREN) return 0;
    advance();

    if (!parse_condition()) return 0;

    if (current_token.type != TOK_RPAREN) return 0;
    advance();

    if (current_token.type != TOK_LBRACE) return 0;
    advance();

    while (current_token.type != TOK_RBRACE && current_token.type != TOK_EOF) {
        if (!parse_statement()) return 0;
    }

    if (current_token.type != TOK_RBRACE) return 0;
    advance();

    // Optional else clause
    if (current_token.type == TOK_ELSE) {
        advance();
        if (current_token.type != TOK_LBRACE) return 0;
        advance();

        while (current_token.type != TOK_RBRACE && current_token.type != TOK_EOF) {
            if (!parse_statement()) return 0;
        }

        if (current_token.type != TOK_RBRACE) return 0;
        advance();
    }

    return 1;
}

// Parse statement: while_statement | if_statement | assignment
int parse_statement() {
    if (current_token.type == TOK_WHILE) {
        return parse_while_statement();
    } else if (current_token.type == TOK_IF) {
        return parse_if_statement();
    } else if (current_token.type == TOK_IDENTIFIER) {
        return parse_assignment();
    }
    return 0;
}

// Parse program: statement*
int parse_program() {
    while (current_token.type != TOK_EOF) {
        if (!parse_statement()) return 0;
    }
    return 1;
}

// Main validation function
int validate_simple_while_grammar(char* code){
    input = code;
    pos = 0;
    len = strlen(code);

    advance(); // Get first token

    if (current_token.type == TOK_ERROR) {
        return 0;
    }

    return parse_program();
}
