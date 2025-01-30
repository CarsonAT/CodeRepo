#!/bin/bash
if g++ --version >/dev/null 2>&1; then
    echo "g++ is properly installed"
else
    echo "g++ is not installed and will be"
    sudo apt install build-essentials
fi
sudo apt-get install sqlite3 libsqlite3-dev
echo "Installation finished"