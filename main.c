// bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// prototipos de funcoes
void playerInfo();
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

char *playerName = NULL;

int main(void) {
  playerInfo();

  savePlayerInfo("points");   // ultima funcao a ser chamada

  return 0;
}

void playerInfo() {
  playerName = (char *) malloc(51 * sizeof(char));
  if (playerName == NULL) {
    printf("Erro ao alocar memória.");
    exit(2);
  }

  printf("Digite o nome do jogador:\nEx.: nome_sobrenome\n\n");
  scanf("%s", playerName);

  playerName = (char *) realloc(playerName, strlen(playerName) * sizeof(char));
  if (playerName == NULL) {
    printf("Erro ao alocar memória.");
    exit(2);
  }
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

  free(playerName);
}
