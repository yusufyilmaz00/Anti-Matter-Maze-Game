#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <windows.h>
#include <time.h>

#define SIZE 24
#define STR_LEN 20
#define LINESIZE 100
#define HIGHSCORESIZE 5
#define MAX_TIME 20.0
#define MAP_DIRECTORY "./maps/" //map dosyasinin dizini
#define SCORETXT "scores.txt" //skorlarin tutuldugu txt dosyasinin ismi

struct FileNames {
    char names[100][256];
    int count;
};


typedef struct Data{
	char *name;
	char *surname;
	char *username;
	char *password;
}DATA;

typedef struct{
	char *username;
	int point;
}SCORE;


// account information functions
int searchUser(DATA *userlist, int size, char *username);
DATA* addUser(DATA *userlist, int* size, DATA *newUser);
SCORE* readScoreList(char *filename);
void printHighScores(SCORE *scoreList);
// account control functions
void loginPage(DATA *userlist,int size,DATA *input,SCORE *highscores);
DATA* siginPage(DATA *userlist,int *size);

//menu functions
void gotoGameMenu(SCORE *highscores,char *username);
char** selectMapMode(char **board,int *R, int *C);
int selectPlayMode(char **board,int *R, int *C);


// file functions
void showTutorial();
void readTutorial(char *filename);
void showHighScores(char *filename);
void list_map_files(const char *folder_path, struct FileNames *fileNames);
void writeAccountListBinary(DATA *userlist,int size,char *filename);
DATA* readAccountList(int *size,char *filename);
// map functions
char** readMapFromFile(char *filename,int *R,int *C);
void printBoard(char** board, int *R,int *C);
// gameplay functions
int userGameplay(char** board,int *R, int *C);
int autoGameplay(char** board,int *R, int *C);
char* addParticle(char *particleList,char object);
void printParticles(char *particleList);
int calculateScore(char* particles,double elapsedTime);
void updateHighscores(SCORE *highscores,int score,char *username);

int main(){
	int list_size=1;
	

	DATA *userlist= calloc(list_size,sizeof(DATA));
	userlist[0].name = "Yusuf";
	userlist[0].surname = "Yilmaz";
	userlist[0].password = "1234";
	userlist[0].username = "CENTURY";
	
	DATA *dataInput= calloc(1,sizeof(DATA));
	dataInput->name = (char*) calloc(STR_LEN, sizeof(char));
	dataInput->surname = (char*) calloc(STR_LEN, sizeof(char));
	dataInput->username = (char*) calloc(STR_LEN, sizeof(char));
	dataInput->password = (char*) calloc(STR_LEN, sizeof(char));

	SCORE *highscores = readScoreList(SCORETXT);
	
	int i,lmFlag = 1,lmInput; // login menu flag & login menu �nput

	while(lmFlag == 1){
		system("cls");
	    printf("\n\t________________________\n");
		printf("\t|                      |\n");
		printf("\t|   ANTI-MATTER MAZE   |\n");
		printf("\t|______________________|\n");
		printf("\t|                      |\n");
		printf("\t|   #1 LOGIN           |\n");
		printf("\t|______________________|\n");
		printf("\t|                      |\n");
		printf("\t|   #2 SIGN-IN         |\n");
		printf("\t|______________________|\n");
		printf("\t|                      |\n");
		printf("\t|   #3 QUIT            |\n");
		printf("\t|______________________|\n\n");	
		
		printf("\tYour choice: ");
		scanf("%d",&lmInput);
		
        switch (lmInput)
        {
        case 1:
            loginPage(userlist,list_size,dataInput,highscores);
            break;
        case 2:
            userlist = siginPage(userlist,&list_size);
            break;
        case 3:
            printf("\n The program will be closed soon...\n");
            sleep(1);
            lmFlag =0;
            break;
        
        default:
            printf("\n Invalid ID, Enter a valid ID !\n");
            sleep(1);
            break;
        }
		
	}

	
	writeAccountListBinary(userlist,list_size,"accounts.txt");
	free(userlist);
	free(dataInput);

    return 0;
}

