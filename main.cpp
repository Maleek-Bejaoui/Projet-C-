#include<iostream> //C’est un fichier d’en-tête de bibliothèque standard C++ pour les flux de sortie d’entrée. Il comprend des fonctionnalités pour lire et écrire à partir du flux.
#include<conio.h> // Conio est une bibliothèque qui permet de manipuler la console Windows
#include<dos.h> // Cette bibliothèque a des fonctions qui sont utilisées pour gérer les interruptions, produire du son, la date et l'heure
#include <windows.h> // contient des déclarations pour toutes les fonctions de l'API Windows, toutes les macros courantes utilisées par les programmeurs Windows et tous les types de données utilisés par les différentes fonctions et sous-systèmes.
#include <time.h> //Cette librairie fournit un ensemble de fonctions permettant la manipulation de dates, de temps et d'intervals.

#define SCREEN_WIDTH 90 // Largeur de de l’écran d’exécution
#define SCREEN_HEIGHT 26 // Hauteur de l’écran d’exécution
#define WIN_WIDTH 70    // Largeur de l’espace de déroulement du jeu
#define MENU_WIDTH 20	// Largeur de de l’écran d’exécution
#define GAP_SIZE 7	// Distance entre « tank » et le bas de l’espace du jeu
#define Enemy_DIF 45    // distance entre « enemy » et « tank »




using namespace std;
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

// declaration  des tableaux et des variables
int enemyY[3]; //table qui contient la position de l’ennemi suivant l’axe Y du console
int enemyX[3];//table qui contient la position de l’ennemi suivant l’axe X du console
int enemyFlag[3]; //  enemyFlag[0]=1 l'existance d'un enemy d'indice 0
char tank[3][5] = { ' ',' ','°',' ',' ',
					'|','°','°','°','|',
					'°','°','°','°','°' };//la forme du « tank »

int tankPos = WIN_WIDTH/2;//Position du « tank » (dans la milieu d’arène )
int score = 0;
int bullets[20][4];// les indices des balletes
int bulletsLife[20];// les indice des vivantes balletes
int bIndex = 0;

// movement  curseur dans la console x: verticalement y: horizantalement
void gotoxy(int x, int y){
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}
//defenir la taile et le visibiltée du curseur
void setcursor(bool visible, DWORD size) {
	if(size == 0)
		size = 20;

	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console,&lpCursor);
}

// Définir la couleur des lignes dans la console
void ChangeColor(int ColorCode )
{
    HANDLE x= GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(x,ColorCode);
}

// dessiner l'arane de notre jeux
void drawBorder(){

	for(int i=0; i<=SCREEN_WIDTH; i++){
		gotoxy(i,SCREEN_HEIGHT); cout<<"°";
	}

	for(int i=0; i<SCREEN_HEIGHT; i++){
		gotoxy(0,i); cout<<"°";
		gotoxy(SCREEN_WIDTH,i); cout<<"°";
	}
	for(int i=0; i<SCREEN_HEIGHT; i++){
		gotoxy(WIN_WIDTH,i); cout<<"°";
	}
}


// générer un ennemi dans l'arene
void genEnemy(int ind){
	enemyX[ind] = 3 + rand()%(WIN_WIDTH-10);
}


// draw la forme de l'enemy dans la console l'ennemi
void drawEnemy(int ind){
	if( enemyFlag[ind] == true ){
		gotoxy(enemyX[ind], enemyY[ind]);  ChangeColor(0x79); cout<<" ** ";
		gotoxy(enemyX[ind], enemyY[ind]+1); cout<<"****";
		gotoxy(enemyX[ind], enemyY[ind]+2); cout<<"****";
		gotoxy(enemyX[ind], enemyY[ind]+3); cout<<".**.";ChangeColor(0x70);

	}
}

// efaser l'ennemi
void eraseEnemy(int ind){
	if( enemyFlag[ind] == true ){
		gotoxy(enemyX[ind], enemyY[ind]); cout<<"    ";
		gotoxy(enemyX[ind], enemyY[ind]+1); cout<<"    ";
		gotoxy(enemyX[ind], enemyY[ind]+2); cout<<"    ";
		gotoxy(enemyX[ind], enemyY[ind]+3); cout<<"    ";
	}
}

// effacer l 'enemy par l'appel de « ereace enemy() » et généré un autre
void resetEnemy(int ind){
	eraseEnemy(ind);
	enemyY[ind] = 4;
	genEnemy(ind);
}

//générer une balle pour tirer sur l’ennemi
void genBullet(){
	bullets[bIndex][0] = 22;
	bullets[bIndex][1] = tankPos;
	bullets[bIndex][2] = 22;
	bullets[bIndex][3] = tankPos+4;
	bIndex++;
	if( bIndex == 20)
		bIndex = 0;
}

