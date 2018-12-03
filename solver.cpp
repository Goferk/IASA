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
  ifstream myfile;
  int numbers[N * N];
  int sdk[N * N * N * N];
  int fmly[N * N * N * N];
  int sumfmly[N * N * N * N];
  int fil = 0;
  int col = 0;

  for (int i = 0; i < N * N; i++)
    numbers[i] = N * N;

  myfile.open ("ks.txt");
  if (myfile.is_open()) {
    while (getline (myfile, line)) {
      if (fil < N * N) {
        col = 0;
        for (int i = 0; i < line.size(); i++) {
          if (isdigit(line[i]) && i > 1 && line[i - 1] == 'C') {
            number = line[i] - 48;
            if (i + 1 < line.size() && isdigit(line[i + 1])) {
              number = number * 10 + line[i + 1] - 48;
              i++;
            }
            fmly[(fil * 9) + col] = number;
            col++;
          }

        }
      }
      std::cout << line << '\n';
      fil++;
    }
    myfile.close();
  } else
    std::cout << "Unable to open file";

  return 0;
}
