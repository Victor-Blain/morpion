#include <stdio.h>
#include <unistd.h>
#define NBROW 3
#define NBCOLUMN 3
#define NBSQUARE NBROW*NBCOLUMN
#define DEPTH 10

void create_grid(unsigned short *tab){
  for (int i=0; i<NBSQUARE;i++) tab[i]= 0;
}

void show_grid(unsigned short *tab){
  char symbols[] = {' ', 'X', 'O'};
  printf("    0   1   2\n");
  printf("0 | %c | %c | %c |\n", symbols[tab[0]],symbols[tab[1]], symbols[tab[2]]);
  printf("3 | %c | %c | %c |\n", symbols[tab[3]],symbols[tab[4]], symbols[tab[5]]);
  printf("6 | %c | %c | %c |\n", symbols[tab[6]],symbols[tab[7]], symbols[tab[8]]);
}

unsigned short have_win_row(unsigned short *tab) {
  for (int i=0; i<NBSQUARE;i+=3){
    if ((tab[i]==tab[i+1]) && (tab[i]==tab[i+2]) && tab[i]) return tab[i];
  }
  return 0;
}
unsigned short have_win_column(unsigned short *tab) {
  for (int i=0; i<NBCOLUMN;i++) {
    if ((tab[i]==tab[i+3]) && (tab[i]==tab[i+6]) && (tab[i])) return tab[i];
  }
  return 0;
}
unsigned short have_win_diagonal(unsigned short *tab) {
  if ((tab[0] == tab[4]) && (tab[0] == tab[8]) && (tab[0])) return tab[0];
  else if ((tab[6] == tab[4]) && (tab[6] == tab[2]) && (tab[6])) return tab[6];
  else return 0;
}
unsigned short have_win(unsigned short *tab) {
  if (have_win_row(tab)) return have_win_row(tab);
  else if (have_win_column(tab)) return have_win_column(tab);
  else if (have_win_diagonal(tab)) return have_win_diagonal(tab);
  else return 0;
}

void put_piece(unsigned short *tab,unsigned short position, unsigned short player){
  tab[position] = player;
}

unsigned short ask_position(unsigned short *tab, unsigned short player){
  unsigned short position;
  char symbols[] = {' ', 'X', 'O'};
  do {
  printf("Player %c, can you give me a position (0-8) to play ? \n", symbols[player]);
  scanf("%hu", &position);
  } while (position > 8 || tab[position] != 0);
  return position;
}

unsigned short grid_full(unsigned short *tab){
  for (int i=0;i<NBSQUARE;i++){
    if (tab[i]==0) return 0;
  }
  return 1;
}

unsigned int eval(unsigned short *tab, unsigned short player){
  int sum=0;
  for(int i=0;i<NBSQUARE;i++){
    if ((i==0)||(i==2)||(i==6)||(i==8)) {
      if (tab[i]==player) sum +=50;
    }
    else if (i==4) {
      if (tab[i]==player) sum +=70;
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

  if (have_win(tab)==player) sum += 10000;
  return sum;
}

int count_gird(unsigned short *tab, unsigned short player_1, unsigned short player_2){
  return eval(tab, player_2) - eval(tab,player_1);
}

int min_max(unsigned short *tab, unsigned short depth, unsigned short player_1,unsigned short player_2, unsigned short is_maximizing){
  int score, best_score=-100000, worse_score=100000;
  if ((depth == 0)||(grid_full(tab))) return count_gird(tab, player_1, player_2);
  if (have_win(tab)==player_1) return 10000+depth*3;
  if (have_win(tab)==player_2) return -10000-depth*3;
  if(is_maximizing == 1){
    for(int i=0;i<NBSQUARE;i++){
      if (!tab[i]) {
        tab[i] = player_1;
        score = min_max(tab, depth-1, player_1, player_2, 0);
        tab[i] = 0;
        if (score>best_score) best_score=score;
      }
    }
    return best_score;
  }
  else {
    for(int i=0;i<NBSQUARE;i++){
      if (!tab[i]) {
        tab[i] = player_2;
        score = min_max(tab, depth-1, player_1, player_2, 1);
        tab[i] = 0;
        if (score<worse_score) worse_score=score;
      }
    }
    return worse_score;
  }
}

unsigned short best_shot(unsigned short *tab,unsigned short depth, unsigned short player_1, unsigned short player_2) {
  unsigned short position=10;
  int score, best_score=-100000;
  for (int i=0;i<NBSQUARE;i++) {
    if (tab[i]==0) {
      tab[i]=player_1;
      score = min_max(tab,DEPTH-1,player_1,player_2,0);
      tab[i]=0;
      if (score>best_score) {
        best_score = score;
        position=i;
      }
    }
  }
  if (position==10) printf("ALERTE ROUGE !!!!! %d\n", best_score);
  printf("pos %u score %d\n",position, best_score);
  return position;
}

unsigned short end_game(unsigned short *tab){
  if (have_win(tab)==1) {
    printf("You win !!!\n");
    return 1;
  }
  else if (have_win(tab)==2) {
    printf("You lose...\n");
    return 2; 
  }
  else if (grid_full(tab)) {
    printf("The grid is full, DRAW !\n");
    return 3;
  }
  return 0;
}

void IA_turn(unsigned short *tab, unsigned short player,unsigned short other_player){
  printf("IA turn\n");
  unsigned short position = best_shot(tab, DEPTH, player, other_player);
  printf("position chosi : %u\n", position);
  put_piece(tab, position, player);
}

void player_turn(unsigned short *tab, unsigned short player){
  printf("Your turn player : %u\n", player);
  show_grid(tab);
  unsigned short position = ask_position(tab, player);
  put_piece(tab, position, player);
}

void game_contre_IA(){
  unsigned short PLAYER = 1;
  unsigned short IA = 2;
  unsigned short tab[NBSQUARE];
  create_grid(tab);
  for(;;) {
    player_turn(tab, PLAYER);
    if (end_game(tab)) break;
    IA_turn(tab, IA, PLAYER);
    if (end_game(tab)) break;
  }
}

void game_IA_IA(){
  unsigned short IA_1=1, IA_2=2;
  unsigned short tab[NBSQUARE];
  create_grid(tab);
  for (;;){
    sleep(1);
    IA_turn(tab, IA_1, IA_2);
    show_grid(tab);
    if (end_game(tab)) break;
    sleep(1);
    IA_turn(tab, IA_2, IA_1);
    show_grid(tab);
    if (end_game(tab)) break;
  }
}

int main(){
  game_IA_IA();
  game_contre_IA();
  return 0;
}
