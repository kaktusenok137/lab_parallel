#include <iostream>
#include <fstream>
#include <string>
#include <mpi.h>
#include <stdlib.h>
#include <cstdlib>

using namespace std;

int main(int argc, char** argv)
{
	//размерность матриц
	int N = 500;
	int rank, size, i, j, t, count, th_num;
	int surplus = 0; //смещение, если размер матрицы не делится поровну на кол-во процессов, то остатки уйдут последнему

	double start, end;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	ifstream file1, file2, file3;
	ofstream result;

	// выделяем память под наши матрицы
	double* A = new double[N * N];
	double* B = new double[N * N];
	double* F = new double[N * N];

	double* D = new double[N * N];
	double* C = new double[N * N];

	//разделяем порции данных между всеми процессами
	count = N / size;
	if (rank == size - 1 && N % size != 0) {
		surplus = N % size;
	}

	//нулевой процесс считывает данные и отправляет данные всем остальным процессам
	if (rank == 0) {
		cout << "Size = " << N << endl;
		// открываем файлы с числами и заполняем матрицы
		file1.open("numbers1.txt");
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				file1 >> A[i * N + j];
			}
		}
		file1.close();

		file2.open("numbers2.txt");
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				file2 >> B[i * N + j];
			}
		}
		file2.close();

		file3.open("numbers3.txt");
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				file3 >> F[i * N + j];
			}
		}
		file3.close();

		start = MPI_Wtime();
	}

	//нулевой процесс, исп-я ф-ю широковещательной передачи посылает данные типа double всем процессам группы, включая себя
	MPI_Bcast(&A[0], N * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&B[0], N * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&F[0], N * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	// перемножаем первые две матрицы
	for (i = rank * count; i < count * (rank + 1) + surplus; i++) {
		for (j = 0; j < N; j++) {
			D[i * N + j] = 0;
			for (t = 0; t < N; t++) {
				D[i * N + j] += A[i * N + t] * B[t * N + j];
			}

		}
	}

	delete[] A;
	delete[] B;

	// перемножаем полученную и третью
	for (i = rank * count; i < count * (rank + 1) + surplus; i++) {
		for (j = 0; j < N; j++) {
			C[i * N + j] = 0;
			for (t = 0; t < N; t++) {
				C[i * N + j] += D[i * N + t] * F[t * N + j];
			}
		}
	}

	delete[] D;
	delete[] F;

	//MPI_GATHERV разрешается принимать от каждого процесса переменное число элементов данных
	//массив, содержащий кол-во элементов, получаемых от каждого процесса
	int* rcounts = new int[size];
	//массив, где элемент определяет смещение относительно rcounts, в котором размещаются данные из процесса th_num
	int* displs = new int[size];

	for (th_num = 0; th_num < size - 1; th_num++) {
		displs[th_num] = th_num * count * N;
		rcounts[th_num] = count * N;
	}
	displs[size - 1] = (size - 1) * count * N;
	rcounts[size - 1] = (count + N % size) * N;

	//результирующая матрица перемножения 3-х матриц
	double* resultC = new double[N * N];

	//нулевой процесс собирает мал. порции перемножения матриц из матрицы С типа double от всех процессов, 
	//располагает полученные данные в порядке возрастания номеров процессов-отправителей(используя смещение displs) в буфер resultC типа double
	MPI_Gatherv(&C[rank * count * N], (count + surplus) * N, MPI_DOUBLE, resultC, rcounts, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	delete[] C;

	//нулевой процесс записывает результат перемножения в файл
	if (rank == 0) {
		end = MPI_Wtime();
		cout << "Time = " << end - start << endl;

		// выводим матрицу в файл
		result.open("res.txt");
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				result << resultC[i * N + j] << " ";
			}
			result << endl;
		}
		result.close();
	}

	MPI_Finalize();

	delete[] resultC;

	return 0;
}
