#include <citro2d.h>
#include <3ds.h>
#ifndef GUI_H
#define GUI_H



extern C2D_TextBuf g_staticBuf, g_dynamicBuf;
// extern C2D_Text g_staticText[12];



void renderMenueBar();


void renderTopMenueScreen();
void renderBottomMenueScreen();

void renderTopSearchScreen();
void renderBottomSearchScreen();

void renderTopSettingsScreen();
void renderBottomSettingsScreen();



void InitMenueScreen();

void InitSettingsScreen();
void InitMenueBar();
void InitSearchScreen();
void initImg();
extern C2D_Sprite img;


void MessageBox(char* title, char* message, int buttons, int screen);

void exitScene();

#endif
