#ifndef HTTP10_H
#define HTTP10_H

const int KB = 1024;

void HTTP_PROTOCOL_(SOCKET& cliente);  // HTTP 1.0
 void MakeResponse(char buffer[], int length);
 struct headerLine;

void HTTP_PROTOCOL_(SOCKET& cliente)
{
 std::string response;                          // DOCUMENT
 char buffer[KB*8] = {0};                       // DOCUMENT_SIZE_MAX
 int lectura = recv(cliente, buffer, KB, 0);    // Message length
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
  MakeResponse(buffer, lectura);
  lectura=send(cliente,buffer,strlen(buffer),0);
  if(lectura <= 0)
  {
   std::cerr<<"Alert message not sended\n";
   return;
  }
}

 struct fragmentedHeader
 {
  std::string name;
  std::string value;
 };

 void MakeResponse(char buffer[], int length)
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
   switch(swapBool)
   {
    case 1:
     container[num].name+=basicHeader[num][i];
     break;
    case 0:
     container[num].value+=basicHeader[num][i];
     break;
   }
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
   name+=container[0].name[index];
  // If there is a file, open it
  if(name != "")
  {
   std::ifstream file(name.c_str());
   if(!file)
   {
    std::cerr<<"Requested file does not exist\n";
    std::string badResponse = "HTTP/1.0 404 Not Found\r\nContent-Length: 0\r\n\r\n";
    strcpy(buffer,badResponse.c_str()); 
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
   }
  }
 }
 else
 {
  std::cout<<"For now this server only accepts get requests.\n";
 }
  delete [] container;
  delete [] basicHeader;
  delete [] method; 
}

#endif
