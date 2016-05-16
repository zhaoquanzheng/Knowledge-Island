//game.c

//TODO put this somewhere in the code
//   int disciplines[] = DEFAULT_DISCIPLINES;
//   int dice[] = DEFAULT_DICE;
//   Game g = newGame (disciplines, dice);

#include "Game.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#define DEFAULT_DISCIPLINES {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ, \
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV, \
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN, \
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ, \
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS}
#define DEFAULT_DICE {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5} 
//I'm putting this here because they said that we weren't allowed to change Game.h and this is commented in Game.h
#define TOTAL_ARCS 72
#define TOTAL_VERTICES 54
#define TOTAL_DISCIPLINES 6
#define TOTAL_SURROUNDING_REGIONS 6
#define TOTAL_ADJACENT_VERTICES 3
#define TOTAL_ADJACENT_ARCS 4
#define TOTAL_JOINING_ARCS 3
#define TOTAL_RETRAINING_PORTS 10
#define TOTAL_RETRAINING_CENTRES 5
 
#define VERTICES_PER_HEX 6
#define EDGES_PER_HEX 6
#define ARC_ENDPOINTS 2
#define SEA_CONST 3
#define X_COORD_MIN -2
#define X_COORD_MAX 2
#define REGIONS_PER_VERTEX 3
#define MAX_GO8 8
 
#define FALSE_REGION (region) {9, 9}
 
#define CAMPUS_KPI 10
#define GO8_KPI 20
#define ARC_KPI 2
#define IP_KPI 10
#define PRESTIGE_KPI 10
#define INVALID 0

#define E_EAST 0
#define S_EAST 1
#define S_WEST 2
#define W_WEST 3
#define N_WEST 4
#define N_EAST 5

typedef struct _vector{
   int previousX;
   int previousY;
   int x;
   int y;
   int ownership;
} vector;
typedef vector * Vector;

typedef struct _player{
  char playername[30]; //maximum of 30 characters
  int KPI;
  int students[TOTAL_DISCIPLINES]; //the value at each index represents the number of the students
  int onDiscount[TOTAL_DISCIPLINES]; //for getExchangeRate
  int patent;
  int papers;
  int numCampuses;
  int numGO8s;
  int numARCs;
}  Player;

typedef struct _board{
   int diceNum;
   int discipline;
   int vertice[6];
   int edge[6];
}  Tile;

typedef struct _game{ //to be Updated as time goes by (regularly!!!)
   int currentTurn;
   int diceThrow;
   Player players[3]; //where Player is a pointer to a struct _player
   Tile board[19];
   int mostArcs;
   int mostArcsBy;
   int mostCampuses;
   int mostCampusesBy;
   int mostGO8;
   int mostGO8By;
   int mostIP;
   int mostIPby;
   int mostPaper;
   int mostPaperby;
   int KPI;
   int mostKPIby;  
   int disboard[12][11]; //DO NOT CONFUSE DISBOARD WITH EARTH
   vector disArc[TOTAL_ARCS]; //has two sets of coordinates as well as an int for ownership
   int ArcCounter;
} game;

//Arc and VertexIndex is now stored as Coord, convertible in their related functions
//E.g. throwDice checks Vertexes, and isLegalAction has both, 
int getArcIndexFromCoord(Game g, path p);
int getvertexIndexFromCoord(Game g, path p);

vector getVector(Game g, path p); //MAGIC #ZQZ #NGNL
void updateGameBoard(Game g);
/*
int main(int argc, char * argv[]){
//  testFunction();
  return EXIT_SUCCESS;
}
*///no main exists
   //and then the list of functions that we may want in our code
   /* 1)changing path information into coordinates of a vertice of the hexagon
      2)changing path information into a specific arc (2 coordinates?)
      3)roll a dice that has values in the interval [2,12]
      4)scanf functions that read in the path, the action, the name of the player etc
      5)calculate the distance away from one point (or between two points) - used in isLegal() to determine if the campuses are
         too close together or not
      6)
   */

