# C-_Multithread_Web_Server
A basic http/1.0 implementation on C++

The Content of the website must be inside the website/ directory.
By default connecting to a servver without specifying a directory will loadan "index.html" within website/
To change the name of the main page of your website check config.ini and replace "index.html" by the new name


[ESPAÑOL]
---------
Una implementación básica del protocolo http/1.0 en C++

El contenido del sitio web debe estar dentro del directorio website/
Por defecto conectarse al servidor sin especificar un directorio cargará un archivo "index.html" dentro de website/
Para cambiar el nombre de la pagina principal de su sitio web revise config.ini y reemplace "index.html" por el nuevo nombre.


Requirements && Compilation
---------------------------
You need a C++ compiler, try:                                               
g++ main.cpp -o server.bin || clang++ -o server.bin


