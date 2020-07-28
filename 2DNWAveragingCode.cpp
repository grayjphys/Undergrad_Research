#include <iostream>
#include <fstream>

using namespace std;

int main()
{
	ifstream infile;
	infile.open("2dout.txt");
	ofstream outfile;
	outfile.open("out.txt");

	double Corr[2][1001][50] = { 0 };
	double Corr2[2][1001][50] = { 0 };
	double Corr3[2][1001][50] = { 0 };

	int round, d;
	double corr, corr2, corr3, corr4, corr5, corr6;

	double Count[1001] = { 0 };

	infile >> round >> d >> corr >> corr2 >> corr3 >> corr4 >> corr5 >> corr6;

	while (infile)
	{
		Count[round] += 1 / 50.0;

		Corr[0][round][d] += corr;
		Corr2[0][round][d] += corr2;
		Corr3[0][round][d] += corr3;

		Corr[1][round][d] += corr4;
		Corr2[1][round][d] += corr5;
		Corr3[1][round][d] += corr6;

		infile >> round >> d >> corr >> corr2 >> corr3 >> corr4 >> corr5 >> corr6;
	}

	double avecorr;
	for (int i = 0; i <= 1000; i++)
	{
		for (int j = 0; j < 50; j++)
		{
			Corr[0][i][j] /= Count[i];
			Corr2[0][i][j] /= Count[i];
			Corr3[0][i][j] /= Count[i];

			Corr[1][i][j] /= Count[i];
			Corr2[1][i][j] /= Count[i];
			Corr3[1][i][j] /= Count[i];

			Corr[0][i][j] = Corr[0][i][j] - (Corr2[0][i][j] * Corr3[0][i][j]);
			Corr[1][i][j] = Corr[1][i][j] - (Corr2[1][i][j] * Corr3[1][i][j]);

			Corr[0][i][j] /= Corr[0][i][0];
			Corr[1][i][j] /= Corr[1][i][0];

			double avecorr = (Corr[0][i][j]+Corr[1][i][j]) / 2;
			outfile << j << "\t" << avecorr << endl;
		}
		outfile << endl;
	}
		infile.close();
		outfile.close();
		return 0;
	}