Game newGame(int discipline[], int dice[]){
   Game newG = (Game) malloc(sizeof(struct _game));
   newG->currentTurn = -1;
      
   //initialising struct _board
   int a = 0;
   int b = 0;
   while(a < 19){
      b = 0;
      newG->board[a].discipline = discipline[a];
      newG->board[a].diceNum    = dice[a];
      while(b < 6){
         newG->board[a].vertice[b] = 0;
         newG->board[a].edge[b]    = 0;
         b++;
      }
      a++; 
   }
   
   newG->mostArcs        = 0;
   newG->mostArcsBy      = NO_ONE;
   newG->mostCampuses = 2;
   newG->mostCampusesBy = UNI_A;
   newG->mostGO8 = 0;
   newG->mostGO8By = NO_ONE;
   newG->mostIP = 0;
   newG->mostIPby = NO_ONE;
   newG->mostPaper = 0;
   newG->mostPaperby = NO_ONE;
   newG->KPI = 20;
   newG->mostKPIby = UNI_A;
   
   //setting up the world of No Game No Life
   a = 0;
   b = 0;
   while(a < 12){
      b = 0;
      while(b < 11){
         newG->disboard[a][b] = 0;
         b++;
      }
      a++;
   }
   
   //initialising struct _vector
   a = 0;
   while(a < 72){
      newG->disArc[a].ownership = 0;
      a++;
   }
   newG->ArcCounter = 0;
   newG->disboard[5][0] = CAMPUS_A;
   newG->disboard[6][10]= CAMPUS_A;
   newG->disboard[0][3] = CAMPUS_B;
   newG->disboard[11][7]= CAMPUS_B;
   newG->disboard[1][8] = CAMPUS_C;
   newG->disboard[10][2]= CAMPUS_C;
   updateGameBoard(newG);
   
   //initialising struct_player
   a = 0;
   while(a < 3){
      newG->players[a].students[STUDENT_BPS] = 3;
      newG->players[a].students[STUDENT_BQN] = 3;
      newG->players[a].students[STUDENT_MTV] = 1;
      newG->players[a].students[STUDENT_MJ] = 1;
      newG->players[a].students[STUDENT_MMONEY] = 1;
      newG->players[a].students[STUDENT_THD] = 0;
      b = 0;
      while(b < TOTAL_DISCIPLINES){
         newG->players[a].onDiscount[b] = 0;
      }
      newG->players[a].patent = 0;
      newG->players[a].papers = 0;
      newG->players[a].numCampuses = 0;
      newG->players[a].numGO8s= 0;
      newG->players[a].numARCs= 0;
      newG->players[a].KPI    = 20;
      a++;
   }
   return newG;
} //We need to improve on struct_game, and then I'll have to add those changes to initialisation

void disposeGame (Game g){
   free(g);
}

