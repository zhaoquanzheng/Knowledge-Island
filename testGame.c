//testgame v1.1
/*To test
Game newGame (int discipline[], int dice[]);
void disposeGame (Game g);
void makeAction (Game g, action a); //where action will be an already defined struct
void throwDice (Game g, int diceScore);
int getDiscipline (Game g, int regionID); //This should return something in the interval [0,5]
int getDiceValue (Game g, int regionID);  //This should return something in the interval [1,12]
int getMostARCs (Game g);                 //This should find the greatest amount of arcs in Game g 
int getMostPublications (Game g);         //Same as above
int getTurnNumber (Game g);               //A number that is constantly kept in Game g
int getWhoseTurn (Game g);                //Divide it through? Turn -1 is before Start, Turn 0 is Player 1's turn
int getCampus(Game g, path pathToVertex); //This should return 0 for empty vertex, or 1/2/3 for the respective player
int getARC(Game g, path pathToEdge);      //Same as above
int isLegalAction (Game g, action a);     //Return 0 if legal
int getKPIpoints (Game g, int player);    //Search Game g, for player's KPIpoints
int getARCs (Game g, int player);         //Similar to above - but this means that Game struct stores where the campuses are
int getGO8s (Game g, int player);         //Similar to above - since player is an int
int getCampuses (Game g, int player);     //Similar to above
int getIPs (Game g, int player);
int getPublications (Game g, int player);
int getStudents (Game g, int player, int discipline);
int getExchangeRate (Game g, int player, 
                     int disciplineFrom, int disciplineTo);
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include "Game.h"

void testGame();

/*
//{ I'll have this in Game.c when it's finished
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
} * Tile;

   typedef struct _game{ //We mainly need to finish the draft of this by Sunday 08 May 2016
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
} game;


//}
*/


int main(int argc, char *argv[]){
   testGame();
   return EXIT_SUCCESS;
}

/*Game newGame(int discipline[], int dice[]){
   Game g = malloc(1000);
   return g;
}*/

