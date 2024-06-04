#include "_SDL.h"

//FUNCION PARA INICIAR SDL Y LA VENTANA
void SDLRun(int x, int y, int bits, char* title){
        if(SDL_Init(SDL_INIT_VIDEO)<0){printf("No se pudo inicializar SDL: %s\n",SDL_GetError());exit(EXIT_FAILURE);}

	//INICIALIZO FUENTES Y ABRO ORIGEN
        TTF_Init();
        tamFuente = 12;
        fuente = TTF_OpenFont("resources/cour.ttf",tamFuente);
        //SETEO COLOR DE LA FUENTE
        fuenteColor.r = 237;
        fuenteColor.g = 28;
        fuenteColor.b = 36;
        screenColor.r=0;
        screenColor.g=0;
        screenColor.b=0;
        cant_filas=5;cant_naves=10;

        
        //// CARGO LAS IMAGENES ////
        SDLSprites = SDL_LoadBMP("resources/sprites.bmp");//NAVES
        SDLScreen = SDL_LoadBMP("resources/screen.bmp");//FONDO
	SDLBotones = SDL_LoadBMP("resources/botones.bmp");//BOTONES
        
	//SETEO EL COLOR TRANSPARENTE
        SDL_SetColorKey(SDLScreen,SDL_SRCCOLORKEY|SDL_RLEACCEL,SDL_MapRGB(SDLScreen->format,255,255,255));
        SDL_SetColorKey(SDLSprites,SDL_SRCCOLORKEY|SDL_RLEACCEL,SDL_MapRGB(SDLSprites->format,255,0,255));
        SDL_SetColorKey(SDLBotones,SDL_SRCCOLORKEY|SDL_RLEACCEL,SDL_MapRGB(SDLBotones->format,255,0,255));
        
        //// CREO LA VENTANA CON LAS DIMENSIONES DADAS ////
        SDLBuffer = SDL_SetVideoMode(x, y, bits, SDL_HWSURFACE );
        //// VERIFICO SI SE PUDO CREAR LA VENTANA ////
        if(SDLBuffer==NULL){printf("Error al entrar a modo grafico: %s\n",SDL_GetError());SDL_Quit();exit(EXIT_FAILURE);}
        //// SETEO EL TITULO DE VENTANA ////
        SDL_WM_SetCaption(title, title);
        
        naveRoja.visible=1;
        frameNave.visible=1;
        frameNave.vidas=3;
        explosion.visible=1;

        //Posiciono las naves, en las posiciones dadas (x,y)
        setPosition(&frameNave, MARGEN_X/2, MARGEN_Y-30);
        setPosition(&naveRoja, MARGEN_X-220, 300);
        setPosition(&frameVida, MARGEN_X - 150, MARGEN_Y -30);
        setPosition(&explosion, 10, 10);

        setSprite(&frameNave,NAVE_PLAYER1);
        setSprite(&frameBala,SHOT);
        setSprite(&frameBalaEnemiga, SHOT);
        setSprite(&frameVida, NAVE_PLAYER1);
        setSprite(&naveRoja, NAVE_NEUTRAL);
        setSprite(&explosion, EXPLOSION);
        
        
        fondoSala = SDL_LoadBMP("resources/splash.bmp");

        setPosition(&btn_stop,308,92);//POSICION BOTON TERMINAR
        setSprite(&btn_stop,BTN_STOP);
        setPosition(&btn_exit,308, 142);//SETEO POSICION DEL BOTON SALIR
        setSprite(&btn_exit,BTN_EXIT);//SETEO IMAGEN DEL BOTON SALIR
}

//PASA LO DEL BUFFER A LA VENTANA
void showScreen(){
        //// DIBUJO LO QUE HAY EN EL BUFFER ////
        SDL_Flip(SDLBuffer);
        //// INDICO QUE HAY UN NUEVO EVENTO ////
        SDL_PollEvent(&event);
}

