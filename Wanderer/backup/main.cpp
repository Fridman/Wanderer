#include <SFML/Graphics.hpp>
#include <iostream>

//Block class
class block {
public:
	int x;
	int y;
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

//Block Node class
class block_node {
private:

	//private variables
	block * value;

public:

	//public variables
	block_node * next;
	block_node * parent;

	//public methods
	block_node() {
		next = NULL;
		parent = NULL;
		value = NULL;
	}

	block_node(block _value) {
		next = NULL;
		parent = NULL;
		value = new block();
		*value = _value;
	}

	//returns true if this node has no value.
	bool is_empty() {
		if (value == NULL) {
			return true;
		}
		return false;
	}

	//returns true if this node has no next node.
	bool has_next() {
		if (next == NULL) {
			return false;
		}
		return true;
	}

	//returns true if this node has no previous node.
	bool has_parent() {
		if (parent == NULL) {
			return false;
		}
		return true;
	}

	block * get_value() {
		return value;
	}
	void set_next(block_node _value) {
		next = new block_node(_value);
	}
};

//Block list class
class block_list {


private:

	block_node * root;

public:

	int size;

	block_list() {
		root = NULL;
		size = 0;
	}


	void add_node(block _value) {
		if (root == NULL) {
			root = new block_node(_value);
			root->next = new block_node();
			size++;
		}
		else if (!(root->has_next())) {
			root->set_next(_value);
			size++;
		}
		else {
			block_node * current = new block_node();
			current = root->next;
			while (current->has_next()) {
				current = current->next;
			}
			current->set_next(_value);
			size++;
		}
	}
	block_node * get_node(int index) {
			if (index == 0) {
				return root;
			}
			block_node * current;
			current = root->next;
			for (int i = 0; i < index; i++) {
				if (current->has_next()) {
					current = current->next;
				}
				else {
					return NULL;
				}
			}
			return current;
	}

};

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

//Controller class
class controller {
private:

	int grid[200][200];
	block_array blocklist;


public:
	controller() {
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

	int get_blocklist_size() {
		return blocklist.get_size();
	}

	sf::Sprite get_node_sprite(int i) {
		
			return *blocklist.get_block(i).get_sprite();
	}

	void add_block(int _x, int _y, sf::Texture * _texture) {
		block * new_block = new block(_texture, (_x*16), (_y*16));
		blocklist.add_block(*new_block);
		grid[_x][_y] = 1;
	}
};
class bullet {
private:

	sf::Sprite bullet_sprite;
	int x;
	int y;
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
class Sam {

	private:

		sf::Sprite sam_s;
		int x;
		int y;

	public:

		//Initialize Sam with no argument
		Sam() {

			printf("Creating Sam\n");
			x = 0;
			y = 0;

		}

		//Initialize Sam with texture argument
		Sam(sf::Texture * sam_texture) {

			printf("Creating Sam\n");
			x = 0;
			y = 0;
			sam_s.setTexture(*sam_texture);

		}

		//Draw Sam
		sf::Sprite get_sprite() {

			return sam_s;

		}
		
		void get_action(controller _controller) {

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				// left key is pressed: move our character
				if (_controller.request_move(x + 1, y)) {
					x += 1;
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				// left key is pressed: move our character
				if (_controller.request_move(x - 1, y)) {
					x -= 1;
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				// left key is pressed: move our character
				if (_controller.request_move(x, y - 1)) {
					y -= 1;
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				// left key is pressed: move our character
				if (_controller.request_move(x, y + 1)) {
					y += 1;
				}
			}
			sam_s.setPosition(x,y);
		}
};


int main()
{
	char ver[256] = "0.0.1";
	printf("Wanderer ver: %s\n", ver);
	printf("Initializing\n");

	//Initialize fonts.
	sf::Text fps_counter;
	sf::Font arial;
	arial.loadFromFile("fonts/arial.ttf");
	fps_counter.setFont(arial);
	fps_counter.setCharacterSize(16);
	fps_counter.setColor(sf::Color::Red);

	//Initialize clock.
	printf("Starting clock\n");
	sf::Clock clock;
	char fps_s[256];
	int fps;
	double game_speed;
	game_speed = 0.0167;
	
	//Load textures.
	printf("Loading textures\n");
	sf::Texture sam_t;
	sf::Texture wal_t;
	sam_t.loadFromFile("images/sam.png");
	wal_t.loadFromFile("images/wal.png");

	//Create Player entity.
	Sam the_sam = Sam(&sam_t);

	//initialize controller.
	controller game_controller;

	//initialize walls
	for (int i = 2; i < 10; i++) {
		for (int ik = 2; ik < 10; ik++) {
			game_controller.add_block(i, ik, &wal_t);
		}
	}

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

        window.clear();
		//Draw everything beginning here.

		window.draw(fps_counter);
		window.draw(the_sam.get_sprite());
		for(int i = 0; i < game_controller.get_blocklist_size(); i++) {
			window.draw(game_controller.get_node_sprite(i));
		}

		//Do not draw passed this line.
        window.display();
    }

    return 0;
}