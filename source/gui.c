#include <3ds.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gui.h"
#include "app.h"
//#include "3dsUiLib/3dsUiLib.h"
#include <citro2d.h>

#define ctablesize 3
#define posxysize 240

int var1 = 240;
char buffer[100];

// typedef struct msgBoxButton
// {
// 	bool okay;
// 	bool okayAbort;
// };


// 	Home symbol
// 	Power symbol

///////////////////////////////////////////
//                 menues                //
///////////////////////////////////////////
C2D_TextBuf g_dynamicBuf, g_staticBufMenueTop, g_staticBufMenueBottom, g_staticBufSearch, g_staticBufSetting;
C2D_Text g_headlineText[3], g_menueText[3], g_staticText[13];

C2D_SpriteSheet spriteSheet;
C2D_Sprite img;
///////////////////////////////////////////
//                  C2D                  //
///////////////////////////////////////////

// void Draw_Text(char* str){
// 	C2D_TextBuf g_statBuf  = C2D_TextBufNew(4096); // support up to 4096 glyphs in the buffer
// 	C2D_Text text;
// 	C2D_TextParse(&text, g_statBuf, str);
// 	C2D_DrawText(&text, C2D_AtBaseline | C2D_WithColor, 170.0f, 230.0f, 0.5f, 1.0f, 1.0f, C2D_Color32f(0.4f,0.5f,1.0f,0.8f));
// }

void InitMenueScreen(void)
{
	g_staticBufMenueTop  = C2D_TextBufNew(4096); // support up to 4096 glyphs in the buffer
	g_staticBufMenueBottom  = C2D_TextBufNew(4096); // support up to 4096 glyphs in the buffer

	//Top screen
	// Parse the static text strings
	C2D_TextParse(&g_headlineText[0], g_staticBufMenueTop, "3Dio App Manager");
	C2D_TextParse(&g_staticText[0], g_staticBufMenueTop, "News");

	// Optimize the static text strings
	C2D_TextOptimize(&g_headlineText[0]);
	C2D_TextOptimize(&g_staticText[0]);
	////////////////////////////////////////////////////////////////////////////////


	//Bottom screen
	C2D_TextParse(&g_staticText[2], g_staticBufMenueBottom, newsContent);

	// Optimize the static text strings
	C2D_TextOptimize(&g_staticText[2]);
}

void InitMenueBar(void)
{
	g_staticBufMenueBottom  = C2D_TextBufNew(4096); // support up to 4096 glyphs in the buffer

	// Parse the static text strings
	C2D_TextParse(&g_menueText[0], g_staticBufMenueBottom, "News");
	C2D_TextParse(&g_menueText[1], g_staticBufMenueBottom, "Search");
	C2D_TextParse(&g_menueText[2], g_staticBufMenueBottom, "Settings");

	// Optimize the static text strings
	C2D_TextOptimize(&g_menueText[0]);
	C2D_TextOptimize(&g_menueText[1]);
	C2D_TextOptimize(&g_menueText[2]);
}

void InitSearchScreen(void)
{
	g_staticBufSearch  = C2D_TextBufNew(4096); // support up to 4096 glyphs in the buffer
	g_dynamicBuf = C2D_TextBufNew(4096);

	//Top Screen
	C2D_TextParse(&g_headlineText[1], g_staticBufSearch, "Search for");//Headline
	C2D_TextParse(&g_staticText[5], g_staticBufSearch, "App Details");//Headline
	C2D_TextOptimize(&g_headlineText[1]);
	C2D_TextOptimize(&g_staticText[5]);


	///////////////////////////////////////////
	//            Bottom screen              //
	///////////////////////////////////////////
	// C2D_TextParse(&g_staticText[3], g_staticBufSearch, "Search");
	// C2D_TextOptimize(&g_staticText[3]);



}

void InitSettingsScreen(void)
{
	g_staticBufSetting  = C2D_TextBufNew(4096); // support up to 4096 glyphs in the buffer


	// Parse the static text strings
	C2D_TextParse(&g_headlineText[2], g_staticBufSetting, "Settings");
	C2D_TextParse(&g_staticText[11], g_staticBufSetting, "Edit sources");
	C2D_TextParse(&g_staticText[12], g_staticBufSetting, "About");

	// Optimize the static text strings
	C2D_TextOptimize(&g_headlineText[2]);
	C2D_TextOptimize(&g_staticText[11]);
	C2D_TextOptimize(&g_staticText[12]);
}