void makeAction (Game g, action a){
   //Dice is already thrown before they take their action
   //when we have written runGame.c, the following becomes usable
   /* while(a != 0){
    *    get(action) - because they are allowed to take multiple actions per turn
    *    ^above is wrong, that loop is written in runGame.c not makeAction()
    */
   int currentPlayer = getWhoseTurn(g); //where currentPlayer shall be 1,2, or 3
   char* pathway = a.destination;
   if(a.actionCode == BUILD_CAMPUS){
      //error we still haven't decided on how to code the vertexes
      vector campus = getVector(g,pathway);
      assert(campus.ownership == NO_ONE);//has to belong to NO_ONE before you can buy it
      g->disboard[campus.x][campus.y] = currentPlayer;
      updateGameBoard(g);
      if((campus.x==3&&campus.y==1)||(campus.x==4&&campus.y==1)){
         g->players[currentPlayer-1].onDiscount[STUDENT_MTV] = 1; //1 means discount; 0 means not
      } else if((campus.x==7&&campus.y==1)||(campus.x==8&&campus.y==1)){
         g->players[currentPlayer-1].onDiscount[STUDENT_MMONEY] = 1;
      } else if((campus.x==2&&campus.y==8)||(campus.x==3&&campus.y==9)){
         g->players[currentPlayer-1].onDiscount[STUDENT_BPS] = 1;
      } else if((campus.x==8&&campus.y==9)||(campus.x==9&&campus.y==8)){
         g->players[currentPlayer-1].onDiscount[STUDENT_MJ] = 1;
      } else if((campus.x==10&&campus.y==6)||(campus.x==11&&campus.y==5)){
         g->players[currentPlayer-1].onDiscount[STUDENT_BQN]  = 1;
      }
      g->players[currentPlayer-1].numCampuses++;  
      
   }else if(a.actionCode == BUILD_GO8){
      vector campus = getVector(g,pathway);
      assert(g->disboard[campus.x][campus.y]==currentPlayer);
      g->disboard[campus.x][campus.y] += 3;
      //GO8 are 4,5,6 compared to 1,2,3
      updateGameBoard(g);
      g->players[currentPlayer-1].numGO8s++;
   }else if(a.actionCode == OBTAIN_ARC){
      vector Arc = getVector(g,pathway);
      assert(Arc.ownership==NO_ONE);
      int counter = g->ArcCounter;
      if(counter >= 72){
         //printf("No arcs left. If the assert failed, somehow security vulnerable...")
         assert(0==1);
      }else{
         g->disArc[counter].ownership = currentPlayer;
         g->disArc[counter].previousX = Arc.previousX;
         g->disArc[counter].previousY = Arc.previousY;
         g->disArc[counter].x = Arc.x;
         g->disArc[counter].y = Arc.y;
         g->ArcCounter++;
         g->players[currentPlayer-1].numARCs++;
      }   
      
   }else if(a.actionCode == OBTAIN_PUBLICATION){ //because the function that is 
                                                //about to call makeAction()
                                                //changes SPINOFF to either Pp, or IP
      g->players[currentPlayer].papers++;
   }else if(a.actionCode == OBTAIN_IP_PATENT){
      //because IP is a paper
      g->players[currentPlayer].patent++;
      g->players[currentPlayer].papers++;
   }else if(a.actionCode == RETRAIN_STUDENTS){
      int price = getExchangeRate (g, currentPlayer, a.disciplineFrom, a.disciplineTo);
      g->players[currentPlayer].students[a.disciplineFrom] -= price;
      g->players[currentPlayer].students[a.disciplineTo] ++;
      
   } //else = pass
   
}

void throwDice( Game g, int score){
//NOTE:THIS FUNCTION RELIES HEAVILY ON HOW WE STORE THE INFORMATION OF CAMPUSES ON THE MAP
//WHETHER IT BE BY A COMBINATION OF THREE HEXAGONS, OR MAINLY AS A COORDINATE
   //I'll need to know which campuses and which players are on which vertex of the regions
   int i = 0;
   int students[4][6] = {{0}};
   while(i < 19){
      if(g->board[i].diceNum == score){
         int type = getDiscipline(g,i);
         int a = 0;
         while(a<6){
            int corner = g->board[i].vertice[a]; //corner holds campusID or GO8ID
            if(corner>=CAMPUS_A && corner<=CAMPUS_C){    //1,2,3
               students[corner][type]++;
            }else if(corner>=GO8_A && corner<=GO8_C){
               students[corner-3][type] += 2;
            }
            a++;
         }
      }
      i++;
   }
   int x = 1;
   int y = 0;
   while(x <= 3){
      y = 0;
      while(y<6){
         g->players[x-1].students[y]+=students[x][y];
         y++;
      }
      x++;
   }
   
   if(score == 7){
      int a = 0;
      int buffer = 0;
      while(a < 3){
         buffer += g->players[a].students[STUDENT_MTV];
         g->players[a].students[STUDENT_MTV] = 0;
         buffer += g->players[a].students[STUDENT_MMONEY];
         g->players[a].students[STUDENT_MMONEY] = 0;
         g->players[a].students[STUDENT_THD] += buffer;
         buffer = 0;
         a++;
      }
   }
   
   g->currentTurn++;
   //I need to go through code to check what increasing turn count does
   //after the turn number has increased, there is a player's turn
   //this means that after the player's turn throwDice() should be called instead of 
   //increasing turn number
}

int getDiscipline (Game g, int regionID){
   // what type of students are produced by the specified region?
   // regionID is the index of the region in the newGame arrays (above) 
   // see discipline codes above
	int discipline;
	discipline = g->board[regionID].discipline;
	return discipline;
} 

int getDiceValue (Game g, int regionID){
   // what dice value produces students in the specified region?
   // 2..12
	int diceValue;
	diceValue  = g->board[regionID].diceNum;
	return  diceValue;
}

