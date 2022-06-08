#ifndef APP_H
#define APP_H

void app();
void StartUp();
void variableReset();
extern int checkStatus();

extern int mode;
extern bool msgbox;
extern int ElementScrollvalue;
extern int ScrollBarValue;
extern int rendered;
extern char *searchTerm;
extern int selectedApp;

extern char *newsContent;

extern int countedSources;
extern char *appList[];
extern char *listAddresses[];
extern int countedApps;
extern char *appName[];
extern char *appDetails[];
extern char *appPath[];

// char *currentSourceContent;
extern char currentSourceContent[];
extern char source[];

extern char *appNames;
// extern char appDetails[];
// extern char appPaths[];

#endif
