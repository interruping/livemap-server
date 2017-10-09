#!/bin/sh
echo "Generate private key..."
openssl genrsa -des3 -out server.key 1024 > /dev/null
echo "Generate certificate signing request..."
openssl req -new -key server.key -out server.csr
echo "Sign certificate with private key..."
openssl x509 -req -days 3650 -in server.csr -signkey server.key -out server.crt
echo "Generate dhparam file..."
openssl dhparam -out dh2048.pem 2048 > /dev/null
echo "Move all file."
mkdir ./build/debug
mkdir ./testclient/build/debug
mv server.key ./build/debug/
cp server.crt server.crt.cpy
mv server.crt ./build/debug/
mv server.crt.cpy ./testclient/build/debug/server.crt
mv dh2048.pem ./build/debug/
rm server.csr
echo "Done..."
