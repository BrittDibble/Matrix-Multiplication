#include <iostream>
#include <fstream>
#include <string.h>
#include <pthread.h>
#include <stdio.h>

using namespace std;

//values that need to be seen by all threads
int aRows, aCols, bRows, bCols, numThreads, *aPointer, *bPointer, *cPointer;
void *runner(void *param);

//thread function
void *runner(void *param)
{
  int currentCol = (int)param;
  int *pointer, currentCell, sum, aValue, bValue;
  while(currentCol < bCols)
    {
      
      for(int j = 0; j < aRows; j++)
	{
	  //find what cell in cMatrix is being found
	  currentCell = currentCol + bCols * j;
	  sum = 0;
	  pointer = cPointer + currentCell;
	  for(int i = 0; i < aCols; i++)
            {
              aValue= *(aPointer + ((currentCell/bCols)*bRows+i));
              bValue= *(bPointer + ((currentCell%bCols)+ (i*bCols)));
              sum += aValue*bValue;
            }
	  *pointer = sum;
	}
      //reassign thread to work on next row
      currentCol += numThreads;
    }
  pointer = NULL;
  pthread_exit(0);
}

int main()
{
  string aName, bName, cName, line;
  ifstream aFile, bFile;
  ofstream cFile;
  int count, mode, numCores, *pointer;

  //get info for the matrices
  numCores = sysconf( _SC_NPROCESSORS_ONLN );
  cout << "Mode 0 uses as many threads as cores on the system." <<endl;
  cout << "Mode 1 uses twice as many threads as Mode 0." << endl;
  cout << "Run in Mode 0 or 1: ";
  cin >> mode;
  if(mode != 0 && mode != 1)
    {
      cout << "Invalid Mode. Program Quit" << endl;
      exit(0);
    }
  if(numCores <= 0)
    {
      cout << "Invalid number of cores. Defaulting to 1.";
      numCores = 1;
    }
  
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
      pointer = aPointer + count;
      *pointer = atoi(line.c_str());
      count ++;
    }
  count = 0;
  while(count < bRows*bCols)
    {
      bFile >> line;
      pointer = bPointer + count;
      *pointer = atoi(line.c_str());
      count ++;
    }

  //make the threads
  if(mode == 0)
    {
      numThreads = numCores;
    }
  else
    {
      numThreads = 2*numCores;
    }
  if(numThreads > bCols)
    {
      numThreads = bCols;
    }
  pthread_t workers[numThreads];
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  for(int i = 0; i < numThreads; i++)
    {
      pthread_create(&workers[i],&attr,runner,(void*)i);
    }

  //join the threads
  for(int i = 0; i < numThreads; i++)
    {
      pthread_join(workers[i],NULL);
    }

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
      pointer = cPointer + i;
      cFile << *pointer << " ";
    }

  //exit
  delete(aPointer);
  delete(bPointer);
  delete(cPointer);
  aPointer = NULL;
  bPointer = NULL;
  cPointer = NULL;
  pointer = NULL;
  aFile.close();
  bFile.close();
  cFile.close();

  return 0;
}
