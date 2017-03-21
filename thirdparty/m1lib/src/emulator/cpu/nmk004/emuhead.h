#ifndef EMUHEAD
#define EMUHEAD

#include <stdint.h>
#include <iostream>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef u32 offs_t;

/* Some optimizations/warnings cleanups for GCC */
#if defined(__GNUC__)
#define ATTR_UNUSED             __attribute__((__unused__))
#define ATTR_PRINTF(x,y)        __attribute__((format(printf, x, y)))
#define ATTR_CONST              __attribute__((const))
#define ATTR_FORCE_INLINE       __attribute__((always_inline))
#define ATTR_HOT                __attribute__((hot))
#define ATTR_COLD               __attribute__((cold))
#define UNEXPECTED(exp)         __builtin_expect(!!(exp), 0)
#define EXPECTED(exp)           __builtin_expect(!!(exp), 1)
#define RESTRICT                __restrict__
#else
#define ATTR_UNUSED
#define ATTR_PRINTF(x,y)
#define ATTR_CONST
#define ATTR_FORCE_INLINE       __forceinline
#define ATTR_HOT
#define ATTR_COLD
#define UNEXPECTED(exp)         (exp)
#define EXPECTED(exp)           (exp)
#define RESTRICT
#endif


//**************************************************************************
//  USEFUL COMPOSITE TYPES
//**************************************************************************

// PAIR is an endian-safe union useful for representing 32-bit CPU registers
union PAIR
{
#ifdef LSB_FIRST
    struct { u8 l,h,h2,h3; } b;
    struct { u16 l,h; } w;
    struct { s8 l,h,h2,h3; } sb;
    struct { s16 l,h; } sw;
#else
    struct { u8 h3,h2,h,l; } b;
    struct { s8 h3,h2,h,l; } sb;
    struct { u16 h,l; } w;
    struct { s16 h,l; } sw;
#endif
    u32 d;
    s32 sd;
};


// PAIR16 is a 16-bit extension of a PAIR
union PAIR16
{
#ifdef LSB_FIRST
    struct { u8 l,h; } b;
    struct { s8 l,h; } sb;
#else
    struct { u8 h,l; } b;
    struct { s8 h,l; } sb;
#endif
    u16 w;
    s16 sw;
};


// PAIR64 is a 64-bit extension of a PAIR
union PAIR64
{
#ifdef LSB_FIRST
    struct { u8 l,h,h2,h3,h4,h5,h6,h7; } b;
    struct { u16 l,h,h2,h3; } w;
    struct { u32 l,h; } d;
    struct { s8 l,h,h2,h3,h4,h5,h6,h7; } sb;
    struct { s16 l,h,h2,h3; } sw;
    struct { s32 l,h; } sd;
#else
    struct { u8 h7,h6,h5,h4,h3,h2,h,l; } b;
    struct { u16 h3,h2,h,l; } w;
    struct { u32 h,l; } d;
    struct { s8 h7,h6,h5,h4,h3,h2,h,l; } sb;
    struct { s16 h3,h2,h,l; } sw;
    struct { s32 h,l; } sd;
#endif
    u64 q;
    s64 sq;
};



//**************************************************************************
//  COMMON CONSTANTS
//**************************************************************************

// constants for expression endianness
enum endianness_t
{
    ENDIANNESS_LITTLE,
    ENDIANNESS_BIG
};

//**************************************************************************
//  MACROS
//**************************************************************************

#define TIMER_CALLBACK_MEMBER(name)     void name(void *ptr, s32 param)

// IRQ callback to be called by device implementations when an IRQ is actually taken
#define IRQ_CALLBACK_MEMBER(func)       int func(device_t &device, int irqline)

// interrupt generator callback called as a VBLANK or periodic interrupt
#define INTERRUPT_GEN_MEMBER(func)      void func(device_t &device)

#define DECLARE_READ8_MEMBER(name)       u8     name(ATTR_UNUSED address_space &space, ATTR_UNUSED offs_t offset, ATTR_UNUSED u8 mem_mask = 0xff)
#define DECLARE_WRITE8_MEMBER(name)     void   name(ATTR_UNUSED address_space &space, ATTR_UNUSED offs_t offset, ATTR_UNUSED u8 data, ATTR_UNUSED u8 mem_mask = 0xff)
#define DECLARE_READ16_MEMBER(name)     u16    name(ATTR_UNUSED address_space &space, ATTR_UNUSED offs_t offset, ATTR_UNUSED u16 mem_mask = 0xffff)
#define DECLARE_WRITE16_MEMBER(name)    void   name(ATTR_UNUSED address_space &space, ATTR_UNUSED offs_t offset, ATTR_UNUSED u16 data, ATTR_UNUSED u16 mem_mask = 0xffff)
#define DECLARE_READ32_MEMBER(name)     u32    name(ATTR_UNUSED address_space &space, ATTR_UNUSED offs_t offset, ATTR_UNUSED u32 mem_mask = 0xffffffff)
#define DECLARE_WRITE32_MEMBER(name)    void   name(ATTR_UNUSED address_space &space, ATTR_UNUSED offs_t offset, ATTR_UNUSED u32 data, ATTR_UNUSED u32 mem_mask = 0xffffffff)
#define DECLARE_READ64_MEMBER(name)     u64    name(ATTR_UNUSED address_space &space, ATTR_UNUSED offs_t offset, ATTR_UNUSED u64 mem_mask = 0xffffffffffffffffU)
#define DECLARE_WRITE64_MEMBER(name)    void   name(ATTR_UNUSED address_space &space, ATTR_UNUSED offs_t offset, ATTR_UNUSED u64 data, ATTR_UNUSED u64 mem_mask = 0xffffffffffffffffU)

// macro for declaring enumerator operators that increment/decrement like plain old C
#define DECLARE_ENUM_OPERATORS(TYPE) \
inline TYPE &operator++(TYPE &value) { return value = TYPE(std::underlying_type_t<TYPE>(value) + 1); } \
inline TYPE operator++(TYPE &value, int) { TYPE const old(value); ++value; return old; } \
inline TYPE &operator--(TYPE &value) { return value = TYPE(std::underlying_type_t<TYPE>(value) - 1); } \
inline TYPE operator--(TYPE &value, int) { TYPE const old(value); --value; return old; }

#endif // EMUHEAD

