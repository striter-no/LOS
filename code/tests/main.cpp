#include <iostream>
#include <bitset>

constexpr int s = 7;

void show(int a){
    std::cout << a << " is: " << std::bitset<s>(a) << std::endl;
}

int main(){
    int a = 0b00000;
    int b = 0b00000;
    int c = 0b00000;

    show(a); show(c);
    
    int imdata_size = 8, buf_size = 3;
    int imdata = 0b10111010;
    int cs = imdata_size / buf_size * buf_size;
    
    a |= (imdata & (0b11100000 >> (0 * buf_size))) >> (buf_size * 1 + (1 - !(cs - buf_size * 0))*(imdata_size % buf_size));
    b |= (imdata & (0b11100000 >> (1 * buf_size))) >> (buf_size * 0 + (1 - !(cs - buf_size * 1))*(imdata_size % buf_size));
    c |= (imdata & (0b11100000 >> (2 * buf_size))) >> (buf_size * 0 + (1 - !(cs - buf_size * 2))*(imdata_size % buf_size));

    show(a);
    show(b);
    show(c);
    show(imdata);
    return 0;
}