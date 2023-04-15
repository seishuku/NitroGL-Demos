#include <AL/al.h>
#include <AL/alc.h>
#include "opengl.h"
#include "math.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Game.h"
#include "3ds.h"

void DrawModel3DS(Model3DS_t *Model);

extern Model3DS_t Model[2];
extern int SwitchObject;

typedef enum
{
	TEXTURE_REFLECT,
	TEXTURE_SPECULAR,

	TEXTURE_ORIGINAL,
	TEXTURE_BLUR0,
	TEXTURE_BLUR1,

	BUFFER_DEPTH,
	BUFFER_ORIGINAL,
	BUFFER_BLUR,

	VERTEX_GENERIC,

	FRAGMENT_REFLECT,
	FRAGMENT_LIGHTING,
	FRAGMENT_DIFFUSE,
	FRAGMENT_BLUR,
	FRAGMENT_COMPOSITE,

	SOUND_LINE,
	SOUND_LINE4,
	SOUND_DROP,
	SOUND_MOVE,
	SOUND_ROTATE,
	SOUND_LEVELUP,

	NUM_OBJECTS
} ObjectNames;

extern unsigned int Objects[NUM_OBJECTS];

extern unsigned int MusicSource;

extern int randrange(int min, int max);

Block_t Blocks[7]=
{
	{	4,	" *  "
			" *  "
			" *  "
			" *  "	},

	{	3,	"    "
			"*** "
			"  * "
			"    "	},

	{	3,	"    "
			"*** "
			"*   "
			"    "	},

	{	3,	"    "
			"*** "
			" *  "
			"    "	},

	{	3,	"**  "
			" ** "
			"    "
			"    "	},

	{	3,	" ** "
			"**  "
			"    "
			"    "	},

	{	2,	"**  "
			"**  "
			"    "
			"    "	}
};

void RemoveLines(Tetris_t *Tetris)
{
	int i, j, k, l;
	int LineFull, LinesRemoved=0, NumLines=0;
	int OldScore;

	j=GAME_HEIGHT-1;

	while(j>=0)
	{
		LineFull=1;

		for(i=1;i<=GAME_WIDTH;i++)
		{
			if(Tetris->Board[j][i]==0)
				LineFull=0;
		}

		if(LineFull)
		{
			NumLines++;
			LinesRemoved=1;

			for(k=j;k>=1;k--)
			{
				for(l=1;l<=GAME_WIDTH;l++)
					Tetris->Board[k][l]=Tetris->Board[k-1][l];
			}

			for(l=1;l<=GAME_WIDTH;l++)
				Tetris->Board[0][l]=0;
		}
		else
			j--;
	}

	OldScore=Tetris->Score;

	switch(NumLines)
	{
		case 1:
			alSourcePlay(Objects[SOUND_LINE]);
			Tetris->Score+=100;
			break;

		case 2:
			alSourcePlay(Objects[SOUND_LINE]);
			Tetris->Score+=400;
			break;

		case 3:
			alSourcePlay(Objects[SOUND_LINE]);
			Tetris->Score+=900;
			break;

		case 4:
			alSourcePlay(Objects[SOUND_LINE4]);
			Tetris->Score+=2000;
			break;

		default:
			alSourcePlay(Objects[SOUND_DROP]);
			break;
	}

	if(OldScore/2000!=Tetris->Score/2000)
	{
		if(Tetris->GameSpeed>1)
		{
			Tetris->GameSpeed--;
			Tetris->MusicSpeed+=0.025f;

			alSourcePlay(Objects[SOUND_LEVELUP]);
		}
	}
}

void ClearBoard(Tetris_t *Tetris)
{
	int i, j;

	for(i=0;i<(GAME_HEIGHT+1);i++)
	{
		for(j=0;j<(GAME_WIDTH+2);j++)
			Tetris->Board[i][j]=0;
	}

	for(i=0;i<(GAME_HEIGHT+1);i++)
	{
		Tetris->Board[i][0]=-1;
		Tetris->Board[i][GAME_WIDTH+1]=-1;
	}

	for(j=0;j<(GAME_WIDTH+2);j++)
		Tetris->Board[GAME_HEIGHT][j]=-1;
}

int HitTest(Tetris_t *Tetris)
{
	int i, j;

	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			if(((Tetris->xPos+i)<(GAME_WIDTH+2))&&((Tetris->xPos+i+1)>=0)&&((Tetris->yPos+j)<(GAME_HEIGHT+1))&&((Tetris->yPos+j)>=0))
			{
				if(Tetris->Board[Tetris->yPos+j][Tetris->xPos+1+i]!=0)
				{
					if(Tetris->CurrentBlock.Elements[j*4+i]=='*')
						return 1;
				}
			}
		}
	}

	return 0;
}

