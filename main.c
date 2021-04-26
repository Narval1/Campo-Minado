// bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// prototipos de funcoes
void savePlayerInfo(char *points);

// struct para cada campo
typedef struct {
  int bomb;           // recebe 0 (false) ou 1 (true)
  int open;           // recebe 0 (false) ou 1 (true)
  int flag;           // recebe 0 (false) ou 1 (true)
  int adjacentBombs;  // recebe um numero entre 0 e 8
} field;

#define length 10
// criacao do tabuleiro (matriz[length][length])
field board[length][length];

char playerName[51];

int main(void) {
  printf("Insira seu nome sem espacos: (ex.: nome_sobrenome)\n");
  scanf("%s", playerName);

  //savePlayerInfo("points");

  return 0;
}

void savePlayerInfo(char *points) {
  FILE *database = fopen("database.txt", "a");
  char text1[10] = "Jogador: ", text2[11] = "; Pontos: ";

  if (!database) {
    printf("Erro ao abrir o arquivo!");
    return;
  }

  fputc('\n', database);

  for (int index = 0; index < strlen(text1); index++)
    fputc(text1[index], database);

  for (int index = 0; index < strlen(playerName); index++)
    fputc(playerName[index], database);
  
  for (int index = 0; index < strlen(text2); index++)
    fputc(text2[index], database);

  for (int index = 0; index < strlen(points); index++)
    fputc(points[index], database);

  fputc('\n', database);

  fclose(database);
}
