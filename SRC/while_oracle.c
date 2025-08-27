#include "ARVADA.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Simple tokenizer for while language
typedef enum {
    TOKEN_EOF,
    TOKEN_ID,
    TOKEN_INT,
    TOKEN_WHILE,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_ASSIGN,    // =
    TOKEN_EQUALS,    // ==
    TOKEN_NOT_EQUALS, // !=
    TOKEN_LESS,      // <
    TOKEN_LESS_EQ,   // <=
    TOKEN_GREATER,   // >
    TOKEN_GREATER_EQ, // >=
    TOKEN_PLUS,      // +
    TOKEN_MINUS,     // -
    TOKEN_MULTIPLY,  // *
    TOKEN_DIVIDE,    // /
    TOKEN_LPAREN,    // (
    TOKEN_RPAREN,    // )
    TOKEN_LBRACE,    // {
    TOKEN_RBRACE,    // }
    TOKEN_SEMICOLON, // ;
    TOKEN_ERROR
} TokenType;

typedef struct {
    TokenType type;
    char value[256];
} Token;

typedef struct {
    const char* input;
    int pos;
    int length;
    Token current_token;
} Parser;

// Skip whitespace and comments
void skip_whitespace(Parser* p) {
    while (p->pos < p->length && isspace(p->input[p->pos])) {
        p->pos++;
    }
}

// Get next token
Token next_token(Parser* p) {
    Token token = {TOKEN_ERROR, ""};

    skip_whitespace(p);

    if (p->pos >= p->length) {
        token.type = TOKEN_EOF;
        return token;
    }

    char c = p->input[p->pos];

    // Single character tokens
    switch (c) {
        case '(': token.type = TOKEN_LPAREN; p->pos++; return token;
        case ')': token.type = TOKEN_RPAREN; p->pos++; return token;
        case '{': token.type = TOKEN_LBRACE; p->pos++; return token;
        case '}': token.type = TOKEN_RBRACE; p->pos++; return token;
        case ';': token.type = TOKEN_SEMICOLON; p->pos++; return token;
        case '+': token.type = TOKEN_PLUS; p->pos++; return token;
        case '-': token.type = TOKEN_MINUS; p->pos++; return token;
        case '*': token.type = TOKEN_MULTIPLY; p->pos++; return token;
        case '/': token.type = TOKEN_DIVIDE; p->pos++; return token;
    }

    // Two character tokens
    if (c == '=' && p->pos + 1 < p->length && p->input[p->pos + 1] == '=') {
        token.type = TOKEN_EQUALS;
        p->pos += 2;
        return token;
    }
    if (c == '!' && p->pos + 1 < p->length && p->input[p->pos + 1] == '=') {
        token.type = TOKEN_NOT_EQUALS;
        p->pos += 2;
        return token;
    }
    if (c == '<' && p->pos + 1 < p->length && p->input[p->pos + 1] == '=') {
        token.type = TOKEN_LESS_EQ;
        p->pos += 2;
        return token;
    }
    if (c == '>' && p->pos + 1 < p->length && p->input[p->pos + 1] == '=') {
        token.type = TOKEN_GREATER_EQ;
        p->pos += 2;
        return token;
    }

    // Single character comparison operators
    if (c == '=') { token.type = TOKEN_ASSIGN; p->pos++; return token; }
    if (c == '<') { token.type = TOKEN_LESS; p->pos++; return token; }
    if (c == '>') { token.type = TOKEN_GREATER; p->pos++; return token; }

    // Numbers
    if (isdigit(c)) {
        token.type = TOKEN_INT;
        int start = p->pos;
        while (p->pos < p->length && isdigit(p->input[p->pos])) {
            p->pos++;
        }
        int len = p->pos - start;
        if (len < 255) {
            strncpy(token.value, &p->input[start], len);
            token.value[len] = '\0';
        }
        return token;
    }

    // Identifiers and keywords
    if (isalpha(c) || c == '_') {
        int start = p->pos;
        while (p->pos < p->length && (isalnum(p->input[p->pos]) || p->input[p->pos] == '_')) {
            p->pos++;
        }
        int len = p->pos - start;
        if (len < 255) {
            strncpy(token.value, &p->input[start], len);
            token.value[len] = '\0';

            // Check for keywords
            if (strcmp(token.value, "while") == 0) token.type = TOKEN_WHILE;
            else if (strcmp(token.value, "if") == 0) token.type = TOKEN_IF;
            else if (strcmp(token.value, "else") == 0) token.type = TOKEN_ELSE;
            else if (strcmp(token.value, "true") == 0) token.type = TOKEN_TRUE;
            else if (strcmp(token.value, "false") == 0) token.type = TOKEN_FALSE;
            else token.type = TOKEN_ID;
        }
        return token;
    }

    // Unknown character
    p->pos++;
    return token;
}

