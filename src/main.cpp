#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <set>
#include <filesystem>

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

int main(int argc,char** argv)
{
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  
  std::string input = " ";

  std::string path(std::getenv("PATH"));
  std::stringstream ss(path);
  std::string temp;
  std::vector<std::string> paths;

  while(std::getline(ss,temp,':'))
  {
    paths.push_back(temp);
  }

  std::vector<std::filesystem::path> dirs;

  for(int i = 0; i<paths.size(); i++)
  {
    dirs.push_back(paths[i]);
  }
  
  std::set<commands> builtins;
  builtins.insert(type);
  builtins.insert(echo);
  builtins.insert(cd);
  builtins.insert(quit);

  while(!input.empty())
  {
    std::cout << "$ ";
    std::getline(std::cin, input);

    std::filesystem::path cmd_path;
    bool isDir = false;
    
    switch(string_to_command(input))
    {
    case echo:
      std::cout << input.substr(5) << '\n';
      break;
    case type:
    cmd_path = input.substr(5);
    isDir = false;
    for(const auto& dir : dirs)
    {
      std::filesystem::path full_path = dir / cmd_path;
      if(std::filesystem::exists(full_path))
      {
        std::cout << input.substr(5) << " is " << full_path.string() << '\n';
        isDir = true;
        break;
      }
    }
    if(!isDir)
    {
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