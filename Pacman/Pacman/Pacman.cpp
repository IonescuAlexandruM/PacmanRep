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
	_pacman->lives = 3;
	_pacman->numberofMunchies = 0;
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
	for (i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i] = new Enemy{ 500 };
		_munchies[i]->frameCount = rand() % 1;
		_munchies[i]->currentFrameTime = 0;
		_munchies[i]->frameCount = rand() % 500 + 50;
		_munchies[i]->dead = false;
	}

	for (i = 0; i < HWALLSCOUNT; i++)
	{
		_hwalls[i] = new Enemy{ 500 };
	}

	for (i = 0; i < VWALLSCOUNT; i++)
	{
		_vwalls[i] = new Enemy{ 500 };
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
	_pop = new SoundEffect(false, 1, 0.5);
	_wow = new SoundEffect(false, 1, 0.5);
	_backsound = new SoundEffect(true, 1, 0.07);

	//Initialise important Game aspects
	Audio::Initialise();
	Graphics::Initialise(argc, argv, this, 750, 800, false, 25, 25, "Pacman", 60);
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
	for (nCount = 0; nCount < MUNCHIECOUNT; nCount++)
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

	for (nCount = 0; nCount < HWALLSCOUNT; nCount++)
	{
		delete _hwalls[nCount]->texture;
		delete _hwalls[nCount]->sourceRect;
		delete _hwalls[nCount]->position;
	}
	delete[] _hwalls;

	for (nCount = 0; nCount < VWALLSCOUNT; nCount++)
	{
		delete _vwalls[nCount]->texture;
		delete _vwalls[nCount]->sourceRect;
		delete _vwalls[nCount]->position;
	}
	delete[] _vwalls;

	delete _menu->background;
	delete _menu->rectangle;


	delete _pop;
	delete _wow;
	delete _backsound;
}

