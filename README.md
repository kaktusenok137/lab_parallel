# Laboratory work on the subject "Parallel programming".


<b>Assignment: </b>
Develop a program in C / C ++ in accordance with the options for laboratory work given in table 1.
In all tasks, it is required to create a matrix (vector) or several matrices (vectors) of the same dimension indicated in Table 1, fill them with values ​​read from a text file. Text files should be prepared in advance by filling them with random numbers. Then implement the task. At the end, output the calculation results again to a text file.

The program interface is a console application. After all calculations are done, the program prints the total running time (in seconds).

Table 1. Description of tasks

<table>
  <tr>
    <td>Dimension of array or matrix</td>
    <td>Data type</td>
    <td>Task description</td>
  </tr>
  <tr>
    <td>[10*10; 3000*3000]</td>
    <td>double</td>
    <td>Compute the product of dense matrices C = A*B*F</td>
  </tr>
  </table>


To do the job, 4 files were first created:

• A.txt for matrix A;

• B.txt for matrix B;

• F.txt for matrix F;

• res.txt to record the result of the multiplication.

At runtime, you need to enter the dimension of the matrix.

The algorithm consists of multiplying matrix A by matrix B, the result is written into matrix D. After that, I multiply the resulting matrix D and matrix F, the result into matrix C.

# File locations:
* par_lab1 - sequential matrix multiplication algorithm;
* par_lab2 - parallel algorithm for matrix multiplication using MPI (point-to-point operations);
* par_lab3 - parallel matrix multiplication algorithm using MPI (collective data transfer operations);
* par_lab4 - parallel matrix multiplication algorithm using OpenMP (data parallelization using for and reduction directives);
* par_lab5 - parallel algorithm for matrix multiplication using OpenMP (parallelization across tasks using the sections directive).


# Visualization of the correct operation of the application

![image](https://user-images.githubusercontent.com/76211121/186193567-d79f230a-b3d7-4052-9235-7f690ea9658f.png)
<p align=center>Visualization of the correct operation of the application</p>

![image](https://user-images.githubusercontent.com/76211121/186193725-48e7c6d7-a6e6-4d72-a44a-3f0f8c2c1dda.png)
<p align=center>File contents res.txt</p>
