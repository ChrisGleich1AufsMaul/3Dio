#include <3ds.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "app.h"
#include "input.h"
#include "gui.h"
#include <stdbool.h>
#include <citro2d.h>
#include <dirent.h>
#include <inttypes.h>
#include "main.h"


int state = 0;
int mode = 1;
bool msgbox = false;

char *searchTerm = "...";
int selectedApp = 0;

char listFile[] = "";

char *newsContent = "Could not download the latest news!";

int countedSources = 0;
int countedApps = 0;
char *listAddresses[];
char *listAddressesCopy[];
char *appList[100];
char *appName[];
char *appDetails[];
char *appPath[];
char currentSourceContent[1000];





//####################################################################################################
//									Touch button event checker
//####################################################################################################
int ButtonClick(char* name, int posX, int posY, int width, int height, int touchX, int touchY) {
	int endX = posX + width - 1;
	int endY = posY + height - 1;

	if (touchX >= posX && touchY >= posY && touchX <= endX && touchY <= endY) {
		return true; //TRUE
	} else {
		return false; //FALSE
	}
}
static SwkbdCallbackResult MyCallback(void* user, const char** ppMessage, const char* text, size_t textlen)
{
	if (strstr(text, "lenny"))
	{
		*ppMessage = "Nice try but I'm not letting you use that meme right now";
		return SWKBD_CALLBACK_CONTINUE;
	}

	if (strstr(text, "brick"))
	{
		*ppMessage = "~Time to visit Brick City~";
		return SWKBD_CALLBACK_CLOSE;
	}

	return SWKBD_CALLBACK_OK;
}


//####################################################################################################
//										download functions
//####################################################################################################
httpcContext context;
// Result downloadFromUrl(HTTPC_RequestMethod method, const char* url)
char* downloadFromUrl(HTTPC_RequestMethod method, const char* url)
{
	Result ret;
	u32 statuscode = 0;
	u32 contentsize = 0;
	u8 *buf;


	httpcInit(0x1000);
	ret = httpcOpenContext(&context, method, url, 0);


	ret = httpcBeginRequest(&context);
	// if (ret != 0)return ret;

	ret = httpcGetResponseStatusCode(&context, &statuscode);//, 0);
	// if (ret != 0)return ret;



	ret = httpcGetDownloadSizeState(&context, NULL, &contentsize);
	// if (ret != 0)return ret;
	unsigned char *buffer = (unsigned char*)malloc(contentsize + 1);


	buf = (u8*)malloc(contentsize);
	// if (buf == NULL)return -1;
	memset(buf, 0, contentsize);


	ret = httpcDownloadData(&context, buffer, contentsize, NULL);
	if (ret != 0)
	{
		free(buf);
		// return ret;
	}


	free(buf);
	return buffer;
	httpcCloseContext(&context);

}
void downloadLists()
{
	//read sources.ini from local
	FILE* fp;
	fp = fopen("3Dio/DownloadFiles/sources.ini","r");
	int size;
	char buffer[1000];
	while((size=fread(buffer,1000,sizeof(char),fp)>0))
			fwrite(buffer,size,sizeof(char),stdout);
	fclose(fp);

	//counts the different Addresses and splits by every ','
	const char *p = buffer;
    do {
        if (*p == ',')
            countedSources++;
    } while (*(p++));
	countedSources++;

	//saves the different (http://) Addresses to listAddresses
    int i = 0;
    char *pToken = strtok (buffer, ",");
    while (pToken != NULL)
    {
        listAddresses[i++] = pToken;
        listAddressesCopy[i] = pToken;
        pToken = strtok (NULL, ",");
	}


	//downloads the listAddresses and saves them to applist[]
	for (int i = 0; i < countedSources; i++)
	{
		strcat(listAddresses[i], "/_appNames.txt");
		char *source = downloadFromUrl(HTTPC_METHOD_GET, listAddresses[i]);

		//copy source in another
		char s[1000];//maximum size;
		strcpy(s, source);
		strcpy(currentSourceContent, source);


		//counts the different Apps
		do {
			if (*source == ',')
				countedApps++;
		} while (*(source++));
		countedApps++;




 ////////////////////////////
		//puts the source.txt content in appList[]-array
		// int i = -2;		
		// char *p = strtok(s, ",");
		// while (p != NULL)
		// {
		// 	//splits the app information
		// 	// appList[i++] = p;
		// 	appList[i] = p;
		// 	p = strtok (NULL, ","); 
		// 	i++;
		// }
		// for(int i = 0; i<= countedApps; i++)
		// 	appList[i] = p;

		// //splits the app information
		// for(int i = 0; i <= countedApps; i++)
		// {	
		// 	// char *pToken = strtok (appList[i], "|");
		// 	char *pToken = strtok (appListCopy[i], "|");
		// 	int counter = 0;
		// 	while (pToken != NULL)//not NULL
		// 	{
		// 		if(counter == 0)
		// 			appName[i] = pToken;
					
		// 		if(counter == 1)
		// 			appDetails[i] = pToken;
					
		// 		if(counter == 2)
		// 			appPath[i] = pToken;//strcat(appList[i], pToken);

		// 		pToken = strtok (NULL, "|");
		// 		counter++;
		// 	}
		// 	counter = 0;
// }
	}

}

