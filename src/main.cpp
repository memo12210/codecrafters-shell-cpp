#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
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

    std::filesystem::path cmd_path,program_path;
    bool isDir = false;
    bool isExe = false;
    
    switch(string_to_command(input))
    {
    case echo:
      std::cout << input.substr(5) << '\n';
      break;
    case type:
    if(string_to_command(input.substr(5)) != invalid)
    {
      std::cout << input.substr(5) << " is a shell builtin" << '\n';
    }
    else 
    {
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
        std::cout << input.substr(5) << ": not found" << '\n';
      }
    }
    break;
    case quit:
      return 0;
    default:
      // may be external program with args
      std::stringstream ss(input);
      std::string temp;
      std::vector<std::string> vec;

      while(std::getline(ss,temp,' '))
      {
        vec.push_back(temp); // vec[0] is program name, vec[1] is program args.
      }

      program_path = vec[0];

      isExe = false;
      for(const auto& dir : dirs)
      {
        std::filesystem::path full_path = dir / program_path;
        if(std::filesystem::exists(full_path))
        {
          isExe = true;
          pid_t pid = fork();
          if(pid == -1)
          {
            std::cerr << "Error forking process!" << '\n';
            return 1;
          }

          if(pid == 0)
          {
            std::vector<const char*> args;
            for(const auto& arg : vec)
            {
              args.push_back(arg.c_str());
            }

            args.push_back(nullptr);

            if(execv(full_path.c_str(),const_cast<char* const*>(args.data())) == -1)
            {
              std::cerr << "Error executing program!" << '\n';
              return 1;
            }
          }

          else
          {
            // parent process
            int status;
            waitpid(pid,&status,0);
          }
          
          break;
        }
      }

      if(!isExe)
      {
        std::cout << input << ": command not found" << '\n';
      }
      break;
    }
  }
  
  return 0;
}