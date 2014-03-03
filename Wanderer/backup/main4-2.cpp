#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stddef.h>
#include <string>

using namespace std;

/*		0
 *		|
 *	3-------1
 *		|
 *		2
 */


//Bullet class
class Bullet {
private:

	sf::Sprite * bullet_s;
	float x;
	float y;
	float target_x, target_y;
	float direction;

public:
	int position;
	int count;
	bool remove;

	Bullet() {
		count = 0;
		position = 0;
		x = 0;
		y = 0;
		remove = false;
		target_x = 0;
		target_y = 0;
		bullet_s = new sf::Sprite();
	}
	Bullet(float _x, float _y, sf::Texture _bullet_t) {
		count = 0;
		position = 0;
		x = _x+8;
		y = _y+8;
		bullet_s = new sf::Sprite();
		bullet_s->setTexture(_bullet_t);
		remove = false;
		target_x = 0;
		target_y = 0;
	}
	int get_x() {
		return int(x);
	}
	int get_y() {
		return int(y);
	}
	void set_target(float _x, float _y) {
		target_x = x - (_x);
		target_y = y - (_y);
		direction = atan(abs(target_x)/abs(target_y));
	}
	void move() {
		if (target_x > 0) {
			x -= (1 * sin(direction));
		}
		else {
			x += (1 * sin(direction));
		}
		if (target_y > 0) {
			y -= (1 * cos(direction));
		}
		else {
			y += (1 * cos(direction));
		}
	}
	void act(int _grid[80][80]) {
		for (int i = 0; i < 2; i++) {
			move();
			if (_grid[int(x/16)][int(y/16)] != 0) {
				remove = true;
			}
		}
		bullet_s->setPosition(x, y);
		if (count > 240) {
			remove = true;
		}
		count++;
	}
	sf::Sprite * get_sprite() {
		return bullet_s;
	}
};

//Bullet Array class.
class Bullet_Array {
	Bullet * values[4];
	int size;
public:
	Bullet_Array() {
		size = 0;
		for (int i = 0; i < 4; i++) {
			values[i] = NULL;
		}
	}
	int add_bullet(Bullet * new_bullet) {
		for (int i = 0; i < 4; i++) {
			if (values[i] == NULL) {
				values[i] = new Bullet();
				values[i] = new_bullet;
				values[i]->position = size;
				size++;
				break;
			}
		}
		return (size-1);
	}

	Bullet * get_Bullet(int i) {
		return values[i];
	}

	int get_size() {
		return size;
	}
	void clear() {
		for (int i = 0; i < size; i++){
			values[i] = NULL;
		}
		size = 0;
	}
	void remove(int i) {
		for (int k = i; k < size; k++){
			values[k] = NULL;
			if (values[k+1] != NULL) {
				values[k] = values[k+1];
			}
		}
		size--;
	}
};

//Block class
class block {
public:
	float x;
	float y;
	sf::Sprite * sprite;

	block() {
		sprite = new sf::Sprite;
		sprite->setPosition(0, 0);
		x = 0;
		y = 0;
	}

	block(int _x, int _y) {
		sprite = new sf::Sprite;
		sprite->setPosition(float(_x), float(_y));
		x = float(_x);
		y = float(_y);
	}

	block(sf::Texture * _texture, int _x, int _y) {
		sprite = new sf::Sprite;
		sprite->setPosition(float(_x), float(_y));
		sprite->setTexture(*_texture);
		x = float(_x);
		y = float(_y);
	}

	sf::Sprite * get_sprite() {
		return sprite;
	}

};

//Block Array class.
class block_array {
	int values[6400][3];
	int size;
public:
	block_array() {
		size = 0;
		for (int i = 0; i < 6400; i++) {
			values[i][0] = 0;
		}
	}
	void add_block(int new_pos[3]) {
		for (int i = 0; i < 6400; i++) {

			if (values[i][0] == 0) {
				values[i][0] = new_pos[0];
				values[i][1] = new_pos[1];
				values[i][2] = new_pos[2];
				size++;
				break;
			}
		}
	}
	int * get_block(int i) {
		return values[i];
	}
	int get_size() {
		return size;
	}
	void clear() {
		for (int i = 0; i < size; i++){
			values[i][0] = 0;
			values[i][1] = 0;
			values[i][2] = 0;
		}
		size = 0;
	}
};

