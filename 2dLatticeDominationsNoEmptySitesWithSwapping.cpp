#include <iostream>
#include <fstream> 
#include <string>
#include <iomanip>
#include <cmath>
#include "random.h"

const int numSpecies = 3;
const int multiplicity = 4;
const int L = numSpecies*multiplicity;
const int spots = pow(L, 2);
const double Initial = (spots*1.0) / numSpecies;

using namespace std;


//void printLattice(string Lattice[L][L], int x, int y, ofstream &outfile, int a, int b, int c,
//	int &Round);


void D2Lattice(string Lattice[L][L], ofstream& outfile, int x, int y, int& chosen, int& predator, int& prey,
	string chosenchar, string predatorchar, string preychar, double BirthRatechosen, double DeathRatechosen,
	double BirthRatepredator, double DeathRatepredator, double BirthRateprey, double DeathRateprey, double SwitchRate);

int main()

{
	ofstream outfile;
	outfile.open("2dout.txt");

	
	time_t a_t;

	unsigned long int seed = time(&a_t);

	//choosing species and direction
	init_genrand(seed);

	double avepopDAmultirun = 0;
	double avepopDBmultirun = 0;
	double avepopDCmultirun = 0;

	//Extinction Time
	double sumExtinctA = 0;
	double sumExtinctB = 0;
	double sumExtinctC = 0;
	double aveExtinctA;
	double aveExtinctB;
	double aveExtinctC;

	for (int run = 1; run <= 100; run++)
	{
		string Lattice[L][L];
		int x, y;
		int a = 0, b = 0, c = 0;
		int Round = 0;
		int TimeStamp = spots;

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
				Lattice[x][y] = "=";
			}
		}
		//printLattice(Lattice,x, y, outfile, a, b, c, Round);

		for (x = 0; x < L; x++)
		{
			for (y = 0; y < L; y++)
			{
				while (Lattice[x][y] == "=")
				{
					int p = genrand_int32() % 3;
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
				}
			}
		}
		//Population Phase Completed
		for (int i = 0; i < L * 20; i++)
		{
			x = genrand_int32() % L;
			y = genrand_int32() % L;
			int xnew = genrand_int32() % L;
			int ynew = genrand_int32() % L;
			string t = Lattice[x][y];
			Lattice[x][y] = Lattice[xnew][ynew];
			Lattice[xnew][ynew] = t;
		}

		//Shuffling to mix Lattice 
		/*
		double population = a + b + c;
		double populationDensitya = a / population;
		double populationDensityb = b / population;
		double populationDensityc = c / population;*/
		/*double avepopDA = populationDensitya;
		double avepopDB = populationDensityb;
		double avepopDC = populationDensityc;
		*/
		//printLattice(Lattice, outfile,x, outfile, a, b, c, Round);

		while (a < spots && b < spots && c < spots)
		{

			Round++;

			int x = genrand_int32() % L;
			int y = genrand_int32() % L;

			if (Lattice[x][y] == "a")
			{
				D2Lattice(Lattice, outfile, x, y, a, c, b,
					"a", "c", "b", BirthRateA, DeathRateA,
					BirthRateC, DeathRateC, BirthRateB, DeathRateB, SwitchRate);
			}
			else if (Lattice[x][y] == "b")
			{
				D2Lattice(Lattice, outfile, x, y, b, a, c,
					"b", "a", "c", BirthRateB, DeathRateB,
					BirthRateA, DeathRateA, BirthRateC, DeathRateC, SwitchRate);
			}
			else if (Lattice[x][y] == "c")
			{
				D2Lattice(Lattice, outfile, x, y, c, b, a,
					"c", "b", "a", BirthRateC, DeathRateC,
					BirthRateB, DeathRateB, BirthRateA, DeathRateA, SwitchRate);
			}

			/*
			population = a + b + c;
			populationDensitya = a / population;
			populationDensityb = b / population;
			populationDensityc = c / population;

			avepopDA = (avepopDA*(Round - 1) + populationDensitya) / Round;
			avepopDB = (avepopDB*(Round - 1) + populationDensityb) / Round;
			avepopDC = (avepopDC*(Round - 1) + populationDensityc) / Round;
			*/
			//printLattice(Lattice, x, y, outfile, a, b, c, Round);
			if (a == spots || b == spots || c == spots)
			{
				cout << (Round - (Round%spots)) / spots << ",";
			}
		}
		/*
		avepopDAmultirun = (avepopDAmultirun*(run - 1) + avepopDA) / run;
		avepopDBmultirun = (avepopDBmultirun*(run - 1) + avepopDB) / run;
		avepopDCmultirun = (avepopDCmultirun*(run - 1) + avepopDC) / run;

		outfile << run << "\t"
		<< avepopDAmultirun << "\t"
		<< avepopDBmultirun << "\t"
		<< avepopDCmultirun << endl;
		*/
		sumExtinctA += extinctionA;
		sumExtinctB += extinctionB;
		sumExtinctC += extinctionC;

		aveExtinctA = sumExtinctA / run;
		aveExtinctB = sumExtinctB / run;
		aveExtinctC = sumExtinctC / run;

	}
	//outfile << L << (aveExtinctA + aveExtinctB + aveExtinctC) / 3;


	outfile.close();
	cin.get();
	return 0;
}

