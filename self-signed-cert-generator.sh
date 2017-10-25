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

mkdir ./build > /dev/null
mkdir ./build/debug > /dev/null
mkdir ./build/release > /dev/null
mkdir ./testclient/build > /dev/null
mkdir ./testclient/build/debug > /dev/null

cp server.key ./build/debug/  > /dev/null
cp server.key ./build/release/  > /dev/null
cp server.crt ./build/debug/  > /dev/null
cp server.crt ./build/release/  > /dev/null
cp server.crt ./testclient/build/debug/ > /dev/null
cp dh2048.pem ./build/debug/ > /dev/null
cp dh2048.pem ./build/release > /dev/null

rm server.csr > /dev/null
rm server.key > /dev/null
rm server.crt > /dev/null
rm dh2048.pem > /dev/null

echo "Done..."