//---------------------------------------------------------------------------------
void initImg() {
//---------------------------------------------------------------------------------
	// Load graphics
	spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
	if (!spriteSheet) svcBreak(USERBREAK_PANIC);
	C2D_SpriteFromSheet(&img, spriteSheet, 0);
	C2D_SpriteSetPos(&img, 200, 200);
}






//Renderfunctions
void renderMenueBar()
{

	int	newsColor = C2D_Color32f(0.5f,0.5f,0.5f,1.0f);
	int	searchColor = C2D_Color32f(0.5f,0.5f,0.5f,1.0f);
	int	settingsColor = C2D_Color32f(0.5f,0.5f,0.5f,1.0f);

	if (mode == 1){
		newsColor = C2D_Color32f(1.0f,1.0f,1.0f,1.0f);
		searchColor = C2D_Color32f(0.5f,0.5f,0.5f,1.0f);
		settingsColor = C2D_Color32f(0.5f,0.5f,0.5f,1.0f);
	} else if (mode == 2){
		newsColor = C2D_Color32f(0.5f,0.5f,0.5f,1.0f);
		searchColor = C2D_Color32f(1.0f,1.0f,1.0f,1.0f);
		settingsColor = C2D_Color32f(0.5f,0.5f,0.5f,1.0f);
	}else if (mode == 3){
		newsColor = C2D_Color32f(0.5f,0.5f,0.5f,1.0f);
		searchColor = C2D_Color32f(0.5f,0.5f,0.5f,1.0f);
		settingsColor = C2D_Color32f(1.0f,1.0f,1.0f,1.0f);
	}


	// Draw static text strings
	C2D_DrawRectSolid(0.0f, 200.0f, 0.0f, 320.0f, 40.0f, C2D_Color32f(0.9f,0.6f,0.2f,1.0f));
	C2D_DrawText(&g_menueText[0], C2D_AtBaseline | C2D_WithColor, 5.0f, 230.0f, 0.5f, 1.0f, 1.0f, newsColor);	
	C2D_DrawText(&g_menueText[1], C2D_AtBaseline | C2D_WithColor, 100.0f, 230.0f, 0.5f, 1.0f, 1.0f, searchColor);	
	C2D_DrawText(&g_menueText[2], C2D_AtBaseline | C2D_WithColor, 210.0f, 230.0f, 0.5f, 1.0f, 1.0f, settingsColor);	
	

}


/////////////////////////////////////////				Menue Screen 			////////////////////////////
void renderBottomMenueScreen()
{  
	//App news
	C2D_DrawText(&g_staticText[2], C2D_AtBaseline | C2D_WithColor, 20.0f, 30.0f, 0.5f, 0.5f, 0.5f, C2D_Color32f(0.0f,0.0f,0.0f,1.0f));	 

}

void renderTopMenueScreen()
{
	// Draw static text strings
	float text2PosX = 400.0f - 90.0f - g_headlineText[0].width*1.0f; // right-justify
	C2D_DrawText(&g_headlineText[0], C2D_AtBaseline | C2D_WithColor, text2PosX, 70.0f, 0.5f, 1.2f, 1.2f, C2D_Color32f(0.9f,0.6f,0.2f,1.0f));


	C2D_DrawText(&g_staticText[0], C2D_AtBaseline | C2D_WithColor, 170.0f, 230.0f, 0.5f, 1.0f, 1.0f, C2D_Color32f(0.4f,0.5f,1.0f,0.8f));
	// Draw_Text("test");

}
/////////////////////////////////////////				Search Screen 			////////////////////////////
void renderBottomSearchScreen()
{
	C2D_DrawCircleSolid(320.0f, 0.0f, 1.0f, 50.0f, C2D_Color32f(0.9f,0.6f,0.2f,1.0f));
	// C2D_DrawText(&g_staticText[3], C2D_AtBaseline | C2D_WithColor, 250.0f, 10.0f, 1.5f, 1.0f, 1.0f, C2D_Color32f(1.0f,1.0f,1.0f,1.0f));	

	//scrollbar
	C2D_DrawRectSolid(300.0f, 70.0f, 0.0f, 5.0f, 110.0f, C2D_Color32f(0.7f,0.7f,0.7f,1.0f));   
	C2D_DrawRectSolid(290.0f, 60.0f, 0.5f, 20.0f, 40.0f, C2D_Color32f(0.7f,0.7f,0.7f,1.0f));   


	//search result
	C2D_TextBufClear(g_dynamicBuf);	
	char buf[160];
	C2D_Text dynText;

	//Print Results
	// for(int index = 0; index <= countedApps; index++)
	for(int index = 0; index < countedApps; index++)
	{	
		// snprintf(buf, sizeof(buf), "%s", appList[0]);
		snprintf(buf, sizeof(buf), "%s", appName[index]);
		C2D_TextParse(&dynText, g_dynamicBuf, buf);
		C2D_TextOptimize(&dynText);
		C2D_DrawRectSolid(10.0f, 10.0f + index * 60.0f, 0.0f, 260.0f, 50.0f, C2D_Color32f(0.7f,0.7f,0.9f,1.0f));   
		C2D_DrawText(&dynText, C2D_AtBaseline | C2D_WithColor, 20.0f, 40.0f + index * 60.0f, 0.4f, 0.6f, 0.6f, C2D_Color32f(0.0f,0.0f,0.0f,1.0f));
	}

}

