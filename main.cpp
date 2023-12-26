#include <iostream>     
#include <sys/socket.h> // FOR socket, bind(), listen() AND accept()
#include <arpa/inet.h>  // FOR sockaddr_in AND htons()
#include <unistd.h>     // FOR close()
#include <cstring>      // FOR memset(), std::stoi() AND std::to_string()
#include <thread>       // FOR std::thread()
#include <fstream>      // FOR std::iostream AND std::ofstream
#include <sstream>      // FOR std::stringstream

typedef int SOCKET;
const int LIMIT_CON = 100; // Set connection limit

#include "headers/Error.h"
#include "headers/SetSocket.h"  // port 80
#include "headers/http(1.0)Protocol.h" // includes config.h AND typeOfFile.h


int main(int argc, char* argv[])
{
 SOCKET listener;                       // Server in listening
 sockaddr_in local;                     // Port and IP adress server
 sockaddr_in cliente;                   // Port and IP address client
 socklen_t localL = sizeof(local);      // Sizeof sockaddr_in server
 socklen_t clienteL = sizeof(cliente);  // Sizeof sockaddr_in client
 SOCKET nuevoSocket;                // Connection server
 char* cliente_ip;                  // IP address of the client
 int valorLectura;                  // Result of send()/recv()

try
{
/***  create the listening socket ***/
  if(!CrearSocket(listener))
   throw ERROR_CREAR_SOCKET;

/*** filling sockaddr_in ***/
  if(!AsignarIP(local))
   throw ERROR_IP_NO_VALIDA;

/*** bind the socket to sockaddr_in ***/
  if(Enlazar(listener,local,localL) != 0)
   throw ERROR_ENLAZANDO;

/*** putting the socket on listen ***/
  if(EnEscucha(listener,LIMIT_CON) != 0)
   throw ERROR_NO_ESCUCHA;

/*** waiting and accepting connections ***/
 bool keepAlive;
 int numberConnections = 0;
 char exit = '0';
 
 do{ 
    nuevoSocket = (EsperarConexion(listener, cliente, clienteL));
    if (nuevoSocket<=0)
     throw ERROR_CONEXION_RECHAZADA;
 
    /*** established conection ***/
    cliente_ip = inet_ntoa(cliente.sin_addr); // make the IP address readable
    std::cout<<"[CONNECTED]\n"
        <<"IP: "<<"<"<<cliente_ip<<">\n";

     numberConnections++;
    /***____ Client handler here___***/
     std::thread t(HTTP_PROTOCOL_,std::ref(nuevoSocket));
     t.detach();
      
   }while( numberConnections < LIMIT_CON);
}
  // CATCH FOR SOCKET ERRORS
  catch(Error& error)
  {
   std::cerr<<"Error: "<<error.msg()<<"\n";
   return -1;
  }
  // CATCH FOR STOI ERRORS
  catch(const std::invalid_argument& e) 
  {
    std::cerr<< "Error: Invalid arguments - " 
    <<e.what()<<"\n";
    return -2;
  }
  catch(const std::out_of_range& e) 
  {
   std::cerr << "Error: Overflow - " 
        << e.what()<<"\n";
   return -2;
  }
  // CATCH FOR BAD_ALLOC
  catch(const std::bad_alloc& e)
  {
   std::cerr << "Error: Bad allocation - "
        << e.what()<<"\n";
   return -3;
  }
/*** Close sockets***/ 
close(nuevoSocket);
close(listener);
return 0;
}
 