void Pacman::LoadContent()
{
	//Load sounds
	_pop->Load("Sounds/pop.wav");
	_wow->Load("Sounds/wow.wav");
	_backsound->Load("Sounds/why-we-lose.wav");
	Audio::Play(_backsound);
	Audio::Pause(_backsound);

	// Load Pacman
	_pacman->texture = new Texture2D();
	_pacman->texture->Load("Textures/Pacman.tga", false);
	_pacman->position = new Vector2(235.0f, 220.0f);
	_pacman->sourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Munchies
	Texture2D* munchieTex = new Texture2D();
	munchieTex->Load("Textures/Munchie.png", true);
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i]->texture = munchieTex;
		
	}

	float j;
 j = 54.0f;
	for (int i = 0; i < 8; i++)
	{
		
			_munchies[i]->position = new Vector2(33.0f,j);
			if (i < 1)
				j = j + 60.0f;
			else if (i == 2)
				j = j + 150.0f;
			else if (i == 4)
			{
				j = j + 110.0f;
				_munchies[i]->position = new Vector2(33.0f, 445.0f);
			}
			else if (i == 6)
				j = j + 60.0f;
			else j = j + 110.0f;
	}
	 j = 54.0f;
	 for (int i = 8; i < 16; i++)
	 {

		 _munchies[i]->position = new Vector2(149.0f, j);
		 if (i < 9)
			 j = j + 60.0f;
		 else if (i == 10)
			 j = j + 150.0f;
		 else if (i == 12)
		 {
			 j = j + 110.0f;
			 _munchies[i]->position = new Vector2(149.0f, 445.0f);
		 }
		 else if (i == 14)
			 j = j + 60.0f;
		 else j = j + 110.0f;
	 }
	 j = 54.0f;
	for (int i = 16; i < 24; i++)
	{
		
		_munchies[i]->position = new Vector2(341.0f,j);
		if (i <17)
			j = j + 60.0f;
		else if (i == 18)
			j = j + 150.0f;
		else if (i == 20)
		{
			j = j + 110.0f;
			_munchies[i]->position = new Vector2(341.0f, 445.0f);
		}
		else if (i == 22)
			j = j + 60.0f;
		else j = j + 110.0f;
	}
	j = 54.0f;
	for (int i = 24; i < 32; i++)
	{
		 
		_munchies[i]->position = new Vector2( 427.0f,j);
		if (i <25)
			j = j + 60.0f;
		else if (i == 26)
			j = j + 150.0f;
		else if (i == 28)
		{
			j = j + 110.0f;
			_munchies[i]->position = new Vector2(427.0f, 445.0f);
		}
		else if (i == 30)
			j = j + 60.0f;
		else j = j + 110.0f;
	}
	j = 54.0f;
	for (int i = 32; i < 40; i++)
	{
		 
		_munchies[i]->position = new Vector2( 546.0f,j);
		if (i < 33)
			j = j + 60.0f;
		else if (i == 34)
			j = j + 150.0f;
		else if (i == 36)
		{
			j = j + 110.0f;
			_munchies[i]->position = new Vector2(546.0f, 445.0f);
		}
		else if (i == 38)
			j = j + 60.0f;
		else j = j + 110.0f;
	}
	 j = 54.0f;
	for (int i = 40; i < 48; i++)
	{
		
		_munchies[i]->position = new Vector2( 710.0f,j);
		if (i <41)
			j = j + 60.0f;
		else if (i ==42)
			j = j + 150.0f;
		else if (i == 44)
		{
			j = j + 110.0f;
			_munchies[i]->position = new Vector2(710.0f, 445.0f);
		}
		else if (i == 46)
			j = j + 60.0f;
		else j = j + 110.0f;
	}
	j = 54.0f;
	for (int i = 48; i < MUNCHIECOUNT; i++)
	{
		 
		_munchies[i]->position = new Vector2(385.0f,415.0f);
		
	}


	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
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
	_menu->background = new Texture2D();
	_menu->background->Load("Textures/Transparency.png", false);
	_menu->rectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menu->startPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f - 80, Graphics::GetViewportHeight() / 2.0f);

	//Set walls
	for (int i = 0; i < VWALLSCOUNT; i++)
	{
		_vwalls[i]->texture = new Texture2D();
		_vwalls[i]->texture->Load("Textures/Vwalls.psd", true);
	}
	_vwalls[0]->position = new Vector2(0.0f, 20.0f);
	_vwalls[1]->position = new Vector2(0.0f, 270.0f);
	_vwalls[2]->position = new Vector2(0.0f, 540.0f);
	_vwalls[3]->position = new Vector2(60.0f, 70.0f);
	_vwalls[4]->position = new Vector2(60.0f, 460.0f);
	_vwalls[5]->position = new Vector2(239.0f, 273.0f);
	_vwalls[6]->position = new Vector2(513.0f, 273.0f);
	_vwalls[7]->position = new Vector2(674.0f, 70.0f);
	_vwalls[8]->position = new Vector2(674.0f, 460.0f);
	_vwalls[9]->position = new Vector2(736.0f, 20.0f);
	_vwalls[10]->position = new Vector2(736.0f, 270.0f);
	_vwalls[11]->position = new Vector2(736.0f, 540.0f);
	
	for (int i = 0; i < VWALLSCOUNT; i++)
	{
		_vwalls[i]->sourceRect = new Rect(0, 0, 16, 290);
	}

	for (int i = 0; i < HWALLSCOUNT; i++)
	{
		_hwalls[i]->texture = new Texture2D();
		_hwalls[i]->texture->Load("Textures/Hwalls.psd", true);
	}
	_hwalls[0]->position = new Vector2(0.0f, 20.0f);
	_hwalls[1]->position = new Vector2(270.0f, 20.0f);
	_hwalls[2]->position = new Vector2(540.0f, 20.0f);
	_hwalls[3]->position = new Vector2(60.0f, 70.0f);
	_hwalls[4]->position = new Vector2(400.0f, 70.0f);
	_hwalls[5]->position = new Vector2(239.0f, 150.0f);
	_hwalls[6]->position = new Vector2(-100.0f, 410.0f);
	_hwalls[7]->position = new Vector2(577.0f, 410.0f);
	_hwalls[8]->position = new Vector2(239.0f, 654.0f);
	_hwalls[9]->position = new Vector2(60.0f, 734.0f);
	_hwalls[10]->position = new Vector2(400.0f, 734.0f);
	_hwalls[11]->position = new Vector2(0.0f, 784.0f);
	_hwalls[12]->position = new Vector2(270.0f, 784.0f);
	_hwalls[13]->position = new Vector2(540.0f, 784.0f);

	for (int i = 0; i < HWALLSCOUNT; i++)
	{
		_hwalls[i]->sourceRect = new Rect(0, 0, 290, 16);
	}

	//Initialise ghost character
	_ghosts[0]->texture = new Texture2D();
	_ghosts[0]->texture->Load("Textures/GhostBlue.png", false);
	_ghosts[1]->texture = new Texture2D();
	_ghosts[1]->texture->Load("Textures/GhostRed.png", false);
	_ghosts[2]->texture = new Texture2D();
	_ghosts[2]->texture->Load("Textures/GhostYellow.png", false);
	_ghosts[3]->texture = new Texture2D();
	_ghosts[3]->texture->Load("Textures/GhostPurple.png", false);

	_ghosts[0]->position = new Vector2(21,760);
	_ghosts[1]->position = new Vector2(80,110);
	_ghosts[2]->position = new Vector2(200,170);
	_ghosts[3]->position = new Vector2(21,380);

	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i]->sourceRect = new Rect(0.0f, 0.0f, 20, 20);
	}
}



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

