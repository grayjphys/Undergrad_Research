#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include "random.h"

const int numSpecies = 4;
const int multiplicity = 25;
const int L = (numSpecies*multiplicity);
const long spots = L*L;
const double Initial = (spots*1.0) / numSpecies;

using namespace std;

void D2Lattice(string Lattice[L][L], int x, int y, int& chosen, int& predator, int& prey,
	string chosenstring, string predatorstring, string preystring, string neutralstring, double SwitchRate);

static void Correlation(string Lattice[L][L], int x1, int y1, int x2, int y2, string chosenstring, string neutralstring, double& corr, double& corr2, double& corr3);

int main()

{

	ofstream OutFile;
	OutFile.open("2dout.txt");


	clock_t t1;
	t1 = clock();

	time_t a_t = t1;

	unsigned long int seed = time(&a_t);

	init_genrand(seed);


	string Lattice[L][L];
	int x, y;
	int a = 0, b = 0, c = 0, d = 0;

	long Round = 0;

	//Correlation values
	double CorrAC = 0;
	double CorrBD = 0;
	double CorrAC2 = 0;
	double CorrBD2 = 0;
	double CorrAC3 = 0;
	double CorrBD3 = 0;

	//correlation values used in the function
	double corr = 0;
	double corr2 = 0;
	double corr3 = 0;

	//Rate of switching: 1 for now
	double SwitchRate = 1;

	//Setting up the lattice
	for (x = 0; x < L; x++)
	{
		for (y = 0; y < L; y++)
		{
			Lattice[x][y] = "=";
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
						a = a++;
					}
				}
				else if (p == 1)
				{
					if (b < Initial)
					{
						Lattice[x][y] = "b";
						b = b++;
					}
				}
				else if (p == 2)
				{
					if (c < Initial)
					{
						Lattice[x][y] = "c";
						c = c++;
					}
				}
				else if (p == 3)
				{
					if (d < Initial)
					{
						Lattice[x][y] = "d";
						d = d++;
					}
				}
			}
		}
	}

	//Correlation values for the initial system, CorrAC, BD, AC2... etc. are 0.5 initially
	for (int d = 0; d < (L / 2); d++)
	{
		CorrAC = 0;
		CorrBD = 0;
		CorrAC2 = 0;
		CorrBD2 = 0;
		CorrAC3 = 0;
		CorrBD3 = 0;


		for (int x1 = 0; x1 < L; x1++)
		{
			for (int y1 = 0; y1 < L; y1++)
			{

				int x2 = (x1 + d) % L;
				int y2 = y1;

				Correlation(Lattice, x1, y1, x2, y2, "a", "c", corr, corr2, corr3);
				CorrAC += (corr / (2 * spots));
				CorrAC2 += (corr2 / (2 * spots));
				CorrAC3 += (corr3 / (2 * spots));
				Correlation(Lattice, x1, y1, x2, y2, "b", "d", corr, corr2, corr3);
				CorrBD += (corr / (2 * spots));
				CorrBD2 += (corr2 / (2 * spots));
				CorrBD3 += (corr3 / (2 * spots));

				x2 = x1;
				y2 = (y1 + d) % L;

				Correlation(Lattice, x1, y1, x2, y2, "a", "c", corr, corr2, corr3);
				CorrAC += (corr / (2 * spots));
				CorrAC2 += (corr2 / (2 * spots));
				CorrAC3 += (corr3 / (2 * spots));
				Correlation(Lattice, x1, y1, x2, y2, "b", "d", corr, corr2, corr3);
				CorrBD += (corr / (2 * spots));
				CorrBD2 += (corr2 / (2 * spots));
				CorrBD3 += (corr3 / (2 * spots));

			}
		}

		OutFile << (Round / spots) << "\t" << d << "\t" << CorrAC << "\t" << CorrAC2 << "\t" << CorrAC3 << "\t" << CorrBD << "\t" << CorrBD2 << "\t" << CorrBD3 << endl;

	}

	//Allows to run until the timestamp = 1000, or domination occurs.
	while (a < spots && b < spots && c < spots && d < spots && a + c < spots && b + d < spots && Round / spots<1000)
	{

		Round++;

		int x = genrand_int32() % L;
		int y = genrand_int32() % L;


		if (Lattice[x][y] == "a")
		{
			D2Lattice(Lattice, x, y, a, d, b,
				"a", "d", "b", "c", SwitchRate);

		}
		else if (Lattice[x][y] == "b")
		{
			D2Lattice(Lattice, x, y, b, a, c,
				"b", "a", "c", "d", SwitchRate);

		}
		else if (Lattice[x][y] == "c")
		{
			D2Lattice(Lattice, x, y, c, b, d,
				"c", "b", "d", "a", SwitchRate);

		}
		else if (Lattice[x][y] == "d")
		{
			D2Lattice(Lattice, x, y, d, c, a,
				"d", "c", "a", "b", SwitchRate);

		}
		//Correlation values for later than 0 timestamps.
		if (Round % spots == 0)//timestamp
		{

			for (int d = 0; d < (L / 2); d++)
			{
				CorrAC = 0;
				CorrBD = 0;
				CorrAC2 = 0;
				CorrBD2 = 0;
				CorrAC3 = 0;
				CorrBD3 = 0;



				for (int x1 = 0; x1 < L; x1++)
				{
					for (int y1 = 0; y1 < L; y1++)
					{

						int x2 = (x1 + d) % L;
						int y2 = y1;

						Correlation(Lattice, x1, y1, x2, y2, "a", "c", corr, corr2, corr3);
						CorrAC += (corr / (2 * spots));
						CorrAC2 += (corr2 / (2 * spots));
						CorrAC3 += (corr3 / (2 * spots));
						Correlation(Lattice, x1, y1, x2, y2, "b", "d", corr, corr2, corr3);
						CorrBD += (corr / (2 * spots));
						CorrBD2 += (corr2 / (2 * spots));
						CorrBD3 += (corr3 / (2 * spots));

						x2 = x1;
						y2 = (y1 + d) % L;

						Correlation(Lattice, x1, y1, x2, y2, "a", "c", corr, corr2, corr3);
						CorrAC += (corr / (2 * spots));
						CorrAC2 += (corr2 / (2 * spots));
						CorrAC3 += (corr3 / (2 * spots));
						Correlation(Lattice, x1, y1, x2, y2, "b", "d", corr, corr2, corr3);
						CorrBD += (corr / (2 * spots));
						CorrBD2 += (corr2 / (2 * spots));
						CorrBD3 += (corr3 / (2 * spots));


					}
				}

				OutFile << (Round / spots) << "\t" << d << "\t" << CorrAC << "\t" << CorrAC2 << "\t" << CorrAC3 << "\t" << CorrBD << "\t" << CorrBD2 << "\t" << CorrBD3 << endl;

			}
		}
	}
	OutFile.close();
	return 0;
}