// kullanici kontrol fonksiyonlari

int searchUser(DATA *userlist,int size, char *username){
	// if the user is registered in the database returns 1 else 0 
	int i;

	if(userlist == NULL){
		// database NULL control	
		return 0;
	}

	for(i=0; i < size ;i++){
		if( strcmp(userlist[i].username,username) == 0 ){
		return 1;
		}
	}
	return 0;
}

DATA* addUser(DATA *userlist,int *size, DATA *newUser){
	*size +=1;
	DATA *newList = (DATA*) realloc(userlist,(*size) * sizeof(DATA));

	newList[(*size)-1].name = strdup(newUser->name);
	newList[(*size)-1].surname = strdup(newUser->surname);
	newList[(*size)-1].username = strdup(newUser->username);
	newList[(*size)-1].password = strdup(newUser->password);
	return newList;
}

char* getPassword(DATA *userlist,int size, char *username){
	int i;
	// We know that the user whose user information we are looking for is registered in the system. 
	for(i=0; i< size;i++){
		if(strcmp(userlist[i].username,username) == 0 ){
			return userlist[i].password;
		}
	}
}

void writeAccountListBinary(DATA *userlist,int size,char *filename){
	FILE *fp = fopen(filename,"wb");
	if(fp ==NULL){
		printf("File Opening Error !\n");
		exit(0);
	}
	int i;
	
	for(i=0; i < size ; i++){
		fwrite(&userlist[i],sizeof(DATA),1,fp);
	}
	
	fclose(fp);
}

DATA* readAccountList(int *size,char *filename){
    FILE *fp = fopen(filename,"rb");
	if(fp ==NULL){
		printf("File Opening Error !\n");
		return NULL;
	}
	*size=1;
	DATA *accounts= calloc(1,sizeof(DATA));   
    accounts[*size].name = (char*) calloc(STR_LEN,sizeof(char));
    accounts[*size].surname = (char*) calloc(STR_LEN,sizeof(char));
    accounts[*size].username = (char*) calloc(STR_LEN,sizeof(char));
    accounts[*size].password = (char*) calloc(STR_LEN,sizeof(char));

	fread(&accounts[(*size)-1],sizeof(DATA),1,fp);
    while(!feof(fp)){
        printf("%s %s %s %s\n",accounts[(*size)-1].name,accounts[(*size)-1].surname,accounts[(*size)-1].username,accounts[(*size)-1].password);
		*size +=1;
		accounts = (DATA*) realloc(accounts,(*size)*sizeof(DATA));
		
		accounts[*size].name = (char*) calloc(STR_LEN,sizeof(char));
		accounts[*size].surname = (char*) calloc(STR_LEN,sizeof(char));
		accounts[*size].username = (char*) calloc(STR_LEN,sizeof(char));
		accounts[*size].password = (char*) calloc(STR_LEN,sizeof(char));
		
		fread(&accounts[(*size)-1],sizeof(DATA),1,fp);
    }
	fclose(fp);
	return accounts;
}

SCORE* readScoreList(char *filename){
	FILE *fp = fopen(filename,"r");
	if(fp ==NULL){
		printf("File Opening Error !\n");
		exit(0);
	}
	SCORE *highscores = calloc(HIGHSCORESIZE+1,sizeof(SCORE));
	int i;
	
	for(i=0;i<HIGHSCORESIZE+1; i++){
		highscores[i].username = (char*) calloc(STR_LEN,sizeof(char));
	}
	
	i=0;
	while(!feof(fp)){
		fscanf(fp,"%s %d",highscores[i].username, &highscores[i].point);
		i++;
	}

	return highscores;
}

