#include <iostream>
#include <string>

enum commands
{
  echo,
  cd,
  invalid
};

commands string_to_command(std::string str)
{
  if(str == "echo") return echo;
  if(str == "cd") return cd;

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

    if(input.empty()) break;
    
    if(string_to_command(input) == invalid)
      std::cout << input << ": command not found" << '\n'; 
  }

  return 0;
}