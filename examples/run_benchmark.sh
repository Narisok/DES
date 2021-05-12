#!/bin/bash

bash ./examples/build_utility_prod.sh
echo -e "\033[32m\033[1mEncrypting...\033[0m"
echo -ne "TIME\tOpenSSL:" 
$(time cat my_file.txt |  openssl des-ecb -K 1234567812345678  > openssl_encrypt.enc)
echo -ne "\nTIME\tUtility:" 
$(time cat my_file.txt | ./Build/Examples/utility -K 1234567812345678 > util_encrypt.enc)

echo -e "\n\033[32m\033[1mDecrypting...\033[0m"
echo -ne "TIME\tOpenSSL:"
 $(time cat openssl_encrypt.enc |  openssl des-ecb -K 1234567812345678  > openssl_encrypt.dec)
echo -ne "\nTIME\tUtility:" 
$(time cat util_encrypt.enc | ./Build/Examples/utility -K 1234567812345678 > util_encrypt.dec)
