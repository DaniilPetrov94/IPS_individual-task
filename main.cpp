#include <math.h>
#include <stdio.h>
#include <cilk/cilk.h>
#include <cilk/reducer_opadd.h>
#include <iostream>
#include <chrono>

using namespace std::chrono;

//// Calculating definete integral with trapezium area 
////The integral has the form = 6.0 / sqrt(x*(2.0 - x)) from 0.5 to 1.
double function(double x)
{
	return 6.0 / sqrt(x*(2.0 - x));
}

//// Main variables for calculating integral value 
//// n - amount trapeziums
//// a - left boundary,
//// b - right boundary.

double Integral(int n, double a, double b)
{
	double integral_value = 0.0;
	double h = (b - a) / (double)n; //// height trapezium (partition);

									//// Start clock
	high_resolution_clock::time_point t1 = high_resolution_clock::now();

	//// We need add reductor for correctly work cilk_for(), because variable (summ) is global
	cilk::reducer_opadd<double> summ(0.0);
	cilk_for(int i = 1; i < n; ++i)
	{
		double x = a + i*h;
		summ += function(x);
	}

	//// Caclulate intagral value
	integral_value = h*((function(a) + function(b)) / 2.0 + summ.get_value())

		//// End time calcualating integral
		high_resolution_clock::time_point t2 = high_resolution_clock::now();

	//// Difference start time end end time
	duration<double> direct_motion_duration = (t2 - t1);

	printf("Time : %.10f ", direct_motion_duration.count());

	return integral_value;
}

int main()
{
	double a = 0.5; //// left boundary,
	double b = 1.0; //// right boundary,
	int n = 1000000; //// amount trapeziums /

					 //// compare the running time of the program depending on the number of partitions
	for (n = 1000; n <= 100000000; n = n * 10)
	{
		printf("Integral value: %.20f , n: %d \n", Integral(n, a, b), n);
	}
	return 0;
}