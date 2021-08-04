#include "Game.h"

//Static Functions

void Game::initVariables()
{
	this->window = nullptr;
	this->fullscreen = false;
	this->dt = 0.f;
}

//Initializer Functions
void Game::initWindow()
{
	//Creates an SFML window using options from a window.ini file
	std::ifstream ifs("Config/window.ini");
	this->videoModes = sf::VideoMode::getFullscreenModes();

	std::string title = "None";
	sf::VideoMode window_bounds = sf::VideoMode::getDesktopMode();
	bool fullscreen = false;
	unsigned framerate_limit = 120;
	bool verticle_sync_enabled = false;
	unsigned antialiasing_level = 0;


	//Streams window settings from window.ini
	if (ifs.is_open())
	{
		std::getline(ifs, title);
		ifs >> window_bounds.width >> window_bounds.height;
		ifs >> fullscreen;
		ifs >> framerate_limit;
		ifs >> verticle_sync_enabled;
		ifs >> antialiasing_level;
	}

	ifs.close();

	//Builds the window and sets view settings
	this->fullscreen = fullscreen;
	this->windowSettings.antialiasingLevel = antialiasing_level;

	if(this->fullscreen)
		this->window = new sf::RenderWindow(window_bounds, title, sf::Style::Fullscreen, windowSettings);
	else
		this->window = new sf::RenderWindow(window_bounds, title, sf::Style::Titlebar | sf::Style::Close, windowSettings);
		
	this->window->setFramerateLimit(framerate_limit);
	this->window->setVerticalSyncEnabled(verticle_sync_enabled);
}

void Game::initKeys()
{
	//Maps keys to a list of supported keys
	std::ifstream ifs("Comfig/supported_keys.ini");

	if (ifs.is_open())
	{
		std::string key = "";
		int key_value = 0;

		while (ifs >> key >> key_value)
		{
			this -> supportedKeys[key] = key_value;
		}
	}
	this->supportedKeys["Escape"] = sf::Keyboard::Key::Escape;
	this->supportedKeys["A"] = sf::Keyboard::Key::A;
	this->supportedKeys["D"] = sf::Keyboard::Key::D;
	this->supportedKeys["W"] = sf::Keyboard::Key::W;
	this->supportedKeys["S"] = sf::Keyboard::Key::S;
}

void Game::initStates()
{
	//Loads current game state and supported keys to a window
	this->states.push(new MainMenuState(this->window, &this->supportedKeys, &this->states));
}



//Constructors / Destructors
Game::Game()
{
	//Constructs an instance of Window when launching the game
	this->initWindow();
	this->initKeys();
	this->initStates();
}

Game::~Game()
{
	//Destroys the instance of Window when closing the game
	delete this->window;

	while (!this->states.empty())
	{
		delete this->states.top();
		this->states.pop();
	}

}

//Functions
void Game::updateSFMLEvents()
{
	//Closes the game if the X button is pressed
	while (this->window->pollEvent(this->sfEvent))
	{
		if (this->sfEvent.type == sf::Event::Closed)
			this->window->close();
	}
}

void Game::endApplication()
{
	std::cout << "Ending Application!" << "\n";
}

void Game::updateDt()
{
	//Updates the Delta Time variable with the time elapsed per update+render of one frame
	this->dt = this->dtClock.restart().asSeconds();
	system("cls");
	std::cout << this->dt << "\n";
}

void Game::update()
{
	this->updateSFMLEvents();

	if (!this->states.empty())
	{
		this->states.top()->update(this->dt);

		if (this->states.top()->getQuit())
		{
			//TODO: Save character info
			this->states.top()->endState();
			delete this->states.top();
			this->states.pop();
		}
	}
	//Application End
	else
	{
		this->endApplication();
		this->window->close();
	}
}

void Game::render()
{
	this->window->clear();

	//Render Items
	if (!this->states.empty())
		this->states.top()->render(this->window);

	this->window->display();
}

void Game::run()
{
	while (this->window->isOpen())
	{
		this->updateDt();
		this->update();
		this->render();
	}
}