void printHighScores(SCORE *scoreList){
	int i;
	system("cls");
	printf("TOP 5 HIGH SCORES\n\n");
	printf("# USERNAME          # SCORE\n");
	printf("------------------------------\n");
	for(i=0;i<HIGHSCORESIZE;i++){
		printf("%s %*d\n",scoreList[i].username,(24-strlen(scoreList[i].username)),scoreList[i].point);
	}

	printf("Back to Game Menu: ");
	scanf("%d",&i);
}

// Menu Fonksiyonlari
void loginPage(DATA *userlist,int size,DATA *input,SCORE *highscores){
	system("cls");
	printf("\n");
	printf("\t_________________________________\n");
	printf("\t|                               |\n");
	printf("\t|          LOGIN PAGE           |\n");
	printf("\t|_______________________________|\n");
	printf("\t                                \n");
	printf("\t  Username: ");
	scanf("%s",input->username);
	
	while(searchUser(userlist,size,input->username)==0){
		printf("\t  Account could not be found\n");
		printf("\t  Username: ");
		scanf("%s",input->username);
	}
	printf("\t|_______________________________|\n");
	printf("\t                                \n");
	sleep(0.5);
	printf("\t  Password: ");
	scanf("%s",input->password);
	
	if(!strcmp(input->password, getPassword(userlist,size,input->username))){
		printf("\n\t  login successful\n");
		printf("\t|_______________________________|\n");
		
		// bir sonraki menuye gecis yapsin
		gotoGameMenu(highscores,input->username);

	}
	else{
		printf("\a\n\t  Password is uncorrect !\n");
	}
	sleep(1);
}

DATA* siginPage(DATA *userlist,int *size){
	DATA *newUser = (DATA*) calloc(1,sizeof(DATA));
	newUser->name = (char*) calloc(STR_LEN,sizeof(char));
	newUser->surname = (char*) calloc(STR_LEN,sizeof(char));
	newUser->username = (char*) calloc(STR_LEN,sizeof(char));
	newUser->password = (char*) calloc(STR_LEN,sizeof(char));

	system("cls");
	printf("\n");
	printf("\t_________________________________\n");
	printf("\t|                               |\n");
	printf("\t|         SIGN IN PAGE          |\n");
	printf("\t|_______________________________|\n");
	printf("\t                                \n");
	
	printf("\t  Name: ");
	scanf("%s",newUser->name);
	printf("\t|_______________________________|\n");
	printf("\t                                \n");
	
	printf("\t  Surname: ");
	scanf("%s",newUser->surname);
	printf("\t|_______________________________|\n");
	printf("\t                                \n");
	
	do{
		printf("\t  Enter unique username :\n\t  ");
		scanf("%s",newUser->username);
	} while (searchUser(userlist,*size, newUser->username) == 1);

	printf("\t|_______________________________|\n");
	printf("\t                                \n");
	
	printf("\t Password: ");
	scanf("%s",newUser->password);
	printf("\t|_______________________________|\n");
	printf("\t                                \n");
	printf("\a\t  Registiration successful !\n");
	printf("\t  Returning to Login Menu\n");
	sleep(2);
	
	if(userlist == NULL){
		return newUser;
	}
	else{
		return addUser(userlist,size,newUser);
	}
}

void showTutorial(){
	int input;
	system("cls");
	// Print the information in the tutorial.txt file
	readTutorial("tutorial.txt");	
	
	printf("\nI got it, get back to Game Menu: ");
	scanf("%d",&input);
}

void readTutorial(char *filename){
	FILE *file=fopen(filename,"r");
	char *line = (char*) malloc(LINESIZE*sizeof(char));
	
	if(file==NULL){
		printf("Tutorial File does not exist, ERROR!");
	}

	while(fgets(line,LINESIZE,file) !=NULL){
        printf("%s",line);
    }
    
	fclose(file);
	free(line);
}

