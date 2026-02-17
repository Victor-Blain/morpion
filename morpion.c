#include <stdio.h>
#define NbRow 3
#define NbColumn 3
#define NbSquare NbRow*NbColumn

void CreateGrid(unsigned short int tab[NbSquare]){
  for (int i=0; i<NbSquare;i++) tab[i]= 0;
}

void ShowGrid(unsigned short int tab[NbSquare]){
  char symbols[] = {' ', 'X', 'O'};
  printf("    0   1   2\n");
  printf("0 | %c | %c | %c |\n", symbols[tab[0]],symbols[tab[1]], symbols[tab[2]]);
  printf("3 | %c | %c | %c |\n", symbols[tab[3]],symbols[tab[4]], symbols[tab[5]]);
  printf("6 | %c | %c | %c |\n", symbols[tab[6]],symbols[tab[7]], symbols[tab[8]]);
}

unsigned short int HaveWinRow(unsigned short int tab[NbSquare]) {
  for (int i=0; i<NbSquare;i+=3){
    if ((tab[i]==tab[i+1]) && (tab[i]==tab[i+2]) && tab[i]) return tab[i];
  }
  return 0;
}

unsigned short int HaveWinColumn(unsigned short int tab[NbSquare]) {
  for (int i=0; i<NbColumn;i++) {
    if ((tab[i]==tab[i+3]) && (tab[i]==tab[i+6]) && (tab[i])) return tab[i];
  }
  return 0;
}

unsigned short int HaveWinDiagonal(unsigned short int tab[NbSquare]) {
  if ((tab[0] == tab[4]) && (tab[0] == tab[8]) && (tab[0])) return tab[0];
  else if ((tab[6] == tab[4]) && (tab[6] == tab[2]) && (tab[6])) return tab[6];
  else return 0;
}

unsigned short int HaveWin(unsigned short int tab[NbSquare]) {
  if (HaveWinRow(tab)) return HaveWinRow(tab);
  else if (HaveWinColumn(tab)) return HaveWinColumn(tab);
  else if (HaveWinDiagonal) return HaveWinDiagonal(tab);
  else return 0;
}

void PutPiece(unsigned short int tab[NbSquare],unsigned short int position, unsigned short int player){
  tab[position] = player;
}

unsigned short int AskPosition(unsigned short int tab[NbSquare], unsigned short int player){
  unsigned short int position;
  char symbols[] = {' ', 'X', 'O'};
  do {
  printf("Player %c, can you gave me a position (0-8) to play ? \n", symbols[player]);
  scanf("%1hu", &position);
  } while (tab[position] != 0);
  return position;
}

unsigned short int GridFull(unsigned short int tab[NbSquare]){
  for (int i=0;i<NbSquare;i++){
    if (tab[i]==0) return 0;
  }
  return 1;
}

int Eval(unsigned short int tab[NbSquare]){
  int sum=0;
  for(int i=0;i<NbSquare;i++){
    if ((i==0)||(i==2)||(i==6)||(i==8)) {
      if (tab[i]==1) sum -=50;
      if (tab[i]==2) sum +=50;
    }
    else if (i==4) {
      if (tab[i]==2) sum +=10;
      if (tab[i]==1) sum -= 10;
    }
    else {
      if (tab[i]==2) sum +=1;
      if (tab[i]==1) sum -= 1;
    }
  }
  for(int i=0;i<NbRow;i++){
    if ((tab[i*3]==tab[1+i*3])&&(tab[i*3]=2)&&(tab[2+i*3]==0)) sum += 1000;
    if ((tab[i*3]==tab[i*3+2])&&(tab[i*3+1]==0)&&(tab[i*3]==2)) sum +=1000;
    if ((tab[i*3+1]==tab[i*3+2])&&(tab[i*3]==0)&&(tab[i*3+1]==2)) sum +=1000;
    if ((tab[i*3]==tab[1+i*3])&&(tab[i*3]=1)&&(tab[2+i*3]==0)) sum -= 1000;
    if ((tab[i*3]==tab[i*3+2])&&(tab[i*3+1]==0)&&(tab[i*3]==1)) sum -=1000;
    if ((tab[i*3+1]==tab[i*3+2])&&(tab[i*3]==0)&&(tab[i*3+1]==1)) sum -=1000;

    if ((tab[i]==tab[i+3])&&(tab[i+6]==0)&&(tab[i]==2)) return +=1000;
    if ((tab[i]==tab[i+6])&&(tab[i+3]==0)&&(tab[i]==2)) return +=1000;
    if ((tab[i+3]==tab[i+6])&&(tab[i]==0)&&(tab[i]==2)) return +=1000;
    if ((tab[i]==tab[i+3])&&(tab[i+6]==0)&&(tab[i]==1)) return -=1000;
    if ((tab[i]==tab[i+6])&&(tab[i+3]==0)&&(tab[i]==1)) return -=1000;
    if ((tab[i+3]==tab[i+6])&&(tab[i]==0)&&(tab[i+3]==1)) return -=1000;
  } 
  return sum;
}
int main(){
  unsigned short int tab[NbSquare]; 
  unsigned short int position;
  CreateGrid(tab);
  while (!(HaveWin(tab))) {
    ShowGrid(tab);
    position = AskPosition(tab,1);
    PutPiece(tab, position, 1);
    ShowGrid(tab);
    position = AskPosition(tab,2);
    PutPiece(tab, position,2);
    printf("Le score pour cette grile est de %d: \n", Eval(tab));
  }
  ShowGrid(tab);
  return 0;
}
