#include <iostream>
#include <string>

enum commands
{
  echo,
  cd,
  quit,
  invalid
};

commands string_to_command(std::string str)
{
  if(str == "echo") return echo;
  if(str == "cd") return cd;
  if(str == "exit 0") return quit;

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
    case quit:
      return 0;
    default:
      std::cout << input << ": command not found" << '\n';
      break;
    }
  }

  return 0;
}