void PlaceBlock(Tetris_t *Tetris)
{
	int i, j;

	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			if(Tetris->CurrentBlock.Elements[j*4+i]=='*')
				Tetris->Board[Tetris->yPos+j][Tetris->xPos+1+i]=Tetris->BlockType;
		}
	}

	RemoveLines(Tetris);
}

void NewBlock(Tetris_t *Tetris, int BlockType)
{
	Tetris->CurrentBlock=Tetris->NextBlock;
	Tetris->BlockType=Tetris->NextBlockType;

	Tetris->NextBlock=Blocks[BlockType];
	Tetris->NextBlockType=BlockType+1;

	Tetris->xPos=4;
	Tetris->yPos=0;

	if(HitTest(Tetris))
	{
		PlaceBlock(Tetris);
		Tetris->GameState=GameState_GameOver;
	}
}

void DrawBlock(Tetris_t *Tetris)
{
	int i, j;

	for(i=0;i<Tetris->CurrentBlock.Size;i++)
	{
		for(j=0;j<Tetris->CurrentBlock.Size;j++)
		{
			if(Tetris->CurrentBlock.Elements[j*4+i]=='*')
			{
				glPushMatrix();
				glTranslatef((float)(Tetris->xPos*Tetris->BlockSize+i*Tetris->BlockSize), 1.0f-(float)(Tetris->yPos*Tetris->BlockSize+j*Tetris->BlockSize), 0.0f);
				glColor3fv(Tetris->ColorTable[Tetris->BlockType-1]);
				DrawModel3DS(&Model[SwitchObject]);
				glPopMatrix();
			}
		}
	}
}

void DrawNextBlock(Tetris_t *Tetris)
{
	int i, j;

	for(i=0;i<Tetris->NextBlock.Size;i++)
	{
		for(j=0;j<Tetris->NextBlock.Size;j++)
		{
			if(Tetris->NextBlock.Elements[j*4+i]=='*')
			{
				glPushMatrix();
				glTranslatef(((float)(Tetris->BlockSize+i*Tetris->BlockSize))+200.0f, (1.0f-(float)(Tetris->BlockSize+j*Tetris->BlockSize))+25.0f, 0.0f);
				glColor3fv(Tetris->ColorTable[Tetris->NextBlockType-1]);
				DrawModel3DS(&Model[SwitchObject]);
				glPopMatrix();
			}
		}
	}
}

void Rotate(Block_t *Block)
{
	char TempBlock[16];

	switch(Block->Size)
	{
		case 4:
			memcpy(TempBlock, Block->Elements, 4*4);

			Block->Elements[ 0]=TempBlock[ 0];
			Block->Elements[ 1]=TempBlock[ 4];
			Block->Elements[ 2]=TempBlock[ 8];
			Block->Elements[ 3]=TempBlock[12];
			Block->Elements[ 4]=TempBlock[ 1];
			Block->Elements[ 5]=TempBlock[ 5];
			Block->Elements[ 6]=TempBlock[ 9];
			Block->Elements[ 7]=TempBlock[13];
			Block->Elements[ 8]=TempBlock[ 2];
			Block->Elements[ 9]=TempBlock[ 6];
			Block->Elements[10]=TempBlock[10];
			Block->Elements[11]=TempBlock[14];
			Block->Elements[12]=TempBlock[ 3];
			Block->Elements[13]=TempBlock[ 7];
			Block->Elements[14]=TempBlock[11];
			Block->Elements[15]=TempBlock[15];
			break;

		case 3:
			memcpy(TempBlock, Block->Elements, 4*4);

			Block->Elements[ 0]=TempBlock[ 2];
			Block->Elements[ 1]=TempBlock[ 6];
			Block->Elements[ 2]=TempBlock[10];
			Block->Elements[ 4]=TempBlock[ 1];
			Block->Elements[ 5]=TempBlock[ 5];
			Block->Elements[ 6]=TempBlock[ 9];
			Block->Elements[ 8]=TempBlock[ 0];
			Block->Elements[ 9]=TempBlock[ 4];
			Block->Elements[10]=TempBlock[ 8];
			break;
	}
}

