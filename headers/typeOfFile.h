#ifndef TYPEOFFILE_H
#define TYPEOFFILE_H
enum TYPE { READABLE, BINARY};
const char EXTENSIONES[][5]= 
{
 // READABLE
 {"html"},
 {"css"},
 {"js"},
 {"txt"},
 // BINARY
  // Audio
 {"mp3"},
  // Image
 {"jpg"},
 {"jpeg"},
 {"webp"},
 {"png"},
 {"gif"},
  // Video
 {"webm"},
 {"ogg"},
 {"mp4"},
 {"3gp"}
};
const int NUM_EXTENSIONES = 11;
TYPE Typeof(std::string extension)
{
 for(int i=0; i < NUM_EXTENSIONES; i++)
 {
  if(strcmp(extension.c_str(),EXTENSIONES[i]) == 0)
  {
   if(i >= 4) return BINARY;
  }
 }
 return READABLE;
}

#endif
