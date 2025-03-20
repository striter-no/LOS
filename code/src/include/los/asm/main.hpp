/*
Registers:
R0-R32

1. R0 - always zero register
2. R1-R31 - your scope variables

Operations

1. Registers
2. Arithmetics
3. Boolean operations
4. Bit manipulation
5. RAM management
6. Branching

*/
#define COMANDS_COUNT 2

#define KI_OPCODE 0
#define KI_IMMDATA_0 1
#define KI_IMMDATA_1 2
#define KI_RD 3
#define KI_RS 4

#define HLT 0b11111 // +


#define XORU HLT-1 // +
#define ORU  XORU-1 // +
#define ANDU ORU-1 // +

#define XOR ANDU-1 // +
#define OR  XOR-1 // +
#define AND OR-1 // +

#define BLSU AND-1 // +
#define BGRU BLSU-1 // +

#define BLS BGRU-1 // +
#define BGR BLS-1 // +
#define BNE BGR-1 // +
#define BEQ BNE-1 // +

#define STORE BEQ-1 // +
#define LOAD  STORE-1 // +

#define LSH LOAD-1 // +
#define RSH LSH-1 // +
#define SUB RSH-1 // +
#define ADD SUB-1 // +

#define REG ADD-1 // +

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdlib.h>
#include <iostream>
using byte = unsigned char;

struct mark {
    char *label;
    int addr;
};

mark marks[100];
int64_t dinamic_memory[1000];

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

mark findMark(const char *label){
    for(int i = 0; i < 100; ++i) {
        if (marks[i].label && strcmp(marks[i].label, label) == 0) {
            return marks[i];
        }
    }
    return {};
}


bool HALT = false;
__int64_t _i64registers[32] = {0};
long long IP = 0;

void hlt()                                  { HALT = true; }

void store(byte reg, long long address){ dinamic_memory[address] = _i64registers[reg]; }
void load (byte reg, long long address){ _i64registers[reg] = dinamic_memory[address]; }

void bit_xoru(byte a, byte b, byte c){ _i64registers[c] = abs(a) ^ abs(b); }
void bit_oru (byte a, byte b, byte c){ _i64registers[c] = abs(a) | abs(b); }
void bit_andu(byte a, byte b, byte c){ _i64registers[c] = abs(a) & abs(b); }

void bit_xor(byte a, byte b, byte c){ _i64registers[c] = a ^ b; }
void bit_or (byte a, byte b, byte c){ _i64registers[c] = a | b; }
void bit_and(byte a, byte b, byte c){ _i64registers[c] = a & b; }

void blsu(byte a, byte b, long long address) { if (abs(_i64registers[a]) < abs(_i64registers[b])) IP = address; }
void bgru(byte a, byte b, long long address) { if (abs(_i64registers[a]) > abs(_i64registers[b])) IP = address; }

void bls(byte a, byte b, long long address) { if (_i64registers[a] < _i64registers[b]) IP = address; }
void bgr(byte a, byte b, long long address) { if (_i64registers[a] > _i64registers[b]) IP = address; }
void beq(byte a, byte b, long long address) { if (_i64registers[a] == _i64registers[b]) IP = address; }

void lsh(byte a, byte b, byte c) { _i64registers[c] = _i64registers[a] << _i64registers[b]; }
void rsh(byte a, byte b, byte c) { _i64registers[c] = _i64registers[a] >> _i64registers[b]; }
void add(byte a, byte b, byte c) { _i64registers[c] = _i64registers[a] + _i64registers[b]; }
void sub(byte a, byte b, byte c) { _i64registers[c] = _i64registers[a] - _i64registers[b]; }

void reg(byte a, const __int64_t &value) { _i64registers[a] = value; }


void execute_line(byte line[5]){
    byte opcode, immdata[2], destination, source;
    rev_line(&opcode, immdata, &destination, &source, line);

    switch (opcode) {
        case STORE:
            store(source, _i64registers[immdata[0]]);
            IP++;
            break;
        case LOAD:
            load(destination, _i64registers[immdata[0]]);
            IP++;
            break;
        case ANDU:
            bit_andu(immdata[0], immdata[1], destination);
            IP++;
            break;
        case XORU:
            bit_xoru(immdata[0], immdata[1], destination);
            IP++;
            break;
        case ORU:
            bit_oru(immdata[0], immdata[1], destination);
            IP++;
            break;
        case AND:
            bit_and(immdata[0], immdata[1], destination);
            IP++;
            break;
        case XOR:
            bit_xor(immdata[0], immdata[1], destination);
            IP++;
            break;
        case OR:
            bit_or(immdata[0], immdata[1], destination);
            IP++;
            break;
        case BLSU:
            blsu(source, destination, immdata[0]);
            break;
        case BGRU:
            bgru(source, destination, immdata[0]);
            break;
        case BLS:
            bls(source, destination, immdata[0]);
            break;
        case BGR:
            bgr(source, destination, immdata[0]);
            break;
        case BEQ:
            beq(source, destination, immdata[0]);
            break;
        case ADD:
            add(immdata[0], immdata[1], destination);
            IP++;
            break;
        case SUB:
            sub(immdata[0], immdata[1], destination);
            IP++;
            break;
        case RSH:
            rsh(immdata[0], immdata[1], destination);
            IP++;
            break;
        case LSH:
            lsh(immdata[0], immdata[1], destination);
            IP++;
            break;
        case REG:
            reg(destination, immdata[0]);
            IP++;
            break;
        case HLT:
            hlt();
            IP++;
            break;
        default:
            break;
    }
}

