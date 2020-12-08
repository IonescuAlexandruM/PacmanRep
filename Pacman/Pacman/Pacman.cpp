#include "Pacman.h"

#include <sstream>

#include <time.h>

#include <iostream>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv) 
{
	//Initialise member variables
	_pacman = new Player{ 0.1f, 250 };
	_pacman->dead = false;
	_pacman->speedMultiplier = 1.0f;
	_pacman->score = 0;
	_cherry = new Enemy{ 500,0.5f };
	_move = new Movement();
	_menu = new MenuState();
	
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i] = new MovingEnemy;
		
		_ghosts[i]->speed = 0.2f;
	}
	_ghosts[0]->direction = 0;
	_ghosts[1]->direction = 1;
	_ghosts[2]->direction = 2;
	_ghosts[3]->direction = 3;

	srand((int)time(NULL));

	int i;
	for(i=0;i<MUNCHIECOUNT;i++)
	{
		_munchies[i] = new Enemy{ 500 };
		_munchies[i]->frameCount = rand() % 1;
		_munchies[i]->currentFrameTime = 0;
		_munchies[i]->frameCount = rand() % 500 + 50;
		_munchies[i]->dead = false;
	}

	//Initialization of all starting points of the game
	
	_menu->pausedScreen = false;
	_menu->p = false;
	_menu->startScreen = true;
	_menu->space = false;
	_move->left = false;
	_move->right = false;
	_move->up = false;
	_move->down = false;
	_cherry->currentFrameTime = 0;
	_cherry->dead = false;
	_pacman->frame = 0;
	_pacman->direction = 0;
	_pacman->frameCount = 0;
	_pacman->currentFrameTime = 0;
	
	//Initialization of sounds
	_pop = new SoundEffect();
	_bruhgh = new SoundEffect();
	_wow = new SoundEffect();
	




	//Initialise important Game aspects
	Audio::Initialise();
	Graphics::Initialise(argc, argv, this, 768, 800, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();


}


//Clean up the textures
Pacman::~Pacman()
{
	delete _pacman->texture;
	delete _pacman->sourceRect;
	delete _pacman->position;
	delete _pacman;

	
	int nCount = 0;
	for (nCount= 0; nCount < MUNCHIECOUNT; nCount++)
	{
		delete _munchies[nCount]->texture;
		delete _munchies[nCount]->sourceRect;
		delete _munchies[nCount]->position;
		delete _munchies[nCount]->outsideposition;
		delete _munchies[nCount];
	}
	delete[] _munchies;

	delete _cherry->sourceRect;
	delete _cherry->texture;
	delete _cherry->position;
	delete _cherry;
	
	for (nCount = 0; nCount < GHOSTCOUNT; nCount++)
	{
		delete _ghosts[nCount]->texture;
		delete _ghosts[nCount]->sourceRect;
		delete _ghosts[nCount]->position;
	}
	delete[] _ghosts;

	delete _pop;
	delete _bruhgh;
	delete _wow;
}

void Pacman::LoadContent()
{
	_pop->Load("Sounds/pop.wav");
	_bruhgh->Load("Sounds/bruhgh.wav");
	_wow->Load("Sounds/wow.wav");
	// Load Pacman
	_pacman->texture = new Texture2D();
	_pacman->texture->Load("Textures/Pacman.tga", false);
	_pacman->position = new Vector2(350.0f, 350.0f);
	_pacman->sourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Munchies
	Texture2D* munchieTex = new Texture2D();
	munchieTex->Load("Textures/Munchie.png", true);
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i]->texture = munchieTex;
		_munchies[i]->position = new Vector2((rand()% Graphics::GetViewportWidth()),(rand()%Graphics::GetViewportHeight()));
		_munchies[i]->sourceRect = new Rect(0, 0, 12, 12);
	}
	
	//Load cherry
	_cherry->texture = new Texture2D();
	_cherry->texture->Load("Textures/redandblue.png", true);
	_cherry->position = new Vector2(300.0f, 500.0f);
	_cherry->sourceRect = new Rect(0, 0, 32, 40);
	
	// Set string position
	_menu->stringPosition = new Vector2(10.0f, 20.0f);

	//Set menu parameters
	_menu->background = new Texture2D;
	_menu->background->Load("Textures/Transparency.png", false);
	_menu->rectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menu->startPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);

	//Initialise ghost character
	_ghosts[0]->texture = new Texture2D();
	_ghosts[0]->texture->Load("Textures/GhostBlue.png", false);
	_ghosts[1]->texture = new Texture2D();
	_ghosts[1]->texture->Load("Textures/GhostRed.png", false);
	_ghosts[2]->texture = new Texture2D();
	_ghosts[2]->texture->Load("Textures/GhostYellow.png", false);
	_ghosts[3]->texture = new Texture2D();
	_ghosts[3]->texture->Load("Textures/GhostPurple.png", false);

	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		//_ghosts[i]->texture = ghostTex;
		_ghosts[i]->position = new Vector2((rand() % (Graphics::GetViewportWidth()-12)), (rand() % (Graphics::GetViewportHeight()-12)));
		_ghosts[i]->sourceRect = new Rect(0.0f, 0.0f, 20, 20);
	}
	
}