void renderTopSearchScreen(){
	//dynamic text
	C2D_TextBufClear(g_dynamicBuf);	
	char buf[160];
	C2D_Text dynText;

	//shows selected eltry
	snprintf(buf, sizeof(buf), "%i", selectedApp);
	C2D_TextParse(&dynText, g_dynamicBuf, buf);
	C2D_TextOptimize(&dynText); 
	C2D_DrawText(&dynText, C2D_AtBaseline | C2D_WithColor, 0.0f, 10.0f, 0.5f, 0.6f, 0.6f, C2D_Color32f(0.0f,0.0f,0.0f,1.0f));

	//float text2PosX = 400.0f - 90.0f - g_headlineText[1].width*1.0f; // right-justify
	//C2D_DrawText(&g_headlineText[1], C2D_AtBaseline | C2D_WithColor, text2PosX, 70.0f, 0.5f, 1.2f, 1.2f, C2D_Color32f(0.9f,0.6f,0.2f,1.0f));


	
	float searchText = 400.0f - 350.0f;
	snprintf(buf, sizeof(buf), "Search for \"%s\"", searchTerm);
	C2D_TextParse(&dynText, g_dynamicBuf, buf);
	C2D_TextOptimize(&dynText);
	C2D_DrawText(&dynText, C2D_AtBaseline | C2D_WithColor, searchText, 70.0f, 0.5f, 1.2f, 1.2f, C2D_Color32f(0.9f,0.6f,0.2f,1.0f));
	

	C2D_DrawRectSolid(10.0f, 100.0f, 0.0, 380.0f, 130.0f, C2D_Color32f(0.6f,0.6f,0.6f,1.0f));

	//app details-box
	C2D_DrawText(&g_staticText[5], C2D_AtBaseline | C2D_WithColor, 150.0f, 130.0f, 0.5f, 0.8f, 0.8f, C2D_Color32f(1.0f,1.0f,1.0f,1.0f));

	//int entry = 2;
	snprintf(buf, sizeof(buf), "%s", appDetails[selectedApp]);// + entry*2]);
	C2D_TextParse(&dynText, g_dynamicBuf, buf);
	C2D_TextOptimize(&dynText); 
	C2D_DrawText(&dynText, C2D_AtBaseline | C2D_WithColor, 20.0f, 150.0f, 0.5f, 0.6f, 0.6f, C2D_Color32f(0.0f,0.0f,0.0f,1.0f));



}
/////////////////////////////////////////				Settings Screen 			////////////////////////////
void renderBottomSettingsScreen()
{
	//"Edit Sources"-Button
	C2D_DrawRectSolid(10.0f, 10.0f, 0.0f, 110.0f, 30.0f, C2D_Color32f(0.5f,0.5f,0.5f,1.0f));   
	C2D_DrawText(&g_staticText[11], C2D_AtBaseline | C2D_WithColor, 20.0f, 30.0f, 0.5f, 0.6f, 0.6f, C2D_Color32f(1.0f,1.0f,1.0f,1.0f));	   
   
   	//"Abouts"-Button
	C2D_DrawRectSolid(10.0f, 10.0f, 0.0f, 110.0f, 30.0f, C2D_Color32f(0.5f,0.5f,0.5f,1.0f));   
	C2D_DrawText(&g_staticText[12], C2D_AtBaseline | C2D_WithColor, 210.0f, 180.0f, 0.0f, 0.6f, 0.6f, C2D_Color32f(0.5f,0.5f,0.5f,1.0f));	 
}   
   
