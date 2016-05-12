//game.c

//TODO put this somewhere in the code
//   int disciplines[] = DEFAULT_DISCIPLINES;
//   int dice[] = DEFAULT_DICE;
//   Game g = newGame (disciplines, dice);

#include "Game.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
/* #include <string.h>
   #include <math.h>
*/
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

typedef struct _player{
  char playername[30]; //maximum of 30 characters
  int KPI;
  int students[6]; //the value at each index represents the number of the students
  int patent;
  int papers;
  //somehow talk about campuses
  //somehow talk about arcs
  //somehow talk about GO8s
} * Player;

typedef struct _board{
   int diceNum;
   int discipline;
   int vertice[6];
   int edge[6];
   //training centres?
} * Tile;

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
} game;



int main(int argc, char * argv[]){
//  testFunction();
  return EXIT_SUCCESS;
}

   //and then the list of functions that we may want in our code
   /* 1)changing path information into coordinates of a vertice of the hexagon
      2)changing path information into a specific arc (2 coordinates?)
      3)roll a dice that has values in the interval [2,12]
      4)scanf functions that read in the path, the action, the name of the player etc
      5)calculate the distance away from one point (or between two points) - used in isLegal() to determine if the campuses are
         too close together or not
      6)
   */
}

Game newGame(int discipline[], int dice[]){
   Game newG = (Game) malloc(sizeof(struct _game));
   newG->currentTurn = -1;
   //printf("The name of the first Univerisity is :\n");
   newG->players[UNI_A]->playername = getPlayerOrAIName();
   //printf("The name of the second Univerisity is :\n");
   newG->players[UNI_B]->playername = getPlayerOrAIName();
   //printf("The name of the third Univerisity is :\n");
   newG->players[UNI_C]->playername = getPlayerOrAIName();
   
   int a = 0;
   while(a < 19){
      board[a]->discipline = discipline[a];
      board[a]->diceNum    = dice[a];
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
    */
   int currPlyr = getWhoseTurn(g); //where currPlyr shall be 1,2, or 3
   char* pathway = a.destination;
   if(a.actionCode == BUILD_CAMPUS){
      //error we still haven't decided on how to code the vertexes
      //int campus = getCampus(g,pathway);
      //so for the following, I don't know whether to use int, int*, or int[][]
      //g->something->(campus?)->?
      //pointer to that campus and change it to 'currPlyr' which therfore changes
      //ownership to that UNI
   }else if(a.actionCode == OBTAIN_ARC){
      //int Arc = getARC(g,pathway);
      //g->arc = currPlyr;
      //^that doesn't really make sense at the moment, I know
   }else if(a.actionCode = OBTAIN_PUBLICATION){ //because the function that is 
                                                //about to call makeAction()
                                                //changes SPINOFF to either Pp, or IP
      g->players[currPlyr]->papers++;
   }else if(a.actionCode = OBTAIN_IP_PATENT){
      g->players[currPlyr]->patent++;
   }else{
      int price = getExchangeRate (g, currPlyr, a.disciplineFrom, a.disciplineTo);
      g->players[currPlyr]->students[a.disciplineFrom] -= price;
      g->players[currPlyr]->students[a.disciplineTo] ++;
      //Please admire how much stuff, we can assume here
      //and this lets you realise how much stuff goes into isLegalAction()
   }
   
   //} for the commented while loop at the top   
}


void throwDice( Game g, int score){
//NOTE:THIS FUNCTION RELIES HEAVILY ON HOW WE STORE THE INFORMATION OF CAMPUSES ON THE MAP
//WHETHER IT BE BY A COMBINATION OF THREE HEXAGONS, OR MAINLY AS A COORDINATE
   //I'll need to know which campuses and which players are on which vertex of the regions
   int i = 0;
   int students[3][6] = {0};
   while(i < 19){
      if(g->board[i]->diceNum == score){
         int type = getDiscipline(g,i); //Should I be relying/trusting on the other functions?
         int a = 0;
         while(a<6){
            int corner = g->board[i]->vertice[a] //CHANGE HERE
            if(corner>=UNI_A && corner<=UNI_C){    //should NO_CAMPUS, or a UNI
               students[corner][type]++;
            }
            a++;
         }
      }
      i++;
   }
   int x = 0;
   int y = 0;
   while(x < 3){
      y = 0;
      while(y<6){
         g->players[x]->students[y]+=students[x][y];
         y++;
      }
      x++;
   }
}


// what type of students are produced by the specified region?
// regionID is the index of the region in the newGame arrays (above) 
// see discipline codes above
int getDiscipline (Game g, int regionID){
	int discipline;
	discipline = g->board[regionID].discipline;
	return discipline;
} 

// what dice value produces students in the specified region?
// 2..12
int getDiceValue (Game g, int regionID){
	int diceValue;
	diceValue  = g->board[regionID].diceNum;
	return  diceValue;
}

// which university currently has the prestige award for the most ARCs?
// this is NO_ONE until the first arc is purchased after the game 
// has started.  
int getMostARCs (Game g){
	int ARCsOfPlayer1 = getARCs (g, UNI_A);
	int ARCsOfPlayer2 = getARCs (g, UNI_B);
	int ARCsOfPlayer3 = getARCs (g, UNI_C);
	if((ARCsOfPlayer1 >= ARCsOfPlayer2) && (ARCsOfPlayer1 >= ARCsOfPlayer3)){
		return UNI_A;
	}
	
	else if((ARCsOfPlayer2 >= ARCsOfPlayer1) && (ARCsOfPlayer2 >= ARCsOfPlayer3)){
		return UNI_B;
	}
	
	else if(ARCsOfPlayer3 >= ARCsOfPlayer1) && (ARCsOfPlayer3 >= ARCsOfPlayer2)){
		return UNI_C;
	}
}


