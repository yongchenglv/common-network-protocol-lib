#!/bin/sh

echo "aclocal --force"
aclocal --force
echo "autoconf  --force"
autoconf  --force
echo "autoheader --force"
autoheader --force
echo "automake --force --add-missing"
automake --force --add-missing
