#include <iostream>
#include <omp.h>
#include <chrono>
#include <random>
#include <time.h>
#include <math.h>

using namespace std;

#define NUMBEROFALLCUBE 10000
#define NUMBEROFITERATIONS 1000
#define M_PI 3.14

unsigned short cubeMatrix[NUMBEROFALLCUBE];
unsigned short sumOfAllCube = 0;

long double entropy;

// Time of calculate without OMP in functions: 5.26161
double StirlingPattern(int value)
{
	if (value != 0)
	{
		double firstPart = 0;
		int i;
 #pragma omp parallel for private(i) reduction(+ : firstPart)
		for (i = 1; i <= value; i++)
		{
			firstPart += log(value / exp(1));
		}

		double secoundPart = log(sqrt(2 * value * M_PI));

		return firstPart + secoundPart;
	}

	return 0;
}

long double EntropyCalculation(int n, int matrix[6])
{
	long double counter = StirlingPattern(n);
	long double denominator = 0;
	int i;

#pragma omp parallel for private(i) reduction(+ : denominator)
	for (i = 0; i < 6; i++)
	{
		denominator += StirlingPattern(matrix[i]);
	}

	return counter - denominator;
}

int main()
{
	cout << "Hello Entropy" << endl;
	// Time of set up value on cube matrix without OMP: 3.279e-05
	auto timeSetUpValueOnCubeMatrixStart = chrono::high_resolution_clock::now();

	int i, j;
	//Time of set up value on cube matrix: 0.00461313
#pragma omp parallel for private(i) shared(cubeMatrix)
	for (i = 0; i < NUMBEROFALLCUBE; i++)
	{
		cubeMatrix[i] = 1;
	}

	auto timeSetUpValueOnCubeMatrixFinish = chrono::high_resolution_clock::now();
	chrono::duration<double> elapsedTimeOfSetUpValueOnCubeMatrix = timeSetUpValueOnCubeMatrixFinish - timeSetUpValueOnCubeMatrixStart;

	auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
	mt19937 generateSeed(seed);
	uniform_int_distribution<int> getRandomNumberOfCube(0, 5);
	uniform_int_distribution<int> getRandomChanceToChangeNumberOnCube(1, 20);
	
	// Time of calculate wtihout OMP: 7.83094
	auto timeCalculateStart = chrono::high_resolution_clock::now();

// Time of calculate: 2.31776
#pragma omp parallel for private(i, j) reduction(+ : sumOfAllCube)
	for (i = 0; i < NUMBEROFITERATIONS; i++)
	{
		int theSameValueOnCube[6] = { 0, 0, 0, 0, 0, 0 };
		sumOfAllCube = 0;
// Time of calculate: 3.80669
//#pragma omp parallel for private(j) reduction(+ : sumOfAllCube)
		for (j = 0; j < NUMBEROFALLCUBE; j++)
		{
			if (getRandomChanceToChangeNumberOnCube(generateSeed) == 5)
			{
				cubeMatrix[j] = getRandomNumberOfCube(generateSeed);
			}

			theSameValueOnCube[cubeMatrix[j]] += 1;
			sumOfAllCube++;
		}

		entropy = EntropyCalculation(NUMBEROFALLCUBE, theSameValueOnCube);

#pragma omp critical
		cout << theSameValueOnCube[0] << ' ' << theSameValueOnCube[1] << ' ' << theSameValueOnCube[2] << ' '
			<< theSameValueOnCube[3] << ' ' << theSameValueOnCube[4] << ' ' << theSameValueOnCube[5]  << " | "
			<< entropy << " | " << sumOfAllCube << " | " << i << endl;
	}

	auto timeCalculateFinish = chrono::high_resolution_clock::now();
	chrono::duration<double> elapsedTimeOfCalculate = timeCalculateFinish - timeCalculateStart;

	cout << "Time of set up value on cube matrix: " << elapsedTimeOfSetUpValueOnCubeMatrix.count() << endl;
	cout << "Time of calculate: " << elapsedTimeOfCalculate.count() << endl;

	getchar();
	return 0;
}