void testGame(){
   printf("Beginning Tests\n");
   int disciplineInRegions[NUM_REGIONS] = {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ, \
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV, \
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN, \
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ, \
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS};
   int dice[NUM_REGIONS] =  {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5};
   Game g = newGame(disciplineInRegions, dice);
   assert(g->currentTurn==-1);
   assert(g->board[0]->diceNum==9);
   printf("newGame() pass\n");
   
   printf("Testing disposeGame()\n");
   disposeGame(g);
   assert(g == NULL);//not sure if disposeGame() is testable
   printf("Finished testing disposeGame()\n");
   
/* not sure how to test this
   printf("Testing makeAction()\n");
   makeAction (Game g, action a); //where action will be an already defined struct
*/
   printf("Testing throwDice\n");
   throwDice(g,4);
   assert(g->diceThrow==4);
   printf("throwDice passed\n");

   printf("Testing getDiscipline\n");
   assert(getDiscipline(g,0)==STUDENT_BQN); //Game g, int regionID; refer to above arrays
   assert(getDiscipline(g,18)==STUDENT_BPS);
   printf("getDiscipline() passed\n");
   
   printf("Testing getDiceValue()\n");
   assert(getDiceValue(g,0)==9);   //Game g, int regionID; refer to above arrays
   assert(getDiceValue(g,18)==5);
   printf("getDiceValue() passed\n");
   
   printf("Testing getMostArcs\n");
   //If the rules are that we start without having built stuff, ie the online app is different
   assert(getMostARCs(g)==NO_ONE);
   g->mostArcs = UNI_B;
   assert(getMostARCs(g)==UNI_B);
   printf("...the function getMostArcs works\n");
   
   printf("Testing getMostPublications\n");
   assert(getMostPublications(g)==NO_ONE);
   //is getMostPublications referring to Papers, or IP Patents?
   printf("getMostPublications may have passed... one test only\n");
   
   printf("Testing getTurnNumber\n");
   assert(getTurnNumber(g)==-1); //start of Game, Terra_Nullis
   printf("...the function getTurnNumber works\n");
   
   printf("Testing getWhoseTurn\n");
   g->currentTurn = 0;
   assert(getWhoseTurn(g)==UNI_A); //UNI_A 's turns are on 0,3,6,9...
   g->currentTurn = 1;
   assert(getWhoseTurn(g)==UNI_B);
   g->currentTurn = 102;
   assert(getWhoseTurn(g)==UNI_A);
   printf("...the function getWhoseTurn works\n");
   
   printf("Testing getCampus\n");
   //paths... here comes the hard part
   path test1 = "LRR";
   assert(getCampus(g,test1)==VACANT_VERTEX);
   printf("...the function getCampus works\n");
   
   printf("Testing getARC\n");
   //test 1 can still be used to test for an edge
   assert(getARC(g,test1)==VACANT_ARC);
   printf("...the function getARC works\n");
   
   printf("---Testing isLegalAction()\n");
   action a;
   printf(".testing about building campus on non existent vertex..");
   a.actionCode = 1;
   a.destination[0] = 'L';
   a.destination[1] = 'L';
   assert(isLegalAction(g,a)==FALSE);
   a.destination[0] = 'R'; //testing if I can change the size of the path array
   a.destination[1] = 'R';
   a.destination[2] = 'R';
   assert(isLegalAction(g,a)==FALSE);
   printf(".that passed\n");
   printf(".testing the pass code; always Legal, regardless of path\n");
   a.actionCode = 0;
   assert(isLegalAction(g,a)==TRUE);
   printf("~And isLegalAction passes!\n");
   
   printf("Going to try testing getKPIpoints()\n");
   int player = UNI_A;
   assert(getKPIpoints(g,player)>=0);//not sure whether we start with 0KPI or 2Campus,2ARC(24KPI)
   printf("one test passed for getKPI()\n");
   
   //I'm going to assume we have the things stated in Game rules from now on
   //that is, everyone starts with zero structures, but 
   printf("Testing getArcs\n");
   assert(getARCs ( g, UNI_A)==0);
   assert(getARCs ( g, UNI_B)==0);
   assert(getARCs ( g, UNI_C)==0);
   printf("getArcs passed!\n");
   
   printf("Testing getGO8s\n");
   assert(getGO8s ( g, UNI_A)==0);
   assert(getGO8s ( g, UNI_B)==0);
   assert(getGO8s ( g, UNI_C)==0);
   printf("getGO8s passed!\n");
   
   printf("Testing getCampuses\n");
   assert(getCampuses ( g, UNI_A)==2);
   assert(getCampuses ( g, UNI_B)==2);
   assert(getCampuses ( g, UNI_C)==2);
   printf("getCampuses passed!\n");
   
   printf("Testing getIPs\n");
   assert(getIPs ( g, UNI_A)==0);
   assert(getIPs ( g, UNI_B)==0);
   assert(getIPs ( g, UNI_C)==0);
   printf("getIPs passed!\n");

   printf("Testing getPublications\n");
   assert(getPublications ( g, UNI_A)==0);
   assert(getPublications ( g, UNI_B)==0);
   assert(getPublications ( g, UNI_C)==0);
   printf("getPublications passed!\n");

   printf("Testing getStudents\n");
   //just testing UNI_A
   assert(getStudents(g,UNI_A,STUDENT_THD)==0);
   assert(getStudents(g,UNI_A,STUDENT_BPS)==3);
   assert(getStudents(g,UNI_A,STUDENT_BQN)==3);
   assert(getStudents(g,UNI_A,STUDENT_MJ)==1);
   assert(getStudents(g,UNI_A,STUDENT_MTV)==1);
   assert(getStudents(g,UNI_A,STUDENT_MMONEY)==1);
   printf("getStudents passed!\n");


   //We haven't written the training centre codes yet so:
   printf("Testing getExchangeRate\n");
   assert(getExchangeRate (g, UNI_A, STUDENT_BPS, STUDENT_MMONEY)==3);
   printf("getExchangeRate passed!\n");
   
   printf("All tests passed!\n");
}
