#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include "random.h"

const int numSpecies = 4;
const int multiplicity =25;
const int L = (numSpecies*multiplicity);
const long spots = L*L;
const double Initial = (spots*1.0) / numSpecies;

using namespace std;

void D2Lattice(string Lattice[L][L], int x, int y, int& chosen, int& predator, int& prey,
	string chosenstring, string predatorstring, string preystring,
	double SwitchRate);

static void Correlation(string Lattice[L][L], string Lattice0[L][L], int x, int y, string chosenstring,string neutralstring, double& corr, double& corr2, double& corr3);

int main()

{

	ofstream OutFile;
	OutFile.open("2dout.txt");

	double Corr[2][101] = { 0 };
	double Corr2[2][101] = { 0 };
	double Corr3[2][101] = { 0 };

	int Count[101] = { 0 };

	long Round = 0;

	double corr = 0;
	double corr2 = 0;
	double corr3 = 0;

	double aveCorr = 0;

	for (int run = 1; run <= 100; run++)
	{
		clock_t t1;
		t1 = clock();

		time_t a_t = t1;

		unsigned long int seed = time(&a_t);

		init_genrand(seed);


		string Lattice[L][L];
		string Lattice0[L][L];
		int x, y;
		int a = 0, b = 0, c = 0, d = 0;

		Round = 0;

		//Correlation values

		corr = 0;
		corr2 = 0;
		corr3 = 0;

		aveCorr = 0;

		//Rate of switching: 1 for now
		double SwitchRate = 1;
		//Real Switch rate. Not half of this number. Only for neutral pairs.



		for (x = 0; x < L; x++)
		{
			for (y = 0; y < L; y++)
			{

				Lattice[x][y] = "=";
				Lattice0[x][y] = "=";

			}
		}

		for (x = 0; x < L; x++)
		{
			for (y = 0; y < L; y++)
			{
				while (Lattice[x][y] == "=")
				{
					int p = genrand_int32() % 4;
					if (p == 0)
					{
						if (a < Initial)
						{
							Lattice[x][y] = "a";
							Lattice0[x][y] = "a";
							a = a++;
						}
					}
					else if (p == 1)
					{
						if (b < Initial)
						{
							Lattice[x][y] = "b";
							Lattice0[x][y] = "b";
							b = b++;
						}
					}
					else if (p == 2)
					{
						if (c < Initial)
						{
							Lattice[x][y] = "c";
							Lattice0[x][y] = "c";
							c = c++;
						}
					}
					else if (p == 3)
					{
						if (d < Initial)
						{
							Lattice[x][y] = "d";
							Lattice0[x][y] = "d";
							d = d++;
						}
					}

				}
			}
		}

		for (int x = 0; x < L; x++)
		{
			for (int y = 0; y < L; y++)
			{

				Correlation(Lattice, Lattice0, x, y, "a","c", corr, corr2, corr3);
				Corr[0][Round / spots] += (corr / spots);
				Corr2[0][Round / spots] += (corr2 / spots);
				Corr3[0][Round / spots] += (corr3 / spots);

				Correlation(Lattice, Lattice0, x, y, "b","d", corr, corr2, corr3);
				Corr[1][Round / spots] += (corr / spots);
				Corr2[1][Round / spots] += (corr2 / spots);
				Corr3[1][Round / spots] += (corr3 / spots);

			}
		}

		Count[0] += 1;



		while (a < spots && b < spots && c < spots && d < spots && a + c < spots && b + d < spots && Round / spots < 100)
		{

			Round++;

			int x = genrand_int32() % L;
			int y = genrand_int32() % L;


			if (Lattice[x][y] == "a")
			{
				D2Lattice(Lattice, x, y, a, d, b,
					"a", "d", "b",
					SwitchRate);

			}
			else if (Lattice[x][y] == "b")
			{
				D2Lattice(Lattice, x, y, b, a, c,
					"b", "a", "c",
					SwitchRate);

			}
			else if (Lattice[x][y] == "c")
			{
				D2Lattice(Lattice, x, y, c, b, d,
					"c", "b", "d",
					SwitchRate);

			}
			else if (Lattice[x][y] == "d")
			{
				D2Lattice(Lattice, x, y, d, c, a,
					"d", "c", "a",
					SwitchRate);

			}


			if (Round%spots == 0)//timestamp
			{
				for (int x = 0; x < L; x++)
				{
					for (int y = 0; y < L; y++)
					{


						Correlation(Lattice, Lattice0, x, y, "a", "c", corr, corr2, corr3);
						Corr[0][Round / spots] += (corr / spots);
						Corr2[0][Round / spots] += (corr2 / spots);
						Corr3[0][Round / spots] += (corr3 / spots);

						Correlation(Lattice, Lattice0, x, y, "b", "d", corr, corr2, corr3);
						Corr[1][Round / spots] += (corr / spots);
						Corr2[1][Round / spots] += (corr2 / spots);
						Corr3[1][Round / spots] += (corr3 / spots);


					}
				}
				Count[Round / spots] += 1;
			}
		}

	}

	double autocorr;
	for (int j = 0; j <= 100; j++)
	{
		autocorr = 0;

		int num = Count[j];

			Corr[0][j] /= (num);
			Corr2[0][j] /= (num);
			Corr3[0][j] /= (num);

			autocorr += (Corr[0][j] - (Corr2[0][j] * Corr3[0][j]));

			Corr[1][j] /= (num);
			Corr2[1][j] /= (num);
			Corr3[1][j] /= (num);

			autocorr += (Corr[1][j] - (Corr2[1][j] * Corr3[1][j]));
		

		OutFile << j << "\t" << autocorr/2 << endl;

	}

	OutFile.close();

	return 0;
}

