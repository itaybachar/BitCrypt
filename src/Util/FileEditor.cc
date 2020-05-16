#include "FileEditor.hh"

//File Reading
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

//Puts
#include <stdio.h>

FileEditor::FileEditor():
  fd(-1),
  size(0) {}

FileEditor::~FileEditor(){
  _closeFile();
}

//Loads a file and sets class variables
bool FileEditor::loadFile(const char filepath[]){
  //Close Old file if was open
  _closeFile();

  this->fd = open(filepath,O_RDWR);
  if(fd<0){
    puts("ERROR: Could not find file");
    return false;
  }

  struct stat buf;
  fstat(fd,&buf);
  this->size = buf.st_size;
  return true;
}

/*
  Reads the currently loaded file(if its size is more than 0)
  Returns number of bytes read of -1 if failed. 
*/
uint32_t FileEditor::readBytes(uint8_t* buf,uint32_t numBytes){
  //No file loaded 
  if(!this->size)
    return -1;    

  //lseek(this->fd,0,SEEK_SET);
  return read(this->fd,buf,numBytes);
}

//Getter Method for size, Also updates variable in case there was a size change
uint32_t FileEditor::fileSize(){
  //Update size
  struct stat buf;
  fstat(fd,&buf);
  this->size = buf.st_size;

  return this->size;
}

//Closes current file
bool FileEditor::_closeFile(){
  return close(fd);
}

uint32_t FileEditor::writeBytes(uint8_t* buf, uint32_t numBytes){
  if(this->fd<0)
    return -1;
  return write(this->fd,buf,numBytes);
}
