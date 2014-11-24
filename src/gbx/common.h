// Copyright (c) 2014 Fabian Barkhau <fabian.barkhau@gmail.com> 
// License: MIT (see LICENSE.TXT file)  

#ifndef GBX_COMMON_H
#define GBX_COMMON_H

#ifndef DEBUG
#define NDEBUG
#endif

#include <iso646.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>


////////////
// MACROS //
////////////

// memory alignment
#define ALIGN(_n)                   __attribute__((aligned(_n)))
#define ALIGN4                      __attribute__((aligned(4)))

// color utils
#define RGB15(r,g,b)                ((r) bitor ((g) << 5) bitor ((b) << 10))
#define RGB15_R(rgb)                (rgb bitand 31)
#define RGB15_G(rgb)                ((rgb >> 5) bitand 31)
#define RGB15_B(rgb)                ((rgb >> 10) bitand 31)
#define INDEX_2D(x,y,w,d)           (d[(y) * (w) + (x)])

// bitmask utils
#define BIT_NUM(n)                  (1 << n)
#define BIT_GET(value, bitmask)     ((value) bitand (bitmask))
#define BIT_SET(value, bitmask)     ((value) or_eq (bitmask))
#define BIT_TOGGLE(value, bitmask)  ((value) xor_eq (bitmask))
#define BIT_CLEAR(value, bitmask)   ((value) and_eq compl (bitmask))


///////////////////
// TYPE WRAPPING //
///////////////////

// wrapping to protect against weak typing errors
#define WRAP_DEF(base_t, wrap_t)    typedef struct { base_t data; } wrap_t
#define WRAP(wrap_t, data)          ((wrap_t){ (data) })
#define WRAP_CONST(data)            { data }
#define WRAP_GET(wrapped)           (wrapped.data)
#define WRAP_NEG(wrapped)           (!wrapped.data)


///////////
// TYPES //
///////////

typedef unsigned char       Bool;
typedef unsigned char       Uint8;
typedef unsigned short      Uint16;
typedef unsigned int        Uint32;
typedef unsigned long long  Uint64;
typedef signed char         Sint8;
typedef signed short        Sint16;
typedef signed int          Sint32;
typedef signed long long    Sint64;

typedef volatile Bool       VBool;
typedef volatile Uint8      VUint8;
typedef volatile Uint16     VUint16;
typedef volatile Uint32     VUint32;
typedef volatile Uint64     VUint64;
typedef volatile Sint8      VSint8;
typedef volatile Sint16     VSint16;
typedef volatile Sint32     VSint32;
typedef volatile Sint64     VSint64;


///////////////
// CALLBACKS //
///////////////

typedef void (*CallbackFunc)(void* data);

typedef struct { 
  CallbackFunc func; 
  void* data; 
} Callback;

#define CALLBACK(func, data)  ((Callback){ (func), (data) })
#define CALLBACK_NULL         ((Callback) { NULL, NULL })

inline void callback_exec(Callback callback){
  if (callback.func != NULL){
    callback.func(callback.data);
  }
}


/////////////////////////
// STRING MANIPULATION //
/////////////////////////

// TODO move to core/src/gbx/lib/str.h

/**
 * Inplace string reverse
 */
inline void strrev(char *p) {
  char *q = p;
  while(q && *q) ++q;
  for(--q; p < q; ++p, --q)
    *p = *p ^ *q,
    *q = *p ^ *q,
    *p = *p ^ *q;
}

/**
 * Converts int to string.
 * Will always be five digits with leading zeros.
 */
inline void uitoa(char buffer[6], Uint16 num){
  // iter 0
  buffer[4] = '0' + (num % 10);

  // iter 1
  num /= 10;
  buffer[3] = '0' + (num % 10);

  // iter 2
  num /= 10;
  buffer[2] = '0' + (num % 10);

  // iter 3
  num /= 10;
  buffer[1] = '0' + (num % 10);

  // iter 4
  num /= 10;
  buffer[0] = '0' + (num % 10);

  // terminate string
  buffer[5] = '\0';
}


//////////
// MISC //
//////////

#define BUILD_HALT(condition)   ((void)sizeof(char[1 - 2*!!(condition)]))
#define SUPPRESS_UNUSED(var)    (var = var)

inline Uint32 index_wrap(Uint32 entries, Uint32 current, Sint32 move){
  assert(current < entries);
  assert((entries + move) < (entries + entries));
  assert(((Sint32)entries + move) >= 0);
  return (current + entries + move) % entries;
}

inline Uint32 menu_index_next(Uint32 entries, Uint32 current){
  return index_wrap(entries, current, 1);
}

inline Uint32 menu_index_prev(Uint32 entries, Uint32 current){
  return index_wrap(entries, current, -1);
}

#endif