class Entity {
private:
public:
	bool los;
	float x;
	float y;
	int position;
	Entity() {
		x = 0;
		y = 0;
		position = 0;
		los = false;
	}
	void set_los(int _player_x, int _player_y, int _grid[80][80]) {
		bool should_break = false;
		float _x1;
		float _y1;
		float _x2;
		float _y2;
		float view_ratio;
		_x2 = x+7;
		_y2 = y+7;
		_x1 = (float(_player_x) - (x));
		_y1 = (float(_player_y) - (y));
		if ((int(abs(_x1)) <= 32) && (int(abs(_y1)) <= 32)) {
			los = true;
		}
		else {
			view_ratio = atan(_y1/_x1);
			abs(view_ratio);
			for (int k = 0; k < (sqrt((_x1*_x1) + (_y1*_y1))); k++) {
				if (_x1 < 0) {
					_x2 -= abs(cos(view_ratio));
				}
				else {
					_x2 += abs(cos(view_ratio));
				}
				if (_y1 < 0){
					_y2 -= abs(sin(view_ratio));
				}
				else {
					_y2 += abs(sin(view_ratio));
				}
				if ((_x2 < 0) || (_y2 < 0)) {
					should_break = true;
					break;
				}
				if (_grid[int(_x2)/16][int(_y2)/16] == 1) {
					los = false;
					break;
				}
				else {
					los = true;
				}
			}
		}
	}
};

class Entity_Array {
	Entity * values[64];
	int size;
public:
	Entity_Array() {
		size = 0;
		for (int i = 0; i < 64; i++) {
			values[i] = NULL;
		}
	}
	int add_entity(Entity * new_entity) {
		for (int i = 0; i < 64; i++) {
			if (values[i] == NULL) {
				values[i] = new Entity();
				values[i] = new_entity;
				values[i]->position = size;
				size++;
				break;
			}
		}
		return (size-1);
	}

	Entity * get_Entity(int i) {
		return values[i];
	}

	int get_size() {
		return size;
	}
	void clear() {
		for (int i = 0; i < size; i++){
			values[i] = NULL;
		}
		size = 0;
	}
};

class Cache : public Entity {
private:

public:
	bool food;
	bool ammo;
	bool in_range;
	sf::Texture cache_t;
	sf::Sprite cache_s;

	Cache() {
		x = rand() % 3;
		in_range = false;
		if (x == 0) {
			ammo = false;
			food = true;
			cache_t.loadFromFile("images/entities/interactable/cache_food.png");
		}
		else if (x == 1) {
			ammo = true;
			food = false;
			cache_t.loadFromFile("images/entities/interactable/cache_ammo.png");
		}
		x = 0;
		y = 0;
		los = false;
		cache_s.setTexture(cache_t);
		cache_s.setPosition(x, y);
	}
	Cache(float _x, float _y) {
		x = rand() % 3;
		in_range = false;
		if (x == 0) {
			ammo = false;
			food = true;
			cache_t.loadFromFile("images/entities/interactable/cache_food.png");
		}
		else if (x > 0) {
			ammo = true;
			food = false;
			cache_t.loadFromFile("images/entities/interactable/cache_ammo.png");
		}
		x = _x;
		y = _y;
		los = false;
		cache_s.setTexture(cache_t);
		cache_s.setPosition(x, y);
	}
	sf::Sprite get_sprite() {
		return cache_s;
	}
	int get_x() {
		return int(x);
	}
	int get_y() {
		return int(y);
	}
	void within_range(bool _in_range) {
		in_range = _in_range;
	}
	bool should_exist() {
		if (food || ammo) {
			return true;
		}
		return false;
	}
	void act() {
		if (los && (!in_range)) {
					cache_s.setColor(sf::Color::White);
		}
		else if (los && in_range) {
					cache_s.setColor(sf::Color(255,255,0));
		}
		else {
					cache_s.setColor(sf::Color::Transparent);
		}
	}
};

class Cache_Array {
	Cache * values[4];
	int size;
public:
	Cache_Array() {
		size = 0;
		for (int i = 0; i < 4; i++) {
			values[i] = NULL;
		}
	}
	int add_cache(Cache * new_cache) {
		for (int i = 0; i < 4; i++) {
			if (values[i] == NULL) {
				values[i] = new Cache();
				values[i] = new_cache;
				values[i]->position = size;
				size++;
				break;
			}
		}
		return (size-1);
	}

	Cache *get_cache(int i) {
		return values[i];
	}

	int get_size() {
		return size;
	}
	void clear() {
		for (int i = 0; i < 4; i++){
			values[i] = NULL;
		}
		size = 0;
	}
	void remove(int i) {
		delete values[i];
		values[i] = NULL;
		for (int k = i; k < size; k++){
			if (values[k+1] != NULL) {
				values[k] = values[k+1];
			}
		}
		size--;
		if (values[i] == NULL) {
		}
	}
};

class Zombie : public Entity {
private:

	sf::Sprite * zom_s;
	sf::Clock sprite_timer;
	sf::Clock target_timer;
	sf::Time last_target;
	sf::Time sprite_change;
	int target_time;
	Entity * target;
	float direction;
	sf::Texture zom_td0;
	sf::Texture zom_td1;
	bool x_stuck;
	bool y_stuck;
	sf::Clock hit_clock;
	sf::Time hit_timer;


public:
	int position;
	int health;
	bool remove;

