#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <cctype>

#define BUFFER_SIZE 100
#define READ_END 0
#define WRITE_END 1

using namespace std;
int main(void)
{
  char write_msg[BUFFER_SIZE];
  char read_msg[BUFFER_SIZE];
  int pipe1[2], pipe2[2];
  pid_t pid;

  if(pipe(pipe1) == -1)
    {
      cout << "Pipe failed" << endl;
      return 1;
    }
  if(pipe(pipe2) == -1)
    {
      cout << "Pipe failed" << endl;
      return 1;
    }

  pid = fork();
  if(pid < 0)//error
    {
      cout << "Fork Failed" << endl;
      return 1;
    }
  if(pid > 0)//parent
    {
      string temp;
      close(pipe1[READ_END]);//close read
      close(pipe2[WRITE_END]);//close write
      
      //get string from user
      cout << "Enter text without numbers" << endl;
      getline (cin,temp);
      for(int i = 0; i < BUFFER_SIZE; i++)
	{
	  write_msg[i] = (char) 0;
	}
      for(int i = 0; i < temp.size(); i++)
	{
	  write_msg[i] = temp[i];
	}
      //send it to child
      write(pipe1[WRITE_END], write_msg, strlen(write_msg)+1);
      //read message from child
      read(pipe2[READ_END], read_msg, BUFFER_SIZE);
      //display changed text
      cout << "This is the changed text: " << read_msg << endl;
      
      //finish closing
      close(pipe1[WRITE_END]);//close write
      close(pipe2[READ_END]);//close read
    }
  else//child
    {
      close(pipe1[WRITE_END]);//close write
      close(pipe2[READ_END]);//close read
      //read from pipe
      read(pipe1[READ_END], read_msg, BUFFER_SIZE);
      //change the chars
      for(int i = 0; i < BUFFER_SIZE; i++)
	{
	  if((int)read_msg[i] ==  0)
	    {	   
	      write_msg[i] = (char) 0;
	    }
	  else  if(isupper(read_msg[i]))
	    {
	      write_msg[i] =(char)tolower(read_msg[i]);
	    }
	  else
	    {
	      write_msg[i] =(char)toupper(read_msg[i]);
	    }
	  if(write_msg[i] != 'z' && write_msg[i] != 'Z' && write_msg[i] != (char)0 && write_msg[i] != ' ')
	    {
	      write_msg[i] = (char)((int)write_msg[i]+1);
	    }
	}
      //write back to parent
      write(pipe2[WRITE_END], write_msg, strlen(write_msg)+1);

      //finish closing
      close(pipe1[READ_END]);//close read
      close(pipe2[WRITE_END]);//close write
    }
}
