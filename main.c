// bibliotecas
#include <stdio.h>
#include <stdlib.h>

// prototipos de funcoes


// struct para cada campo
typedef struct {
  int bomb;           // recebe 0 (false) ou 1 (true)
  int open;           // recebe 0 (false) ou 1 (true)
  int flag;           // recebe 0 (false) ou 1 (true)
  int adjacentBombs;  // recebe um numero entre 0 e 9
} field;

#define length 10
// criacao do tabuleiro (matriz[length][length])
field board[length][length];

int main(void) {
  

  return 0;
}