//%10=0-9
//%10+1=1-10
//%9=0-8


void Pacman::Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState)
{
	//Movement of pacman
	float pacmanSpeed = _pacman->speed * elapsedTime * _pacman->speedMultiplier;
	
	{
		if (state->IsKeyDown(Input::Keys::D))
			_move->right = true;
		else _move->right = false;

		if (state->IsKeyDown(Input::Keys::A))
			_move->left = true;
		else _move->left = false;
		
		if (state->IsKeyDown(Input::Keys::S))
			_move->down = true;
		else		_move->down = false;
		
		if (state->IsKeyDown(Input::Keys::W))
			_move->up = true;
		else _move->up = false;
		
		if (_move->right)
		{
			_pacman->position->X += pacmanSpeed;
			_pacman->direction = 0;
		}
		else if (_move->left)
		{
			_pacman->position->X -= pacmanSpeed;
			_pacman->direction = 2;
		}
		else if (_move->up)
		{
			_pacman->position->Y -= pacmanSpeed;
			_pacman->direction = 3;
		}
		else if (_move->down)
		{
			_pacman->position->Y += pacmanSpeed;
			_pacman->direction = 1;
		}
	}

}

void Pacman::CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey)
{
	//Pause screen pop up
	if (state->IsKeyDown(Input::Keys::P) && !_menu->p)
	{
		_menu->p = true;
		_menu->pausedScreen = !_menu->pausedScreen;

	}
	if (state->IsKeyUp(Input::Keys::P))
		_menu->p = false; 
}



void Pacman::CheckStart(Input::KeyboardState* state, Input::Keys startKey)
{
		//start screen pop down
	if (state->IsKeyDown(Input::Keys::SPACE) && !_menu->space)
	{
		_menu->space = true;
		_menu->startScreen = false;
	}
	if (state->IsKeyUp(Input::Keys::SPACE))
		_menu->space = false;
}

void Pacman::CheckViewportCollision()
{
	int WinWidth = Graphics::GetViewportWidth() + 10;
	int WinHeight = Graphics::GetViewportHeight() + 10;
	int point0 = 20;
	//Walls
	if (_pacman->position->X + _pacman->sourceRect->Width > WinWidth)
	{
		_pacman->position->X = point0 - _pacman->sourceRect->Width;
	}
	if (_pacman->position->X + _pacman->sourceRect->Width < point0)
	{
		_pacman->position->X = WinWidth - _pacman->sourceRect->Width;
	}
	if (_pacman->position->Y + _pacman->sourceRect->Width > WinHeight)
	{
		_pacman->position->Y = point0 - _pacman->sourceRect->Width;
	}
	if (_pacman->position->Y + _pacman->sourceRect->Width < point0)
	{
		_pacman->position->Y = WinHeight - _pacman->sourceRect->Width;
	}
}