// Forward declarations
int parse_program(Parser* p);
int parse_statement(Parser* p);
int parse_expression(Parser* p);

// Parse expression with operator precedence
int parse_expression(Parser* p) {
    // Simple expression parsing - handles precedence manually

    // Parse primary expression
    if (p->current_token.type == TOKEN_ID ||
        p->current_token.type == TOKEN_INT ||
        p->current_token.type == TOKEN_TRUE ||
        p->current_token.type == TOKEN_FALSE) {
        p->current_token = next_token(p);
    } else if (p->current_token.type == TOKEN_LPAREN) {
        p->current_token = next_token(p);
        if (!parse_expression(p)) return 0;
        if (p->current_token.type != TOKEN_RPAREN) return 0;
        p->current_token = next_token(p);
    } else {
        return 0;
    }

    // Parse binary operators (simplified)
    while (p->current_token.type == TOKEN_PLUS ||
           p->current_token.type == TOKEN_MINUS ||
           p->current_token.type == TOKEN_MULTIPLY ||
           p->current_token.type == TOKEN_DIVIDE ||
           p->current_token.type == TOKEN_EQUALS ||
           p->current_token.type == TOKEN_NOT_EQUALS ||
           p->current_token.type == TOKEN_LESS ||
           p->current_token.type == TOKEN_LESS_EQ ||
           p->current_token.type == TOKEN_GREATER ||
           p->current_token.type == TOKEN_GREATER_EQ) {
        p->current_token = next_token(p);

        // Parse right operand
        if (p->current_token.type == TOKEN_ID ||
            p->current_token.type == TOKEN_INT ||
            p->current_token.type == TOKEN_TRUE ||
            p->current_token.type == TOKEN_FALSE) {
            p->current_token = next_token(p);
        } else if (p->current_token.type == TOKEN_LPAREN) {
            p->current_token = next_token(p);
            if (!parse_expression(p)) return 0;
            if (p->current_token.type != TOKEN_RPAREN) return 0;
            p->current_token = next_token(p);
        } else {
            return 0;
        }
    }

    return 1;
}

// Parse statement
int parse_statement(Parser* p) {
    // Assignment: ID = expression ;
    if (p->current_token.type == TOKEN_ID) {
        p->current_token = next_token(p);
        if (p->current_token.type != TOKEN_ASSIGN) return 0;
        p->current_token = next_token(p);
        if (!parse_expression(p)) return 0;
        if (p->current_token.type != TOKEN_SEMICOLON) return 0;
        p->current_token = next_token(p);
        return 1;
    }

    // While statement: while ( expression ) statement
    if (p->current_token.type == TOKEN_WHILE) {
        p->current_token = next_token(p);
        if (p->current_token.type != TOKEN_LPAREN) return 0;
        p->current_token = next_token(p);
        if (!parse_expression(p)) return 0;
        if (p->current_token.type != TOKEN_RPAREN) return 0;
        p->current_token = next_token(p);
        if (!parse_statement(p)) return 0;
        return 1;
    }

    // If statement: if ( expression ) statement [else statement]
    if (p->current_token.type == TOKEN_IF) {
        p->current_token = next_token(p);
        if (p->current_token.type != TOKEN_LPAREN) return 0;
        p->current_token = next_token(p);
        if (!parse_expression(p)) return 0;
        if (p->current_token.type != TOKEN_RPAREN) return 0;
        p->current_token = next_token(p);
        if (!parse_statement(p)) return 0;

        // Optional else
        if (p->current_token.type == TOKEN_ELSE) {
            p->current_token = next_token(p);
            if (!parse_statement(p)) return 0;
        }
        return 1;
    }

    // Block: { statement* }
    if (p->current_token.type == TOKEN_LBRACE) {
        p->current_token = next_token(p);
        while (p->current_token.type != TOKEN_RBRACE && p->current_token.type != TOKEN_EOF) {
            if (!parse_statement(p)) return 0;
        }
        if (p->current_token.type != TOKEN_RBRACE) return 0;
        p->current_token = next_token(p);
        return 1;
    }

    return 0;
}

// Parse program (list of statements)
int parse_program(Parser* p) {
    while (p->current_token.type != TOKEN_EOF) {
        if (!parse_statement(p)) return 0;
    }
    return 1;
}

// Main validation function - returns 1 if valid, 0 if invalid
int validate_while_program(const char* input) {
    if (!input) return 0;

    Parser parser;
    parser.input = input;
    parser.pos = 0;
    parser.length = strlen(input);

    // Get first token
    parser.current_token = next_token(&parser);

    // Parse program
    int result = parse_program(&parser);

    // Must consume all input
    return result && (parser.current_token.type == TOKEN_EOF);
}
