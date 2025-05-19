#!/bin/bash

set -e

dd if=/dev/urandom of=privateaes.bin bs=1 count=32
