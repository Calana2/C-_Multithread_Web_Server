#ifndef SOCKETSET_H
#define SOCKETSET_H

// PREPARATION
bool CrearSocket(int& sockNum);
bool AsignarIP(sockaddr_in& sockDir, int& PUERTO);
bool Enlazar(int socket, struct sockaddr_in& sockDir, int sockDirSize);
bool EnEscucha(int& sockNum,int conMax);
int EsperarConexion(int& sockNum, sockaddr_in& sockDir,int sockDirSize);

/*******************************************************************/
bool CrearSocket(int& sockNum)
{
 sockNum = socket(AF_INET, SOCK_STREAM, 0);
 if(sockNum <= 0)
  return false;
 return true;
}
 
bool AsignarIP(sockaddr_in& sockDir)
{
 sockDir.sin_family=AF_INET;         // familia TCP/UDP
// if((inet_pton(AF_INET,INADDR_ANY,&sockDir.sin_addr)) <= 0)  // Asignar direccion IP de la tarjeta de red
//  return false;
 sockDir.sin_addr.s_addr = htonl(INADDR_ANY);
 sockDir.sin_port=htons(80);      
 return true;
}

bool Enlazar(int socket, struct sockaddr_in& sockDir, int sockDirSize)
{
  return bind(socket,(struct sockaddr*)&sockDir, sockDirSize);
}

bool EnEscucha(int& sockNum,int conMax)
{
 std::cout<<"Waiting for connections..."<<std::endl;
 return listen(sockNum,conMax);
}

int EsperarConexion(int& sockNum, sockaddr_in& sockDir,int sockDirSize)
{
 return accept(sockNum,(struct sockaddr*)&sockDir,(socklen_t*)&sockDirSize) ; 
}
/*******************************************************************/


#endif

