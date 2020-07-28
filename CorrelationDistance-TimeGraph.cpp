#include <iostream>
#include <fstream>

using namespace std;

int main()
{
	ifstream InFile;
	InFile.open("in.txt");
	ofstream OutFile;
	OutFile.open("out.dat");

	double corrtest = 0.15;
	double corrhigh, corrlow;// values of the correlation at integer distances
	int L = 25;// L = L/2, in this case L = 100.
	int d;
	double corr;
	double slope = 0, x = 0;
	//y-y_1=m(x-x_1)=mx-mx_1,y-y_1+mx_1=mx,(1/m)(y-y_1+mx_1)=x, where y = corrtest, y_1 = corrlow, corrhigh, m = slope, x_1 = i,i-1.


	int round = 0;
	InFile >> d >> corr;

	corrhigh = corr;
	int j = 0;
	for (int i = 1; i < L; i++)
	{

		InFile >> d >> corr;
		corrlow = corr;
		if (corrlow < corrtest && corrhigh > corrtest)// finds the interval where corrtest is between. 
		{
			slope = corrlow - corrhigh;//dx = 1;
			x = (1 / slope)*(corrtest - corrlow + (slope*i));// finds the value of the distance at corrtest
			j = i + 1;// j reads the rest of the rounds data without assaigning a value to x.
			i = L;// This ends the reading loop for the round.
		}
		else
		{
			corrhigh = corr;
			if (i == L-1 && (corrlow > corrtest) || (corrhigh < corrtest))
			{
				//x = 0;//If corrtest hasn't fit between any two values, this assigns x = 0 and gives a time where the curves go above corrtest.
					  // Any data after this point is excluded.
				continue;
			}

		}
		
	}
	for (j; j < L; j++)
	{
		InFile >> d >> corr;
	}

	OutFile << round*1 << "\t" << x <<  endl;

	round++;

	while (InFile)
	{
		
		InFile >> d >> corr;

		corrhigh = corr;
		int j = 0;
		for (int i = 1; i < L; i++)
		{

			InFile >> d >> corr;
			corrlow = corr;
			if (corrlow < corrtest && corrhigh > corrtest)
			{
				slope = corrlow - corrhigh;
				x = (1 / slope)*(corrtest - corrlow + (slope*i));
				j = i + 1;// j reads the rest of the rounds data without assaigning a value to x.
				i = L;// This ends the reading loop for the round.
			}
			else
			{
				corrhigh = corr;
				if (i == L-1 && (corrlow > corrtest) || (corrhigh < corrtest))
				{
					//x = 0;//If corrtest hasn't fit between any two values, this assigns x = 0 and gives a time where the curves go above corrtest.
					// Any data after this point is excluded.
					continue;
				}

			}
			
		}
		for (j; j < L; j++)
		{
			InFile >> d >> corr;
		}
		OutFile << round*1 << "\t" << x <<  endl;

		round++;

	}



	InFile.close();
	OutFile.close();

	return 0;
}