	Zombie() {
		los = false;
		zom_td0.loadFromFile("images/entities/ghouls/zom-d-0.png");
		zom_td1.loadFromFile("images/entities/ghouls/zom-d-1.png");
		x_stuck = false;
		y_stuck = false;
		zom_s = new sf::Sprite;
		x = 0;
		y = 0;
		zom_s->setPosition(x,y);
		target = NULL;
		direction = 0;
		zom_s->setTexture(zom_td0);
		sprite_change = sprite_timer.restart();
		last_target = target_timer.restart();
		hit_timer = hit_clock.restart();
		target_time = 0;
		health = 5;
		remove = false;
	}

	Zombie(int _x, int _y) {
		los = false;
		zom_td0.loadFromFile("images/entities/ghouls/zom-d-0.png");
		zom_td1.loadFromFile("images/entities/ghouls/zom-d-1.png");
		x_stuck = false;
		y_stuck = false;
		direction = 0;
		zom_s = new sf::Sprite;
		x = float(_x);
		y = float(_y);
		zom_s->setPosition(x,y);
		zom_s->setTexture(zom_td0);
		target = NULL;
		sprite_change = sprite_timer.restart();
		last_target = target_timer.restart();
		hit_timer = hit_clock.restart();
		target_time = 0;
		health = 5;
		remove = false;
	}
	sf::Sprite * get_sprite() {
		return zom_s;
	}
	int get_x() {
		return int(x);
	}
	int get_y() {
		return int(y);
	}
	
	void hit(int i) {
		health -= 1;
		zom_s->setColor(sf::Color(255, 155, 155));
		hit_timer = hit_clock.restart();
	}

	void set_target(Entity_Array _entitylist) {
		last_target = target_timer.getElapsedTime();
		for (int i = 0; i < _entitylist.get_size(); i++) {
			if ((_entitylist.get_Entity(i)->x < x+(6*16)) && (_entitylist.get_Entity(i)->x > x-(6*16)) && (_entitylist.get_Entity(i)->y < y+(6*16)) && (_entitylist.get_Entity(i)->y > y-(6*16))) {
				if (target == NULL) {
					target = new Entity();
				}
				target = _entitylist.get_Entity(i);
				target_time = int(last_target.asSeconds());
				break;
			}
			else if ((los == true) &&(_entitylist.get_Entity(i)->x < x+(14*16)) && (_entitylist.get_Entity(i)->x > x-(14*16)) && (_entitylist.get_Entity(i)->y < y+(14*16)) && (_entitylist.get_Entity(i)->y > y-(14*16))) {
				if (target == NULL) {
					target = new Entity();
				}
				target = _entitylist.get_Entity(i);
				target_time = int(last_target.asSeconds());
				break;
			}
			else {
				if ((last_target.asSeconds() - target_time) > 2) {
					target = NULL;
				}

			}
		}
	}

	void move(int _grid[80][80]) {
		sprite_change = sprite_timer.getElapsedTime();
		float target_x = x;
		float target_y = y;
		if (target != NULL) {
			if (sprite_change.asSeconds() <= 0.4) {
				zom_s->setTexture(zom_td0);
			}
			else if (sprite_change.asSeconds() <= 0.8) {
				zom_s->setTexture(zom_td1);
			}
			else {
				sprite_change = sprite_timer.restart();
			}
			target_x = (x - target->x);
			target_y = (y - target->y);
			direction = atan(abs(target_x)/abs(target_y));
			if (target_x > 0) {
				target_x = x;
				if ((_grid[(int(x-1)/16)][int(y)/16] == 0) && (_grid[(int(x-1)/16)][int(y+15)/16] == 0)) {
					if (!(y_stuck)) {
						target_x -= float(0.8 * sin(direction));
					}
					else {
						target_x -= float(0.8);
					}
					x_stuck = false;
				}
				else {
					x_stuck = true;
				}
			}
			else {
				target_x = x;
				if ((_grid[(int(x+16)/16)][int(y)/16] == 0) && (_grid[(int(x+16)/16)][int(y+15)/16] == 0)) {
					if (!(y_stuck)) {
						target_x += float(0.8 * sin(direction));
					}
					else {
						target_x += float(0.8);
					}
					x_stuck = false;
				}
				else {
					x_stuck = true;
				}
				
			}
			if (target_y > 0) {
				target_y = y;
				if ((_grid[(int(x)/16)][int(y-1)/16] == 0) && (_grid[(int(x+15)/16)][int(y-1)/16] == 0)) {
					if (!(x_stuck)) {
						target_y -= float(0.8 * cos(direction));
					}
					else {
						target_y -= 0.8;
					}
						y_stuck = false;
				}
				else {
					y_stuck = true;
				}
			}
			else {
				target_y = y;
				if ((_grid[(int(x)/16)][int(y+16)/16] == 0) && (_grid[(int(x+15)/16)][int(y+16)/16] == 0)) {
					if (!(x_stuck)) {
						target_y += float(0.8 * cos(direction));
					}
					else {
						target_y += 0.8;
						y_stuck = false;
					}
				}
				else {
					y_stuck = true;
				}
			}

		}
		else {
		}
		x = target_x;
		y = target_y;
		zom_s->setPosition(x,y);
	}
	void act() {
		hit_timer = hit_clock.getElapsedTime();
		if (health <= 0) {
			remove = true;
		}
		if (los && (hit_timer.asSeconds() > 0.1)) {
			zom_s->setColor(sf::Color::White);
		}
		else if (los && (hit_timer.asSeconds() < 0.1)) {
			zom_s->setColor(sf::Color(255, 155, 155));
		}
		else {
			zom_s->setColor(sf::Color::Transparent);
		}
	}
};

