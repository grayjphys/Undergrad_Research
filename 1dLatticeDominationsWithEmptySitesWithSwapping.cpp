#include <iostream>
#include <fstream> 
#include <string>
#include <iomanip>
#include <cmath>
#include "random.h"

const int numSpecies = 3;
const int multiplicity = 4;
const int L = numSpecies*multiplicity;
const double Initial = (L*1.0) / numSpecies;

using namespace std;


void D1Lattice(string Lattice[L], ofstream& outfile,int x, int& chosen, int& predator, int& prey,
	string chosenchar, string predatorchar, string preychar, double BirthRatechosen, double DeathRatechosen,
	double BirthRatepredator, double DeathRatepredator, double BirthRateprey, double DeathRateprey, double SwitchRate);

void D1Empty(string Lattice[L], ofstream& outfile, int x, int direction, int& chosen,
	string chosenchar, double BirthRatechosen, double SwitchRate);


int main()

{
	ofstream outfile;
	outfile.open("1dout.txt");

	time_t a_t;
	unsigned long int seed = time(&a_t);

	//choosing species and direction
	init_genrand(seed);
	
	//Extinction Time
	double sumExtinctA = 0;
	double sumExtinctB = 0;
	double sumExtinctC = 0;
	double aveExtinctA;
	double aveExtinctB;
	double aveExtinctC;

	for (int run = 1; run <= 100; run++)
	{
		string Lattice[L];
		int x;
		int a = 0, b = 0, c = 0;
		int Round = 0;
		int TimeStamp = L;

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
		for (int i = 0; i < L * 20; i++)
		{
			x = genrand_int32()%L;
			int xnew = genrand_int32()%L;
			string t = Lattice[x];
			Lattice[x] = Lattice[xnew];
			Lattice[xnew] = t;
		}

		//Shuffling to mix Lattice 
		
		while (a < L && b < L && c < L)
		{

			Round++;

			int x = genrand_int32()%L;
			
			if (Lattice[x] == "a")
			{
				D1Lattice(Lattice, outfile,x, a, c, b,
					"a", "c", "b", BirthRateA, DeathRateA,
					BirthRateC, DeathRateC, BirthRateB, DeathRateB, SwitchRate);
			}
			else if (Lattice[x] == "b")
			{
				D1Lattice(Lattice, outfile,x, b, a, c,
					"b", "a", "c", BirthRateB, DeathRateB,
					BirthRateA, DeathRateA, BirthRateC, DeathRateC, SwitchRate);
			}
			else if (Lattice[x] == "c")
			{
				D1Lattice(Lattice, outfile,x, c, b, a,
					"c", "b", "a", BirthRateC, DeathRateC,
					BirthRateB, DeathRateB, BirthRateA, DeathRateA, SwitchRate);
			}
			else if (Lattice[x] == "0")
			{
				int direction = genrand_int32() % 2;
				//choosing direction out of 2 (1 left, 0 right)
				D1Empty(Lattice, outfile, x, direction, a,
					"a", BirthRateA, SwitchRate);
				D1Empty(Lattice, outfile, x,direction, c,
					"c", BirthRateC, SwitchRate);
				D1Empty(Lattice, outfile, x, direction, b,
					"b", BirthRateB, SwitchRate);
			}
			
			if (a == L || b == L || c == L)
			{
				cout << (Round - (Round%L))/L << ",";
			}
		}
		
		sumExtinctA += extinctionA;
		sumExtinctB += extinctionB;
		sumExtinctC += extinctionC;

		aveExtinctA = sumExtinctA / run;
		aveExtinctB = sumExtinctB / run;
		aveExtinctC = sumExtinctC / run;

	}


	outfile.close();

	return 0;
}