//void Pacman::CheckViewportCollision()
//{
//	int WinWidth = Graphics::GetViewportWidth();
//	int WinHeight = Graphics::GetViewportHeight();
//	int point0 = 25;
//	//Walls
//
//	if (_pacman->position->X + _pacman->sourceRect->Width > WinWidth - 25)//     ->   0
//	{
//		_pacman->position->X = WinWidth - _pacman->sourceRect->Width - 25;
//	}
//
//	if (_pacman->position->X < 25)//                                                           <-   1
//	{
//
//		_pacman->position->X = point0 - _pacman->sourceRect->Width + 33;
//	}
//
//	if (_pacman->position->Y + _pacman->sourceRect->Height > Graphics::GetViewportHeight() - 25)//  \|/   2
//	{
//
//		_pacman->position->Y = WinHeight - _pacman->sourceRect->Height - 25;
//	}
//
//	if (_pacman->position->Y < 45)//                                "                         /|\   3                                  "
//	{
//
//		_pacman->position->Y = point0 - _pacman->sourceRect->Height + 53;
//	}
//}


void Pacman::CheckGhostCollisions()
{
	int i = 0;
	for (i = 0; i < GHOSTCOUNT; i++)
	{
		if (CheckCollision(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height, _ghosts[i]->position->X, _ghosts[i]->position->Y, _ghosts[i]->sourceRect->Width, _ghosts[i]->sourceRect->Height))
		{
			_pacman->dead = true;
			_pacman->score = 0;
			_pacman->lives -= 1;
		}
	}

}
void Pacman::CheckCherryCollisions()
{
	if (CheckCollision(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height, _cherry->position->X, _cherry->position->Y, _cherry->sourceRect->Width, _cherry->sourceRect->Height))
	{
		_cherry->dead = true;
		Audio::Play(_wow);
		_pacman->score = _pacman->score + 3000;
	}
}