class Zombie_Array {
	Zombie * values[64];
	int size;
public:
	Zombie_Array() {
		size = 0;
		for (int i = 0; i < 64; i++) {
			values[i] = NULL;
		}
	}
	int add_zombie(Zombie new_zombie) {
		for (int i = 0; i < 64; i++) {
			if (values[i] == NULL) {
				values[i] = new Zombie();
				*values[i] = new_zombie;
				values[i]->position = size;
				size++;
				break;
			}
		}
		return (size-1);
	}

	Zombie * get_Zombie(int i) {
		return values[i];
	}

	int get_size() {
		return size;
	}
	void clear() {
		for (int i = 0; i < size; i++){
			values[i] = NULL;
		}
		size = 0;
	}
	void remove(int i) {
		values[i] = NULL;
		for (int k = i; k < size; k++){
			if (values[k+1] != NULL) {
				values[k] = values[k+1];
			}
		}
		size--;
	}
};

//Controller class
class controller {
private:
	Entity * player;
	sf::Texture bullet_t;
	int meta_x, meta_y;

public:
	float player_x;
	float player_y;
	int grid[80][80];
	block_array blocklist;
	Zombie_Array zombielist;
	Entity_Array entitylist;
	Bullet_Array bulletlist;
	Cache_Array cachelist;
	bool edge;

	controller() {
		bullet_t.loadFromFile("images/entities/bullet.png");
		player = NULL;
		edge = false;
		player_x = 0;
		player_y = 0;
		for (int i = 0; i < 80; i++) {
			for (int k = 0; k < 80; k++) {
				grid[i][k] = 0;
			}
		}
		gen_map();
		meta_x = 0;
		meta_y = 0;
	}

	void save_map() {
		ofstream sav;
		char file_name[256];
		string output;
		sprintf(file_name, "%i_%i.wap", meta_x, meta_y);
		sav.open(file_name);
		for (int i = 0; i < 80; i++) {
            char temp[256];
			for (int k = 0; k < 80; k++) {
				sprintf(temp, "%i:\n", grid[k][i]);
                output += temp;
			}
			output += ";";
		}
        sav << output;
        sav.close();
	}

	void load_map() {
		string line = "";
		char file_name[256];
		printf("%i\n", meta_x);
		sprintf(file_name, "%i_%i.wap", meta_x, meta_y);
        ifstream sav (file_name);
        int i = 0;
		int k = 0;
        if (sav.is_open()) {
            int i = 0;
            while (sav.good()) {
                getline (sav, line);
                int firstColon = line.find_first_of(":");
                if (firstColon != -1) {
					string temp = line.substr(0, 1);
                    if ( temp == "1" ) {
						add_block(i, k, (rand() % 6));
					}
					else if (temp == ";") {
						k++;
						i = 0;
					}
					temp = line.substr(0, 1);
					i++;
                }
            }
            sav.close();
        }
		else {
			gen_map();
		}
	}

	//Function to generate 80x80 chunk.
	void gen_map() {
		int random_walls = 0;
		int stored_walls = 0;
		int sprite_choice = 0;
		int gen_house = 0;
		for (int j = 0; j < 8; j++) {
			for (int k = 0; k < 8; k++) {
				gen_house = rand() % 6;
				if (gen_house < 4) {
					for (int i = gen_house; i < 8; i++) {
						for (int ik = gen_house; ik < 8; ik++) {
							random_walls = rand() % 9;
							if (((i == int(gen_house)) || (i ==  7)) || ((ik == int(gen_house)) ||(ik ==  7))) {
								random_walls = rand() % 15;
							}
							if (stored_walls > 7) { 
								random_walls+=gen_house;
							}
							if (random_walls > 7) {
								sprite_choice = rand() % 6;
								add_block(1+i+(j*8), ik+(1+k*8), sprite_choice);
							}
							if (random_walls == 1) {
								random_walls = rand() % 105;
								if (random_walls == 0) {
									add_cache(1+i+(j*8), ik+(1+k*8));
								}
							}
							else if (random_walls == 0) {
								random_walls = rand() % 20;
								if (random_walls == 0) {
									add_zombie(1+i+(j*8), ik+(1+k*8));
								}
							}
							stored_walls = random_walls;
						}
					}
				}
			}
		}
	}