void Pacman::CheckGhostCollisions()
{
	int i = 0;
	int bottom1 = _pacman->position->Y + _pacman->sourceRect->Height;
	int bottom2 = 0;
	int left1 = _pacman->position->X;
	int left2 = 0;
	int right1 = _pacman->position->X + _pacman->sourceRect->Width;
	int right2 = 0;
	int top1 = _pacman->position->Y;
	int top2 = 0;
	for (i = 0; i < GHOSTCOUNT; i++)
	{
		//populate variables with ghost data
		bottom2 = _ghosts[i]->position->Y + _ghosts[i]->sourceRect->Height;
		left2 = _ghosts[i]->position->X;
		right2= _ghosts[i]->position->X + _ghosts[i]->sourceRect->Width;
		top2 = _ghosts[i]->position->Y;
		
		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			_pacman->dead = true;
			i = GHOSTCOUNT;
			_pacman->score -= _pacman->score;
			Audio::Play(_bruhgh);
		}
	}
}
void Pacman::CheckCherryCollisions()
{
	int i = 0;
	int bottom1 = _pacman->position->Y + _pacman->sourceRect->Height;
	int bottom2 = 0;
	int left1 = _pacman->position->X;
	int left2 = 0;
	int right1 = _pacman->position->X + _pacman->sourceRect->Width;
	int right2 = 0;
	int top1 = _pacman->position->Y;
	int top2 = 0;
	
	
		//populate variables with cherry data
		bottom2 = _cherry->position->Y + _cherry->sourceRect->Height;
		left2 = _cherry->position->X;
		right2 = _cherry->position->X + _cherry->sourceRect->Width;
		top2 = _cherry->position->Y;

		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			_cherry->dead = true;
			_pacman->score += 3000;
			Audio::Play(_wow);
		}
	
}

void Pacman::CheckMunchieCollisions()
{
	int i = 0;
	int bottom1 = _pacman->position->Y + _pacman->sourceRect->Height;
	int bottom2 = 0;
	int left1 = _pacman->position->X;
	int left2 = 0;
	int right1 = _pacman->position->X + _pacman->sourceRect->Width;
	int right2 = 0;
	int top1 = _pacman->position->Y;
	int top2 = 0;
	for (i = 0; i < MUNCHIECOUNT; i++)
	{
		//populate variables with ghost data
		bottom2 = _munchies[i]->position->Y + _munchies[i]->sourceRect->Height;
		left2 = _munchies[i]->position->X;
		right2 = _munchies[i]->position->X + _munchies[i]->sourceRect->Width;
		top2 = _munchies[i]->position->Y;

		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			_munchies[i]->dead = true;
			Audio::Play(_pop);
			_pacman->score = _pacman->score + 1000;
		}
	}
}


void Pacman::UpdatePacman(int elapsedTime)
{
		if (_pacman->currentFrameTime > _pacman->cframeTime)
	{
		_pacman->frame++;
		if (_pacman->frame >= 2)
			_pacman->frame = 0;

		_pacman->currentFrameTime = 0;
	}
	_pacman->sourceRect->X = _pacman->sourceRect->Width * _pacman->frame;

}

void Pacman::UpdateMunchies(Enemy* pEnmy, int elapsedTime)
{
	
	pEnmy->currentFrameTime += elapsedTime;
		if (pEnmy->currentFrameTime > pEnmy->cframeTime)
	{
			pEnmy->frameCount++;
			if (pEnmy->frameCount >= 2)
				pEnmy->frameCount = 0;


			pEnmy->currentFrameTime = 0;

	}
		pEnmy->sourceRect->X = pEnmy->sourceRect->Width * pEnmy->frameCount;
		
}

void Pacman::UpdateCherry(int elapsedTime)
{
	//Timing cherry
	_cherry->currentFrameTime += elapsedTime;
	if (_cherry->currentFrameTime > _cherry->cframeTime)
	{
		_cherry->frameCount++;
		if (_cherry->frameCount >= 2)
			_cherry->frameCount = 0;


		_cherry->currentFrameTime = 0;

	}
	_cherry->sourceRect->X = _cherry->sourceRect->Width * _cherry->frameCount;


}

