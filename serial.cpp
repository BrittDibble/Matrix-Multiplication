#include <iostream>
#include <fstream>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <vector>

using namespace std;

//values that need to be seen by all threads
int aRows, aCols, bRows, bCols, *aPointer, *bPointer, *cPointer;
void *runner(void *param);

//thread function
void *runner(void *param)
{
  int sum, aValue, bValue, count, *pointer;
  count = aRows*bCols;
  for(int i = 0; i < count; i++)
    {
      pointer = cPointer + i;
      sum = 0;
      for(int j = 0; j < bRows; j++)
	{
	  aValue = *(aPointer + (i/bCols*bRows + j));
	  bValue = *(bPointer + (i%bCols+j*bCols));
	  sum += aValue * bValue;
	}
      *pointer = sum;
    }
  pointer = NULL;
  pthread_exit(0);
}

int main()
{
  string aName, bName, cName, line;
  ifstream aFile, bFile;
  ofstream cFile;
  int count, *pointer1;

  //get info for the matrices
  cout << "Enter name of matrix A: ";
  cin >> aName;
  cout <<"Enter name of matrix B: ";
  cin >> bName;
  cout << "Enter name of answer matrix: ";
  cin >> cName;

  //open files
  aName = aName + ".txt";
  bName = bName + ".txt";
  aFile.open(aName.c_str());
  bFile.open(bName.c_str());
  if(!aFile.is_open() || !bFile.is_open())
    {
      cout << "One or both of the matrix files does not exist. Program closing\
." << endl;
      exit(0);
    }

  //get values from files
  getline(aFile, line);
  aRows = atoi(line.c_str());
  getline(aFile, line);
  aCols = atoi(line.c_str());
  getline(bFile, line);
  bRows = atoi(line.c_str());
  getline(bFile, line);
  bCols = atoi(line.c_str());

  //check to see if multiplication is possible
  if(aCols != bRows)
    {
      cout << "Multiplication is not possible. Program closing." << endl;
      exit(0);
    }

  //get rest of values from files
  aPointer = new int[aRows*aCols];
  bPointer = new int[bRows*bCols];
  cPointer = new int[aRows*bCols];

  count = 0;
  while(count < aRows*aCols)
    {
      aFile >> line;
      pointer1 = aPointer + count;
      *pointer1 = atoi(line.c_str());
      count ++;
    }
  count = 0;
  while(count < bRows*bCols)
    {
      bFile >> line;
      pointer1 = bPointer + count;
      *pointer1 = atoi(line.c_str());
      count ++;
    }

  //make the thread
  pthread_t worker;
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_create(&worker,&attr,runner,0);

  //join the thread
    pthread_join(worker,NULL);

  //write out
  cName = cName + ".txt";
  cFile.open(cName.c_str());
  cFile << aRows << endl;
  cFile << bCols;
  for(int i = 0; i < aRows*bCols; i++)
    {
      if(i%bCols == 0)
        {
          cFile << endl;
        }
      pointer1 = cPointer + i;
      cFile << *pointer1 << " ";
    }

  //exit
  delete(aPointer);
  delete(bPointer);
  delete(cPointer);
  pointer1 = NULL;
  aPointer = NULL;
  bPointer = NULL;
  cPointer = NULL;
  
  aFile.close();
  bFile.close();
  cFile.close();
  return 0;
}
