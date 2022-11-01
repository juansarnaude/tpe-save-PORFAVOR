#include <_play.h>
#include <userstdlib.h>


static uint8_t * currentPlayer1Pos = (uint8_t*)0xB8000+1960; // cambiarlo
static uint8_t * currentPlayer2Pos = (uint8_t*)0xB8000+2040; //cambiarlo
static uint32_t playersPaintedPixels[AREA] = {0};//Array que nos sirve para guardar el recorrido de los jugadores

/**
 *         0 if no player crashed
 *         1 if Colision was caused by player 1
 *         2 if Colision was caused by player 2
 *         3 if both crashed
 */
static uint32_t crash = 0;
static uint32_t collisionState1= 0;
char lastLetter1 = 'd';
char lastLetter2 = 'j';
//80 * 12 = 960
//960 * 2 = 1920
//20 * 2 = 40
//1920 + 40 = 1960 -> primera pos player1
//1920 + 120 = 2040 -> primera pos player2

void play(unsigned int fd){
    char letter[1] = {0};
    int flag=1;
    while(1){
        letter [0]=read_char();
        if(letter[0] <= 0){
            drawMovement(lastLetter1,1);
            drawMovement(lastLetter2,2);
        }
        if(letter[0] > 0){
            if( letter[0]=='w' || letter[0]=='a' || letter[0]=='s' || letter[0]=='d'){
                drawMovement(letter[0],1);
                drawMovement(lastLetter2,2);
                lastLetter1 = letter[0];
            }
            else if( letter[0]=='j' || letter[0]=='k' || letter[0]=='l' || letter[0]=='i'){
                drawMovement(lastLetter1,1);
                drawMovement(letter[0],2);
                lastLetter2 = letter[0];
            }
            letter[0] = 0;

        }
        if(crash!=0){
            endGame();
            flag=0;
        }
        my_printf("hola");
        sleepMiliseconds(1);
    }
}

int endGame(){

    if(crash==1){
        char str[12] = "Gano el azul";
        sprint(STDOUT,str);
    }
    else if(crash==2){
        char str[12] = "Gano el rojo";
        sprint(STDOUT,str);
    }
    else {
        char str[6] = "Empate";
        sprint(STDOUT,str);
    }

    for( int i=0 ; i<AREA ; i++ ){
        playersPaintedPixels[i]=0;
    }
    currentPlayer1Pos = (uint8_t*)0xB8000+1960;
    currentPlayer2Pos = (uint8_t*)0xB8000+2040;
    lastLetter1 = 'd';
    lastLetter2 = 'j';
    crash = 0;
    collisionState1= 0;
    sleepMiliseconds(1000);
}

int drawMovement(char c, int player){
    int firstColumn = 0;
    int lastColumn=0;
	if(player==1) {
        if((((uint32_t) currentPlayer1Pos - 0xB8000) / 2) % (WIDTH) == 0)
            firstColumn = 1;
        if((((uint32_t) currentPlayer1Pos - 0xB8000) / 2) % (WIDTH) == 79)
            lastColumn = 1;
        if(c=='d' || c=='D'){
            currentPlayer1Pos += 2;
            paint(1,RIGHT,currentPlayer1Pos);
		}
		else if(c=='a' || c=='A'){
            currentPlayer1Pos -= 2;
            paint(1,LEFT,currentPlayer1Pos);
		}
		else if(c=='w' || c=='W'){
            currentPlayer1Pos -= 2 * WIDTH;
            paint(1,UP,currentPlayer1Pos);
		}
		else if(c=='s' || c=='S'){
            currentPlayer1Pos += 2 * WIDTH;
			paint(1,DOWN,currentPlayer1Pos);
		}
        if((((((uint32_t) currentPlayer1Pos - 0xB8000) / 2) % (WIDTH) == 79) && firstColumn == 1 )|| (((((uint32_t) currentPlayer1Pos - 0xB8000) / 2) % (WIDTH) == 0) && lastColumn == 1)){
                        crash+=1;
        }
       collisionState1=checkCollision(1);
            playersPaintedPixels[((uint32_t) currentPlayer1Pos - 0xB8000) / 2] = 1;
        }
	else{
        if((((uint32_t) currentPlayer2Pos - 0xB8000) / 2) % (WIDTH) == 0)
            firstColumn = 1;
        if((((uint32_t) currentPlayer2Pos - 0xB8000) / 2) % (WIDTH) == 79)
            lastColumn = 1;
		if(c=='l' || c=='L'){
            currentPlayer2Pos += 2;
			paint(1,RIGHT,currentPlayer2Pos);
		}
		else if(c=='j' || c=='J'){
            currentPlayer2Pos -= 2;
			paint(1,LEFT,currentPlayer2Pos);
		}
		else if(c=='i' || c=='I'){
            currentPlayer2Pos -= 2 * WIDTH;
			paint(1,UP,currentPlayer2Pos);
		}
		else if(c=='k' || c=='K'){
            currentPlayer2Pos += 2 * WIDTH;
			paint(1,DOWN,currentPlayer2Pos);
		}
        if((((((uint32_t) currentPlayer2Pos - 0xB8000) / 2) % (WIDTH) == 79) && firstColumn == 1)|| (((((uint32_t) currentPlayer2Pos - 0xB8000) / 2) % (WIDTH) == 0) && lastColumn == 1)){
            crash+=2;
        }
        checkCollision(2);
        playersPaintedPixels[((uint32_t)currentPlayer2Pos - 0xB8000)/2] = 1;
	}
    
}


int checkCollision(int player){
    if(player==1){
        if(playersPaintedPixels[((uint32_t)currentPlayer1Pos - 0xB8000)/2]==1 || AREA<((uint32_t)currentPlayer1Pos - 0xB8000)/2 || 0==WIDTH%((uint32_t)currentPlayer1Pos - 0xB8000)/(2*HEIGHT)){
            crash += 1;
            return 1;
        }
    }
    if(player==2){
        if(playersPaintedPixels[((uint32_t)currentPlayer2Pos - 0xB8000)/2]==1  || AREA<((uint32_t)currentPlayer2Pos - 0xB8000)/2|| 0==WIDTH%((uint32_t)currentPlayer2Pos - 0xB8000)/(2*HEIGHT)){
            crash += 2;
            if(!collisionState1)
                drawMovement(lastLetter1,1);
            return 2;
        }
    }
    return 0;
}