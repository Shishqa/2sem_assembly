#include <cstring>

#include "argument.hpp"

Argument::Argument(const char* arg_ptr) {
    memcpy(reinterpret_cast<void*>(this), arg_ptr, sizeof(Argument));
}
