#include <iostream>
#include <cmath>
#include <fstream> 
#include <string>
#include <iomanip>
#include <cmath>
#include "random.h"

const int numSpecies = 3;
const int multiplicity = 4;
const int L = numSpecies*multiplicity;
const double numBlocks = pow(multiplicity, 3);
const int spots = pow(L, 3);
const double Initial = (spots*1.0) / numSpecies;

using namespace std;

void printLattice(string Lattice[L][L][L], int x, int y, int z, ofstream &Output, int a, int b, int c,
	double& populationDensitya, double& populationDensityb, double& populationDensityc, int &Round);

void D3Lattice(string Lattice[L][L][L], int x, int y, int z, int& chosen, int& predator, int& prey,
	string chosenstring, string predatorstring, string preystring, double BirthRatechosen, double DeathRatechosen,
	double BirthRatepredator, double DeathRatepredator, double BirthRateprey, double DeathRateprey, double SwitchRate);

int main()

{
	//Running Average
	/*double avepopDAmultirun = 0;
	double avepopDBmultirun = 0;
	double avepopDCmultirun = 0;*/

	//Total Average
	/*
	double sumAmultirunDensity = 0;
	double sumBmultirunDensity = 0;
	double sumCmultirunDensity = 0;
	double aveAmultirunDensity;
	double aveBmultirunDensity;
	double aveCmultirunDensity;
	*/
	//Extinction Time
	double sumExtinctA = 0;
	double sumExtinctB = 0;
	double sumExtinctC = 0;
	double aveExtinctA;
	double aveExtinctB;
	double aveExtinctC;

	for (int run = 1; run <= 10; run++)
	{
		//This is to create a better seed based off of time. 
		//I will have to change it later to make it quicker but it works for now.

		clock_t t1, t2;
		t1 = clock();

		for (int i = 0; i < 1000000000; i++)
		{
			int x = 1;
		}

		t2 = clock();

		float diff = (((float)t2 - (float)t1) / 10000000.0F) * 10000;

		t1 += diff;

		time_t a_t = t1;

		unsigned long int seed = time(&a_t);

		init_genrand(seed);

		string Lattice[L][L][L];
		int x, y, z;
		int a = 0, b = 0, c = 0;
		int Round = 0;
		int TimeStamp = spots;

		double populationDensitya;
		double populationDensityb;
		double populationDensityc;

		double averageAdensity;
		double averageBdensity;
		double averageCdensity;
		double sumAdensity;
		double sumBdensity;
		double sumCdensity;

		double extinctionA = 0;
		double extinctionB = 0;
		double extinctionC = 0;

		//Rate of switching: 1 for now
		double SwitchRate = 1;
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
			for (y = 0; y < L; y++)
			{
				for (z = 0; z < L; z++)
				{
					Lattice[x][y][z] = "=";
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
						int p = genrand_int32() % 3;
						if (p == 0)
						{
							if (a < Initial)
							{
								Lattice[x][y][z] = "a";
								a = a++;
							}
						}
						else if (p == 1)
						{
							if (b < Initial)
							{
								Lattice[x][y][z] = "b";
								b = b++;
							}
						}
						else if (p == 2)
						{
							if (c < Initial)
							{
								Lattice[x][y][z] = "c";
								c = c++;
							}
						}
					}
				}
			}
		}
		//Population Phase Completed
		for (int i = 0; i < spots * 20; i++)
		{
			x = genrand_int32() % L;
			y = genrand_int32() % L;
			z = genrand_int32() % L;
			int xnew = genrand_int32() % L;
			int ynew = genrand_int32() % L;
			int znew = genrand_int32() % L;
			string t = Lattice[x][y][z];
			Lattice[x][y][z] = Lattice[xnew][ynew][znew];
			Lattice[xnew][ynew][znew] = t;
		}

		//Shuffling to mix Lattice 
		//printLattice(Lattice, x, y, z, OutFile, a, b, c,
		//populationDensitya, populationDensityb, populationDensityc, Round);

		//For total averages
		/*
		sumAdensity = populationDensitya;
		sumBdensity = populationDensityb;
		sumCdensity = populationDensityc;
		*/
		//For running averages
		/*
		double avepopDA = populationDensitya;
		double avepopDB = populationDensityb;
		double avepopDC = populationDensityc;
		*/

		/*
		OutFile << " " << avepopDA << " "
		<< avepopDB << " "<< avepopDC << endl;
		Round
		*/

		while (a < spots && b < spots && c < spots)
		{

			Round++;

			int x = genrand_int32() % L;
			int y = genrand_int32() % L;
			int z = genrand_int32() % L;

			if (Lattice[x][y][z] == "a")
			{
				D3Lattice(Lattice, x, y, z, a, c, b,
					"a", "c", "b", BirthRateA, DeathRateA,
					BirthRateC, DeathRateC, BirthRateB, DeathRateB, SwitchRate);
			}
			else if (Lattice[x][y][z] == "b")
			{
				D3Lattice(Lattice, x, y, z, b, a, c,
					"b", "a", "c", BirthRateB, DeathRateB,
					BirthRateA, DeathRateA, BirthRateC, DeathRateC, SwitchRate);
			}
			else if (Lattice[x][y][z] == "c")
			{
				D3Lattice(Lattice, x, y, z, c, b, a,
					"c", "b", "a", BirthRateC, DeathRateC,
					BirthRateB, DeathRateB, BirthRateA, DeathRateA, SwitchRate);
			}

			//printLattice(Lattice, x, y, z, OutFile, a, b, c,
			//populationDensitya, populationDensityb, populationDensityc, Round);

			//Total Average
			/*
			sumAdensity += populationDensitya;
			sumBdensity += populationDensityb;
			sumCdensity += populationDensityc;
			averageAdensity = sumAdensity / Round;
			averageBdensity = sumBdensity / Round;
			averageCdensity = sumCdensity / Round;
			*/

			//Running Average
			/*
			avepopDA = (avepopDA*(Round - 1) + populationDensitya) / Round;
			avepopDB = (avepopDB*(Round - 1) + populationDensityb) / Round;
			avepopDC = (avepopDC*(Round - 1) + populationDensityc) / Round;
			*/

			/*
			if (Round%spots == 0 || a == spots
			|| b == spots || c == spots)
			{
			OutFile << " "<< avepopDA << " "
			<< avepopDB << " "<< avepopDC << endl;
			}
			*/
			if (a == spots || b == spots || c == spots)
			{
				cout << (Round - (Round%spots)) / spots << ",";
			}

		}
		//Total Average

		/*
		OutFile << run << "\t" << averageAdensity << "\t" << averageBdensity << "\t" << averageCdensity << endl;
		sumAmultirunDensity += averageAdensity;
		sumBmultirunDensity += averageBdensity;
		sumCmultirunDensity += averageCdensity;
		*/

		//Running Average

		/*
		avepopDAmultirun = (avepopDAmultirun*(run - 1) + avepopDA) / run;
		avepopDBmultirun = (avepopDBmultirun*(run - 1) + avepopDB) / run;
		avepopDCmultirun = (avepopDCmultirun*(run - 1) + avepopDC) / run;

		OutFile << run << " "
		<< avepopDAmultirun << " "
		<< avepopDBmultirun << " "
		<< avepopDCmultirun << endl;
		*/

		/*sumExtinctA += extinctionA;
		sumExtinctB += extinctionB;
		sumExtinctC += extinctionC;

		aveExtinctA = sumExtinctA / run;
		aveExtinctB = sumExtinctB / run;
		aveExtinctC = sumExtinctC / run;*/
	}
	/*
	aveAmultirunDensity = sumAmultirunDensity / run;
	aveBmultirunDensity = sumBmultirunDensity / run;
	aveCmultirunDensity = sumCmultirunDensity / run;
	*/
	//OutFile << aveAmultirunDensity << "\t" << aveBmultirunDensity << "\t" << aveCmultirunDensity << endl;

	return 0;
}