void D1Lattice(string Lattice[L], ofstream& outfile, int x, int& chosen, int& predator, int& prey,
	string chosenchar, string predatorchar, string preychar, double BirthRatechosen, double DeathRatechosen,
	double BirthRatepredator, double DeathRatepredator, double BirthRateprey, double DeathRateprey, double SwitchRate)
{

	int direction = genrand_int32() % 2;
	//choosing direction out of 2 (1 left, 0 right)

	int Event = genrand_int32() % 2;
	//chance of event (<= 0.5 switching, <= 0.5 death or birth in the case of species vs empty spot)

	double RandSwitch = genrand_real3();
	//Rate of switching: 1 for now

	double RandDeath = genrand_real3();

	double RandBirth = genrand_real3();
	if (direction == 0)//chosen direction right
	{
		
		if (Lattice[(x + 1) % L] == preychar)
		{
			if (Event == 0)
			{
				if (RandSwitch <= SwitchRate)
				{
					Lattice[x] = preychar;
					Lattice[(x + 1) % L] = chosenchar;
				}
			}
			else if (Event == 1 && prey > 0)
			{
				if (RandDeath <= DeathRateprey)
				{
					Lattice[(x + 1) % L] = "0";
					prey--;
				}
			}
		}
		else if (Lattice[(x + 1) % L] == predatorchar)
		{
			if (Event == 0)
			{
				if (RandSwitch <= SwitchRate)
				{
					Lattice[x] = predatorchar;
					Lattice[(x + 1) % L] = chosenchar;
				}
			}
			else if (Event == 1 && chosen > 0)
			{
				if (RandDeath <= DeathRatechosen)
				{
					Lattice[x] = "0";
					chosen--;
				}
			}
		}
		else if (Lattice[(x + 1) % L] == "0")
		{
			if (Event == 0)
			{
				Lattice[x] = "0";
				Lattice[(x + 1) % L] = chosenchar;
			}
			else if (Event == 1)
			{
				if (RandBirth <= BirthRatechosen)
				{
					Lattice[(x + 1) % L] = chosenchar;
					chosen++;
				}
			}
		}
	}
	else if (direction == 1)//chosen direction left
	{
	
		if (Lattice[(x - 1 + 2 * L) % (L)] == preychar)
		{
			if (Event == 0)
			{
				if (RandSwitch <= SwitchRate)
				{
					Lattice[x] = preychar;
					Lattice[(x - 1 + 2 * L) % (L)] = chosenchar;
				}
			}
			else if (Event == 1 && prey > 0)
			{
				if (RandDeath <= DeathRateprey)
				{
					Lattice[(x - 1 + 2 * L) % (L)] = "0";
					prey--;
				}
			}
		}
		else if (Lattice[(x - 1 + 2 * L) % (L)] == predatorchar)
		{
			if (Event == 0)
			{
				if (RandSwitch <= SwitchRate)
				{
					Lattice[x] = predatorchar;
					Lattice[(x - 1 + 2 * L) % (L)] = chosenchar;
				}
			}
			else if (Event == 1 && chosen > 0)
			{
				if (RandDeath <= DeathRatechosen)
				{
					Lattice[x] = "0";
					chosen--;
				}
			}
		}
		else if (Lattice[(x - 1 + 2 * L) % (L)] == "0")
		{
			if (Event == 0)
			{
				if (RandSwitch <= SwitchRate)
				{
					Lattice[x] = "0";
					Lattice[(x - 1 + 2 * L) % (L)] = chosenchar;
				}
			}
			else if (Event == 1)
			{
				if (RandBirth <= BirthRatechosen)
				{
					Lattice[(x - 1 + 2 * L) % (L)] = chosenchar;
					chosen++;
				}
			}
		}
	}
}
void D1Empty(string Lattice[L], ofstream& outfile, int x, int direction,int& chosen,
	string chosenchar, double BirthRatechosen, double SwitchRate)
{

	int Event = genrand_int32() % 2;
	//chance of event (<= 0.5 switching, <= 0.5 death or birth in the case of species vs empty spot)

	double RandSwitch = genrand_real3();
	//Rate of switching: 1 for now

	double RandBirth = genrand_real3();
	
	if (direction == 0)
	{
		if (Lattice[(x + 1) % L] == chosenchar)
		{
			if (Event == 0)
			{
				if (RandSwitch <= SwitchRate)
				{
					Lattice[x] = chosenchar;
					Lattice[(x + 1) % L] = "0";
				}
			}
			else if (Event == 1)
			{
				if (RandBirth <= BirthRatechosen)
				{
					Lattice[x] = chosenchar;
					chosen++; 
				}
			}
		}
	}
	else if (direction == 1)
	{
		if (Lattice[(x - 1 + 2 * L) % (L)] == chosenchar)
		{
			if (Event == 0)
			{
				if (RandSwitch <= SwitchRate)
				{
					Lattice[x] = chosenchar;
					Lattice[(x - 1 + 2 * L) % (L)] = "0";
				}
			}
			else if (Event == 1)
			{
				if (RandBirth <= BirthRatechosen)
				{
					Lattice[x] = chosenchar;
					chosen++; 
				}
			}
		}
	}
}