	bool request_move(float _x, float _y) {
		if ((grid[(int(_x/16))][(int(_y/16))] || grid[((int(_x+15)/16))][int((_y/16))] || grid[int((_x/16))][int(((_y+15)/16))] || grid[int(((_x+15)/16))][int(((_y+15)/16))]) != 0) {
			return false;
		}
		return true;
	}
	//---Beginning of Player handling functions.

	void add_player(Entity * _player) {
		player = _player;
	}

	void set_position(float _x, float _y) {
		player_x = _x;
		player_y = _y;
	}

	//Checks for collision with a zombie.
	bool get_zom_collision(int _x, int _y) {
		for (int i = 0; i < zombielist.get_size(); i++) {
			if ((zombielist.get_Zombie(i)->get_x() < _x+15) && (zombielist.get_Zombie(i)->get_x() > _x-15) && (zombielist.get_Zombie(i)->get_y() < _y+15) && (zombielist.get_Zombie(i)->get_y() > _y-15)) {
				return true;
			}
		}
		return false;
	}

	void add_existing_entity(Entity * _entity) {
		entitylist.add_entity(_entity);
		_entity->position = entitylist.get_size();
	}

	//End of Player handling functions.

	//Beginning of block handling functions.

	//Adds a block to the block_array.
	void add_block(int _x, int _y, int texture_index) {
		int new_pos[3] = {_x, _y, texture_index};
		blocklist.add_block(new_pos);
		grid[_x][_y] = 1;
	}

	//Gets the size of the block_array.
	int get_blocklist_size(int chunk) {
		return blocklist.get_size();
	}

	//Gets and returns the sprite of the block at position i.
	int * get_block(int i, int chunk) {
			return blocklist.get_block(i);
	}
	//End of block handling functions.

	//Beginning of zombie handling functions

	//Adds a zombie to the Zombie Array.
	void add_zombie(int _x, int _y) {
		Zombie * new_zombie = new Zombie((_x*16), (_y*16));
		zombielist.add_zombie(*new_zombie);
		//grid[_x][_y] = 2;
	}
	
	//Gets the size of the Zombie_Array.
	int get_zombielist_size() {
		return zombielist.get_size();
	}

	//Gets and returns the sprite of the zombie at position i.
	sf::Sprite get_zombie_sprite(int i) {
			return *zombielist.get_Zombie(i)->get_sprite();
	}

	//End of zombie handling functions.

	void add_bullet(float _x, float _y, sf::Vector2i * mouse_pos){
		Bullet * _bullet = new Bullet(_x, _y, bullet_t);
		_bullet->set_target(float((((mouse_pos->x) - 320) + _x)), float((((mouse_pos->y) - 240) +_y)));
		bulletlist.add_bullet(_bullet);
	}

	int get_bulletlist_size() {
		return bulletlist.get_size();
	}
	
	sf::Sprite get_Bullet_sprite(int i) {
			return *bulletlist.get_Bullet(i)->get_sprite();
	}

	void add_cache(float _x, float _y){
		Cache * _cache = new Cache(_x*16, _y*16);
		cachelist.add_cache(_cache);
	}

	int get_cachelist_size() {
		return cachelist.get_size();
	}
	
	sf::Sprite get_cache_sprite(int i) {
			return cachelist.get_cache(i)->get_sprite();
	}

	bool interact_with_cache(int _x, int _y) {
		for (int i = 0; i < cachelist.get_size(); i++) {
			if ((abs(cachelist.get_cache(i)->x - (_x)) < 23) && (abs(cachelist.get_cache(i)->y - (_y)) < 23)) {
				cachelist.get_cache(i)->within_range(1);
				return true;
			}
			cachelist.get_cache(i)->within_range(0);
		}
		return false;
	}

