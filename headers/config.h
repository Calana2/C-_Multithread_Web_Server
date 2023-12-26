#ifndef CONFIG_H
#define CONFIG_H

struct configLine
{
 std::string prop;
 std::string value;
};

std::string getName()
{
 std::string line;
 configLine actual;
 std::ifstream config("config.ini");
 while(std::getline(config,line))
 {
  if(line[0]!='[')
  {
   actual.prop=line.substr(0,line.find('='));
   actual.value=line.substr(line.find('=') + 1,line.length()-line.find("=") - 1);
   if(strcmp(actual.prop.c_str(),"SiteName") == 0)
    return actual.value;
  } 
 }
 std::string name = "index.html";
 return name;
}
#endif
