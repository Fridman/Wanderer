#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stddef.h>
#include <string>
#include <sys/stat.h>
#include <direct.h>

using namespace std;

#define SIZE 30

class Zombie;

class Item {
private:
public:
	int type;
	int price;
	Item() {
		type = 0;
	}
	Item(int _type) {
		type = _type;
		price = 0;
		if (type == 0) {
			price = 10;
		}
		else if (type == 1) {
			price = 8;
		}
		else if (type == 100) {
			price = 300;
		}
		price = price;
	}
};

class Item_Array {
	Item * values[4];
	int size;
public:
	Item_Array() {
		size = 0;
		for (int i = 0; i < 4; i++) {
			values[i] = NULL;
		}
	}
	void add_Item(Item * new_Item) {
		values[size] = new_Item;
		size++;
	}

	int add_Item(Item new_Item) {
		for (int i = 0; i < 4; i++) {
			if (values[i] == NULL) {
				values[i] = new Item();
				*values[i] = new_Item;
				size++;
				break;
			}
		}
		return (size-1);
	}

	Item * get_Item(int i) {
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
			else {
				values[k] = NULL;
			}
		}
		size--;
	}
	void swap() {
		Item * temp = values[0];
		values[0] = values[1];
		values[1] = temp;
	}
};

class Gun {
private:
public:
	char name[256];
	int type;
	int price;
	int capacity;
	int left;
	int damage;
	int rate;
	bool automatic;

	Gun() {
		type = 0;
		price = 50;
		capacity = 10;
		left = 10;
		damage = 1;
		rate = 1;
		automatic = false;
		sprintf_s(name, set_name());
	}
	
	Gun(int _type) {
		type = _type;
		if (type == 0) {
				capacity = ((rand() % 7) + 8);
				left = capacity;
				damage = ((rand() % 15) + 15);
				rate = 100;
				automatic = false;
		}
		else if (type == 1) {
			//If gun is a rifle.
			if ((rand() % 2) == 0) {
				capacity = ((rand() % 10) + 10);
				left = capacity;
				damage = ((rand() % 25) + 25);
				rate = 100;
				automatic = false;
			}
			else {
				capacity = ((rand() % 25) + 15);
				left = capacity;
				damage = ((rand() % 25) + 25);
				rate = ((rand() % 7) + 3);
				automatic = true;
			}
		}
		price = int(((1.5 * capacity) + (2 * damage) + (50 * type)) / 1.5);
		sprintf_s(name, set_name());
	}
	Gun(string _name, int _type, int _capacity, int _damage, int _rate, bool _auto) {
		type = _type;
		capacity = _capacity;
		left = capacity;
		damage = _damage;
		rate = _rate;
		automatic = _auto;
		sprintf_s(name, _name.c_str());
		price = int(((1.5 * capacity) + (2 * damage) + (50 * type)) / 1.5);

	}
	char * set_name() {
		char _return[256];
		char brand[256];
		char noun1[256];
		char noun2[256];
		string temp;
		sprintf_s(_return, "");
		char file_name[256];
		sprintf_s(file_name, "data/gun_brands.txt");
		string line = "";
        ifstream sav (file_name);
        if (sav.is_open()) {
			bool can_break = false;
            while (sav.good()) {
				while (1) {
					getline (sav, line);
					int line_end= line.find_first_of(":");
					if (sav.eof()) {
						sav.clear();
						sav.seekg(0);
					}
					temp = line.substr(0, line_end);
					if ((rand() % 20) == 0) {
						sprintf_s(brand, temp.c_str());
						can_break = true;
						break;
					}
					line.erase(0, line_end);
                }
				if (can_break) {
					break;
				}
            }
            sav.close();
        }
		sprintf_s(file_name, "data/gun_nouns.txt");
        sav.open(file_name);
        if (sav.is_open()) {
			bool can_break = false;
            while (sav.good()) {
				while (1) {
					getline (sav, line);
					int line_end= line.find_first_of(":");
					if (sav.eof()) {
						sav.clear();
						sav.seekg(0);
					}
					temp = line.substr(0, line_end);
					if ((rand() % 20) == 0) {
						sprintf_s(noun1, temp.c_str());
						can_break = true;
						break;
					}
					line.erase(0, line_end);
                }
				if (can_break) {
					break;
				}
            }
            sav.close();

        }
		sprintf_s(file_name, "data/gun_suffix.txt");
        sav.open(file_name);
        if (sav.is_open()) {
			bool can_break = false;
            while (sav.good()) {
				while (1) {
					getline (sav, line);
					int line_end = line.find_first_of(":");
					if (sav.eof()) {
						sav.clear();
						sav.seekg(0);
					}
					temp = line.substr(0, line_end);
					if ((rand() % 20) == 0) {
						sprintf_s(noun2, temp.c_str());
						can_break = true;
						break;
					}
                }
				if (can_break) {
					break;
				}
            }
            sav.close();
        }
		sprintf_s(_return, "%s %s-%s", brand, noun1, noun2);
		return _return;
	}
};

class Gun_Array {
	Gun * values[4];
	int size;
public:
	Gun_Array() {
		size = 0;
		for (int i = 0; i < 4; i++) {
			values[i] = NULL;
		}
	}
	void add_Gun(Gun * new_Gun) {
		for (int i = 0; i <= size; i++) {
			if (values[i] == NULL) {
				values[size] = new_Gun;
			}
		}
		size++;
	}

	void add_Gun(Gun new_Gun) {
		for (int i = 0; i <= size; i++) {
			if (values[i] == NULL) {
				values[i] = new Gun;
				*values[i] = new_Gun;
			}
		}
		size++;
	}

	Gun * get_Gun(int i) {
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
		delete values[i];
		for (int k = i; k < size; k++){
			values[k] = NULL;
			if (values[k+1] != NULL) {
				values[k] = values[k+1];
			}
			else {
				values[k] = NULL;
			}
		}
		size--;
	}
	void swap() {
		Gun * temp = values[0];
		values[0] = values[1];
		values[1] = temp;
	}
};

//Bullet class
class Bullet {
private:

	sf::Sprite * bullet_s;
	sf::Texture bullet_t;
	float x;
	float y;
	float target_x, target_y;
	float direction;

public:
	int position;
	int count;
	int damage;
	bool remove;

