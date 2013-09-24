//
// Achtung die kurve
// zrobiony przy pomocy szablonu gry w Allegro 5.
// (C) Kółko Informatyczne Szkoły Żagle
//

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include <iostream>
using namespace std;


const int screen_w = 1366;   // szerokość ekranu (screen width)
const int screen_h = 768;   // wysokość ekranu (screen height)

/****************************************
 * Kod poniżej jest w miarę generyczny  *
 ****************************************/
 
const float FPS = 700; //60      // Frames Per Second
bool key[ALLEGRO_KEY_MAX];  // wciśnięte klawisze

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_BITMAP * snakes = NULL;


int init()
{
    if(!al_init()) {
        cerr << "Błąd podczas inicjalizacji allegro." << endl;
        return -1;
    }
  
    if (!al_init_primitives_addon()) {
        cerr << "Błąd podczas inicjalizacji dodatku 'primitives'." << endl;
        return -1;
    }
    
    if (!al_install_keyboard()) {
        cerr << "Błąd podczas inicjalizacji klawiatury." << endl;
        return -1;
    }

    if (!al_install_mouse()) {
        cerr << "Błąd podczas inicjalizacji myszy." << endl;
        return -1;
    }
  
    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
        cerr << "Błąd podczas inicjalizacji zegara." << endl;
        return -1;
    }
 
    display = al_create_display(screen_w, screen_h);
    if(!display) {
        cerr << "Błąd podczas inicjalizacji ekranu." << endl;
        al_destroy_timer(timer);
        return -1;
    }

    event_queue = al_create_event_queue();
    if(!event_queue) {
        cerr << "Błąd podczas inicjalizacji kolejki zdarzeń." << endl;
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }
  
    al_register_event_source(event_queue, al_get_display_event_source(display));  
    al_register_event_source(event_queue, al_get_timer_event_source(timer));  
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_clear_to_color(al_map_rgb(0,0,0));
  
    al_flip_display();  
    al_start_timer(timer);

    return 0;
}
/****************************************
 * Tu rozpoczyna się istotna część kodu *
 ****************************************/

//
//kofiguracja gry
//
	const int number_of_player=1;
	int xpl=1071;
	int ypl=688;
	int czas=0;
	int lastczas;
	//player1
	float xplayer1=5;
	float yplayer1=5;
	float stepplayer1=0.1;
	float radiusplayer1=3;//promień 
	float spaceplayer1=90;
	float  alfaplayer1=0.3;
	float degreesplayer1=0;

//
// Struktury danych
//
	typedef struct type_of_player{
		int x;
		int y;
		int step;
		int radius;
		int space;
		int alfa;
		int degrees;
	};
	type_of_player player[number_of_player];
	typedef struct type{
		int nrplayer;
		int time;
	};
	type board[1082][689];//+2

//
// Zmienne
//
	bool przegrales=false;
    bool dotyka=false;
    
//
//Czyszczenie
//
void clean()
{
	srandom(time(NULL)+getpid());
	for(int i=1;i<1081;i++){
		for(int a=1;a<688;a++){
		board[i][a].nrplayer=-1;
		board[i][a].time=-1;
		}
	}
	for(int i=0;i<1082;i++){
	board[i][0].nrplayer=100;
	board[i][688].nrplayer=100;
	board[i][0].time=-100;
	board[i][688].time=-100;
		if(i<688){
		board[0][i].nrplayer=100;
		board[1081][i].nrplayer=100;
		board[0][i].time=-100;
		board[1081][i].time=-100;
		}
	}
	player[0].x=5;
	player[0].y=5;
	player[0].radius=3;
	player[0].step=0.1;
	player[0].space=90;
	player[0].alfa=0.3;
	player[0].degrees=0;
	while(player[0].x<=20+player[0].radius || player[0].y<=20+player[0].radius){
	player[0].x=random()%1072-2*player[0].radius;
	player[0].y=random()%679-2*player[0].radius;
	}
	snakes = al_create_bitmap(1080,687);
	al_set_target_bitmap(snakes);
	al_clear_to_color(al_map_rgba(0, 0, 0, 0));
	al_set_target_backbuffer(display);
}

//
// Rysowanie planszy
//

