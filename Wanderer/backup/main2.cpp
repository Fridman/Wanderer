#include <SFML/Graphics.hpp>
#include <iostream>

class controller;

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
		sprite->setPosition(_x, _y);
		x = _x;
		y = _y;
	}

	block(sf::Texture * _texture, int _x, int _y) {
		sprite = new sf::Sprite;
		sprite->setPosition(_x, _y);
		sprite->setTexture(*_texture);
		x = _x;
		y = _y;
	}

	sf::Sprite * get_sprite() {
		return sprite;
	}

};

//Block Array class.
class block_array {
	block * values[40000];
	int size;
public:
	block_array() {
		size = 0;
		for (int i = 0; i < 40000; i++) {
			values[i] = NULL;
		}
	}
	void add_block(block new_block) {
		for (int i = 0; i < 40000; i++) {
			if (values[i] == NULL) {
				values[i] = new block();
				*values[i] = new_block;
				size++;
				break;
			}
		}
	}
	block get_block(int i) {
		return *values[i];
	}
	int get_size() {
		return size;
	}
};

class Entity {
public:
	float x;
	float y;
	int position;
	Entity() {
		x = 0;
		y = 0;
		position = 0;
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
};

class Zombie : public Entity {
private:

	sf::Sprite * zom_s;
	float x;
	float y;
	Entity * target;
	float direction;

public:
	int position;

	Zombie() {
		zom_s = new sf::Sprite;
		x = 0;
		y = 0;
		zom_s->setPosition(x,y);
		target = NULL;
		direction = 0;
	}

	Zombie(int _x, int _y, sf::Texture * _texture) {
		direction = 0;
		zom_s = new sf::Sprite;
		x = _x;
		y = _y;
		zom_s->setPosition(x,y);
		zom_s->setTexture(*_texture);
		target = NULL;
	}
	sf::Sprite * get_sprite() {
		return zom_s;
	}
	int get_x() {
		return x;
	}
	int get_y() {
		return y;
	}

	void set_target(Entity_Array _entitylist) {
		for (int i = 0; i < _entitylist.get_size(); i++) {
			if ((_entitylist.get_Entity(i)->x < x+(13*16)) && (_entitylist.get_Entity(i)->x > x-(13*16)) && (_entitylist.get_Entity(i)->y < y+(13*16)) && (_entitylist.get_Entity(i)->y > y-(13*16))) {
				if (target == NULL) {
					target = new Entity();
				}
				target = _entitylist.get_Entity(i);
				break;
			}
			else {
				target = NULL;
			}
		}
	}

	void act(controller * _controller) {
		set_target(_controller->entitylist);
		if (target != NULL) {
			float target_x = (x - target->x);
			float target_y = (y - target->y);
			direction = atan(abs(target_x)/abs(target_y));
			if (target_x > 0) {
				target_x -= sin(direction);
			}
			else {
				target_x += sin(direction);
			}
			if (target_y > 0) {
				target_y -= cos(direction);
			}
			else {
				target_y += cos(direction);
			}
			if (_controller->request_move(target_x, target_y)) {
				x = target_x;
				y = target_y;
			}

		}
		else {
		}
		zom_s->setPosition(x, y);
	}

};

class Zombie_Array {
	Entity * values[64];
	int size;
public:
	Zombie_Array() {
		size = 0;
		for (int i = 0; i < 64; i++) {
			values[i] = NULL;
		}
	}
	int add_zombie(Entity new_zombie) {
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

	Entity * get_Zombie(int i) {
		return values[i];
	}

	int get_size() {
		return size;
	}
};




//Controller class
class controller {
private:
	int player_position[2];
	int grid[200][200];
	block_array blocklist;
	Zombie_Array zombielist;
	Entity_Array entitylist;

public:
	controller() {
		player_position[0] = 0;
		player_position[1] = 0;
		for (int i = 0; i < 200; i++) {
			for (int k = 0; k < 200; k++) {
				grid[i][k] = 0;
			}
		}
	}
	bool request_move(int _x, int _y) {
		if ((grid[(_x/16)][(_y/16)] || grid[((_x+15)/16)][(_y/16)] || grid[(_x/16)][((_y+15)/16)] || grid[((_x+15)/16)][((_y+15)/16)]) == 0) {
			return true;
		}
		return false;
	}
	//Beginning of Player handling functions.

