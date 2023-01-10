#include <stdio.h >
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <omp.h>
#include <chrono>


using namespace std;

int main()
{
	int N = 1;

	cout << "Enter matrices size: N = ";
	cin >> N;

	cout << endl;

		cout << "Work in progress..." << endl << endl;

		srand(time(0)); 

		//Получение числа домтупных потоков процессора
		const int ntr = int(omp_get_max_threads());

		cout << ntr << " threads available. " << endl;

		int i, j, k;

		//Создание матриц
		int** mat1;
		int** mat2;
		int** res;
		double* chronica = new double[ntr];

		mat1 = new int* [N];
		mat2 = new int* [N];
		res = new int* [N];

		for (i = 0; i < N; i++) {
			mat1[i] = new int[N];
			mat2[i] = new int[N];
			res[i] = new int[N];
		}

		//Заполнение матриц
		for (int m = 0; m < N; m++) {

			for (int n = 0; n < N; n++) {

				mat1[m][n] = 1 + rand() % 150;
				mat2[m][n] = 1 + rand() % 350;
			}
		
		}

		
		//Вычисление произведения матриц со снижением числа исполняющих потоков на каждой итерации с фиксацией времени выполнения
		for (int th = ntr; th > 0; th--) { 

			int threadsNum = th;
			omp_set_num_threads(threadsNum);
			
			cout << th << " threads cicle is in progress" << endl;

			auto start = chrono::steady_clock::now();
		
			#pragma omp parallel shared(mat1, mat2, res, chronica) private(i, j, k) 

			{
				#pragma omp for schedule(dynamic, 1)

					for (i = 0; i < N; i++) {

						for (j = 0; j < N; j++) {

							res[i][j] = 0;

							for (k = 0; k < N; k++)
							{

								res[i][j] += (mat1[i][k] * mat2[k][j]); 

							}
						}
					}
			}

			auto end = chrono::steady_clock::now();
			double result = double(chrono::duration_cast <chrono::nanoseconds> (end - start).count());
			cout << th << " threads Processing time is: " << result / 1e9 << " s" << endl << endl;
			chronica[ntr-th] = result / 1e9;
		
		}

		for (i = 0; i < ntr; i++) { cout << i + 1 << " threads time was " << chronica[ntr-(i+1)] << endl; }

	system("pause");
	
}



