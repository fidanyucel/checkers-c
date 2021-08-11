#include <stdio.h>
#include <string.h>
#include <math.h>


//#include "board.h"
//#include "movevalidator.h"
//#include "game.h"
//#include "input.h"

enum Turn{
	PROBLEM = -1,
	WHITE = 0,
	BLACK = 1,
}typedef Turn;


enum Error{
	//chosen piece errors
	WRONG_COORDINATE = -6,
	RIVAL_PIECE = -5,
	//movement errors
	SAME_PLACE_MOVEMENT = -4,
	WRONG_PLACE_MOVEMENT = -3,
	RIVAL_PIECE_MOVEMENT = -2,
	//input error
	WRONG_INPUT = -1,
	NO_PROBLEM = 0,
}typedef Error;



void gamePlay(char board[8][8], Turn turn);
void moveAndUpdateFile(char board[8][8], Turn *turn, int rowSrc, int colSrc, int rowDest, int colDest);
void registerMove(Turn turn, int rowSrc, int colSrc, int rowDest, int colDest);
void clearRegister(char* path);
void changeTurn(Turn *turn);
void generateChecker(char board[8][8]);
void askForEntry(Turn turn);
Error checkInput(char input[30]);
void splitInput(char input[30], int *rowSource, int *columnSource, int *rowDestination, int *columnDestination);
void askInput(char board[8][8], Turn *turn, char input[30], int *rowSource, int *columnSource, int *rowDestination, int *columnDestination);
Error isSourcePositionConvenient(char board[8][8], Turn turn, int srcRow, int destColumn);
Error isDestPositionConvenient(char board[8][8], Turn turn, int destRow, int destColumn);
Error isMovementConvenient(char board[8][8], Turn *turn, int srcRow, int srcCol, int destRow, int destCol);
Error isThisJump(char board[8][8], Turn *turn, int rowSrc, int colSrc, int rowDest, int colDest);
int isThereYourOwnPieceAtThisCol(char board[8][8], Turn turn, int srcRow, int destRow, int constCol);
int isThereYourOwnPieceAtThisRow(char board[8][8], Turn turn, int srcCol, int destCol, int constRow);
int isThereMoreThanOneRivalPieceAtThisCol(char board[8][8], Turn turn, int srcRow, int destRow, int constCol);
int isThereMoreThanOneRivalPieceAtThisRow(char board[8][8], Turn turn, int srcCol, int destCol, int constRow);
void deleteTheRivalAtThisRow(char board[8][8], int srcCol, int destCol, int constRow);
void deleteTheRivalAtThisCol(char board[8][8], int srcRow, int destRow, int constCol);
Error checkerPieceMovement(char board[8][8], Turn turn, int rowSrc, int colSrc, int rowDest, int colDest);
const char* getErrorErrorMessage(Error e);
void initBoard(char board[8][8]);
void printBoard(char board[8][8]);
void initFileFromScratch(const char* pathText);
void fetchBoardFromFile(char board[8][8], Turn *turn);
void postBoardToFile(char board[8][8], Turn turn);
void closeAllKeys(int *keyForWhitespace, int *keyForBlack, int *keyForWhite);
void menu();






int main()
{
	menu();
}




void menu()
{
	char board[8][8];
	initBoard(board);
	char input[30];
	Turn turn;

	int isGameContinue = 1;
	while(isGameContinue)
	{
		printf("%s\n", "1. New game");
		printf("%s\n", "2. Continue game");
		printf("%s\n", "3. End game");
		printf("%s", "Please choose an option: ");
		int option;
		scanf("%d", &option);
		fflush(stdin);
		printf("\n");

		switch(option)
		{
			case 1:
				initFileFromScratch("gameboard.txt");
				clearRegister("register.txt");
				fetchBoardFromFile(board, &turn);
				gamePlay(board, turn);
				break;
			case 2:
				fetchBoardFromFile(board, &turn);         
				gamePlay(board, turn);
				break;
			case 3:
				isGameContinue = 0;
				break;
			default:
				printf("%s\n", "An error occured!");
				break;
		}

	}

}

