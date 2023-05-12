#ifndef WORD_SHOOTER_CPP
#define WORD_SHOOTER_CPP
#include "util.h"
#include "Board.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <fstream>
#define MAX(A,B) ((A) > (B) ? (A):(B)) // defining single line functions....
#define MIN(A,B) ((A) < (B) ? (A):(B))
#define ABS(A) ((A) < (0) ? -(A):(A))
#define FPS 10
#define KEY_ESC 27 

using namespace std;

int dictionarysize = 370099;
string * dictionary = new string[dictionarysize];

/* My variables */

auto circles_color = YELLOW;

Board *board_b = new Board(10,150);

bool regenerateAlphabets = 0;
bool regenerateShooterValue = 1;
bool shootHitCheck = 0;
bool matchStatus = 0;
bool matchWordStatus = 0;

bool gameMenu = 1;
bool startGameStatus = 0;
bool highscoresStatus = 0;
bool exitStatus = 0;
bool timeOverStatus = 0;

int width = 950, height = 840; // i have set my window size to be 950 x 840
int hitAlphaIndex_1 = -1;
int hitAlphaIndex_2 = -1;
int scores = 0;
int m_wordsCount;
int timer = 0;
int max_timer = 120;
int highscores[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

int lineXpoints = -1;
int lineYpoints = -1;

int alphabets_xaxisPoints[3][9];
int alphabets_yaxisPoints[3];

string alphabets[26] = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z",};
string alphabets_array1 [9] = {"S", "T", "U", "D", "E", "X", "Y", "H", "I"};
string alphabets_array2 [9] = {"A", "B", "C", "M", "N", "F", "G", "Q", "R"};
string alphabets_array3 [9] = {"J", "K", "L", "V", "W", "O", "P", "Z", "A"};
string temp_alphabets_array [3][9];
string shooter_val = "A";
string next_val = "A";
string h_words[3][35];
string v_words[27];
string d_words[21];
string m_word="";
string m_word1="";
string m_word2="";
string m_word4="";


/* Extra Functions Start */

/* This function stores the highscores in the file */
void saveHighScores(){  
    int tempNum;
    for (int i=0; i<10; i++){
        for (int j=i; j<10; j++){
            if (highscores[i] < highscores[j]){
                tempNum = highscores[i];
                highscores[i] = highscores[j];
                highscores[j] = tempNum;
            }
        }
    }

    fstream hsfile;
    hsfile.open ("highscores.txt", ios::out);
    for (int i=0; i<10; i++){
        hsfile<<highscores[i]<<endl;
    }
    hsfile.close();
}

// Load highscores from the file highscores.txt
void loadHighScores(){
    string tempStr;
    fstream hsfile;
    hsfile.open ("highscores.txt", ios::in);
    for (int i=0; i<10; i++){
        getline (hsfile, tempStr);
        highscores[i] = stoi(tempStr);
    }
    hsfile.close();
}

// timer function --> used for timer in the game
void Timer_clock () {
    sleep(0.0001);
    timer++;
}

// It compares the strings from the game with dictionary
bool stringEqualityChecker(string s1, string s2){

    if (s1.length() != s2.length() - 1)
        return 0;

    else {
        for (int i=0; i<s1.length(); i++){
            if (s1[i] != s2[i])
                return 0;
        }
    }
    
    return 1;    
}

// It searches the given string and return 0 or 1
bool searchWords(string str){
    char tempChar = str[0];
    int startPoint = 0;
    int endPoint = dictionarysize; 

    // for fast search it divides the array
    if (tempChar > 'm') 
        startPoint = 196390;
    else
        endPoint = 196400;    
    

    for (int i=startPoint; i<endPoint; i++){
        if ( stringEqualityChecker(str, dictionary[i]) ){
            return 1;
        }
    }

    return 0;
}

// It loads the data of words from the file into an array dictionary
bool loadWordsFromFile (){
    string str2; 
    int count = 0;
    fstream words_file;
    words_file.open("words_alpha.txt", ios::in);
    while (! words_file.eof() ){
        getline (words_file, str2);
        if (count>=0 && count < dictionarysize){
            dictionary[count] = str2;
        }

        count++;
    }
    words_file.close();
    return 0;
}

string toLowerCase (string str) {
    for (int i=0; i<str.length(); i++){
        if (str[i] >= 'A' && str[i] <= 'Z'){
            str[i]+=32;
        }
    }
    
    return str;
}

string toUpperCase (string str) {
    for (int i=0; i<str.length(); i++){
        if (str[i] >= 'a' && str[i] <= 'z'){
            str[i]-=32;
        }
    }
    
    return str;
}


/* It calculate all the possible words in the horizontal direction and sort the array on the basis on length */

void copyHorizontalWords () {
    for (int i=0; i<3; i++){
        for (int j=0; j<35; j++){
            h_words[i][j] = "";
        }
    }

    for (int i=0, temp1 = 0; i<3; i++){
        temp1=0;
        for (int j=0; j<35; ){
            int k=0, p=-1;
            for (k=8; k>temp1; k--){
                for (p=temp1; p<=k; p++){
                    h_words[i][j] += toLowerCase(temp_alphabets_array[i][p]);
                }
                j++;
            }
            temp1++;
        }
    }

    // sorting horizontal arrays length wise
    string tempStr;
    for (int i=0; i<3; i++){
        for (int j=0; j<35; j++){
            for (int k=j; k<35; k++){
                if (h_words[i][j].length() < h_words[i][k].length()){
                       tempStr = h_words[i][j];
                       h_words[i][j] = h_words[i][k];
                       h_words[i][k] = tempStr;

                }
            }
        }
    }
}

/* It calculate all the possible words in the vertical direction and sort the array on the basis on length */

void copyVerticalEntries(){
    string tempStr;
    for (int i=0, j=0; i<9; i++, j++){

        v_words[j] = toLowerCase(temp_alphabets_array[0][i]);

        v_words[j] += toLowerCase(temp_alphabets_array[1][i]);

        
        j++;
        

        v_words[j] = toLowerCase(temp_alphabets_array[1][i]);
        v_words[j] += toLowerCase(temp_alphabets_array[2][i]);

        j++;
        
        v_words[j] = toLowerCase(temp_alphabets_array[0][i]);
        v_words[j] += toLowerCase(temp_alphabets_array[1][i]);

        v_words[j] += toLowerCase(temp_alphabets_array[2][i]);


    }

    for (int i=0; i<27; i++){
        for (int j=i; j<27; j++){
            if (v_words[i].length() < v_words[j].length()){
                tempStr = v_words[i];
                v_words[i] = v_words[j];
                v_words[j] = tempStr;
            }
        }
    }
}

/* It calculate all the possible words in the diagonal direction and sort the array on the basis on length */

void copyDiagonalEntries () {
    string tempStr;
            
    for (int i=0, j=0; i<7; i++, j++){

        d_words[j] = toLowerCase(temp_alphabets_array[0][i]);
        d_words[j] += toLowerCase(temp_alphabets_array[1][i+1]);
        
        j++;
        d_words[j] = toLowerCase(temp_alphabets_array[1][i+1]);
        d_words[j] += toLowerCase(temp_alphabets_array[2][i+2]);
        
        j++;
        d_words[j] = toLowerCase(temp_alphabets_array[0][i]);
        d_words[j] += toLowerCase(temp_alphabets_array[1][i+1]);
        d_words[j] += toLowerCase(temp_alphabets_array[2][i+2]);
    }

    for (int i=0; i<21; i++){
        for (int j=i; j<21; j++){
            if (d_words[i].length() < d_words[j].length()){
                tempStr = d_words[i];
                d_words[i] = d_words[j];
                d_words[j] = tempStr;
            }
        }
    }
}

/* It search all the horizontal combinations and returns the first matched because it will be the longest match */

int searchHorizontalEntries() {
    for (int i=0; i<35; i++){
        for (int j=0; j<3; j++){
            if (searchWords(toLowerCase(h_words[j][i]))){
                m_word1 = h_words[j][i];
                matchWordStatus = 1;
                return m_word1.length();
            }
        }
    }
    
    return 0;    
}

/* It search all the vertical combinations and returns the first matched because it will be the longest match */

int searchVerticalEntries() {
    for (int i=0; i<27; i++){
        if (searchWords(v_words[i])){
            m_word2 = v_words[i];
            matchWordStatus = 1;
            return m_word2.length();
        }
    }

    return 0;
}

/* It search all the diagonal combinations and returns the first matched because it will be the longest match */

int searchDiagonalEntries () {
    for (int i=0; i<21; i++){
        if (searchWords(d_words[i])){
            m_word4 = d_words[i];
            matchWordStatus = 1;
            return m_word4.length();
        }
    }

    return 0;
}

/* It calculate all the words matched horizontally, vertically and diagonally and assign the longest word to matched string */

bool checkMatchingWords () {
    
    int vLength = searchVerticalEntries();
    int dLength = searchDiagonalEntries();
    int hLength = searchHorizontalEntries();
    
    cout<<"Horizontal Word Matched : "<<m_word1<<endl;
    cout<<"Vertical Word Matched : "<<m_word2<<endl;
    cout<<"Diagonal Word Matched : "<<m_word4<<endl;
        
    if (vLength > dLength && vLength > hLength && matchWordStatus){
        m_word = m_word2;
        return 1;
    }

    else if (dLength > hLength && matchWordStatus) {
        m_word = m_word4;
        return 1;
    }
    
    else if (matchWordStatus) {
        m_word = m_word1;
        return 1;
    }
    
    return 0;
}

// It sets the value when shooter matches with an alphabet
bool matchShooterSelecter () {
    if (shooter_val == temp_alphabets_array[hitAlphaIndex_2][hitAlphaIndex_1]){
        matchStatus = 1;
        return 1;
    }
    
    return 0;    
}

// It checks shooter position with all alphabets and return 1 if matches
bool checkShooterPoints (int x, int y){
    y = 840 - y;    // For matching with the values of alphabets else pixels do not match 
    bool check2 = 0;
    bool check3 = 0;

    if ( (y > alphabets_yaxisPoints[0] ) && y < 705 ) {
        check2 = 1;
        hitAlphaIndex_2 = 0;
    }
    
    else if ( (y < 620) && (y > alphabets_yaxisPoints[1] ) ) {
        check2 = 1;
        hitAlphaIndex_2 = 1;
    }

    else if ( (y < 530) && (y > alphabets_yaxisPoints[2] ) ) {
        check2 = 1;
        hitAlphaIndex_2 = 2;
    }

    if (check2 == 0){
        cout<<"Y-Axis Not Matched"<<endl;
    }

    if (check2){
        int i;
        if (hitAlphaIndex_2 >= 0 && hitAlphaIndex_2 < 3)
            i = hitAlphaIndex_2;
        else 
            i = 0;

        for (int j=0; j<9; j++){
            if ( (x > (alphabets_xaxisPoints[i][j]-40)) && (x < (alphabets_xaxisPoints[i][j]+40))){
                check3 = 1;    
                hitAlphaIndex_1 = j;
                break;
            }
        }  
    }

    return check3;
}

// It generates random indexes and place the value from alphabets array to the new array
void randomGenerateAlphabets () {
    for (int i=0; i<3; i++){
        for (int j=0; j<9; j++){
            int randomChoice = rand()%3;
            if (randomChoice == 0)
                temp_alphabets_array[i][j] = alphabets_array1[rand()%9];
            else if (randomChoice == 1)
                temp_alphabets_array[i][j] = alphabets_array2[rand()%9];
            else 
                temp_alphabets_array[i][j] = alphabets_array3[rand()%9];
    
        }    
    }

}

// It displays either the words is matched or not
void displayMatchedWord () {
    if (matchWordStatus){
        DrawRectangle(300, 705, 380, 80, colors[LEMON_CHIFFON]); 
        DrawString(330, 735, "WORD MATCHED : "+toUpperCase(m_word), colors[BLACK]);  
        matchWordStatus = 0;
        cout<<"\nMatched String : "<<m_word<<endl<<endl;
    }

    else {
        DrawRectangle(300, 705, 380, 80, colors[SKY_BLUE]); 
        DrawString(330, 735, "NO WORD MATCH !!!!!!!", colors[WHITE]);  
    }
}

// display timer and scores
void displayTimerAndScore () {
    DrawString(15, 735, "SCORE = "+to_string(scores), colors[BLACK]);    
    DrawString(800, 735, "TIMER | "+to_string(timer), colors[BLACK]);    
}

// draw line from shooter to anywhere in the board
void drawLine () {
    if ((lineXpoints >= 8 && lineXpoints < 935) && (lineYpoints >= 350 && lineYpoints <= 700)){
        DrawLine( 470, 230, lineXpoints, lineYpoints, 50, colors[PINK]);
        DrawCircle(lineXpoints,lineYpoints,30,colors[WHITE]);
        DrawString(lineXpoints-10,lineYpoints,shooter_val,colors[RED]);
    }
}

// draw shooter and co-shooter
void drawShooter () {

    DrawCircle(470,200,40,colors[PINK]);
    DrawString(462,190,shooter_val,colors[RED]);

    DrawCircle(570,200,40,colors[BLUE]);
    DrawString(562,190,next_val,colors[YELLOW]);

}

// It draw/displays all the alphabets on the board
void drawAlphabets () {
    auto colorRG = RED; // If mismatch then red color
    if (matchStatus == 1){
        colorRG = GREEN;    // if correct match then green color
    }

    for (int i=0, temp=0; i<9; i++, temp+=100){
    
        if (hitAlphaIndex_2 == 0 && hitAlphaIndex_1 == i){  // changes color
            DrawCircle(70+temp,640,40,colors[colorRG]);
            DrawString(60+temp, 630, temp_alphabets_array[0][i], colors[BLUE]);
        }

        else {        
            DrawCircle(70+temp,640,40,colors[circles_color]);
            DrawString(60+temp, 630, temp_alphabets_array[0][i], colors[BLUE]);
        }

        alphabets_xaxisPoints[0][i] = 70+temp;
    }

    alphabets_yaxisPoints[0] = 640;

    for (int i=0, temp=0; i<9; i++, temp+=100){

        if (hitAlphaIndex_2 == 1 && hitAlphaIndex_1 == i){
            DrawCircle(70+temp,540,40,colors[colorRG]);
            DrawString(60+temp, 530, temp_alphabets_array[1][i], colors[BLUE]);
        }

        else {
            DrawCircle(70+temp,540,40,colors[circles_color]);
            DrawString(60+temp, 530, temp_alphabets_array[1][i], colors[BLUE]);
        }

        alphabets_xaxisPoints[1][i] = 70+temp;
    }

    alphabets_yaxisPoints[1] = 550;

    for (int i=0, temp=0; i<9; i++, temp+=100){

        if (hitAlphaIndex_2 == 2 && hitAlphaIndex_1 == i){
            DrawCircle(70+temp,440,40,colors[colorRG]);
            DrawString(60+temp, 430, temp_alphabets_array[2][i], colors[BLUE]);
        }
        
        else {
            DrawCircle(70+temp,440,40,colors[circles_color]);
            DrawString(60+temp, 430, temp_alphabets_array[2][i], colors[BLUE]);
        }
        
        alphabets_xaxisPoints[2][i] = 70+temp;
    }
    
    alphabets_yaxisPoints[2] = 470;
}

void displayExit(){
    saveHighScores();
    sleep(2);
    exit(0);
}

// Game Over Menu 
void displayGameOver(){
    DrawRectangle(0, 0, 950, 840, colors[BISQUE]);
    DrawLine( 200, 0, 200, 840, 50, colors[BLACK]);
    DrawLine( 205, 0, 205, 840, 50, colors[BLACK]);
    DrawString(400, 550, "GAME OVER", colors[BLUE]);
    DrawString(400, 500, "YOUR SCORE : "+to_string(scores), colors[BLUE]);
    DrawString(400, 450, "PRESS SPACE BUTTON", colors[BLUE]);
}

// Highscores menu 
void displayHighScores(){
    DrawRectangle(0, 0, 950, 840, colors[STEEL_BLUE]);
    DrawRectangle(280, 50, 370, 700, colors[PALE_TURQUOISE]);
    DrawString(380, 710, "HIGHSCORES", colors[ROSY_BROWN]);    
    
    for (int i=0, temp=0; i<10; i++, temp-=50){
        DrawString(450, 650+temp, to_string(highscores[i]), colors[BLACK]);    
    }

    DrawString(335, 80, "PRESS SPACE BUTTON", colors[BLACK]);
}

// It displays the menu of the game
void displayMenu () {
    DrawRectangle(0, 0, 950, 840, colors[PAPAYA_WHIP]);
    DrawLine( 200, 0, 200, 840, 50, colors[BLACK]);
    DrawString(400, 550, "1 - START GAME", colors[BLUE]);
    DrawString(400, 500, "2 - HIGH SCORE", colors[BLUE]);
    DrawString(400, 450, "3 - EXIT", colors[BLUE]);
}

/* Extra Functions End */

/* Function sets canvas size (drawing area) in pixels...
 *  that is what dimensions (x and y) your game will have
 *  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
 * */

void SetCanvasSize(int width, int height) {
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
}

void GameDisplay()/**/{
	// set the background color using function glClearColor.
	// to change the background play with the red, green and blue values below.
	// Note that r, g and b values must be in the range [0,1] where 0 means dim rid and 1 means pure red and so on.

	glClearColor(0.5/*Red Component*/, 0.5,//148.0/255/*Green Component*/,
			0.5/*Blue Component*/, 0 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
	glClear (GL_COLOR_BUFFER_BIT); //Update the colors

    if (startGameStatus == 0 && gameMenu == 1){ //it displays the main menu
	 scores = 0;
        timer = 0;    
        displayMenu();
        DrawString(300, 810, "WELCOME TO WORD SHOOTER", colors[BLACK]);    
    }

    if (highscoresStatus){  // display highscores on pressing 2 
        displayHighScores();
    }    

    if (exitStatus){    // save and exit from the game
        displayExit();
    }

    if (startGameStatus) {  // playgame
        board_b->Draw();    // drawboard using board.cpp
        displayTimerAndScore();
        drawAlphabets();
        drawShooter();
        drawLine();
        displayMatchedWord();
        DrawString(350, 810, "WORD SHOOTER", colors[BLACK]);    
        DrawString(350, 100, "Press R to Reload SHOOTER", colors[WHITE]);    
    }
    
	if (timeOverStatus){
        if (scores > highscores[9])
            highscores[9] = scores;

	    displayGameOver();
	}	
	
	glutSwapBuffers(); // do not modify this line.. or draw anything below this line
}

/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
 * is pressed from the keyboard
 *
 * You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...
 *
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 *
 * */

void NonPrintableKeys(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/) {
	
	            
		// what to do when left key is pressed...

	} else if (key	== GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/) {

	} else if (key	== GLUT_KEY_UP/*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/) {

	}

	else if (key == GLUT_KEY_DOWN/*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/) {
	}

	/* This function calls the Display function to redo the drawing. Whenever you need to redraw just call
	 * this function*/

	glutPostRedisplay();

}

/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
 * is pressed from the keyboard
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 * */
void PrintableKeys(unsigned char key, int x, int y) {
	if (key == 27/* Escape key ASCII*/) {
		exit(1); // exit the program when escape key is pressed.
	}

    if (key == 'R' || key == 'r'){
        regenerateShooterValue = 1;
    }

    if (key == '1'){    // gameplay
        startGameStatus = 1;
    }

    if (key == '2'){    // highscores
        if(!startGameStatus && gameMenu == 1){
            gameMenu = 0;
            highscoresStatus = 1;            
        }

    }

    // returns to main menu and save scores to file from highscores and gameover menu 
    if (key == 32 && (highscoresStatus || timeOverStatus)){ 
        saveHighScores();
        gameMenu = 1;
        highscoresStatus = 0;    
        timeOverStatus = 0;
    }

    if (key == '3' && !highscoresStatus && gameMenu && !startGameStatus){   // Exit Key
        exitStatus = 1;
    }

	glutPostRedisplay();
}

/*
 * This function is called after every 1000.0/FPS milliseconds
 * (FPS is defined on in the beginning).
 * You can use this function to animate objects and control the
 * speed of different moving objects by varying the constant FPS.
 *
 * */
void Timer(int m) {
	// implement your functionality here

    if (matchStatus == 1){
        matchStatus = 0;
    }
    
    if (timer >= max_timer){
        timeOverStatus = 1;
        startGameStatus = 0;
        gameMenu = 0;
        timer = 0;
    }
    
    if (startGameStatus){
        Timer_clock();
    }
    
    if (regenerateAlphabets == 1){
        //sleep(1);
        randomGenerateAlphabets();    
        regenerateAlphabets = 0;
        hitAlphaIndex_1 = -1;
        hitAlphaIndex_2 = -1;
        copyHorizontalWords();
        copyVerticalEntries();
        copyDiagonalEntries();
        
        int clr = rand()%11;
        if (clr == 0)
           circles_color = ORANGE;
        else if (clr == 1)
            circles_color = YELLOW;
        else if (clr == 2)
            circles_color = PINK;
        else if (clr == 3)
            circles_color = DIM_GRAY;
        else if (clr == 4)
            circles_color = BURLY_WOOD;
        else if (clr == 5)
            circles_color = HOT_PINK;
        else if (clr == 6)
            circles_color = THISTLE;
        else if (clr == 7)
            circles_color = CRIMSON;
        else if (clr == 8)
            circles_color = GOLD;
        else if (clr == 9)
            circles_color = LIME;
        else if (clr == 10)
            circles_color = GREEN;
        else 
            circles_color = GREEN;          
        
    }           

    if (regenerateShooterValue == 1){
        shooter_val = next_val;
        next_val = alphabets[rand()%26];
        regenerateShooterValue = 0;
    }

	// once again we tell the library to call our Timer function after next 1000/FPS
	glutTimerFunc(2000.0, Timer, 0);
	glutPostRedisplay();
}

/*This function is called (automatically) whenever your mouse moves witin inside the game window
 *
 * You will have to add the necessary code here for finding the direction of shooting
 *
 * This function has two arguments: x & y that tells the coordinate of current position of move mouse
 *
 * */
void MousePressedAndMoved(int x, int y) {
	//cout <<"x = " << x <<" y = "<<y<< " y2 = " << 840-y << endl;
	glutPostRedisplay();
}
void MouseMoved(int x, int y) {
    lineXpoints = x;
    lineYpoints = (840 - y) - 60;
	glutPostRedisplay();
}


/*This function is called (automatically) whenever your mouse button is clicked witin inside the game window
 *
 * You will have to add the necessary code here for shooting, etc.
 *
 * This function has four arguments: button (Left, Middle or Right), state (button is pressed or released),
 * x & y that tells the coordinate of current position of move mouse
 *
 * */
void MouseClicked(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON) // dealing only with left button
	{
	    if (state == GLUT_DOWN){
	        bool check1 = checkShooterPoints (x, y);
 	        if (check1){
                bool check4 = matchShooterSelecter ();
                if (check4){
                    scores++;
                    regenerateShooterValue = 1;
                    regenerateAlphabets = 1;
                    bool check5 = checkMatchingWords();
                    if (check5 && matchWordStatus){
                        scores+=1;
                    }
                }
 	        }
 	        
	    }
	} 
	
	else if (button == GLUT_RIGHT_BUTTON) // dealing with right button
	{
//	    cout<<x<<" "<<y<<" "<<840-y<<endl;
	
	}

	glutPostRedisplay();
}

/*
 * our gateway main function
 * */
int main(int argc, char*argv[]) {
	srand(time(NULL));
	loadHighScores();	
    loadWordsFromFile();
           
    randomGenerateAlphabets();               
//    copyHorizontalWords();
//    copyVerticalEntries();
//    copyDiagonalEntries();

    if (checkMatchingWords() && matchWordStatus){
        scores+=1;
        randomGenerateAlphabets();               
        copyHorizontalWords();
        copyVerticalEntries();
        copyDiagonalEntries();
    }

	InitRandomizer(); // seed the random number generator...
	glutInit(&argc, argv); // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 50); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("Word Shooter"); // set the title of our game window
	SetCanvasSize(width, height); // set the number of pixels...

	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.
	//glutDisplayFunc(display); // tell library which function to call for drawing Canvas.

	glutDisplayFunc(GameDisplay); // tell library which function to call for drawing Canvas.
	
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters


	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
	
	
	// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
	glutTimerFunc(100.0, Timer, 0);
               
    glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved); // Mouse
	glutMotionFunc(MousePressedAndMoved); // Mouse

	// now handle the control to library and it will call our registered functions when
	// it deems necessary...
	glutMainLoop();
	return 1;
}
#endif /* */