byte opcode_map(const char *str){
    if (strcmp(str, "add") == 0) return ADD;
    if (strcmp(str, "reg") == 0) return REG;
    if (strcmp(str, "hlt") == 0) return HLT;
    if (strcmp(str, "xoru") == 0) return XORU;
    if (strcmp(str, "oru") == 0)  return ORU;
    if (strcmp(str, "andu") == 0) return ANDU;
    if (strcmp(str, "xor") == 0)  return XOR;
    if (strcmp(str, "or") == 0)   return OR;
    if (strcmp(str, "and") == 0)  return AND;
    if (strcmp(str, "blsu") == 0) return BLSU;
    if (strcmp(str, "bgru") == 0) return BGRU;
    if (strcmp(str, "bls") == 0) return BLS;
    if (strcmp(str, "bgr") == 0) return BGR;
    if (strcmp(str, "beq") == 0) return BEQ;
    if (strcmp(str, "st") == 0)  return STORE;
    if (strcmp(str, "ld") == 0)  return LOAD;
    if (strcmp(str, "ls") == 0)  return LSH;
    if (strcmp(str, "rs") == 0)  return RSH;
    if (strcmp(str, "sub") == 0) return SUB;
    std::cerr << "Unknown opcode: " << str << std::endl;
    return -1;
}

int ki_opcode(byte opcode, int ki){
    switch(opcode) {
        case XORU:
            if (ki == KI_OPCODE) return KI_IMMDATA_0;
            if (ki == KI_IMMDATA_0) return KI_IMMDATA_1;
            if (ki == KI_IMMDATA_1) return KI_RD;
            break;
        case ORU:
            if (ki == KI_OPCODE) return KI_IMMDATA_0;
            if (ki == KI_IMMDATA_0) return KI_IMMDATA_1;
            if (ki == KI_IMMDATA_1) return KI_RD;
            break;
        case ANDU:
            if (ki == KI_OPCODE) return KI_IMMDATA_0;
            if (ki == KI_IMMDATA_0) return KI_IMMDATA_1;
            if (ki == KI_IMMDATA_1) return KI_RD;
            break;
        case XOR:
            if (ki == KI_OPCODE) return KI_IMMDATA_0;
            if (ki == KI_IMMDATA_0) return KI_IMMDATA_1;
            if (ki == KI_IMMDATA_1) return KI_RD;
            break;
        case OR:
            if (ki == KI_OPCODE) return KI_IMMDATA_0;
            if (ki == KI_IMMDATA_0) return KI_IMMDATA_1;
            if (ki == KI_IMMDATA_1) return KI_RD;
            break;
        case AND:
            if (ki == KI_OPCODE) return KI_IMMDATA_0;
            if (ki == KI_IMMDATA_0) return KI_IMMDATA_1;
            if (ki == KI_IMMDATA_1) return KI_RD;
            break;
        case BLSU:
            if (ki == KI_OPCODE) return KI_RS;
            if (ki == KI_RS) return KI_RD;
            if (ki == KI_RD) return KI_IMMDATA_0;
            break;
        case BGRU:
            if (ki == KI_OPCODE) return KI_RS;
            if (ki == KI_RS) return KI_RD;
            if (ki == KI_RD) return KI_IMMDATA_0;
            break;
        case BLS:
            if (ki == KI_OPCODE) return KI_RS;
            if (ki == KI_RS) return KI_RD;
            if (ki == KI_RD) return KI_IMMDATA_0;
            break;
        case BGR:
            if (ki == KI_OPCODE) return KI_RS;
            if (ki == KI_RS) return KI_RD;
            if (ki == KI_RD) return KI_IMMDATA_0;
            break;
        case BEQ:
            if (ki == KI_OPCODE) return KI_RS;
            if (ki == KI_RS) return KI_RD;
            if (ki == KI_RD) return KI_IMMDATA_0;
            break;
        case STORE:
            if (ki == KI_OPCODE) return KI_RS;
            if (ki == KI_RS) return KI_IMMDATA_0;
            break;
        case LOAD:
            if (ki == KI_OPCODE) return KI_RD;
            if (ki == KI_RD) return KI_IMMDATA_0;
            break;
        case LSH:
            if (ki == KI_OPCODE) return KI_IMMDATA_0;
            if (ki == KI_IMMDATA_0) return KI_IMMDATA_1;
            if (ki == KI_IMMDATA_1) return KI_RD;
            break;
        case RSH:
            if (ki == KI_OPCODE) return KI_IMMDATA_0;
            if (ki == KI_IMMDATA_0) return KI_IMMDATA_1;
            if (ki == KI_IMMDATA_1) return KI_RD;
            break;
        case SUB:
            if (ki == KI_OPCODE) return KI_IMMDATA_0;
            if (ki == KI_IMMDATA_0) return KI_IMMDATA_1;
            if (ki == KI_IMMDATA_1) return KI_RD;
            break;
        case ADD:
            if (ki == KI_OPCODE) return KI_IMMDATA_0;
            if (ki == KI_IMMDATA_0) return KI_IMMDATA_1;
            if (ki == KI_IMMDATA_1) return KI_RD;
            break;
        case REG:
            if (ki == KI_OPCODE) return KI_RD;
            if (ki == KI_RD) return KI_IMMDATA_0;
            break;
        case HLT:
            return -1;
        default:
            return -1;
    }
}

