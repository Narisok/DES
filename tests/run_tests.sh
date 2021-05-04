#!/bin/bash

gcc -g tests/check_decryptor.c DES.c -I. -o tests/Build/check_decryptor.a
./tests/Build/check_decryptor.a