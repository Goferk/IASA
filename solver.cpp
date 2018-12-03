#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <math.h>
#include <cmath>
#include <random>
#include <utility>
#include <algorithm>

using namespace std;

const int N = 3;

void swap(int * list, int a, int b) {
  int aux = list[b];
  list[b] = list[a];
  list[a] = aux;
}

void poblateSdk(int * sdk, bool * isGiven, int * numbers) {
  for (int i = 0; i < N * N ; i++) {
    for (int j = 0; j < N * N; j++) {
      if (!isGiven[i * N * N + j]) {
        for (int k = i + j; k < i + j + (N * N); k++) {
          if (numbers[k % (N * N)] > 0) {
            numbers[k % (N * N)]--;
            sdk[i * N * N + j] = (k % (N * N)) + 1;
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
  int fmly[N * N * N * N];
  int sumfmly[N * N * N * N] = { };
  int expectedSumfmly[N * N * N * N];
  int fil = 0;
  int col = 0;

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
  poblateSdk(sdk, isGiven, numbers);

  /*
  std::cout<< '\n' << "Tablero" << '\n';
  for (int i = 0; i < N*N; i++) {
    for (int j = 0; j < N*N; j++)
      std::cout << sdk[i * N * N + j] << '\t';
    std::cout << "\n\n\n\n";
  }
  */

  return 0;
}