void renderTopSettingsScreen()
{
	float text2PosX = 400.0f - 130.0f - g_headlineText[2].width*1.2f; // right-justify
	C2D_DrawText(&g_headlineText[2], C2D_AtBaseline | C2D_WithColor, text2PosX, 70.0f, 0.5f, 1.2f, 1.2f, C2D_Color32f(0.9f,0.6f,0.2f,1.0f));
}


void MessageBox(char* title, char* message, int buttons, int screen)
{
	C2D_TextBufClear(g_dynamicBuf);
	
	char buf[160];
	C2D_Text messageText;
	C2D_Text titleText;
	C2D_Text buttonsText;

	C2D_DrawRectSolid(0.0f, 0.0f, 0.7f, 400.0f, 240.0f, C2D_Color32f(0.2f,0.2f,0.2f,0.2f));//Background dimmed
	C2D_DrawRectSolid(30.0f, 30.0f, 0.8f, 340.0f, 180.0f, C2D_Color32f(0.8f,0.8f,0.8f,1.0f)); 

	float align_center = 200.f - (strlen(title)*1.0f)/2;
	snprintf(buf, sizeof(buf), "%s", title);
	C2D_TextParse(&titleText, g_dynamicBuf, buf);
	C2D_TextOptimize(&titleText); 
	// C2D_DrawText(&titleText, C2D_AtBaseline | C2D_WithColor, 40.0f, 60.0f, 0.9f, 1.0f, 1.0f, C2D_Color32f(0.0f,0.0f,0.0f,1.0f));
	C2D_DrawText(&titleText, C2D_AtBaseline | C2D_WithColor, align_center, 60.0f, 0.9f, 1.0f, 1.0f, C2D_Color32f(0.0f,0.0f,0.0f,1.0f));


	snprintf(buf, sizeof(buf), "%s", message);
	C2D_TextParse(&messageText, g_dynamicBuf, buf);
	C2D_TextOptimize(&messageText); 
	C2D_DrawText(&messageText, C2D_AtBaseline | C2D_WithColor, 40.0f, 90.0f, 0.9f, 0.6f, 0.6f, C2D_Color32f(0.0f,0.0f,0.0f,1.0f));

	if (buttons == 1){		
		snprintf(buf, sizeof(buf), "%s", " Okay");
		C2D_TextParse(&buttonsText, g_dynamicBuf, buf);
		C2D_TextOptimize(&buttonsText); 
		C2D_DrawText(&buttonsText, C2D_AtBaseline | C2D_WithColor, 170.0f, 200.0f, 0.9f, 0.6f, 0.6f, C2D_Color32f(0.0f,0.0f,0.0f,1.0f));
	} else 	if (buttons == 2){		
		snprintf(buf, sizeof(buf), "%s", " Okay");
		C2D_TextParse(&buttonsText, g_dynamicBuf, buf);
		C2D_TextOptimize(&buttonsText); 
		C2D_DrawText(&buttonsText, C2D_AtBaseline | C2D_WithColor, 100.0f, 200.0f, 0.9f, 0.6f, 0.6f, C2D_Color32f(0.0f,0.0f,0.0f,1.0f));

		snprintf(buf, sizeof(buf), "%s", " Abort");
		C2D_TextParse(&buttonsText, g_dynamicBuf, buf);
		C2D_TextOptimize(&buttonsText); 
		C2D_DrawText(&buttonsText, C2D_AtBaseline | C2D_WithColor, 240.0f, 200.0f, 0.9f, 0.6f, 0.6f, C2D_Color32f(0.0f,0.0f,0.0f,1.0f));
	}

}

   

 
void exitScene(void)
{
	// Delete the text buffers
	C2D_TextBufDelete(g_dynamicBuf);
	C2D_TextBufDelete(g_staticBufMenueTop);
	C2D_TextBufDelete(g_staticBufMenueBottom);
	C2D_TextBufDelete(g_staticBufSetting);
	C2D_TextBufDelete(g_staticBufSearch);

	//delete sprites
	// C2D_SpriteSheetFree(spriteSheet);
}