void Pacman::CheckMunchieCollisions()
{
	int i;
	for (i = 0; i < MUNCHIECOUNT; i++)
	{
		if (CheckCollision(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height, _munchies[i]->position->X, _munchies[i]->position->Y, _munchies[i]->sourceRect->Width, _munchies[i]->sourceRect->Height))
		{
			_munchies[i]->dead = true;
			Audio::Play(_pop);
			_pacman->score = _pacman->score + 1000;
			_pacman->numberofMunchies += 1;
		}
	}
}

bool Pacman::CheckCollision(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2)
{
	int left1 = x1;
	int left2 = x2;
	int right1 = x1 + width1;
	int right2 = x2 + width2;
	int top1 = y1;
	int top2 = y2;
	int bottom1 = y1 + height1;
	int bottom2 = y2 + height2;

	if (bottom1 < top2)
		return false;
	if (top1 > bottom2)
		return false;
	if (right1 < left2)
		return false;
	if (left1 > right2)
		return false;

	return true;

}

void Pacman::CheckHorizontalWallsCollision(int elapsedTime)
{
	int i = 0;

	//populate variables with walls
	for (int i = 0; i < HWALLSCOUNT; i++)
	{
		if (CheckCollision(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height, _hwalls[i]->position->X, _hwalls[i]->position->Y, _hwalls[i]->sourceRect->Width, _hwalls[i]->sourceRect->Height))
		{
			if (_pacman->direction == 0)
				_pacman->position->X = _hwalls[i]->position->X - _pacman->sourceRect->Width - 1;
			if (_pacman->direction == 1)
				_pacman->position->Y = _hwalls[i]->position->Y - _pacman->sourceRect->Height - 1;
			if (_pacman->direction == 2)
				_pacman->position->X = _hwalls[i]->position->X + _hwalls[i]->sourceRect->Width + 1;
			if (_pacman->direction == 3)
				_pacman->position->Y = _hwalls[i]->position->Y + _hwalls[i]->sourceRect->Height + 1;
		}
	}
}

