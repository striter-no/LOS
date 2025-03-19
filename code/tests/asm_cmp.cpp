#include <los/asm/main.hpp>
#include <iostream>
#include <utils/files.hpp>

int count(const char *str, char c) {
    int count = 0;
    while (*str) {
        if (*str++ == c) count++;
    }
    return count;
}

int main(){
    std::string m = utils::fls::getFile("./code/lasm/test.lasm");
    const char *code = m.data();

    byte **lines = new byte*[count(code, '\n')+1];
    for(int i = 0; i < count(code, '\n') + 1; ++i) {
        lines[i] = new byte[5]{0};
    }

    char *codeline = new char[50]{' '};
    for (int i = 0, li = 0, nl = 0; i < strlen(code) + 1; ++i) {
        if (code[i] == '\n' || code[i] == '\0') {
            codeline[li] = '\0';
            cmp_from_cstring(codeline, strlen(codeline), lines[nl]);
            execute_line(lines[nl]);
            li = 0;
            codeline = new char[50]{' '};
            nl++;
        } else {
            codeline[li++] = code[i];
        }
    }
    
    delete[] codeline;
    
    for (int i = 0; i < count(code, '\n') + 1; ++i) {
        delete[] lines[i];
    }
    delete[] lines;

    std::cout << _64registers[0] << std::endl;
    std::cout << _64registers[1] << std::endl;
    std::cout << _64registers[2] << std::endl;
    std::cout << _64registers[3] << std::endl;
    
}