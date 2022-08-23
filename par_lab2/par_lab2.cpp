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
	int rank, size, count, i, j, t, th_num;
	int surplus = 0; //смещение, если размер матрицы не делится поровну на кол-во процессов, то остатки уйдут последнему
	double start, end;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Status status;

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

		//0 процесс отправляет данные всем остальным
		for (th_num = 1; th_num < size; th_num++) {
			//отправляем элементы А типа double, 1 процессу, с тэгом 1 внутри коммуникатора
			MPI_Send(&A[th_num * count * N], (th_num == size - 1) ? (count + N % size) * N : count * N, MPI_DOUBLE, th_num, 1, MPI_COMM_WORLD);
			MPI_Send(&B[0], N * N, MPI_DOUBLE, th_num, 2, MPI_COMM_WORLD);
			MPI_Send(&F[0], N * N, MPI_DOUBLE, th_num, 3, MPI_COMM_WORLD);
		}
	}
	//остальные процессы сидят в Recv-e до тех пор, пока им не придут данные
	else {
		//процесс с рангом 1 принимает данные матрицы А типа double от 0 процесса с тегом 1 внутри коммуникатора
		MPI_Recv(&A[rank * count * N], (count + surplus) * N, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
		MPI_Recv(&B[0], N * N, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD, &status);
		MPI_Recv(&F[0], N * N, MPI_DOUBLE, 0, 3, MPI_COMM_WORLD, &status);
	}

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

	if (rank != 0) {
		//процессы, кроме нулевого, отправляют полученные рез-ты перемножения нулевому процессу с тегом 4 внутри коммуникатора
		MPI_Send(&C[rank * count * N], (count + surplus) * N, MPI_DOUBLE, 0, 4, MPI_COMM_WORLD);
	}
	else {
		//нулевой процесс принимает данные перемножения от всех процессов типа double с тегом 4 внутри коммуникатора
		for (th_num = 1; th_num < size; th_num++) {
			MPI_Recv(&C[th_num * count * N], (th_num == size - 1) ? (count + N % size) * N : count * N, MPI_DOUBLE, th_num, 4, MPI_COMM_WORLD, &status);
		}
		end = MPI_Wtime();

		cout << "Time = " << end - start << endl;

		// выводим матрицу в файл
		result.open("res.txt");
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				result << C[i * N + j] << " ";
			}
			result << endl;
		}
		result.close();
	}

	MPI_Finalize();
	delete[] C;
	return 0;
}
