// Copyright (c) 2014 Fabian Barkhau <fabian.barkhau@gmail.com> 
// License: MIT (see LICENSE.TXT file)  

#ifndef GBX_INPUT_H
#define GBX_INPUT_H

#include <src/gbx/common.h>

#define input_reg_key     (*(VUint16*)(0x04000130))

#define INPUT_KEY_MASK    0x03FF
#define INPUT_KEY_A       0x0001
#define INPUT_KEY_B       0x0002
#define INPUT_KEY_SELECT  0x0004
#define INPUT_KEY_START   0x0008
#define INPUT_KEY_RIGHT   0x0010
#define INPUT_KEY_LEFT    0x0020
#define INPUT_KEY_UP      0x0040
#define INPUT_KEY_DOWN    0x0080
#define INPUT_KEY_R       0x0100
#define INPUT_KEY_L       0x0200

#define INPUT_KEYS_DPAD   (INPUT_KEY_UP bitor INPUT_KEY_DOWN bitor \
                           INPUT_KEY_LEFT bitor INPUT_KEY_RIGHT)
#define INPUT_KEYS_ALL    INPUT_KEY_MASK

extern Uint16 _input_key_curr;
extern Uint16 _input_key_prev;

// Read user input.
inline void input_poll(){
  _input_key_prev = _input_key_curr;
  _input_key_curr = compl input_reg_key bitand INPUT_KEY_MASK;
}

// Check if key is currently pressed.
inline Bool input_key_down(Uint16 key){
  return  _input_key_curr bitand key;
}

// Check if key is currently not pressed.
inline Bool input_key_up(Uint16 key){
  return compl _input_key_curr bitand key;
}

// Check if key is held (down now and before).
inline Bool input_key_held(Uint16 key){   
  return (_input_key_curr bitand _input_key_prev) bitand key;  
}

// Check if key is pressed now but not before.
inline Bool input_key_hit(Uint16 key){   
  return (_input_key_curr bitand compl _input_key_prev) bitand key;  
}

// Check if key was pressed before but not now.
inline Bool input_key_released(Uint16 key){   
  return (compl _input_key_curr bitand _input_key_prev) bitand key;  
}

#endif