int checkStatus()
{
	httpcContext contextCheck;
	int ret;
	u32 statuscode = 0;

	httpcInit(0x1000);
	ret = httpcOpenContext(&contextCheck, HTTPC_METHOD_GET, "http://google.com", 0);

	ret = httpcBeginRequest(&contextCheck);
	if (ret != 0)return ret;

	ret = httpcGetResponseStatusCode(&contextCheck, &statuscode);
	if (ret != 0)return ret;

	return statuscode;
}
//####################################################################################################




bool ignoreMessage = false;
//repeats
//Different events and actions, based of the current mode
void app()
{

	u32 kDown = hidKeysDown();


	//Switch modes by pressing on the D-Pad left/right
	if (kDown & KEY_RIGHT){
		if (mode < 3)
			mode++;
	}

	if (kDown & KEY_LEFT){
		if (mode > 1)
			mode--;
	}



	bool diditSearch = false;
	bool didit = false;
	static SwkbdState swkbd;
	static char mybuf[60];
	static SwkbdStatusData swkbdStatus;
	static SwkbdLearningData swkbdLearning;
	SwkbdButton button = SWKBD_BUTTON_NONE;



//////////////////////////////////
	// httpcContext contextCheck;
	// int ret;
	// u32 statuscode = 0;

	// httpcInit(0x1000);
	// ret = httpcOpenContext(&contextCheck, HTTPC_METHOD_GET, "http://google.com", 0);
	// ret = httpcBeginRequest(&contextCheck);
	// ret = httpcGetResponseStatusCode(&contextCheck, &statuscode);

	//// if ((checkStatus() != 301) )//&& (ignoreMessage == false))//No connection -> ERROR
	// if ((statuscode == -660561818) && (ignoreMessage == false))//No connection -> ERROR
	// {
	// 	mode = 99;
	// }
	// // else{
	// // 	ignoreMessage = true;
	// // }


	// // if ((checkStatus() == 301) && (ignoreMessage))//connection -> reset Error message
	// // {
	// // 	ignoreMessage == false;
	// // 	StartUp();
	// // }

	if (mode == 99)//Error message box
	{
		// if (kDown & KEY_A)//By pressing A, message will be closed and you can continue
		// {
		// 	mode = 1;
		// 	ignoreMessage = true;
		// }
	}



	if (mode == 1)//App menu
	{
		//menue bar
		if (ButtonClick("search", 100, 200, 110, 40, posX, posY)) {
			mode = 2; //Mode for search
		}
		if (ButtonClick("settings", 210, 200, 110, 40, posX, posY)) {
			mode = 3; //Mode for settings
		}
	}
	else if (mode == 2) //SEARCH
	{
		//menue bar
		if (ButtonClick("news", 0, 200, 100, 40, posX, posY)) {
			mode = 1; //Mode for News
		}
		if (ButtonClick("settings", 210, 200, 110, 40, posX, posY)) {
			mode = 3; //Mode for settings
		}

		//Touchfunctions
		if (ButtonClick("SearchButton", 270, 0, 50, 50, posX, posY)){//Bottom Screen: Top Right
			diditSearch = true;
			swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 2, -1);
			//swkbdSetInitialText(&swkbd, mybuf);
			swkbdSetHintText(&swkbd, "Enter Searchterm here");
			swkbdSetValidation(&swkbd, SWKBD_NOTEMPTY_NOTBLANK, 0, 0);
			swkbdSetFilterCallback(&swkbd, MyCallback, NULL);
			swkbdSetButton(&swkbd, SWKBD_BUTTON_RIGHT, "Search...", true);
			swkbdSetFeatures(&swkbd, SWKBD_PREDICTIVE_INPUT);
			static bool reload = false;
			swkbdSetStatusData(&swkbd, &swkbdStatus, reload, true);
			swkbdSetLearningData(&swkbd, &swkbdLearning, reload, true);
			reload = true;
			button = swkbdInputText(&swkbd, mybuf, sizeof(mybuf));

		}
		
		if (ButtonClick("Placeholder1", 10, 10, 260, 50, posX, posY)){
			selectedApp = 0;
		}		

		if (ButtonClick("Placeholder2", 10.0f, 10.0f + 1 * 60.0f, 260, 50, posX, posY)){
			selectedApp = 1;
		}

		if (ButtonClick("Placeholder3", 10.0f, 10.0f + 2 * 60.0f, 260, 50, posX, posY)){
			selectedApp = 2;
		}


	}
	else if (mode == 3) //SETTINGS
	{
		if(msgbox == false)
		{		
			//menue bar
			if (ButtonClick("news", 0, 200, 100, 40, posX, posY)) {
				mode = 1;
			}
			if (ButtonClick("search", 100, 200, 110, 40, posX, posY)) {
				mode = 2;
			}


			if (ButtonClick("SourcesIni", 10, 10, 110, 30, posX, posY) && didit == false) {
				FILE* fp;
				fp = fopen("3Dio/DownloadFiles/sources.ini","r");


				int size;
				char buffer[1000];
				while((size=fread(buffer,1000,sizeof(char),fp)>0))
						fwrite(buffer,size,sizeof(char),stdout);
				fclose(fp);


				// didit = true;
				// swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 2, -1);
				// swkbdSetInitialText(&swkbd, buffer);
				// swkbdSetHintText(&swkbd, "Edit sources.ini");
				// swkbdSetValidation(&swkbd, SWKBD_NOTEMPTY_NOTBLANK, 0, 0);
				// swkbdSetFilterCallback(&swkbd, MyCallback, NULL);
				// swkbdSetButton(&swkbd, SWKBD_BUTTON_RIGHT, "Finish", true);
				// swkbdSetFeatures(&swkbd, SWKBD_PREDICTIVE_INPUT);
				// SwkbdDictWord words[2];
				// swkbdSetDictWord(&words[0], "chrisonline", "http://chrisonline.bplaced.net/3Dio/source.txt");
				// swkbdSetDictWord(&words[1], "github", "https://www.github.com/");
				// swkbdSetDictionary(&swkbd, words, sizeof(words) / sizeof(SwkbdDictWord));
				// static bool reload = false;
				// swkbdSetStatusData(&swkbd, &swkbdStatus, reload, true);
				// swkbdSetLearningData(&swkbd, &swkbdLearning, reload, true);
				// reload = true;
				// button = swkbdInputText(&swkbd, mybuf, sizeof(mybuf));

			}

			if (ButtonClick("About", 210, 170, 60, 10, posX, posY)){
				msgbox = true;
				renderMsgBox("About", "©2021 ChrisCross19\n github.com/ChrisCross19", 1);

			}
		}
		else
		{
			if (kDown & KEY_A){
				msgbox = false;
			}
		}


	}


	//search keyboard
	if (diditSearch)
	{
		if (button != SWKBD_BUTTON_LEFT)
		{
			searchTerm = mybuf;
			//Downloads the lists from the Internet
			//downloadLists();
		}
	}
	diditSearch = false;

	//Edit sources.ini
	if (didit)
	{
		if (button != SWKBD_BUTTON_LEFT)
		{
			FILE* fp;
			fp = fopen("3Dio/DownloadFiles/sources.ini","w");
			fprintf(fp, "%s", mybuf);
			fclose(fp);
		}
	}
	didit = false;
}






void StartUp()
{

	//if (mkdir("3Dio", 0777)!= EEXIST)
		mkdir("3Dio", 0777);

	//if (mkdir("3Dio/DownloadFiles", 0777)!= EEXIST)
		mkdir("3Dio/DownloadFiles", 0777);

	FILE* fp; // Zeiger auf Datenstrom der Datei
	if((fopen("3Dio/DownloadFiles/sources.ini", "r")) == NULL)
	{
		fp = fopen("3Dio/DownloadFiles/sources.ini","w");
		fprintf(fp, "http://chrisonline.bplaced.net/3dio/source.txt");
		fclose(fp);
	}


	//load news 
	if(checkStatus() == 301){
		newsContent = (char*) downloadFromUrl(HTTPC_METHOD_GET, "http://chrisonline.bplaced.net/3dio/news.txt");
		//downloadLists();
	}
}