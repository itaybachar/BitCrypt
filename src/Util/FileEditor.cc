#include <FileEditor.hh>

//File control
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <cstring>

//String
#include <string>

#include <stdexcept>

//Check for O_BINARY, needed for Windows writing
#ifndef O_BINARY
#define O_BINARY 0
#endif

FileEditor::FileEditor(const char* filepath, bool write) :
  m_avail(0),
  m_filepath(filepath)
{
  //Open File
  if(write){
    m_flags = O_WRONLY | O_BINARY | O_CREAT;
    m_fd = open(m_filepath,m_flags,S_IRUSR | S_IWUSR);

    if(m_fd<0)
      throw std::runtime_error("Could not open file for writing!");

  }else{
    m_flags = O_RDONLY | O_BINARY;
    m_fd = open(m_filepath,m_flags);

    if(m_fd<0)
      throw std::runtime_error("Could not open file for reading!");

    //Prepare read buffer
    m_prebuf = new uint8_t[s_SIZE + s_PREBUFSIZE];
    m_buf = m_prebuf + s_PREBUFSIZE;
    m_cur = m_buf;
  }
}

FileEditor::~FileEditor(){
  //Close Current File
  if(m_fd>=0)
    close(m_fd);

  delete[] m_prebuf;
}

//Takes in the input file(to read), renames it to ***.tmp and creates 
//another FileEditor creating a new file with the original name for wiritng
FileEditor* FileEditor::prepareFile(){
  //Rename file
  std::string* temp = new std::string(m_filepath);
  *temp += ".tmp";

  close(m_fd);
  rename(m_filepath, temp->c_str());

  //Create new File Editor for writing
  FileEditor* writeFile = new FileEditor(m_filepath,true);

  //Update member variable
  m_filepath = temp->c_str();
  m_fd = open(m_filepath, m_flags);

  return writeFile;
}

//Reads the next chunk of data to the buffer
void FileEditor::readChunk(){
  //Copy excess bytes into prebuf
  for(uint32_t i = 0; i<m_avail;i++){
    m_prebuf[(s_PREBUFSIZE-m_avail)+i] = m_cur[i];
  }

  //load the next chunk into memory
  ssize_t bytesRead = read(m_fd,m_buf,s_SIZE);
  m_cur = m_buf-m_avail;
  m_avail += bytesRead;
}

//User read method using readChunk
//numBytes must be <= s_SIZE
ssize_t FileEditor::readBytes(uint8_t* buf, uint32_t numBytes){
  if(m_flags  == (O_RDONLY | O_BINARY) && numBytes <= s_SIZE){
    uint8_t* tmp = buf;
    ssize_t totalRead = 0;

    //If another chunk of data is needed
    if(numBytes>m_avail){
      memcpy(tmp,m_cur,m_avail);
      m_cur += m_avail;
      numBytes-=m_avail;
      tmp += m_avail;
      totalRead+=m_avail;
      m_avail = 0;
      readChunk();
    }

    //If still missing bytes(if eof) then return what is left
    if(numBytes>m_avail){
      memcpy(tmp,m_cur,m_avail);
      m_cur += m_avail;

      totalRead += m_avail;
      m_avail = 0;
      return totalRead;
    }

    //Finally, if there is enough data, copy
    memcpy(tmp,m_cur,numBytes);
    m_cur += numBytes;
    m_avail -= numBytes;
    totalRead +=numBytes;

    return totalRead;
  }
  return 0;
}

//User write method
ssize_t FileEditor::writeBytes(uint8_t* buf, uint32_t numBytes){
  if(m_flags & O_WRONLY){
    return write(m_fd,buf,numBytes);
  }
  return 0;
}

void FileEditor::skip(uint32_t num){
  if(m_flags  == (O_RDONLY | O_BINARY) && num <= s_SIZE){

    //If another chunk of data is needed
    if(num>m_avail)
      readChunk();

    if(num>m_avail){
      m_cur += m_avail;
      m_avail = 0;
      return;
    }
    m_cur += num;
    m_avail -= num;
  }
}

void FileEditor::deleteFile(){
  close(m_fd);
  remove(m_filepath);
}
