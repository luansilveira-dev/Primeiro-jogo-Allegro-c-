#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include "obj.h"

// MAY CONST

const int WIDTH = 448*3.2, HEIGTH = 960, FPS=60;
const int QTD_MISSIL = 3;

enum TECLAS{CIMA, BAIXO, ESQUERDA, DIREITA, ESPACO};

//####  FUNÇÕES DEFINHIDAS  #########
void IniciarNave(Nave &nave);
void DrawNave(Nave &nave, ALLEGRO_BITMAP * image);
void MoverCima(Nave &nave);
void MoverBaixo(Nave &nave);
void MoverDireita(Nave &nave);
void MoverEsquerda(Nave &nave);

void IniciarMissil(Missil missil[], int qtdMissil);
void AtirarMissil(Missil missil[], int qtdMissil, Nave nave);
void AtualizarMissil(Missil missil[], int qtdMissil);
void DrawMissil(Missil missil[], int qtdMissil, ALLEGRO_BITMAP * image);


void IniciarFundo(Fundo &fundo);
void MoverFundo(Fundo &fundo);
void DrawFundo(Fundo &fundo, ALLEGRO_BITMAP * image);


int main()
{
    ALLEGRO_EVENT_QUEUE * gameEvent = NULL;
    ALLEGRO_TIMER * timer = NULL;
    ALLEGRO_BITMAP * imageNave = NULL, * imageMissil = NULL, * imageFundo;

    bool draw = true;
    bool run = true;
    bool teclas[] = {false, false, false, false, false};

    // ##########  INICCIALIZAÇÃO DE OBJETOS   ############

    Nave nave;
    Missil missil[QTD_MISSIL];
    Fundo fundo;


    //###################################################



    // CRIAÇÃO DE MINHA JANELA
    ALLEGRO_DISPLAY * janela = NULL;
    if (!al_init()){}

    janela = al_create_display(WIDTH, HEIGTH);

    if (!janela){}

    // INICIALIZAÇÃO DE ADDONS E INSTALAÇÕES --
    al_install_keyboard();
    al_init_primitives_addon();
    al_init_image_addon();

    // CRIAO DE FILAS DE DISPOSITIVOS
    gameEvent = al_create_event_queue();
    timer = al_create_timer(1.0/FPS);
    imageNave = al_load_bitmap("image/nave.png");
    imageMissil = al_load_bitmap("image/missil.png");
    imageFundo = al_load_bitmap("image/espaco.png");


    al_set_window_position(janela, -10,-20);


    // REGISTRO DE SORCES "FONTES"

    al_register_event_source(gameEvent, al_get_display_event_source(janela));
    al_register_event_source(gameEvent, al_get_keyboard_event_source());
    al_register_event_source(gameEvent, al_get_timer_event_source(timer));

    //######   INICIAR FUNÇÕES ######
    IniciarNave(nave);
    IniciarMissil(missil, QTD_MISSIL);
    IniciarFundo(fundo);

    //###############################

    al_start_timer(timer);

    while(run)
    {
        ALLEGRO_EVENT evento;
        al_wait_for_event(gameEvent, &evento);


        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            run = false;
        }

        else if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            switch (evento.keyboard.keycode)
            {
            case ALLEGRO_KEY_ESCAPE:
                run = false;
                break;
            case ALLEGRO_KEY_UP:
                teclas[CIMA] = true;
                break;
            case ALLEGRO_KEY_DOWN:
                teclas[BAIXO] = true;
                break;
            case ALLEGRO_KEY_RIGHT:
                teclas[DIREITA] = true;
                break;
            case ALLEGRO_KEY_LEFT:
                teclas[ESQUERDA] = true;
                break;
            case ALLEGRO_KEY_SPACE:
                teclas[ESPACO] = true;
                AtirarMissil(missil,QTD_MISSIL, nave);
                break;
            }
        }

        else if (evento.type == ALLEGRO_EVENT_KEY_UP)
        {
            switch (evento.keyboard.keycode)
            {
            case ALLEGRO_KEY_UP:
                teclas[CIMA] = false;
                break;
            case ALLEGRO_KEY_DOWN:
                teclas[BAIXO] = false;
                break;
            case ALLEGRO_KEY_RIGHT:
                teclas[DIREITA] = false;
                break;
            case ALLEGRO_KEY_LEFT:
                teclas[ESQUERDA] = false;
                break;
            }
        }

        if (evento.type == ALLEGRO_EVENT_TIMER)
        {
            draw = true;
            if (teclas[CIMA])
                MoverCima(nave);

            if (teclas[BAIXO])
                MoverBaixo(nave);

            if (teclas[DIREITA])
                MoverDireita(nave);

            if (teclas[ESQUERDA])
                MoverEsquerda(nave);
            if (teclas[ESPACO])
                AtualizarMissil(missil, QTD_MISSIL);

            MoverFundo(fundo);

        }

        /* ######################################
        ###########  DESENHOS GRÁFICOS  #########*/
        if (draw && al_is_event_queue_empty(gameEvent))
        {
            draw = false;
            DrawFundo(fundo, imageFundo);
            DrawMissil(missil, QTD_MISSIL, imageMissil);
            DrawNave(nave, imageNave);


            al_flip_display();
            al_clear_to_color(al_map_rgb(255,255,255));
        }
    }

    al_destroy_display(janela);
    al_destroy_bitmap(imageNave);

    return 0;

}



