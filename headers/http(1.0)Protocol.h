#ifndef HTTP10_H
#define HTTP10_H
#include "config.h"
#include "typeOfFile.h"

// Constants
const int KB = 1024;
const std::string badResponse = "HTTP/1.0 404 Not Found\r\nContent-Length: 0\r\n\r\n";
const std::string siteName = "website/" + getName();

// Prototypes
void HTTP_PROTOCOL_(SOCKET& cliente);  // HTTP 1.0
 int MakeResponse(char buffer[], int length, SOCKET& cliente);
 struct fragmentedHeader;
 void DeleteInfo(fragmentedHeader* container, std::string* basicHeader, char* method);

/*******************************************************************/
void HTTP_PROTOCOL_(SOCKET& cliente)
{
 std::string response;                          // DOCUMENT
 char buffer[KB*8] = {0};                       // DOCUMENT_SIZE_MAX
 int lectura = recv(cliente, buffer, KB*8, 0);    // Message length
 if(lectura <= 0)
 { 
  std::cerr<<"Alert: message not received \n";
  return;
 }
 // Showing request
  std::cout<<"REQUEST:\n"
      <<"********************************\n"
      <<buffer
      <<"********************************\n";
  // Answering to the client
  lectura = MakeResponse(buffer, lectura, cliente);
  if(lectura <= 0)
  {
   std::cerr<<"Alert message not sended\n";
   return;
  }
}
/*******************************************************************/
 struct fragmentedHeader
 {
  std::string name;
  std::string value;
 };

 void DeleteInfo(fragmentedHeader* container, std::string* basicHeader, char* method)
{
 delete [] container;
 delete [] basicHeader;
 delete [] method;
}
/*******************************************************************/
 int MakeResponse(char buffer[], int length, SOCKET& cliente)
{
 //***  Get headerLines ***/
 int hlNum = 0; 
 // Counting number of headerLines
  for(int charNow=0; charNow < length; charNow++)
  {
   if(buffer[charNow] == '\r' && buffer[charNow+1] == '\n')
    hlNum++;
  }
 // Getting
  std::string* basicHeader = new std::string[hlNum];
  for(int charNow=0, i=0; charNow < length; charNow++)
  {
   if(buffer[charNow] == '\r' && buffer[charNow+1] == '\n') 
   { 
     i++; 
     charNow+=2;
   }
   basicHeader[i]+=buffer[charNow];
  }

  fragmentedHeader* container = new fragmentedHeader[hlNum];
  
  bool swapBool = true; // true for name, false for value
  for(int i=0, num=0; num<hlNum-1; i++)
  {
   if(basicHeader[num][i] == '\0')
   {
    num++;
    i=0;
    swapBool=true;
   }
   if(swapBool && basicHeader[num][i]==':')
    swapBool=false;
   if(swapBool)
     container[num].name+=basicHeader[num][i];
   else 
    container[num].value+=basicHeader[num][i];
  }

  /*** Getting method ***/
  char* method = new char[10];
  std::string name="";
  int index;
  memset(method,0,10);

  for(index=0; container[0].name[index] != ' ' || container[0].name[index] == '\0'; index++)
   method[index]=container[0].name[index];
  index+=2;
 
 
 /*** Make Response ***/
 if(strcmp(method,"GET") == 0) 
 {
  for(;container[0].name[index] != ' ';index++)
//->   /* file name here! */
   name+=container[0].name[index];
   name = "website/" + name;
   std::string extension = name.substr(name.find('.')+1,name.length()-1);
   //Maybe i should put error handling over here
  
// For binary files
if(Typeof(extension) == BINARY)
  {
   std::ifstream file(name.c_str(),std::ios::binary); // open the file
   if(!file)
   {
    std::cerr<<"Requested file does not exist\n";
    strcpy(buffer,badResponse.c_str());
    /***/
    DeleteInfo(container, basicHeader, method);
    /***/
     /*->*/ return send(cliente,buffer,strlen(buffer),0);
   }
   file.seekg(0,std::ios::end);       // Move the pointer to the end
   std::streampos size = file.tellg(); // Get the pointer position (size)
   if(size == -1)
   {
    std::cerr<<"Error getting the size of the file\n";
    strcpy(buffer,badResponse.c_str());
    /***/
    DeleteInfo(container, basicHeader, method);
    /***/
    /*->*/ return send(cliente,buffer,strlen(buffer),0);
   }
   else
   {
    file.seekg(0,std::ios::beg); // Move the pointer to the beggining
    std::stringstream fileContainer;
    fileContainer << file.rdbuf(); // put the file inside
    std::string response = "HTTP/1.1 200 OK\r\nContent-Type: video/mp4\r\nContent-Length: "
                           + std::to_string(fileContainer.str().length()) + "\r\n\r\n"
                           + fileContainer.str();
    std::cout<<"Sending "+name+" to the client...\n"; 
    /***/
    DeleteInfo(container, basicHeader, method);
    /***/ 
    return send(cliente,response.c_str(),response.length(),0);
   }
  }

// For readable files
else
  {
   std::ifstream file;
   if(name == "website/")
    {file.open(siteName.c_str());}
   else
    {
     file.open(name.c_str());}
   if(!file)
   {
    std::cerr<<"Requested file does not exist\n";
    strcpy(buffer,badResponse.c_str());
  /***/
  DeleteInfo(container, basicHeader, method);
  /***/
    /*->*/ return send(cliente,buffer,strlen(buffer),0);
   }
   else
   {
    std::stringstream buff;
    buff<<file.rdbuf();      // Put the file inside buff
    std::string response = "HTTP/1.0 200 OK\r\nContent-Length: "
                           + std::to_string(buff.str().length())
                           + "\r\n\r\n"
                           + buff.str(); 
    strcpy(buffer,response.c_str()); 
    std::cout<<"Sending "+name+" to the client...\n"; 
  /***/
  DeleteInfo(container, basicHeader, method);
  /***/
    /*->*/ return send(cliente,buffer,strlen(buffer),0);
   }
  }
 }
 else
  return -1;
}
/*******************************************************************/

#endif