static void Correlation(string Lattice[L][L], string Lattice0[L][L], int x, int y, string chosenstring, string neutralstring,
	double& corr, double& corr2, double& corr3)
{
	corr = 0;
	corr2 = 0;
	corr3 = 0;

	if ((Lattice[x][y] == chosenstring && Lattice0[x][y] == chosenstring) || (Lattice[x][y] == chosenstring && Lattice0[x][y] == neutralstring)
		|| (Lattice[x][y] == neutralstring && Lattice0[x][y] == neutralstring) || (Lattice[x][y] == neutralstring && Lattice0[x][y] == chosenstring))
	{
		corr = 1;
	}
	if (Lattice0[x][y] == chosenstring || Lattice0[x][y] == neutralstring)
	{
		corr2 = 1;
	}
	if (Lattice[x][y] == chosenstring || Lattice[x][y] == neutralstring)
	{
		corr3 = 1;
	}

}

void D2Lattice(string Lattice[L][L], int x, int y, int& chosen, int& predator, int& prey,
	string chosenstring, string predatorstring, string preystring,
	double SwitchRate)
{

	int direction = genrand_int32() % 4;

	double RandSwitch = genrand_real3();


	if (direction == 0)//chosen direction right
	{
		if (Lattice[(x + 1) % L][y] == preystring)
		{
			Lattice[(x + 1) % L][y] = chosenstring;
			chosen++;
			prey--;
		}
		else if (Lattice[(x + 1) % L][y] == predatorstring)
		{
			Lattice[x][y] = predatorstring;
			predator++;
			chosen--;
		}
		else if (Lattice[(x + 1) % L][y] != predatorstring && Lattice[(x + 1) % L][y] != preystring && Lattice[(x + 1) % L][y] != chosenstring)
		{

			if (RandSwitch <= SwitchRate)
			{
				string temp = Lattice[x][y];
				Lattice[x][y] = Lattice[(x + 1) % L][y];
				Lattice[(x + 1) % L][y] = temp;
			}

		}
	}
	else if (direction == 1)//chosen direction up
	{
		if (Lattice[x][(y + 1) % L] == preystring)
		{
			Lattice[x][(y + 1) % L] = chosenstring;
			chosen++;
			prey--;
		}
		else if (Lattice[x][(y + 1) % L] == predatorstring)
		{
			Lattice[x][y] = predatorstring;
			predator++;
			chosen--;
		}
		else if (Lattice[x][(y + 1) % L] != predatorstring && Lattice[x][(y + 1) % L] != preystring && Lattice[x][(y + 1) % L] != chosenstring)
		{

			if (RandSwitch <= SwitchRate)
			{
				string temp = Lattice[x][y];
				Lattice[x][y] = Lattice[x][(y + 1) % L];
				Lattice[x][(y + 1) % L] = temp;
			}

		}
	}
	else if (direction == 2)//chosen direction left
	{
		if (Lattice[(x - 1 + L) % (L)][y] == preystring)
		{
			Lattice[(x - 1 + L) % (L)][y] = chosenstring;
			chosen++;
			prey--;
		}
		else if (Lattice[(x - 1 + L) % (L)][y] == predatorstring)
		{
			Lattice[x][y] = predatorstring;
			predator++;
			chosen--;
		}
		else if (Lattice[(x - 1 + L) % (L)][y] != predatorstring && Lattice[(x - 1 + L) % (L)][y] != preystring && Lattice[(x - 1 + L) % (L)][y] != chosenstring)
		{

			if (RandSwitch <= SwitchRate)
			{
				string temp = Lattice[x][y];
				Lattice[x][y] = Lattice[(x - 1 + L) % (L)][y];
				Lattice[(x - 1 + L) % (L)][y] = temp;
			}

		}
	}
	else if (direction == 3)//chosen direction down 
	{
		if (Lattice[x][(y - 1 + L) % L] == preystring)
		{
			Lattice[x][(y - 1 + L) % L] = chosenstring;
			chosen++;
			prey--;
		}
		else if (Lattice[x][(y - 1 + L) % L] == predatorstring)
		{
			Lattice[x][y] = predatorstring;
			predator++;
			chosen--;
		}
		else if (Lattice[x][(y - 1 + L) % L] != predatorstring && Lattice[x][(y - 1 + L) % L] != preystring && Lattice[x][(y - 1 + L) % L] != chosenstring)
		{

			if (RandSwitch <= SwitchRate)
			{
				string temp = Lattice[x][y];
				Lattice[x][y] = Lattice[x][(y - 1 + L) % L];
				Lattice[x][(y - 1 + L) % L] = temp;
			}

		}
	}
}