//void printLattice(string Lattice[L][L], int x, int y, ofstream &outfile, int a, int b, int c,
//	int &round)
//{
//
//	if (round == 0 || spots%spots == 0 || a == spots
//		|| b == spots || c == spots)
//	{
//		cout << "Time is: " << round << endl;
//
//		for (x = 0; x < L; x++)
//		{
//			for (y = 0; y < L; y++)
//			{
//				cout << Lattice[x][y];
//			}cout << endl;
//		}
//		cout << "a: " << a << endl << "b: " << b << endl << "c: " << c << endl;
//
//	}
//}


	void D2Lattice(string Lattice[L][L], ofstream& outfile, int x, int y, int& chosen, int& predator, int& prey,
		string chosenchar, string predatorchar, string preychar, double BirthRatechosen, double DeathRatechosen,
		double BirthRatepredator, double DeathRatepredator, double BirthRateprey, double DeathRateprey, double SwitchRate)
	{

		int direction = genrand_int32() % 4;
		//choosing direction out of 2 (1 left, 0 right)

		int Event = genrand_int32() % 2;
		//chance of event (<= 0.5 switching, <= 0.5 death or birth in the case of species vs empty spot)

		double RandSwitch = genrand_real3();
		//Rate of switching: 1 for now

		double RandDeath = genrand_real3();

		double RandBirth = genrand_real3();
		if (direction == 0)//chosen direction right
		{

			if (Lattice[(x + 1) % L][y] == preychar)
			{
				if (Event == 0)
				{
					if (RandSwitch <= SwitchRate)
					{
						Lattice[x][y] = preychar;
						Lattice[(x + 1) % L][y] = chosenchar;
					}
				}
				else if (Event == 1 &&prey>0)
				{
					if (RandDeath <= DeathRateprey)
					{
						Lattice[(x + 1) % L][y] = chosenchar;
						prey--;
						chosen++;
					}
				}
			}
			else if (Lattice[(x + 1) % L][y] == predatorchar)
			{
				if (Event == 0)
				{
					if (RandSwitch <= SwitchRate)
					{
						Lattice[x][y] = predatorchar;
						Lattice[(x + 1) % L][y] = chosenchar;
					}
				}
				else if (Event == 1 && chosen > 0)
				{
					if (RandDeath <= DeathRatechosen)
					{
						Lattice[x][y] = predatorchar;
						chosen--;
						predator++;
					}
				}
			}
		}
		else if (direction == 1)//chosen direction left
		{

			if (Lattice[(x - 1 + 2 * L) % (L)][y] == preychar)
			{
				if (Event == 0)
				{
					if (RandSwitch <= SwitchRate)
					{
						Lattice[x][y] = preychar;
						Lattice[(x - 1 + 2 * L) % (L)][y] = chosenchar;
					}
				}
				else if (Event == 1 && prey > 0)
				{
					if (RandDeath <= DeathRateprey)
					{
						Lattice[(x - 1 + 2 * L) % (L)][y] = chosenchar;;
						prey--;
						chosen++;
					}
				}
			}
			else if (Lattice[(x - 1 + 2 * L) % (L)][y] == predatorchar)
			{
				if (Event == 0)
				{
					if (RandSwitch <= SwitchRate)
					{
						Lattice[x][y] = predatorchar;
						Lattice[(x - 1 + 2 * L) % (L)][y] = chosenchar;
					}
				}
				else if (Event == 1 && chosen > 0)
				{
					if (RandDeath <= DeathRatechosen)
					{
						Lattice[x][y] = predatorchar;
						chosen--;
						predator++;
					}
				}
			}
		}
		else if (direction == 2)//chosen direction up
		{

			if (Lattice[x][(y + 1) % L] == preychar)
			{
				if (Event == 0)
				{
					if (RandSwitch <= SwitchRate)
					{
						Lattice[x][y] = preychar;
						Lattice[x][(y + 1) % L] = chosenchar;
					}
				}
				else if (Event == 1 && prey > 0)
				{
					if (RandDeath <= DeathRateprey)
					{
						Lattice[x][(y + 1) % L] = chosenchar;
						prey--;
						chosen++;
					}
				}
			}
			else if (Lattice[x][(y + 1) % L] == predatorchar)
			{
				if (Event == 0)
				{
					if (RandSwitch <= SwitchRate)
					{
						Lattice[x][y] = predatorchar;
						Lattice[x][(y + 1) % L] = chosenchar;
					}
				}
				else if (Event == 1 && chosen > 0)
				{
					if (RandDeath <= DeathRatechosen)
					{
						Lattice[x][y] = predatorchar;
						chosen--;
						predator++;
					}
				}
			}
		}
		else if (direction == 3)//chosen direction down
		{

			if (Lattice[x][(y - 1 + 2 * L) % (L)] == preychar)
			{
				if (Event == 0)
				{
					if (RandSwitch <= SwitchRate)
					{
						Lattice[x][y] = preychar;
						Lattice[x][(y - 1 + 2 * L) % (L)] = chosenchar;
					}
				}
				else if (Event == 1 && prey > 0)
				{
					if (RandDeath <= DeathRateprey)
					{
						Lattice[x][(y - 1 + 2 * L) % (L)] = chosenchar;;
						prey--;
						chosen++;
					}
				}
			}
			else if (Lattice[x][(y - 1 + 2 * L) % (L)] == predatorchar)
			{
				if (Event == 0)
				{
					if (RandSwitch <= SwitchRate)
					{
						Lattice[x][y] = predatorchar;
						Lattice[x][(y - 1 + 2 * L) % (L)] = chosenchar;
					}
				}
				else if (Event == 1 && chosen > 0)
				{
					if (RandDeath <= DeathRatechosen)
					{
						Lattice[x][y] = predatorchar;
						chosen--;
						predator++;
					}
				}
			}
		}
	}
