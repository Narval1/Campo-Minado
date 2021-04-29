 // bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // adicionei só pra ficar bonitinho

// prototipos de funcoes
void playerInfo();
void savePlayerInfo(char *points);

// struct para cada campo
typedef struct {
  int bomb;           // recebe 0 (false) ou 1 (true)
  int open;           // recebe 0 (false) ou 1 (true)
  int flag;           // recebe 0 (false) ou 1 (true)
  int adjacentBombs;  // recebe um numero entre 0 e 8
  int firstplay = 0;  // contador pra colocar as bombas só depois da primeira jogada
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

 // verificação da coordenada no campo.
int ValidCoord(int line, int column){
  if(line >=0 && line < length && column >=0 && column < length )
    return 1;
    
  return 0;
}

// abre a coordenada e caso seja zero, abre as adjacentes 
void openCoord(int linha, int coluna) {
  
  if (ValidCoord(linha, coluna) == 1 && board[linha][coluna].open == 0 && board[linha][coluna].flag == 0 ) {
    board[linha][coluna].open = 1;
    if (board[linha][coluna].vizinhos == 0) {
      openCoord(linha - 1, coluna);
      openCoord(linha -1, coluna -1);
      openCoord(linha -1, coluna + 1);
      openCoord(linha + 1, coluna);
      openCoord(linha + 1, coluna -1);
      openCoord(linha + 1, coluna + 1);
      openCoord(linha, coluna + 1);
      openCoord(linha - 1, coluna - 1);
    }
  }
}

// lê a jogada
void jogar() {
  int linha, coluna;
  char symbol;

  printf("\nDigite a linha e a coluna: ");
  scanf("%d%d",&linha, &coluna);

  if (ValidCoord(linha, coluna) == 0) {
    printf("coordenadas invalidas, digite novamente:\n");
    jogar();
  }
  else {
    action(linha, coluna);
  }
}

// realoquei a checagem de simbolo do Rafa aqui
// Lê se o player quer abrir ou marcar uma bandeira e executa
void action(linha, coluna) {
  printf("\nO que dejesa fazer?('O' para open e 'F' para flag):")
    scanf("%c",&symbol);
    symbol=toupper(symbol);

    if (symbol == 'F') {
      board[linha][coluna].flag == 1
    } 
    else if (symbol == 'O') {
      putBombs(linha, coluna);
      openCoord(linha, coluna);
      firstplay = 1;
    }
    else {
      printf("simbolo inválido, digite novamente:\n");
    action(linha, coluna);
    }
}


// Checagem para por bombas longe de onde foi dado o primeiro clique
int preBombs(linha, coluna, l, c) {
  if (board[linha][coluna] == board[l][c] {
    return 1;
  }
  else if (board[linha-1][coluna] == board[l][c] {
    return 1;
  }
  else if (board[linha+1][coluna] == board[l][c] {
    return 1;
  }
  else if (board[linha][coluna-1] == board[l][c] {
    return 1;
  }
  else if (board[linha][coluna+1] == board[l][c] {
    return 1;
  }
  else if (board[linha-1][coluna+1] == board[l][c] {
    return 1;
  }
  else if (board[linha-1][coluna-1] == board[l][c] {
    return 1;
  }
  else if (board[linha+1][coluna-1] == board[l][c] {
    return 1;
  }
  else if (board[linha+1][coluna+1] == board[l][c] {
    return 1;
  }
  return 0;
}

// coloca bombas em locais aleatorios, porém longe do primeiro clique
void putBombs(linha, coluna) {
  int n = 21;
  int i;
  srand(time(NULL));
  if (firstplay == 0) {
    for (i = 1; i <= n; i++){
      l = rand() % length;
      c = rand() % length;
      if (preBombs(linha, coluna, l, c) == 0) {
        if (board[l][c].bomb == 0) {
          board[l][c].bomb = 1;
        }
        else {
          i--;
        }
      }
      else {
        i--;
      }
    }
  }
}

//checa vitória
int checkWin() {
  int close == 0;
  int l, c;
  for (l = 0, l < length, l++ ){
    for (c = 0, c < length, c++ ){
      if (board[l][c].open == 0 && board[l][c].bomb == 0){
        close ++;
      }
    }
  }
  return close;
}

//checa derrota
int checkLost() {
  int explode == 0;
  int l, c;
  for (l = 0, l < length, l++ ){
    for (c = 0, c < length, c++ ){
      if (board[l][c].open == 1 && board[l][c].bomb == 1){
        close ++;
      }
    }
  }
  return explode;
}