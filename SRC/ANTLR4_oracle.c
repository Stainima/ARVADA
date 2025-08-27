#include "ARVADA.h"

// Ultra-simple oracle - returns 1 for valid, 0 for invalid
int validate(const char* str) {
    if (!str) return 0;

    pANTLR3_INPUT_STREAM input = antlr3StringStreamNew(
        (pANTLR3_UINT8)str, ANTLR3_ENC_8BIT, strlen(str), (pANTLR3_UINT8)"");
    pExprLexer lexer = ExprLexerNew(input);
    pANTLR3_COMMON_TOKEN_STREAM tokens = antlr3CommonTokenStreamSourceNew(
        ANTLR3_SIZE_HINT, TOKENSOURCE(lexer));
    pExprParser parser = ExprParserNew(tokens);

    parser->expr(parser);  // Parse

    // Check: no errors + consumed all input
    int valid = (lexer->pLexer->rec->state->errorCount == 0) &&
                (parser->pParser->rec->state->errorCount == 0) &&
                (tokens->_LT(tokens, 1)->getType(tokens->_LT(tokens, 1)) == ANTLR3_TOKEN_EOF);

    // Cleanup
    parser->free(parser);
    tokens->free(tokens);
    lexer->free(lexer);
    input->close(input);

    return valid;  // Returns 1 or 0
}