void rysuj_plansze()
{
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_rectangle(19, 19, 1101, 709, al_map_rgb(255, 255, 255), 0 );//x1,y1,x2,y2,kolor,szerokosc;
	al_set_target_bitmap(snakes);
    if (dotyka) {
    	al_draw_filled_circle(xplayer1-1, yplayer1-1, radiusplayer1, al_map_rgb(155, 23, 0));
    } else {
    	al_draw_filled_circle(xplayer1, yplayer1-1, radiusplayer1, al_map_rgb(0, 23, 155));
    }
	al_draw_filled_circle(player[0].x, player[0].y, player[0].radius, al_map_rgb(23, 23, 23));
	al_set_target_backbuffer(display);
	al_draw_bitmap(snakes, 20, 20, 0);
}

//
// Mechanika gry
//

void aktualizuj_plansze()
{
    dotyka = false;
	czas++;
	player[0].x=player[0].x+player[0].step*cos(degreesplayer1);
	player[0].y=player[0].y+player[0].step*sin(degreesplayer1);
	xplayer1=xplayer1+stepplayer1*cos(degreesplayer1);
	yplayer1=yplayer1+stepplayer1*sin(degreesplayer1);
	for(int i=0;i<player[0].radius;i++){
		for(int a=0;a<player[0].radius;a++){
			float f_iks=player[0].x-(player[0].radius/2.0)+i;
			float f_igrek=player[0].y-(player[0].radius/2.0)+a;
			if (f_iks < 0) { f_iks = 0; }
			if (f_igrek < 0) { f_igrek = 0; }
			if (f_iks > 1081) { f_iks = 1081; }
			if (f_igrek > 688) { f_igrek = 688; }
			
			if((f_igrek-player[0].y)*(f_igrek-player[0].y)+(player[0].x-f_iks)*(player[0].x-f_iks)<player[0].radius*player[0].radius){
				int iks = (int) f_iks;
				int igrek = (int) f_igrek;
				
				if(board[iks][igrek].nrplayer!=-1){
					if(board[iks][igrek].nrplayer==1){
						if(board[iks][igrek].time<czas-100){
							cout<<"wjechales w siebie"<<endl;
//							przegrales=true;
                            dotyka = true;
							break;
						}
					}else if(board[iks][igrek].nrplayer!=100){
						cout<<"wiechales w kolege"<<endl;
//						przegrales=true;
                        dotyka = true;
						break;
					}else if(board[iks][igrek].nrplayer==100){
						cout<<"wjechales w sciane"<<endl;
//						przegrales=true;
                        dotyka = true;
						break;
					}
				}else{
					board[iks][igrek].nrplayer=1;
					board[iks][igrek].time=czas;
				}
			}
		}
	}
	
}

//
// Reakcja na akcje graczy
//

void co_robia_gracze()
{
	if(key[ALLEGRO_KEY_LEFT] && czas-lastczas>spaceplayer1){
	degreesplayer1=degreesplayer1-alfaplayer1;
	lastczas=czas;
	}
	if(key[ALLEGRO_KEY_RIGHT] && czas-lastczas>spaceplayer1){
	degreesplayer1=degreesplayer1+alfaplayer1;
	lastczas=czas;
	}
	if(key[ALLEGRO_KEY_LEFT] && czas-lastczas>player[0].space){
	player[0].degrees=player[0].degrees+player[0].alfa;
	lastczas=czas;
	}


}



int main(int argc, char ** argv)
{
    if (init() != 0) {
        cerr << "Inicjalizacja nie powiodła się." << endl;
        return -1;
    }

    bool przerysuj = true;
    bool wyjdz = false;

	clean();
    //
    // Event loop - główna pętla programu
    //
        
    while(!wyjdz && !przegrales)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if(ev.type == ALLEGRO_EVENT_TIMER) {
            //
            // minęła 1/60 (1/FPS) część sekundy
            //
            przerysuj = true;

            co_robia_gracze();

            aktualizuj_plansze();

        } else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            key[ev.keyboard.keycode] = true;
        } else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
            key[ev.keyboard.keycode] = false ;

            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                wyjdz = true;
            }
        }

        if(przerysuj && al_is_event_queue_empty(event_queue)) {
            przerysuj = false;

            rysuj_plansze();

            al_flip_display();
         }
	if(przegrales){cout<<"YOU LOST BABY ON X= "<<xplayer1<<" and Y= "<<yplayer1<<endl;}
    }

    return 0;
}