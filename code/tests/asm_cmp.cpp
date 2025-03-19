#include <los/asm/main.hpp>
#include <iostream>
#include <utils/files.hpp>
#include <sys/stat.h>

long get_file_size(const char *filename) {
    struct stat stat_buf;
    int rc = stat(filename, &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1; // Return -1 on error
}

void write_bin(const char *filename, byte **lines, int count){
    FILE *f = fopen(filename, "wb");
    for(int i = 0; i < count; ++i) {
        fwrite(lines[i], 1, 5, f);
    }
    fclose(f);
}

void read_bin(const char *filename, byte ***lines, int *count) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        *count = 0;
        return; // Handle file open error
    }

    *count = get_file_size(filename) / 5; // Calculate number of lines
    *lines = new byte*[*count]; // Allocate memory for lines

    for (int i = 0; i < *count; ++i) {
        (*lines)[i] = new byte[5]; // Allocate memory for each line
        fread((*lines)[i], 1, 5, f); // Read the line
    }
    fclose(f);
}

void compile_lasm(const char *source, const char *output){
    std::string m = utils::fls::getFile(source);
    const char *code = m.data();
    int cmd_count = count(code, '\n')+1, instr_count = 0;

    byte **lines = new byte*[cmd_count];
    for(int i = 0; i < count(code, '\n') + 1; ++i) {
        lines[i] = new byte[5]{0};
    }
    cmp_lines(code, lines, instr_count);
    free_marks();
    // Writing to binary file
    write_bin(output, lines, instr_count);
}

int main(){
    compile_lasm("./code/lasm/test.lasm", "./code/bin/test.bin");
    std::cout << "Compiled successfully!" << std::endl;

    int cmds_num = 0;
    byte **lines = nullptr;
    read_bin("./code/bin/test.bin", &lines, &cmds_num);
    std::cout << "Instructions count: " << cmds_num << std::endl;

    while (IP < cmds_num || !HALT) {
        execute_line(lines[IP]);
    }
    free_lines(lines, cmds_num);

    std::cout << _i64registers[0] << std::endl;
    std::cout << _i64registers[1] << std::endl;
    std::cout << _i64registers[2] << std::endl;
    std::cout << _i64registers[3] << std::endl;
    
}