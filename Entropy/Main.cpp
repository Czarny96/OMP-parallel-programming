#include <iostream>
#include <omp.h>
#include <chrono>
#include <random>
#include <time.h>

using namespace std;

/*
Entropy pattern: S = ln(N! / (n1! * n2! * ... * n6!) )
N - number of all cube
n1 - number of all number 1 on cube
n2 - number of all number 2 on cube
.
.
.
n6 - number of all number 6 on cube
*/

#define NUMBEROFALLCUBE 600
#define NUMBEROFITERATIONS 10

unsigned short cubeMatrix[NUMBEROFALLCUBE];
unsigned short sumOfAllCube = 0;

int main()
{
	cout << "Hello Entropy" << endl;

	auto timeSetUpValueOnCubeMatrixStart = chrono::high_resolution_clock::now();

	int i, j;

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
	
	auto timeCalculateEntropyStart = chrono::high_resolution_clock::now();

	for (i = 0; i < NUMBEROFITERATIONS; i++)
	{
		unsigned short theSameValueOnCube[6] = { 0, 0, 0, 0, 0, 0 };

		for (j = 0; j < NUMBEROFALLCUBE; j++)
		{
			if (getRandomChanceToChangeNumberOnCube(generateSeed) == 5)
			{
				cubeMatrix[j] = getRandomNumberOfCube(generateSeed);
			}

			theSameValueOnCube[cubeMatrix[j]] += 1;
		}

		// TODO: CALCULATE ENTROPY


		sumOfAllCube = 0;

		for (j = 0; j < 6; j++)
		{
			sumOfAllCube += theSameValueOnCube[j];
		}

		cout << theSameValueOnCube[0] << ' ' << theSameValueOnCube[1] << ' ' << theSameValueOnCube[2] << ' '
			<< theSameValueOnCube[3] << ' ' << theSameValueOnCube[4] << ' ' << theSameValueOnCube[5]  << " | " << sumOfAllCube << " | " << i << endl;

	}

	auto timeCalculateEntropyFinish = chrono::high_resolution_clock::now();
	chrono::duration<double> elapsedTimeOfCalculateEntropy = timeCalculateEntropyFinish - timeCalculateEntropyStart;

	cout << "Time of set up value on cube matrix: " << elapsedTimeOfSetUpValueOnCubeMatrix.count() << endl;
	cout << "Time of calculate entropy: " << elapsedTimeOfCalculateEntropy.count() << endl;

	getchar();
	return 0;
}