void showHighScores(char *filename){
	FILE *file = fopen(filename,"r");
	char *line = (char*) malloc(LINESIZE*sizeof(char));
	int i=1,input;
	
	if(file==NULL){
		printf("Tutorial File does not exist, ERROR!");
	}
	
	system("cls");
	printf("\t#Rank  #High Score List\n");
	printf("\t-----  -------------------------------------\n");
	while(fgets(line,LINESIZE-1,file) !=NULL && i<=5){
        printf("\t# %d     %s",i,line);
        i++;
    }
    printf("\nGet back to Game Menu: ");
	scanf("%d",&input);
	sleep(2);
}

void gotoGameMenu(SCORE *highscores,char *username){
	int score,gmFlag=1,gmInput; // game menu input and flag
	int R,C; // row and column of game board
	char **board; // game board
	
	while(gmFlag ==1){
		system("cls");
		printf("\n");
		printf("\t________________________\n");
		printf("\t|                      |\n");
		printf("\t|      GAME MENU       |\n");
		printf("\t|______________________|\n");
		printf("\t|                      |\n");
		printf("\t|   #1 NEW GAME        |\n");
		printf("\t|______________________|\n");
		printf("\t|                      |\n");
		printf("\t|   #2 TUTORIAL        |\n");
		printf("\t|______________________|\n");
		printf("\t|                      |\n");
		printf("\t|   #3 HIGH SCORES     |\n");
		printf("\t|______________________|\n");
		printf("\t|                      |\n");
		printf("\t|   #4 BACK TO MENU    |\n");
		printf("\t|______________________|\n\n");		
		printf("\tYour choice: ");
		scanf("%d",&gmInput);
		
		switch (gmInput)
        {
        case 1:
			board= selectMapMode(board,&R,&C);
			score= selectPlayMode(board,&R,&C);
			// score kayit
			updateHighscores(highscores,score,username);
            break;
        case 2:
            showTutorial();
            break;
        case 3:
            printHighScores(highscores);
			break;
        case 4:
            printf("\n Returning the Login Menu...\n");
            sleep(1);
            gmFlag = 0;
            break;
        
        default:
            printf("\n Invalid ID, Enter a valid ID !\n");
            sleep(2);
            break;
        }
	}
}

// MAP FUNCTIONS

// maps klasorunde hazir harita var mi onu kontrol eder.
void list_map_files(const char *folder_path, struct FileNames *fileNames) {
    DIR *dir;
    struct dirent *entry;
	int i;

    // Open the folder
    dir = opendir(folder_path);

    // Check if the folder is opened successfully
    if (dir == NULL) {
        perror("Unable to open the folder");
        return;
    }

    // Initialize the file count
    fileNames->count = 0;

    // Read files in the folder
    while ((entry = readdir(dir)) != NULL) {
        // Check the file name
        if (strstr(entry->d_name, ".txt") != NULL) {
            // Determine the file type (in this case TXT)
            struct stat file_stat;
            char file_path[256];
            snprintf(file_path, sizeof(file_path), "%s/%s", folder_path, entry->d_name);

            if (stat(file_path, &file_stat) == 0) {
                // Check the file type (S_ISREG checks if it is a regular file)
                if (S_ISREG(file_stat.st_mode)) {
                    // If the file has a txt extension, add the file name to the structure
                    snprintf(fileNames->names[fileNames->count], sizeof(fileNames->names[fileNames->count]), "%s", entry->d_name);
                    fileNames->count++;
                }
            }
        }
    }

    // Close the folder
    closedir(dir);

	// Print file names to the screen
	if(fileNames->count ==0){
		printf("\n\tThere is no game map file ! You can not play in map-select mode !\n");
		sleep(2);
	}
	else{
		printf("\n\tAvaliable Maps:\n");
		printf("-----------------------------\n");
		for (i = 0; i < fileNames->count; i++) {
        	printf("\t%d) %s\n",i+1,fileNames->names[i]);
    	}
	}
	
	
}

