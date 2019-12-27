#include <stdio.h>
#include <allegro5/allegro.h>
#include<allegro5/allegro_image.h>
#include<vector>
#include<string>
#include<math.h>
#include<iostream>
using namespace std;

const float FPS = 60;
const int SCREEN_W = 1000;
const int SCREEN_H = 800;
const int BOUNCER_SIZE = 0;
const int ELEMENTWIDTH = 100;

class element {
private:
	int xpos;
	int ypos;
	int index;
	bool lifted;
	string name;
	ALLEGRO_BITMAP *image;

public:
	element();
	element(int x, int  y, int i, string n, ALLEGRO_BITMAP *im);
	void initElement(int x, int y, int i, ALLEGRO_BITMAP *picture, string type);
	void drawElement();
	void PrintInfo();
	bool clicked(int x, int y);
	void liftUp();
	bool isLifted();
	void move(int x, int y);
	void PutDown();
	int overlap(element x);
	int getX();
	int getY();
	string getName();
	int getIndex();


};

int checkCombo(element a, element b);

int main()
{
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *bouncer = NULL;
	float bouncer_x = SCREEN_W / 2.0 - BOUNCER_SIZE / 2.0;
	float bouncer_y = SCREEN_H / 2.0 - BOUNCER_SIZE / 2.0;
	bool redraw = true;

	al_init();
	al_init_image_addon();
	al_install_mouse();
	timer = al_create_timer(1.0 / FPS);
	display = al_create_display(SCREEN_W, SCREEN_H);
	bouncer = al_create_bitmap(BOUNCER_SIZE, BOUNCER_SIZE);

	al_set_target_bitmap(bouncer);

	al_clear_to_color(al_map_rgb(255, 0, 255));

	al_set_target_bitmap(al_get_backbuffer(display));

	event_queue = al_create_event_queue();

	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	al_register_event_source(event_queue, al_get_mouse_event_source());

	al_clear_to_color(al_map_rgb(255, 255, 255));

	al_flip_display();

	al_start_timer(timer);
	ALLEGRO_BITMAP *airPic = al_load_bitmap("air.jpg");
	ALLEGRO_BITMAP *waterPic = al_load_bitmap("water.png");
	ALLEGRO_BITMAP *firePic = al_load_bitmap("fire.png");
	ALLEGRO_BITMAP *earthPic = al_load_bitmap("earth.png");
	ALLEGRO_BITMAP *lavaPic = al_load_bitmap("lava.png");
	element air;
	element water;
	element fire;
	element earth;

	air.initElement(50, 50, 1, airPic, "air");
	water.initElement(300, 100, 2, waterPic, "water");
	fire.initElement(100, 400, 3, firePic, "fire");
	earth.initElement(200, 200, 4, earthPic, "earth");

	vector <element> items;
	vector <element> ::iterator i;
	vector <element> ::iterator j;


	items.push_back(earth);
	items.push_back(fire);
	items.push_back(air);
	items.push_back(water);

	// Integer constants for quick reference to element names.
	const int AIR = 1;
	const int WATER = 2;
	const int FIRE = 3;
	const int EARTH = 4;
	const int LAVA = 5;

	int elementToCreate = 0;

	while (1)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER) {
		
			//move item around if you've clicked on one
			for (i = items.begin(); i < items.end(); i++)
				if (i->isLifted())
					i->move(bouncer_x, bouncer_y);

			//check for items overlapping to combine
			for (i = items.begin(); i < items.end(); i++)

				//if(i>=items.end()) cout << "Oh no! i is outside the vector!" << endl; // Trace

				for (j = items.begin(); j < items.end(); j++)

					//if (j >= items.end()) cout << "Oh no! j is outside the vector!" << endl; // Trace

					if (i->overlap(*j))

						if (checkCombo(*i, *j) == 5) { // Don't just check the index, check the type of the element!
							cout << "creating lava" << endl; // Trace
							elementToCreate = LAVA; // Just using a placeholder value here; you may want to use constants with element name.
							//++i;  //Why would we iterate again; we're already iterating in the for loop expression
						}

						// NOTE! You need to add a way to dispose of the elements you "used up" as well! Otherwise, you have
						// infinite behavior!

			redraw = true;

			// Add new elements to the vector
			if(elementToCreate==LAVA) items.push_back(*(new element(bouncer_x, bouncer_y, 5, "lava", lavaPic)));

		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES ||
			ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {

			bouncer_x = ev.mouse.x;
			bouncer_y = ev.mouse.y;


		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			


			for (i = items.begin(); i < items.end(); i++)
				if (i->clicked(bouncer_x, bouncer_y))
					if (i->isLifted() == false)
						i->liftUp();
					else {
						i->PutDown();
					}
			

		}

		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {

		}


		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;

			al_clear_to_color(al_map_rgb(255, 255, 255));

			al_draw_bitmap(bouncer, bouncer_x, bouncer_y, 0);

			for (i = items.begin(); i < items.end(); i++)
				i->drawElement();


			al_flip_display();
		}
	}

	al_destroy_bitmap(bouncer);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}

int checkCombo(element a, element b) {
	if ((a.getIndex() == 3 && b.getIndex() == 4) || (a.getIndex() == 4 && b.getIndex() == 3))
		return 5;
	else
		return 0;
}
element::element() {
	
}

element::element(int x, int  y, int i, string n, ALLEGRO_BITMAP *im) {
	xpos=x;
	ypos=y;
	index=i;
	lifted = false;
	name=n;
	image=im;


}
void element::initElement(int x, int y, int i, ALLEGRO_BITMAP *picture, string type) {
	xpos = x;
	ypos = y;
	index = i;
	lifted = false;
	image = picture;
	name = type;
}
void element::drawElement() {
	al_draw_bitmap(image, xpos, ypos, 0);
}
void element::PrintInfo() {}

bool element::clicked(int x, int y) {

	//distance formula
	if (sqrt((((xpos + ELEMENTWIDTH / 2) - x)*((xpos + ELEMENTWIDTH / 2) - x)) + (((ypos + ELEMENTWIDTH / 2) - y)*((ypos + ELEMENTWIDTH / 2) - y))) < ELEMENTWIDTH / 2) {
		//cout << "element clicked!" << endl;
		return true;

	}
	else
		return false;


}

void element::liftUp() {
	lifted = true;

}

void element::PutDown() {
	lifted = false;

}

bool element::isLifted() {

	return lifted;
}

void element::move(int x, int y) {
	xpos = x - ELEMENTWIDTH / 2;
	ypos = y - ELEMENTWIDTH / 2;


}

int element::overlap(element x) {
	if (sqrt((((xpos + ELEMENTWIDTH / 2) - x.getX())*((xpos + ELEMENTWIDTH / 2) - x.getX())) + (((ypos + ELEMENTWIDTH / 2) - x.getY())*((ypos + ELEMENTWIDTH / 2) - x.getY()))) < ELEMENTWIDTH / 2) {
		cout << "overlap between " << name << " and " << x.getName()<< endl;
		return 1;
	}
	else
		return 0;

}

int element::getX() {

	return xpos;
}

int element::getY() {
	return ypos;
}

string element::getName() {

	return name;
}
int element::getIndex() {
	return index;
}