	//Checks for collision with a zombie.
	bool get_zom_collision(int _x, int _y) {
		bool collision = false;
		for (int i = ((_x/16)-1); i <= ((_x/16)+1); i++) {
			for (int k = ((_y/16)-1); k <= ((_y/16)+1); k++) {
				if (grid[i][k] == 2) {
					collision = true;
					break;
				}
			}
			if (collision == true) {
				break;
			}
		}
		if (collision == true) {
			for (int i = 0; i < zombielist.get_size(); i++) {
				if ((zombielist.get_Zombie(i)->get_x() < _x+17) && (zombielist.get_Zombie(i)->get_x() > _x-17) && (zombielist.get_Zombie(i)->get_y() < _y+17) && (zombielist.get_Zombie(i)->get_y() > _y-17)) {
					printf("hello");
					return true;
				}
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
	void add_block(int _x, int _y, sf::Texture * _texture) {
		block * new_block = new block(_texture, (_x*16), (_y*16));
		blocklist.add_block(*new_block);
		grid[_x][_y] = 1;
	}

	//Gets the size of the block_array.
	int get_blocklist_size() {
		return blocklist.get_size();
	}

	//Gets and returns the sprite of the block at position i.
	sf::Sprite get_block_sprite(int i) {
			return *blocklist.get_block(i).get_sprite();
	}
	//End of block handling functions.

	//Beginning of zombie handling functions

	//Adds a zombie to the Zombie Array.
	void add_zombie(int _x, int _y, sf::Texture * _texture) {
		Zombie * new_zombie = new Zombie((_x*16), (_y*16), _texture);
		zombielist.add_zombie(*new_zombie);
		grid[_x][_y] = 2;
	}
	
	//Gets the size of the Zombie_Array.
	int get_Zombielist_size() {
		return zombielist.get_size();
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

	void act() {
		for (int i = 0; i < zombielist.get_size(); i++) {
			zombielist.get_Zombie(i)->;
			zombielist.get_Zombie(i)->act(this);
		}
	}
};


//Bullet class
class bullet {
private:

	sf::Sprite bullet_sprite;
	float x;
	float y;
	int target_x, target_y;

public:
	bullet() {
		x = 0;
		y = 0;
	}
	bullet(int _x, int _y) {
		x = _x;
		y = _y;
	}
	void set_target(int _x, int _y) {
		target_x = _x;
		target_y = _y;
	}
	void act() {

	}
};



//Player class is defined here.
class Sam : public Entity {

	private:
		
		sf::Color colour;
		sf::Sprite sam_s;
		int health;
		sf::Clock internal_clock;
		sf::Time int_elapsed;

	public:

		//Initialize Sam with no argument
		Sam() {

			printf("Creating Sam\n");
			colour = sf::Color::White;
			x = 0;
			y = 0;
			health = 100;
			position = 0;
			sf::Time int_elapsed = internal_clock.restart();

		}

		//Initialize Sam with texture argument
		Sam(sf::Texture * sam_texture) {

			printf("Creating Sam\n");
			x = 0;
			y = 0;
			health = 100;
			sam_s.setTexture(*sam_texture);
			sam_s.setColor(colour);
			sf::Time int_elapsed = internal_clock.restart();

		}

		//Draw Sam
		sf::Sprite get_sprite() {

			return sam_s;

		}

		int get_health() {
			return health;
		}
		
		void get_action(controller _controller) {

			int_elapsed = internal_clock.getElapsedTime();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				// left key is pressed: move our character
				if (_controller.request_move(x + 1.1, y)) {
					x += 1.1;
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				// left key is pressed: move our character
				if (_controller.request_move(x - 1.1, y)) {
					x -= 1.1;
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				// left key is pressed: move our character
				if (_controller.request_move(x, y - 1.1)) {
					y -= 1.1;
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				// left key is pressed: move our character
				if (_controller.request_move(x, y + 1.1)) {
					y += 1.1;
				}
			}
			sam_s.setPosition(x,y);
			if (_controller.get_zom_collision(x, y)) {
				if (int_elapsed.asSeconds() > 1) {
					health -= 20;
					internal_clock.restart();
				}
			}
			if (int_elapsed.asSeconds() > 0.2) {
				colour = sf::Color::White;
			}
			sam_s.setColor(colour);
		}
};


int main()
{
	char ver[256] = "0.0.1";
	printf("Wanderer ver: %s\n", ver);
	printf("Initializing\n");

	//Initialize fonts.
	sf::Text fps_counter;
	sf::Text player_health;
	sf::Font arial;
	arial.loadFromFile("fonts/arial.ttf");
	fps_counter.setFont(arial);
	fps_counter.setCharacterSize(16);
	fps_counter.setColor(sf::Color::Red);
	player_health.setFont(arial);
	player_health.setCharacterSize(16);
	player_health.setColor(sf::Color::Red);
	player_health.setPosition(600, 0);

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
	sf::Texture wal_t;
	sf::Texture zom_t;
	sam_t.loadFromFile("images/sam.png");
	wal_t.loadFromFile("images/wal.png");
	zom_t.loadFromFile("images/zom.png");

	//Create Player entity.
	Sam the_sam = Sam(&sam_t);

	//initialize controller.
	controller game_controller;
	game_controller.add_existing_entity(&the_sam);

	//initialize walls
	int random_walls = 0;
	int stored_walls = 0;
	for (int i = 2; i < 38; i++) {
		for (int ik = 2; ik < 28; ik++) {
			random_walls = rand() % 7;
			if (stored_walls > 5) {
				random_walls+=3;
			}
			if (random_walls > 5) {
				game_controller.add_block(i, ik, &wal_t);
			}
			stored_walls = random_walls;
		}
	}

	//initialize Zombies
	game_controller.add_zombie(1, 2, &zom_t);

	//Initializing window.
	printf("Opening window\n");
	char title[256];
	sprintf_s(title,"Wanderer %s", ver);
    sf::RenderWindow window(sf::VideoMode(640, 480), title);


	printf("Running Wanderer\n");
    while (window.isOpen())
    {
		//Set up FPS counter.
		sf::Time elapsed = clock.restart();
		fps = 1/elapsed.asSeconds();
		//Lock game speed.
		if (elapsed.asSeconds() < game_speed) {
			sf::Time sleep_time = sf::seconds(game_speed - elapsed.asSeconds());
			sf::sleep(sleep_time);
			fps = 1/(sleep_time.asSeconds()+elapsed.asSeconds());
		}
		sprintf_s(fps_s, "%i", fps);
		fps_counter.setString(fps_s);



        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

		the_sam.get_action(game_controller);
		game_controller.act();

		char health_s[256];
		sprintf_s(health_s, "%i", the_sam.get_health());
		player_health.setString(health_s);

        window.clear();
		//Draw everything beginning here.

		window.draw(the_sam.get_sprite());
		for(int i = 0; i < game_controller.get_blocklist_size(); i++) {
			window.draw(game_controller.get_block_sprite(i));
		}
		for(int i = 0; i < game_controller.get_zombielist_size(); i++) {
			window.draw(game_controller.get_zombie_sprite(i));
		}
		window.draw(player_health);

		//Do not draw passed this line.
		window.draw(fps_counter);
        window.display();
    }

    return 0;
}