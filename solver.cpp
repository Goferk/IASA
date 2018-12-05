#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <math.h>
#include <cmath>
#include <random>
#include <utility>
#include <algorithm>

using namespace std;

// PARAMETROS FIJOS
const int N = 3; // Largo de submatriz
const int D = 100; // Descuento por cada restriccion no cumplida del sudoku normal
const int D2 = 5; // Descuento por cada unidad de diferencia del killer sudoku
const int ITER = 10000000; // Iteraciones totales antes de terminar el algoritmo
const double ITEMP = 50; // Temperatura inicial
const double DTEMP = 5; // Cuanto decrece la temperatura
const int DTEMPITER = 20; // Iteraciones que demora en decrecer la temperatura
const double ATEMP = 100; // Cuanto aumenta la temperatura
const int ATEMPITER = 500; // Iteraciones que demora en aumentar la temperatura
const double MINTEMP = 10; // Minima temperatura posible

void swap(int * sdk, int a, int b, int * fmly, int * sumfmly) {
  int aux = sdk[b];
  sumfmly[fmly[a]] += sdk[b] - sdk[a];
  sumfmly[fmly[b]] += sdk[a] - sdk[b];
  sdk[b] = sdk[a];
  sdk[a] = aux;
}

void copySdk(int * sdk, int * bestSdk) {
  for (int i = 0; i < N * N * N * N; i++)
    bestSdk[i] = sdk[i];
}

int evaluate(int * sdk, int * fmly, int * sumfmly, int * expectedSumfmly) {
  long double cont = 0;
  for (int i = 0; i < N * N; i++) {
    if (sumfmly[i] != expectedSumfmly[i]) {
      cont += abs(expectedSumfmly[i] - sumfmly[i]) * D2;
    }
    for (int j = 0; j < N * N; j++) {
      for (int k = 0; k < N * N; k++) {
        // Check distinct value in all column
        if (k > i && sdk[i * N * N + j] == sdk[k * N * N + j]) {
          cont += 2 * D;
        }

        // Check distinct value in all row
        if (k > j && sdk[i * N * N + j] == sdk[i * N * N + k]) {
          cont += 2 * D;
        }

        // Check distinct value in all submatrix
        for (int m = 0; m < N && k < N; m++) {
          if (i != ((i/N) * N + k) && j != ((j/N) * N + m) && sdk[i * N * N + j] == sdk[((i/N) * N + k) * N * N + ((j/N) * N + m)]) {
            cont += D;
          }
        }
      }
    }
  }
  return cont;
}

void poblateSdk(int * sdk, bool * isGiven, int * numbers, int * sumfmly, int * fmly) {
  for (int i = 0; i < N * N ; i++) {
    for (int j = 0; j < N * N; j++) {
      if (!isGiven[i * N * N + j]) {
        for (int k = i + j; k < i + j + (N * N); k++) {
          if (numbers[k % (N * N)] > 0) {
            numbers[k % (N * N)]--;
            sdk[i * N * N + j] = (k % (N * N)) + 1;
            sumfmly[fmly[i * N * N + j]] += (k % (N * N)) + 1;
            break;
          }
        }
      }
    }
  }
}

