#pragma once

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

typedef struct i_vector2d {
	int x, y;
} i_vector2d;

typedef struct f_vector2d {
	float x, y;
} f_vector2d;

typedef struct d_vector2d {
	long double x, y;
} d_vector2d;

class Box {
private:
	d_vector2d pos, size, vel;
	int collision_num = 0;
	double mass;
public:
	Box(d_vector2d pos, d_vector2d vel, d_vector2d size, double mass) {
		this->pos.x = pos.x;	this->pos.y = pos.y;
		this->size.x = size.x;	this->size.y = size.y;
		this->vel.x = vel.x;   this->vel.y = vel.y;
		this->mass = mass;
	}

	Box(double posx, double posy, double velx, double vely, double sizex, double sizey, double mass) {
		this->pos.x = posx;	this->pos.y = posy;
		this->size.x = sizex;	this->size.y = sizey;
		this->vel.x = velx;   this->vel.y = vely;
		this->mass = mass;
	}

	void update() {
		// Check wall collisions and add velocity to position
		pos.x += vel.x;
		if (pos.x < 0) {
			pos.x = 0;
			vel.x *= -1;
			this->incrementCollisionNum();
		}
		if (pos.x > 640 - size.x) {
			//pos.x = 640 - size.x;
			//vel.x *= -1;
			//this->incrementCollisionNum();
		}

		pos.y += vel.y;
		if (pos.y > 480 - size.y) {
			pos.y = 480 - size.y;
			vel.y *= -1;
			this->incrementCollisionNum();
		}
		if (pos.y < 0) pos.y = 0;
	}

	bool isColliding(Box& other) {
		if (this->pos.x + this->size.x > other.pos.x) {
			this->incrementCollisionNum();
			other.incrementCollisionNum();
			return true;
		}

		return false;
	}

	void render() {
		al_draw_filled_rectangle((float)pos.x, (float)pos.y, (float)pos.x + (float)size.x, (float)pos.y + (float)size.y, al_map_rgb_f(1, 1, 1));
	}

	// ----- GETTERS AND SETTERS -----
	d_vector2d getPos() { return this->pos; }
	d_vector2d getSize() { return this->size; }
	d_vector2d getVel() { return this->vel; }
	int getCollisionNum() { return this->collision_num; }
	double getMass() { return this->mass; }

	void setPos(d_vector2d pos) { this->pos = pos; }
	void setSize(d_vector2d size) { this->size = size; }
	void setVel(d_vector2d vel) { this->vel = vel; }
	void incrementCollisionNum() { this->collision_num++; }
};