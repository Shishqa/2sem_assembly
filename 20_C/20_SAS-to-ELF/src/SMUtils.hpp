#ifndef SMUTILS_HPP
#define SMUTILS_HPP

#include <cstdio>

namespace SMU {

    void sas_to_elf64(const char* in_path, const char* out_path);

    size_t sas_to_x86_64(char* out_buf, const char* in_buf);

}

#endif //SMUTILS_HPP