byte reg_map(const char *str, bool &ok){
    ok = false;
    if (str[0] != 'r') return -1;
    const char *p = str + 1;
    ok = true;
    return atoi(p);
}

int count(const char *str, char c) {
    int count = 0;
    while (*str) {
        if (*str++ == c) count++;
    }
    return count;
}

int smart_stoi(const char *str){
    int base = 10;
    bool registered = false;
    byte reg_guess = reg_map(str, registered);
    
    if (registered) return reg_guess;

    if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
        base = 16;
    } else if (str[0] == '0' && (str[1] == 'b' || str[1] == 'B')) {
        base = 2;
    } else if (str[0] == '0') {
        base = 8;
    }
    
    return strtol(str, NULL, base);
}

/*
OPCODE <IMMDATA> <REG_S> <REG_D>
*/
void cmp_from_cstring(const char *str, size_t len, byte outline[5]){
    byte opcode, immdata[2], destination, source;
    
    char *word = new char[len + 1]{' '};
    short ki = 0;
    for (size_t i = 0, li = 0; i < len + 1; ++i) {
        const char &c = str[i];

        if (isspace(c) || c == '\0') {
            word[li] = '\0';
            
            bool val_registr = false;
            byte val;

            if (ki == KI_IMMDATA_0 && opcode >= BEQ && opcode <= BLSU) {
                val = findMark(word).addr;
            } else {
                if (ki == KI_RD || ki == KI_RS) val = reg_map(word, val_registr);
                else if (ki != KI_OPCODE) val = smart_stoi(word);
                else val = opcode_map(word);
            }

            switch (ki) {
                case KI_OPCODE: opcode      = val; break; 
                case KI_IMMDATA_0: immdata[0]  = val; break; 
                case KI_IMMDATA_1: immdata[1]  = val; break; 
                case KI_RD: destination = val; break; 
                case KI_RS: source      = val; break; 
            }
            ki = ki_opcode(opcode, ki);
            li = 0;
            
            word = new char[len + 1]{' '};
        } else {
            word[li++] = c;
        }
    }

    delete[] word;
    cmp_line(opcode, immdata, destination, source, outline);
}

void cmp_lines(const char *code, byte **lines, int &instr_count){
    char *codeline = new char[50]{' '};
    int spaces = 0;
    bool onlyspaces = true, mark = false;
    int len = strlen(code);
    for (int i = 0, li = 0, nl = 0; i < len + 1; ++i) {
        if (li == 0 && code[i] == '.'){
            mark = true;
            continue;
        }
        if (isspace(code[i]) && onlyspaces){
            spaces++;
        } else if (onlyspaces && !isspace(code[i]) || !onlyspaces) {
            onlyspaces = false;
            if (code[i] == '\n' || code[i] == '\0') {
                codeline[li] = '\0';
                if (mark) {
                    marks[nl].label = new char[strlen(codeline) + 1];
                    strcpy(marks[nl].label, &codeline[0]);
                    marks[nl].addr = nl;
                    mark = false;

                } else {
                    cmp_from_cstring(codeline, strlen(codeline), lines[nl]);
                    instr_count++;
                    nl++;
                }
                codeline = new char[50]{' '};
                li = 0; spaces = 0;
            } else {
                codeline[li++] = code[i];
            }
        }

        if (code[i] == '\n' || code[i] == '\0') {
            onlyspaces = true;
        }
    }

    delete[] codeline;
}

void free_lines(byte **lines, int count){
    for (int i = 0; i < count; ++i) {
        delete[] lines[i];
    }
    delete[] lines;
}

void free_marks(){
    for (int i = 0; i < 100; ++i) {
        if (marks[i].label) 
            delete[] marks[i].label;
    }
}