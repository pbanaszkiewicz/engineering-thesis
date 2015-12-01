#include "Arduino.h"
#include "encryption_lib.h"

/* Encryption-decryption arrays below were generated with following
 * Python code:

import random

random.seed(3307124)
L = list(range(256))
P = list(range(256))
random.shuffle(P)

encryption = P[:]
decryption = [v for k, v in dict(zip(P, L)).items()]

print('encryption = [')
for i, v in enumerate(encryption):
    print(hex(v), end=', ')
    if (i + 1) % 8 == 0:
        print('')
print(']')

print('decryption = [')
for i, v in enumerate(decryption):
    print(hex(v), end=', ')
    if (i + 1) % 8 == 0:
        print('')
print(']')

 */

byte _encryption_array[256] = {
    0xcd, 0x9c, 0x19, 0xda, 0xf8, 0x56, 0x9, 0x1a,
    0xbd, 0x5f, 0xe, 0xed, 0x14, 0x75, 0x9d, 0xd5,
    0x99, 0x26, 0xdb, 0x33, 0xe4, 0x62, 0x87, 0xc4,
    0x25, 0x2c, 0x2a, 0xae, 0xb3, 0xd6, 0xca, 0x4b,
    0x6e, 0x27, 0x61, 0x22, 0x78, 0x8b, 0x57, 0xcc,
    0x8f, 0xa7, 0x37, 0xc3, 0x50, 0x3d, 0x17, 0xcf,
    0x7, 0xee, 0x0, 0x3b, 0x2f, 0xa8, 0xac, 0xba,
    0xb4, 0xa2, 0x43, 0xa5, 0x30, 0xfe, 0xb1, 0x68,
    0x77, 0x85, 0x5a, 0x4d, 0x93, 0x90, 0x69, 0xe6,
    0xd, 0x53, 0x2d, 0x36, 0xf2, 0xc, 0xfa, 0x6c,
    0xcb, 0xaf, 0xbb, 0xdf, 0x29, 0x7d, 0x82, 0xd3,
    0x4c, 0x4f, 0x63, 0xa6, 0xb7, 0x5c, 0x64, 0xf1,
    0x67, 0xe9, 0xa9, 0xde, 0x52, 0x86, 0x95, 0xad,
    0xdc, 0x8c, 0x51, 0xe2, 0xf4, 0x79, 0xd7, 0xd0,
    0xdd, 0x2e, 0xfc, 0x81, 0xaa, 0x3a, 0xab, 0x47,
    0xb9, 0x40, 0xec, 0x55, 0x4, 0x32, 0x60, 0x9a,
    0xb, 0x13, 0x5, 0x3f, 0x88, 0x16, 0xe7, 0x89,
    0xeb, 0xbf, 0x7e, 0x41, 0xc6, 0xb2, 0x9f, 0xa4,
    0x7a, 0x3c, 0x34, 0x9b, 0xe3, 0x7b, 0xa3, 0x97,
    0x28, 0x3, 0xf, 0x1, 0x42, 0xc9, 0x92, 0xa1,
    0x31, 0xbc, 0x15, 0xa, 0x8d, 0xf6, 0xe5, 0x5b,
    0x8, 0x1d, 0xf5, 0x80, 0x44, 0x72, 0x45, 0x12,
    0xd1, 0x6b, 0xfb, 0x98, 0xff, 0x49, 0x71, 0xc7,
    0x2, 0x83, 0x76, 0xd9, 0xa0, 0x65, 0x9e, 0xf9,
    0x59, 0x58, 0x8a, 0x8e, 0xea, 0x5e, 0xbe, 0x94,
    0x1b, 0xfd, 0x54, 0x21, 0x1f, 0x2b, 0x20, 0x3e,
    0xe1, 0xe0, 0xb0, 0xe8, 0x23, 0x48, 0x10, 0xf0,
    0x39, 0xf3, 0x66, 0x11, 0x6d, 0xc5, 0xb8, 0x6,
    0x18, 0x24, 0xd2, 0x5d, 0x6f, 0x70, 0xc8, 0xef,
    0xf7, 0x4a, 0x7f, 0xd4, 0xb5, 0x84, 0xd8, 0xb6,
    0xc0, 0x7c, 0xce, 0x46, 0x38, 0x91, 0xc2, 0x35,
    0x6a, 0x4e, 0x96, 0x73, 0x1c, 0xc1, 0x74, 0x1e
};

