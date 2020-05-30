#include <iostream>
#include "FileEditor2.hh"

int main(){
  
  FileEditor2 f1("/home/itay/Desktop/2020 W4 Crestron - Itay Bachar.pdf");
  auto f2 = f1.prepareFile();

  uint8_t* in = new uint8_t[FileEditor2::s_SIZE];

  ssize_t read = 0;
  while((read = f1.readBytes(in,FileEditor2::s_SIZE)) > 0){

    f2->writeBytes(in,read);

  }


  return 0;
}