static void Correlation(string Lattice[L][L], int x1, int y1, int x2, int y2, string chosenstring, string neutralstring, double& corr, double& corr2, double& corr3)// 1/N sum(x, y, z){S(t,x,y,z)S(t,x+d,y+d,z+d)}- (1/N sum(x, y, z){S(t,x,y,z)})^2
{
	corr = 0;
	corr2 = 0;
	corr3 = 0;

	if ((Lattice[x1][y1] == chosenstring || Lattice[x1][y1] == neutralstring) && (Lattice[x2][y2] == chosenstring || Lattice[x2][y2] == neutralstring))
	{
		corr = 1;

	}
	if (Lattice[x1][y1] == chosenstring || Lattice[x1][y1] == neutralstring)
	{
		corr2 = 1;
	}
	if (Lattice[x2][y2] == chosenstring || Lattice[x2][y2] == neutralstring)
	{
		corr3 = 1;
	}
}

void D2Lattice(string Lattice[L][L], int x, int y, int& chosen, int& predator, int& prey,
	string chosenstring, string predatorstring, string preystring, string neutralstring,
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
		//If Lattice[direction chosen] is neutral to the chosen species
		else if (Lattice[(x + 1) % L][y] == neutralstring)
		{

			if (RandSwitch <= SwitchRate)
			{
				Lattice[x][y] = neutralstring;
				Lattice[(x + 1) % L][y] = chosenstring;
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
		//If Lattice[direction chosen] is neutral to the chosen species
		else if (Lattice[x][(y + 1) % L] == neutralstring)
		{

			if (RandSwitch <= SwitchRate)
			{
				Lattice[x][y] = neutralstring;
				Lattice[x][(y + 1) % L] = chosenstring;
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
		//If Lattice[direction chosen] is neutral to the chosen species
		else if (Lattice[(x - 1 + L) % (L)][y] == neutralstring)
		{

			if (RandSwitch <= SwitchRate)
			{
				Lattice[x][y] = neutralstring;
				Lattice[(x - 1 + L) % (L)][y] = chosenstring;
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
		//If Lattice[direction chosen] is neutral to the chosen species
		else if (Lattice[x][(y - 1 + L) % L] == neutralstring)
		{

			if (RandSwitch <= SwitchRate)
			{
				Lattice[x][y] = neutralstring;
				Lattice[x][(y - 1 + L) % L] = chosenstring;
			}

		}
	}
}