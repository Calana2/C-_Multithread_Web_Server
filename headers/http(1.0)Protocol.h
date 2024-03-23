#ifndef HTTP10_H
#define HTTP10_H
#include "config.h"
#include "typeOfFile.h"
#include "mimeOf.h"    



// Constants
const int KB = 1024;
const std::string badResponse = "HTTP/1.0 404 Not Found\r\nContent-Length: 0\r\n\r\n";
const std::string siteName = std::filesystem::current_path().string()+"/website/" + getName();



// Prototypes
void HTTP_PROTOCOL_(SOCKET& cliente);                                                  // Protocol Wrapper 
 int MakeResponse(char buffer[], int length, SOCKET& cliente);                         // HTTP 1.0                        
 struct fragmentedHeader;                                                              // Header-lines in name-value format 
 void DeleteInfo(fragmentedHeader* container, std::string* basicHeader, char* method); // just for remove dynamic allocation



/*******************************************************************/
void HTTP_PROTOCOL_(SOCKET& cliente)
{
 std::string response;                            // DOCUMENT
 char buffer[KB*512] = {0};                       // DOCUMENT_SIZE_MAX
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



/*******************************************************************/
 struct fragmentedHeader
 {
  std::string name;
  std::string value;
 };
/*******************************************************************/



/*******************************************************************/
 void DeleteInfo(fragmentedHeader* container, std::string* basicHeader, char* method)
{
 delete [] container;
 delete [] basicHeader;
 delete [] method;
}
/*******************************************************************/



/*******************************************************************/
 int MakeResponse(char buffer[], int length, SOCKET& cliente)
{
 //***  Getting headerLines ***/
 int hlNum = 0; 

  // Counting number of headerLines
   for(int charNow=0; charNow < length; charNow++)
   {
    if(buffer[charNow] == '\r' && buffer[charNow+1] == '\n')
     hlNum++;
   }

  // Getting the headerLines has raw strings
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

   // Getting the headerLines has name-value format
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

  for(index=0; container[0].name[index] != ' ' || container[0].name[index] == '\0'; index++) {method[index]=container[0].name[index];}
  index+=2;
 
 
 /*** Make Response ***/
 if(strcmp(method,"GET") == 0) 
 {
  for(;container[0].name[index] != ' ';index++){
    name+=container[0].name[index];                                        // name of the requested file
  }
  name = std::filesystem::current_path().string()+"/website/" + name;       // adding the route

  // Getting the extension
  std::string extension; 
  int dotPos= name.find('.');
  if(dotPos != std::string::npos){
   extension = name.substr(dotPos+1);
  } else {
   if(name != std::filesystem::current_path().string()+"/website/"){
    extension = "html";  // Asumir que es html un archivo sin extension 
    name=name+"."+extension;
   }
  }
  std::cout<<name<<std::endl;

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
   file.seekg(0,std::ios::end);                 // Move the pointer to the end
   std::streampos size = file.tellg();          // Get the pointer position (size)
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
    std::string response = "HTTP/1.1 200 OK\r\nContent-Type: "                          // Content-Type
                           + mimeOf(extension)                                  // *Content-Type value                        
                           + "\r\nContent-Length: "                       // Content-Length 
                           + std::to_string(fileContainer.str().length()) // *Content-length value
                           + "\r\n\r\n"                                   // Empty line
                           + fileContainer.str();                         // *Body
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
   std::string tipo;
   if(name == std::filesystem::current_path().string()+"/website/"){
    file.open(siteName.c_str());
    tipo="text/html";
   } else {
     file.open(name.c_str());
     tipo=mimeOf(extension);
   }

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
    std::string response = "HTTP/1.0 200 OK\r\nContent-Type: "
                           + tipo
                           + "\r\nContent-Length: "
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
