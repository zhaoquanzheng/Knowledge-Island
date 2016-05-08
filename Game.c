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

typedef struct _game{ //We mainly need to finish the draft of this by Sunday 08 May 2016
  int currentTurn;
  int diceThrow;
  Player players[3]; //where Player is a pointer to a struct _player
  Tile board[19];
  int board[12][11];
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
} game;

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
} board


int main(int argc, char * argv[]){
//  testFunction();
  return EXTI_SUCCESS;
}

//void testFunction(){ moving to separate testGame.c
  //need to test these functions (or not -e.g. void functions)
  //and write them
  //and then comment in some of the test, and then compile and check, and then... and then smile >_<
  /*  Game newGame (int discipline[], int dice[]);
      void disposeGame (Game g);
      void makeAction (Game g, action a); //this is a big one
      void throwDice (Game g, int diceScore);
      int getDiscipline (Game g, int regionID);
      int getDiceValue (Game g, int regionID);
      int getMostARCs (Game g);
      int getMostPublications (Game g);
      int getTurnNumber (Game g);
      int getWhoseTurn (Game g);
      int getCampus(Game g, path pathToVertex);
      int getARC(Game g, path pathToEdge);
      int isLegalAction (Game g, action a); //this is a big one
      int getKPIpoints (Game g, int player);
      int getARCs (Game g, int player);
      int getGO8s (Game g, int player);
      int getCampuses (Game g, int player);
      int getIPs (Game g, int player);
      int getPublications (Game g, int player);
      int getStudents (Game g, int player, int discipline);
      int getExchangeRate (Game g, int player, 
                     int disciplineFrom, int disciplineTo);
   */
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
