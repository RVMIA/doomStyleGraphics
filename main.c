#include <raylib.h>
#include <stdio.h>
#include <math.h>
#define FPS 60
#define SCREENW 900
#define SCREENH 900
#define MAPSIZE 20
#define TILESIZE SCREENH/MAPSIZE
#define PIHALVES PI/2

Color GRUVGREY = {29, 32, 33, 255};

float px = 159, py = 345, pa = .75;


int map[MAPSIZE][MAPSIZE]= {
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};


void drawMap(int map[MAPSIZE][MAPSIZE]){
  for (int i = 0; i < MAPSIZE; i++){
    for (int j = 0; j < MAPSIZE; j++){
      Color color = GRUVGREY;
      if (map[i][j] == 1)
	color = GRAY;
      if (map[i][j] == 2){
	//map[i][j] = 0;
	color = SKYBLUE;
      }
      DrawRectangle((j * TILESIZE)+1, (i * TILESIZE)+1, TILESIZE-2, TILESIZE-2, color);
    }
  }
}
void drawPlayer(){

  /* Controls */

  if (IsKeyDown(KEY_W)){
    py += sin(pa);
    px += cos(pa);
  }
  if (IsKeyDown(KEY_S)){
    py -= sin(pa);
    px -= cos(pa);
  }
    if (IsKeyDown(KEY_D)){
      pa += 0.01;
      if (pa >= 2*PI){
	pa = 0;
      }
    }
    if (IsKeyDown(KEY_A)){
      pa -= 0.01;
      if (pa < 0){
	pa = 2*PI;
      }
    }

    Vector3 playerPos = {px, py, pa};
    Vector2 playerPos2 = {playerPos.x, playerPos.y};
    Vector2 tile = {(int)px % 45, (int)py % 45};
    
    float tanAng = tan(pa);
    float invTanAng = (float)1/tanAng;

#define ITERATIONS 5

    Vector2 xPositive = { TILESIZE, TILESIZE * tanAng};
    Vector2 yPositive = { TILESIZE * invTanAng, TILESIZE};
    
    Vector2 xPositiveShift = { TILESIZE - tile.x, TILESIZE * tanAng - tile.x * tanAng };
    Vector2 xNegativeShift = { 0 - tile.x, 0 - tile.x * tanAng };
    Vector2 yPositiveShift = { TILESIZE * invTanAng - tile.y * invTanAng, TILESIZE - tile.y };
    Vector2 yNegativeShift = { - tile.y * invTanAng, - tile.y };

    bool wallX = false;
    bool wallY = false;
    for (int i = 0; i < 10; i++){
      if (!wallX){
	if (pa < PIHALVES || pa > 3*PIHALVES){	
	  Vector2 endX = {playerPos.x + xPositiveShift.x + i*xPositive.x, playerPos.y + xPositiveShift.y + i*xPositive.y};
	  //DrawLineEx(playerPos2, endX, 6, GREEN);
	  DrawRectangle(endX.x-5, endX.y-5, 10, 10, BLUE);
	  Vector2 wallTileX = {(int)(endX.x+1)/(int)(TILESIZE), (int)(endX.y+1)/(int)(TILESIZE)};
	  if (map[(int)wallTileX.y][(int)wallTileX.x] == 1){
	    wallX = true;
	  }
	} else {
	  Vector2 endX = {playerPos.x + xNegativeShift.x - i*xPositive.x, playerPos.y + xNegativeShift.y - i*xPositive.y};
	  //DrawLineEx(playerPos2, endX, 6, GREEN);
	  DrawRectangle(endX.x-5, endX.y-5, 10, 10, BLUE);
	  Vector2 wallTileX = {(int)(endX.x)/(int)(TILESIZE), (int)(endX.y)/(int)(TILESIZE)};
	  if (map[(int)wallTileX.y][(int)wallTileX.x] == 1){
	    wallX = true;
	  }
	}
      } 
      if (!wallY){
	if (pa < PI){
	  Vector2 endY = {playerPos.x + yPositiveShift.x + i*yPositive.x, playerPos.y + yPositiveShift.y + i*yPositive.y};
	  //DrawLineEx(playerPos2, endY, 4, ORANGE);
	  DrawRectangle(endY.x-5, endY.y-5, 10, 10, RED);
	  Vector2 wallTileY = {(int)(endY.x)/(int)(TILESIZE), (int)(endY.y)/(int)(TILESIZE)};
	  if (map[(int)wallTileY.y][(int)wallTileY.x] == 1){
	    wallY = true;
	  }
	} else if (pa != 0) {
	  Vector2 endY = {playerPos.x + yNegativeShift.x - i*yPositive.x, playerPos.y + yNegativeShift.y - i*yPositive.y};
	  // DrawLineEx(playerPos2, endY, 4, ORANGE);
	  DrawRectangle(endY.x-5, endY.y-5, 10, 10, RED);
	  Vector2 wallTileY = {(int)(endY.x)/(int)(TILESIZE), (int)(endY.y)/(int)(TILESIZE)};
	  if (map[(int)wallTileY.y][(int)wallTileY.x] == 1){
	    wallY = true;
	    
	  }
	}
      } 
    }
    DrawText(TextFormat("x: %.2f\ty: %.2f\ta: %.2f", px, py, pa), 600, 800, 20, RAYWHITE);

    DrawLineEx((Vector2){px, py}, (Vector2){px + TILESIZE *  cos(pa), py + TILESIZE * sin(pa)}, 1, RED); /*Player Direction Ray*/
    DrawRectangle(px-5, py-5, 10, 10, YELLOW); /* Player Sprite Location */
    
}

double length(Vector2 start, Vector2 end){
  int xDiff = (end.x - start.x);
  int yDiff = (end.y - start.y);

  int xDiff2 = xDiff * xDiff;
  int yDiff2 = yDiff * yDiff;
  double distance = sqrt((double)xDiff2 + (double)yDiff2);
  return distance;
  }

void drawRay(){
  float rayX, rayY;
  DrawLine(px, py, rayX, rayY, RED);
}

int main(){

  InitWindow(SCREENW, SCREENH, "Raycaster Test");

  SetTargetFPS(FPS);
  while (!WindowShouldClose()){
    BeginDrawing();
    ClearBackground(BLACK);
    drawMap(map);
    drawPlayer();
    EndDrawing();
  }
  CloseWindow();
  return 0;
}

