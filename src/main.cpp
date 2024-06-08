#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>

enum commands
{
  type,
  echo,
  cd,
  quit,
  invalid
};

commands string_to_command(std::string str)
{
  if(str.find("type")!= std::string::npos) return type;
  if(str.find("echo") != std::string::npos) return echo;
  if(str.find("cd") != std::string::npos) return cd;
  if(str.find("exit") != std::string::npos) return quit;
  
  return invalid;
}

std::vector<std::string> get_path_dirs()
{
  std::vector<std::string> dirs;
  char* path = getenv("PATH");
  if(path != nullptr)
  {
    std::string pathStr(path);
    size_t start = 0,end;
    while((end = pathStr.find(':',start) != std::string::npos))
    {
      dirs.push_back(pathStr.substr(start,end - start));
      start = end + 1;
    }
    dirs.push_back(pathStr.substr(start));
  }
  return dirs;
}

int main(int argc,char** argv)
{
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  
  std::string input = " ";

  std::vector<std::string> directories = get_path_dirs();
  
  while(!input.empty())
  {
    std::cout << "$ ";
    std::getline(std::cin, input);
    
    switch(string_to_command(input))
    {
    case echo:
      std::cout << input.substr(5) << '\n';
      break;
    case type:
      if(string_to_command(input.substr(5)) != invalid)
      {
        // shell builtin command
        std::string cmd_path = directories[0] + "/" + input.substr(5);
        if(std::errc::is_a_directory(cmd_path))
          std::cout << input.substr(5) << " is " << cmd_path << '\n';
        else
          std::cout << input.substr(5) << " not found" << '\n';
      }
      else
      {
        std::string cmd_path = directories[1] + "/" + input.substr(5);
        if(std::errc::is_a_directory(cmd_path))
          std::cout << input.substr(5) << " is " << cmd_path << '\n';
        else
          std::cout << input.substr(5) << ": command not found" << '\n';
      }
      break;
    case quit:
      return 0;
    default:
      std::cout << input << ": command not found" << '\n';
      break;
    }
  }
  
  return 0;
}