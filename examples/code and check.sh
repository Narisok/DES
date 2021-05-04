#!/bin/bash

bash ./examples/build_utility.sh
echo "Utility:" $(time echo $1 | ./Build/Examples/utility | ./Build/Examples/utility -d)
echo "OpenSSL:" $(time echo $1 | openssl des-ecb -K 1234567812345678 | openssl des-ecb -d -K 1234567812345678)