void TetrisNewGame(Tetris_t *Tetris)
{
	Tetris->BlockSize=20;
	Tetris->GameSpeed=10;
	Tetris->MusicSpeed=1.0f;

	Tetris->BlockType=0;
	Tetris->NumDropCount=0;
	Tetris->IsDropping=0;

	Tetris->Score=0;

	ClearBoard(Tetris);

	Tetris->ColorTable[0][0]=1.0f;
	Tetris->ColorTable[0][1]=0.0f;
	Tetris->ColorTable[0][2]=0.0f;

	Tetris->ColorTable[1][0]=1.0f;
	Tetris->ColorTable[1][1]=1.0f;
	Tetris->ColorTable[1][2]=0.0f;

	Tetris->ColorTable[2][0]=0.0f;
	Tetris->ColorTable[2][1]=1.0f;
	Tetris->ColorTable[2][2]=0.0f;

	Tetris->ColorTable[3][0]=0.0f;
	Tetris->ColorTable[3][1]=1.0f;
	Tetris->ColorTable[3][2]=1.0f;

	Tetris->ColorTable[4][0]=0.0f;
	Tetris->ColorTable[4][1]=0.0f;
	Tetris->ColorTable[4][2]=1.0f;

	Tetris->ColorTable[5][0]=1.0f;
	Tetris->ColorTable[5][1]=0.0f;
	Tetris->ColorTable[5][2]=1.0f;

	Tetris->ColorTable[6][0]=1.0f;
	Tetris->ColorTable[6][1]=1.0f;
	Tetris->ColorTable[6][2]=1.0f;

	NewBlock(Tetris, randrange(0, 6));
	NewBlock(Tetris, randrange(0, 6));

	Tetris->GameState=GameState_Running;
}

void TetrisGenBType(Tetris_t *Tetris)
{
	int i;

	TetrisNewGame(Tetris);

	for(i=0;i<50;i++)
		Tetris->Board[GAME_HEIGHT-randrange(0, GAME_HEIGHT/2)][randrange(0, GAME_WIDTH)]=randrange(0, 6);
}

void TetrisRunStep(Tetris_t *Tetris)
{
	switch(Tetris->GameState)
	{
		case GameState_GameOver:
			break;

		case GameState_Running:
			Tetris->NumDropCount++;

			if((Tetris->IsDropping)||(Tetris->NumDropCount==Tetris->GameSpeed))
			{
				Tetris->NumDropCount=0;
				Tetris->yPos++;

				if(HitTest(Tetris))
				{
					Tetris->yPos--;

					PlaceBlock(Tetris);
					RemoveLines(Tetris);

					NewBlock(Tetris, randrange(0, 6));
				}
			}
			break;

		case GameState_Paused:
			break;
	}
}

void TetrisLeft(Tetris_t *Tetris)
{
	if(Tetris->GameState==GameState_Paused)
		return;

	Tetris->xPos--;

	if(HitTest(Tetris))
		Tetris->xPos++;
	else
		alSourcePlay(Objects[SOUND_MOVE]);
}

void TetrisRight(Tetris_t *Tetris)
{
	if(Tetris->GameState==GameState_Paused)
		return;

	Tetris->xPos++;

	if(HitTest(Tetris))
		Tetris->xPos--;
	else
		alSourcePlay(Objects[SOUND_MOVE]);
}

void TetrisRotate(Tetris_t *Tetris)
{
	if(Tetris->GameState==GameState_Paused)
		return;

	Rotate(&Tetris->CurrentBlock);

	if(HitTest(Tetris)) 
		Rotate(&Tetris->CurrentBlock);

	if(HitTest(Tetris)) 
		Rotate(&Tetris->CurrentBlock);

	if(HitTest(Tetris)) 
		Rotate(&Tetris->CurrentBlock);

	alSourcePlay(Objects[SOUND_ROTATE]);
}

void TetrisPause(Tetris_t *Tetris)
{
	if(Tetris->GameState==GameState_Running)
		Tetris->GameState=GameState_Paused;
	else
	{
		if(Tetris->GameState==GameState_Paused)
			Tetris->GameState=GameState_Running;
	}
}

void TetrisDraw(Tetris_t *Tetris)
{
	int i, j;
	float Position[3]={ (((float)Tetris->xPos/(GAME_WIDTH-1))*2.0f-1.0f), 0.0f, -1.0f };

	glTranslatef(-(GAME_WIDTH*Tetris->BlockSize)*0.5f, (GAME_HEIGHT*Tetris->BlockSize)*0.5f, 0.0f);

	for(j=0;j<GAME_HEIGHT;j++)
	{
		for(i=0;i<GAME_WIDTH;i++)
		{
			if(Tetris->Board[j][i+1]!=0)
			{
				glPushMatrix();
				glTranslatef((float)(i*Tetris->BlockSize), 1.0f-(float)(j*Tetris->BlockSize), 0.0f);
				glColor3fv(Tetris->ColorTable[Tetris->Board[j][i+1]-1]);
				DrawModel3DS(&Model[SwitchObject]);
				glPopMatrix();
			}
		}
	}

	DrawNextBlock(Tetris);

	alSourcefv(Objects[SOUND_MOVE], AL_POSITION, Position);
	alSourcefv(Objects[SOUND_ROTATE], AL_POSITION, Position);
	alSourcef(MusicSource, AL_PITCH, Tetris->MusicSpeed);

	DrawBlock(Tetris);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}
