#!/bin/bash

rm -f /tmp/core*

make clean
make
sudo make install
/home/y/bin/php wxwork_finance_sdk.php

ls -l /tmp/core*