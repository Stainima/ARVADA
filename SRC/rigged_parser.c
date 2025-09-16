#include "ARVADA.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Simple parser for exactly two patterns:
// 1: "While true & false do L = n"
// 2: "L = n ; L = (n + n)"

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

int match_exact_word(Parser* p, const char* word) {
    skip_whitespace(p);
    int word_len = strlen(word);

    if (p->pos + word_len > p->len) {
        return 0;
    }

    if (strncmp(p->input + p->pos, word, word_len) != 0) {
        return 0;
    }

    // Check that it's a complete word (not part of a longer word)
    if (p->pos + word_len < p->len &&
        (isalnum(p->input[p->pos + word_len]) || p->input[p->pos + word_len] == '_')) {
        return 0;
    }

    p->pos += word_len;
    return 1;
}

int match_char(Parser* p, char c) {
    skip_whitespace(p);
    if (p->pos < p->len && p->input[p->pos] == c) {
        p->pos++;
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

// Pattern 1: "While true & false do L = n"
int parse_pattern1(Parser* p) {
    if (!match_exact_word(p, "While")) return 0;
    if (!match_exact_word(p, "true")) return 0;
    if (!match_char(p, '&')) return 0;
    if (!match_exact_word(p, "false")) return 0;
    if (!match_exact_word(p, "do")) return 0;
    if (!match_identifier(p)) return 0;
    if (!match_char(p, '=')) return 0;
    if (!match_identifier(p)) return 0;

    skip_whitespace(p);
    return p->pos == p->len;
}

// Pattern 2: "L = n ; L = (n + n)"
int parse_pattern2(Parser* p) {
    if (!match_identifier(p)) return 0;
    if (!match_char(p, '=')) return 0;
    if (!match_identifier(p)) return 0;
    if (!match_char(p, ';')) return 0;
    if (!match_identifier(p)) return 0;
    if (!match_char(p, '=')) return 0;
    if (!match_char(p, '(')) return 0;
    if (!match_identifier(p)) return 0;
    if (!match_char(p, '+')) return 0;
    if (!match_identifier(p)) return 0;
    if (!match_char(p, ')')) return 0;

    skip_whitespace(p);
    return p->pos == p->len;
}

int check_rigged_parser(const char* input) {
    Parser p1 = {input, 0, strlen(input)};
    Parser p2 = {input, 0, strlen(input)};

    return parse_pattern1(&p1) || parse_pattern2(&p2);
}
