#include <iostream>
#include <string>

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

int main()
{
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  
  std::string input = " ";
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
        std::cout << input.substr(5) << " is a shell builtin" << '\n';
      else
        std::cout << input.substr(5) << " not found" << '\n';
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