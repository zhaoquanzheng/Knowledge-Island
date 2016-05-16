//test space
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Game.h"

//path code

#define E_EAST 0
#define S_EAST 1
#define S_WEST 2
#define W_WEST 3
#define N_WEST 4
#define N_EAST 5


//Vector Orientation
typedef struct _vector{
   int x;
   int y;
} vector;
typedef vector * Vector;

vector function(path p);

int main(int argc, char * argv[]){
   path p = "RLRRRLL";
   vector result = function(p);
   assert(result.x==0 && result.y==3);
   printf("Test 1 passed, now at Campus B\n");
   
   path q = "LRRRRR";
   result = function(q);
   assert(result.x == 5 && result.y == 0);
   printf("Test 2 passed, now at Campus A\n");
   
   path r = "LRRRRRBB";
   result = function(r);
   assert(result.x==5 && result.y==0);
   printf("Test 3 passed, now at Campus A\n");
   
   printf("All tests passed!\n");
   return EXIT_SUCCESS;
}

vector function(path p){
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
   
      //{ coordinates data, 0 means doesn't exist, 1 means onboard
   //Imagine Earth, filled in lava, and we look at it from an objective view
   //If the grid point only has height 0, it is lava and we can't do anything to it
   //If the grid point is height 1AU above lava, that should mean there should be land...
   int coord[12][11] = {{0}};
   coord[0][3] = 1;
   coord[0][5] = 1;
   coord[0][7] = 1;
   coord[1][2] = 1;
   coord[1][4] = 1;
   coord[1][6] = 1;
   coord[1][8] = 1;
   coord[2][2] = 1;
   coord[2][4] = 1;
   coord[2][6] = 1;
   coord[2][8] = 1;
   coord[3][1] = 1;
   coord[3][3] = 1;
   coord[3][5] = 1;
   coord[3][7] = 1;
   coord[3][9] = 1;
   coord[4][1] = 1;
   coord[4][3] = 1;
   coord[4][5] = 1;
   coord[4][7] = 1;
   coord[4][9] = 1;
   coord[5][0] = 1;
   coord[5][2] = 1;
   coord[5][4] = 1;
   coord[5][6] = 1;
   coord[5][8] = 1;
   coord[5][10] = 1;
   coord[6][0] = 1;
   coord[6][2] = 1;
   coord[6][4] = 1;
   coord[6][6] = 1;
   coord[6][8] = 1;
   coord[6][10] = 1;
   coord[7][1] = 1;
   coord[7][3] = 1;
   coord[7][5] = 1;
   coord[7][7] = 1;
   coord[7][9] = 1;
   coord[8][1] = 1;
   coord[8][3] = 1;
   coord[8][5] = 1;
   coord[8][7] = 1;
   coord[8][9] = 1;
   coord[9][2] = 1;
   coord[9][4] = 1;
   coord[9][6] = 1;
   coord[9][8] = 1;
   coord[10][2] = 1;
   coord[10][4] = 1;
   coord[10][6] = 1;
   coord[10][8] = 1;
   coord[11][3] = 1;
   coord[11][5] = 1;
   coord[11][7] = 1;
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
         //printf("Invalid Input\n");
         assert(EXIT_FAILURE != 1);
      }
      start.x += direction[orientation].x;
      start.y += direction[orientation].y;
      //we've moved in this direction, now we check if it's in the plane
      printf("%d,%d\n",start.x,start.y);
      assert(coord[start.x][start.y]==1);
      //If it does, we continue to move on along the vectors
      ccc++;
   }
   return start;
}