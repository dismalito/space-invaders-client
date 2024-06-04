#ifndef _SDL
#define _SDL
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#define SOUND_ARENA 3
#define NAVE_PLAYER1 1
#define NAVE_PLAYER2 2
#define ENEMY_A 3
#define ENEMY_B 4
#define ENEMY_C 5
#define ENEMY_D 6
#define NAVE_NEUTRAL 7
#define SHOT 8
#define EXPLOSION 10
#define ASCII 4
#define ROJA 9

#define BTN_STOP 11
#define BTN_STOP_FOCUS 12
#define BTN_STOP_PRESS 13
#define BTN_EXIT 14
#define BTN_EXIT_FOCUS 15
#define UNION 16
#define DEFAULT 0
#define BOX 1
#define POSITION 2
#define BOTH 3
#define BOTON 17
#define MARGEN_X 861//MARGEN DEL ALTO
#define MARGEN_Y 437//MARGEN DEL ANCHO
#define BITS 32//BITS POR PIXEL (DEFINE LA CALIDAD DE IMAGEN)


SDL_Event event;        
//// CONTROLA LOS EVENTOS ////
//// SE USA PARA CONTROLAR LAS TELCAS ////
Uint8* teclas;

//ESTRUCTURA PARA MANEJAR LOS OBJETOS VISIBLES
typedef struct{
        SDL_Rect position;
        SDL_Rect box;
        int visible;
	int vidas;
}Frame;

//ESTRUCTURA PARA IMPLEMENTAR TTF
typedef struct{
        SDL_Surface *imageText;
        SDL_Rect position;
}FrameText;


//ESTRUCTURA PARA MANEJAR LOS ESCUDOS
typedef struct{
        Frame bloques[12];//ESCUDO CONFORMADO POR 12 BLOQUES
}FrameShield;

//// VARIABLES PARA EL DIBUJO ////
SDL_Surface *SDLBuffer,*SDLSprites,*SDLScreen,*SDLBotones;  // EL BUFFER VA A SER EL LIENZO DONDE SE DIBUJARAN LOS DEMAS OBJETOS
Frame frameNave,frameBala,frameBalaEnemiga,frameVida,explosion,naveRoja;             // NAVE Y BALA QUE SE DIBUJARAN SOBRE EL BUFFER
Frame naves[5][10];
FrameShield bothShield1,bothShield2,bothShield3,bothShield4;
FrameShield topShield1, topShield2, topShield3, topShield4;
Frame btn_exit,btn_stop;
SDL_Surface *fondoSala;
//para el TTF
int tamFuente,cant_filas,cant_naves;
FrameText ttext;
TTF_Font *fuente;
SDL_Color fuenteColor;
SDL_Color screenColor;

void SDLRun(int x, int y, int bits, char* title);
void showScreen();
void addScreen(Frame *frame,int mode);
void setPosition(Frame *frame, int x, int y);
void setBox(Frame *frame, int x, int y, int w, int h);
void setSprite(Frame *frame, int nave);
void SDLClear();
void addText(char*,int,int);

#endif