char** selectMapMode(char **board,int *R, int *C){
	int mmFlag = 1,mmInput; // map menu flag & map menu input
	const char *folder_path = MAP_DIRECTORY;
	struct FileNames fileNames;
	char *mapDirectory, *mapNameInput= (char*) malloc(50*sizeof(char));
	

	while(mmFlag == 1){
		system("cls");
	    printf("\n\t________________________\n");
		printf("\t|                      |\n");
		printf("\t|  MAP SELECTING MENU  |\n");
		printf("\t|______________________|\n");
		printf("\t|                      |\n");
		printf("\t|   #1 AVAILABLE MAPS  |\n");
		printf("\t|______________________|\n");
		printf("\t|                      |\n");
		printf("\t|   #2 QUIT            |\n");
		printf("\t|______________________|\n\n");	
		
		printf("\tYour choice: ");
		scanf("%d",&mmInput);
		
        switch (mmInput)
        {
        case 1:
            printf("Available maps is opening\n");

			//burada mevcut haritalarin listesi gosterilmeli
			list_map_files(folder_path,&fileNames);
			printf("\nEnter the map name like 'map1.txt' : ");
			scanf("%s",mapNameInput);
			// allocating memory for the map directory as length of sum of maps folder path length and map file names length 
			mapDirectory = (char*) malloc(( strlen(MAP_DIRECTORY)+strlen(mapNameInput) )*sizeof(char));
			// copy maps folder path into mapDirectory pointer
			strcpy(mapDirectory,MAP_DIRECTORY);
			// we are combine the map directory and map file name
			strcat(mapDirectory, mapNameInput);
			// so the mapDirectory is now ./maps/mapName.txt
			printf("Acilacak harita : %s\n",mapDirectory);
			// dosya okuyup matrise donusturur.
			board = readMapFromFile(mapDirectory,R,C);
			if(board != NULL){
				printf("\nMap successfully opened !");
				mmFlag=0;
			}
			//sleep(1);
            break;
        case 2:
            printf("\nReturning to the game menu...\n");
            sleep(1);
            mmFlag =0;
            break;
        
        default:
            printf("\n Invalid ID, Enter a valid ID!\n");
            sleep(1);
            break;
        }
	}
	return board;
}

char** readMapFromFile(char *filename,int *R,int *C){
	FILE *fp = fopen(filename,"r");
	if(fp == NULL){
		printf("File did not opening");
		return NULL;
	}
	
	int i,j,row,column;
	char **board;
	fscanf(fp,"%d",&row);
	fscanf(fp,"%d",&column);
	
	*R=row;
	*C=column;

	board= (char**) malloc(row*sizeof(char*));
	for(i=0; i<row; i++){
		board[i] = (char*) malloc(column*sizeof(char));
	}
	// indicator now at the end of the line
	for(i=0; i<row; i++){
		// Therefore for the passing next line we run fgetc() function
		fgetc(fp);
		for(j=0; j<column; j++){
			board[i][j]= fgetc(fp);
		}
	}

	return board;
}

void printmatris(char** board, int *R,int *C){
	int i,j;
	printf("\n");
	for (i=0; i < (*R); i++){
		for (j=0; j < (*C); j++){
			printf("%c ",board[i][j]);
		}printf("\n");
	}
}

