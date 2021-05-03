// bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// prototipos de funcoes
void playerInfo(void);
void boardInit(void);
void putBombs(int line, int column);
int preBombs(int line, int column);
void countBombs(void);
void printBoard(void);
void play(void);
void action(int line, int column);
void openCoord(int line, int column);
int checkWin(void);
void savePlayerInfo(char *points);
int checkLost(void);
void timerInit(void);
void timerEnd(void);
int firstPlay = 1;
//void convertTime(void);
char * points(void);
char * convertPointsItoa(int pontos);
int validCoord(int line, int column);
char* itoa(int val, int base);

// struct para cada campo
typedef struct {
  int bomb;           // recebe 0 (false) ou 1 (true)
  int open;           // recebe 0 (false) ou 1 (true)
  int flag;           // recebe 0 (false) ou 1 (true)
  int adjacentBombs;  // recebe um numero entre 0 e 8
} field;

// criacao do tabuleiro (matriz[length][length])
#define length 10
field board[length][length];

char *playerName = NULL;

// declaracao das variaveis globais de tempo
time_t t_ini, t_fim;
int tempo, hour = 0, minute = 0, second = 0;

int main(void) {
  srand(time(NULL));

  playerInfo();

  boardInit();

  play();

  savePlayerInfo(points());

  return 0;
}

// recebe o nome do jogador
void playerInfo(void) {
  playerName = (char *) malloc(51 * sizeof(char));
  if (playerName == NULL) {
    printf("Erro ao alocar memoria.");
    exit(2);
  }

  printf("Digite o nome do jogador:\nEx.: nome_sobrenome\n");
  scanf("%s", playerName);

  playerName = (char *) realloc(playerName, strlen(playerName) * sizeof(char));
  if (playerName == NULL) {
    printf("Erro ao alocar memoria.");
    exit(2);
  }
}

// salva as informacoes no arquivo database.txt
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

// funcao para inicializar a matriz  do  campo minado
void boardInit(void) {
  for (int l = 0; l < length; l++)
    for(int c = 0; c < length; c++){
      board[l][c].bomb = 0;
      board[l][c].open = 0;
      board[l][c].flag = 0;
      board[l][c].adjacentBombs = 0;
    }
}

// funcao para printar o campo
void printBoard(void) {
  printf("\n\n\t  ");

  for (int l = 0; l<length; l++)
    printf("  %d ", l);

  printf("\n\t  -----------------------------------------\n");

  for (int l = 0; l < length; l++){
    printf("\t%d |", l);

    for (int c = 0; c < length; c++){
      if(board[l][c].open)
        if (board[l][c].bomb)
          printf(" * ");
        else
          printf(" %d ", board[l][c].adjacentBombs);
      else if (board[l][c].flag)
        printf(" F ");
      else
        printf("   ");

      printf("|");
    }

    printf("\n\t  -----------------------------------------\n");
  }
}

// verificacao da coordenada no campo.
int validCoord(int line, int column) {
  if(line >= 0 && line < length && column >= 0 && column < length )
    return 1;

  return 0;
}

// funcao que inicia o registro do tempo.
void timerInit(void){
  tempo = 0;
  t_ini = time(NULL);
}

//funcao que finaliza o registro do tempo.
void timerEnd(void) {
  t_fim = time(NULL);
  tempo = difftime(t_fim, t_ini);
}

// calcula a pontuacao do usuario caso ele venca 
char * points(void){
  int points;

  if (tempo != 0)
    points = (100000 / tempo);

  return itoa(points, 10);
}

// transforma os segundos em horas, minutos e segundos.
void convertTime(void){
  hour= tempo / 3600;
  minute = (tempo % 3600) / 60;
  second = (tempo % 3600) % 60;
}

// funcao que converte o valor dos pontos em um vetor alocado dinamicamente
char * convertPointsItoa(int points){
  char *playerPoints = (char *) malloc(101*sizeof(char));

  //itoa(points, playerPoints, 10);

  playerPoints = (char *) realloc(playerPoints, strlen(playerPoints) * sizeof(char));

  return playerPoints;
}

// funcao para converter int para char
char* itoa(int val, int base){
	static char buf[32] = {0};
	
	int i = 30;
	
	for(; val && i ; --i, val /= base)
	
		buf[i] = "0123456789abcdef"[val % base];
	
	return &buf[i+1];
}