//déplacer la balle bers le haut dans le console
void moveBullet(){
	for(int i=0; i<20; i++){
		if( bullets[i][0] > 2 )
			bullets[i][0]--;
		else
			bullets[i][0] = 0;

		if( bullets[i][2] > 2 )
			bullets[i][2]--;
		else
			bullets[i][2] = 0;
	}
}

// dessiner deux bullets
void drawBullets(){
	for(int i=0; i<20; i++){
		if( bullets[i][0] > 1){
			gotoxy(bullets[i][1],bullets[i][0]); ChangeColor(0x74); cout<<"°";
			gotoxy(bullets[i][3],bullets[i][2]); ChangeColor(0x74);cout<<"°";ChangeColor(0x70);
		}
	}
}

//effacer les bullets
void eraseBullet(int i){
	gotoxy(bullets[i][1],bullets[i][0]); cout<<" ";
	gotoxy(bullets[i][3],bullets[i][2]); cout<<" ";
}

// effacer les bullets  l'lorsqu’elles arrive au fin du largeur de espace de jeux
void eraseBullets(){
	for(int i=0; i<20; i++){
		if( bullets[i][0] >= 1 ){
			eraseBullet(i);
		}
	}
}



// dessiner la « tank »
void drawtank(){
    ChangeColor(0x7D);
	for(int i=0; i<3; i++){
		for(int j=0; j<5; j++){
			gotoxy(j+tankPos, i+22);cout<<tank[i][j];
		}
	}
	ChangeColor(0x70);
}

// effacer la « tank »
void erasetank(){
	for(int i=0; i<3; i++){
		for(int j=0; j<5; j++){
			gotoxy(j+tankPos, i+22); cout<<" ";
		}
	}
}

// verifiez si le char est touché par l’ennemi
int collision(){
	if( enemyY[0]+4 >= 23 ){
		if( enemyX[0] + 4 - tankPos >= 0 && enemyX[0] + 4 - tankPos < 8  ){
			return 1;
		}
	}
	return 0;
}


// vérifier si l'ennemie num (0 ou 1) est abattu (autrement dit si la ballettes touche l'ennemi)
int bulletHit(){

	for(int i=0; i<20; i++){
		for(int j=0; j<4; j+=2){

            // view if enemy 0 is hited and deleted

			if( bullets[i][j] != 0 )
            {
				if( bullets[i][j] >= enemyY[0] && bullets[i][j] <= enemyY[0]+4 ){
					if( bullets[i][j+1] >= enemyX[0] && bullets[i][j+1] <= enemyX[0]+4 ){
						eraseBullet(i);
						bullets[i][j] = 0;
						resetEnemy(0);
						return 1;
					}
				}

                // view if enemy 1 is hited deleted

				if( bullets[i][j] >= enemyY[1] && bullets[i][j] <= enemyY[1]+4 ){
					if( bullets[i][j+1] >= enemyX[1] && bullets[i][j+1] <= enemyX[1]+4 ){
						eraseBullet(i);
						resetEnemy(1);
						bullets[i][j] = 0;
						return 1;
					}
				}
			}
		}
	}
	return 0;
}

// afficher "game over" dans la console lorsque nous perdons le jeu
void gameover(){
	system("cls");

	cout<<endl;
	gotoxy(49,8);cout<<"__________________________";
	gotoxy(49,9);cout<<"-------- Game Over -------";ChangeColor(0x70);
	gotoxy(49,10);cout<<"__________________________";
	gotoxy(45,16);ChangeColor(0x75);cout<<"Press any key to go back to menu.";ChangeColor(0x70);
	getch();
}

// montrer notre score mis à jour lorsque l’ennemi a tiré
void updateScore(){
	gotoxy(WIN_WIDTH + 7, 5);ChangeColor(0x74);cout<<"Score: "<<score<<endl;ChangeColor(0x70);
}

//  afficher le blocks des instruction de notre jeux
void instructions(){

	system("cls");
	gotoxy(49,8);ChangeColor(0x71);cout<<"Instructions";
	gotoxy(47,10);ChangeColor(0x70);cout<<"----------------";
	gotoxy(39,12);ChangeColor(0x7C);cout<<"Press 'spacebar' to make tank fly";
	gotoxy(39,14);ChangeColor(0x7A);cout<<"Press 'A' to move the tank lef ";
	gotoxy(39,16);ChangeColor(0x75);cout<<"Press 'Z' to make tank right";
	gotoxy(39,18);ChangeColor(0x7C);cout<<"Press 'echap' to quit the game ";
	gotoxy(27,21);ChangeColor(0x70);cout<<"Press any key to sort from the instruction and  go back to menu";ChangeColor(0x70);
	getch();
}