int getMostARCs (Game g){
   // which university currently has the prestige award for the most ARCs?
   // this is NO_ONE until the first arc is purchased after the game 
   // has started.  
   int better = g->mostArcsBy;
	int ARCsOfPlayer1 = getARCs (g, UNI_A);
	int ARCsOfPlayer2 = getARCs (g, UNI_B);
	int ARCsOfPlayer3 = getARCs (g, UNI_C);
   //original code doesn't work if they all have the same number of Arcs
	if(ARCsOfPlayer1 > getARCs(g,better)){
		better = UNI_A;
	}else if(ARCsOfPlayer2 > getARCs(g,better)){
		better = UNI_B;
	}else if(ARCsOfPlayer3 > getARCs(g,better)){
		better = UNI_C;
	}
   g->mostArcsBy = better;
   g->mostArcs = getARCs(g,better);
   return better;
}

int getMostPublications (Game g){
   // which university currently has the prestige award for the most pubs?
   // this is NO_ONE until the first publication is made.
   //If a university tries to start a business spinoff 
   //they can either get a research publication (probability 2/3) 
   //or some valuable IP/Patent (probability 1/3)

   int mostPubPerson = g->mostPaperby;
	int PublicationsOfPlayer1 = getPublications (g, UNI_A);
	int PublicationsOfPlayer2 = getPublications (g, UNI_B);
	int PublicationsOfPlayer3 = getPublications (g, UNI_C);
	if(PublicationsOfPlayer1 > g->mostPaper){
		mostPubPerson = UNI_A;
	}else if(PublicationsOfPlayer2 > g->mostPaper){
		mostPubPerson = UNI_B;
	}else if(PublicationsOfPlayer3 > g->mostPaper){
		mostPubPerson = UNI_C;
	}
   
   //just incase this doesn't go where I want it to go
   g->mostPaperby = mostPubPerson;
   g->mostPaper = getPublications(g,mostPubPerson);
   
   return mostPubPerson;
}


// return the current turn number of the game -1,0,1, ..
int getTurnNumber (Game g) {
	int turnNumber;
	turnNumber = g->currentTurn;
	return turnNumber;
}

// return the player id of the player whose turn it is 
// the result of this function is NO_ONE during Terra Nullis
int getWhoseTurn (Game g) {
    int whoseTurn; 
    whoseTurn = (g->currentTurn)%3 +1;
    //Turn 0 is Player1's turn
    //Turn 1 is Player2's turn
    //Turn 2 is Player3's turn
    //Turn 3 is Player1's turn
    return whoseTurn;
}

// return the contents of the given vertex (ie campus code or 
// VACANT_VERTEX)
int getCampus (Game g, path pathToVertex) {
   vector search = getVector(g,pathToVertex);
   return search.ownership;
}

// the contents of the given edge (ie ARC code or vacent ARC)
//Write function for getArcIndexFromCoord
int getARC (Game g, path pathToEdge) {
   //found nothing calling this in Game.c
   //be careful of getARC() and getARCs()
   vector search = getVector(g, pathToEdge);
   int a = 0;
   while(a <= 72){
      if((g->disArc[a].previousX == search.previousX) &&
         (g->disArc[a].previousY == search.previousY) &&
         (g->disArc[a].x == search.x) &&
         (g->disArc[a].y == search.y)){
            search.ownership = g->disArc[a].ownership;
            a = 9000;
      }else{
            a++;
      }
   }
   return search.ownership;
}

// return the number of KPI points the specified player currently has
int getKPIpoints (Game g, int player) {
	int playerKPI = 0;

	playerKPI += ARC_KPI * getARCs(g, player); //It's a good thing ARCs won't be destroyed 
	playerKPI += CAMPUS_KPI * getCampuses(g, player);	//define CAMPUS_KPI = 10
	playerKPI += GO8_KPI * getGO8s(g, player);	//define G08_KPI = 20
	playerKPI += IP_KPI * getIPs(g, player);	//define IP_KPI = 10

	if (player == getMostARCs(g)) { 
		playerKPI += 10;
	}
	if (player == getMostPublications(g)) {
		playerKPI += 10;
	}

	return  playerKPI;
}

// return the number of ARC grants the specified player currently has
int getARCs (Game g, int player) {
	int playerARCs;
	playerARCs = (g->players[player-1].numARCs); 
	return playerARCs;
}

