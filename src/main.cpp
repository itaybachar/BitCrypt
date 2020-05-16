#include <AES.hh>
#include <FileEditor.hh>

void AESTests(), FileTest();

int main(){
  FileTest();
  return 0;
}

void FileTest(){
  uint8_t buf[131940];
  const char file[] = "/home/itay/Desktop/hi.txt";
  char text[] = "GET WIPED SON!";
  FileEditor fedit;
  if(fedit.loadFile(file)){
    fedit.writeBytes((uint8_t*)text,14);
    uint32_t readNum = fedit.readBytes(buf,fedit.fileSize()); 
    for(uint32_t i = 0; i<readNum; i++){
      std::cout<<char(buf[i]);
    } 
    fedit.writeBytes((uint8_t*)text,14);
  }

}

void AESTests(){
#if 0
  //Test 128 bit
  AES a(128); 
  uint8_t key[] = {
    0x00, 0x01, 0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f
  };
  uint8_t keylen = 16;
#endif

#if 0
  //Test 196 bit
  AES a(196); 
  uint8_t key[] = {
    0x00, 0x01, 0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
    0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17
  };
  uint8_t keylen = 24;
#endif

#if 1
  //Test 196 bit
  AES a(256); 
  uint8_t key[] = {
    0x00, 0x01, 0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f, 0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f
  };
  uint8_t keylen = 32;
#endif
  uint32_t outlen = 0;

  std::string text = "Hello, My Name Is SJ!";
  //Encryption
  //auto encrypted = a.encrypt(input,16,key,keylen,&outlen);
  auto encrypted = a.encrypt(reinterpret_cast<uint8_t*>(text.data()),text.size(),key,keylen,&outlen);
  std::cout<<"Input String: " << text.c_str()<<'\n'<<std::endl;
  std::cout<<"Ecrypted: \n";
  std::cout<<std::hex<<std::internal<<std::setfill('0');
  for(int i = 0; i<outlen;i++)
    std::cout<<std::setw(2)<<unsigned(encrypted[i])<<' ';

  std::cout<<"\n\n";
  //Decryption
  auto decrypted = a.decrypt(encrypted,outlen,key,keylen,&outlen);

  std::cout<<"Decrypted: \n";
  //std::cout<<std::hex<<std::internal<<std::setfill('0');
  for(int i = 0; i<outlen;i++)
    std::cout<<char(decrypted[i]);
  //std::cout<<std::setw(2)<<unsigned(decrypted[i])<<' ';

  std::cout<<'\n';

}