// which university currently has the prestige award for the most pubs?
// this is NO_ONE until the first publication is made.
//If a university tries to start a business spinoff they can either get a research publication (probability 2/3) or some valuable IP/Patent (probability 1/3)

int getMostPublications (Game g){
	int PublicationsOfPlayer1 = getPublications (g, UNI_A);
	int PublicationsOfPlayer2 = getPublications (g, UNI_B);
	int PublicationsOfPlayer3 = getPublications (g, UNI_C);
	if((PublicationsOfPlayer1 >= PublicationsOfPlayer2) && (PublicationsOfPlayer1 >= PublicationsOfPlayer3))
		return UNI_A;
	else if((PublicationsOfPlayer2 >= PublicationsOfPlayer1) && (PublicationsOfPlayer2 >= PublicationsOfPlayer3))
		return UNI_B;
	else if(PublicationsOfPlayer3 >= PublicationsOfPlayer1) && (PublicationsOfPlayer3 >= PublicationsOfPlayer2))
		return UNI_C;
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
    whoseTurn = (g->currentTurn)%3;
    //return 1,2,3 (representing each player's ID)
    return whoseTurn;
}

// return the contents of the given vertex (ie campus code or 
// VACANT_VERTEX)
//pathToVertex is passed into function to get VertexIndex
//Write function for getVertexIndexFromCoord
int getCampus (Game g, path pathToVertex) {
    int vertexIndex;
    if (getVertexIndexFromCoord (g, pathToVertex) != INVALID) { //#define INVALID
        vertexIndex = g->boardVertices[getVertexIndexFromCoord (g, v)].contents;
    } else {
    	vertexIndex = VACANT_VERTEX;
    }
    return vertexIndex;

}

// the contents of the given edge (ie ARC code or vacent ARC)
//Write function for getArcIndexFromCoord
int getARC (Game g, path pathToEdge) {
    int arcIndex;
    if (getArcIndexFromCoord (g, pathToArc) != INVALID) { //#define INVALID
        arcIndex = g->boardVertices[getarcIndexFromCoord (a, v)].contents;
    } else {
    	arcIndex = VACANT_ARC;
    }
    return arcIndex;
}

// return the number of KPI points the specified player currently has
int getKPIpoints (Game g, int player) {
	int playerKPI = 0;

	//define ARC_KPI = 2
	playerKPI += ARC_KPI * getARCs(g, player); 
	//define CAMPUS_KPI = 10
	playerKPI += CAMPUS_KPI * getCampuses(g, player);
	//define G08_KPI = 20
	playerKPI += GO8_KPI * getG08s(g, player);
	//define IP_KPI = 10
	playerKPI += IP_KPI * getIPs(g, player);

	if (player == getMostArcs(g)) { 
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
	playerARCs = (g->players[player-1].numARCgrants); // add to player struct
	return playerARCs;
}

// return the number of GO8 campuses the specified player currently has
int getG08s (Game g, int player) {
	int playerG08s;
	playerG08s = (g->players[player-1].numG08s); // add to player struct
	return playerG08s;
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
	playerips = (g->players[player-1].patent); // add to player struct
	return playerIPs;
	
// return the number of Publications the specified player currently has
int getPublications (Game g, int player){
	int playerPublications;
	playerPublications = (g->players[player-1].paper); // add to player struct
	return playerPublications;
}

// return the number of students of the specified discipline type 
// the specified player currently has
int getStudents (Game g, int player, int discipline) {
	int playerNumStudents;
	playerNumStudents = (g->players[player-1].students[discipline]);
	return playerNumStudents
}

// return how many students of discipline type disciplineFrom
// the specified player would need to retrain in order to get one 
// student of discipline type disciplineTo.  This will depend 
// on what retraining centers, if any, they have a campus at.
int getExchangeRate (Game g, int player, int disciplineFrom, int disciplineTo) {
	int rate = 3;
   //im not sure how to do this one
}