void gamePlay(char board[8][8], Turn turn)
{
	int isContinuePlaying = 1;
	Error error;
	char input[30];
	int rowSrc, columnSrc, rowDest, columnDest;
	while(isContinuePlaying)
	{
		 printBoard(board);
		 askInput(board, &turn, input, &rowSrc, &columnSrc, &rowDest, &columnDest);
		 if(strcmp(input, "back") == 0) break;
		 moveAndUpdateFile(board, &turn, rowSrc, columnSrc, rowDest, columnDest);
		 registerMove(turn, rowSrc, columnSrc, rowDest, columnDest);
	}
}


void moveAndUpdateFile(char board[8][8], Turn *turn, int rowSrc, int colSrc, int rowDest, int colDest)
{

	board[rowDest][colDest] = board[rowSrc][colSrc];
	board[rowSrc][colSrc] = 0;

	
	
	changeTurn(turn);
	generateChecker(board);
	postBoardToFile(board, *turn);
}

void registerMove(Turn turn, int rowSrc, int colSrc, int rowDest, int colDest)
{
	FILE* fp = fopen("register.txt", "a");
	const char* turnString;
	if(turn == WHITE) turnString = "Black";
	else turnString = "White";

	int oldRow = 8 - rowSrc;
	int newRow = 8 - rowDest;

	char oldCol = colSrc + 65;
	char newCol = colDest + 65;

	fprintf(fp, "%s: oldPosition: (%d, %c) / newPosition: (%d, %c)  => Format(row, column)\n", turnString, oldRow, oldCol, newRow, newCol);
	fclose(fp);
}

void changeTurn(Turn *turn)
{
	if(*turn == BLACK) *turn = WHITE;
	else *turn = BLACK;
}


void generateChecker(char board[8][8])
{
	//We'll check first and last row of the checker board. [0] and [7]

	for (int i = 0; i < 8; ++i)
	{
		if(board[0][i] == 'b') board[0][i] = 'B';
	}
	for (int i = 0; i < 8; ++i)
	{
		if(board[7][i] == 'w') board[7][i] = 'W';
	}
}

void clearRegister(char* path)
{
	FILE* fp = fopen(path, "w");
	fclose(fp);
}

void askForEntry(Turn turn)
{
	switch(turn)
	{	
		case BLACK:
			printf("%s", "Team black please enter your command (\"back\" in order to go back): ");
			break;
		case WHITE:
			printf("%s", "Team white please enter your command (\"back\" in order to go back): ");
			break;
		default:
			printf("%s\n", "An error occured");
			break;
	}
}

Error checkInput(char input[30])
{
	int input0 = input[0];
	int input1 = input[1] - '0';
	int input2 = input[2]; 	
	int input3 = input[3] - '0';


	if(strlen(input) != 4) return WRONG_INPUT;
	else if(input0 > 104 || input0 < 97 || input2 > 104 || input2 < 97) return WRONG_INPUT;
	else if(input1 > 8 || input1  < 1 || input3 > 8 || input3  < 1) return WRONG_INPUT;
	else return NO_PROBLEM;
}

void splitInput(char input[30], int *rowSource, int *columnSource, int *rowDestination, int *columnDestination)
{
	*columnSource = (int)input[0] - 97;
	*columnDestination = (int)input[2] - 97;
	*rowSource = 8 - (input[1] - '0');
	*rowDestination = 8 - (input[3] - '0');
}

void askInput(char board[8][8] ,Turn *turn, char input[30], int *rowSrc, int *colSrc, int *rowDest, int *colDest)
{
	Error err;

	while(1){
		askForEntry(*turn);
		char ch;
		int i = 0;
		for(;;) {
	        ch = fgetc(stdin);
	        if(ch == '\n')
	            break;
	        input[i++] = ch;
	    }
	    input[i] = '\0';
	    if(strcmp(input, "back") == 0) break;
	    if(err = checkInput(input) < 0)
	    {
	    	printf("%s\n", getErrorErrorMessage(err));
	    	continue;
	    }
	    else	splitInput(input, rowSrc, colSrc, rowDest, colDest);
	    
	    if((err = isMovementConvenient(board, turn, *rowSrc, *colSrc, *rowDest, *colDest)) < 0)
	    {
	    	printf("%s\n", getErrorErrorMessage(err));
	    	continue;
	    }
	    else
	    {
	    	break;
	    }
	}
}
Error isSourcePositionConvenient(char board[8][8], Turn turn, int row, int column)
{
	if(board[row][column] == 0) return WRONG_COORDINATE;
	else if((board[row][column] == 'w' || board[row][column] == 'W') && turn == BLACK) return RIVAL_PIECE;
	else if((board[row][column] == 'b' || board[row][column] == 'B') && turn == WHITE) return RIVAL_PIECE;
	else return NO_PROBLEM;
}