/*#################################################################
##################    FUNÇOES DE CÓDIGOS DA NAVE  ################
###################################################################*/

void IniciarNave(Nave &nave)
{
    nave.x = WIDTH/2-40;
    nave.y = HEIGTH/2*1.3;
    nave.ID = JOGADOR;
    nave.vidas = 3;
    nave.velocidade = 10;
    nave.borda_x = 6;
    nave.borda_y = 7;
    nave.pontos = 0;

}

void DrawNave(Nave &nave, ALLEGRO_BITMAP * image)
{
    al_draw_scaled_rotated_bitmap(image, 0, 0, nave.x, nave.y, 0.48, 0.54, 0,0);
}

void MoverCima(Nave &nave)
{
    nave.y -= nave.velocidade;
    if(nave.y < HEIGTH/4*0.5)
        nave.y = HEIGTH/4*0.5;

}
void MoverBaixo(Nave &nave)
{
    nave.y += nave.velocidade;
    if (nave.y > HEIGTH/2*1.5+25)
        nave.y = HEIGTH/2*1.5+25;
}
void MoverDireita(Nave &nave)
{
    nave.x += nave.velocidade;
    if (nave.x > WIDTH/2*1.83)
        nave.x = WIDTH/2*1.83;
}
void MoverEsquerda(Nave &nave)
{
    nave.x -= nave.velocidade;
    if (nave.x < 10)
        nave.x = 10;
}

/*####################################################
##############  FUNÇOES E CÓDIGOS DO MISSIL ##########
######################################################*/


void IniciarMissil(Missil missil[], int qtdMissil)
{
    for (int i = 0; i < qtdMissil; i++)
    {
        missil[i].ID = MISSIL;
        missil[i].velocidade = 25;
        missil[i].ativo = false;
    }

}
void AtirarMissil(Missil missil[], int qtdMissil, Nave nave)
{
    for (int i = 0; i < qtdMissil;  i++)
    {
        if (!missil[i].ativo)
        {
            missil[i].x = nave.x + 20;
            missil[i].y = nave.y + 50;
            missil[i].ativo = true;
            break;
        }
    }
}
void AtualizarMissil(Missil missil[], int qtdMissil)
{
    for (int i = 0; i < qtdMissil; i++)
    {
        missil[i].y -= missil[i].velocidade;

        if (missil[i].y < -100)
            missil[i].ativo = false;
    }
}
void DrawMissil(Missil missil[], int qtdMissil, ALLEGRO_BITMAP * image)
{
    for (int i = 0; i < qtdMissil; i++)
    {
        if (missil[i].ativo)
        {
            al_draw_scaled_rotated_bitmap(image, 0, 0, missil[i].x, missil[i].y, 0.3, 0.5, 0,0);
            al_draw_scaled_rotated_bitmap(image, 0, 0, missil[i].x+55, missil[i].y, 0.3, 0.5, 0,0);
        }
    }
}


/*####################################################
##############  FUNÇOES E CÓDIGOS DO FUNDO BG ##########
######################################################*/


void IniciarFundo(Fundo &fundo)
{
    fundo.x = 0;
    fundo.y = 960;
    fundo.velocidade = 20;
}
void MoverFundo(Fundo &fundo)
{
    fundo.y += fundo.velocidade;
    if (fundo.y > HEIGTH*2)
        fundo.y = 960;



}
void DrawFundo(Fundo &fundo, ALLEGRO_BITMAP * image)
{
    al_draw_scaled_rotated_bitmap(image, 0, 0, fundo.x , fundo.y ,4,4 ,300.02 ,0);
    al_draw_scaled_rotated_bitmap(image, 0, 0, fundo.x , fundo.y-HEIGTH ,4,4, 300.02 ,0);

    al_draw_scaled_rotated_bitmap(image, 0, 0, WIDTH/2 , fundo.y ,4,4 ,300.02 ,0);
    al_draw_scaled_rotated_bitmap(image, 0, 0, WIDTH/2 , fundo.y-HEIGTH ,4,4, 300.02 ,0);



}

