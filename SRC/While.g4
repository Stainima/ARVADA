grammar While;

program: statement* ;

statement: assignment
         | whileStatement
         | ifStatement
         | block
         ;

assignment: ID '=' expression ';' ;

whileStatement: 'while' '(' expression ')' statement ;

ifStatement: 'if' '(' expression ')' statement ('else' statement)? ;

block: '{' statement* '}' ;

expression: expression ('=='|'!='|'<'|'<='|'>'|'>=') expression
          | expression ('+'|'-') expression
          | expression ('*'|'/') expression
          | '(' expression ')'
          | ID
          | INT
          | 'true'
          | 'false'
          ;

ID: [a-zA-Z][a-zA-Z0-9_]* ;
INT: [0-9]+ ;
WS: [ \t\r\n]+ -> skip ;
COMMENT: '//' ~[\r\n]* -> skip ;
