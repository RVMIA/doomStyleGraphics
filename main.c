#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define FPS 60
#define SCREENW 1280
#define SCREENH 1020
#define MAPSIZE 20
#define TILESIZE SCREENH/MAPSIZE

/* https://lodev.org/cgtutor/raycasting.html */

Color GRUVGREY = {29, 32, 33, 255};

/* Initial Position */


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

int main(){
  float pa = PI;
  float px = 5, py = 10;
  double dirX = -1, dirY = 0;
  double planeX = 0, planeY = 0.66;

  InitWindow(SCREENW, SCREENH, "Raycaster");
  SetTargetFPS(FPS);
  while (!WindowShouldClose()){
    BeginDrawing();
    ClearBackground(BLACK);

    
  
    for(int x = 0; x < SCREENW; x++){
      double cameraX = 2 * x / (double)SCREENW - 1;
      double rayDirX = dirX + planeX * cameraX;
      double rayDirY = dirY + planeY * cameraX;
      
      int mapX = (int)px;
      int mapY = (int)py;
      
      double sideDistX, sideDistY;
      
      double deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1/rayDirX);
      double deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1/rayDirY);
      double perpWallDist;
      
      int stepX, stepY;
      int hit = 0;
      int side;
      
      if (rayDirX < 0){
	stepX = -1;
	sideDistX = (px - mapX) * deltaDistX;
      } else {
	stepX = 1;
	sideDistX = (mapX + 1 - px) * deltaDistX;
      }
      if (rayDirY < 0){
	stepY = -1;
	sideDistY = (py - mapY) * deltaDistY;
      } else {
	stepY = 1;
	sideDistY = (mapY + 1 - py) * deltaDistY;
      }
      
      
      while (hit == 0){
	if (sideDistX < sideDistY){
	  sideDistX += deltaDistX;
	  mapX += stepX;
	  side = 0;
	} else {
	  sideDistY += deltaDistY;
	  mapY += stepY;
	  side = 1;
	}
	if (map[mapY][mapX] > 0){
	  hit = 1;
	}
      }
      if (side == 0){
	perpWallDist = (sideDistX - deltaDistX);
      } else {
	perpWallDist = (sideDistY - deltaDistY);
      }
      int lineHeight = (int)(SCREENH / perpWallDist);
      int drawStart = -lineHeight/2 + SCREENH/2;
      if (drawStart < 0)
	drawStart = 0;
      int drawEnd = lineHeight / 2 + SCREENH / 2;
      if (drawEnd >= SCREENH)
	drawEnd = SCREENH - 1;
      Color color = WHITE;
      if (perpWallDist > 15){
	color = GRUVGREY;
      }
      else if (perpWallDist > 10){
	color = GRAY;
      } else if (perpWallDist > 5){
	color = LIGHTGRAY;
      }
      
      DrawLineEx((Vector2){x, drawStart}, (Vector2){x, drawEnd}, 1, color);
      
    }
    double frametime = GetFrameTime();
    double moveSpeed = frametime * 5.0;
    double rotSpeed = frametime * 3.0;
    if (IsKeyDown(KEY_W)){
      py += dirY * moveSpeed;
      px += dirX * moveSpeed;
    }
    if (IsKeyDown(KEY_S)){
      py -= dirY * moveSpeed;
      px -= dirX * moveSpeed;
    }
    if (IsKeyDown(KEY_D)){
      double oldDirX = dirX;
      dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
      dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
      double oldPlaneX = planeX;
      planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
      planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
    }
    if (IsKeyDown(KEY_A)){
      double oldDirX = dirX;
      dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
      dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
      double oldPlaneX = planeX;
      planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
      planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
    }
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
 