void Pacman::CheckVerticalWallsCollision(int elapsedTime)
{
	int i = 0;


	//populate variables with walls
	for (int i = 0; i < VWALLSCOUNT; i++)
	{
		if (CheckCollision(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height, _vwalls[i]->position->X, _vwalls[i]->position->Y, _vwalls[i]->sourceRect->Width, _vwalls[i]->sourceRect->Height))
		{
			if (_pacman->direction == 0)
				_pacman->position->X = _vwalls[i]->position->X - _pacman->sourceRect->Width - 1;
			if (_pacman->direction == 1)
				_pacman->position->Y = _vwalls[i]->position->Y - _pacman->sourceRect->Height - 1;
			if (_pacman->direction == 2)
				_pacman->position->X = _vwalls[i]->position->X + _vwalls[i]->sourceRect->Width + 1;
			if (_pacman->direction == 3)
				_pacman->position->Y = _vwalls[i]->position->Y + _vwalls[i]->sourceRect->Height + 1;
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

	for (int i = 0; i < VWALLSCOUNT; i++)
		if (CheckCollision(ghost->position->X, ghost->position->Y, ghost->sourceRect->Width, ghost->sourceRect->Height, _vwalls[i]->position->X, _vwalls[i]->position->Y, _vwalls[i]->sourceRect->Width, _vwalls[i]->sourceRect->Height))
		{
			if (ghost->direction == 0)
			{
				ghost->direction = 1;
				ghost->position->X = _vwalls[i]->position->X - ghost->sourceRect->Width - 4;
			}
			else if (ghost->direction == 1)
			{
				ghost->direction = 0;
				ghost->position->X = _vwalls[i]->position->X + _vwalls[i]->sourceRect->Width + 4;
			}
			else if (ghost->direction == 2)
			{
				ghost->direction = 3;
				ghost->position->Y = _vwalls[i]->position->Y - ghost->sourceRect->Height - 4;
			}
			else if (ghost->direction == 3)
			{
				ghost->direction = 2;
				ghost->position->Y = _vwalls[i]->position->Y + _vwalls[i]->sourceRect->Height + 4;
			}
		}

	for (int i = 0; i < HWALLSCOUNT; i++)
		if (CheckCollision(ghost->position->X, ghost->position->Y, ghost->sourceRect->Width, ghost->sourceRect->Height, _hwalls[i]->position->X, _hwalls[i]->position->Y, _hwalls[i]->sourceRect->Width, _hwalls[i]->sourceRect->Height))
		{
			if (ghost->direction == 0)
			{
				ghost->direction = 1;
				ghost->position->X = _hwalls[i]->position->X - ghost->sourceRect->Width - 4;
			}
			else if (ghost->direction == 1)
			{
				ghost->direction = 0;
				ghost->position->X = _hwalls[i]->position->X + _hwalls[i]->sourceRect->Width + 4;
			}
			else if (ghost->direction == 2)
			{
				ghost->direction = 3;
				ghost->position->Y = _hwalls[i]->position->Y - ghost->sourceRect->Height - 4;
			}
			else if (ghost->direction == 3)
			{
				ghost->direction = 2;
				ghost->position->Y = _hwalls[i]->position->Y + _hwalls[i]->sourceRect->Height + 4;
			}
		}
}

void Pacman::UpdateOneGhost(MovingEnemy* ghost, int elapsedTime)
{
	int k = 0;
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

	for (int i = 0; i < VWALLSCOUNT; i++)
		if (CheckCollision(ghost->position->X, ghost->position->Y, ghost->sourceRect->Width, ghost->sourceRect->Height, _vwalls[i]->position->X, _vwalls[i]->position->Y, _vwalls[i]->sourceRect->Width, _vwalls[i]->sourceRect->Height))
		{
			if (ghost->direction == 0)
			{
				ghost->direction = 2;
				ghost->position->X = _vwalls[i]->position->X - ghost->sourceRect->Width - 4;
			}
			else if (ghost->direction == 1)
			{
				ghost->direction = 3;
				ghost->position->X = _vwalls[i]->position->X + _vwalls[i]->sourceRect->Width + 4;
			}
			else if (ghost->direction == 2)
			{
				ghost->direction = 1;
				ghost->position->Y = _vwalls[i]->position->Y - ghost->sourceRect->Height - 4;
			}
			else if (ghost->direction == 3)
			{
				ghost->direction = 0;
				ghost->position->Y = _vwalls[i]->position->Y + _vwalls[i]->sourceRect->Height + 4;
			}
		}

	for (int i = 0; i < HWALLSCOUNT; i++)
		if (CheckCollision(ghost->position->X, ghost->position->Y, ghost->sourceRect->Width, ghost->sourceRect->Height, _hwalls[i]->position->X, _hwalls[i]->position->Y, _hwalls[i]->sourceRect->Width, _hwalls[i]->sourceRect->Height))
		{
			if (ghost->direction == 0)
			{
				ghost->direction = 2;
				ghost->position->X = _hwalls[i]->position->X - ghost->sourceRect->Width - 4;
			}
			else if (ghost->direction == 1)
			{
				ghost->direction = 3;
				ghost->position->X = _hwalls[i]->position->X + _hwalls[i]->sourceRect->Width + 4;
			}
			else if (ghost->direction == 2)
			{
				ghost->direction = 1;
				ghost->position->Y = _hwalls[i]->position->Y - ghost->sourceRect->Height - 4;
			}
			else if (ghost->direction == 3)
			{
				ghost->direction = 0;
				ghost->position->Y = _hwalls[i]->position->Y + _hwalls[i]->sourceRect->Height + 4;
			}
		}
}

void Pacman::Update(int elapsedTime)
{
	Audio::Pause(_backsound);
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
		if (!_pacman->dead)
		{
			CheckPaused(keyboardState, Input::Keys::P);
			if (!_menu->pausedScreen)
			{
				if (_pacman->numberofMunchies < MUNCHIECOUNT)
				{
					Audio::Resume(_backsound);
					_pacman->currentFrameTime += elapsedTime;
					_pacman->sourceRect->Y = _pacman->sourceRect->Height * _pacman->direction;
					Input(elapsedTime, keyboardState, mouseState);
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
					CheckHorizontalWallsCollision(elapsedTime);
					CheckVerticalWallsCollision(elapsedTime);
					bool CheckCollision(int x1, int y1, int weigth1, int height1, int x2, int y2, int weight2, int height2);

					for (int i = 0; i < MUNCHIECOUNT; i++)
					{
						UpdateMunchies(_munchies[i], elapsedTime);
					}
					if (!_cherry->dead)
						if (keyboardState->IsKeyDown(Input::Keys::R))
						{
							_cherry->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
						}
					if (!_cherry->dead)
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
					if (!_wow->IsLoaded())
					{
						std::cout << "_wow member sound effect has not loaded" << std::endl;
					}
					if (!_backsound->IsLoaded())
					{
						std::cout << "_backsound member sound effect has not loaded" << std::endl;
					}
				}
				else if (_pacman->numberofMunchies >= MUNCHIECOUNT)
				{
					Audio::Resume(_backsound);
				}
			}
		}
		if (_pacman->dead && _pacman->lives > 0)

			if (keyboardState->IsKeyDown(Input::Keys::Y))
			{
				_pacman->dead = false;
				_pacman->position->X = 235.0f;
				_pacman->position->Y = 220.0f;
			}

	}
}

void Pacman::Draw(int elapsedTime)
{
// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << _pacman->position->X << "   Y: " << _pacman->position->Y << "   Score: " << _pacman->score << "   Lives: " << _pacman->lives;


	SpriteBatch::BeginDraw(); // Starts Drawing
	if (!_pacman->dead)
	{
		SpriteBatch::Draw(_pacman->texture, _pacman->position, _pacman->sourceRect); // Draws Pacman
	}



//Draw walls
	for (int i = 0; i < HWALLSCOUNT; i++)
	{
		SpriteBatch::Draw(_hwalls[i]->texture, _hwalls[i]->position, _hwalls[i]->sourceRect);
	}

	for (int i = 0; i < VWALLSCOUNT; i++)
	{
		SpriteBatch::Draw(_vwalls[i]->texture, _vwalls[i]->position, _vwalls[i]->sourceRect);
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

	if (_pacman->dead)
		if (_pacman->lives > 0)
		{
			std::stringstream menuStream;
			menuStream << "A ghost ate you, and you died!\n Press 'Y' to try again;";
			SpriteBatch::Draw(_menu->background, _menu->rectangle, nullptr);
			SpriteBatch::DrawString(menuStream.str().c_str(), _menu->startPosition, Color::Magenta);
		}

		else if (_pacman->lives <= 0)
		{
			std::stringstream menuStream;
			menuStream << "You lost the game. Your score is: " << _pacman->score;
			SpriteBatch::Draw(_menu->background, _menu->rectangle, nullptr);
			SpriteBatch::DrawString(menuStream.str().c_str(), _menu->startPosition, Color::White);
		}

	if (_pacman->numberofMunchies == MUNCHIECOUNT)
	{
		std::stringstream menuStream;
		menuStream << "Congratulations!!\nYou won the game.\nYour score is: " << _pacman->score;;
		SpriteBatch::Draw(_menu->background, _menu->rectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menu->startPosition, Color::White);
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
	if (_menu->pausedScreen)
	{
		std::stringstream menuStream;
		menuStream << "Paused!";
		SpriteBatch::Draw(_menu->background, _menu->rectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menu->startPosition, Color::Red);
	}
	if (_menu->startScreen)
	{
		std::stringstream menuStream;
		menuStream << "Press space to play";
		SpriteBatch::Draw(_menu->background, _menu->rectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menu->startPosition, Color::Cyan);
	}
	SpriteBatch::EndDraw(); // Ends Drawing
}