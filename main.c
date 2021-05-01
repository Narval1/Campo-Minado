  // bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// prototipos de funcoes
void playerInfo(void);
void savePlayerInfo(char *points);
int validCoord(int line, int column);
void openCoord(int line, int column);
void play(void);
void action(int line, int column);
void putBombs(int line, int column);
int preBombs(int line, int column);
int checkWin(void);
int checkLost(void);
int timer_inicio(void);
int timer_fim(void)
int points(void);
int *convertpointsitoa(int pontos);
int convertime(void);
 
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


// declaração das variáveis globais de tempo
int tempo=0;
time_t t_ini, t_fim;
int hour=0, minute=0, second=0; // usei como global pra não ter que passar três parâmetros e etc 

int main(void) {
  playerInfo();

  savePlayerInfo("points");   // ultima funcao a ser chamada

  return 0;
}

// recebe o nome do jogador
void playerInfo(void) {
  playerName = (char *) malloc(51 * sizeof(char));
  if (playerName == NULL) {
    printf("Erro ao alocar memoria.");
    exit(2);
  }

  printf("Digite o nome do jogador:\nEx.: nome_sobrenome\n\n");
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

void BoardInit(){
  int c, l;
    for (int l = 0; l < length; l++){
        for(c = 0; c < length; c++){
            board[l][c].bomb = 0;
            board[l][c].open = 0;
            board[l][c].flag = 0;
            board[l][c].adjacentBombs = 0;
        }
    }
}

// funcao para printar o campo
void PrintBoard(){
    printf("\n\n\t  ");
    for (int l = 0; l<length; l++){
        printf("  %d ", l);
    }
    printf("\n\t  -----------------------------------------\n");
    for (int l = 0; l < length; l++){
        printf("\t%d |", l);
        for (int c = 0; c < length; c++){
            if(board[l][c].open){
                if (board[l][c].bomb) printf(" * ");
                else printf(" %d ", board[l][c].adjacentBombs);
            }
            else if (board[l][c].flag) printf(" |> ");
            else printf("   ");
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

// função que inicia o registro do tempo.
int timer_inicio(void){
  tempo=0;
  t_ini= time(NULL);
}

//função que finaliza o registro do tempo caso o usuário vença.
int timer_fim(void){   // Chamar essa função quando o usuário ganhar.
  if(checkwin(void)==1){
    t_fim= time(NULL);
    tempo= difftime(t_fim,t_ini);
    points();
  }
}

// calcula a pontuação do usuário caso ele vença 
int points(void){
   int pontos=0;

  if (tempo!=0)
    pontos=(100000/tempo); // usei o 1000000 pra impedir que a divisão seja menor do que 1 por ser inteiro e o 126 pra dar uma estética legal no valor da pontuação  

  convertpointsitoa(pontos);

  return pontos;   
}

// transforma os segundos em horas, minutos e segundos.
int convertime(void){
  int resto; 
  
  hour= tempo/3600;
  resto = tempo % 3600; 
  
  minute = resto/60;
  second = resto%60; 

  return;
}

// função que converte o valor dos pontos em um vetor alocado dinamicamente
int *convertpointsitoa(int pontos){
  char *arr=NULL;

  arr= (char*) malloc(50*sizeof(char));
  itoa(pontos, arr, 10);

  arr=(char *) realloc(arr, strlen(arr)*sizeof(char)); // se eu não fiz cagada no realloc, é isso 

  return(arr);
}

// abre a coordenada e caso seja zero, abre as adjacentes 
void openCoord(int line, int column) {
  if (validCoord(line, column) && !board[line][column].open && !board[line][column].flag) {
    board[line][column].open = 1;

    if (board[line][column].adjacentBombs == 0) {
      openCoord(line - 1, column);
      openCoord(line - 1, column - 1);
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

  printf("\nDigite a linha e a coluna: ");
  scanf("%d%d", &line, &column);
  timer_inicio();

  if (!validCoord(line, column)) {
    printf("coordenadas invalidas!\n");
    play();
    return;
  }

  action(line, column);
}

// Checa se o player quer abrir ou marcar com uma bandeira
void action(int line, int column) {
  int i;
  int firstPlay = 1;
  char symbol;

  printf("\nO que dejesa fazer? ('O' para abrir e 'F' para colocar bandeira):");
  scanf(" %c",&symbol);
  symbol = toupper(symbol);

  if (symbol == 'F') {
    board[line][column].flag = 1;
    return;
  }

  if (symbol == 'O') {
    if (firstPlay) {
      putBombs(line, column);
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
  int i;
  int numberOfBombs = (length * length) * 0.25;

  for (int bombs = 0; bombs <= numberOfBombs; bombs++){
    if (preBombs(line, column))
      i--;
  }
}

// Checagem para por bombas longe de onde foi dado o primeiro clique
int preBombs(int line, int column) {
  srand(time(NULL));
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

