!#/bin/bash

#Move Executable and change permissions
echo Install started...
chmod +x BitCrypt
sudo mv BitCrypt /usr/bin

#Move Icon file
sudo mv icon.xpm /usr/share/icons/BitCrypt

#Move .desktop file
mv bitcrypt.desktop /usr/share/applications

echo Finished
