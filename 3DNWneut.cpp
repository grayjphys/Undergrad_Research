#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include "random.h"

const int numSpecies = 4;
const int multiplicity = 12;
const int L = (numSpecies*multiplicity) + 2;
const long spots = L*L*L;
const double Initial = (spots*1.0) / numSpecies;

using namespace std;

void D3Lattice(string Lattice[L][L][L], int x, int y, int z, int& chosen, int& predator, int& prey,
	string chosenstring, string predatorstring, string preystring, string neutralstring, double SwitchRate);

void Correlation(string Lattice[L][L][L], string Lattice0[L][L][L], int x, int y, int z, string chosenstring, string neutralstring, double& corr, double& corr2, double& corr3);

int main()

{

	ofstream OutFile;
	OutFile.open("3dout.txt");

	double CorrAC = 0;
	double CorrAC2 = 0;
	double CorrAC3 = 0;

	double CorrBD = 0;
	double CorrBD2 = 0;
	double CorrBD3 = 0;

	long Round = 0;

	double corr = 0;
	double corr2 = 0;
	double corr3 = 0;

	double aveCorr = 0;

	clock_t t1;
	t1 = clock();

	time_t a_t = t1;

	unsigned long int seed = time(&a_t);

	init_genrand(seed);


	string Lattice[L][L][L];
	string Lattice0[L][L][L];
	int x, y, z;
	int a = 0, b = 0, c = 0, d = 0;

	//Rate of switching: 1 for now
	double SwitchRate = 1;

	for (x = 0; x < L; x++)
	{
		for (y = 0; y < L; y++)
		{
			for (z = 0; z < L; z++)
			{
				Lattice[x][y][z] = "=";
				Lattice0[x][y][z] = "=";
			}
		}
	}

	for (x = 0; x < L; x++)
	{
		for (y = 0; y < L; y++)
		{
			for (z = 0; z < L; z++)
			{
				while (Lattice[x][y][z] == "=")
				{
					int p = genrand_int32() % 4;
					if (p == 0)
					{
						if (a < Initial)
						{
							Lattice[x][y][z] = "a";
							Lattice0[x][y][z] = "a";
							a = a++;
						}
					}
					else if (p == 1)
					{
						if (b < Initial)
						{
							Lattice[x][y][z] = "b";
							Lattice0[x][y][z] = "b";
							b = b++;
						}
					}
					else if (p == 2)
					{
						if (c < Initial)
						{
							Lattice[x][y][z] = "c";
							Lattice0[x][y][z] = "c";
							c = c++;
						}
					}
					else if (p == 3)
					{
						if (d < Initial)
						{
							Lattice[x][y][z] = "d";
							Lattice0[x][y][z] = "d";
							d = d++;
						}
					}
				}
			}
		}
	}



	for (int x = 0; x < L; x++)
	{
		for (int y = 0; y < L; y++)
		{
			for (int z = 0; z < L; z++)
			{

				Correlation(Lattice, Lattice0, x, y, z, "a", "c", corr, corr2, corr3);
				CorrAC += (corr / spots);
				CorrAC2 += (corr2 / spots);
				CorrAC3 += (corr3 / spots);

				Correlation(Lattice, Lattice0, x, y, z, "b", "d", corr, corr2, corr3);
				CorrBD += (corr / spots);
				CorrBD2 += (corr2 / spots);
				CorrBD3 += (corr3 / spots);
			}
		}
	}

	cout << Round / spots << "\t" << CorrAC << "\t" << CorrAC2 << "\t" << CorrAC3 << "\t" << CorrBD << "\t" << CorrBD2 << "\t" << CorrBD3 << endl;

	while (a < spots && b < spots && c < spots && d < spots && a + c < spots && b + d < spots && Round / spots < 1000)
	{
		Round++;

		int x = genrand_int32() % L;
		int y = genrand_int32() % L;
		int z = genrand_int32() % L;

		if (Lattice[x][y][z] == "a")
		{
			D3Lattice(Lattice, x, y, z, a, d, b,
				"a", "d", "b", "c", SwitchRate);

		}
		else if (Lattice[x][y][z] == "b")
		{
			D3Lattice(Lattice, x, y, z, b, a, c,
				"b", "a", "c", "d", SwitchRate);

		}
		else if (Lattice[x][y][z] == "c")
		{
			D3Lattice(Lattice, x, y, z, c, b, d,
				"c", "b", "d", "a", SwitchRate);

		}
		else if (Lattice[x][y][z] == "d")
		{
			D3Lattice(Lattice, x, y, z, d, c, a,
				"d", "c", "a", "b", SwitchRate);

		}


		if (Round%spots == 0)//timestamp
		{
			CorrAC = 0;
			CorrAC2 = 0;
			CorrAC3 = 0;
			CorrBD = 0;
			CorrBD2 = 0;
			CorrBD3 = 0;

			for (int x = 0; x < L; x++)
			{
				for (int y = 0; y < L; y++)
				{
					for (int z = 0; z < L; z++)
					{

						Correlation(Lattice, Lattice0, x, y, z, "a", "c", corr, corr2, corr3);
						CorrAC += (corr / spots);
						CorrAC2 += (corr2 / spots);
						CorrAC3 += (corr3 / spots);

						Correlation(Lattice, Lattice0, x, y, z, "b", "d", corr, corr2, corr3);
						CorrBD += (corr / spots);
						CorrBD2 += (corr2 / spots);
						CorrBD3 += (corr3 / spots);
					}
				}
			}
			cout << Round / spots << "\t" << CorrAC << "\t" << CorrAC2 << "\t" << CorrAC3 << "\t" << CorrBD << "\t" << CorrBD2 << "\t" << CorrBD3 << endl;
		}

	}

	OutFile.close();

	return 0;
}