//AGREGA UN FRAME AL BUFFER
//DEFAULT:  AGREGA EL FRAME COMPLETO SOBRE EL BUFFER DESDE EL (0,0)
//BOX:      AGREGA UN TROZO DEL FRAME DADO POR frame.box SOBRE EL BUFFER DESDE EL (0,0)
//POSITION: AGREGA EL FRAME COMPLETO SOBRE EL BUFFER DESDE LA POSICION DADA POR frame.position
//BOTH:     AGREGA UN TROZO DEL FRAME DADO POR frame.box SOBRE EL BUFFER DESDE LA POSICION DADA POR frame.position
//ASCII:    AGREGA FRAMES DE LETRAS
void addScreen(Frame *frame,int mode){
        switch (mode){
                case DEFAULT:   SDL_BlitSurface(SDLScreen,NULL,SDLBuffer,NULL);break;
                case BOX:       SDL_BlitSurface(SDLSprites,&(*frame).box,SDLBuffer,NULL);break;
                case POSITION:  SDL_BlitSurface(SDLSprites,NULL,SDLBuffer,&(*frame).position);break;
                case BOTH:      SDL_BlitSurface(SDLSprites,&(*frame).box,SDLBuffer,&(*frame).position);break;
		case BOTON:	SDL_BlitSurface(SDLBotones, &(*frame).box, SDLBuffer, &(*frame).position);break;          
		default:        break;
        }
}

//SETEA LA POSICION DEL FRAME
void setPosition(Frame *frame, int x, int y) {(*frame).position.x=x;(*frame).position.y=y;}
//SETEA EL TROZO DEL FRAME
void setBox(Frame *frame, int x, int y, int w, int h) {(*frame).box.x=x;(*frame).box.y=y;(*frame).box.w=w;(*frame).box.h=h;}

//SETEA LOS BOX DE LOS SPRITES
void setSprite(Frame *frame, int nave){
        switch (nave){
                case ENEMY_A:           setBox(frame,0,0,23,17);break;
                case ENEMY_B:           setBox(frame,23,0,23,17);break;
                case ENEMY_C:           setBox(frame,46,0,16,16);break;
                case ENEMY_D:           setBox(frame,62,0,16,16);break;
		case EXPLOSION: 	setBox(frame,78,0,16,16);break;                
		case NAVE_PLAYER2:      setBox(frame,193,0,26,21);break;
                case NAVE_PLAYER1:      setBox(frame,167,0,26,21);break;
                case NAVE_NEUTRAL:      setBox(frame,135,0,32,14);break;                
                case SHOT:              setBox(frame,136,15,2,6);break;
                case BTN_STOP:           setBox(frame,0,0,249,23);break;
                case BTN_STOP_FOCUS:     setBox(frame,0,23,249,23);break;
                case BTN_STOP_PRESS:     setBox(frame,0,46,249,23);break;
                case BTN_EXIT:           setBox(frame,0,69,249,23);break;
                case BTN_EXIT_FOCUS:     setBox(frame,0,92,249,23);break;
                case UNION:              setBox(frame,0,155,22,19);break; 
                default: break;
        }
}

//LIMPIA LA PANTALLA
void SDLClear(){
        SDL_Rect PosScr;
        PosScr.x = 0;
        PosScr.y = 0;
        PosScr.w = SDLBuffer->w;
        PosScr.h = SDLBuffer->h;
        SDL_FillRect(SDLBuffer, &PosScr, SDL_MapRGB(SDLBuffer->format, 0, 0, 0));
}

void addText(char *text,int x,int y){
	ttext.imageText=TTF_RenderText_Shaded(fuente,text,fuenteColor,screenColor);
	ttext.position.x=x;
	ttext.position.y=y;
	SDL_BlitSurface(ttext.imageText,NULL,SDLBuffer,&ttext.position);//AGREGO TEXTO
}