	void act() {
		for (int i = 0; i < bulletlist.get_size(); i++) {
			bool _break = false;
			for (int j = 0; j < 8; j++) {
				bulletlist.get_Bullet(i)->act(grid);
				for (int k = 0; k < zombielist.get_size(); k++) {
					if ((zombielist.get_Zombie(k)->get_x() < (bulletlist.get_Bullet(i)->get_x()-2)) && ((zombielist.get_Zombie(k)->get_x()+15) > bulletlist.get_Bullet(i)->get_x()) && (zombielist.get_Zombie(k)->get_y() < (bulletlist.get_Bullet(i)->get_y()-2)) && ((zombielist.get_Zombie(k)->get_y()+15) > bulletlist.get_Bullet(i)->get_y())) {
							bulletlist.remove(i);
							zombielist.get_Zombie(k)->hit(k);
							_break = true;
							break;
					}
				}
				if (_break == true) {
					break;
				}
			}
			if (_break == true) {
				break;
			}
			if (bulletlist.get_Bullet(i)->remove == true) {
				bulletlist.remove(i);
			}
		}
		for (int i = 0; i < zombielist.get_size(); i++) {
			zombielist.get_Zombie(i)->set_los(int(player_x), int(player_y), grid);
			zombielist.get_Zombie(i)->act();
			if (zombielist.get_Zombie(i)->remove == true) {
				zombielist.remove(i);
				break;
			}
			zombielist.get_Zombie(i)->set_target(entitylist);
			zombielist.get_Zombie(i)->move(grid);
		}
		interact_with_cache(player_x, player_y);
		for (int i = 0; i < cachelist.get_size(); i++) {
			if (!(cachelist.get_cache(i)->should_exist())) {
				cachelist.remove(i);
				break;
			}
			cachelist.get_cache(i)->set_los(int(player_x), int(player_y), grid);
			cachelist.get_cache(i)->act();
		}
		if (player_x < 0) {
			save_map();
			meta_x--;
			edge = true;
			player_x = 1040;
		}
		if (player_x > 1040) {
			save_map();
			meta_x++;
			edge = true;
			player_x = 0;
		}
		if (player_y < 0) {
			save_map();
			meta_y--;
			edge = true;
			player_y = 1040;
		}
		if (player_y > 1040) {
			save_map();
			meta_y++;
			edge = true;
			player_y = 0;
		}
		if (edge == true) {
			blocklist.clear();
			zombielist.clear();
			entitylist.clear();
			cachelist.clear();
			for (int i = 0; i < 80; i++) {
				for (int k = 0; k < 80; k++) {
					grid[i][k] = 0;
				}
			}
			load_map();
			add_existing_entity(player);
		}
	}

};

//Player class is defined here.
class Sam : public Entity {

	private:
		
		sf::Color colour;
		sf::Sprite sam_s;
		sf::Texture sam_td0;
		sf::Texture sam_td1;
		sf::Texture sam_tr0;
		sf::Texture sam_tr1;
		sf::Texture sam_tl0;
		sf::Texture sam_tl1;
		sf::Texture sam_tu0;
		sf::Texture sam_tu1;
		int health;
		sf::Clock internal_clock;
		sf::Time int_elapsed;
		sf::Clock gun_clock;
		sf::Time gun_timer;
		sf::Time sprite_change;
		sf::Clock sprite_timer;
		sf::Vector2i * mouse_pos;

	public:
		int hunger;
		int ammo;

		//Initialize Sam with no argument
		Sam() {
			printf("Creating Sam\n");
			sam_td0.loadFromFile("images/entities/sam/sam-d-0.png");
			sam_td1.loadFromFile("images/entities/sam/sam-d-1.png");
			sam_tu0.loadFromFile("images/entities/sam/sam-u-0.png");
			sam_tu1.loadFromFile("images/entities/sam/sam-u-1.png");
			sam_tr0.loadFromFile("images/entities/sam/sam-r-0.png");
			sam_tr1.loadFromFile("images/entities/sam/sam-r-1.png");
			sam_tl0.loadFromFile("images/entities/sam/sam-l-0.png");
			sam_tl1.loadFromFile("images/entities/sam/sam-l-1.png");
			colour = sf::Color::White;
			x = 0;
			y = 0;
			health = 100;
			position = 0;
			sf::Time int_elapsed = internal_clock.restart();
			sf::Time sprite_change = sprite_timer.restart();
			sf::Time gun_timer = gun_clock.restart();
			hunger = 50000;
			ammo = 20;

		}

		//Initialize Sam with texture argument
		Sam(sf::Vector2i * _mouse_pos) {

			printf("Creating Sam\n");
			sam_td0.loadFromFile("images/entities/sam/sam-d-0.png");
			sam_td1.loadFromFile("images/entities/sam/sam-d-1.png");
			sam_tu0.loadFromFile("images/entities/sam/sam-u-0.png");
			sam_tu1.loadFromFile("images/entities/sam/sam-u-1.png");
			sam_tr0.loadFromFile("images/entities/sam/sam-r-0.png");
			sam_tr1.loadFromFile("images/entities/sam/sam-r-1.png");
			sam_tl0.loadFromFile("images/entities/sam/sam-l-0.png");
			sam_tl1.loadFromFile("images/entities/sam/sam-l-1.png");
			x = 0;
			y = 0;
			health = 100;
			sam_s.setTexture(sam_td0);
			colour = sf::Color::White;
			sam_s.setColor(colour);
			sf::Time int_elapsed = internal_clock.restart();
			sf::Time sprite_change = sprite_timer.restart();
			sf::Time gun_timer = gun_clock.restart();
			mouse_pos = _mouse_pos;
			hunger = 50000;
			ammo = 20;
		}

