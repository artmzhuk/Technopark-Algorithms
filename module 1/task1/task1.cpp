/*
 * 1.3 Если в числе содержится только один бит со значением 1, записать в выходной поток OK. Иначе записать FAIL
*/

#include <cstdint>
#include <iostream>

bool CheckOnly1Bit(uint32_t x){
    uint8_t one_bit_counter = 0;
    for(uint8_t i = 0; i < 32 && one_bit_counter <= 2; ++i){
        if(x & (1 << i)){
            one_bit_counter++;
        }
    }
    return one_bit_counter == 1;
}

int main(){
    uint32_t input_value = 0;
    std::cin >>  input_value;
    if(CheckOnly1Bit( input_value)){
        std::cout << "OK" << std::endl;
    } else {
        std::cout << "FAIL" << std::endl;
    }
    return 0;
}
