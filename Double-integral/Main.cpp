#include <iostream>
#include <chrono>
#include <omp.h>
#include <math.h>

using namespace std;

struct Integral
{
	double min;
	double max;
	int numberOfRectangles;
};

// Wolframalpha result: 2.311
double Pattern(double x, double y)
{
	return ( (pow(x, 2) + pow(y, 2)) / (sqrt(x + y + 1)) );
}

int main()
{
	cout << "Hello Integral" << endl;

	Integral inner =
	{
		1,
		2,
		5000
	};

	Integral outer =
	{
		1,
		2,

		5000
	};

	double result = 0.0;
	double innerIntegralResult = 0.0;

	double innerRectWidth = ( (inner.max - inner.min) / inner.numberOfRectangles );
	double outerRectWidth = ((outer.max - outer.min) / outer.numberOfRectangles);

	int x, y;

	// Time of calculate integral without omp: 2.4992
	auto timeCalculateIntegralStart = chrono::high_resolution_clock::now();

	// Time of calculate integral: 0.566551
#pragma omp parallel for private(x, y, innerIntegralResult) reduction(+:result)
	for (y = 1; y <= outer.numberOfRectangles; ++y)
	{
		innerIntegralResult = 0.0;

// Time of calculate integral: 0.893089
// #pragma omp parallel for private(x) reduction(+:innerIntegralResult)
		for (x = 1; x <= inner.numberOfRectangles; ++x)
		{
			innerIntegralResult += Pattern(inner.min + x * innerRectWidth, outer.min + y * outerRectWidth) * innerRectWidth;
		}

		result += innerIntegralResult * outerRectWidth;
	}

	cout << "Result: " << result << endl;
	
	auto timeCalculateIntegralFinish = chrono::high_resolution_clock::now();
	chrono::duration<double> elapsedTimeOfCalculateIntegral = timeCalculateIntegralFinish - timeCalculateIntegralStart;
	cout << "Time of calculate integral: " << elapsedTimeOfCalculateIntegral.count();


	getchar();
	return 0;
}