		//Draw Sam
		sf::Sprite get_sprite() {

			return sam_s;

		}

		int get_health() {
			return health;
		}
		
		void get_action(controller& _controller) {

			int_elapsed = internal_clock.getElapsedTime();
			sf::Time sprite_change = sprite_timer.getElapsedTime();
			gun_timer = gun_clock.getElapsedTime();
			if (_controller.edge == true) {
				x = _controller.player_x;
				y = _controller.player_y;
				_controller.edge = false;
			}
			_controller.set_position(x, y);

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				if (ammo > 0) {
					if (gun_timer.asSeconds() > 0.4) {
						_controller.add_bullet(x, y, mouse_pos);
						gun_timer = gun_clock.restart();
						ammo--;
					}
				}
			}
			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				// space key is pressed: interact!
				if (_controller.interact_with_cache(x, y)) {
					if (_controller.cachelist.get_cache(0)->food) {
						hunger += 12500;
						if (hunger > 50000) {
							hunger = 50000;
						}
						_controller.cachelist.get_cache(0)->food = false;
					}
					if (_controller.cachelist.get_cache(0)->ammo) {
						ammo += 14;
						_controller.cachelist.get_cache(0)->ammo = false;
					}
				}
				
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				// left key is pressed: move our character
				if (_controller.request_move(x + 1, y)) {
					x += 1;
					if (sprite_change.asSeconds() <= 0.2) {
						sam_s.setTexture(sam_tr0);
					}
					else if (sprite_change.asSeconds() <= 0.4) {
						sam_s.setTexture(sam_tr1);
					}
					else {
						sprite_change = sprite_timer.restart();
					}
				}
				
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				// left key is pressed: move our character
				if (_controller.request_move(x - 1, y)) {
					x -= 1;
					if (sprite_change.asSeconds() <= 0.2) {
						sam_s.setTexture(sam_tl0);
					}
					else if (sprite_change.asSeconds() <= 0.4) {
						sam_s.setTexture(sam_tl1);
					}
					else {
						sprite_change = sprite_timer.restart();
					}
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				// left key is pressed: move our character
				if (_controller.request_move(x, y - 1)) {
					y -= 1;
					if (sprite_change.asSeconds() <= 0.2) {
						sam_s.setTexture(sam_tu0);
					}
					else if (sprite_change.asSeconds() <= 0.4) {
						sam_s.setTexture(sam_tu1);
					}
					else {
						sprite_change = sprite_timer.restart();
					}
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				// left key is pressed: move our character
				if (_controller.request_move(x, y + 1)) {
					y += 1;
					if (sprite_change.asSeconds() <= 0.2) {
						sam_s.setTexture(sam_td0);
					}
					else if (sprite_change.asSeconds() <= 0.4) {
						sam_s.setTexture(sam_td1);
					}
					else {
						sprite_change = sprite_timer.restart();
					}
				}
			}
			sam_s.setPosition(x,y);
			int hurt = false;
			if (_controller.get_zom_collision(int(x), int(y))) {
				if (int_elapsed.asSeconds() > 1) {
					health -= 20;
					sam_s.setColor(sf::Color(255, 155, 155));
					int_elapsed = internal_clock.restart();
					hurt = true;
				}
			}
			if ((hurt == false) && (int_elapsed.asSeconds() > 0.1)) {
				colour = sf::Color::White;
				sam_s.setColor(colour);
			}
			hunger--;
		}
};

