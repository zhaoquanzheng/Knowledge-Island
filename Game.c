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
