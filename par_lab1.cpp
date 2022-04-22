// par_lab1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <fstream>
#include <locale>
#include <ctime>
#include <cstdlib>
#include <omp.h>

using namespace std;

int main()
{
	// объявляем матрицы
	int N;
	double** A = NULL, ** B = NULL, ** F = NULL, ** D = NULL, ** C = NULL;
	ifstream file1, file2, file3;
	ofstream result;
	// подключаем рус язык
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

	for (int i = 0; i < N; i++)
	{
		A[i] = new double[N];
		B[i] = new double[N];
		F[i] = new double[N];
		D[i] = new double[N];
		C[i] = new double[N];
	}

	// открываем файлы с числами и заполняем матрицы
	file1.open("numbers1.txt");
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			file1 >> A[i][j];
		}
	}
	file1.close();

	file2.open("numbers2.txt");
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			file2 >> B[i][j];
		}
	}
	file2.close();

	file3.open("numbers3.txt");
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			file3 >> F[i][j];
		}
	}
	file3.close();

	/*cout << "A:" << endl;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << A[i][j] << "\t";
		}
		cout << endl;
	}
	cout << endl;

	cout << "B:" << endl;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << B[i][j] << "\t";
		}
		cout << endl;
	}
	cout << endl;

	cout << "F:" << endl;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << F[i][j] << "\t";
		}
		cout << endl;
	}
	cout << endl;*/


	double time_s = omp_get_wtime();

	// перемножаем первые две матрицы
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			D[i][j] = 0;
			for (int t = 0; t < N; t++) {
				D[i][j] += A[i][t] * B[t][j];
			}

		}
	}

	// перемножаем полученную и третью
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			C[i][j] = 0;
			for (int t = 0; t < N; t++) {
				C[i][j] += D[i][t] * F[t][j];
			}
		}
	}

	time_s = omp_get_wtime() - time_s;
	cout << "Time = " << time_s << endl;


	/*cout << "C:" << endl;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << C[i][j] << "\t";
		}
		cout << endl;
	}
	cout << endl;*/


	// выводим матрицу в файл
	result.open("res.txt");
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			result << C[i][j] << " ";
		}
		result << endl;
	}
	result.close();

	return 0;
}