void Pacman::UpdateGhost(MovingEnemy* ghost, int elapsedTime)
{
	if(ghost->direction==0)
	{
		ghost->position->X += ghost->speed * elapsedTime;
	}
	else if(ghost->direction==1)
	{
		ghost->position->X -= ghost->speed * elapsedTime;
	}
	if (ghost->direction == 2)
	{
		ghost->position->Y += ghost->speed * elapsedTime;
	}
	else if (ghost->direction == 3)
	{
		ghost->position->Y -= ghost->speed * elapsedTime;
	}

	if(ghost->position->X+ghost->sourceRect->Width>=Graphics::GetViewportWidth())
	{
		ghost->direction = 1;
	}
	else if(ghost->position->X<=0)
	{
		ghost->direction = 0;
	}
	if (ghost->position->Y + ghost->sourceRect->Height >= Graphics::GetViewportHeight())
	{
		ghost->direction = 3;
	}
	else if (ghost->position->Y <= 0)
	{
		ghost->direction = 2;
	}
}

void Pacman::UpdateOneGhost(MovingEnemy* ghost, int elapsedTime)
{
	int WinWidth = Graphics::GetViewportWidth() ;
	int WinHeight = Graphics::GetViewportHeight() ;
	int point0 = 20;
	if (ghost->direction == 0)
	{
		ghost->position->X += ghost->speed * elapsedTime;
	}
	else if (ghost->direction == 1)
	{
		ghost->position->X -= ghost->speed * elapsedTime;
	}
	if (ghost->direction == 2)
	{
		ghost->position->Y += ghost->speed * elapsedTime;
	}
	else if (ghost->direction == 3)
	{
		ghost->position->Y -= ghost->speed * elapsedTime;
	}


	if (ghost->position->X + ghost->sourceRect->Width > Graphics::GetViewportWidth())//     ->   0
	{
		ghost->direction = 2 ;
		ghost->position->X = WinWidth - ghost->sourceRect->Width;
	}

	if (ghost->position->X < 0)//                                                           <-   1
	{
		ghost->direction = 3;
		ghost->position->X = point0 - ghost->sourceRect->Width;
	}

	if (ghost->position->Y + ghost->sourceRect->Height > Graphics::GetViewportHeight())//  \|/   2
	{
		ghost->direction = 1;
		ghost->position->Y = WinHeight - ghost->sourceRect->Height;
	}
	
	if (ghost->position->Y < 0)//                                "                         /|\   3                                  "
	{
		ghost->direction = 0;
		ghost->position->Y = point0 - ghost->sourceRect->Height;
	}

	
	
	/*if (ghost->position->X + ghost->sourceRect->Width > WinWidth)
	{
		ghost->position->X = point0 - ghost->sourceRect->Width;
	}
	if (ghost->position->X + ghost->sourceRect->Width < point0)
	{
		ghost->position->X = WinWidth - ghost->sourceRect->Width;
	}
	if (ghost->position->Y + ghost->sourceRect->Width > WinHeight)
	{
		ghost->position->Y = point0 - ghost->sourceRect->Width;
	}
	if (ghost->position->Y + ghost->sourceRect->Width < point0)
	{
		ghost->position->Y = WinHeight - ghost->sourceRect->Width;
	}*/

}
void Pacman::Update(int elapsedTime)
{

	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	Input::MouseState* mouseState = Input::Mouse::GetState();


	if (_menu->startScreen)
	{
		//check for start
		CheckStart(keyboardState, Input::Keys::SPACE);
	}
	else
	{
		CheckPaused(keyboardState, Input::Keys::P);
		if (!_menu->pausedScreen && !_pacman->dead)
		{
			_pacman->currentFrameTime += elapsedTime;
			_pacman->sourceRect->Y = _pacman->sourceRect->Height * _pacman->direction;
			Input(elapsedTime, keyboardState, mouseState);
			CheckViewportCollision();
			UpdatePacman(elapsedTime);
			UpdateCherry(elapsedTime);
			for (int i = 0; i < GHOSTCOUNT - 1; i++)
			{
				UpdateGhost(_ghosts[i], elapsedTime);
			}
			UpdateOneGhost(_ghosts[GHOSTCOUNT - 1], elapsedTime);
			CheckGhostCollisions();
			CheckCherryCollisions();
			CheckMunchieCollisions();

			for (int i = 0; i < MUNCHIECOUNT; i++)
			{
				UpdateMunchies(_munchies[i], elapsedTime);			
			}

			if (keyboardState->IsKeyDown(Input::Keys::R))
			{
				_cherry->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
			}

			if (mouseState->LeftButton == Input::ButtonState::PRESSED)
			{
				_cherry->position->X = mouseState->X;
				_cherry->position->Y = mouseState->Y;
			}

			if (keyboardState->IsKeyDown(Input::Keys::LEFTSHIFT))
			{
				//Apply the multiplier
				_pacman->speedMultiplier = 5.0f;
			}
			else
			{
				//Reset to normal speed
				_pacman->speedMultiplier = 1.0f;
			}

			if (!Audio::IsInitialised())
			{
				std::cout << "Audio in not initialised" << std::endl;
			}

			if (!_pop->IsLoaded())
			{
				std::cout << "_pop member sound effect has not loaded" << std::endl;
			}
			if (!_bruhgh->IsLoaded())
			{
				std::cout << "_bruhgh member sound effect has not loaded" << std::endl;
			}


		}
		if (_pacman->dead)
		{
			if (keyboardState->IsKeyDown(Input::Keys::Y))
			{
				_pacman->dead = false;
				_pacman->position->X = 350.0f;
				_pacman->position->Y = 350.0f;
			}
		}
	}
	
	
	
	


	// old Player movement
	/*if (keyboardState->IsKeyDown(Input::Keys::D))
		_pacmanPosition->X += _cPacmanSpeed * elapsedTime;
	if (keyboardState->IsKeyDown(Input::Keys::A))
		_pacmanPosition->X -= _cPacmanSpeed * elapsedTime;
	if (keyboardState->IsKeyDown(Input::Keys::S))
		_pacmanPosition->Y += _cPacmanSpeed * elapsedTime;
	if (keyboardState->IsKeyDown(Input::Keys::W))
		_pacmanPosition->Y -= _cPacmanSpeed * elapsedTime;
	*/

}

