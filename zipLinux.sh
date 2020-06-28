#!/bin/bash

FILE=bin/unx/BitCrypt

if [ -f "$FILE" ]; then
	echo "Executable Found!"
	cp res/icon.xpm bin/unx
	cp res/unx/* bin/unx
	tar -czvf BitCrypt.tar -C bin unx
	cd bin/unx
	find . -maxdepth 1 ! -name 'BitCrypt' -exec rm -rf {} \;
else
	echo "Can't find linux executable, run make?"
fi
