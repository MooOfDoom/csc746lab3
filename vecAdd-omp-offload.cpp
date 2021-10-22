#include <iostream>
#include <iomanip>
#include <math.h>
#include <chrono>
#include <omp.h>

// function to add the elements of two arrays
void add(int n, float *x, float *y)
{
#pragma omp target data map(to:x[0:n], n) map(tofrom:y[0:n])
	{
#pragma omp target teams distribute parallel for
		for (int i = 0; i < n; i++)
			y[i] = x[i] + y[i];
	}// end pragma omp target data map
	
}

int main(void)
{
	int N = 1<<20; // 1M elements
	// int N = 1<<23; // 8M elements
	// int N = 1<<30; // 1G elements
	
	std::cout << "Doing vector addition of " << N << " elements." << std::endl;
	
	float *x = new float[N];
	float *y = new float[N];
	
	// initialize x and y arrays on the host
	for (int i = 0; i < N; i++) {
		x[i] = 1.0f;
		y[i] = 2.0f;
	}
	
	// insert timer code here
	std::cout << std::fixed << std::setprecision(5);
	std::chrono::time_point<std::chrono::high_resolution_clock> start_time = std::chrono::high_resolution_clock::now();
	
	// Run kernel on 1M elements on the CPU
	add(N, x, y);
	
	// insert timer code here
	std::chrono::time_point<std::chrono::high_resolution_clock> end_time = std::chrono::high_resolution_clock::now();
	
	std::chrono::duration<double> elapsed = end_time - start_time;
	
	std::cout << " Elapsed time is : " << elapsed.count() << " sec, or " << elapsed.count()*1000 << " ms " << std::endl;
	
	// Check for errors (all values should be 3.0f)
	float maxError = 0.0f;
	for (int i = 0; i < N; i++)
		maxError = fmax(maxError, fabs(y[i]-3.0f));
	std::cout << "Max error: " << maxError << std::endl;
	
	// Free memory
	delete [] x;
	delete [] y;
	
	return 0;
}