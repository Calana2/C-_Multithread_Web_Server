#ifndef MIMEOF_H
#define MIMEOF_H

std::string mimeOf(std::string extension){
// yes, i should be used a switch
 std::string type;
 if(extension == "mp3")
  type = "audio/mp3";      
 else if(extension == "jpg" || extension == "jpeg")
  type = "image/jpeg";
 else if(extension == "webp")
  type = "image/webp";
 else if(extension == "png")
  type = "image/png";
 else if(extension == "gif")
  type = "image/gif";
 else if(extension == "webm")
  type = "video/webm";
 else if(extension == "ogg")
  type = "video/ogg";
 else if(extension == "mp4")
  type = "video/mp4";
 else if(extension == "3gp")
  type = "video/3gpp";
 else if(extension == "html")
  type = "text/html";
 else if(extension == "css")
  type = "text/css";
 else if(extension == "js")
  type = "application/javascript";
 else
  type = "text/plain";
 return type;
}
#endif