// return the number of GO8 campuses the specified player currently has
int getGO8s (Game g, int player) {
	int playerGO8s;
	playerGO8s = (g->players[player-1].numGO8s); 
	return playerGO8s;
}

// return the number of normal Campuses the specified player currently has
int getCampuses (Game g, int player) {
	int playerCampuses;
	playerCampuses = (g->players[player-1].numCampuses); // add to player struct
	return playerCampuses;
}

// return the number of IP Patents the specified player currently has
int getIPs (Game g, int player){
	int playerIPs;
	playerIPs = (g->players[player-1].patent); // add to player struct
	return playerIPs;
}
	
// return the number of Publications the specified player currently has
int getPublications (Game g, int player){
	int playerPublications;
	playerPublications = (g->players[player-1].papers); // add to player struct
	return playerPublications;
}

// return the number of students of the specified discipline type 
// the specified player currently has
int getStudents (Game g, int player, int discipline) {
	int playerNumStudents;
	playerNumStudents = (g->players[player-1].students[discipline]);
	return playerNumStudents;
}

// return how many students of discipline type disciplineFrom
// the specified player would need to retrain in order to get one 
// student of discipline type disciplineTo.  This will depend 
// on what retraining centers, if any, they have a campus at.
int getExchangeRate (Game g, int player, int disciplineFrom, int disciplineTo) {
	int rate = 3;
   //We'll write something in getCampus() and if they bought something related to 
   //training centres then we'll update something in the player struct
   if(g->players[player-1].onDiscount[disciplineFrom]==1){
      rate = 2; //onDiscount 0 means not on Discount
   }
   return rate;
}

