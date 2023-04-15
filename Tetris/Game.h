#ifndef __GAME_H__

#define GAME_WIDTH 10
#define GAME_HEIGHT 20

typedef enum
{
	GameState_GameOver,
	GameState_Running,
	GameState_Paused
} GameState_e;

typedef struct
{
	int Size;
	char Elements[16];
} Block_t;

typedef struct
{
	GameState_e GameState;

	Block_t CurrentBlock, NextBlock;

	float ColorTable[7][3];

	int xPos, yPos, BlockType, NextBlockType;
	int Board[GAME_HEIGHT+1][GAME_WIDTH+2];

	int Score;

	int NumDropCount;
	int IsDropping;

	int GameSpeed;
	float MusicSpeed;

	int BlockSize;
} Tetris_t;

void TetrisNewGame(Tetris_t *Tetris);
void TetrisGenBType(Tetris_t *Tetris);
void TetrisRunStep(Tetris_t *Tetris);
void TetrisLeft(Tetris_t *Tetris);
void TetrisRight(Tetris_t *Tetris);
void TetrisRotate(Tetris_t *Tetris);
void TetrisPause(Tetris_t *Tetris);
void TetrisDraw(Tetris_t *Tetris);

#endif
