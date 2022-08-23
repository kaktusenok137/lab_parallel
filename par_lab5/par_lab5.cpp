#include <iostream>
#include <time.h>
#include <fstream>
#include <vector>
#include <omp.h>
#define NUM_THREADS 2

using namespace std;

int main()
{
	omp_set_num_threads(NUM_THREADS);
	// объявляем матрицы
	int N, th_num, num_ths;
	double** A = NULL, ** B = NULL, ** F = NULL, ** D = NULL, ** C = NULL;
	ifstream file1, file2, file3;
	ofstream result;
	// подключаем русский язык
	setlocale(0, "");
	// вводим размерность

	cout << "Введите размерность матрицы: ";
	cin >> N;

	// выделяем память под наши матрицы
	A = new double* [N];
	B = new double* [N];
	F = new double* [N];

	D = new double* [N];
	C = new double* [N];

	for (int i = 0; i < N; i++) {
		A[i] = new double[N];
		B[i] = new double[N];
		F[i] = new double[N];

		D[i] = new double[N];
		C[i] = new double[N];
	}

	int i, j,t;

	double time_s = omp_get_wtime();
	// открываем файлы с числами и заполняем матрицы
#pragma omp parallel private(i, j, t)
	{
#pragma omp sections
		{
#pragma omp section
			{
				file1.open("numbers1.txt");
				for (i = 0; i < N; i++) {
					for (j = 0; j < N; j++) {
						file1 >> A[i][j];
					}
				}
				file1.close();
			}
#pragma omp section
			{
				file2.open("numbers2.txt");
				for (i = 0; i < N; i++) {
					for (j = 0; j < N; j++) {
						file2 >> B[i][j];
					}
				}
				file2.close();
			}
#pragma omp section
			{
				file3.open("numbers3.txt");
				for (i = 0; i < N; i++) {
					for (j = 0; j < N; j++) {
						file3 >> F[i][j];
					}
				}
				file3.close();
			}
		}
#pragma omp for
		// перемножаем первые две матрицы
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				D[i][j] = 0;
				for (t = 0; t < N; t++) {
					D[i][j] += A[i][t] * B[t][j];
				}
			}
		}
#pragma omp for
		// перемножаем полученную и третью
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				C[i][j] = 0;
				for (t = 0; t < N; t++) {
					C[i][j] += D[i][t] * F[t][j];
				}
			}
		}
	}
	time_s = omp_get_wtime() - time_s;
	cout << "Time = " << time_s << endl;

	// выводим матрицу в файл
	result.open("res.txt");
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			result << C[i][j] << " ";
		}
		result << endl;
	}
	result.close();

	system("pause");
	return 0;
}