// abre a coordenada e caso seja zero, abre as adjacentes 
void openCoord(int line, int column) {
  if (validCoord(line, column) && !board[line][column].open && !board[line][column].flag) {
    board[line][column].open = 1;

    if (board[line][column].adjacentBombs == 0) {
      openCoord(line - 1, column);
      openCoord(line, column - 1);
      openCoord(line - 1, column + 1);
      openCoord(line + 1, column);
      openCoord(line + 1, column - 1);
      openCoord(line + 1, column + 1);
      openCoord(line, column + 1);
      openCoord(line - 1, column - 1);
    }
  }
}

// recebe a jogada
void play(void) {
  int line, column;
  char symbol;

  while (!checkWin() || !checkLost()) {
    printBoard();

    printf("\nDigite a linha e a coluna: ");
    scanf("%d%d", &line, &column);

    if (!validCoord(line, column)) {
      printf("coordenadas invalidas!\n");
      play();
      return;
    }

    action(line, column);
  }
}

// Checa se o player quer abrir ou marcar com uma bandeira
void action(int line, int column) {
  char symbol;

  printf("\nO que dejesa fazer? ('O' para abrir e 'F' para colocar bandeira): ");
  scanf(" %c",&symbol);
  symbol = toupper(symbol);

  if (symbol == 'F') {
    if (board[line][column].flag == 1) {
      board[line][column].flag = 0;
      return;
    }
    else {
      board[line][column].flag = 1;
      return;
    }
  }

  if (symbol == 'O') {
    if (firstPlay) {
      putBombs(line, column);
      countBombs();
      firstPlay = 0;
    }

    openCoord(line, column);
    return;
  }

  printf("Jogada invalida!\n");
  action(line, column);
}

// Coloca bombas em locais aleatorios, porem longe do primeiro clique
void putBombs(int line, int column) {
  int numberOfBombs = (length * length) * 0.25;

  for (int bombs = 0; bombs <= numberOfBombs; bombs++)
    if (preBombs(line, column))
      bombs--;
}

// Checagem para por bombas longe de onde foi dado o primeiro clique
int preBombs(int line, int column) {
  int l = rand() % length, c = rand() % length;

  if (l == line && c == column)
    return 1;
  if (l == line - 1 && c == column)
    return 1;
  if (l == line + 1 && c == column)
    return 1;
  if (l == line && c == column - 1)
    return 1;
  if (l == line && c == column + 1)
    return 1;
  if (l == line - 1 && c == column + 1)
    return 1;
  if (l == line - 1 && c == column - 1)
    return 1;
  if (l == line + 1 && c == column - 1)
    return 1;
  if (l == line + 1 && c == column + 1)
    return 1;

  if (board[l][c].bomb)
    return 1;

  board[l][c].bomb = 1;
  return 0;
}

void countBombs(void) {
  for (int line = 0; line < length; line++)
    for (int column = 0; column < length; column++) {
    if (board[line - 1][column].bomb && validCoord(line-1, column))
      board[line][column].adjacentBombs++;
    if (board[line - 1][column + 1].bomb && validCoord(line-1, column+1))
      board[line][column].adjacentBombs++;
    if (board[line - 1][column - 1].bomb && validCoord(line-1, column-1))
      board[line][column].adjacentBombs++;
    if (board[line][column + 1].bomb && validCoord(line, column+1))
      board[line][column].adjacentBombs++;
    if (board[line][column - 1].bomb && validCoord(line, column-1))
      board[line][column].adjacentBombs++;
    if (board[line + 1][column].bomb && validCoord(line+1, column))
      board[line][column].adjacentBombs++;
    if (board[line + 1][column + 1].bomb && validCoord(line+1, column+1))
      board[line][column].adjacentBombs++;
    if (board[line + 1][column - 1].bomb && validCoord(line+1, column-1)) 
      board[line][column].adjacentBombs++;
    }
}

//checa vitoria
int checkWin(void) {
  for (int l = 0; l < length; l++)
    for (int c = 0; c < length; c++)
      if (!board[l][c].open && !board[l][c].bomb)
        return 1;

  return 0;
}

// FUNCAO DESNECESSARIA
//checa derrota
int checkLost(void) {
  for (int l = 0; l < length; l++ ){
    for (int c = 0; c < length; c++ ){
      if (board[l][c].open && board[l][c].bomb)
        return 1;
    }
  }
  return 0;
}
