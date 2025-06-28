#!/bin/sh

DIR=$1

python3 -m http.server -d $1 8000