Error isDestPositionConvenient(char board[8][8], Turn turn, int row, int col)
{
	if((board[row][col] ==  'b' || board[row][col] == 'B') && turn == WHITE) return RIVAL_PIECE_MOVEMENT;
	else if((board[row][col] ==  'w' || board[row][col] == 'W') && turn == BLACK) return RIVAL_PIECE_MOVEMENT;
	else if(board[row][col] == 0) return NO_PROBLEM;
	else return WRONG_PLACE_MOVEMENT;
}


Error isMovementConvenient(char board[8][8], Turn *turn, int srcRow, int srcCol, int destRow, int destCol)
{
	Error er;
	if((er = isSourcePositionConvenient(board, *turn, srcRow, srcCol)) < 0) return er;
	else if((er = isDestPositionConvenient(board, *turn, destRow, destCol)) < 0) return er;

	if(board[srcRow][srcCol] == 'b' || board[srcRow][srcCol] == 'w')
	{
		int rowMovement = abs(srcRow - destRow);
		int colMovement = abs(srcCol - destCol);
		int totalMovement = rowMovement + colMovement;

		if(totalMovement == 1) return NO_PROBLEM;
		else if(totalMovement == 2) {
			return isThisJump(board, turn, srcRow, srcCol, destRow, destCol);
		}
		else return WRONG_PLACE_MOVEMENT;
	}
	else	// 'W' or 'B'
	{
		return checkerPieceMovement(board, *turn, srcRow, srcCol, destRow, destCol);
	}
}

int isThereYourOwnPieceAtThisCol(char board[8][8], Turn turn, int srcRow, int destRow, int constCol)
{
	int isThere = 0;
	int big, small;
	if(srcRow > destRow){
		big = srcRow;
		small = destRow;
	}
	else
	{
		big = destRow;
		small = srcRow;
	} 
	for (int i = small+1; i < big; ++i)
	{
		if((board[i][constCol] == 'b' || board[i][constCol] == 'B') && turn == BLACK)	isThere = 1;
		if((board[i][constCol] == 'w' || board[i][constCol] == 'W') && turn == WHITE)	isThere = 1;
	}
	return isThere;
}

int isThereYourOwnPieceAtThisRow(char board[8][8], Turn turn, int srcCol, int destCol, int constRow)
{
	int isThere = 0;
	int big, small;
	if(srcCol > destCol){
		big = srcCol;
		small = destCol;
	}
	else
	{
		big = destCol;
		small = srcCol;
	} 
	for (int i = small+1; i < big; ++i)
	{
		if((board[constRow][i] == 'b' || board[constRow][i] == 'B') && turn == BLACK)	isThere = 1;
		if((board[constRow][i] == 'w' || board[constRow][i] == 'W') && turn == WHITE)	isThere = 1;
	}
	return isThere;
}

int isThereMoreThanOneRivalPieceAtThisRow(char board[8][8], Turn turn, int srcCol, int destCol, int constRow)
{
	int howMany = 0;
	int big, small;
	if(srcCol > destCol){
		big = srcCol;
		small = destCol;
	}
	else
	{
		big = destCol;
		small = srcCol;
	}
	for (int i = small+1; i < big; ++i)
	{
		if((board[constRow][i] == 'b' || board[constRow][i] == 'B') && turn == WHITE)	howMany++;
		if((board[constRow][i] == 'w' || board[constRow][i] == 'W') && turn == BLACK)	howMany++;
	}
	if(howMany >= 2) return 1;
	else return 0; 
}

int isThereMoreThanOneRivalPieceAtThisCol(char board[8][8], Turn turn, int srcRow, int destRow, int constCol)
{
	int howMany = 0;
	int big, small;
	if(srcRow > destRow){
		big = srcRow;
		small = destRow;
	}
	else
	{
		big = destRow;
		small = srcRow;
	}
	for (int i = small+1; i < big; ++i)
	{
		if((board[i][constCol] == 'b' || board[i][constCol] == 'B') && turn == WHITE)	howMany++;
		if((board[i][constCol] == 'w' || board[i][constCol] == 'W') && turn == BLACK)	howMany++;
	}
	if(howMany >= 2) return 1;
	else return 0; 
}

