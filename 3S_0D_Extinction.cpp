#include <iostream>
#include <fstream> 
#include <string>
#include <iomanip>
#include <cmath>
#include "random.h"

const int snum = 3;

using namespace std;

int main()
{
	ofstream out;
	out.open("out.txt");

	time_t a_t;
	unsigned long int seed = time(&a_t);

	//choosing species
	init_genrand(seed);
	for (int init = 1; init <= 66; init++)
	{
	int tot = snum *init;
	int a = init, b = init, c = init;
	int round = 0;
	double tau = tot*1.0;

	int first = 1;

	double sumfirst = 0;
	double sumsecond = 0;

	int rnum = 10000;

	for (int run = 0; run < rnum; run++)
	{
		round = 0;
		a = init, b = init, c = init;
		first = 1;

		while (a < tot && b < tot && c < tot)
		{
			if (a > 0 && b > 0 && c > 0)
			{
				int s1 = genrand_int32() % 3;
				int s2 = genrand_int32() % 3;

				if ((s1 == 0 && s2 == 1) || (s2 == 0 && s1 == 1))
				{
					a++;
					b--;
				}

				else if ((s1 == 0 && s2 == 2) || (s2 == 0 && s1 == 2))
				{
					c++;
					a--;
				}

				else if ((s1 == 1 && s2 == 2) || (s2 == 1 && s1 == 2))
				{
					b++;
					c--;
				}
			}

			else if (a == 0 && b > 0 && c > 0)
			{
				int s1 = genrand_int32() % 2;
				int s2 = genrand_int32() % 2;

				if ((s1 == 0 && s2 == 1) || (s2 == 0 && s1 == 1))
				{
					b++;
					c--;
				}
			}

			else if (b == 0 && a > 0 && c > 0)
			{
				int s1 = genrand_int32() % 2;
				int s2 = genrand_int32() % 2;

				if ((s1 == 0 && s2 == 1) || (s2 == 0 && s1 == 1))
				{
					c++;
					a--;
				}
			}

			else if (c == 0 && b > 0 && a > 0)
			{
				int s1 = genrand_int32() % 2;
				int s2 = genrand_int32() % 2;

				if ((s1 == 0 && s2 == 1) || (s2 == 0 && s1 == 1))
				{
					a++;
					b--;
				}
			}
			round++;
			if (a == 0 || b == 0 || c == 0)
			{
				if (first)
				{
					sumfirst += round / (rnum*1.0);
					first = 0;
				}

				if (a == tot || b == tot || c == tot)
				{
					sumsecond += round / (rnum*1.0);
				}
			}
		}
	}
	out << tot << "\t" << sumfirst / tot << "\t" << sumsecond / tot <<endl;
}
	return 0;
}