//syncs the grid with the regionIDs
void updateGameBoard(Game g){
   g->board[0].vertice[5] = g->disboard[0][3];
   g->board[1].vertice[5] = g->disboard[0][5];
   g->board[2].vertice[5] = g->disboard[0][7];
   g->board[0].vertice[0] = g->disboard[1][2];
   g->board[0].vertice[4] = g->disboard[1][4];
      g->board[1].vertice[0] = g->disboard[1][4];
   g->board[1].vertice[4] = g->disboard[1][6];
      g->board[2].vertice[0] = g->disboard[1][6];
   g->board[2].vertice[4] = g->disboard[1][8];
   g->board[0].vertice[1] = g->disboard[2][2];
      g->board[3].vertice[5] = g->disboard[2][2];
   g->board[0].vertice[3] = g->disboard[2][4];
      g->board[1].vertice[1] = g->disboard[2][4];
      g->board[4].vertice[5] = g->disboard[2][4];
   g->board[1].vertice[3] = g->disboard[2][6];
      g->board[2].vertice[1] = g->disboard[2][6];
      g->board[5].vertice[5] = g->disboard[2][6];
   g->board[2].vertice[3] = g->disboard[2][8];
      g->board[6].vertice[5] = g->disboard[2][8];
   g->board[3].vertice[0] = g->disboard[3][1];
   g->board[0].vertice[2] = g->disboard[3][3];
      g->board[3].vertice[4] = g->disboard[3][3];
      g->board[4].vertice[0] = g->disboard[3][3];
   g->board[1].vertice[2] = g->disboard[3][5];
      g->board[4].vertice[4] = g->disboard[3][5];
      g->board[5].vertice[0] = g->disboard[3][5];
   g->board[2].vertice[2] = g->disboard[3][7];
      g->board[5].vertice[4] = g->disboard[3][7];
      g->board[6].vertice[0] = g->disboard[3][7];
   g->board[6].vertice[4] = g->disboard[3][9];
   g->board[3].vertice[1] = g->disboard[4][1];
      g->board[7].vertice[5] = g->disboard[4][1];
   g->board[3].vertice[3] = g->disboard[4][3];
      g->board[4].vertice[1] = g->disboard[4][3];
      g->board[8].vertice[5] = g->disboard[4][3];
   g->board[4].vertice[3] = g->disboard[4][5];
      g->board[5].vertice[1] = g->disboard[4][5];
      g->board[9].vertice[5] = g->disboard[4][5];
   g->board[5].vertice[3] = g->disboard[4][7];
      g->board[6].vertice[1] = g->disboard[4][7];
      g->board[10].vertice[5] = g->disboard[4][7];
   g->board[6].vertice[3] = g->disboard[4][9];
      g->board[11].vertice[5] = g->disboard[4][9];
   g->board[7].vertice[0] = g->disboard[5][0];
   g->board[3].vertice[2] = g->disboard[5][2];
      g->board[7].vertice[4] = g->disboard[5][2];
      g->board[8].vertice[0] = g->disboard[5][2];
   g->board[4].vertice[2] = g->disboard[5][4];
      g->board[8].vertice[4] = g->disboard[5][4];
      g->board[9].vertice[0] = g->disboard[5][4];
   g->board[5].vertice[2] = g->disboard[5][6];
      g->board[9].vertice[4] = g->disboard[5][6];
      g->board[10].vertice[0] = g->disboard[5][6];
   g->board[6].vertice[2] = g->disboard[5][8];
      g->board[10].vertice[4] = g->disboard[5][8];
      g->board[11].vertice[0] = g->disboard[5][8];
   g->board[11].vertice[4] = g->disboard[5][10];
   g->board[7].vertice[1] = g->disboard[6][0];
   g->board[7].vertice[3] = g->disboard[6][2];
      g->board[8].vertice[1] = g->disboard[6][2];
      g->board[12].vertice[5] = g->disboard[6][2];
   g->board[8].vertice[3] = g->disboard[6][4];
      g->board[9].vertice[1] = g->disboard[6][4];
      g->board[13].vertice[5] = g->disboard[6][4];
   g->board[9].vertice[3] = g->disboard[6][6];
      g->board[10].vertice[1] = g->disboard[6][6];
      g->board[14].vertice[5] = g->disboard[6][6];
   g->board[10].vertice[3] = g->disboard[6][8];
      g->board[11].vertice[1] = g->disboard[6][8];
      g->board[15].vertice[5] = g->disboard[6][8];
   g->board[11].vertice[3] = g->disboard[6][10];
   g->board[7].vertice[2] = g->disboard[7][1];
      g->board[12].vertice[0] = g->disboard[7][1];
   g->board[8].vertice[2] = g->disboard[7][3];
      g->board[12].vertice[4] = g->disboard[7][3];
      g->board[13].vertice[0] = g->disboard[7][3];
   g->board[9].vertice[2] = g->disboard[7][5];
      g->board[13].vertice[4] = g->disboard[7][5];
      g->board[14].vertice[0] = g->disboard[7][5];
   g->board[10].vertice[2] = g->disboard[7][7];
      g->board[14].vertice[4] = g->disboard[7][7];
      g->board[15].vertice[0] = g->disboard[7][7];
   g->board[11].vertice[2] = g->disboard[7][9];
      g->board[15].vertice[4] = g->disboard[7][9];
   g->board[12].vertice[1] = g->disboard[8][1];
   g->board[12].vertice[3] = g->disboard[8][3];
      g->board[13].vertice[1] = g->disboard[8][3];
      g->board[16].vertice[5] = g->disboard[8][3];
   g->board[13].vertice[3] = g->disboard[8][5];
      g->board[14].vertice[1] = g->disboard[8][5];
      g->board[17].vertice[5] = g->disboard[8][5];
   g->board[14].vertice[3] = g->disboard[8][7];
      g->board[15].vertice[1] = g->disboard[8][7];
      g->board[18].vertice[5] = g->disboard[8][7];
   g->board[15].vertice[3] = g->disboard[8][9];

   g->board[12].vertice[2] = g->disboard[9][2];
      g->board[16].vertice[0] = g->disboard[9][2];
   g->board[13].vertice[2] = g->disboard[9][4];
      g->board[16].vertice[4] = g->disboard[9][4];
      g->board[17].vertice[0] = g->disboard[9][4];
   g->board[14].vertice[2] = g->disboard[9][6];
      g->board[17].vertice[4] = g->disboard[9][6];
      g->board[18].vertice[0] = g->disboard[9][6];
   g->board[15].vertice[2] = g->disboard[9][8];
      g->board[18].vertice[4] = g->disboard[9][8];
   g->board[16].vertice[1] = g->disboard[10][2];
   g->board[16].vertice[3] = g->disboard[10][4];
      g->board[17].vertice[1] = g->disboard[10][4];
   g->board[17].vertice[3] = g->disboard[10][6];
      g->board[18].vertice[1] = g->disboard[10][6];
   g->board[18].vertice[3] = g->disboard[10][8];
   g->board[16].vertice[2] = g->disboard[11][3];
   g->board[17].vertice[2] = g->disboard[11][5];
   g->board[18].vertice[2] = g->disboard[11][7];
}