byte _decryption_array[256] = {
    0x32, 0x9b, 0xb8, 0x99, 0x7c, 0x82, 0xdf, 0x30,
    0xa8, 0x6, 0xa3, 0x80, 0x4d, 0x48, 0xa, 0x9a,
    0xd6, 0xdb, 0xaf, 0x81, 0xc, 0xa2, 0x85, 0x2e,
    0xe0, 0x2, 0x7, 0xc8, 0xfc, 0xa9, 0xff, 0xcc,
    0xce, 0xcb, 0x23, 0xd4, 0xe1, 0x18, 0x11, 0x21,
    0x98, 0x54, 0x1a, 0xcd, 0x19, 0x4a, 0x71, 0x34,
    0x3c, 0xa0, 0x7d, 0x13, 0x92, 0xf7, 0x4b, 0x2a,
    0xf4, 0xd8, 0x75, 0x33, 0x91, 0x2d, 0xcf, 0x83,
    0x79, 0x8b, 0x9c, 0x3a, 0xac, 0xae, 0xf3, 0x77,
    0xd5, 0xb5, 0xe9, 0x1f, 0x58, 0x43, 0xf9, 0x59,
    0x2c, 0x6a, 0x64, 0x49, 0xca, 0x7b, 0x5, 0x26,
    0xc1, 0xc0, 0x42, 0xa7, 0x5d, 0xe3, 0xc5, 0x9,
    0x7e, 0x22, 0x15, 0x5a, 0x5e, 0xbd, 0xda, 0x60,
    0x3f, 0x46, 0xf8, 0xb1, 0x4f, 0xdc, 0x20, 0xe4,
    0xe5, 0xb6, 0xad, 0xfb, 0xfe, 0xd, 0xba, 0x40,
    0x24, 0x6d, 0x90, 0x95, 0xf1, 0x55, 0x8a, 0xea,
    0xab, 0x73, 0x56, 0xb9, 0xed, 0x41, 0x65, 0x16,
    0x84, 0x87, 0xc2, 0x25, 0x69, 0xa4, 0xc3, 0x28,
    0x45, 0xf5, 0x9e, 0x44, 0xc7, 0x66, 0xfa, 0x97,
    0xb3, 0x10, 0x7f, 0x93, 0x1, 0xe, 0xbe, 0x8e,
    0xbc, 0x9f, 0x39, 0x96, 0x8f, 0x3b, 0x5b, 0x29,
    0x35, 0x62, 0x74, 0x76, 0x36, 0x67, 0x1b, 0x51,
    0xd2, 0x3e, 0x8d, 0x1c, 0x38, 0xec, 0xef, 0x5c,
    0xde, 0x78, 0x37, 0x52, 0xa1, 0x8, 0xc6, 0x89,
    0xf0, 0xfd, 0xf6, 0x2b, 0x17, 0xdd, 0x8c, 0xb7,
    0xe6, 0x9d, 0x1e, 0x50, 0x27, 0x0, 0xf2, 0x2f,
    0x6f, 0xb0, 0xe2, 0x57, 0xeb, 0xf, 0x1d, 0x6e,
    0xee, 0xbb, 0x3, 0x12, 0x68, 0x70, 0x63, 0x53,
    0xd1, 0xd0, 0x6b, 0x94, 0x14, 0xa6, 0x47, 0x86,
    0xd3, 0x61, 0xc4, 0x88, 0x7a, 0xb, 0x31, 0xe7,
    0xd7, 0x5f, 0x4c, 0xd9, 0x6c, 0xaa, 0xa5, 0xe8,
    0x4, 0xbf, 0x4e, 0xb2, 0x72, 0xc9, 0x3d, 0xb4
};

byte encrypt(byte b) {
    return _encryption_array[b];
}

void encrypt_array(byte* array, unsigned int length) {
    for (int i = 0; i < length; i++) {
        array[i] = _encryption_array[array[i]];
    }
}

byte decrypt(byte b) {
    return _decryption_array[b];
}

void decrypt_array(byte* array, unsigned int length) {
    for (int i = 0; i < length; i++) {
        array[i] = _decryption_array[array[i]];
    }
}

