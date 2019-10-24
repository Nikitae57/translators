//
// Created by nikita on 17.10.2019.
//

#ifndef LAB2_STRUCTS_H
#define LAB2_STRUCTS_H

enum TYPES {
    ID, CONST_INT, CONST_FLOAT, ERROR
};

enum SYMBOL_CLASS {
    NUMBER,
    OPERATION_MUL, OPERATION_DIV, OPERATION_ADD, OPERATION_SUB,
    OPEN_BRACKET, CLOSE_BRACKET
};

union SYMBOL_VALUE {
    char special_char;
    float num;
};

struct TOKEN {
    SYMBOL_CLASS type;
    SYMBOL_VALUE sym;
};

#endif //LAB2_STRUCTS_H