void printBoard(char** board, int *R,int *C){
    int i,j;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    for ( i=0; i < (*R); i++) {
        for ( j=0; j < (*C); j++) {
            // Renkleri ayarla
            if (board[i][j] == 'C') {
                SetConsoleTextAttribute(hConsole, 4);
            } else if (board[i][j] == 'K') {
                SetConsoleTextAttribute(hConsole, 5); 
            } else if (board[i][j] == 'E') {
                SetConsoleTextAttribute(hConsole, 3);  
            } else if (board[i][j] == 'e') {
                SetConsoleTextAttribute(hConsole, 3);  
            } else if (board[i][j] == 'P') {
                SetConsoleTextAttribute(hConsole, 14);  
            } else if (board[i][j] == 'p') {
                SetConsoleTextAttribute(hConsole, 14); 
            } else if (board[i][j] == '1') {
                SetConsoleTextAttribute(hConsole, 12);
            } else if (board[i][j] == '0') {
                SetConsoleTextAttribute(hConsole, 8);
            } else if (board[i][j] == 'X') {
                SetConsoleTextAttribute(hConsole, 2);
            } else if (board[i][j] == 'G') {
                SetConsoleTextAttribute(hConsole, 2);
            } else {
                SetConsoleTextAttribute(hConsole, 7);
            }

            // matrisi yazdir
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }

    // renkleri sifirlama
    SetConsoleTextAttribute(hConsole, 7);
}

// PLAYING FUNCTIONS

int selectPlayMode(char **board,int *R, int *C){
	int score=0,input,pmFlag = 1,pmInput; // play menu flag & play menu input

	while(pmFlag == 1){
		system("cls");
	    printf("\n\t________________________\n");
		printf("\t|                      |\n");
		printf("\t|    PLAY MODE MENU    |\n");
		printf("\t|______________________|\n");
		printf("\t|                      |\n");
		printf("\t|   #1 USER-GAMEPLAY   |\n");
		printf("\t|______________________|\n");
		printf("\t|                      |\n");
		printf("\t|   #2 AUTO-GAMEPLAY   |\n");
		printf("\t|______________________|\n");
		printf("\t|                      |\n");
		printf("\t|   #3 QUIT            |\n");
		printf("\t|______________________|\n\n");	
		
		printf("\tYour choice: ");
		scanf("%d",&pmInput);
		
        switch (pmInput){
			case 1:
				printf("User gameplay mode\n");
				score=userGameplay(board,R,C);
				printf("Back to Game Menu, type any number: ");
				scanf("%d",&input);
				sleep(1);
				pmFlag=0;
				break;
			case 2:
				printf("Auto gameplay mode\n");
				sleep(2);
				break;
			case 3:
				printf("\nReturning to the game menu...\n");
				sleep(2);
				pmFlag =0;
				break;
			
			default:
				printf("\n Invalid ID, Enter a valid ID!\n");
				sleep(2);
				break;
        }
	}
	return score;
}

int userGameplay(char** board,int *R, int *C){
	// score return eder
	int i,j,flag=1,score=0,player_r,player_c; // flag for the game loop, player score, player_row , player_column
	// according to player move, player coordinates change (+1 or -1 or 0 not change)
	int move_r, move_c, start_r, start_c, finish_r, finish_c;  
	char moveInput;
	char *particles = (char*) calloc(1,sizeof(char));
	double elapsedTime;
	// detect starting point and ending point
	for(i=0; i< (*R); i++){
		for(j=0; j< (*C); j++){
			if(board[i][j]== 'G'){
				player_r = i;
				player_c= j;
				start_r = i;
				start_c = j;
			}
			else if(board[i][j]=='C'){
				finish_r = i;
				finish_c = j;
			}
		}
	}

	// game starts
	time_t startTime = time(NULL);
	while(flag ==1){
		printBoard(board,R,C);
		printf("\n");
		printParticles(particles);
		
		moveInput = getch();
		
		if(moveInput == -32){
			moveInput = getch();
			switch (moveInput){
				case 72:
					/* UP */
					move_r = -1;
					move_c = 0; 
					break;
				case 80:
					/* DOWN */
					move_r = 1;
					move_c = 0; 
					break;
				case 77:
					/* RIGHT */
					move_r = 0;
					move_c = 1;
					break;
				case 75:
					/* LEFT */
					move_r = 0;
					move_c = -1; 
					break;
				case 27:
					/* ESC */
					printf("\nExiting the game !\n");
					return 0;
					break;
				default:
					break;
			}
		}
	

		// if there is no obstacle (walls or board edges) then move
		if( (player_r + move_r) >=0 && (player_r + move_r) < (*R) && (player_c + move_c) >=0 && (player_c + move_c) < (*C) && board[player_r + move_r][player_c + move_c] !='1'){
			// passed ways turn into '0' (road) 
			board[player_r][player_c] = '0';
			player_r += move_r;
			player_c += move_c;
		}
		
		// black hole control
		if(board[player_r][player_c] == 'K'){
			printf("You are lost into black hole !\n");
			printf("You got 0 point.\n");
			return 0;
		}
		// Exit point control
		else if(board[player_r][player_c] == 'C'){
			printf("\n---------------------------\nYou reached the exit !\n");
			return calculateScore(particles,elapsedTime);
		}
		// yeme islemlerini yap
		particles = addParticle(particles,board[player_r][player_c]);
		
		// gidilen lokasyonu X ile isaretle
		board[player_r][player_c] = 'X';
		board[start_r][start_c] = 'G';
		board[finish_r][finish_c] = 'C';

	
		elapsedTime = difftime( time(NULL), startTime);
		if(elapsedTime >= MAX_TIME){
			printf("\n           GAME OVER !               \nYou didn't reach to Exit on time.\n");
			return 0;
		}
	}
}

int autoGameplay(char** board,int *R, int *C){
	// yapamadım.
}

char* addParticle(char *particleList,char object){
	if(object =='C' || object == 'G' || object == '0' || object == 'X'){
		return particleList;
	}
	int len= strlen(particleList);
	char *list = realloc(particleList, ( len + 1 )*sizeof(char) );
	list[len] = object;
	return list;
}

void printParticles(char *particleList){
	int i,len = strlen(particleList);
	printf("Collected Particles:\n");
	for(i=0; i< len ;i++){
		switch (particleList[i]){
		case 'P':
			printf("P+ ");
			break;
		case 'p':
			printf("P- ");
			break;
		case 'E':
			printf("e+ ");
			break;
		case 'e':
			printf("e- ");
			break;
		default:
			break;
		}
	}printf("\n");
}

int calculateScore(char* particles,double elapsedTime){
	
	int i,point=0, antiparticle=0, p_count=0, e_count=0; // minimum proton count & minimum electron counter
	int *counter = (int*) calloc(4,sizeof(int));
	int len = strlen(particles);
	
	for(i=0; i< len ;i++){
		switch (particles[i]){
		case 'P':
			counter[0] += 1;
			break;
		case 'p':
			counter[1] += 1;
			break;
		case 'E':
			counter[2] += 1;
			break;
		case 'e':
			counter[3] += 1;
			break;
		}
	}	
	
	// condition ? expression_if_true : expression_if_false;
	p_count = (counter[0] >= counter[1]) ? counter[1] : counter[0]; 
	e_count = (counter[2] >= counter[3]) ? counter[3] : counter[2]; 
	
	counter[0] -= p_count; 
	counter[1] -= p_count; // p-
	counter[2] -= e_count; // e+
	counter[3] -= e_count; 
	
	// p- ve e+ sayilarindan min olani alir
	antiparticle = (counter[1] >= counter[2]) ? counter[2] : counter[1]; 

	point = (antiparticle*250) + (int)( (MAX_TIME - elapsedTime) *10);

	printf("%d anti particles were created.\n",antiparticle);
	printf("It took %.1lf second\n",elapsedTime);
	printf("You got %d point.\n",point);
	return point;
}

void updateHighscores(SCORE *highscores,int score,char *username){
	int i=0, index;
	while(i<HIGHSCORESIZE && highscores[i].point > score){
		i++;
	}
	index = i;

	for(i=HIGHSCORESIZE-1; i >= index; i--){
		highscores[i+1].point = highscores[i].point;
		strcpy(highscores[i+1].username, highscores[i].username);
	}
	highscores[index].point = score;
	strcpy(highscores[index].username, username);
}

void writeScoreList(char *filename,SCORE *highscores){
	FILE *fp = fopen(filename, "w");

    if(fp ==NULL){
		printf("File Opening Error !\n");
		exit(0);
	}
	int i;
	for(i=0; i< HIGHSCORESIZE; i++){
		fprintf(fp, "%s %d\n",highscores[i].username,highscores[i].point);
	}
    fclose(fp);
}
