#!/bin/bash

set -e

PRIVATE_KEY=keys/private.pem
PUBLIC_KEY=keys/public.pem

openssl ecparam -genkey -name secp256k1 -noout -out $PRIVATE_KEY
openssl ec -in $PRIVATE_KEY -pubout -out $PUBLIC_KEY