int main() {
  string line;
  int number;
  int number2;
  int aux;
  bool isGiven[N * N * N * N] = { };
  ifstream myfile;
  int numbers[N * N];
  int sdk[N * N * N * N];
  int bestSdk[N * N * N * N];
  int fmly[N * N * N * N];
  int sumfmly[N * N * N * N] = { };
  int expectedSumfmly[N * N * N * N];
  int fil = 0;
  int col = 0;
  long double bestSol;
  long double actualSol;
  int pvt1;
  int pvt2;
  double temp = ITEMP;
  double rand01;
  double cond;

  std::random_device generator;
  std::uniform_int_distribution<int> distribution(0, 80);
  std::uniform_int_distribution<int> distribution2(0, 79);
  std::uniform_real_distribution<double> distribution3(0, 1);

  // Seteo la cantidad de numeros posibles
  for (int i = 0; i < N * N; i++)
    numbers[i] = N * N;

  // Abro el archivo de killer sudoku
  myfile.open ("ks.txt");

  // Si se abrio correctamente, continuamos
  if (myfile.is_open()) {
    // Leo cada linea
    while (getline (myfile, line)) {
      // Armo el sudoku inicial
      if (fil < N * N) {
        col = 0;
        // Recorro la linea caracter por caracter
        for (int i = 0; i < line.size(); i++) {
          // Caso en el que el caracter es el numero de la familia
          if (isdigit(line[i]) && i > 1 && line[i - 1] == 'C') {
            // Guardamos el primer digito
            number = line[i] - 48;

            // Recorremos hasta no encontrar mas digitos
            for (int j = i; j + 1 < line.size() && isdigit(line[j + 1]); j++) {
              // Unimos los digitos
              number = number * 10 + line[i + 1] - 48;

              // Actualizamos el valor de i
              i = j;
            }

            // Guardamos el numero de familia en la casilla
            fmly[fil * N * N + col] = number;

            // Guardamos la suma total de la familia actual
            sumfmly[number - 1] += sdk[fil * N * N + col];

            // Pasamos a la siguiente columna
            col++;

          // Caso en el que el caracter es el valor de la casilla y es entregado como pista
        } else if (isdigit(line[i]) && ((i > 1 && line[i - 1] == ' ') || i == 0)) {
            // Guardamos el primer digito
            number = line[i] - 48;

            // Recorremos hasta no encontrar mas digitos
            for (int j = i; j + 1 < line.size() && isdigit(line[j + 1]); j++) {
              // Unimos los digitos
              number = number * 10 + line[i + 1] - 48;

              // Actualizamos el valor de i
              i = j;
            }
            // Actualizamos el valor de la casilla
            sdk[(fil * N * N) + col] = number;
            isGiven[(fil * N * N) + col] = true;

            // Atualizamos la cantidad de valores disponibles
            numbers[number - 1]--;

            // Nos saltamos la 'C' de la casilla
            i++;

          // Caso en el que el caracter es el valor de la casilla y no es entregado como pista
          } else if (line[i] == '.') {
            // Actualizamos el valor de la casilla
            sdk[fil * N * N + col] = 0;

            // Nos saltamos la 'C' de la casilla
            i++;
          }
        }
        fil++;
      } else {
        if (line[0] == 'C') {
          // Guardamos el primer digito
          number = line[1] - 48;
          aux = 1;

          // Recorremos hasta no encontrar mas digitos
          for (int i = 1; isdigit(line[i + 1]); i++) {
            // Unimos los digitos
            number = number * 10 + line[i + 1] - 48;
            aux++;
          }

          // Guardamos el primer digito
          number2 = line[aux + 2] - 48;

          // Recorremos hasta no encontrar mas digitos
          for (int i = aux + 2; isdigit(line[i + 1]); i++) {
            // Unimos los digitos
            number2 = number2 * 10 + line[i + 1] - 48;
          }

          // Guardamos la suma de los valores de las casillas
          expectedSumfmly[number - 1] = number2;
        }
      }
    }
    myfile.close();

  } else {
    std::cout << "Unable to open file";
  }
  poblateSdk(sdk, isGiven, numbers, sumfmly, expectedSumfmly);
  bestSol = evaluate(sdk, fmly, sumfmly, expectedSumfmly);
  copySdk(sdk, bestSdk);
  number = 0;
  number2 = 0;
  for (int i = 0; i < ITER; i++) {
    do {
      pvt1 = distribution(generator);
    } while (isGiven[pvt1]);

    do {
      pvt2 = distribution2(generator);
      if (pvt2 >= pvt1)
        pvt2++;
    } while (isGiven[pvt2]);


    swap(sdk, pvt1, pvt2, fmly, sumfmly);
    actualSol = evaluate(sdk, fmly, sumfmly, expectedSumfmly);

    cond = std::exp((bestSol - actualSol) / temp);
    rand01 = distribution3(generator);

    if (actualSol > bestSol && cond <= rand01) {
      number++;
      swap(sdk, pvt1, pvt2, fmly, sumfmly);
    } else if (actualSol < bestSol) {
      number2++;
      bestSol = actualSol;
      copySdk(sdk, bestSdk);
    }

    if ((i % ATEMPITER) == 0 && temp > 0 && i > 0) {
      temp += ATEMP;
    }
    else if ((i % DTEMPITER) == 0 && temp - DTEMP > 0  && i > 0)
      temp -= DTEMP;
    else if ((i % DTEMPITER) == 0  && i > 0)
      temp = MINTEMP;
  }
  std::cout << "No acepto la solucion: " << number << '\n';
  std::cout << "Acepto la solucion porque era mejor: " << number2 << '\n';
  std::cout << "Acepto la solucion siendo peor por random: " << ITER - (number2 + number) << '\n';
  std::cout << "Mejor solucion: " << bestSol << '\n';

  std::cout << "\n ++===+===+===++===+===+===++===+===+===++\n";
  for (int i = 0; i < N * N; i++) {
    for (int j = 0; j < N * N; j++) {
      if (j % 3 == 0)
        std::cout << " || " << bestSdk[i * N * N + j];
      else
        std::cout << " | " << bestSdk[i * N * N + j];
    }
    if ((i + 1) % 3 == 0)
      std::cout << " ||\n ++===+===+===++===+===+===++===+===+===++\n";
    else
      std::cout << " ||\n ++---+---+---++---+---+---++---+---+---++\n";
  }



  return 0;
}
