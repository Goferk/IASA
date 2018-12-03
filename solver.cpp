#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <math.h>

using namespace std;

const int N = 3;

int main() {
  string line;
  int number;
  int aux;
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
            // Guardamos el primero digito
            number = line[i] - 48;

            // Variable auxiliar para saber cuantos digitos llevamos
            aux = 1;

            // Recorremos hasta no encontrar mas digitos
            for (int j = i; j + 1 < line.size() && isdigit(line[j + 1]); j++) {
              // Unimos los digitos
              number = number * pow(10, aux) + line[i + 1] - 48;

              // Actualizamos el valor de i
              i = j;

              // Actualizamos la cantidad de digitos
              aux++;
            }

            // Guardamos el numero de familia en la casilla
            fmly[fil * N * N + col] = number;

            // Guardamos la suma total de la familia actual
            sumfmly[number - 1] += sdk[fil * N * N + col];

            // Pasamos a la siguiente columna
            col++;

          // Caso en el que el caracter es el valor de la casilla y es entregado como pista
          } else if (isdigit(line[i])) {
            number = line[i] - 48;
            aux = 1;

            // Recorremos hasta no encontrar mas digitos
            for (int j = i; j + 1 < line.size() && isdigit(line[j + 1]); j++) {
              // Unimos los digitos
              number = number * pow(10, aux) + line[i + 1] - 48;

              // Actualizamos el valor de i
              i = j;

              // Actualizamos la cantidad de digitos
              aux++;
            }
            // Actualizamos el valor de la casilla
            sdk[(fil * N * N) + col] = number;

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
      }
      std::cout << line << '\n';
      fil++;
    }
    myfile.close();

/*
    std::cout << "Familia" << '\n';
    for (int i = 0; i < N*N*N*N; i++)
      std::cout << fmly[i] << ", ";
    std::cout << '\n' << "Tablero" << '\n';
    for (int i = 0; i < N*N*N*N; i++)
      std::cout << sdk[i] << ", ";
*/
  } else
    std::cout << "Unable to open file";

  return 0;
}
