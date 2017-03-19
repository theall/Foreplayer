#ifndef EMUHEAD
#define EMUHEAD

#include <stdint.h>

// macro for declaring enumerator operators that increment/decrement like plain old C
#define DECLARE_ENUM_OPERATORS(TYPE) \
inline TYPE &operator++(TYPE &value) { return value = TYPE(std::underlying_type_t<TYPE>(value) + 1); } \
inline TYPE operator++(TYPE &value, int) { TYPE const old(value); ++value; return old; } \
inline TYPE &operator--(TYPE &value) { return value = TYPE(std::underlying_type_t<TYPE>(value) - 1); } \
inline TYPE operator--(TYPE &value, int) { TYPE const old(value); --value; return old; }

#endif // EMUHEAD

