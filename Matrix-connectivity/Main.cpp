#include <iostream>
#include <omp.h>
#include <chrono>
#include <math.h>

using namespace std;

#define SIZE 1000

double matrixA[SIZE][SIZE];
double matrixB[SIZE][SIZE];
double matrixC[SIZE][SIZE];
double matrixD[SIZE][SIZE];

double firstCalculationResult[SIZE][SIZE];
double secoundCalculaionResult[SIZE][SIZE];
double thirdCalculationResult[SIZE][SIZE];

double tempMatrix1[SIZE][SIZE];
double tempMatrix2[SIZE][SIZE];

double SetMatrixA(int i, int j)
{
	return (((double)(i)) / (i + j + 7.0));
}

double SetMatrixB(int i, int j)
{
	return (((double)(j)) / (2 * i + 3 * j));
}

double SetMatrixC(int i, int j)
{
	return (((double)(i + j)) / (i + j + 5.0));
}

double SetMatrixD(int i, int j)
{
	return (((double)(i - j)) / (i + j + 4.0));
}

// Time of SetAllMatrix() without omp: 0.149125
void SetAllMatix()
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			matrixA[i][j] = SetMatrixA(i, j);
			matrixB[i][j] = SetMatrixB(i, j);
			matrixC[i][j] = SetMatrixC(i, j);
			matrixD[i][j] = SetMatrixD(i, j);
		}
	}
}

// Time of MultiplyingTheMatrix(..., ..., ...) without omp: 57.709
void MultiplyingTheMatrix(double matrixA[SIZE][SIZE], double matrixB[SIZE][SIZE], double multiplyingMatrix[SIZE][SIZE])
{
	int i, j, k;

	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			multiplyingMatrix[i][j] = 0;
			for (k = 0; k < SIZE; k++)
			{
				multiplyingMatrix[i][j] += matrixA[i][k] * matrixB[k][j];
			}
		}
	}
}

// Time of CheckingMatrixConnectivity(..., ..., ...) without omp: 0.031708
bool CheckingMatrixConnectivity(double matrixA[SIZE][SIZE], double matrixB[SIZE][SIZE], double matrixC[SIZE][SIZE])
{
	int i, j;
	double checksum = 0.0005;

	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			if ((abs(matrixA[i][j] - matrixB[i][j]) >= checksum) &&
				(abs(matrixA[i][j] - matrixC[i][j]) >= checksum) &&
				(abs(matrixB[i][j] - matrixC[i][j]) >= checksum))
			{
				return false;
			}
		}
	}

	return true;
}

int main()
{
	cout << "Hello Matrix" << endl;

	auto timeSetAllMatrixStart = chrono::high_resolution_clock::now();
	SetAllMatix();
	auto timeSetAllMatrixFinish = chrono::high_resolution_clock::now();

	chrono::duration<double> elapsedTimeOfSetAllMatrix = timeSetAllMatrixFinish - timeSetAllMatrixStart;

	auto timeMultiplyingTheMatrixStart = chrono::high_resolution_clock::now();
	
	// (matrixA * matrixB) * (matrixC * matrixD)
	MultiplyingTheMatrix(matrixA, matrixB, tempMatrix1);
	MultiplyingTheMatrix(matrixC, matrixD, tempMatrix2);
	MultiplyingTheMatrix(tempMatrix1, tempMatrix2, firstCalculationResult);

	// (((matrixA * matrixB) * matrixB) * matrixD)
	MultiplyingTheMatrix(matrixA, matrixB, tempMatrix1);
	MultiplyingTheMatrix(tempMatrix1, matrixC, tempMatrix2);
	MultiplyingTheMatrix(tempMatrix2, matrixD, secoundCalculaionResult);

	// (matrixA * (matrixB * (matrixC * matrixD)))
	MultiplyingTheMatrix(matrixC, matrixD, tempMatrix1);
	MultiplyingTheMatrix(tempMatrix1, matrixB, tempMatrix2);
	MultiplyingTheMatrix(tempMatrix2, matrixA, secoundCalculaionResult);

	auto timeMultiplyingTheMatrixFinish = chrono::high_resolution_clock::now();

	chrono::duration<double> elapsedTimeOfMultiplyingTheMatrix = timeMultiplyingTheMatrixFinish - timeMultiplyingTheMatrixStart;
	
	auto timeCheckingMatrixConnectivityStart = chrono::high_resolution_clock::now();

	if (CheckingMatrixConnectivity(firstCalculationResult, secoundCalculaionResult, thirdCalculationResult))
		cout << "Matrices are connectivity" << endl;
	else
		cout << "Matrices are not connectivity" << endl;

	auto timeCheckingMatrixConnectivityFinish = chrono::high_resolution_clock::now();

	chrono::duration<double> elapsedTimeOfCheckingMatrixConnectivity = timeCheckingMatrixConnectivityFinish - timeCheckingMatrixConnectivityStart;
	
	cout << "Time of SetAllMatrix(): " << elapsedTimeOfSetAllMatrix.count() << endl;
	cout << "Time of MultiplyingTheMatrix(..., ..., ...): " << elapsedTimeOfMultiplyingTheMatrix.count() << endl;
	cout << "Time of CheckingMatrixConnectivity(..., ..., ...): " << elapsedTimeOfCheckingMatrixConnectivity.count() << endl;


	getchar();
	return 0;
}