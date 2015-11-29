#ifndef encryption_lib_h
#define encryption_lib_h
#include "Arduino.h"

byte encrypt(byte);
void encrypt_array(byte*, unsigned int);
byte decrypt(byte);
void decrypt_array(byte*, unsigned int);

#endif
