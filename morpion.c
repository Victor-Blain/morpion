#include <stdio.h>
#define NBROW 3
#define NBCOLUMN 3
#define NBSQUARE NBROW*NBCOLUMN
#define DEPTH 10
#define IA 2
#define PLAYER 1

void CreateGrid(unsigned short *tab){
  for (int i=0; i<NBSQUARE;i++) tab[i]= 0;
}

void ShowGrid(unsigned short *tab){
  char symbols[] = {' ', 'X', 'O'};
  printf("    0   1   2\n");
  printf("0 | %c | %c | %c |\n", symbols[tab[0]],symbols[tab[1]], symbols[tab[2]]);
  printf("3 | %c | %c | %c |\n", symbols[tab[3]],symbols[tab[4]], symbols[tab[5]]);
  printf("6 | %c | %c | %c |\n", symbols[tab[6]],symbols[tab[7]], symbols[tab[8]]);
}

unsigned short HaveWinRow(unsigned short *tab) {
  for (int i=0; i<NBSQUARE;i+=3){
    if ((tab[i]==tab[i+1]) && (tab[i]==tab[i+2]) && tab[i]) return tab[i];
  }
  return 0;
}

unsigned short HaveWinColumn(unsigned short *tab) {
  for (int i=0; i<NBCOLUMN;i++) {
    if ((tab[i]==tab[i+3]) && (tab[i]==tab[i+6]) && (tab[i])) return tab[i];
  }
  return 0;
}

unsigned short HaveWinDiagonal(unsigned short *tab) {
  if ((tab[0] == tab[4]) && (tab[0] == tab[8]) && (tab[0])) return tab[0];
  else if ((tab[6] == tab[4]) && (tab[6] == tab[2]) && (tab[6])) return tab[6];
  else return 0;
}

unsigned short HaveWin(unsigned short *tab) {
  if (HaveWinRow(tab)) return HaveWinRow(tab);
  else if (HaveWinColumn(tab)) return HaveWinColumn(tab);
  else if (HaveWinDiagonal(tab)) return HaveWinDiagonal(tab);
  else return 0;
}

void PutPiece(unsigned short *tab,unsigned short position, unsigned short player){
  tab[position] = player;
}

unsigned short AskPosition(unsigned short *tab, unsigned short player){
  unsigned short position;
  char symbols[] = {' ', 'X', 'O'};
  do {
  printf("Player %c, can you gave me a position (0-8) to play ? \n", symbols[player]);
  scanf("%1hu", &position);
  } while (tab[position] != 0);
  return position;
}

unsigned short GridFull(unsigned short *tab){
  for (int i=0;i<NBSQUARE;i++){
    if (tab[i]==0) return 0;
  }
  return 1;
}

unsigned int Eval(unsigned short *tab, unsigned short player){
  int sum=0;
  for(int i=0;i<NBSQUARE;i++){
    if ((i==0)||(i==2)||(i==6)||(i==8)) {
      if (tab[i]==player) sum +=50;
    }
    else if (i==4) {
      if (tab[i]==player) sum +=10;
    }
    else {
      if (tab[i]==player) sum +=1;
    }
  }
  for(int i=0;i<NBROW;i++){
    // horizontal evaluation
    if ((tab[i*3]==tab[1+i*3])&&(tab[i*3]==player)&&(tab[2+i*3]==0)) sum += 1000;
    if ((tab[i*3]==tab[i*3+2])&&(tab[i*3+1]==0)&&(tab[i*3]==player)) sum +=1000;
    if ((tab[i*3+1]==tab[i*3+2])&&(tab[i*3]==0)&&(tab[i*3+1]==player)) sum +=1000;

    // vertical evaluation
    if ((tab[i]==tab[i+3])&&(tab[i+6]==0)&&(tab[i]==player)) sum +=1000;
    if ((tab[i]==tab[i+6])&&(tab[i+3]==0)&&(tab[i]==player)) sum +=1000;
    if ((tab[i+3]==tab[i+6])&&(tab[i]==0)&&(tab[i]==player)) sum +=1000;
  } 

  // diagonal verification
  if ((tab[0]==tab[4])&&(tab[8]==0)&&(tab[0]==player)) sum +=1000;
  if ((tab[8]==tab[4])&&(tab[0]==0)&&(tab[8]==player)) sum +=1000;
  if ((tab[0]==tab[8])&&(tab[4]==0)&&(tab[0]==player)) sum +=1000;

  if ((tab[2]==tab[4])&&(tab[6]==0)&&(tab[2]==player)) sum +=1000;
  if ((tab[2]==tab[6])&&(tab[4]==0)&&(tab[2]==player)) sum +=1000;
  if ((tab[4]==tab[6])&&(tab[2]==0)&&(tab[4]==player)) sum +=1000;

  return sum;
}

unsigned int CountGrid(unsigned short *tab){
  return Eval(tab, 2) - Eval(tab,1);
}

int min_max(unsigned short *tab, unsigned short depth, unsigned short actual_player){
  int score, best_score=-10000, worse_score=10000;

  if (HaveWin(tab)==1) return -1000;
  if (HaveWin(tab)==2) return 1000;
  if (GridFull(tab)) return 0;
  if (depth == 0) return CountGrid(tab);
  if(actual_player == IA){
    for(int i=0;i<NBSQUARE;i++){
      if (!tab[i]) {
        tab[i] = IA;
        score = min_max(tab, depth-1, PLAYER);
        tab[i] = 0;
        if (score>best_score) best_score=score;
      }
    }
    return best_score;
  }
  if (actual_player == PLAYER) {
    for(int i=0;i<NBSQUARE;i++){
      if (!tab[i]) {
        tab[i] = PLAYER;
        score = min_max(tab, depth-1, IA);
        tab[i] = 0;
        if (score<worse_score) worse_score=score;
      }
    }
  return worse_score;
  } 
}

unsigned short best_shot(unsigned short *tab,unsigned short depth) {
  unsigned short position=10;
  int score, best_shot=-1000;
  for (int i=0;i<NBSQUARE;i++) {
    if (tab[i]==0) {
      score = min_max(tab,DEPTH, 2);
      if (score>best_shot) {
        best_shot = score;
        position=i;
      }
    }
  }
  if (position==10) printf("On a un soucis dans la fonction best_shot\n");
  return position;
}

void game_contre_IA(){
  unsigned short tab[NBSQUARE], position, win=0;
  CreateGrid(tab);
  while (!(win)) {
    printf("A VOUS DE JOUER\n"); // a modifier
    ShowGrid(tab);
    position = AskPosition(tab, PLAYER);
    PutPiece(tab, position, PLAYER);
    ShowGrid(tab);
    if (HaveWin(tab)) win=1;
    printf("L'IA VA JOUER\n"); // a modifier
    position = best_shot(tab, DEPTH);
    PutPiece(tab, position, IA);
    if (HaveWin(tab)) win=1;
  }
}

int main(){
  game_contre_IA();
  return 0;
}
