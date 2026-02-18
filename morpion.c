#include <stdio.h>
#define NBROW 3
#define NBCOLUMN 3
#define NBSQUARE NBROW*NBCOLUMN

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
  else if (HaveWinDiagonal) return HaveWinDiagonal(tab);
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
    if ((tab[i*3]==tab[1+i*3])&&(tab[i*3]=player)&&(tab[2+i*3]==0)) sum += 1000;
    if ((tab[i*3]==tab[i*3+2])&&(tab[i*3+1]==0)&&(tab[i*3]==player)) sum +=1000;
    if ((tab[i*3+1]==tab[i*3+2])&&(tab[i*3]==0)&&(tab[i*3+1]==player)) sum +=1000;

    // vertical evaluation
    if ((tab[i]==tab[i+3])&&(tab[i+6]==0)&&(tab[i]==player)) return sum +=1000;
    if ((tab[i]==tab[i+6])&&(tab[i+3]==0)&&(tab[i]==player)) return sum +=1000;
    if ((tab[i+3]==tab[i+6])&&(tab[i]==0)&&(tab[i]==player)) return sum +=1000;
  } 

  // diagonal verification
  if ((tab[0]==tab[4])&&(tab[8]==0)&&(tab[0]==player)) return sum +=1000;
  if ((tab[8]==tab[4])&&(tab[0]==0)&&(tab[8]==player)) return sum +=1000;
  if ((tab[0]==tab[8])&&(tab[4]==0)&&(tab[0]==player)) return sum +=1000;

  if ((tab[2]==tab[4])&&(tab[6]==0)&&(tab[2]==player)) return sum +=1000;
  if ((tab[2]==tab[6])&&(tab[4]==0)&&(tab[2]==player)) return sum +=1000;
  if ((tab[4]==tab[6])&&(tab[2]==0)&&(tab[4]==player)) return sum +=1000;

  return sum;
}

unsigned int CountGrid(unsigned short *tab){
  return Eval(tab, 2) - Eval(tab,1);
}

int min_max(unsigned short *tab, unsigned short profondeur, unsigned short actual_player){
  unsigned short IA = 2;
  unsigned short player = 1;
  int score, best_score=0, worse_score=0;

  if (GridFull(tab) || profondeur == 0) return CountGrid(tab);
  if(actual_player == IA){
    for(int i=0;i<NBSQUARE;i++){
      if (!tab[i]) {
        tab[i] = IA;
        score = min_max(tab, profondeur-1, player);
        tab[i] = 0;
        if (score>best_score) best_score=score;
      }
    }
    return best_score;
  }
  if (actual_player == player) {
    for(int i=0;i<NBSQUARE;i++){
      if (!tab[i]) {
        tab[i] = player;
        score = min_max(tab, profondeur-1, IA);
        tab[i] = 0;
        if (score<worse_score) worse_score=score;
      }
    }
  }
  
}

int main(){
  unsigned short tab[NBSQUARE]; 
  unsigned short position;
  CreateGrid(tab);
  while (!(HaveWin(tab))) {
    ShowGrid(tab);
    position = AskPosition(tab,1);
    PutPiece(tab, position, 1);
    ShowGrid(tab);
    position = AskPosition(tab,2);
    PutPiece(tab, position,2);
    printf("Le score pour cette grile est de %d: \n", Eval(tab,1));
  }
  ShowGrid(tab);
  return 0;
}