	Bullet() {
		count = 0;
		position = 0;
		x = 0;
		y = 0;
		remove = false;
		target_x = 0;
		target_y = 0;
		damage = 1;
		bullet_s = new sf::Sprite();
		bullet_t.loadFromFile("images/entities/bullet.png");
		bullet_s->setTexture(bullet_t);
	}
	Bullet(float _x, float _y, int _damage) {
		count = 0;
		position = 0;
		x = _x+8;
		y = _y+8;
		bullet_s = new sf::Sprite();
		bullet_t.loadFromFile("images/entities/bullet.png");
		bullet_s->setTexture(bullet_t);
		remove = false;
		target_x = 0;
		target_y = 0;
		damage = _damage;
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
	void act(int _grid[SIZE*3][SIZE*3]) {
		for (int i = 0; i < 2; i++) {
			move();
			if (_grid[int(x/16)][int(y/16)] >= 10) {
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
			else {
				values[k] = NULL;
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

//Entity class.
class Entity {
private:
public:
	bool dead;
	sf::Sprite sprite;
	bool plos;
	bool in_range;
	float x;
	float y;
	int xshift;
	int yshift;
	int position;
	int visibility;
	int faction;
	string type;

	Entity() {
		dead = false;
		faction = -1;
		x = 0;
		y = 0;
		position = 0;
		plos = false;
		visibility = 0;
		type = "";
		xshift = 0;
		yshift = 0;
	}
	
	bool check_los(int _x, int _y, int _grid[SIZE*3][SIZE*3]) {
		float _x1;
		float _y1;
		float _x2;
		float _y2;
		float view_ratio;
		_x2 = x+7;
		_y2 = y+7;
		_x1 = (float(_x) - (x));
		_y1 = (float(_y) - (y));
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
				return false;
			}
			if (_grid[int(_x2)/16][int(_y2)/16] >= 10) {
				return false;
			}
		}
		return true;
	}

	void set_plos(int _player_x, int _player_y, int _grid[SIZE*3][SIZE*3]) {
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
			plos = true;
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
				if (_grid[int(_x2)/16][int(_y2)/16] >= 10) {
					plos = false;
					break;
				}
				else {
					plos = true;
				}
			}
		}
	}

	sf::Sprite get_sprite() {
		return sprite;
	}
	
	float get_x() {
		return x;
	}
	float get_y() {
		return y;
	}
	void within_range(bool _in_range) {
		in_range = _in_range;
	}

};

class Entity_Array {
	Entity * values[128];
	int size;
public:
	Entity_Array() {
		size = 0;
		for (int i = 0; i < 128; i++) {
			values[i] = NULL;
		}
	}
	int add_entity(Entity * new_entity) {
		for (int i = 0; i < 128; i++) {
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
	void remove(int i) {
		values[i] = NULL;
		for (int k = i; k < size; k++){
			if (values[k+1] != NULL) {
				values[k] = values[k+1];
				values[k]->position--;
			}
			else {
				values[k] = NULL;
			}
		}
		size--;
	}
};

class Portal : public Entity {
	sf::Texture portal_t_d;
	sf::Texture portal_t_u;
	sf::Sprite portal_s;
public:
	int dir;
	Portal() {
		portal_t_d.loadFromFile("images/static/hatchd.png");
		sprite.setTexture(portal_t_d);
		dir = 0;
	}
	Portal(float _x, float _y, int _dir) {
		portal_t_u.loadFromFile("images/static/hatchu.png");
		portal_t_d.loadFromFile("images/static/hatchd.png");
		if (_dir == -1) {
			sprite.setTexture(portal_t_d);
		}
		else {
			sprite.setTexture(portal_t_u);
		}
		x = _x;
		y = _y;
		sprite.setPosition(x, y);
		dir = _dir;
	}

	void change_sprite() {
		if (dir == -1) {
			sprite.setTexture(portal_t_d);
		}
		else if (dir == 1) {
			sprite.setTexture(portal_t_u);
		}
	}
};

class Portal_Array {
	Portal * values[64];
	int size;
public:
	Portal_Array() {
		size = 0;
		for (int i = 0; i < 64; i++) {
			values[i] = NULL;
		}
	}
	int add_portal(Portal * new_portal) {
		for (int i = 0; i < 64; i++) {
			if (values[i] == NULL) {
				values[i] = new Portal();
				values[i] = new_portal;
				size++;
				break;
			}
		}
		return (size-1);
	}

	Portal * get_Portal(int i) {
		return values[i];
	}

	int get_size() {
		return size;
	}
	
	void clear(int _dir) {
		int count = 0;
		for (int i = 0; i < size; i++){
			if (values[i]->dir != _dir) {
				for (int k = i; k <= size; k++) {
					values[k] = values[k+1];
					values[k+1] = NULL;
				}
				i--;
				size--;
			}
			else {
				if (values[i]->dir == 1) {
					values[i]->dir = -1;
				}
				else if (values[i]->dir == -1) {
					values[i]->dir = 1;
				}
				values[i]->change_sprite();
			}
		}
	}
};

class Cache : public Entity {
private:

public:
	int type;
	Gun contained;
	bool in_range;
	sf::Texture cache_t;
	sf::Sprite cache_s;
	bool disp_stats;

	Cache() {
		x = float(rand() % 3);
		in_range = false;
		if (int(x) == 0) {
			type = 0;
			cache_t.loadFromFile("images/entities/interactable/cache_food.png");
		}
		else if ((x == 1)) {
			type = 1;
			cache_t.loadFromFile("images/entities/interactable/cache_ammo.png");
		}
		else if (x == 2) {
			type = 2;
			if ((rand() % 2) == 0) {
				//Gun(Type, Price, Capacity, Damage, Rate, Auto.
				//If gun is a pistol.
				cache_t.loadFromFile("images/entities/interactable/cache_pistol.png");
				contained = Gun(0);
			}
			else {
				//If gun is a rifle.
				contained = Gun(1);
				if (contained.automatic == false) {
					cache_t.loadFromFile("images/entities/interactable/cache_rifle_s.png");
				}
				else if (contained.automatic == true) {
					cache_t.loadFromFile("images/entities/interactable/cache_rifle_a.png");
				}
			}
		}
		x = 0;
		y = 0;
		plos = false;
		cache_s.setTexture(cache_t);
		cache_s.setPosition(x, y);
		disp_stats = false;
	}
	Cache(float _x, float _y) {
		x = float(rand() % 3);
		in_range = false;
		if (int(x) == 0) {
			type = 0;
			cache_t.loadFromFile("images/entities/interactable/cache_food.png");
		}
		else if ((x == 1)) {
			type = 1;
			cache_t.loadFromFile("images/entities/interactable/cache_ammo.png");
		}
		else if (x == 2) {
			type = 2;
			if ((rand() % 2) == 0) {
				//Gun(Type, Price, Capacity, Damage, Rate, Auto.
				//If gun is a pistol.
				cache_t.loadFromFile("images/entities/interactable/cache_pistol.png");
				contained = Gun(0);
			}
			else {
				//If gun is a rifle.
				contained = Gun(1);
				if (contained.automatic == false) {
					cache_t.loadFromFile("images/entities/interactable/cache_rifle_s.png");
				}
				else if (contained.automatic == true) {
					cache_t.loadFromFile("images/entities/interactable/cache_rifle_a.png");
				}
			}
		}
		x = _x;
		y = _y;
		plos = false;
		cache_s.setTexture(cache_t);
		cache_s.setPosition(x, y);
		disp_stats = false;
	} 
	Cache(float _x, float _y, string _name, int _type, int _cap, int _dmg, int _rate, int _autom) {
		in_range = false;
		type = 2;
		contained = Gun(_name, _type, _cap, _dmg, _rate, _autom);
		if (contained.type == 0) {
			//If gun is a pistol.
			cache_t.loadFromFile("images/entities/interactable/cache_pistol.png");
		}
		else {
			//If gun is a rifle.
			if (contained.automatic == false) {
				cache_t.loadFromFile("images/entities/interactable/cache_rifle_s.png");
			}
			else if (contained.automatic == true) {
				cache_t.loadFromFile("images/entities/interactable/cache_rifle_a.png");
			}
		}
		x = _x;
		y = _y;
		plos = false;
		cache_s.setTexture(cache_t);
		cache_s.setPosition(x, y);
		disp_stats = false;
	}
	Cache(float _x, float _y, int _type) {
		in_range = false;
		type = _type;
		if (_type == 0) {
			cache_t.loadFromFile("images/entities/interactable/cache_food.png");
		}
		else if (_type == 1) {
			cache_t.loadFromFile("images/entities/interactable/cache_ammo.png");
		}
		else if (_type == 2) {
			if ((rand() % 2) == 0) {
				//If gun is a pistol.
				cache_t.loadFromFile("images/entities/interactable/cache_pistol.png");
				contained = Gun(0);
			}
			else {
				//If gun is a rifle.
				contained = Gun(1);
				if (contained.automatic == false) {
					cache_t.loadFromFile("images/entities/interactable/cache_rifle_s.png");
				}
				else if (contained.automatic == true) {
					cache_t.loadFromFile("images/entities/interactable/cache_rifle_a.png");
				}
			}
		}
		else if (type == 100) {
			cache_t.loadFromFile("images/entities/interactable/cache_coral.png");
		}
		x = _x;
		y = _y;
		plos = false;
		cache_s.setTexture(cache_t);
		cache_s.setPosition(x, y);
		disp_stats = false;
	}
	Cache(float _x, float _y, Gun _contained) {
		in_range = false;
		type = 2;
		contained = _contained;
		if (contained.type == 0) {
			cache_t.loadFromFile("images/entities/interactable/cache_pistol.png");
		}
		if (contained.type == 1) {
			if (contained.automatic) {
				cache_t.loadFromFile("images/entities/interactable/cache_rifle_a.png");
			}
			else {
				cache_t.loadFromFile("images/entities/interactable/cache_rifle_s.png");
			}

		}
		x = _x;
		y = _y;
		plos = false;
		cache_s.setTexture(cache_t);
		cache_s.setPosition(x, y);
		disp_stats = false;
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
		if (type == -1) {
			return false;
		}
		return true;
	}
	void act() {
		if (plos && (!in_range)) {
					cache_s.setColor(sf::Color::White);
					disp_stats = false;
		}
		else if (plos && in_range) {
					cache_s.setColor(sf::Color(255,255,0));
					if (type == 2) {
						disp_stats = true;
					}
		}
		else {
					cache_s.setColor(sf::Color::Transparent);
					disp_stats = false;
		}
	}
	Gun get_gun() {
		return contained;
	}
	void set_gun(Gun _new) {
		contained = _new;
		if (contained.type == 0) {
			cache_t.loadFromFile("images/entities/interactable/cache_pistol.png");
		}
		if (contained.type == 1) {
			if (contained.automatic) {
				cache_t.loadFromFile("images/entities/interactable/cache_rifle_a.png");
			}
			else {
				cache_t.loadFromFile("images/entities/interactable/cache_rifle_s.png");
			}

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
				size++;
				break;
			}
		}
		return (size-1);
	}

	Cache *get_close() {
		for (int k = 0; k < size; k++){
			if (values[k]->in_range == true) {
				return values[k];
			}
		}
	}
	Cache *get_cache(int i) {
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
			else {
				values[k] = NULL;
			}
		}
		size--;
	}
};

class Human : public Cache {
private:
	sf::Texture hum_td0;
	sf::Texture hum_td1;
	sf::Texture hum_tu0;
	sf::Texture hum_tu1;
	sf::Texture hum_tl0;
	sf::Texture hum_tl1;
	sf::Texture hum_tr0;
	sf::Texture hum_tr1;
	sf::Clock hit_clock;
	sf::Time hit_timer;
	sf::Clock sprite_timer;
	sf::Clock target_timer;
	sf::Time last_target;
	sf::Time sprite_change;
	int target_time;
	Entity * target;
	float direction;
	bool x_stuck;
	bool y_stuck;
	sf::Clock pause_clock;
	sf::Time pause_timer;
	char dir_c;
	int modifier;
	float target_x;
	float target_y;
	string behaviour;
	bool will_move;

public:
	Item_Array Item_inventory;
	Gun_Array Gun_inventory;
	sf::Sprite human_s;
	bool in_range;
	int trading;
	int health;
	bool dead;

	Human() {
		visibility = 1;
		int temp = rand() % 2;
		if ((rand() % 2) == 0) {
			//Gun(Type, Price, Capacity, Damage, Rate, Auto.
			//If gun is a pistol.
			Gun _new_gun = Gun(0);
			Gun_inventory.add_Gun(_new_gun);
		}
		else {
			//If gun is a rifle.
			if ((rand() % 2) == 0) {
				Gun _new_gun = Gun(1);
				Gun_inventory.add_Gun(_new_gun);
			}
			else {
				Gun _new_gun = Gun(1);
				Gun_inventory.add_Gun(_new_gun);
			}
		}
		temp = rand() % 2;
		if ((rand() % 2) == 0) {
			//Gun(Type, Price, Capacity, Damage, Rate, Auto.
			//If gun is a pistol.
			Gun _new_gun = Gun(0);
			Gun_inventory.add_Gun(_new_gun);
		}
		else {
			//If gun is a rifle.
			if ((rand() % 2) == 0) {
				Gun _new_gun = Gun(1);
				Gun_inventory.add_Gun(_new_gun);
			}
			else {
				Gun _new_gun = Gun(1);
				Gun_inventory.add_Gun(_new_gun);
			}
		}
		faction = 1;
		Item_inventory.add_Item(new Item(rand() % 2));
		Item_inventory.add_Item(new Item(rand() % 2));
		Item_inventory.add_Item(new Item(rand() % 2));
		Item_inventory.add_Item(new Item(rand() % 2));
		hum_td0.loadFromFile("images/entities/human/trader_d_0.png");
		hum_td1.loadFromFile("images/entities/human/trader_d_1.png");
		hum_tu0.loadFromFile("images/entities/human/trader_u_0.png");
		hum_tu1.loadFromFile("images/entities/human/trader_u_1.png");
		hum_tl0.loadFromFile("images/entities/human/trader_l_0.png");
		hum_tl1.loadFromFile("images/entities/human/trader_l_1.png");
		hum_tr0.loadFromFile("images/entities/human/trader_r_0.png");
		hum_tr1.loadFromFile("images/entities/human/trader_r_1.png");
		human_s.setTexture(hum_td0);
		plos = false;
		trading = false;
		health = 80;
		dead = false;
		hit_timer = hit_clock.restart();
		x_stuck = false;
		y_stuck = false;
		direction = 0;
		target = NULL;
		sprite_change = sprite_timer.restart();
		last_target = target_timer.restart();
		hit_timer = hit_clock.restart();
		target_time = 0;
		health = 20;
		dir_c = 'd';
		pause_timer = pause_clock.restart();
		target_x = x;
		target_y = y;
		behaviour = "idle";
		human_s.setPosition(x,y);
		will_move = true;
	}
	Human(float _x, float _y) {
		faction = 1;
		visibility = 2;
		int temp = rand() % 2;
		if (temp == 0) {
			//Gun(Type, Price, Capacity, Damage, Rate, Auto.
			//If gun is a pistol.
			Gun _new_gun = Gun(0);
			Gun_inventory.add_Gun(_new_gun);
		}
		else {
			//If gun is a rifle.
			temp = rand() % 2;
			if ((temp) == 0) {
				Gun _new_gun = Gun(1);
				Gun_inventory.add_Gun(_new_gun);
			}
			else {
				Gun _new_gun = Gun(1);
				Gun_inventory.add_Gun(_new_gun);
			}
		}
		temp = rand() % 2;
		if (temp == 0) {
			//Gun(Type, Price, Capacity, Damage, Rate, Auto.
			//If gun is a pistol.
			Gun _new_gun = Gun(0);
			Gun_inventory.add_Gun(_new_gun);
		}
		else {
			//If gun is a rifle.
			temp = rand() % 2;
			if ((temp) == 0) {
				Gun _new_gun = Gun(1);
				Gun_inventory.add_Gun(_new_gun);
			}
			else {
				Gun _new_gun = Gun(1);
				Gun_inventory.add_Gun(_new_gun);
			}
		}
		Item_inventory.add_Item(new Item(rand() % 2));
		Item_inventory.add_Item(new Item(rand() % 2));
		Item_inventory.add_Item(new Item(rand() % 2));
		Item_inventory.add_Item(new Item(rand() % 2));
		hum_td0.loadFromFile("images/entities/human/trader_d_0.png");
		hum_td1.loadFromFile("images/entities/human/trader_d_1.png");
		hum_tu0.loadFromFile("images/entities/human/trader_u_0.png");
		hum_tu1.loadFromFile("images/entities/human/trader_u_1.png");
		hum_tl0.loadFromFile("images/entities/human/trader_l_0.png");
		hum_tl1.loadFromFile("images/entities/human/trader_l_1.png");
		hum_tr0.loadFromFile("images/entities/human/trader_r_0.png");
		hum_tr1.loadFromFile("images/entities/human/trader_r_1.png");
		human_s.setTexture(hum_td0);
		x = _x;
		y = _y;
		plos = false;
		trading = false;
		health = 80;
		dead = false;
		hit_timer = hit_clock.restart();
		x_stuck = false;
		y_stuck = false;
		direction = 0;
		target = NULL;
		sprite_change = sprite_timer.restart();
		last_target = target_timer.restart();
		hit_timer = hit_clock.restart();
		target_time = 0;
		health = 20;
		dir_c = 'd';
		pause_timer = pause_clock.restart();
		target_x = x;
		target_y = y;
		behaviour = "idle";
		human_s.setPosition(x,y);
		will_move = true;
	}
	Gun get_Gun(int i) {
		return *Gun_inventory.get_Gun(i);
	}
	Item get_Item(int i) {
		return *Item_inventory.get_Item(i);
	}
	sf::Sprite get_sprite() {
		return human_s;
	}
	void within_range(bool _in_range) {
		in_range = _in_range;
	}
	void get_hit(int damage) {
		health -= damage;
		human_s.setColor(sf::Color(255,155,155));
		hit_clock.restart();
		x = -1000;
		y = -1000;
	}
	

	void wander(int _grid[SIZE*3][SIZE*3]) {
		will_move = true;
		while (true) {
			int temp_x = ((rand() % 1040));
			int temp_y = ((rand() % 1040));
			if (_grid[temp_x/16][temp_y/16] < 10) {
				target_x = temp_x;
				target_y = temp_y;
				break;
			}
		}
	}

	void flee(int _grid[SIZE*3][SIZE*3]) {
		if (target != NULL) {
			will_move = true;
			target_x = x - (((target->x - x)));
			target_y = y - (((target->y - y))); 
		}
	}

	void set_target(Entity_Array _entitylist,int _grid[SIZE*3][SIZE*3]) {
		int ent_vis = 0;
		for (int i = 0; i < _entitylist.get_size(); i++) {
			if (_entitylist.get_Entity(i) != NULL) {
				ent_vis = _entitylist.get_Entity(i)->visibility;
				if ((behaviour != "idle")) {
					if ((abs(abs(_entitylist.get_Entity(i)->x) - abs(x)) < (16*2*ent_vis)) && (abs(abs(_entitylist.get_Entity(i)->y) - abs(y)) < (16*2*ent_vis))) {
						if (check_los(_entitylist.get_Entity(i)->x, _entitylist.get_Entity(i)->y, _grid)) {
							if (target == NULL) {
								target = new Entity();
							}
							target = _entitylist.get_Entity(i);
							//behaviour = "chase";
							if ((_entitylist.get_Entity(i)->faction == -1)) {
								behaviour = "flee";
								break;
							}
							else if (_entitylist.get_Entity(i)->type == "sam") {
								behaviour = "stop";
								break;
							}
						}
					}
				}
				else if (behaviour == "chase") {
					if ((_entitylist.get_Entity(i)->x < x+(16*17)) && (_entitylist.get_Entity(i)->x > x-(16*17)) && (_entitylist.get_Entity(i)->y < y+(16*17)) && (_entitylist.get_Entity(i)->y > y-(16*17))) {
						if (_entitylist.get_Entity(i)->faction != faction) {
							if (target == NULL) {
								target = new Entity();
								target = _entitylist.get_Entity(i);
							}
							else if ((abs(_entitylist.get_Entity(i)->x - x) < (abs(target->x - x)/2)) && (abs(_entitylist.get_Entity(i)->y - y) < (abs(target->y - y)/2 ))) {
								target = _entitylist.get_Entity(i);
							}
							break;
						}
					}
				}
				if (behaviour == "stop") {
					if ((_entitylist.get_Entity(i)->x < x+(16*6)) && (_entitylist.get_Entity(i)->x > x-(16*6)) && (_entitylist.get_Entity(i)->y < y+(16*6)) && (_entitylist.get_Entity(i)->y > y-(16*6))) {
						if (plos && (_entitylist.get_Entity(i)->type == "sam")) {
							if (_entitylist.get_Entity(i)->faction == faction) {
								target = new Entity();
								target = _entitylist.get_Entity(i);
								break;
							}
						}
					}
					if (!plos) {
						behaviour = "idle";
						target = NULL;
						break;
					}
				}
			}
		}
	}

	//movement functions.
	bool unstick(int _grid[SIZE*3][SIZE*3], char dir_c, bool _x, bool _y) {
		if (_y) {
			if (dir_c  == 'u') {
				if (_grid[int((x)/16)][int((y-1)/16)] < 10) {
					x -= 0.4;
					return true;
				}
				else if (_grid[int((x+14)/16)][int((y-1)/16)] < 10) {
					x += 0.4;
					return true;
				}
			}
			if (dir_c  == 'd') {
				if (_grid[int((x)/16)][int((y+16)/16)] < 10) {
					x -= 0.4;
					return true;
				}
				else if (_grid[int((x+14)/16)][int((y+16)/16)] < 10) {
					x += 0.4;
					return true;
				}
			}
		}
		if (_x) {
			if (dir_c  == 'l') {
				if (_grid[int((x-1)/16)][int((y)/16)] < 10) {
					y -= 0.4;
					return true;
				}
				else if (_grid[int((x-1)/16)][int((y+14)/16)] < 10) {
					y += 0.4;
					return true;
				}
			}
			if (dir_c  == 'r') {
				if (_grid[int((x+16)/16)][int((y)/16)] < 10) {
					y -= 0.4;
					return true;
				}
				else if (_grid[int((x+16)/16)][int((y+14)/16)] < 10) {
					y += 0.4;
					return true;
				}
			}
		}
		return false;
	}

	void move(int _grid[SIZE*3][SIZE*3], Entity_Array _entitylist) {
		sprite_change = sprite_timer.getElapsedTime();
		float temp_x = x;
		float temp_y = y;
		float speed = ((0.8));
		if (behaviour == "wander") {
			speed = 0.2;
		}
		if ((int(target_x) != int(x)) || (int(target_y) != int(y))) {
			if (sprite_change.asSeconds() <= 0.3) {
				if (dir_c == 'u') {
					human_s.setTexture(hum_tu0);
				}
				else if (dir_c == 'd'){
					human_s.setTexture(hum_td0);
				}
				else if (dir_c == 'l'){
					human_s.setTexture(hum_tl0);
				}
				else if (dir_c == 'r'){
					human_s.setTexture(hum_tr0);
				}
			}
			else if ((sprite_change.asSeconds() <= 0.6) && will_move) {
				if (dir_c == 'u') {
					human_s.setTexture(hum_tu1);
				}
				else if (dir_c == 'd'){
					human_s.setTexture(hum_td1);
				}
				else if (dir_c == 'l'){
					human_s.setTexture(hum_tl1);
				}
				else if (dir_c == 'r'){
					human_s.setTexture(hum_tr1);
				}
			}
			else {
				sprite_change = sprite_timer.restart();
			}
			temp_x = (x - target_x);
			temp_y = (y - target_y);
			
			if (abs(temp_x) > abs(temp_y)) {
				if (temp_x > 0) {
					dir_c = 'l';
				}
				if (temp_x < 0) {
					dir_c = 'r';
				}
			}
			else {
				if (temp_y < 0) {
					dir_c = 'd';
				}
				if (temp_y > 0) {
					dir_c = 'u';
				}
			}
			if (will_move) {
				direction = atan(abs(temp_x)/abs(temp_y));
				bool can_move = true;
				int stuck_dir = 1;
				if (temp_x > 0) {
					stuck_dir = -1;
				}
				for (int i = 0; i < _entitylist.get_size(); i++) {
					if ((_entitylist.get_Entity(i)->get_x()+16 < x+17) && (_entitylist.get_Entity(i)->get_x() > x+17) && (_entitylist.get_Entity(i)->get_y()+16 > y) && (_entitylist.get_Entity(i)->get_y() < y+16)) {
						can_move = false;
						break;
					}
				}
				temp_x = x;
				if ((_grid[(int((x+8)+(8*stuck_dir))/16)][int(y+2)/16] < 10) && (_grid[(int((x+8)+(8*stuck_dir))/16)][int(y+14)/16] < 10)) {
					if (can_move) {
						if (!(y_stuck)) {
							x += (float(speed * (sin(direction)) * stuck_dir));
						}
						else {
							if (abs(temp_x) < 15) {
								if (!unstick(_grid, dir_c, x_stuck, y_stuck)) {
									x += (float(speed * (stuck_dir)));
								}
							}
							else {
								x += (float(speed * (stuck_dir)));
							}
						}
						x_stuck = false;
					}
				}
				else {
					x_stuck = true;
				}
				stuck_dir = 1;
				if (temp_y > 0) {
					stuck_dir = -1;
				}
				for (int i = 0; i < _entitylist.get_size(); i++) {
					if ((_entitylist.get_Entity(i)->get_x() < x) && (_entitylist.get_Entity(i)->get_x()+16 > x) && (_entitylist.get_Entity(i)->get_y()+16 > y+17) && (_entitylist.get_Entity(i)->get_y() < y+17)) {
						can_move = false;
						break;
					}
				}
				temp_y = y;
				if ((_grid[(int(x+2)/16)][int((y+8)+(8*stuck_dir))/16] < 10) && (_grid[(int(x+14)/16)][int((y+8)+(8*stuck_dir))/16] < 10)) {
					if (can_move) {
						if (!(x_stuck)) {
							y += (float(speed * (cos(direction)) * stuck_dir));
						}
						else {
							if (abs(temp_y) < 15) {
								if (!unstick(_grid, dir_c, x_stuck, y_stuck)) {
									y += (float(speed * (stuck_dir)));
								}
							}
							else {
								y += float(speed * (stuck_dir));
							}
						}
						y_stuck = false;
					}
				}
				else {
					y_stuck = true;
				}
			}
		}
		else {
			target = NULL;
			behaviour = "idle";
		}
	}
	
	//Chasing an entity behaviour. Sets target location to a target character.
	void stop() {
		if (target != NULL) {
			target_x = target->x;
			target_y = target->y;
			will_move = false;
		}
	}

	void act(int _grid[SIZE*3][SIZE*3], Entity_Array _entitylist, Cache_Array _cachelist) {
		hit_timer = hit_clock.getElapsedTime();
		if (plos && (!in_range) &&(hit_timer.asSeconds() > 0.1)) {
				human_s.setColor(sf::Color::White);
				trading = false;
		}
		else if (plos && in_range) {
					human_s.setColor(sf::Color(255,255,0));
		}
		else if (!plos){
					human_s.setColor(sf::Color::Transparent);
		}
		set_target(_entitylist, _grid);
		if (behaviour == "stop") {
			stop();
		}
		if (behaviour == "flee") {
			flee(_grid);
		}
		if (behaviour == "idle") {
			wander(_grid);
			behaviour = "wander";
		}
		if ((behaviour == "wander") && ((rand() % 800) == 0)) {
			wander(_grid);
			behaviour = "wander";
		}
		
		human_s.setPosition(x,y);
		move(_grid, _entitylist);
		if (health <= 0) {
			dead = true;
		}
	}

};

class Human_Array {
	Human * values[64];
	int size;
public:
	Human_Array() {
		size = 0;
		for (int i = 0; i < 64; i++) {
			values[i] = NULL;
		}
	}
	void add_Human(Human * new_Human) {
		if (size < 64) {
			values[size] = new_Human;
			size++;
		}
	}

	Human * get_Human(int i) {
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
			else {
				values[k] = NULL;
			}
		}
		size--;
	}
	void shift(int xshift, int yshift) {
		for (int i = size-1; i >= 0; i--) {
			values[i]->x = values[i]->x+(xshift*SIZE*16);
			values[i]->y = values[i]->y+(yshift*SIZE*16);
			if (values[i]->x > SIZE*3*16 || values[i]->x < 0) {
				remove(i);
			} else if (values[i]->y > SIZE*3*16 || values[i]->y < 0) {
				remove(i);
			}
		}
	}
};

class Zombie_Array {
	Zombie * values[64];
	int size;
public:
	int xshift;
	int yshift;

