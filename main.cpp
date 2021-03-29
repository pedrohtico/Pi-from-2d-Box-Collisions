#include <allegro5/allegro_font.h>
#include <iostream>
#include <string>

#include "Box.cpp"

void must_init(bool init, std::string description) {
	if (init) return;

	std::cout << "couldn't initialize " << description << std::endl;
	exit(1);
}

int main()
{
	// ----- INITIALIZATION -----
	must_init(al_init(), "allegro");
	must_init(al_install_keyboard(), "keyboard");

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 120.0);
	must_init(timer, "timer");

	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	must_init(queue, "queue");

	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
	ALLEGRO_DISPLAY* display = al_create_display(640, 480);
	must_init(display, "display");

	ALLEGRO_FONT* font = al_create_builtin_font();
	must_init(font, "font");

	must_init(al_init_primitives_addon(), "primitives addon");

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	// ----- MAIN LOOP -----
	bool done = false;
	bool redraw = true;
	ALLEGRO_EVENT event;
	int collisions = -1;

	Box* box1 = new Box(50, 475, 0, 0, 5, 5, 1);
	Box* box2 = new Box(100, 480, -.1, 0, 20, 20, 10000);

	al_start_timer(timer);
	while (1)
	{
		al_wait_for_event(queue, &event);

		switch (event.type)
		{
		case ALLEGRO_EVENT_TIMER:

			// UPDATE HERE
			box1->update();
			box2->update();

			if (box1->isColliding(*box2)) {
				d_vector2d aux = { 0, 0 };

				if (box2->getPos().x <= box1->getSize().x) {
					aux = box1->getPos();
					aux.x = 0;
					box1->setPos(aux);

					aux = box2->getPos();
					aux.x = box1->getSize().x;
					box2->setPos(aux);
				}

				if (box1->getPos().x + box1->getSize().x >= box2->getPos().x) {
					aux = box2->getPos();
					aux.x -= box1->getSize().x;
					aux.y = box1->getPos().y;
					box1->setPos(aux);
				}

				double v1 = box1->getVel().x;
				double v2 = box2->getVel().x;
				double m1 = box1->getMass();
				double m2 = box2->getMass();

				aux.y = 0;
				aux.x = ((m1 - m2) * v1 + (2 * m2) * v2) / (m2 + m1);
				box1->setVel(aux);

				aux.x = ((m2 - m1) * v2 + (2 * m1 * v1)) / (m2 + m1);
				box2->setVel(aux);
			}

			if (box1->getCollisionNum() > 0 && box1->getCollisionNum() != collisions) {
				collisions = box1->getCollisionNum();
				std::cout << "Collisions: " << collisions << std::endl;
			}

			redraw = true;
			break;

			//case ALLEGRO_EVENT_KEY_DOWN:
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			done = true;
			break;

		default:
			break;
		}

		if (done) break;

		// ----- RENDERING -----
		if (redraw && al_is_event_queue_empty(queue))
		{
			al_clear_to_color(al_map_rgb(0, 0, 0));

			// RENDER HERE
			box1->render();
			box2->render();

			al_flip_display();
			redraw = false;
		}
	}

	// ----- TERMINATION -----
	al_destroy_font(font);
	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);

	return 0;
}