#include "ARVADA.h"
#include <string.h>
#include <ctype.h>

// Token types for the specific grammar
typedef enum {
    TOK_WHILE,
    TOK_IF,
    TOK_THEN,
    TOK_ELSE,
    TOK_DO,
    TOK_TRUE,
    TOK_FALSE,
    TOK_AND,        // &
    TOK_NOT,        // ~
    TOK_EQUAL,      // ==
    TOK_ASSIGN,     // =
    TOK_LPAREN,     // (
    TOK_RPAREN,     // )
    TOK_PLUS,       // +
    TOK_SEMICOLON,  // ;
    TOK_IDENTIFIER, // L, n, etc.
    TOK_NUMBER,     // numbers
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
int parse_start();
int parse_stmt();
int parse_boolexpr_base();
int parse_boolexpr();
int parse_numexpr();
void skip_whitespace();

// Skip whitespace
void skip_whitespace() {
    while (pos < len && isspace(input[pos])) {
        pos++;
    }
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
        case '+':
            token.type = TOK_PLUS;
            strcpy(token.value, "+");
            pos++;
            return token;
        case ';':
            token.type = TOK_SEMICOLON;
            strcpy(token.value, ";");
            pos++;
            return token;
        case '&':
            token.type = TOK_AND;
            strcpy(token.value, "&");
            pos++;
            return token;
        case '~':
            token.type = TOK_NOT;
            strcpy(token.value, "~");
            pos++;
            return token;
    }

    // Two character tokens
    if (ch == '=' && pos + 1 < len) {
        if (input[pos + 1] == '=') {
            token.type = TOK_EQUAL;
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

        // Check for keywords
        if (strcmp(token.value, "while") == 0) {
            token.type = TOK_WHILE;
        } else if (strcmp(token.value, "if") == 0) {
            token.type = TOK_IF;
        } else if (strcmp(token.value, "then") == 0) {
            token.type = TOK_THEN;
        } else if (strcmp(token.value, "else") == 0) {
            token.type = TOK_ELSE;
        } else if (strcmp(token.value, "do") == 0) {
            token.type = TOK_DO;
        } else if (strcmp(token.value, "true") == 0) {
            token.type = TOK_TRUE;
        } else if (strcmp(token.value, "false") == 0) {
            token.type = TOK_FALSE;
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

// Parse numexpr: ( numexpr + numexpr ) | L | n
int parse_numexpr() {
    if (current_token.type == TOK_LPAREN) {
        advance(); // consume '('
        if (!parse_numexpr()) return 0;
        if (current_token.type != TOK_PLUS) return 0;
        advance(); // consume '+'
        if (!parse_numexpr()) return 0;
        if (current_token.type != TOK_RPAREN) return 0;
        advance(); // consume ')'
        return 1;
    } else if (current_token.type == TOK_IDENTIFIER || current_token.type == TOK_NUMBER) {
        advance();
        return 1;
    }
    return 0;
}

// Parse base boolean expressions (atoms)
int parse_boolexpr_base() {
    if (current_token.type == TOK_NOT) {
        // ~boolexpr
        advance(); // consume '~'
        return parse_boolexpr();
    } else if (current_token.type == TOK_TRUE || current_token.type == TOK_FALSE) {
        // true | false
        advance();
        return 1;
    } else {
        // Try numexpr == numexpr
        if (!parse_numexpr()) return 0;
        if (current_token.type != TOK_EQUAL) return 0;
        advance(); // consume '=='
        return parse_numexpr();
    }
}

// Parse boolexpr with proper precedence handling
int parse_boolexpr() {
    if (!parse_boolexpr_base()) return 0;

    // Handle & operator (left associative)
    while (current_token.type == TOK_AND) {
        advance(); // consume '&'
        if (!parse_boolexpr_base()) return 0;
    }

    return 1;
}

// Parse stmt according to the grammar
int parse_stmt() {
    if (current_token.type == TOK_WHILE) {
        // while boolexpr do stmt
        advance(); // consume 'while'
        if (!parse_boolexpr()) return 0;
        if (current_token.type != TOK_DO) return 0;
        advance(); // consume 'do'
        return parse_stmt();
    } else if (current_token.type == TOK_IF) {
        // if boolexpr then stmt else stmt
        advance(); // consume 'if'
        if (!parse_boolexpr()) return 0;
        if (current_token.type != TOK_THEN) return 0;
        advance(); // consume 'then'
        if (!parse_stmt()) return 0;
        if (current_token.type != TOK_ELSE) return 0;
        advance(); // consume 'else'
        return parse_stmt();
    } else if (current_token.type == TOK_IDENTIFIER) {
        // L = numexpr
        advance(); // consume identifier
        if (current_token.type != TOK_ASSIGN) return 0;
        advance(); // consume '='
        return parse_numexpr();
    }
    return 0;
}

// Parse start: stmt
int parse_start() {
    if (!parse_stmt()) return 0;

    // Handle stmt ; stmt pattern
    while (current_token.type == TOK_SEMICOLON) {
        advance(); // consume ';'
        if (current_token.type == TOK_EOF) break; // Allow trailing semicolon
        if (!parse_stmt()) return 0;
    }

    return current_token.type == TOK_EOF;
}

// Main validation function
int validate_special_while_grammar(char* code) {
    input = code;
    pos = 0;
    len = strlen(code);

    advance(); // Get first token

    if (current_token.type == TOK_ERROR) {
        printf("Lexical error: %s at position %d\n", current_token.value, current_token.pos);
        return 0;
    }

    int result = parse_start();
    if (!result) {
        printf("Parse error at token '%s' (type %d) at position %d\n",
               current_token.value, current_token.type, current_token.pos);
    }
    return result;
}