void Correlation(string Lattice[L][L][L], string Lattice0[L][L][L], int x, int y, int z, string chosenstring, string neutralstring,
	double& corr, double& corr2, double& corr3)
{
	corr = 0;
	corr2 = 0;
	corr3 = 0;

	if ((Lattice[x][y][z] == chosenstring || Lattice[x][y][z] == neutralstring) && (Lattice0[x][y][z] == chosenstring || Lattice0[x][y][z] == neutralstring))
	{
		corr = 1;
	}
	if (Lattice[x][y][z] == chosenstring || Lattice[x][y][z] == neutralstring)
	{
		corr2 = 1;
	}
	if (Lattice0[x][y][z] == chosenstring || Lattice0[x][y][z] == neutralstring)
	{
		corr3 = 1;
	}

}

void D3Lattice(string Lattice[L][L][L], int x, int y, int z, int& chosen, int& predator, int& prey,
	string chosenstring, string predatorstring, string preystring, string neutralstring, double SwitchRate)
{

	int direction = genrand_int32() % 6;

	double RandSwitch = genrand_real3();

	if (direction == 0)//chosen direction right
	{
		if (Lattice[(x + 1) % L][y][z] == preystring)
		{
			Lattice[(x + 1) % L][y][z] = chosenstring;
			chosen++;
			prey--;
		}
		else if (Lattice[(x + 1) % L][y][z] == predatorstring)
		{
			Lattice[x][y][z] = predatorstring;
			predator++;
			chosen--;
		}
		else if (Lattice[(x + 1) % L][y][z] == neutralstring)
		{
			if (RandSwitch <= SwitchRate)
			{
				Lattice[x][y][z] = neutralstring;
				Lattice[(x + 1) % L][y][z] = chosenstring;
			}
		}
	}
	else if (direction == 1)//chosen direction up
	{
		if (Lattice[x][(y + 1) % L][z] == preystring)
		{
			Lattice[x][(y + 1) % L][z] = chosenstring;
			chosen++;
			prey--;
		}
		else if (Lattice[x][(y + 1) % L][z] == predatorstring)
		{
			Lattice[x][y][z] = predatorstring;
			predator++;
			chosen--;
		}
		else if (Lattice[x][(y + 1) % L][z] == neutralstring)
		{

			if (RandSwitch <= SwitchRate)
			{
				Lattice[x][y][z] = neutralstring;
				Lattice[x][(y + 1) % L][z] = chosenstring;
			}

		}
	}
	else if (direction == 2)//chosen direction left
	{
		if (Lattice[(x - 1 + 2 * L) % (L)][y][z] == preystring)
		{
			Lattice[(x - 1 + 2 * L) % (L)][y][z] = chosenstring;
			chosen++;
			prey--;
		}
		else if (Lattice[(x - 1 + 2 * L) % (L)][y][z] == predatorstring)
		{
			Lattice[x][y][z] = predatorstring;
			predator++;
			chosen--;
		}
		else if (Lattice[(x - 1 + 2 * L) % (L)][y][z] == neutralstring)
		{

			if (RandSwitch <= SwitchRate)
			{
				Lattice[x][y][z] = neutralstring;
				Lattice[(x - 1 + 2 * L) % (L)][y][z] = chosenstring;
			}

		}
	}
	else if (direction == 3)//chosen direction down 
	{
		if (Lattice[x][(y - 1 + 2 * L) % L][z] == preystring)
		{
			Lattice[x][(y - 1 + 2 * L) % L][z] = chosenstring;
			chosen++;
			prey--;
		}
		else if (Lattice[x][(y - 1 + 2 * L) % L][z] == predatorstring)
		{
			Lattice[x][y][z] = predatorstring;
			predator++;
			chosen--;
		}
		else if (Lattice[x][(y - 1 + 2 * L) % L][z] == neutralstring)
		{

			if (RandSwitch <= SwitchRate)
			{
				Lattice[x][y][z] = neutralstring;
				Lattice[x][(y - 1 + 2 * L) % L][z] = chosenstring;
			}

		}
	}
	else if (direction == 4) //positive z
	{
		if (Lattice[x][y][(z + 1) % L] == preystring)
		{
			Lattice[x][y][(z + 1) % L] = chosenstring;
			chosen++;
			prey--;
		}
		else if (Lattice[x][y][(z + 1) % L] == predatorstring)
		{
			Lattice[x][y][z] = predatorstring;
			predator++;
			chosen--;
		}
		else if (Lattice[x][y][(z + 1) % L] == neutralstring)
		{

			if (RandSwitch <= SwitchRate)
			{
				Lattice[x][y][z] = neutralstring;
				Lattice[x][y][(z + 1) % L] = chosenstring;
			}

		}
	}
	else if (direction == 5)//  negative z
	{
		if (Lattice[x][y][(z - 1 + 2 * L) % (L)] == preystring)
		{
			Lattice[x][y][(z - 1 + 2 * L) % (L)] = chosenstring;
			chosen++;
			prey--;
		}
		else if (Lattice[x][y][(z - 1 + 2 * L) % (L)] == predatorstring)
		{
			Lattice[x][y][z] = predatorstring;
			predator++;
			chosen--;
		}
		else if (Lattice[x][y][(z - 1 + 2 * L) % (L)] == neutralstring)
		{

			if (RandSwitch <= SwitchRate)
			{
				Lattice[x][y][z] = neutralstring;
				Lattice[x][y][(z - 1 + 2 * L) % (L)] = chosenstring;
			}

		}
	}
}