#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;

int main()
{
  string matrixName;
  ofstream file;
  int rows, cols;

  //get information from user
  cout << "Please enter the name of this matrix: ";
  cin >> matrixName;
  cout << "Please enter the number of rows for the matrix: ";
  cin >> rows;
  cout << "Please enter the number of columns for the matrix: ";
  cin >> cols;

  //make file
  matrixName = matrixName + ".txt";
  file.open(matrixName.c_str());

  //enter information into the file
  file << rows << endl;
  file << cols << endl;
  for(int i = 0; i < rows; i++)
    {
      for(int j = 0; j < cols; j++)
	{
	  file << rand() % 100 + 1 << " ";
	}
      file << endl;
    }

  //end
  file.close();
  return 0;
}
