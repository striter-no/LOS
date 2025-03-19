#include <los/asm/main.hpp>
#include <iostream>

int main(){
    const char *code 
        = "reg 0x1 0xFF\nreg 0x2 0xFA\nreg 0x3 0xFB";
    
    char *codeline = new char[50]{' '};
    for (int i = 0, li = 0; i < strlen(code) + 1; ++i) {
        if (code[i] == '\n' || code[i] == '\0') {
            byte line[5];
            codeline[li] = '\0';

            cmp_from_cstring(codeline, strlen(codeline), line);
            execute_line(line);
            li = 0;
            codeline = new char[50]{' '};
        } else {
            codeline[li++] = code[i];
        }
    }

    delete[] codeline;
    std::cout << _64registers[0] << std::endl;
    std::cout << _64registers[1] << std::endl;
    std::cout << _64registers[2] << std::endl;
    std::cout << _64registers[3] << std::endl;
}