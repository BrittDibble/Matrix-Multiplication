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
  int i, sum, aValue, bValue, *pointer;
  sum = 0;
  pointer = cPointer + (int)param; //points to the value of cMatrix being calcted
  for(i = 0; i < bRows; i++)
    {
      aValue= *(aPointer + (((int)param/bCols)*bRows+i));
      bValue= *(bPointer + (((int)param%bCols)+ (i*bCols)));
      sum += aValue*bValue;
    }
  *pointer = sum;
  pointer = NULL;
  pthread_exit(0);
}

int main()
{
  string aName, bName, cName, line;
  ifstream aFile, bFile;
  ofstream cFile;
  int count, *pointer;

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
  vector<pthread_t> workers;
  workers.resize(aRows*bCols);
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  int makeDo = 0;
  for(int i = 0; i < aRows*bCols; i++)
    {
      if(pthread_create(&workers[i],&attr,runner,(void*)i)!=0)
	{
	  for(int j = makeDo; j<i; j++)
	    {
	      pthread_join(workers[j],NULL);
	    }
	  makeDo = i;
	  pthread_create(&workers[i],&attr,runner,(void*)i);
	}
    }

  //join the threads
  for(int i = makeDo; i < aRows*bCols; i++)
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
  aFile.close();
  bFile.close();
  cFile.close();
  delete(aPointer);
  delete(bPointer);
  delete(cPointer);
  aPointer = NULL;
  bPointer = NULL;
  cPointer= NULL;
  pointer = NULL;

  return 0;
}