void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << _pacman->position->X << "   Y: " << _pacman->position->Y << "   Score: " << _pacman->score;

	SpriteBatch::BeginDraw(); // Starts Drawing
	if (!_pacman->dead)
	{
		SpriteBatch::Draw(_pacman->texture, _pacman->position, _pacman->sourceRect); // Draws Pacman
	}
	else if(_pacman->dead)
	{
		std::stringstream menuStream;
		menuStream << "A ghost ate you, and you died!\n Press 'Y' to restart;";
		SpriteBatch::Draw(_menu->background, _menu->rectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menu->startPosition, Color::Magenta);
	}
		
	//Draw munchies

	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		if (!_munchies[i]->dead)
		{
			SpriteBatch::Draw(_munchies[i]->texture, _munchies[i]->position, _munchies[i]->sourceRect);
			
		}
		else if (_munchies[i]->dead)
		{
			SpriteBatch::Draw(_munchies[i]->texture, _munchies[i]->position, _munchies[i]->sourceRect);
			_munchies[i]->position->X = -50;
		}
		
	}

	//Draw cherry
		if (!_cherry->dead)
		{
			SpriteBatch::Draw(_cherry->texture, _cherry->position, _cherry->sourceRect);
		}
		else if (_cherry->dead)
		{
			SpriteBatch::Draw(_cherry->texture, _cherry->position, _cherry->sourceRect);
			_cherry->position->X = -50;
		}
	//Draw ghosts
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		SpriteBatch::Draw(_ghosts[i]->texture, _ghosts[i]->position, _ghosts[i]->sourceRect);
	}
	
	
	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _menu->stringPosition, Color::Green);
	if(_menu->pausedScreen)
	{
		std::stringstream menuStream;
		menuStream << "Paused!";
		SpriteBatch::Draw(_menu->background, _menu->rectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menu->startPosition, Color::Red);
	}
	if(_menu->startScreen)
	{
		std::stringstream menuStream;
		menuStream << "Press space to play";
		SpriteBatch::Draw(_menu->background, _menu->rectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menu->startPosition, Color::Cyan);
	}
	SpriteBatch::EndDraw(); // Ends Drawing
}