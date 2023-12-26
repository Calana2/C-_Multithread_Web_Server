#ifndef ERRORCLASS_H
#define ERRORCLASS_H

class Error 
{
 private:
 char* mensaje;
 int sockNum;
 public:
 Error(const char msg[])
 {
  mensaje = new char[100];
  for(int i=0; msg[i] != '\0'; i++)
  {
   mensaje[i]=msg[i];
  }
 }
 Error(Error& otroError)
 {
  mensaje = new char[100];
  for(int i=0; otroError.mensaje[i] != '\0'; i++)
  {
   mensaje[i]=otroError.mensaje[i];
  }
 }
 char* msg()
 {
  return mensaje;
 }
 ~Error()
 {
  delete [] mensaje;
 }
};

Error ERROR_CREAR_SOCKET("Error al crear el socket");
Error ERROR_ENLAZANDO("Error al intentar enlazar el socket a una direccion IP y Puerto");
Error ERROR_NO_ESCUCHA("Error al intentar poner en escucha al servidor");
Error ERROR_IP_NO_VALIDA("La direccion IP seleccionada no es valida");
Error ERROR_CONEXION_RECHAZADA("La conexion no se establecio correctamente");


#endif
