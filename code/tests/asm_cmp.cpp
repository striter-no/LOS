#include <los/asm/main.hpp>
#include <iostream>

int main(){
    const char *code 
        = "reg 0x1 0xFF ";
    
    byte line[5];
    cmp_from_cstring(code, strlen(code), line);
    execute_line(line);

    std::cout << _64registers[1] << std::endl;
}