void deleteTheRivalAtThisRow(char board[8][8], int srcCol, int destCol, int constRow)
{
	int big, small;
	if(srcCol > destCol){
		big = srcCol;
		small = destCol;
	}
	else
	{
		big = destCol;
		small = srcCol;
	}
	for (int i = small+1; i < big; ++i)
	{
		if(board[constRow][i] == 'w' || board[constRow][i] == 'W' || board[constRow][i] == 'b' || board[constRow][i] == 'B')
			board[constRow][i] = 0;
	}
}

void deleteTheRivalAtThisCol(char board[8][8], int srcRow, int destRow, int constCol)
{
	int big, small;
	if(srcRow > destRow){
		big = srcRow;
		small = destRow;
	}
	else
	{
		big = destRow;
		small = srcRow;
	}
	for (int i = small+1; i < big; ++i)
	{
		if(board[i][constCol] == 'w' || board[i][constCol] == 'W' || board[i][constCol] == 'b' || board[i][constCol] == 'B')
			board[i][constCol] = 0;
	}
}

Error checkerPieceMovement(char board[8][8], Turn turn, int rowSrc, int colSrc, int rowDest, int colDest)
{
	if((rowSrc != rowDest) && (colSrc != colDest)) return WRONG_PLACE_MOVEMENT;

	if(rowSrc == rowDest)
	{
		if(isThereYourOwnPieceAtThisRow(board, turn, colSrc, colDest, rowSrc)) return WRONG_PLACE_MOVEMENT;
		if(isThereMoreThanOneRivalPieceAtThisRow(board, turn, colSrc, colDest, rowSrc)) return WRONG_PLACE_MOVEMENT;
		deleteTheRivalAtThisRow(board, colSrc, colDest, rowSrc);
	}
	else if(colSrc == colDest)
	{
		if(isThereYourOwnPieceAtThisCol(board, turn, rowSrc, rowDest, colSrc)) return WRONG_PLACE_MOVEMENT;	
		if(isThereMoreThanOneRivalPieceAtThisCol(board, turn, rowSrc, rowDest, colSrc)) return WRONG_PLACE_MOVEMENT;
		deleteTheRivalAtThisCol(board, rowSrc, rowDest, colSrc);
	}

	else
		return NO_PROBLEM;
}	

Error isThisJump(char board[8][8], Turn *turn, int rowSrc, int colSrc, int rowDest, int colDest)
{
	int rowGap = rowSrc - rowDest;
	int colGap = colSrc - colDest;
	
	if(rowGap == 0)	//in the same row
	{
		int controlCol = (colSrc + colDest)/2;
		char controlChar = board[rowSrc][controlCol];
		if(*turn == WHITE && (controlChar == 'w' || controlChar == 'W')) return WRONG_PLACE_MOVEMENT;
		else if(*turn == BLACK && (controlChar == 'b' || controlChar == 'B')) return WRONG_PLACE_MOVEMENT;
		else if(board[rowSrc][controlCol] == 0) return WRONG_PLACE_MOVEMENT;
		else{
			board[rowSrc][controlCol] = 0;
			if(*turn == WHITE)
				*turn = BLACK;
			else
				*turn = WHITE;
			return NO_PROBLEM;
		} 
	}
	else if(colGap == 0)//in the same column
	{
		int controlRow = (rowSrc + rowDest)/2;
		char controlChar = board[controlRow][colSrc];
		if(*turn == WHITE && (controlChar == 'w' || controlChar == 'W')) return WRONG_PLACE_MOVEMENT;
		else if(*turn == BLACK && (controlChar == 'b' || controlChar == 'B')) return WRONG_PLACE_MOVEMENT;
		else if(board[controlRow][colSrc] == 0) return WRONG_PLACE_MOVEMENT;
		else{
			board[controlRow][colSrc] = 0;
			if(*turn == WHITE)
				*turn = BLACK;
			else
				*turn = WHITE;
			return NO_PROBLEM;
		} 
	}
	else
	{
		return WRONG_PLACE_MOVEMENT;
	}
}