void about()
{
    system("cls");
    gotoxy(49,9);ChangeColor(0x71);cout<<"    About this game   ";
    gotoxy(49,10);ChangeColor(0x70);cout<<"_____________________";
	gotoxy(39,12);ChangeColor(0x75);cout<<"this game is developped by MALEK bejaoui as";
	gotoxy(39,13);ChangeColor(0x75);cout<<"a part of project framed by Mss MOUNA .";
	gotoxy(39,14);ChangeColor(0x75);cout<<"ENET'COM.";ChangeColor(0x70);
	getch();
}
/************************************************************************************/
// cette fonction contient la programme principale de notre jeux
void play()
{


	tankPos =  (WIN_WIDTH/2) -1;
	score = 0;
	enemyFlag[0] = 1;
	enemyFlag[1] = 1;
	enemyY[0] = enemyY[1] = 4;

	for(int i=0; i<20; i++){
		bullets[i][0] = bullets[i][1] = 0;
	}

	system("cls");
	drawBorder();
	genEnemy(0);
	genEnemy(1);
	updateScore();

	gotoxy(WIN_WIDTH + 5, 2);cout<<"Free Fire Game ";
	gotoxy(WIN_WIDTH + 6, 4);cout<<"----------";
	gotoxy(WIN_WIDTH + 6, 6);cout<<"----------";
	gotoxy(WIN_WIDTH + 4, 12);ChangeColor(0x7A);cout<<"Control Option ";ChangeColor(0x70);
	gotoxy(WIN_WIDTH + 7, 13);cout<<"-------- ";
	gotoxy(WIN_WIDTH + 2, 14);cout<<"   'A'  - Left";
	gotoxy(WIN_WIDTH + 2, 15);cout<<"   'Z'  - Right";
	gotoxy(WIN_WIDTH + 2, 16);cout<<"'Spacebar'- Shoot";

	gotoxy(25, 12);cout<<"Press any key to start";
	getch();
	gotoxy(25, 12);cout<<"                      ";
    int S=200;
	while(1){

        // La fonctionnalité kbhit () correspond essentiellement à la frappe au clavier. Cette fonction traite de l'appui sur le clavier
		if(kbhit()){

        // détecter le bouton enfoncé pour (déplacer ou tirer) z pour se déplacer vers la droite et A pour se déplacer vers la gauche barre d'espace pour tirer et echap pour quitter
			char ch = getch();
			if( ch=='a' || ch=='A' ){
				if( tankPos > 2 )
					tankPos -= 2;
			}
			if( ch=='z' || ch=='Z' ){
				if( tankPos < WIN_WIDTH-7 )
					tankPos += 2;
			}
			if(ch==32){
				genBullet();
			}
			if(ch==27){
				break;
			}
		}
		// dessiner l'enemy et les  bullets
		drawtank();
		drawEnemy(0);
		drawEnemy(1);
		drawBullets();


        //verifier si le tank est abattu
		if( collision() == 1  ){
			gameover();
			return;
		}

        // socore incremeter lorsque l'enemy et abattu
		if(  bulletHit() == 1 ){
			score+=2;
			updateScore();
		}

        //retarder 100 ms et effacer le tank et les Enemys et faire avancer les Bullets

		Sleep(100-score);
		erasetank();
		eraseEnemy(0);
		eraseEnemy(1);
		eraseBullets();
		moveBullet();

		// faire avancer l'enemy si n'est pas battu ou depacer l'arene du jeu
		if( enemyFlag[0] == 1 )
			enemyY[0] += 1;

		if( enemyFlag[1] == 1 )
			enemyY[1] += 1;

		if( enemyY[0] > SCREEN_HEIGHT-5 ){
			resetEnemy(0);
		}
		if( enemyY[1] > SCREEN_HEIGHT-5 ){
			resetEnemy(1);
		}
	}
}



int main()
{
    ChangeColor(0x70);
	setcursor(0,0);
	srand( (unsigned)time(NULL));


	do{
		system("cls");

		ChangeColor(0x79);
		gotoxy(42,6); cout<<" __________________________";
		gotoxy(42,7); cout<<"|       Mini Projet       |";
		gotoxy(42,8); cout<<"|     Free Fire Game      |";
		gotoxy(42,9); cout<<" __________________________";

		gotoxy(50,12);ChangeColor(0x72);cout<<"1. Start Game";
		gotoxy(50,14); ChangeColor(0x71); cout<<"2. Instructions";
		gotoxy(50,16); ChangeColor(0x75);cout<<"3. About";
		gotoxy(50,18); ChangeColor(0x74);cout<<"4. Quit";
        gotoxy(10,25);  ChangeColor(0x72); cout<<"Press 'S' to START game |";ChangeColor(0x71);cout<< "Press 'I' to see the INSTRUCTIONS";ChangeColor(0x74);cout<<"| Press 'Q' to QUIT the game";
        gotoxy(35,26); ChangeColor(0x75);cout<<"Press 'O' the see some informations about this game";
		gotoxy(40,27); ChangeColor(0x70);cout<<"this game is devoloped by Malek Bejaoui\n";ChangeColor(0x70);
		char op = getche();

		if( op=='S' || op=='s') play();
		else if( op=='I' || op=='i') instructions();
		else if( op=='Q' || op=='q') exit(0);
		else if (op=='O' || op=='o') about();

	}while(1);
     return 0;

}