	Zombie_Array() {
		xshift = 0;
		yshift = 0;
		size = 0;
		for (int i = 0; i < 64; i++) {
			values[i] = NULL;
		}
	}
	int add_zombie(Zombie * new_zombie) {
		for (int i = 0; i < 64; i++) {
			if (values[i] == NULL) {
				values[i] = new_zombie;
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
			else {
				values[k] = NULL;
			}
		}
		size--;
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
	sf::Texture zom_tu0;
	sf::Texture zom_tu1;
	sf::Texture zom_tl0;
	sf::Texture zom_tl1;
	sf::Texture zom_tr0;
	sf::Texture zom_tr1;
	bool x_stuck;
	bool y_stuck;
	sf::Clock hit_clock;
	sf::Time hit_timer;
	sf::Clock pause_clock;
	sf::Time pause_timer;
	char dir_c;
	int modifier;
	float target_x;
	float target_y;
	string behaviour;
	bool newzom;

public:
	int position;
	int health;
	bool remove;

	Zombie() {
		newzom = true;
		visibility = 3;
		faction = -1;
		plos = false;
		zom_td0.loadFromFile("images/entities/ghouls/zom-d-0.png");
		zom_td1.loadFromFile("images/entities/ghouls/zom-d-1.png");
		zom_tu0.loadFromFile("images/entities/ghouls/zom-u-0.png");
		zom_tu1.loadFromFile("images/entities/ghouls/zom-u-1.png");
		zom_tl0.loadFromFile("images/entities/ghouls/zom-l-0.png");
		zom_tl1.loadFromFile("images/entities/ghouls/zom-l-1.png");
		zom_tr0.loadFromFile("images/entities/ghouls/zom-r-0.png");
		zom_tr1.loadFromFile("images/entities/ghouls/zom-r-1.png");
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
		modifier = rand() % 10;
		health = 90 + modifier;
		remove = false;
		dir_c = 'd';
		pause_timer = pause_clock.restart();
		target_x = x;
		target_y = y;
		behaviour = "idle";
	}

	Zombie(int _x, int _y, int mod_x, int mod_y) {
		newzom = true;
		visibility = 3;
		faction = -1;
		plos = false;
		zom_td0.loadFromFile("images/entities/ghouls/zom-d-0.png");
		zom_td1.loadFromFile("images/entities/ghouls/zom-d-1.png");
		zom_tu0.loadFromFile("images/entities/ghouls/zom-u-0.png");
		zom_tu1.loadFromFile("images/entities/ghouls/zom-u-1.png");
		zom_tl0.loadFromFile("images/entities/ghouls/zom-l-0.png");
		zom_tl1.loadFromFile("images/entities/ghouls/zom-l-1.png");
		zom_tr0.loadFromFile("images/entities/ghouls/zom-r-0.png");
		zom_tr1.loadFromFile("images/entities/ghouls/zom-r-1.png");
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
		modifier = (rand() % 10) + (abs(mod_x) + abs(mod_y));
		health = 75 + modifier * ((rand() % 2) + 1);
		remove = false;
		dir_c = 'd';
		pause_timer = pause_clock.restart();
		target_x = x;
		target_y = y;
		behaviour = "idle";
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
	
	void hit( int _damage) {
		health -= _damage;
		zom_s->setColor(sf::Color(255, 155, 155));
		hit_timer = hit_clock.restart();
	}

	void set_target(Entity_Array _entitylist, int _grid[SIZE*3][SIZE*3]) {
		int ent_vis = 0;
		for (int i = 0; i < _entitylist.get_size(); i++) {
			if (_entitylist.get_Entity(i) != NULL) {
				if (_entitylist.get_Entity(i)->faction != -1) {
					ent_vis = _entitylist.get_Entity(i)->visibility;
					if (behaviour != "chase") {
						if ((_entitylist.get_Entity(i)->x < x+(8)) && (_entitylist.get_Entity(i)->x > x-(8)) && (_entitylist.get_Entity(i)->y < y+(8)) && (_entitylist.get_Entity(i)->y > y-(8))) {
							if (target == NULL) {
								target = new Entity();
							}
							behaviour = "chase";
							target = _entitylist.get_Entity(i);
							break;
						}
						else if (check_los(_entitylist.get_Entity(i)->x, _entitylist.get_Entity(i)->y, _grid) && (_entitylist.get_Entity(i)->x < x+(16*5*ent_vis)) && (_entitylist.get_Entity(i)->x > x-(16*5*ent_vis)) && (_entitylist.get_Entity(i)->y < y+(16*5*ent_vis)) && (_entitylist.get_Entity(i)->y > y-(16*5*ent_vis))) {
							if (target == NULL) {
								target = new Entity();
							}
							behaviour = "chase";
							target = _entitylist.get_Entity(i);
							break;
						}
					}
					else if (behaviour == "chase") {
						if (check_los(_entitylist.get_Entity(i)->x, _entitylist.get_Entity(i)->y, _grid) && (_entitylist.get_Entity(i)->x < x+(16*17)) && (_entitylist.get_Entity(i)->x > x-(16*17)) && (_entitylist.get_Entity(i)->y < y+(16*17)) && (_entitylist.get_Entity(i)->y > y-(16*17))) {
							if (target == NULL) {
								target = new Entity();
								target = _entitylist.get_Entity(i);
								break;
							}
							else if ((abs(_entitylist.get_Entity(i)->x - x) < ((abs(target->x - x)/3)*2)) && (abs(_entitylist.get_Entity(i)->y - y) < ((abs(target->y - y)/3)*2))) {
								target = _entitylist.get_Entity(i);
								break;
							}
						}
						if (target) {
							if (!(check_los(target->x, target->y, _grid))) {
								target = NULL;
							}
						}
					}
				}
			}
		}
	}

	void pause() {
		pause_clock.restart();
	}

	//movement functions.
	bool unstick(int _grid[SIZE*3][SIZE*3], char dir_c, bool _x, bool _y) {
		if (_y) {
			if (dir_c  == 'u') {
				if (_grid[int((x)/16)][int((y-1)/16)] < 10) {
					x -= 0.4;
					return true;
				}
				else if (_grid[int((x+14)/16)][int((y-1)/16)] < 10) {
					x += 0.4;
					return true;
				}
			}
			if (dir_c  == 'd') {
				if (_grid[int((x)/16)][int((y+16)/16)] < 10) {
					x -= 0.4;
					return true;
				}
				else if (_grid[int((x+14)/16)][int((y+16)/16)] < 10) {
					x += 0.4;
					return true;
				}
			}
		}
		if (_x) {
			if (dir_c  == 'l') {
				if (_grid[int((x-1)/16)][int((y)/16)] < 10) {
					y -= 0.4;
					return true;
				}
				else if (_grid[int((x-1)/16)][int((y+14)/16)] < 10) {
					y += 0.4;
					return true;
				}
			}
			if (dir_c  == 'r') {
				if (_grid[int((x+16)/16)][int((y)/16)] < 10) {
					y -= 0.4;
					return true;
				}
				else if (_grid[int((x+16)/16)][int((y+14)/16)] < 10) {
					y += 0.4;
					return true;
				}
			}
		}
		return false;
	}

	//Chasing an entity behaviour. Sets target location to a target character.
	void chase() {
		if (target != NULL) {
			target_x = target->x;
			target_y = target->y;
		}
	}

	//Wandering/idle behaviour. Sets target location to a random spot on the map.
	void wander(int _grid[SIZE*3][SIZE*3]) {
		while (true) {
			int temp_x = ((rand() % SIZE*3*16));
			int temp_y = ((rand() % SIZE*3*16));
			if (_grid[temp_x/16][temp_y/16] < 10) {
				target_x = temp_x;
				target_y = temp_y;
				break;
			}
		}
	}

	//Make sure movement is possible and move.
	void move(int _grid[SIZE*3][SIZE*3], Zombie_Array _zombielist) {
		pause_timer = pause_clock.getElapsedTime();
		if (pause_timer.asSeconds() > 0.4) {
			sprite_change = sprite_timer.getElapsedTime();
			float temp_x = x;
			float temp_y = y;
			float speed = 0.4;
			if (behaviour != "wander") {
				speed = ((0.5) + (float(modifier)/100));
			}
			if ((int(target_x) != int(x)) || (int(target_y) != int(y))) {
				if (sprite_change.asSeconds() <= 0.3) {
					if (dir_c == 'u') {
						zom_s->setTexture(zom_tu0);
					}
					else if (dir_c == 'd'){
						zom_s->setTexture(zom_td0);
					}
					else if (dir_c == 'l'){
						zom_s->setTexture(zom_tl0);
					}
					else if (dir_c == 'r'){
						zom_s->setTexture(zom_tr0);
					}
				}
				else if (sprite_change.asSeconds() <= 0.6) {
					if (dir_c == 'u') {
						zom_s->setTexture(zom_tu1);
					}
					else if (dir_c == 'd'){
						zom_s->setTexture(zom_td1);
					}
					else if (dir_c == 'l'){
						zom_s->setTexture(zom_tl1);
					}
					else if (dir_c == 'r'){
						zom_s->setTexture(zom_tr1);
					}
				}
				else {
					sprite_change = sprite_timer.restart();
				}
				temp_x = (x - target_x);
				temp_y = (y - target_y);
				if (abs(temp_x) > abs(temp_y)) {
					if (temp_x > 0) {
						dir_c = 'l';
					}
					if (temp_x < 0) {
						dir_c = 'r';
					}
				}
				else {
					if (temp_y < 0) {
						dir_c = 'd';
					}
					if (temp_y > 0) {
						dir_c = 'u';
					}
				}
				direction = atan(abs(temp_x)/abs(temp_y));
				bool can_move = true;
				int stuck_dir = 1;
				if (temp_x > 0) {
					stuck_dir = -1;
				}
				for (int i = 0; i < _zombielist.get_size(); i++) {
					if ((_zombielist.get_Zombie(i)->get_x()+16 < x+17) && (_zombielist.get_Zombie(i)->get_x() > x+17) && (_zombielist.get_Zombie(i)->get_y()+16 > y) && (_zombielist.get_Zombie(i)->get_y() < y+16)) {
						can_move = false;
						break;
					}
				}
				temp_x = x;
				if ((_grid[(int((x+8)+(8*stuck_dir))/16)][int(y+2)/16] < 10) && (_grid[(int((x+8)+(8*stuck_dir))/16)][int(y+14)/16] < 10)) {
					if (can_move) {
						if (!(y_stuck)) {
							x += (float(speed * (sin(direction)) * stuck_dir));
						}
						else {
							if (abs(temp_x) < 15) {
								if (!unstick(_grid, dir_c, x_stuck, y_stuck)) {
									x += (float(speed * (stuck_dir)));
								}
							}
							else {
								x += (float(speed * (stuck_dir)));
							}
						}
						x_stuck = false;
					}
				}
				else {
					x_stuck = true;
				}
				stuck_dir = 1;
				if (temp_y > 0) {
					stuck_dir = -1;
				}
				for (int i = 0; i < _zombielist.get_size(); i++) {
					if ((_zombielist.get_Zombie(i)->get_x() < x) && (_zombielist.get_Zombie(i)->get_x()+16 > x) && (_zombielist.get_Zombie(i)->get_y()+16 > y+17) && (_zombielist.get_Zombie(i)->get_y() < y+17)) {
						can_move = false;
						break;
					}
				}
				temp_y = y;
				if ((_grid[(int(x+2)/16)][int((y+8)+(8*stuck_dir))/16] < 10) && (_grid[(int(x+14)/16)][int((y+8)+(8*stuck_dir))/16] < 10)) {
					if (can_move) {
						if (!(x_stuck)) {
							y += (float(speed * (cos(direction)) * stuck_dir));
						}
						else {
							if (abs(temp_y) < 15) {
								if (!unstick(_grid, dir_c, x_stuck, y_stuck)) {
									y += (float(speed * (stuck_dir)));
								}
							}
							else {
								y += float(speed * (stuck_dir));
							}
						}
						y_stuck = false;
					}
				}
				else {
					y_stuck = true;
				}
			}
			else {
				target = NULL;
				behaviour = "idle";
			}
		}
	}

	void act(int _grid[SIZE*3][SIZE*3], Zombie_Array _zombielist, Entity_Array _entitylist) {
		hit_timer = hit_clock.getElapsedTime();
		if (health <= 0) {
			remove = true;
		}
		if (plos && (hit_timer.asSeconds() > 0.1)) {
			zom_s->setColor(sf::Color((255-(modifier*5)),255-(modifier* 5),255-(modifier*5)));
		}
		else if (!plos){
			zom_s->setColor(sf::Color::Transparent);
		}
		set_target(_entitylist, _grid);
		if (behaviour == "chase") {
			chase();
		}
		else {
			if (behaviour == "idle") {
				behaviour = "wander";
				wander(_grid);
			}
			else if ((rand() % 800) == 0) {
				behaviour = "wander";
				wander(_grid);
			}
		}
		move(_grid, _zombielist);
		zom_s->setPosition(x,y);
		newzom = false;
	}
	void forcemove(int _x, int _y) {
		x = x+_x;
		y = y+_y;
		zom_s->setPosition(x,y);
	}
};

//Level chunk class
class Chunk {
public:
	int grid[SIZE][SIZE];

	Chunk() {
		//Construct the chunk class. Set all to zero.
		for (int i = 0; i < SIZE; i++) {
			for (int k = 0; k < SIZE; k++) {
				grid[i][k] = 0;
			}
		}
	}

	void reset() {
		//Reset the level to zero all.
		for (int i = 0; i < SIZE; i++) {
			for (int k = 0; k < SIZE; k++) {
				grid[i][k] = 0;
			}
		}
	}
};

class Chunk_Handler {
	//class for handling chunk class.

	Chunk chunklist[3][3];
	Chunk_Handler() {
		for (int i = 0; i < 3; i++) {
			for (int k = 0; k < 3; k++) {
					chunklist[i][k].reset();
			}
		}
	}
};

//Controller class
class controller {
private:
	Entity * player;
	sf::Texture bullet_t;

public:
	float player_x;
	float player_y;
	int grid[SIZE*3][SIZE*3];
	block_array blocklist;
	Zombie_Array zombielist;
	Entity_Array entitylist;
	Portal_Array objectlist;
	Bullet_Array bulletlist;
	Cache_Array cachelist;
	Human_Array humanlist;
	bool edge;
	bool stair;
	int meta_x, meta_y, meta_z, max_x, max_y, max_z, min_x, min_y, min_z;
	sf::Texture floor_t[12][8];
	
	controller() {
		bullet_t.loadFromFile("images/entities/bullet.png");
		floor_t[0][0].loadFromFile("images/static/g_floor0.png");
		floor_t[0][1].loadFromFile("images/static/g_floor1.png");
		floor_t[0][2].loadFromFile("images/static/g_floor2.png");
		floor_t[0][3].loadFromFile("images/static/g_floor3.png");
		floor_t[0][4].loadFromFile("images/static/g_floor4.png");
		floor_t[0][5].loadFromFile("images/static/g_floor1.png");
		floor_t[0][6].loadFromFile("images/static/g_floor2.png");
		floor_t[0][7].loadFromFile("images/static/g_floor3.png");
		floor_t[1][0].loadFromFile("images/static/c_floor0.png");
		floor_t[1][1].loadFromFile("images/static/c_floor1.png");
		floor_t[1][2].loadFromFile("images/static/c_floor2.png");
		floor_t[1][3].loadFromFile("images/static/c_floor3.png");
		floor_t[1][4].loadFromFile("images/static/c_floor4.png");
		floor_t[1][5].loadFromFile("images/static/c_floor5.png");
		floor_t[1][6].loadFromFile("images/static/c_floor6.png");
		floor_t[1][7].loadFromFile("images/static/c_floor7.png");
		floor_t[2][0].loadFromFile("images/static/l_floor0.png");
		floor_t[2][1].loadFromFile("images/static/l_floor1.png");
		floor_t[2][2].loadFromFile("images/static/l_floor2.png");
		floor_t[2][3].loadFromFile("images/static/l_floor3.png");
		floor_t[2][4].loadFromFile("images/static/l_floor2.png");
		floor_t[2][5].loadFromFile("images/static/l_floor1.png");
		floor_t[2][6].loadFromFile("images/static/l_floor0.png");
		floor_t[2][7].loadFromFile("images/static/l_floor0.png");
		floor_t[10][0].loadFromFile("images/static/walls/wal0.png");
		floor_t[10][1].loadFromFile("images/static/walls/wal1.png");
		floor_t[10][2].loadFromFile("images/static/walls/wal2.png");
		floor_t[10][3].loadFromFile("images/static/walls/wal3.png");
		floor_t[10][4].loadFromFile("images/static/walls/wal4.png");
		floor_t[10][5].loadFromFile("images/static/walls/wal5.png");
		floor_t[10][6].loadFromFile("images/static/walls/wal0.png");
		floor_t[10][7].loadFromFile("images/static/walls/wal1.png");
		floor_t[11][0].loadFromFile("images/static/l_wall0.png");
		floor_t[11][1].loadFromFile("images/static/l_wall0.png");
		floor_t[11][2].loadFromFile("images/static/l_wall0.png");
		floor_t[11][3].loadFromFile("images/static/l_wall0.png");
		floor_t[11][4].loadFromFile("images/static/l_wall0.png");
		floor_t[11][5].loadFromFile("images/static/l_wall0.png");
		floor_t[11][6].loadFromFile("images/static/l_wall0.png");
		floor_t[11][7].loadFromFile("images/static/l_wall0.png");
		player = NULL;
		edge = false;
		stair = false;
		player_x = SIZE + (SIZE/2);
		player_y = SIZE + (SIZE/2);
		meta_x = 0;
		meta_y = 0;
		meta_z = 0;
		max_x = 0;
		max_y = 0;
		max_z = 0;
		min_y = 0;
		min_x = 0;
		min_z = 0;
		for (int i = 0; i < SIZE*3; i++) {
			for (int k = 0; k < SIZE*3; k++) {
				grid[i][k] = 0;
			}
		}
	}

	controller(sf::RenderTexture & bg_r) {
		bullet_t.loadFromFile("images/entities/bullet.png");
		floor_t[0][0].loadFromFile("images/static/g_floor0.png");
		floor_t[0][1].loadFromFile("images/static/g_floor1.png");
		floor_t[0][2].loadFromFile("images/static/g_floor2.png");
		floor_t[0][3].loadFromFile("images/static/g_floor3.png");
		floor_t[0][4].loadFromFile("images/static/g_floor4.png");
		floor_t[0][5].loadFromFile("images/static/g_floor1.png");
		floor_t[0][6].loadFromFile("images/static/g_floor2.png");
		floor_t[0][7].loadFromFile("images/static/g_floor3.png");
		floor_t[1][0].loadFromFile("images/static/c_floor0.png");
		floor_t[1][1].loadFromFile("images/static/c_floor1.png");
		floor_t[1][2].loadFromFile("images/static/c_floor2.png");
		floor_t[1][3].loadFromFile("images/static/c_floor3.png");
		floor_t[1][4].loadFromFile("images/static/c_floor4.png");
		floor_t[1][5].loadFromFile("images/static/c_floor5.png");
		floor_t[1][6].loadFromFile("images/static/c_floor6.png");
		floor_t[1][7].loadFromFile("images/static/c_floor7.png");
		floor_t[2][0].loadFromFile("images/static/l_floor0.png");
		floor_t[2][1].loadFromFile("images/static/l_floor1.png");
		floor_t[2][2].loadFromFile("images/static/l_floor2.png");
		floor_t[2][3].loadFromFile("images/static/l_floor3.png");
		floor_t[2][4].loadFromFile("images/static/l_floor2.png");
		floor_t[2][5].loadFromFile("images/static/l_floor1.png");
		floor_t[2][6].loadFromFile("images/static/l_floor0.png");
		floor_t[2][7].loadFromFile("images/static/l_floor0.png");
		floor_t[10][0].loadFromFile("images/static/walls/wal0.png");
		floor_t[10][1].loadFromFile("images/static/walls/wal1.png");
		floor_t[10][2].loadFromFile("images/static/walls/wal2.png");
		floor_t[10][3].loadFromFile("images/static/walls/wal3.png");
		floor_t[10][4].loadFromFile("images/static/walls/wal4.png");
		floor_t[10][5].loadFromFile("images/static/walls/wal5.png");
		floor_t[10][6].loadFromFile("images/static/walls/wal0.png");
		floor_t[10][7].loadFromFile("images/static/walls/wal1.png");
		floor_t[11][0].loadFromFile("images/static/walls/l_wal0.png");
		floor_t[11][1].loadFromFile("images/static/walls/l_wal0.png");
		floor_t[11][2].loadFromFile("images/static/walls/l_wal0.png");
		floor_t[11][3].loadFromFile("images/static/walls/l_wal0.png");
		floor_t[11][4].loadFromFile("images/static/walls/l_wal0.png");
		floor_t[11][5].loadFromFile("images/static/walls/l_wal0.png");
		floor_t[11][6].loadFromFile("images/static/walls/l_wal0.png");
		floor_t[11][7].loadFromFile("images/static/walls/l_wal0.png");
		player = NULL;
		edge = false;
		stair = false;
		player_x = SIZE + (SIZE/2);
		player_y = SIZE + (SIZE/2);
		meta_x = 0;
		meta_y = 0;
		meta_z = 0;
		max_x = 0;
		max_y = 0;
		max_z = 0;
		min_y = 0;
		min_x = 0;
		min_z = 0;
		sf::Texture bg_t;
		bg_t.create(SIZE*3*16, SIZE*3*16);
		load_map(bg_r, bg_t, true);
		load_map(bg_r, bg_t, 2, 1, true);
		load_map(bg_r, bg_t, 2, 2, true);
		load_map(bg_r, bg_t, 1, 2, true);
		load_map(bg_r, bg_t, 0, 1, true);
		load_map(bg_r, bg_t, 0, 0, true);
		load_map(bg_r, bg_t, 1, 0, true);
		load_map(bg_r, bg_t, 0, 2, true);
		load_map(bg_r, bg_t, 2, 0, true);
	}

	void save_map(int offx = 1, int offy = 1, bool addent = false) {
		ofstream sav;
		char file_name[256];
		string output;
		sprintf_s(file_name, "maps/%i_%i_%i.wap", meta_x+offx-1, meta_y+offy-1, meta_z);
		sav.open(file_name);
		for (int i = 0+(offy*SIZE); i < SIZE+(offy*SIZE); i++) {
            char temp[256];
			for (int k = 0+(offx*SIZE); k < SIZE+(offx*SIZE); k++) {
				sprintf_s(temp, "%i,", grid[k][i]);
                output += temp;
				for (int j = 0; j < zombielist.get_size(); j++) {
					if ((int(zombielist.get_Zombie(j)->get_x()/16) == k) && (int(zombielist.get_Zombie(j)->get_y()/16) == i)) {
						if (addent) {
							sprintf_s(temp, "z,");
							output += temp;
						}
						break;
					}
				}
				for (int j = 0; j < humanlist.get_size(); j++) {
					if ((int(humanlist.get_Human(j)->get_x()/16) == k) && (int(humanlist.get_Human(j)->get_y()/16) == i)) {
						if (addent) {
							sprintf_s(temp, "human,");
							output += temp;
						}
						break;
					}
				}
				for (int j = 0; j < objectlist.get_size(); j++) {
					if ((int(objectlist.get_Portal(j)->x/16) == k) && (int(objectlist.get_Portal(j)->y/16) == i)) {
						if (objectlist.get_Portal(j)-> dir == 1) {
							sprintf_s(temp, "portal1,");
						}
						else {
							sprintf_s(temp, "portal2,");
						}
						output += temp;
						break;
					}
				}
				for (int j = 0; j < cachelist.get_size(); j++) {
					if ((int(cachelist.get_cache(j)->get_x()/16) == k) && (int(cachelist.get_cache(j)->get_y()/16) == i)) {
						if (cachelist.get_cache(j)->type == 0) {
							sprintf_s(temp, "f,");
						}
						else if (cachelist.get_cache(j)->type == 1){
							sprintf_s(temp, "a,");
						}
						else if (cachelist.get_cache(j)->type == 2){
							sprintf_s(temp, "w,%s.%i.%i.%i.%i.%i.", cachelist.get_cache(j)->get_gun().name,\
								cachelist.get_cache(j)->get_gun().type,	cachelist.get_cache(j)->get_gun().capacity, cachelist.get_cache(j)->get_gun().damage\
								, cachelist.get_cache(j)->get_gun().rate, cachelist.get_cache(j)->get_gun().automatic);
						}
						else if (cachelist.get_cache(j)->type == 100){
							sprintf_s(temp, "c,");
						}
						output += temp;
						break;
					}
				}
				sprintf_s(temp, ":\n");
                output += temp;
			}
			output += ";,:";
		}
        sav << output;
        sav.close();
	}

	void load_map(sf::RenderTexture & bg, sf::Texture &bg_t, int offx = 1, int offy = 1, bool addent = false) {
		bg.clear();
		sf::Sprite bg_s(bg_t);
		bg_s.setTexture(bg_t);
		bg.draw(bg_s);
		bg.display();
		string line = "";
		char file_name[256];
		sprintf_s(file_name, "maps/%i_%i_%i.wap", meta_x+offx-1, meta_y+offy-1, meta_z);
        ifstream sav (file_name);
        int i = 0;
		int k = 0;
		string temp;
        if (sav.is_open()) {
            while (sav.good()) {
                getline (sav, line);
                int firstColon = line.find_first_of(":");
                while (firstColon != -1) {
					int firstComma = line.find_first_of(",");
					firstColon = line.find_first_of(":");
					temp = line.substr(0, firstComma);
					if (temp == ";") {
						k++;
						i = 0;
					}
                    else if ( temp == "10" ) {
						add_block(i+(offx*SIZE), k+(offy*SIZE), (rand() % 6));
						line.erase(0, 1);
					}
                    else if ( temp == "11" ) {
						add_texture(i+(offx*SIZE), k+(offy*SIZE), 11);
						line.erase(0, 1);
					}
                    else if ( temp == "1" ) {
						add_texture(i+(offx*SIZE), k+(offy*SIZE), 1);
					}
                    else if ( temp == "2" ) {
						add_texture(i+(offx*SIZE), k+(offy*SIZE), 2);
					}
                    else if ( temp == "z" ) {
						if (addent) {
							add_zombie(i+(offx*SIZE), k+(offy*SIZE));
						}
					}
                    else if ( temp == "human" ) {
						if (addent) {
							add_Human(i+(offx*SIZE), k+(offy*SIZE));
						}
						line.erase(0, 4);
					}
                    else if ( temp == "portal1" ) {
						bool good = true;
						for (int a = 0; a < objectlist.get_size(); a++) {
							if (((objectlist.get_Portal(a)->x) == (i*16)) && ((objectlist.get_Portal(a)->y) == (k*16))) {
								good = false;
							}
						}
						if (good == true) {
							add_portal(i+(offx*SIZE), k+(offy*SIZE), 1);
						}
						line.erase(0, 6);
					}
                    else if ( temp == "portal2" ) {
						bool good = true;
						for (int a = 0; a < objectlist.get_size(); a++) {
							if (((objectlist.get_Portal(a)->x) == (i*16)) && ((objectlist.get_Portal(a)->y) == (k*16))) {
								good = false;
							}
						}
						if (good == true) {
							add_portal(i+(offx*SIZE), k+(offy*SIZE), -1);
						}
						line.erase(0, 6);
					}
                    else if ( temp == "f" ) {
						Cache * _new_cache = new Cache(i*16+(offx*(16*SIZE)), k*16+(offy*(16*SIZE)), 0);
						cachelist.add_cache(_new_cache);
					}
                    else if ( temp == "a" ) {
						Cache * _new_cache = new Cache(i*16+(offx*(16*SIZE)), k*16+(offy*(16*SIZE)), 1);
						cachelist.add_cache(_new_cache);
					}
                    else if ( temp == "w" ) { //%s\nType: %s\nPrice: %i\nCapacity: %i\nDamage: %i\nFire Rate: %i\nAutomatic: %s
						line.erase(0, 2);
						int firstPer = line.find_first_of(".");
						string name = line.substr(0, firstPer);
						line.erase(0, firstPer+1);
						firstPer = line.find_first_of(".");
						string type = line.substr(0, firstPer);
						line.erase(0, firstPer+1);
						firstPer = line.find_first_of(".");
						string cap = line.substr(0, firstPer);
						line.erase(0, firstPer+1);
						firstPer = line.find_first_of(".");
						string dmg = line.substr(0, firstPer);
						line.erase(0, firstPer+1);
						firstPer = line.find_first_of(".");
						string rate = line.substr(0, firstPer);
						line.erase(0, firstPer+1);
						firstPer = line.find_first_of(".");
						string autom = line.substr(0, firstPer);
						Cache * _new_cache = new Cache(i*16+(offx*(16*SIZE)), k*16+(offy*(16*SIZE)), name, atoi(type.c_str()), atoi(cap.c_str()), atoi(dmg.c_str()), atoi(rate.c_str()), atoi(autom.c_str()));
						cachelist.add_cache(_new_cache);
					}
                    else if ( temp == "c" ) {
						Cache * _new_cache = new Cache(i*16+(offx*(16*SIZE)), k*16+(offy*(16*SIZE)), 100);
						sf::Texture coral_base_t;
						coral_base_t.loadFromFile("images/static/coral_base.png");
						sf::Sprite coral_base_s(coral_base_t);
						coral_base_s.setPosition((i-4)*16+(offx*(16*SIZE)), (k-4)*16+(offy*(16*SIZE)));
						bg.draw(coral_base_s);
						cachelist.add_cache(_new_cache);
					}
					line.erase(0, 1);
                }
				i++;
            }
            sav.close();
        }
		else {
			gen_map(bg, offx, offy);
		}
	}

	//Function to generate SIZExSIZE chunk.
	void gen_map(sf::RenderTexture &_background, int offx = 1, int offy = 1) {
		int type;
		if (rand() % 30 == 0) {
			type = 1;
		}
		else {
			type = 0;
		}
		sf::Texture map_block_t;
		int random_walls = 0;
		int storel_walls = 0;
		int sprite_choice = 0;
		int gen_house = 0;
		if (meta_z == 0) {
			for (int i = 0; i < SIZE; i++) {
				for (int k = 0; k < SIZE; k++) {
					grid[i+(offx*SIZE)][k+(offy*SIZE)] = 0;
				}
			}
			for (int j = 0; j < (SIZE/10); j++) {
				for (int k = 0; k < (SIZE/10); k++) {
					gen_house = rand() % 6;
					if (gen_house < 3) {
						for (int i = (gen_house/2); i < (9-(gen_house/2)); i++) {
							for (int ik = (gen_house/2); ik < (9-(gen_house/2)); ik++) {
								add_texture(1+i+(j*9)+(offx*SIZE), ik+(1+k*9)+(offy*SIZE), 1);
								random_walls = rand() % 9;
								storel_walls = random_walls;
								if (((i == int((gen_house/2))) || (i == (9-(gen_house/2)))) || ((ik == int((gen_house/2))) ||(ik == (9-(gen_house/2))))) {
									random_walls = rand() % 20;
								}
								if (storel_walls > 7) { 
									random_walls += gen_house;
								}
								if (random_walls > 7) {
									add_block(1+i+(j*9)+(offx*SIZE), ik+(1+k*9)+(offy*SIZE), rand() % 6);
								}
								if (random_walls == 1) {
									random_walls = rand() % (350-(abs(meta_x) + abs(meta_y))*2);
									if (random_walls == 0) {
										add_cache(1+i+(j*9)+(offx*SIZE), ik+(1+k*9)+(offy*SIZE));
									}
								}
								if (random_walls == 3) {
									if (type == 1) {
										random_walls = rand() % 15;
									}
									else if (type == 0) {
										random_walls = rand() % 400;
									}
									if (random_walls == 0) {
										add_Human(1+i+(j*9)+(offx*SIZE), ik+(1+k*9)+(offy*SIZE));
									}
								}
								else if (random_walls == 0) {
									if (((abs(meta_x) + abs(meta_y))*3) < 50) {
										random_walls = rand() % (100 - ((abs(meta_x) + abs(meta_y)) * 3));
									}
									else {
										random_walls = rand() % 30;
									}

									if (random_walls <= 0) {
										if (type == 0) {
											add_zombie(1+i+(j*9)+(offx*SIZE), ik+(1+k*9)+(offy*SIZE));
										}
									}
								}
								if (random_walls == 2) {
									random_walls = rand() % 200;
									if (random_walls == 0) {
										add_portal(1+i+(j*9)+(offx*SIZE), ik+(1+k*9)+(offy*SIZE), -1);
									}
								}
							}
						}
					}
					else {
						random_walls = rand() % 2000;
						if (random_walls == 0) {
							add_artifact((j*8) + 4+(offx*SIZE), (k*8) + 4+(offy*SIZE));
						}
					}
				}
			}
		}
		else if (meta_z < 0) {
			for (int i = 0; i < SIZE*3; i++) {
				for (int k = 0; k < SIZE*3; k++) {
						add_texture(i, k, 11);
				}
			}
			bool done = 0;
			for (int i = 0; i < objectlist.get_size(); i++) {
				gen_dungeon(int(objectlist.get_Portal(i)->x)/16, int(objectlist.get_Portal(i)->y)/16, rand() % 4, 1, 1);
			}
		}
	}

	void gen_dungeon(int _x, int _y, int chance, int count_stairs, int count_items) {
		int random;
		for (int i = _x; i < (_x+4); i++) {
			for (int ik = _y; ik < (_y+4); ik++) {
				if ((i < SIZE*2) && (i > SIZE) && (ik < SIZE*2) && (ik > SIZE)) {
					add_texture(i, ik, 2);
					random = rand() % 9;
					if (random == 1) {
						random = rand() % ((250 + (count_items*250)) - (abs(meta_x) + abs(meta_y))*2);
						if (random == 0) {
							add_cache(i, ik);
							count_items++;
						}
						else if (random == 1){
							random = rand() % 20;
							if (random == 0) {
								add_Human(i, ik);
							}
						}
						else if (random == 2) {
							if (((abs(meta_x) + abs(meta_y))/10) < 5) {
								random = rand() % (7 - ((abs(meta_x) + abs(meta_y))/10));
							}
							else {
								random = rand() % 5;
							}
							if (random <= 0) {
								add_zombie(i, ik);
							}
						}
						if ((rand() % (450 * count_stairs)) == 1) {
							add_portal(i, ik, -1);
							count_stairs = (count_stairs * count_stairs * count_stairs);
							count_stairs++;
						}
					}
				}
			}
		}
		int gen_house = chance + (rand() % 3) - 1;
		for (int a = 0; a < ((rand() % 2) + 1); a++) {
			gen_house = chance + (rand() % 3) - 1;
			if (gen_house > 3) {
				gen_house--;
			}
			if (gen_house < 0) {
				gen_house++;
			}
			if (gen_house == 0) {
				if ((_x+4) < SIZE*2) {
					if (grid[_x+4][_y] > 9) {
						gen_dungeon(_x+4, _y, gen_house, count_stairs, count_items);
					}
					else if ((rand() % 3) == 0) {
						gen_dungeon(_x, _y, gen_house, count_stairs, count_items);
					}
				}
			}
			if (gen_house == 1) {
				if ((_y+4) < SIZE*2) {
					if (grid[_x][_y+4] > 9) {
						gen_dungeon(_x, _y+4,  gen_house, count_stairs, count_items);
					}
					else if ((rand() % 3) == 0) {
						gen_dungeon(_x, _y, gen_house, count_stairs, count_items);
					}
				}
			}
			if (gen_house == 2) {
				if ((_y-4) > SIZE) {
					if (grid[_x][_y-4] > 9) {
						gen_dungeon(_x, _y-4,  gen_house, count_stairs, count_items);
					}
					else if ((rand() % 3) == 0) {
						gen_dungeon(_x, _y, gen_house, count_stairs, count_items);
					}
				}
			}
			if (gen_house == 3) {
				if ((_x-4) > SIZE) {
					if (grid[_x-4][_y] > 9) {
						gen_dungeon(_x-4, _y,  gen_house, count_stairs, count_items);
					}
					else if ((rand() % 3) == 0) {
						gen_dungeon(_x, _y, gen_house, count_stairs, count_items);
					}
				}
			}
		}
	}

	void draw_textures(sf::RenderTexture &_background, int offx = 1, int offy = 1) {
		_background.clear();
		sf::Sprite map_block_s;
		for (int i = 0; i < SIZE*3; i++) {
			for (int k = 0; k < SIZE*3; k++) {
				map_block_s.setPosition(i*16, k*16);
				if ((rand() % 30) != 0) {
					map_block_s.setTexture(floor_t[grid[i][k]][0]);
				}
				else {
					map_block_s.setTexture(floor_t[grid[i][k]][(rand() % 6)+1]);
				}
				_background.draw(map_block_s);
			}
		}
		for (int i = 0; i < cachelist.get_size(); i++) {
			if (cachelist.get_cache(i)->type == 100) {
				sf::Texture map_block_t;
				map_block_t.loadFromFile("images/static/coral_base.png");
				sf::Sprite map_block_s(map_block_t);
				map_block_s.setPosition((((cachelist.get_cache(i)->x)/16)-1.95) * 16, (((cachelist.get_cache(i)->y)/16)-(1.45)) * 16);
				_background.draw(map_block_s);
			}
		}
	}

	bool request_move(float _x, float _y) {
		if (((grid[(int(_x/16))][(int(_y/16))] < 10) && (grid[((int(_x+15)/16))][int((_y/16))] < 10) && (grid[int((_x/16))][int(((_y+15)/16))] < 10) && (grid[int(((_x+15)/16))][int(((_y+15)/16))]) < 10)) {
			return true;
		}
		else {
			return false;
		}
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
				zombielist.get_Zombie(i)->pause();
				return true;
			}
		}
		return false;
	}

	void add_existing_entity(Entity * _entity) {
		entitylist.add_entity(_entity);
	}

	//End of Player handling functions.

	//Beginning of block handling functions.
	
	//Adds a texture to the block_array.
	void add_texture(int _x, int _y, int texture_index) {
		grid[_x][_y] = texture_index;
	}

	//Adds a block to the block_array.
	void add_block(int _x, int _y, int texture_index) {
		int new_pos[3] = {_x, _y, texture_index};
		blocklist.add_block(new_pos);
		grid[_x][_y] = 10;
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
		Zombie * new_zombie = new Zombie((_x*16), (_y*16), meta_x, meta_y);
		zombielist.add_zombie(new_zombie);
		entitylist.add_entity(new_zombie);
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

	void shiftZombies(int xdir, int ydir) {
		for (int i = zombielist.get_size()-1; i >= 0; i--) {
			zombielist.get_Zombie(i)->forcemove(xdir*SIZE*16, ydir*SIZE*16);
			if (zombielist.get_Zombie(i)->x/16 >= (SIZE*3)-1 || zombielist.get_Zombie(i)->x <= 16) {
				zombielist.remove(i);
			} else if (zombielist.get_Zombie(i)->y/16 >= (SIZE*3)-1 || zombielist.get_Zombie(i)->y <= 16) {
				zombielist.remove(i);
			}
		}
	}

	//End of zombie handling functions.

	void add_bullet(float _x, float _y, sf::Vector2i * mouse_pos, int damage){
		Bullet * _bullet = new Bullet(_x, _y, damage);
		_bullet->set_target(float((((mouse_pos->x) - 320) + _x)), float((((mouse_pos->y) - 240) +_y)));
		bulletlist.add_bullet(_bullet);
	}

	int get_bulletlist_size() {
		return bulletlist.get_size();
	}
	
	sf::Sprite get_Bullet_sprite(int i) {
			return *bulletlist.get_Bullet(i)->get_sprite();
	}

	void add_artifact(float _x, float _y){
		Cache * _cache = new Cache(_x*16, _y*16, 100);
		cachelist.add_cache(_cache);
	}

	void add_portal(float _x, float _y, int _dir){
		Portal * _portal = new Portal(_x*16, _y*16, _dir);
		objectlist.add_portal(_portal);
	}

	void add_cache(float _x, float _y){
		Cache * _cache = new Cache(_x*16, _y*16);
		cachelist.add_cache(_cache);
	}
	void add_cache(float _x, float _y, int _type){
		Cache * _cache = new Cache(_x*16, _y*16, _type);
		cachelist.add_cache(_cache);
	}
	void add_cache(float _x, float _y, Gun _type){
		Cache * _cache = new Cache(_x*16, _y*16, _type);
		cachelist.add_cache(_cache);
	}

	int get_cachelist_size() {
		return cachelist.get_size();
	}
	
	sf::Sprite get_cache_sprite(int i) {
			return cachelist.get_cache(i)->get_sprite();
	}

	bool interact_with_cache(int _x, int _y) {
		bool istrue = false;
		for (int i = 0; i < cachelist.get_size(); i++) {
			if (((abs(cachelist.get_cache(i)->x - (_x)) < 23) && (abs(cachelist.get_cache(i)->y - (_y)) < 23)) && (!istrue)) {
				cachelist.get_cache(i)->within_range(1);
				istrue = true;
			}
			else {
				cachelist.get_cache(i)->within_range(0);
			}
		}
		if (istrue) {
			return true;
		}
		return false;
	}

	int interact_with_human(int _x, int _y) {
		int istrue = -1;
		for (int i = 0; i < humanlist.get_size(); i++) {
			if (((abs(humanlist.get_Human(i)->x - (_x)) < 23) && (abs(humanlist.get_Human(i)->y - (_y)) < 23)) && (istrue == -1)) {
				humanlist.get_Human(i)->within_range(1);
				istrue = i;
			}
			else {
				humanlist.get_Human(i)->within_range(0);
			}
		}
			return istrue;
	}

	
	Portal * interact_with_object(int _x, int _y) {
		Portal * temp = new Portal();
		temp = NULL;
		int istrue = -1;
		for (int i = 0; i < objectlist.get_size(); i++) {
			if (((abs(objectlist.get_Portal(i)->x - (_x)) < 16) && (abs(objectlist.get_Portal(i)->y - (_y)) < 16)) && (istrue == -1)) {
				objectlist.get_Portal(i)->within_range(1);
				player_x = objectlist.get_Portal(i)->x;
				player_y = objectlist.get_Portal(i)->y;
				temp = objectlist.get_Portal(i);
				istrue = 1;
			}
			else {
				objectlist.get_Portal(i)->within_range(0);
			}
		}
		return temp;
	}

	sf::Sprite get_human_sprite(int i) {
			return humanlist.get_Human(i)->get_sprite();
	}

	int get_objectlist_size() {
		return objectlist.get_size();
	}

	sf::Sprite get_object_sprite(int i) {
			return objectlist.get_Portal(i)->get_sprite();
	}
	
	void add_Human(float _x, float _y){
		Human * _Human = new Human(_x*16, _y*16);
		humanlist.add_Human(_Human);
		entitylist.add_entity(_Human);
	}

	void act(sf::RenderTexture & bg) {
		interact_with_cache(int(player_x), int(player_y));
		interact_with_human(int(player_x), int(player_y));
		for (int i = 0; i < 64; i++) {
			if (i < bulletlist.get_size()) {
				bool _break = false;
				for (int j = 0; j < 8; j++) {
					bulletlist.get_Bullet(i)->act(grid);
					for (int k = 0; k < zombielist.get_size(); k++) {
						if ((zombielist.get_Zombie(k)->get_x() < (bulletlist.get_Bullet(i)->get_x()-2)) && ((zombielist.get_Zombie(k)->get_x()+15) > bulletlist.get_Bullet(i)->get_x()) && (zombielist.get_Zombie(k)->get_y() < (bulletlist.get_Bullet(i)->get_y()-2)) && ((zombielist.get_Zombie(k)->get_y()+15) > bulletlist.get_Bullet(i)->get_y())) {
							zombielist.get_Zombie(k)->hit(bulletlist.get_Bullet(i)->damage);
							bulletlist.remove(i);
							_break = true;
							break;
						}
					}
					if (_break) {
						break;
					}
					for (int k = 0; k < humanlist.get_size(); k++) {
						if ((humanlist.get_Human(k)->get_x() < (bulletlist.get_Bullet(i)->get_x()-2)) && ((humanlist.get_Human(k)->get_x()+15) > bulletlist.get_Bullet(i)->get_x()) && (humanlist.get_Human(k)->get_y() < (bulletlist.get_Bullet(i)->get_y()-2)) && ((humanlist.get_Human(k)->get_y()+15) > bulletlist.get_Bullet(i)->get_y())) {
							humanlist.get_Human(k)->get_hit(bulletlist.get_Bullet(i)->damage);
							bulletlist.remove(i);
							_break = true;
							break;
						}
					}
					if (_break) {
						break;
					}
				}
				if (_break) {
					break;
				}
				if (bulletlist.get_Bullet(i)->remove == true) {
					bulletlist.remove(i);
				}
			}
			if (i < zombielist.get_size()) {
				zombielist.get_Zombie(i)->set_plos(int(player_x), int(player_y), grid);
				zombielist.get_Zombie(i)->act(grid, zombielist, entitylist);
				if (zombielist.xshift != 0 || zombielist.yshift != 0) {
					zombielist.get_size();
				}
				if (zombielist.get_Zombie(i)->remove == true) {
					entitylist.remove(zombielist.get_Zombie(i)->Entity::position);
					zombielist.remove(i);
					continue;
				}
			}
			if (i < humanlist.get_size()) {
				humanlist.get_Human(i)->set_plos(int(player_x), int(player_y), grid);
				humanlist.get_Human(i)->act(grid, entitylist, cachelist);
				for (int k = 0; k < zombielist.get_size(); k++) {
					if ((abs(abs(zombielist.get_Zombie(k)->get_x()) - abs(humanlist.get_Human(i)->get_x())) < 16) && (abs(abs(zombielist.get_Zombie(k)->get_y()) - abs(humanlist.get_Human(i)->get_y())) < 16)) {
						humanlist.get_Human(i)->get_hit(20);
					}
				}
				if (humanlist.get_Human(i)->dead) {
					humanlist.remove(i);
				}
			}
			if (i < entitylist.get_size()) {
				if (entitylist.get_Entity(i)->dead) {
					entitylist.remove(i);
				}
			}
			if (i < cachelist.get_size()) {
				if (!(cachelist.get_cache(i)->should_exist())) {
					cachelist.remove(i);
					continue;
				}
				cachelist.get_cache(i)->set_plos(int(player_x), int(player_y), grid);
				cachelist.get_cache(i)->act();
			}
		}
		zombielist.xshift = 0;
		zombielist.yshift = 0;
		sf::Texture bg_t;
		bg_t.create(SIZE*3*16, SIZE*3*16);
		if (player_x < SIZE*16) {
			save_map();
			save_map(2, 1, true);
			save_map(2, 2, true);
			save_map(1, 2);
			save_map(0, 1);
			save_map(0, 0);
			save_map(1, 0);
			save_map(0, 2);
			save_map(2, 0, true);
			meta_x--;
			edge = true;
			player_x = SIZE*32;
			if (meta_x-1 < min_x) {
				min_x = meta_x-1;
			}
			objectlist.clear(0);
			shiftZombies(1, 0);
			blocklist.clear();
			entitylist.clear();
			cachelist.clear();
			humanlist.shift(1, 0);
			for (int i = 0; i < SIZE*3; i++) {
				for (int k = 0; k < SIZE*3; k++) {
					grid[i][k] = 0;
				}
				if (i < 64) {
					if (zombielist.get_Zombie(i) != NULL) {
						add_existing_entity(zombielist.get_Zombie(i));
					}
					if (humanlist.get_Human(i) != NULL) {
						add_existing_entity(humanlist.get_Human(i));
					}
				}
			}
			load_map(bg, bg_t);
			load_map(bg, bg_t, 2, 1);
			load_map(bg, bg_t, 2, 2);
			load_map(bg, bg_t, 1, 2);
			load_map(bg, bg_t, 0, 1, true);
			load_map(bg, bg_t, 0, 0, true);
			load_map(bg, bg_t, 1, 0);
			load_map(bg, bg_t, 0, 2, true);
			load_map(bg, bg_t, 2, 0);
			add_existing_entity(player);
		}
		if (player_x > SIZE*32) {
			save_map();
			save_map(2, 1);
			save_map(2, 2);
			save_map(1, 2);
			save_map(0, 1, true);
			save_map(0, 0, true);
			save_map(1, 0);
			save_map(0, 2, true);
			save_map(2, 0);
			meta_x++;
			edge = true;
			player_x = SIZE*16;
			if (meta_x+1 > max_x) {
				max_x = meta_x+1;
			}
			objectlist.clear(0);
			shiftZombies(-1, 0);
			blocklist.clear();
			entitylist.clear();
			cachelist.clear();
			humanlist.shift(-1, 0);
			for (int i = 0; i < SIZE*3; i++) {
				for (int k = 0; k < SIZE*3; k++) {
					grid[i][k] = 0;
				}
				if (i < 64) {
					if (zombielist.get_Zombie(i) != NULL) {
						add_existing_entity(zombielist.get_Zombie(i));
					}
					if (humanlist.get_Human(i) != NULL) {
						add_existing_entity(humanlist.get_Human(i));
					}
				}
			}
			load_map(bg, bg_t);
			load_map(bg, bg_t, 2, 1, true);
			load_map(bg, bg_t, 2, 2, true);
			load_map(bg, bg_t, 1, 2);
			load_map(bg, bg_t, 0, 1);
			load_map(bg, bg_t, 0, 0);
			load_map(bg, bg_t, 1, 0);
			load_map(bg, bg_t, 0, 2);
			load_map(bg, bg_t, 2, 0, true);
			add_existing_entity(player);
		}
		if (player_y < SIZE*16) {
			save_map();
			save_map(2, 1);
			save_map(2, 2, true);
			save_map(1, 2, true);
			save_map(0, 1);
			save_map(0, 0);
			save_map(1, 0);
			save_map(0, 2, true);
			save_map(2, 0);
			meta_y--;
			edge = true;
			player_y = SIZE*32;
			if (meta_y-1 < min_y) {
				min_y = meta_y-1;
			}
			objectlist.clear(0);
			shiftZombies(0, 1);
			blocklist.clear();
			entitylist.clear();
			cachelist.clear();
			humanlist.shift(0, 1);
			for (int i = 0; i < SIZE*3; i++) {
				for (int k = 0; k < SIZE*3; k++) {
					grid[i][k] = 0;
				}
				if (i < 64) {
					if (zombielist.get_Zombie(i) != NULL) {
						add_existing_entity(zombielist.get_Zombie(i));
					}
					if (humanlist.get_Human(i) != NULL) {
						add_existing_entity(humanlist.get_Human(i));
					}
				}
			}
			load_map(bg, bg_t);
			load_map(bg, bg_t, 2, 1);
			load_map(bg, bg_t, 2, 2);
			load_map(bg, bg_t, 1, 2);
			load_map(bg, bg_t, 0, 1);
			load_map(bg, bg_t, 0, 0, true);
			load_map(bg, bg_t, 1, 0, true);
			load_map(bg, bg_t, 0, 2);
			load_map(bg, bg_t, 2, 0, true);
			add_existing_entity(player);
		}
		if (player_y > SIZE*32) {
			save_map();
			save_map(2, 1);
			save_map(2, 2);
			save_map(1, 2);
			save_map(0, 1);
			save_map(0, 0, true);
			save_map(1, 0, true);
			save_map(0, 2);
			save_map(2, 0, true);
			meta_y++;
			edge = true;
			player_y = SIZE*16;
			if (meta_y+1 > max_y) {
				max_y = meta_y+1;
			}
			objectlist.clear(0);
			shiftZombies(0, -1);
			blocklist.clear();
			entitylist.clear();
			cachelist.clear();
			humanlist.shift(0, -1);
			for (int i = 0; i < SIZE*3; i++) {
				for (int k = 0; k < SIZE*3; k++) {
					grid[i][k] = 0;
				}
				if (i < 64) {
					if (zombielist.get_Zombie(i) != NULL) {
						add_existing_entity(zombielist.get_Zombie(i));
					}
					if (humanlist.get_Human(i) != NULL) {
						add_existing_entity(humanlist.get_Human(i));
					}
				}
			}
			load_map(bg, bg_t);
			load_map(bg, bg_t, 2, 1);
			load_map(bg, bg_t, 2, 2, true);
			load_map(bg, bg_t, 1, 2, true);
			load_map(bg, bg_t, 0, 1);
			load_map(bg, bg_t, 0, 0);
			load_map(bg, bg_t, 1, 0);
			load_map(bg, bg_t, 0, 2, true);
			load_map(bg, bg_t, 2, 0);
			add_existing_entity(player);
		}
		if (edge == true) {
			draw_textures(bg);
			add_existing_entity(player);
		}
		if (stair == true) {
			zombielist.clear();
			blocklist.clear();
			entitylist.clear();
			cachelist.clear();
			humanlist.clear();
			entitylist.clear();
			for (int i = 0; i < SIZE*3; i++) {
				for (int k = 0; k < SIZE*3; k++) {
					grid[i][k] = 0;
				}
			}
			load_map(bg, bg_t, true);
			load_map(bg, bg_t, 2, 1, true);
			load_map(bg, bg_t, 2, 2, true);
			load_map(bg, bg_t, 1, 2, true);
			load_map(bg, bg_t, 0, 1, true);
			load_map(bg, bg_t, 0, 0, true);
			load_map(bg, bg_t, 1, 0, true);
			load_map(bg, bg_t, 0, 2, true);
			load_map(bg, bg_t, 2, 0, true);
			draw_textures(bg);
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
		sf::Clock stat_clock;
		sf::Time stat_timer;
		sf::Clock gun_clock;
		sf::Time gun_timer;
		sf::Time sprite_change;
		sf::Clock sprite_timer;
		sf::Clock step_clock;
		sf::Time step_timer;
		sf::Vector2i * mouse_pos;
		bool can_shoot;
		bool equipped;
		bool can_interact;
		bool moving_x;
		bool moving_y;
		Gun_Array gun_inventory;
		bool can_use;
		sf::SoundBuffer gun_buffer;
		sf::SoundBuffer foot_step;
		sf::SoundBuffer reload;
		sf::SoundBuffer zipper;
		bool play_steps;

	public:
		Gun * equipped_gun;
		int hunger;
		int ammo;
		Item_Array Item_inventory;
		sf::Sound player_sound;
		sf::Sound gun_sound;
		sf::Sound zipper_sound;
		sf::Sound reload_sound;
		int transaction;
		int human_interacting;
		int eq_id;
		int money;

		//Initialize Sam with no argument
		Sam() {
			type = "sam";
			faction = 1;
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
			sf::Time step_timer = step_clock.restart();
			hunger = 100;
			gun_inventory.add_Gun(new Gun(0));
			Item_inventory.add_Item(new Item(1));
			equipped_gun = gun_inventory.get_Gun(0);
			ammo = equipped_gun->left;
			can_shoot = false;
			equipped = true;
			moving_x = false;
			moving_y = false;
			stat_timer = stat_clock.restart();
			gun_buffer.loadFromFile("Audio/Effects/Gun/Gunshot.wav");
			foot_step.loadFromFile("Audio/Effects/Player/foot_step.wav");
			reload.loadFromFile("Audio/Effects/Gun/reload.wav");
			zipper.loadFromFile("Audio/Effects/Player/zipper.wav");
			player_sound.setBuffer(foot_step);
			player_sound.setPitch(float(1000/((rand() % 300)+500)));
			player_sound.setVolume(40);
			play_steps = true;
			transaction = 0;
			eq_id = 0;
			money = 0;
			human_interacting = -1;
			visibility = 1;
		}

		//Initialize Sam with texture argument
		Sam(sf::Vector2i * _mouse_pos) {
			faction = 1;
			type = "sam";
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
			sf::Time step_timer = step_clock.restart();
			mouse_pos = _mouse_pos;
			hunger = 100;
			gun_inventory.add_Gun(new Gun("The Peewee", 0, 6, 15, 100, false));
			equipped_gun = gun_inventory.get_Gun(0);
			Item_inventory.add_Item(new Item(1));
			ammo = equipped_gun->left;
			can_shoot = false;
			equipped = true;
			moving_x = false;
			moving_y = false;
			stat_timer = stat_clock.restart();
			gun_buffer.loadFromFile("Audio/Effects/Gun/Gunshot.wav");
			foot_step.loadFromFile("Audio/Effects/Player/foot_step.wav");
			reload.loadFromFile("Audio/Effects/Gun/reload.wav");
			zipper.loadFromFile("Audio/Effects/Player/zipper.wav");
			player_sound.setBuffer(foot_step);
			player_sound.setPitch(float(1000/((rand() % 300)+500)));
			player_sound.setVolume(30);
			play_steps = true;
			transaction = 0;
			eq_id = 0;
			money = 0;
			human_interacting = -1;
			visibility = 1;
		}

		//Draw Sam
		sf::Sprite get_sprite() {

			return sam_s;

		}

		int get_health() {
			return health;
		}

		Gun * get_equipped() {
			return equipped_gun;
		}
		
		void get_action(controller& _controller) {
			stat_timer = stat_clock.getElapsedTime();
			step_timer = step_clock.getElapsedTime();

			int_elapsed = internal_clock.getElapsedTime();
			if (hunger > 100) {
				hunger = 100;
			}
			sf::Time sprite_change = sprite_timer.getElapsedTime();
			gun_timer = gun_clock.getElapsedTime();
			if (_controller.edge == true) {
				x = _controller.player_x;
				y = _controller.player_y;
				_controller.edge = false;
			}
			if (_controller.stair == true) {
				x = _controller.player_x;
				y = _controller.player_y;
				_controller.stair = false;
			}
			_controller.set_position(x, y);

			if (equipped_gun != NULL) {
				ammo = equipped_gun->left;
				if ((sf::Mouse::isButtonPressed(sf::Mouse::Left)) && can_shoot) {
					if (equipped_gun->left > 0) {
						if (gun_timer.asSeconds() > 1/float((equipped_gun->rate))) {
							visibility = 3;
							gun_sound.setBuffer(gun_buffer);
							gun_sound.setVolume(40);
							gun_sound.setPitch(float(1000/((rand() % 300)+500)));
							gun_sound.play();
							_controller.add_bullet(x, y, mouse_pos, equipped_gun->damage);
							gun_timer = gun_clock.restart();
							equipped_gun->left--;
							can_shoot = equipped_gun->automatic;
						}
					}
				}
			
			}
			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2)) {
				play_steps = false;
			}
			if (!(sf::Mouse::isButtonPressed(sf::Mouse::Left))) {
				can_shoot = true;
			}
			
			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) && can_interact) {
				// space key is pressed: interact!
				human_interacting = int(_controller.interact_with_human(x, y));
				if (_controller.interact_with_cache(int(x), int(y))) {
					can_interact = false;
					if (Item_inventory.get_size() < 4) {
						if (_controller.cachelist.get_close()->type == 0) {
							zipper_sound.setVolume(30);
							zipper_sound.setBuffer(zipper);
							zipper_sound.play();
							Item_inventory.add_Item(new Item(0));
							_controller.cachelist.get_close()->type = -1;
						}
						if (_controller.cachelist.get_close()->type == 1) {
							zipper_sound.setVolume(30);
							zipper_sound.setBuffer(zipper);
							zipper_sound.play();
							Item_inventory.add_Item(new Item(1));
							_controller.cachelist.get_close()->type = -1;
						}
						if (_controller.cachelist.get_close()->type == 100) {
							zipper_sound.setVolume(30);
							zipper_sound.setBuffer(zipper);
							zipper_sound.play();
							Item_inventory.add_Item(new Item(100));
							_controller.cachelist.get_close()->type = -1;
						}
					}
					if (_controller.cachelist.get_close()->type == 2) {
						zipper_sound.setVolume(30);
						zipper_sound.setBuffer(zipper);
						zipper_sound.play();
						if (gun_inventory.get_size() <= 2) {
							if (gun_inventory.get_size() == 2) {
								Gun temp = *gun_inventory.get_Gun(0);
								*gun_inventory.get_Gun(0) = _controller.cachelist.get_close()->contained;
								_controller.cachelist.get_close()->set_gun(temp);
								equipped_gun = gun_inventory.get_Gun(0);
							}
							else {
								gun_inventory.add_Gun(_controller.cachelist.get_close()->contained);
								_controller.cachelist.get_close()->type = -1;
								equipped_gun = gun_inventory.get_Gun(0);
							}
						}
					}
				}
				else if ((human_interacting != -1) && can_interact) {
					can_interact = false;
					zipper_sound.setVolume(30);
					zipper_sound.setBuffer(zipper);
					zipper_sound.play();
					if (transaction == 0) {
						transaction = 1;
						_controller.humanlist.get_Human(human_interacting)->trading = true;
					}
				}
				else if ((_controller.interact_with_object(x, y) != NULL)) {
					can_interact = false;
					_controller.save_map(1, 1, true);
					_controller.save_map(2, 1, true);
					_controller.save_map(2, 2, true);
					_controller.save_map(1, 2, true);
					_controller.save_map(0, 1, true);
					_controller.save_map(0, 0, true);
					_controller.save_map(1, 0, true);
					_controller.save_map(0, 2, true);
					_controller.save_map(2, 0, true);
					x = _controller.player_x;
					y = _controller.player_y;
					_controller.meta_z += _controller.interact_with_object(x, y)->dir;
					_controller.objectlist.clear(_controller.interact_with_object(x, y)->dir);
					_controller.stair = true;
				}
			}
			if (can_interact && (!(transaction == 0))) {
				if (_controller.humanlist.get_Human(human_interacting)->trading == false) {
					transaction = 0;
				}
				//Trading controls
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && (eq_id < ((_controller.humanlist.get_Human(human_interacting)->Gun_inventory.get_size() + _controller.humanlist.get_Human(human_interacting)->Item_inventory.get_size())-1))) {
					// Cycle up through trade window items!
					eq_id++;
					can_interact = false;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && (eq_id > 0)) {
					// Cycle down through trade window items!
					eq_id--;
					can_interact = false;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
					// Buy something while trading.
					if (transaction = 1) {
						can_interact = false;
						if (eq_id < (_controller.humanlist.get_Human(human_interacting)->Gun_inventory.get_size())) {
							if (money >= _controller.humanlist.get_Human(human_interacting)->Gun_inventory.get_Gun(eq_id)->price) {
								if (gun_inventory.get_size() < 2) {
									money -= _controller.humanlist.get_Human(human_interacting)->Gun_inventory.get_Gun(eq_id)->price;
									gun_inventory.add_Gun(*_controller.humanlist.get_Human(human_interacting)->Gun_inventory.get_Gun(eq_id));
									_controller.humanlist.get_Human(human_interacting)->Gun_inventory.remove(eq_id);
									equipped_gun = gun_inventory.get_Gun(0);
								}
							}
						}
						else {
							if (Item_inventory.get_size() < 4) {
								if (money >= _controller.humanlist.get_Human(human_interacting)->Item_inventory.get_Item(eq_id - (_controller.humanlist.get_Human(human_interacting)->Gun_inventory.get_size()))->price) {
									money -= _controller.humanlist.get_Human(human_interacting)->Item_inventory.get_Item(eq_id - (_controller.humanlist.get_Human(human_interacting)->Gun_inventory.get_size()))->price;
									Item_inventory.add_Item(*_controller.humanlist.get_Human(human_interacting)->Item_inventory.get_Item(eq_id-(_controller.humanlist.get_Human(human_interacting)->Gun_inventory.get_size())) );
								}
							}
						}
					}
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
					if (Item_inventory.get_size() > 0) {
						can_interact = false;
						money += Item_inventory.get_Item(0)->price;
						Item_inventory.remove(0);
					}
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
					if (Item_inventory.get_size() > 1) {
						can_interact = false;
						money += Item_inventory.get_Item(1)->price;
						Item_inventory.remove(1);
					}
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
					if (Item_inventory.get_size() > 2) {
						can_interact = false;
						money += Item_inventory.get_Item(2)->price;
						Item_inventory.remove(2);
					}
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
					if (Item_inventory.get_size() > 3) {
						can_interact = false;
						money += int(Item_inventory.get_Item(3)->price);
						Item_inventory.remove(3);
					}
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0)) {
					if (gun_inventory.get_Gun(0) != NULL) {
						can_interact = false;
						money += gun_inventory.get_Gun(0)->price;
						equipped_gun = NULL;
						gun_inventory.remove(0);
						equipped_gun = gun_inventory.get_Gun(0);
					}
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) && can_interact) {
				if (Item_inventory.get_size() > 0) {
					can_interact = false;
					_controller.add_cache(int(x/16), int(y/16), Item_inventory.get_Item(0)->type);
					Item_inventory.remove(0);
					can_interact = false;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) && can_interact) {
				if (Item_inventory.get_size() > 1) {
					can_interact = false;
					_controller.add_cache(int(x/16), int(y/16), Item_inventory.get_Item(1)->type);
					Item_inventory.remove(1);
					can_interact = false;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3) && can_interact) {
				if (Item_inventory.get_size() > 2) {
					can_interact = false;
					_controller.add_cache(int(x/16), int(y/16), Item_inventory.get_Item(2)->type);
					Item_inventory.remove(2);
					can_interact = false;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4) && can_interact) {
				if (Item_inventory.get_size() > 3) {
					can_interact = false;
					_controller.add_cache(int(x/16), int(y/16), Item_inventory.get_Item(3)->type);
					Item_inventory.remove(3);
					can_interact = false;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)) {
				// Close trade.
				if (transaction != 0) {
					transaction = 0;
					human_interacting = -1;
				}
				else  if (can_interact == true) {
					if (gun_inventory.get_Gun(0) != NULL) {
						Gun temp = *gun_inventory.get_Gun(0);
						_controller.add_cache(int(x/16), int(y/16), temp);
						gun_inventory.remove(0);
						equipped_gun = gun_inventory.get_Gun(0);
						can_interact = false;
					}
				}
			}
			//End of trading controls

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab) && equipped) {
				// tab key is pressed: swap weapons!
				gun_inventory.swap();
				equipped_gun = gun_inventory.get_Gun(0);
				equipped = false;
			}
			if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))) {
				// tab key is not pressed allow you to switch weapons!
				equipped = true;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::F) && can_use) {
				// tab key is pressed: swap weapons!
				for (int i = 0; i < Item_inventory.get_size(); i++) {
					if (Item_inventory.get_Item(i)->type == 0) {
						hunger += 25;
						Item_inventory.remove(i);
						can_use = false;
						break;
					}
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && can_use) {
				// tab key is pressed: swap weapons!
				if (equipped_gun != NULL) {
					for (int i = 0; i < Item_inventory.get_size(); i++) {
						if (Item_inventory.get_Item(i)->type == 1) {
							reload_sound.setVolume(10);
							reload_sound.setBuffer(reload);
							reload_sound.play();
							equipped_gun->left = 0;
							equipped_gun->left += equipped_gun->capacity;
							Item_inventory.remove(i);
							can_use = false;
							break;
						}
					}
				}
			}
			if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::R)) && !(sf::Keyboard::isKeyPressed(sf::Keyboard::F)))
			{
				can_use = true;
			}
			if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) && !(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) && !(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) && !(sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) && !(sf::Keyboard::isKeyPressed(sf::Keyboard::Num0)) && !(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) && !(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) && !(sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) && !(sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) && !(sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))) {
				// tab key is pressed: swap weapons!
				can_interact = true;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				moving_x = true;
				// left key is pressed: move our character
				if (_controller.request_move(x + 1, y)) {
					visibility = 2;
					if (step_timer.asSeconds() >= 0.3) {
						if (play_steps) {
							player_sound.setBuffer(foot_step);
							player_sound.setPitch(float(1000/((rand() % 300)+500)));
							player_sound.play();
							step_clock.restart();
						}
					}
					if (moving_y == false) {
						if (equipped_gun!= NULL) {
							if (equipped_gun->type != 0) {
								x += 0.8;
							}
							else {
								x += 1;
							}
						}
						else {
						x += 1;
						}
					}
					else {
						if (equipped_gun!= NULL) {
							if (equipped_gun->type != 0) {
								x += 0.566;
							}
							else {
								x += 0.707;
							}
						}
						else {
							x += 0.707;
						}
					}
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
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				moving_x = true;
				// left key is pressed: move our character
				if (_controller.request_move(x - 1, y)) {
					visibility = 2;
					if (step_timer.asSeconds() >= 0.3) {
						if (play_steps) {
							player_sound.setBuffer(foot_step);
							player_sound.play();
							step_clock.restart();
						}
					}
					if (moving_y == false) {
						if (equipped_gun!= NULL) {
							if (equipped_gun->type != 0) {
								x -= 0.8;
							}
							else {
								x -= 1;
							}
						}
						else {
						x -= 1;
						}
					}
					else {
						if (equipped_gun!= NULL) {
							if (equipped_gun->type != 0) {
								x -= 0.566;
							}
							else {
								x -= 0.707;
							}
						}
						else {
							x -= 0.707;
						}
					}
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
			else {
				moving_x = false;
			}


			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				moving_y = true;
				// left key is pressed: move our character
				if (_controller.request_move(x, y - 1)) {
					visibility = 2;
					if (step_timer.asSeconds() >= 0.3) {
						if (play_steps) {
							player_sound.setBuffer(foot_step);
							player_sound.play();
							step_clock.restart();
						}
					}
					if (moving_x == false) {
						if (equipped_gun!= NULL) {
							if (equipped_gun->type != 0) {
								y -= 0.8;
							}
							else {
								y -= 1;
							}
						}
						else {
							y -= 1;
						}
					}
					else {
						if (equipped_gun!= NULL) {
							if (equipped_gun->type != 0) {
								y -= 0.566;
							}
							else {
								y -= 0.707;
							}
						}
						else {
							y -= 0.707;
						}
					}
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

			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				moving_y = true;
				// left key is pressed: move our character
				if (_controller.request_move(x, y + 1)) {
					visibility = 2;
					if (step_timer.asSeconds() >= 0.3) {
						if (play_steps) {
							player_sound.setBuffer(foot_step);
							player_sound.play();
							step_clock.restart();
						}
					}
					if (moving_x == false) {
						if (equipped_gun!= NULL) {
							if (equipped_gun->type != 0) {
								y += 0.8;
							}
							else {
								y += 1;
							}
						}
						else {
						y += 1;
						}
					}
					else {
						if (equipped_gun!= NULL) {
							if (equipped_gun->type != 0) {
								y += 0.566;
							}
							else {
								y += 0.707;
							}
						}
						else {
							y += 0.707;
						}
					}
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
			else {
				moving_y = false;
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
			if (stat_timer.asSeconds() > 10) {
				stat_timer = stat_clock.restart();
				if (hunger > 0) {
					hunger--;
				}
				if ((hunger > 80) && health < 100) {
						health++;
				}
				else if ((hunger <= 0)) {
						health -= 5;
				}
			}
		}
};

//Main game loop and initializations.
int main()
{
	char ver[256] = "0.0.5";
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

	//Initialize music.
	sf::Music Usedtobe;
	Usedtobe.openFromFile("Audio/Music/Thisusedtobeacity.ogg");

	//Initialize clock.
	printf("Starting clock\n");
	sf::Clock clock;
	char fps_s[256];
	int fps;
	float game_speed;
	game_speed = 0.0166;

	//initialize random.
	printf("Seeding random number generator\n");
	srand (time(NULL));
	
	//Load textures.
	printf("Loading textures\n");
	sf::RenderTexture bg;
	bg.clear();
	bg.create(3840, 3840);
	sf::Texture bg_t;
	sf::Texture sam_t;
	sf::Texture wal_t[6];
	sf::Texture zom_t;
	sf::Texture help_t;
	sf::Texture pistol_t;
	sf::Texture rifle_s_t;
	sf::Texture rifle_a_t;
	sf::Texture inventory_t;
	sf::Texture inventory_c_t;
	sf::Texture ui_arrows;
	sf::Texture food_i;
	sf::Texture ammo_i;
	sf::Texture coral_i;
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
	bg.draw(bg_s);
	bg.display();
	inventory_t.loadFromFile("images/ui/inventory.png");
	inventory_c_t.loadFromFile("images/ui/inventory_c.png");
	food_i.loadFromFile("images/ui/can.png");
	ammo_i.loadFromFile("images/ui/mag.png");
	pistol_t.loadFromFile("images/ui/pistol.png");
	rifle_s_t.loadFromFile("images/ui/rifle_s.png");
	rifle_a_t.loadFromFile("images/ui/rifle_a.png");
	ui_arrows.loadFromFile("images/ui/arrows_c.png");
	sf::Texture bolts_t;
	bolts_t.loadFromFile("images/ui/bolts.png");
	coral_i.loadFromFile("images/ui/coral.png");
	help_t.loadFromFile("images/ui/help screen.png");
	sf::Sprite help_s;
	sf::Sprite player_ui;
	player_ui.setTexture(inventory_t);
	help_s.setTexture(help_t);

	//Create Player entity.
	sf::Vector2i mouse_pos;
	Sam the_sam = Sam(&mouse_pos);
	sf::Sprite wal_s;
	wal_s.setTexture(wal_t[0]);

	//initialize controller.
	controller game_controller(bg);
	game_controller.add_player(&the_sam);
	game_controller.add_existing_entity(&the_sam);

	//Create "maps" directory.
	mkdir("maps");
	//initialize Zombies

	//Initializing window.
	printf("Opening window\n");
	char title[256];
	sprintf_s(title,"Wanderer %s", ver);
	int window_x = 640;
	int window_y = 480;
	float win_ratio_x = 0;
	float win_ratio_y = 0;
    sf::RenderWindow window(sf::VideoMode(window_x, window_y), title);

	//initializing view
	sf::View view;
	view.reset(sf::FloatRect(0, 0, window_x, window_y));
	window.setView(view);


	printf("Running Wanderer\n");
	bool help;
	bool has_helped = false;
	bool game_over = false;
	Usedtobe.setVolume(85);
	Usedtobe.play();
	sf::Time elapsed = clock.getElapsedTime();
    while (window.isOpen())
    {
		elapsed = clock.getElapsedTime();
		if (elapsed.asSeconds() >= game_speed) {
			//Set up FPS counter.
			//Lock game speed.	
			fps = float(1)/elapsed.asSeconds();
			clock.restart();

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
			sprintf_s(fps_s, "%i", fps);
			fps_counter.setString(fps_s);

			char hunger_s[256];
			sprintf_s(hunger_s, "Hunger: %i", the_sam.hunger);
			char ammo_s[256];
			sprintf_s(ammo_s, "%i", the_sam.ammo);



			sf::Event event;

			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
			}

		
			mouse_pos = sf::Mouse::getPosition(window);
			win_ratio_x = float(640)/window.getSize().x;
			win_ratio_y = float(480)/window.getSize().y;
			mouse_pos.x = float(mouse_pos.x) * win_ratio_x;
			mouse_pos.y = float(mouse_pos.y) * win_ratio_y;
		
			if (!help) {
				for (int i = 0; i < 2; i++) {
					game_controller.act(bg);
					the_sam.get_action(game_controller);
					view.reset(sf::FloatRect(the_sam.x-320, the_sam.y-240, 640, 480));
					fps_counter.setPosition(the_sam.x+300, the_sam.y-240);
					window.setView(view);
				}
			}

			char health_s[256];
			sprintf_s(health_s, "HP: %i", the_sam.get_health());
			bg_s.setTexture(bg.getTexture());
			bg.display();
			window.clear();
			window.draw(bg_s);

			int loops = 64;
			if (loops < game_controller.get_blocklist_size(0)) {
				loops = game_controller.get_blocklist_size(0);
			}

			//Draw everything beginning here.
			for (int i = 0; i < loops; i++) {

				if( i < game_controller.get_objectlist_size()) {
					window.draw(game_controller.get_object_sprite(i));
				}
				if (i < game_controller.get_blocklist_size(0)) {
			
					wal_s.setTexture(wal_t[game_controller.get_block(i, 0)[2]]);
					wal_s.setPosition(float(game_controller.get_block(i, 0)[0]*16), float(game_controller.get_block(i, 0)[1]*16));
					window.draw(wal_s);
				}
				if (i < game_controller.bulletlist.get_size()) {
					window.draw(game_controller.get_Bullet_sprite(i));
				}
				if (i < game_controller.humanlist.get_size()) {
					window.draw(game_controller.get_human_sprite(i));
				}
				if( i < game_controller.get_zombielist_size()) {
					window.draw(game_controller.get_zombie_sprite(i));
				}
			}

			if (the_sam.transaction != 0) {
				player_ui.setTexture(inventory_t, true);
				player_ui.setPosition(the_sam.x-320, the_sam.y-94);
				window.draw(player_ui);
				char stats[256];
				if (the_sam.eq_id < game_controller.humanlist.get_Human(the_sam.human_interacting)->Gun_inventory.get_size()) {
					Gun temp_gun = *game_controller.humanlist.get_Human(the_sam.human_interacting)->Gun_inventory.get_Gun(the_sam.eq_id);
					char _auto[256];
					char type[256];
					if (temp_gun.type == 0) {
						sprintf_s(type, "Pistol");
					}
					else if (temp_gun.type == 1) {
						sprintf_s(type, "Rifle");
					}
					if (temp_gun.automatic) {
						sprintf_s(_auto, "True");
					}
					else {
						sprintf_s(_auto, "False");
					}
					sprintf_s(stats, "%s\nType: %s\nPrice: %i\nCapacity: %i\nDamage: %i\nFire Rate: %i\nAutomatic: %s", temp_gun.name, type, temp_gun.price, temp_gun.capacity, temp_gun.damage, temp_gun.rate, _auto);
					player_info.setString(stats);
					player_info.setPosition(the_sam.x-320, the_sam.y-220);
					window.draw(player_info);
					if (temp_gun.type == 0) {
						player_ui.setTexture(pistol_t, true);
					}
					if (temp_gun.type == 1) {
						if (temp_gun.automatic == true) {
							player_ui.setTexture(rifle_a_t, true);
						}
						else if (temp_gun.automatic == false) {
							player_ui.setTexture(rifle_s_t, true);
						}
					}
					player_ui.setPosition(the_sam.x-320, the_sam.y-94);
				}
				else {
					Item temp_item = *game_controller.humanlist.get_Human(the_sam.human_interacting)->Item_inventory.get_Item(the_sam.eq_id-(game_controller.humanlist.get_Human(the_sam.human_interacting)->Gun_inventory.get_size()));
					if(temp_item.type == 0) {
						sprintf_s(stats, "Food\nPrice: %i", temp_item.price);
						player_ui.setTexture(food_i, true);
					}
					else {
						sprintf_s(stats, "Ammo\nPrice: %i", temp_item.price);
						player_ui.setTexture(ammo_i, true);
					}
					player_ui.setPosition(the_sam.x-304, the_sam.y-94);
				}
				player_info.setString(stats);
				player_info.setPosition(the_sam.x-320, the_sam.y-220);
				window.draw(player_info);
				window.draw(player_ui);
				player_ui.setTexture(ui_arrows, true);
				player_ui.setPosition(the_sam.x-256, the_sam.y-94);
				window.draw(player_ui);
			}
			for(int i = 0; i < 4; i++) {
				player_ui.setTexture(inventory_c_t, true);
				player_ui.setPosition(the_sam.x-256 + (i * 32), the_sam.y+208);
				window.draw(player_ui);
				char temp[256];
				sprintf_s(temp, "%i", i+1);
				player_info.setString(temp);
				player_info.setCharacterSize(9);
				player_info.setPosition(the_sam.x-242 + (i * 32), the_sam.y+230);
				window.draw(player_info);
				player_info.setCharacterSize(16);
				window.draw(the_sam.get_sprite());
				if (i < the_sam.Item_inventory.get_size()) {
					if (the_sam.Item_inventory.get_Item(i)->type == 0) {
						player_ui.setTexture(food_i);
					}
					else if (the_sam.Item_inventory.get_Item(i)->type == 1) {
						player_ui.setTexture(ammo_i);
					}
					else if (the_sam.Item_inventory.get_Item(i)->type == 100) {
						player_ui.setTexture(coral_i);
					}
					player_ui.setPosition(the_sam.x-256 + (i * 32), the_sam.y+208);
					window.draw(player_ui);
				}
				if (i < game_controller.get_cachelist_size()) {
					window.draw(game_controller.get_cache_sprite(i));
					if (game_controller.cachelist.get_cache(i)->disp_stats) {
						char _auto[256];
						char type[256];
						if (game_controller.cachelist.get_cache(i)->contained.type == 0) {
							sprintf_s(type, "Pistol");
						}
						else if (game_controller.cachelist.get_cache(i)->contained.type == 1) {
							sprintf_s(type, "Rifle");
						}
						if (game_controller.cachelist.get_cache(i)->contained.automatic) {
							sprintf_s(_auto, "True");
						}
						else {
							sprintf_s(_auto, "False");
						}
						char stats[256];
						sprintf_s(stats, "%s\nType: %s\nPrice: %i\nCapacity: %i\nDamage: %i\nFire Rate: %i\nAutomatic: %s", game_controller.cachelist.get_cache(i)->contained.name, type, game_controller.cachelist.get_cache(i)->contained.price, game_controller.cachelist.get_cache(i)->contained.capacity, game_controller.cachelist.get_cache(i)->contained.damage, game_controller.cachelist.get_cache(i)->contained.rate, _auto);
						player_info.setString(stats);
						player_info.setPosition(the_sam.x-320, the_sam.y-220);
						window.draw(player_info);
					}
				}
			}
			//Do not draw passed this line.
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tilde)) {
				if (the_sam.equipped_gun != NULL) {
					char _auto[256];
					char type[256];
					if (the_sam.equipped_gun->type == 0) {
						sprintf_s(type, "Pistol");
					}
					else if (the_sam.equipped_gun->type == 1) {
						sprintf_s(type, "Rifle");
					}
					if (the_sam.equipped_gun->automatic) {
						sprintf_s(_auto, "True");
					}
					else {
						sprintf_s(_auto, "False");
					}
					char stats[256];
					sprintf_s(stats, "%s\nType: %s\nPrice: %i\nCapacity: %i\nDamage: %i\nFire Rate: %i\nAutomatic: %s", the_sam.equipped_gun->name, type, the_sam.equipped_gun->price, the_sam.equipped_gun->capacity, the_sam.equipped_gun->damage, the_sam.equipped_gun->rate, _auto);
					player_info.setString(stats);
					player_info.setPosition(the_sam.x-320, the_sam.y+48);
					window.draw(player_info);
				}
			}
			window.draw(fps_counter);
			player_info.setString(health_s);
			player_info.setPosition(the_sam.x-320, the_sam.y-240);
			window.draw(player_info);
			char money[256];
			sprintf_s(money, "%i", the_sam.money);
			player_info.setString(money);
			player_info.setPosition(the_sam.x-92, the_sam.y+220);
			window.draw(player_info);
			player_ui.setTexture(bolts_t, true);
			player_ui.setPosition(the_sam.x-112, the_sam.y+220);
			window.draw(player_ui);
			char coord[256];
			sprintf_s(coord, "Map x, y: %i, %i", game_controller.meta_x, game_controller.meta_y);
			player_info.setString(coord);
			player_info.setPosition(the_sam.x, the_sam.y+220);
			window.draw(player_info);
			player_info.setString(hunger_s);
			player_info.setPosition(the_sam.x-48, the_sam.y-240);
			window.draw(player_info);
			player_ui.setTexture(inventory_t, true);
			player_ui.setPosition(the_sam.x-320, the_sam.y+208);
			window.draw(player_ui);
			if (game_controller.humanlist.get_size() > 0){
				player_ui.setColor(sf::Color(0,255,0));
			}
			else {
				player_ui.setColor(sf::Color::White);
			}
			player_ui.setColor(sf::Color::White);
			if (the_sam.get_equipped() != NULL) {
				player_info.setString(ammo_s);
				player_info.setColor(sf::Color::White);
				player_info.setPosition(the_sam.x-320, the_sam.y+182);
				window.draw(player_info);
				if (the_sam.get_equipped()->type == 0) {
					player_ui.setTexture(pistol_t, true);
				}
				if (the_sam.get_equipped()->type == 1) {
					if (the_sam.get_equipped()->automatic) {
						player_ui.setTexture(rifle_a_t, true);
					}
					else {
						player_ui.setTexture(rifle_s_t, true);
					}
				}
				player_ui.setPosition(the_sam.x-320, the_sam.y+208);
				window.draw(player_ui);
				player_info.setString(the_sam.get_equipped()->name);
				player_info.setPosition(the_sam.x-256, the_sam.y+182);
				window.draw(player_info);
				sprintf_s(money, "Zombies: %i", game_controller.zombielist.get_size());
				player_info.setString(money);
				player_info.setPosition(the_sam.x+192, the_sam.y+180);
				window.draw(player_info);
			}
			if (!(has_helped)) {
				player_info.setString("Press 'F1' for help.");
				player_info.setPosition(the_sam.x+192, the_sam.y+220);
				window.draw(player_info);
			}
			if (help == true) {
				help_s.setPosition(the_sam.x-240, the_sam.y-240);
				window.draw(help_s);
			}
			window.display();
			if (game_over) {
				char _file[256];
				for (int i = game_controller.min_x-1; i < game_controller.max_x+1; i++) {
					for (int k = game_controller.min_y-1; k < game_controller.max_y+1; k++) {
						for (int j = -2; j < 2; j++) {
							sprintf_s(_file, "maps/%i_%i_%i.wap", i, k, j);
							remove(_file);
						}
					}
				}
				window.close();
			}
		}
    }
    return 0;
}
//end