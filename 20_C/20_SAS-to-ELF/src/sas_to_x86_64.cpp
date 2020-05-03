#include "SMUtils.hpp"

size_t SMU::sas_to_x86_64(char* out_buf, const char* in_buf) {

    const unsigned char return_0[12] = {0x50, 0x5B, 0xB8, 0x3C,
                               0x00, 0x00, 0x00, 0x48,
                               0x31, 0xFF, 0x0F, 0x05};

    for (size_t i = 0; i < 12; ++i) {

        out_buf[i] = return_0[i];
    }
    
    return 12;
}