void printLattice(string Lattice[L][L][L], int y, int z, int x, ofstream &OutFile, int a, int b, int c,
	double& populationDensitya, double& populationDensityb, double& populationDensityc, int &Round)
{
	/*double population = a + b + c;
	populationDensitya = a / population;
	populationDensityb = b / population;
	populationDensityc = c / population;
	*/
	/*if (Round == 0 ||Round%spots == 0 || a == spots
	|| b == spots || c == spots)
	{
	OutFile << "Time is: " << Round << endl;

	for (y = 0; y < L; y++)
	{
	for (z = 0; z < L; z++)
	{
	for (x = 0; x < L; x++)
	{
	OutFile<< Lattice[x][y][z];
	}OutFile << "|";
	}OutFile << endl;
	}
	OutFile << "a: " << a << endl << "b: " << b << endl << "c: " << c << endl;*/
	//<<"Population Density A: " << fixed << setprecision(12) << populationDensitya << endl <<
	//"Population Density B: " << fixed << setprecision(12) << populationDensityb << endl <<
	//"Population Density C: " << fixed << setprecision(12) << populationDensityc << endl;
	//}


}

void D3Lattice(string Lattice[L][L][L], int x, int y, int z, int& chosen, int& predator, int& prey,
	string chosenstring, string predatorstring, string preystring, double BirthRatechosen, double DeathRatechosen,
	double BirthRatepredator, double DeathRatepredator, double BirthRateprey, double DeathRateprey, double SwitchRate)
{

	int direction = genrand_int32() % 6;

	int Event = genrand_int32() % 2;

	double RandSwitch = genrand_real3();

	double RandDeath = genrand_real3();

	double RandBirth = genrand_real3();

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
}
