#include <fstream>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <math.h>

using namespace std;

int main()
{
	ifstream inFile;
	inFile.open("in.txt");

	ofstream OutFile;
	OutFile.open("out.txt");

	double array[1001] = {0};
	int count[1001] = { 0 };

	int num;
	double corr;

	inFile >> num >> corr;
	array[num/100] += corr;
	count[num/100] += 1;

	while (inFile)
	{
		inFile >> num >> corr;
		array[num/100] += corr;
		count[num/100] += 1;
	}
	for (int i = 0; i < 1001; i++)
	{
		array[i] /= count[i];
		OutFile << i*100 << "\t" << array[i] <<endl;
	}

	inFile.close();
	OutFile.close();

	return 0;
}