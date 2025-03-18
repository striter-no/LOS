/*
Registers:
R0-R32

1. R0 - always zero register
2. R1-R31 - your scope variables

Operations

1. Registers
    mov r0, r1 - Copy value from R1 to R0
    reg r0, VALUE - Store value in R0

2. Arithmetics
3. Boolean operations
4. Bit manipulation
5. RAM management
6. Branching

*/
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <iostream>
using byte = unsigned char;

#define COMANDS_COUNT 2

#define REG 0b00010
#define ADD 0b00001

__int64_t _64registers[32] = {0};

void add(byte a, byte b, byte c) { _64registers[c] = _64registers[a] + _64registers[b]; }
void reg(byte a, const __int64_t &value) { _64registers[a] = value; }

__int64_t commands[COMANDS_COUNT]{
    ADD, REG
};

/*
Working version:
    opcode | immdata | destination register | source register |
    8 bits | 16 bits |       8 bits         |    8 bits       |

    bytes:
    | opcode(8) | immdata (8) 1| immdata (8) 2| rd(8) | rs(8) |

Nova version:
    
    opcode | immdata | destination register | source register |
    5 bits | 25 bits |       5 bits         |    5 bits       |

    bytes:

    | opcode(5) + immdata(3) 0| immdata (8) 1| immdata (8) 2| immdata (6)3 + rd(2) | rd(3) + rs(5) |

*/
void cmp_line(byte opcode, byte immdata[2], byte destination, byte source, byte out[5]){
    out[0] = opcode;
    out[1] = immdata[0];
    out[2] = immdata[1];
    out[3] = destination;
    out[4] = source;
}

void rev_line(byte *opcode, byte immdata[2], byte *destination, byte *source, byte in[5]){
    *opcode = in[0];
    immdata[0] = in[1];
    immdata[1] = in[2];
    *destination = in[3];
    *source = in[4];
}

void execute_line(byte line[5]){
    byte opcode, immdata[2], destination, source;
    rev_line(&opcode, immdata, &destination, &source, line);

    switch (opcode) {
        case ADD:
            add(destination, source, destination);
            break;
        case REG:
            reg(destination, immdata[0]);
            break;
        default:
            break;
    }
}

byte opcode_map(const char *str){
    if (strcmp(str, "add") == 0) return ADD;
    if (strcmp(str, "reg") == 0) return REG;
    return -1;
}

int ki_opcode(byte opcode, int ki){
    switch(opcode) {
        case ADD:
            if (ki == 0) return 1;
            if (ki == 1) return 2;
            if (ki == 2) return 3;
            break;
        case REG:
            if (ki == 0) return 3;
            if (ki == 3) return 1;
            break;
        default:
            return -1;
    }
}

/*
OPCODE <IMMDATA> <REG_S> <REG_D>
*/
void cmp_from_cstring(const char *str, size_t len, byte outline[5]){
    byte opcode, immdata[2], destination, source;
    
    char *word = new char[len + 1]{' '};
    short ki = 0;
    for (size_t i = 0, li = 0; i < len; ++i) {
        const char &c = str[i];

        if (isspace(c)){
            word[li] = '\0';
            // std::cout << "word: " << word << std::endl;

            byte val;
            if (ki != 0) val = (int)strtol(word, NULL, 16);
            else val = opcode_map(word);
            
            // std::cout << "ki: " << (int)ki << std::endl;
            // std::cout << "val: " << (int)val << std::endl;


            switch (ki) {
                case 0: opcode      = val; break;
                case 1: immdata[0]  = val; break;
                case 2: immdata[1]  = val; break;
                case 3: destination = val; break;
                case 4: source      = val; break;
            }
            ki = ki_opcode(opcode, ki);
            li = 0;
            
            word = new char[len + 1]{' '};
            // std::cout << std::endl;
        } else {
            word[li++] = c;
        }
    }

    cmp_line(opcode, immdata, destination, source, outline);
}