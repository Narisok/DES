#!/bin/bash

bash ./examples/build_utility_prod.sh

echo "OpenSSL:" $(time cat my_file.txt |  openssl des-ecb -K 1234567812345678  > openssl_encrypt.enc)
echo "Utility:" $(time cat my_file.txt | ./Build/Examples/utility > util_encrypt.enc)

# echo "Utility:" $(time echo $1 | ./Build/Examples/utility | ./Build/Examples/utility -d)
# echo "OpenSSL:" $(time echo $1 | openssl des-ecb -K 1234567812345678 | openssl des-ecb -d -K 1234567812345678)
