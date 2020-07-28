#include <iostream>
#include <fstream> 
#include <string>
#include <iomanip>
#include <cmath>
#include <vector>
#include "random.h"

const int numSpecies = 3;

using namespace std;


//void printLattice(string Lattice[L], int x, ofstream &outfile, int a, int b, int c,
//	int &round);


void D1Lattice(vector <string> &Lattice, int L, ofstream& outfile, int x, int& chosen, int& predator, int& prey,
	string chosenchar, string predatorchar, string preychar, double BirthRatechosen, double DeathRatechosen,
	double BirthRatepredator, double DeathRatepredator, double BirthRateprey, double DeathRateprey, double SwitchRate);


int main()

{
	ofstream outfile;
	outfile.open("1dout.txt");

	time_t a_t;
	unsigned long int seed = time(&a_t);

	//choosing species and direction
	init_genrand(seed);

	int multnum = 3;
	vector<double> fext(multnum);
	vector<double> sext(multnum);

	for (int multiplicity = 1; multiplicity <= multnum; multiplicity++)
	{
		int L = numSpecies*multiplicity;
		double Initial = (L*1.0) / numSpecies;

		int round = 0;
		double tau = L*1.0;

		int first = 1;

		double sumfirst = 0;
		double sumsecond = 0;

		int rnum = 1000;

		for (int run = 1; run <= rnum; run++)
		{
			vector<string> Lattice(L);
			int x;
			int a = 0, b = 0, c = 0;
			round = 0;
			first = 1;


			//Rate of switching: 1 for now
			double SwitchRate = 0;
			//Rate of eating: 1 for now
			double DeathRateA = 1;
			double DeathRateB = 1;
			double DeathRateC = 1;
			//Rate of reproducing: 1 for now
			double BirthRateA = 1;
			double BirthRateB = 1;
			double BirthRateC = 1;

			for (x = 0; x < L; x++)
			{
				Lattice[x] = "=";
			}

			for (x = 0; x < L; x++)
			{
				while (Lattice[x] == "=")
				{
					int p = genrand_int32() % 3;
					if (p == 0)
					{
						if (a < Initial)
						{
							Lattice[x] = "a";
							a = a++;
						}
					}
					else if (p == 1)
					{
						if (b < Initial)
						{
							Lattice[x] = "b";
							b = b++;
						}
					}
					else if (p == 2)
					{
						if (c < Initial)
						{
							Lattice[x] = "c";
							c = c++;
						}
					}
				}
			}
			//Population Phase Completed
			//Shuffling to mix Lattice 
			for (int i = 0; i < L * 20; i++)
			{
				x = genrand_int32() % L;
				int xnew = genrand_int32() % L;
				string t = Lattice[x];
				Lattice[x] = Lattice[xnew];
				Lattice[xnew] = t;
			}

			//printLattice(Lattice, outfile,x, outfile, a, b, c, round);

			while (a < L && b < L && c < L)
			{
				round++;
				int x = genrand_int32() % L;

				if (Lattice[x] == "a")
				{
					D1Lattice(Lattice, L, outfile, x, a, c, b,
						"a", "c", "b", BirthRateA, DeathRateA,
						BirthRateC, DeathRateC, BirthRateB, DeathRateB, SwitchRate);
				}
				else if (Lattice[x] == "b")
				{
					D1Lattice(Lattice, L, outfile, x, b, a, c,
						"b", "a", "c", BirthRateB, DeathRateB,
						BirthRateA, DeathRateA, BirthRateC, DeathRateC, SwitchRate);
				}
				else if (Lattice[x] == "c")
				{
					D1Lattice(Lattice, L, outfile, x, c, b, a,
						"c", "b", "a", BirthRateC, DeathRateC,
						BirthRateB, DeathRateB, BirthRateA, DeathRateA, SwitchRate);
				}

				if (a == 0 || b == 0 || c == 0)
				{
					if (first)
					{
						sumfirst += round / (rnum*1.0);
						first = 0;
					}
				}
				if (a == L || b == L || c == L)
				{
					sumsecond += round / (rnum*1.0);
				}
			}
		}

		fext[multiplicity - 1] = sumfirst / tau;
		sext[multiplicity - 1] = sumsecond / tau;
	}
	for (int i = 1; i <= multnum; i++)
	{
		outfile << i*numSpecies << "\t" << fext[i - 1] << endl;
	}
	outfile << endl;
	for (int i = 1; i <= multnum; i++)
	{
		outfile << i*numSpecies << "\t" << sext[i - 1] << endl;
	}
	outfile.close();

	return 0;
}


void D1Lattice(vector <string> &Lattice, int L, ofstream& outfile, int x, int & chosen, int& predator, int& prey,
	string chosenchar, string predatorchar, string preychar, double BirthRatechosen, double DeathRatechosen,
	double BirthRatepredator, double DeathRatepredator, double BirthRateprey, double DeathRateprey, double SwitchRate)
{

	int direction = genrand_int32() % 2;
	//choosing direction out of 2 (1 left, 0 right)

	double RandDeath = genrand_real3();

	if (direction == 0)//chosen direction right
	{

		if (Lattice[(x + 1) % L] == preychar)
		{
			if (prey > 0)
			{
				if (RandDeath <= DeathRateprey)
				{
					Lattice[(x + 1) % L] = chosenchar;
					prey--;
					chosen++;
				}
			}
		}
		else if (Lattice[(x + 1) % L] == predatorchar)
		{
			if (chosen > 0)
			{
				if (RandDeath <= DeathRatechosen)
				{
					Lattice[x] = predatorchar;
					chosen--;
					predator++;
				}
			}
		}
	}
	else if (direction == 1)//chosen direction left
	{
		if (Lattice[(x - 1 + L) % (L)] == preychar)
		{
			if (prey > 0)
			{
				if (RandDeath <= DeathRateprey)
				{
					Lattice[(x - 1 + L) % (L)] = chosenchar;
					prey--;
					chosen++;
				}
			}
		}
		else if (Lattice[(x - 1 + L) % (L)] == predatorchar)
		{
			if (chosen > 0)
			{
				if (RandDeath <= DeathRatechosen)
				{
					Lattice[x] = predatorchar;
					chosen--;
					predator++;
				}
			}
		}
	}
}