int main()
{
	char ver[256] = "0.0.1";
	printf("Wanderer ver: %s\n", ver);
	printf("Initializing\n");

	//Initialize fonts.
	sf::Text fps_counter;
	sf::Text player_info;
	sf::Font arial;
	arial.loadFromFile("fonts/arial.ttf");
	fps_counter.setFont(arial);
	fps_counter.setCharacterSize(16);
	fps_counter.setColor(sf::Color::Red);
	player_info.setFont(arial);
	player_info.setCharacterSize(16);
	player_info.setColor(sf::Color::Red);
	player_info.setPosition(620, 0);

	//Initialize clock.
	printf("Starting clock\n");
	sf::Clock clock;
	char fps_s[256];
	int fps;
	double game_speed;
	game_speed = 0.0167;

	//initialize random.
	printf("Seeding random number generator\n");
	srand (time(NULL));
	
	//Load textures.
	printf("Loading textures\n");
	sf::Texture sam_t;
	sf::Texture wal_t[6];
	sf::Texture zom_t;
	sf::Texture bg_t;
	sf::Texture help_t;
	sam_t.loadFromFile("images/entities/sam/sam-d-0.png");
	wal_t[0].loadFromFile("images/static/walls/wal0.png");
	wal_t[1].loadFromFile("images/static/walls/wal1.png");
	wal_t[2].loadFromFile("images/static/walls/wal2.png");
	wal_t[3].loadFromFile("images/static/walls/wal3.png");
	wal_t[4].loadFromFile("images/static/walls/wal4.png");
	wal_t[5].loadFromFile("images/static/walls/wal5.png");
	zom_t.loadFromFile("images/entities/ghouls/zom.png");
	bg_t.loadFromFile("images/static/bg.png");
	sf::Sprite bg_s;
	bg_s.setTexture(bg_t);
	help_t.loadFromFile("images/ui/help screen.png");
	sf::Sprite help_s;
	help_s.setTexture(help_t);

	//Create Player entity.
	sf::Vector2i mouse_pos;
	Sam the_sam = Sam(&mouse_pos);
	sf::Sprite wal_s;
	wal_s.setTexture(wal_t[0]);

	//initialize controller.
	controller game_controller;
	game_controller.add_player(&the_sam);
	game_controller.add_existing_entity(&the_sam);

	//initialize walls

	//initialize Zombies

	//Initializing window.
	printf("Opening window\n");
	char title[256];
	sprintf_s(title,"Wanderer %s", ver);
    sf::RenderWindow window(sf::VideoMode(640, 480), title);

	//initializing view
	sf::View view;
	view.reset(sf::FloatRect(0, 0, 640, 480));
	window.setView(view);


	printf("Running Wanderer\n");
	bool help;
	bool has_helped = false;
	bool game_over = false;
    while (window.isOpen())
    {
		help = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1)) {
			help = true;
			has_helped = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))  {
			game_over = true;
		}
		if (the_sam.get_health() <= 0) {
			game_over = true;
		}
		//Set up FPS counter.
		sf::Time elapsed = clock.restart();
		fps = int(1/elapsed.asSeconds());
		//Lock game speed.
		if (elapsed.asSeconds() < game_speed) {
			sf::Time sleep_time = sf::seconds(float(game_speed - elapsed.asSeconds()));
			sf::sleep(sleep_time);
			fps = int(1/(sleep_time.asSeconds()+elapsed.asSeconds()));
		}
		sprintf_s(fps_s, "%i", fps);
		fps_counter.setString(fps_s);

		char hunger_s[256];
		sprintf_s(hunger_s, "Hunger: %i", the_sam.hunger/500);
		char ammo_s[256];
		sprintf_s(ammo_s, "Ammo: %i", the_sam.ammo);



        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

		
		mouse_pos = sf::Mouse::getPosition(window);
		
		if (!help) {
			the_sam.get_action(game_controller);
			view.reset(sf::FloatRect(the_sam.x-320, the_sam.y-240, 640, 480));
			fps_counter.setPosition(the_sam.x+300, the_sam.y-240);
			window.setView(view);
			game_controller.act();
		}

		char health_s[256];
		sprintf_s(health_s, "HP: %i", the_sam.get_health());
        window.clear();
		window.draw(bg_s);
		//Draw everything beginning here.

		for(int i = 0; i < game_controller.get_zombielist_size(); i++) {
			window.draw(game_controller.get_zombie_sprite(i));
		}
		for(int i = 0; i < game_controller.get_cachelist_size(); i++) {
			window.draw(game_controller.get_cache_sprite(i));
		}
		window.draw(the_sam.get_sprite());
		for(int i = 0; i < game_controller.get_blocklist_size(0); i++) {
			
			wal_s.setTexture(wal_t[game_controller.get_block(i, 0)[2]]);
			wal_s.setPosition(float(game_controller.get_block(i, 0)[0]*16), float(game_controller.get_block(i, 0)[1]*16));
			window.draw(wal_s);
		}
		for (int i = 0; i < game_controller.bulletlist.get_size(); i++) {
			window.draw(game_controller.get_Bullet_sprite(i));
		}

		//Do not draw passed this line.
		if (help == true) {
			help_s.setPosition(the_sam.x-240, the_sam.y-240);
			window.draw(help_s);
		}
		window.draw(fps_counter);
		player_info.setString(health_s);
		player_info.setPosition(the_sam.x-320, the_sam.y-240);
		window.draw(player_info);
		player_info.setString(hunger_s);
		player_info.setPosition(the_sam.x, the_sam.y-240);
		window.draw(player_info);
		player_info.setString(ammo_s);
		player_info.setPosition(the_sam.x-320, the_sam.y+220);
		window.draw(player_info);
		if (!(has_helped)) {
			player_info.setString("Press 'F1' for help.");
			player_info.setPosition(the_sam.x, the_sam.y+220);
			window.draw(player_info);
		}
        window.display();
		if (game_over) {
			window.close();
		}
    }

    return 0;
}