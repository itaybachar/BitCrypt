#include "FileEditor.hh"

//File Reading
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

//Puts
#include <stdio.h>

//Memcpy
#include <cstring>

//Check for O_BINARY, needed for Windows writing
#ifndef O_BINARY
  #define O_BINARY 0
#endif

FileEditor::FileEditor():
  fd(-1),
  size(0),
  loc(0),
  truncate(true),
  flags(0)
{}

FileEditor::~FileEditor(){
  closeFile();

  delete[] buf;
}

//Loads a file and sets class variables
bool FileEditor::loadFile(const char filepath[], bool truncate){
  //Close Old file if was open
  closeFile();

  this->flags = O_RDONLY | O_BINARY;
  this->truncate = truncate;
  this->fd = open(filepath,flags);
  this->filepath = filepath;
  if(fd<0){
    puts("ERROR: Could not find file");
    return false;
  }

  struct stat fileStats;
  fstat(fd,&fileStats);
  this->size = fileStats.st_size;
  this->buf = new uint8_t[size];

  avail = read(this->fd,this->buf,this->size);
  loc = 0;

  return true;
}

/*
   Reads the currently loaded file(if its size is more than 0)
   Returns number of bytes read of -1 if failed. 
 */
int32_t FileEditor::readBytes(uint8_t* buf,uint32_t numBytes){
  if(avail>=numBytes){
    memcpy(buf,&this->buf[loc],numBytes);
    avail-=numBytes;
    loc+=numBytes;

    return numBytes;
  } else return -1;
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
bool FileEditor::closeFile(){
  return close(this->fd);
}

int32_t FileEditor::writeBytes(uint8_t* buf, uint32_t numBytes){
  if(this->fd<0)
    return -1;

  if(this->flags == (O_RDONLY | O_BINARY)){
    closeFile();
    this->flags = O_WRONLY | O_BINARY;
    if(this->truncate)
      this->flags |= O_TRUNC;
    else this->flags |= O_APPEND;
    this->fd = open(this->filepath,this->flags);
  }

  uint32_t numWrite = write(this->fd,buf,numBytes);
  return numWrite;
}

uint32_t FileEditor::filePointerLoc(){
  return this->loc;
}

