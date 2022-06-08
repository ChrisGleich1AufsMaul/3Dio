#include <3ds.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "main.h"
#include "app.h"
#include "input.h"
#include "gui.h"
#include <citro2d.h>

char* msgTitle = "";
char* msgMessage = "";
int msgButtons = 1;

// C2D_Sprite img;

int main()
{
///////////////////////////////////////////
//                  C2D                  //
///////////////////////////////////////////	
	// Initialize the libs
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();

	// Create screen
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);




	StartUp();
	InitMenueScreen();
	InitMenueBar();
	InitSearchScreen();
	InitSettingsScreen();
	// initImg();
	


	// Main loop
	while (aptMainLoop())
	{


		getInput();
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break;

		

		app();


		// The content is rendered depending on the mode
		if (mode == 1) // NEWS/HOME
		{
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(top, C2D_Color32(0xD9, 0xD9, 0xD9, 0xD9));
			C2D_SceneBegin(top);
			 	renderTopMenueScreen();
				
			C2D_TargetClear(bottom, C2D_Color32(0xD9, 0xD9, 0xD9, 0xD9));
			C2D_SceneBegin(bottom);
				renderBottomMenueScreen();
				renderMenueBar();
			C3D_FrameEnd(0);
		}
		else if (mode == 2)//Search
		{
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(top, C2D_Color32(0xD9, 0xD9, 0xD9, 0xD9));
			C2D_SceneBegin(top);
				renderTopSearchScreen();


		// 	char currentSourceContentSplitted[1000];
		// 	strcpy(currentSourceContentSplitted, currentSourceContent);
		// 	//int property = 0;
		// 	int entryIndex = 0;
		// 	//bool save = true;
		// 	int len = strlen(currentSourceContentSplitted);

		// 	for (int i = 0; i <= len; i++){

		// 		//in case it's a seperator: new entry
		// 		if (currentSourceContentSplitted[i] == ','){
		// 			currentSourceContentSplitted[i] = 0;
		// 			entryIndex++;
		// 			//property = 0;
		// 			//save = true;
		// 		//in case it's a seperator: property of an entry
		// 		}
		// 		//else if (currentSourceContentSplitted[i] == '|'){
		// 		// 	currentSourceContentSplitted[i] = 0;
		// 		// 	property++;
		// 		// 	save = true;
		// 		// //otherwise it needs to be 'save = true'
		// 		// }else if (save == true) {
		// 		// 	if(property == 0){
		// 		// 		appName[entryIndex] = &currentSourceContentSplitted[i];//wird der ganze array übertragen?
		// 		// 	}else if(property == 1){
		// 		// 		appDetails[entryIndex] = &currentSourceContentSplitted[i];
		// 		// 	}else if(property == 2){
		// 		// 		appPath[entryIndex] = &currentSourceContentSplitted[i];
		// 		// 	}
		// 		// 	save = false;

		// 		//} 
		// 		else {
		// 			appName[entryIndex] += currentSourceContentSplitted[i];
		// 		}

		// 		// if (currentSourceContentSplitted[i] == 0){
		// 		// 	break;
		// 		// }
		// 	}
			
		// 	gfxInitDefault();
		// 	consoleInit(GFX_TOP, NULL);

		// 	printf("Apps: %d\n", countedApps);
		// 	printf("Quelleninhalt: %s\n\n", currentSourceContent);
		// 	for(int i = 0; i < countedApps; i++)
		// 	{
		// 		printf("\n\tName: \t%s\n", appName[i]);
		// 		printf("\tDetails: %s\n", appDetails[i]);
		// 		printf("\tPath: \t%s\n\n", appPath[i]);
		// 	}
		// 	/*printf(appName[0]);//Richtig sortiert
		// 	printf(appName[1]);//Richtig sortiert
		// 	printf(appName[2]);//falsch	 sortiert*/

				
			C2D_TargetClear(bottom, C2D_Color32(0xD9, 0xD9, 0xD9, 0xD9));
			C2D_SceneBegin(bottom);
				renderBottomSearchScreen();
				renderMenueBar();
				// C2D_DrawSprite(&img);
			C3D_FrameEnd(0);
			
		}
		else if (mode == 3)//Settings
		{
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(top, C2D_Color32(0xD9, 0xD9, 0xD9, 0xD9));
			C2D_SceneBegin(top);
				renderTopSettingsScreen();
			if(msgbox){
				C2D_SceneBegin(top);
					MessageBox(msgTitle, msgMessage, msgButtons, 1);				
			}
			
				
			C2D_TargetClear(bottom, C2D_Color32(0xD9, 0xD9, 0xD9, 0xD9));
			C2D_SceneBegin(bottom);
				renderBottomSettingsScreen();
				renderMenueBar();
			
			C3D_FrameEnd(0);
		}
		else if (mode == 99)//No internet connection
		{
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(top, C2D_Color32(0xD9, 0xD9, 0xD9, 0xD9));
			C2D_SceneBegin(top);
				MessageBox("Internet connection Failed", "       The 3DS is not connected to the \n                        internet.", 1, 1);
			C3D_FrameEnd(0);	
			//renderMsgBox("Internet connection Failed", "    Your Nintendo is not connected to the \n                        internet.", 1);		
		}

		// if(msgbox){
		// 	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		// 	C2D_TargetClear(top, C2D_Color32(0xD9, 0xD9, 0xD9, 0xD9));
		// 	C2D_SceneBegin(top);
		// 		MessageBox(msgTitle, msgMessage, msgButtons);
		// 	C3D_FrameEnd(0);				
		// }


		//Exit code
		if (mode==0) break;

		
		// Flush and swap framebuffers
		gfxFlushBuffers();
		//gfxSwapBuffers();

		//Wait for VBlank
		gspWaitForVBlank();	


	}
	// Deinitialize the scene
	exitScene();
	// Deinitialize the libs
	C2D_Fini();
	C3D_Fini();	
	gfxExit();

	return 0;
}

void renderMsgBox(char* title, char* message, int buttons){
	// C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	// C2D_TargetClear(top, C2D_Color32(0xD9, 0xD9, 0xD9, 0xD9));
	// C2D_SceneBegin(top);
	// 	MessageBox(title, message, buttons);
	// C3D_FrameEnd(0);
	msgTitle = title;
	msgMessage = message;
	msgButtons = buttons;
}