const char* getErrorErrorMessage(Error e)
{
	switch(e)
	{
		case WRONG_COORDINATE: 
			return "Choosen coordinate is empty!"; 
			break;
		case RIVAL_PIECE:
			return "Chosen coordinate belongs to rival!";
			break;
		case SAME_PLACE_MOVEMENT:
			return "You can't play to same place!";
			break;
		case WRONG_PLACE_MOVEMENT:
			return "Your piece is not eligible to go there!";
			break;
		case RIVAL_PIECE_MOVEMENT:
			return "You can't position your piece over the rival's piece!";
			break;
		case WRONG_INPUT:
			return "Your input is wrong!";
			break;
		default:
			return "An error occured!"; 
			break;
	}
}

void initBoard(char board[8][8])
{
	for (int i = 0; i < 8; ++i)
		for (int j = 0; j < 8; ++j)
			board[i][j] = 0;
}

void printBoard(char board[8][8])
{
	for (int row = 0; row < 9; ++row)
	{
		for (int column = 0; column < 9; ++column)
		{
			if(row == 8 && column == 0) printf("\t");
			else if(column == 0) printf("%d\t", 8-row);
			else if(row == 8) printf("%c\t", column+64);
			else{
				if(board[row][column-1] == 'B') printf("%s\t", "bb");
				else if(board[row][column-1] == 'W') printf("%s\t", "ww");
				else printf("%c\t", board[row][column-1]);

			}	
		}
		printf("\n\n");
	}
}

void initFileFromScratch(const char* pathTxt)
{
	FILE* fp = fopen(pathTxt, "w");
	for (int row = 0; row < 8; ++row)
	{
		for (int column = 0; column < 8; ++column)
		{
			if(row == 1 || row == 2){
				fputc('w', fp);
				fputc(' ', fp);
				fputc('\t', fp);
			}	
			else if(row == 5 || row == 6){
				fputc('b', fp);
				fputc(' ', fp);
				fputc('\t', fp);
			}
			else{
				fputc(' ', fp);
				fputc(' ', fp);
				fputc('\t', fp);
			}	
		}
		fputc('\n', fp);
	}
	fputc('w', fp);
	fclose(fp);
}

void fetchBoardFromFile(char board[8][8], Turn *turn)
{
	FILE *fp = fopen("gameboard.txt", "r");

	char ch;
	int row = 0, column = 0;
	int keyForWhitespace = 0, keyForWhite = 0, keyForBlack = 0;
	while((ch = fgetc(fp)) != EOF)
	{	
		if(column == 8){
			row++;
			column = 0;
		}
		if(row == 8) break;	
		if(ch == '\t'){
			closeAllKeys(&keyForWhitespace, &keyForBlack, &keyForWhite);
			continue;
		} 
		if(ch == '\n'){
			closeAllKeys(&keyForWhitespace, &keyForBlack, &keyForWhite);
		}
		else if(ch == ' ')
		{
			if(keyForBlack)
			{
				board[row][column++] = 'b';
			}
			else if(keyForWhite)
			{
				board[row][column++] = 'w';
			}
			else if(keyForWhitespace)
			{
				board[row][column++] = 0;
			}
			else
				keyForWhitespace = 1;
		}
		else if(ch == 'w')
		{
			if(keyForWhite)
			{
				board[row][column++] = 'W';
			}
			else
				keyForWhite = 1;
		}
		else if(ch == 'b')
		{
			if(keyForBlack)
			{
				board[row][column++] = 'B';
			}
			else
				keyForBlack = 1;
		}
	}

	ch = fgetc(fp);		//get newline character
	//For turn
	ch = fgetc(fp);
	if(ch == 'w') *turn = WHITE;
	else *turn = BLACK;
	fclose(fp);
}

void postBoardToFile(char board[8][8], Turn turn)
{
	FILE *fp = fopen("gameboard.txt", "w");
	for (int row = 0; row < 8; ++row)
	{
		for (int column = 0; column < 8; ++column)
		{
			if(board[row][column] == 0)	fputs("  \t", fp);
			else if(board[row][column] == 'w') fputs("w \t", fp);
			else if(board[row][column] == 'b')	fputs("b \t", fp);
			else if(board[row][column] == 'W') fputs("ww\t", fp);
			else if(board[row][column] == 'B') fputs("bb\t", fp);
		}
		fputc('\n', fp);
	}

	if(turn == WHITE)	fputc('w', fp);
	else 	fputc('b', fp);

	fclose(fp);
}

void closeAllKeys(int *keyForWhitespace, int *keyForBlack, int *keyForWhite)
{
	*keyForWhitespace = 0;
	*keyForWhite = 0;
	*keyForBlack = 0;
}

