vector getVector(Game g, path p){
   Vector direction = (Vector) malloc(6*sizeof(vector));
   direction[E_EAST].x = 1;
   direction[E_EAST].y = 0;
   direction[S_EAST].x = 1;
   direction[S_EAST].y = 1;
   direction[S_WEST].x = -1;
   direction[S_WEST].y = 1;
   direction[W_WEST].x = -1;
   direction[W_WEST].y = 0;
   direction[N_WEST].x = -1;
   direction[N_WEST].y = -1;
   direction[N_EAST].x = 1;
   direction[N_EAST].y = -1;
   //double check all east has x +1; west has x-1;
   //north has y-1; south has y+1;
   
      //{ earthinates data, 0 means doesn't exist, 1 means onboard
   //Imagine Earth, filled in lava, and we look at it from an objective view
   //If the grid point only has height 0, it is lava and we can't do anything to it
   //If the grid point is height 1AU above lava, that should mean there should be land...
   int earth[12][11] = {{0}};
   earth[0][3] = 1;
   earth[0][5] = 1;
   earth[0][7] = 1;
   earth[1][2] = 1;
   earth[1][4] = 1;
   earth[1][6] = 1;
   earth[1][8] = 1;
   earth[2][2] = 1;
   earth[2][4] = 1;
   earth[2][6] = 1;
   earth[2][8] = 1;
   earth[3][1] = 1;
   earth[3][3] = 1;
   earth[3][5] = 1;
   earth[3][7] = 1;
   earth[3][9] = 1;
   earth[4][1] = 1;
   earth[4][3] = 1;
   earth[4][5] = 1;
   earth[4][7] = 1;
   earth[4][9] = 1;
   earth[5][0] = 1;
   earth[5][2] = 1;
   earth[5][4] = 1;
   earth[5][6] = 1;
   earth[5][8] = 1;
   earth[5][10] = 1;
   earth[6][0] = 1;
   earth[6][2] = 1;
   earth[6][4] = 1;
   earth[6][6] = 1;
   earth[6][8] = 1;
   earth[6][10] = 1;
   earth[7][1] = 1;
   earth[7][3] = 1;
   earth[7][5] = 1;
   earth[7][7] = 1;
   earth[7][9] = 1;
   earth[8][1] = 1;
   earth[8][3] = 1;
   earth[8][5] = 1;
   earth[8][7] = 1;
   earth[8][9] = 1;
   earth[9][2] = 1;
   earth[9][4] = 1;
   earth[9][6] = 1;
   earth[9][8] = 1;
   earth[10][2] = 1;
   earth[10][4] = 1;
   earth[10][6] = 1;
   earth[10][8] = 1;
   earth[11][3] = 1;
   earth[11][5] = 1;
   earth[11][7] = 1;
   //}

   //given a path
   vector start;
   start.x = 5; //Castle A is at (5,0)
   start.y = 0; 
   int orientation = S_EAST;
   int ccc = 0;
   while(p[ccc] != '\0'){ //may need fixing this end value
      //assuming the elements of p can only be 'L' 'R' 'B' or EOF
      printf("%c ",p[ccc]);
      if(p[ccc]=='L'){
         orientation--;
      }else if(p[ccc]== 'R'){
         orientation++;
      }else if(p[ccc]== 'B'){
         orientation = (orientation+3)%6;
      }else{
         assert(1==2);
      }
      if(ccc >= 1){
         start.previousX = start.x;
         start.previousY = start.y; //having a previous will help calculating ARC
      }
      start.x += direction[orientation].x;
      start.y += direction[orientation].y;
      //we've moved in this direction, now we check if it's in the plane
      printf("%d,%d\n",start.x,start.y);
      assert(earth[start.x][start.y]==1);
      //If it does, we continue to move on along the vectors
      ccc++;
   }
   return start;
}

int isLegalAction(Game g, action a){
   int legal = 1;
   //return 0 if legal;
   if(a == PASS){
      legal = 0;
   }
   return legal;
}

