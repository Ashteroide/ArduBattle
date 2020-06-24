#include <Arduboy2.h>
#include <ArduboyTones.h>

Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);

#include "characterPlane.h"
#include "TitleScreen.h"
#include "characterBullet.h"
#include "characterPlaneFlash.h"
#include "enemyPlane.h"

enum class GameState
{
  TitleScreen,
  GamePlay,
  GameOver,
};
GameState gameState = GameState::TitleScreen;

//Variables
int characterPlaneX;
int characterPlaneY;
int characterBulletX;
int characterBulletY;
int maxBulletDistance;
bool bulletFired;

int enemyPlaneX;
int enemyPlaneY;
bool enemyFlying;

void reset()
{
  
  characterPlaneX = 55;
  characterPlaneY =  35;
  characterBulletX = characterPlaneX + 10;
  characterBulletY = characterPlaneY - 15;
  bulletFired = false;

  enemyPlaneX = 30;
  enemyPlaneY = 2;
  enemyFlying = true;
}

void setup() {
  arduboy.begin();
  Arduboy2Audio::on();
  arduboy.audio.on();
  reset();

}

void loop() {
  if(!arduboy.nextFrame()) return;
  arduboy.pollButtons();
  arduboy.clear();

  switch(gameState)
  {
    case GameState::TitleScreen:
      updateTitleScreen();
      drawTitleScreen();
      break;
      
    case GameState::GamePlay:
      updateGamePlay();
      drawGamePlay();
      break;
      
    case GameState::GameOver:
      updateGameOver();
      drawGameOver();
      break;
  }
  arduboy.display();
}

//TitleScreen
void updateTitleScreen()
{
  if(arduboy.justPressed(A_BUTTON)) gameState = GameState::GamePlay;
}

void drawTitleScreen()
{
  Sprites::drawOverwrite(6, 14, titleScreen, 0);
  arduboy.setCursor(17,35);
  arduboy.print(F("Press A to Start"));
}

//GamePlay
void updateGamePlay()
{ 
  updateCharacterPlane();
  updateCharacterBullet();
  updateEnemyPlane();
  arduboy.setCursor(0,0);
  arduboy.print(bulletFired ? F("True") : F("False"));
}

void drawGamePlay()
{
  drawCharacterPlane();
  drawCharacterBullet();
}

//GameOver
void updateGameOver()
{
  if(arduboy.justPressed(A_BUTTON)) gameState = GameState::TitleScreen;
}

void drawGameOver()
{
  arduboy.print(F("Game Over!"));
}

//Character
void updateCharacterPlane()
{ 
  if(arduboy.pressed(LEFT_BUTTON) && (characterPlaneX > 0))
    --characterPlaneX;
  if(arduboy.pressed(RIGHT_BUTTON) && (characterPlaneX < 108))
    ++characterPlaneX;
  if(arduboy.pressed(UP_BUTTON) && (characterPlaneY > 0))
    --characterPlaneY;
  if(arduboy.pressed(DOWN_BUTTON) && (characterPlaneY < 51))
    ++characterPlaneY;
    
  maxBulletDistance = characterPlaneY - Arduboy2::height();
}

void drawCharacterPlane()
{
  Sprites::drawSelfMasked(characterPlaneX, characterPlaneY, characterPlane, 0);
}

//Character Bullet
void updateCharacterBullet()
{ 
  sound.tone(500, 500);
  if(arduboy.pressed(A_BUTTON))
  {
    if(bulletFired) drawPlaneBulletFlash();
    else fireBullet();
  }

  if(characterBulletY > maxBulletDistance) characterBulletY -= 4;
  else bulletFired = false;
}

void fireBullet()
{
  characterBulletX = characterPlaneX + 9;
  characterBulletY = characterPlaneY - 2;
  bulletFired = true;
}

void characterBulletFired()
{
  characterBulletX = characterPlaneX + 9;
  characterBulletY = characterPlaneY - 6;
}

void drawCharacterBullet()
{
  Sprites::drawOverwrite(characterBulletX, characterBulletY, characterBullet, 0);
}

void drawPlaneBulletFlash()
{
  Sprites::drawOverwrite(characterPlaneX + 7, characterPlaneY - 8, characterPlaneFlash, 0);
}



//Enemy Plane
void updateEnemyPlane()
{
  ++enemyPlaneY;
  if(enemyPlaneY > 80)
  {
    enemyPlaneY = -20;
  }
  updateEnemyPlaneDead();
  drawEnemyPlane();
}

void updateEnemyPlaneDead()
{
  if(enemyPlaneY == characterBulletY)
  {
    enemyPlaneY = -20;
  }
  else return;
}

void drawEnemyPlane()
{
  Sprites::drawSelfMasked(enemyPlaneX, enemyPlaneY, enemyPlane, 0);
}
