#include "ARVADA.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Grammar for natural while loops:
// WHILE_STMT -> while CONDITION do STATEMENTS | while CONDITION STATEMENTS
// CONDITION -> EXPR | EXPR LOGICAL_OP EXPR
// LOGICAL_OP -> & | and | | | or
// EXPR -> IDENTIFIER | NUMBER | true | false | ( EXPR )
//       | IDENTIFIER COMP_OP EXPR
// COMP_OP -> < | > | <= | >= | == | != | =
// STATEMENTS -> STATEMENT | STATEMENT ; STATEMENTS
// STATEMENT -> IDENTIFIER = EXPR | IDENTIFIER = ( EXPR )
// EXPR -> IDENTIFIER | NUMBER | EXPR + EXPR | EXPR - EXPR | EXPR * EXPR | EXPR / EXPR
// IDENTIFIER -> [a-zA-Z][a-zA-Z0-9_]*
// NUMBER -> [0-9]+

typedef struct {
    const char* input;
    int pos;
    int len;
} Parser;

void skip_whitespace(Parser* p) {
    while (p->pos < p->len && isspace(p->input[p->pos])) {
        p->pos++;
    }
}

int match_keyword(Parser* p, const char* keyword) {
    skip_whitespace(p);
    int keyword_len = strlen(keyword);
    if (p->pos + keyword_len <= p->len &&
        strncmp(p->input + p->pos, keyword, keyword_len) == 0 &&
        (p->pos + keyword_len == p->len ||
         !isalnum(p->input[p->pos + keyword_len]))) {
        p->pos += keyword_len;
        return 1;
    }
    return 0;
}

int match_char(Parser* p, char c) {
    skip_whitespace(p);
    if (p->pos < p->len && p->input[p->pos] == c) {
        p->pos++;
        return 1;
    }
    return 0;
}

int match_string(Parser* p, const char* str) {
    skip_whitespace(p);
    int str_len = strlen(str);
    if (p->pos + str_len <= p->len &&
        strncmp(p->input + p->pos, str, str_len) == 0) {
        p->pos += str_len;
        return 1;
    }
    return 0;
}

int match_identifier(Parser* p) {
    skip_whitespace(p);
    if (p->pos >= p->len || !isalpha(p->input[p->pos])) {
        return 0;
    }
    p->pos++;
    while (p->pos < p->len && (isalnum(p->input[p->pos]) || p->input[p->pos] == '_')) {
        p->pos++;
    }
    return 1;
}

int match_number(Parser* p) {
    skip_whitespace(p);
    if (p->pos >= p->len || !isdigit(p->input[p->pos])) {
        return 0;
    }
    while (p->pos < p->len && isdigit(p->input[p->pos])) {
        p->pos++;
    }
    return 1;
}

int match_boolean(Parser* p) {
    return match_keyword(p, "true") || match_keyword(p, "false");
}

int match_comparison_operator(Parser* p) {
    skip_whitespace(p);

    if (match_string(p, "<=") || match_string(p, ">=") ||
        match_string(p, "==") || match_string(p, "!=")) {
        return 1;
    }

    if (match_char(p, '<') || match_char(p, '>') || match_char(p, '=')) {
        return 1;
    }

    return 0;
}

int match_logical_operator(Parser* p) {
    skip_whitespace(p);

    if (match_keyword(p, "and") || match_keyword(p, "or") ||
        match_char(p, '&') || match_char(p, '|')) {
        return 1;
    }

    return 0;
}

int match_arithmetic_operator(Parser* p) {
    skip_whitespace(p);
    if (match_char(p, '+') || match_char(p, '-') ||
        match_char(p, '*') || match_char(p, '/')) {
        return 1;
    }
    return 0;
}

// Forward declarations
int parse_expression(Parser* p);
int parse_condition(Parser* p);

int parse_primary_expression(Parser* p) {
    skip_whitespace(p);

    // Try parenthesized expression
    if (match_char(p, '(')) {
        if (parse_expression(p) && match_char(p, ')')) {
            return 1;
        }
        return 0;
    }

    // Try identifier, number, or boolean
    return match_identifier(p) || match_number(p) || match_boolean(p);
}

int parse_expression(Parser* p) {
    if (!parse_primary_expression(p)) {
        return 0;
    }

    // Try to parse additional terms with operators
    while (match_arithmetic_operator(p)) {
        if (!parse_primary_expression(p)) {
            return 0;
        }
    }

    return 1;
}

int parse_simple_condition(Parser* p) {
    int saved_pos = p->pos;

    // Try: EXPR COMP_OP EXPR
    if (parse_expression(p)) {
        if (match_comparison_operator(p)) {
            if (parse_expression(p)) {
                return 1;
            }
        }
    }

    // Reset and try just expression (like "true" or "false")
    p->pos = saved_pos;
    return parse_expression(p);
}

int parse_condition(Parser* p) {
    if (!parse_simple_condition(p)) {
        return 0;
    }

    // Try to parse logical operators
    while (match_logical_operator(p)) {
        if (!parse_simple_condition(p)) {
            return 0;
        }
    }

    return 1;
}

int parse_assignment(Parser* p) {
    if (!match_identifier(p)) {
        return 0;
    }

    if (!match_char(p, '=')) {
        return 0;
    }

    // Handle parenthesized expressions
    if (match_char(p, '(')) {
        if (parse_expression(p) && match_char(p, ')')) {
            return 1;
        }
        return 0;
    }

    return parse_expression(p);
}

int parse_statement(Parser* p) {
    return parse_assignment(p);
}

int parse_statements(Parser* p) {
    if (!parse_statement(p)) {
        return 0;
    }

    // Try to parse more statements separated by semicolons
    while (match_char(p, ';')) {
        skip_whitespace(p);
        if (p->pos >= p->len) break; // Allow trailing semicolon

        if (!parse_statement(p)) {
            return 0;
        }
    }

    return 1;
}

int parse_while_statement(Parser* p) {
    if (!match_keyword(p, "while")) {
        return 0;
    }

    if (!parse_condition(p)) {
        return 0;
    }

    // Optional "do" keyword
    match_keyword(p, "do");

    if (!parse_statements(p)) {
        return 0;
    }

    skip_whitespace(p);
    return p->pos == p->len; // Should consume entire input
}

int is_while_loop_valid(const char* input) {
    Parser parser = {input, 0, strlen(input)};
    return parse_while_statement(&parser) ? 1 : 0;
}
