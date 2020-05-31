#include <iostream>
#include "BitCrypt.hh"

int main(){
  BitCrypt b(AES_128);

  const char* file = "/home/itay/Desktop/2020 W4 Crestron - Itay Bachar.pdf";
  const char* file2 = "/home/itay/Desktop/2020 W4 Crestron - Itay Bachar.pdf.tmp";
  b.encryptFile(file,"hi",2);
  b.decryptFile(file,"hi",2);

  return 0;
}
