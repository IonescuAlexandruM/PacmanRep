#include "Pacman.h"

#include <sstream>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(3.5f), _cPacmanFrameTime(250)
{
	//Initialization of all starting points of the game
	_frameCount = 0;
	_paused = false;
	_pKeyDown = false;
	_StartScreen = true;
	_spaceKeyDown = false;
	_leftmove = false;
	_rightmove = false;
	_upmove = false;
	_downmove = false;
	_pacmanDirection = 0;
	_pacmanCurrentFrameTime = 0;
	_pacmanFrame = 0;



	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();


}

Pacman::~Pacman()
{
	delete _pacmanTexture;
	delete _pacmanSourceRect;
	delete _munchieBlueTexture;
	delete _munchieInvertedTexture;
	delete _munchieRect;
}

void Pacman::LoadContent()
{
	// Load Pacman
	_pacmanTexture = new Texture2D();
	_pacmanTexture->Load("Textures/Pacman.tga", false);
	_pacmanPosition = new Vector2(350.0f, 350.0f);
	_pacmanSourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Munchie
	_munchieBlueTexture = new Texture2D();
	_munchieBlueTexture->Load("Textures/Munchie.tga", true);
	_munchieInvertedTexture = new Texture2D();
	_munchieInvertedTexture->Load("Textures/MunchieInverted.tga", true);
	_munchieRect = new Rect(100.0f, 450.0f, 12, 12);

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	//Set menu parameters
	_menuBackground = new Texture2D;
	_menuBackground->Load("Textures/Transparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
}
void Pacman::Update(int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	_pacmanCurrentFrameTime += elapsedTime;
	_pacmanSourceRect->Y = _pacmanSourceRect->Height * _pacmanDirection;

	int WinWidth = Graphics::GetViewportWidth()+10;
	int WinHeight = Graphics::GetViewportHeight()+10;
	int point0 = 20;

	// Player movement
	/*if (keyboardState->IsKeyDown(Input::Keys::D))
		_pacmanPosition->X += _cPacmanSpeed * elapsedTime;
	if (keyboardState->IsKeyDown(Input::Keys::A))
		_pacmanPosition->X -= _cPacmanSpeed * elapsedTime;
	if (keyboardState->IsKeyDown(Input::Keys::S))
		_pacmanPosition->Y += _cPacmanSpeed * elapsedTime;
	if (keyboardState->IsKeyDown(Input::Keys::W))
		_pacmanPosition->Y -= _cPacmanSpeed * elapsedTime;
	*/
	//Walls
	if (_pacmanPosition->X + _pacmanSourceRect-> Width>WinWidth)
	{
		_pacmanPosition->X = point0 - _pacmanSourceRect->Width;
	}
	if (_pacmanPosition->X + _pacmanSourceRect->Width < point0)
	{
		_pacmanPosition->X = WinWidth - _pacmanSourceRect->Width;
	}
	if (_pacmanPosition->Y + _pacmanSourceRect->Width > WinHeight)
	{
		_pacmanPosition->Y = point0 - _pacmanSourceRect->Width;
	}
	if (_pacmanPosition->Y + _pacmanSourceRect->Width < point0)
	{
		_pacmanPosition->Y = WinHeight - _pacmanSourceRect->Width;
	}

	//Pause screen pop up
	if (keyboardState->IsKeyDown(Input::Keys::P) && !_pKeyDown)
	{
		_pKeyDown = true;
		_paused = !_paused;

	}
	if (keyboardState->IsKeyUp(Input::Keys::P))
		_pKeyDown = false;
	
	//start screen pop down
	if(keyboardState->IsKeyDown(Input::Keys::SPACE) && !_spaceKeyDown)
	{
		_spaceKeyDown = true;
		_StartScreen = false;
	}
	if (keyboardState->IsKeyUp(Input::Keys::SPACE))
		_spaceKeyDown = false;

	//Movement of pacman
	if (!_paused && !_StartScreen)
	{
		if (keyboardState->IsKeyDown(Input::Keys::D))
			_rightmove = true;
		if (keyboardState->IsKeyDown(Input::Keys::A))
			_leftmove = true;
		if (keyboardState->IsKeyDown(Input::Keys::S))
			_downmove = true;
		if (keyboardState->IsKeyDown(Input::Keys::W))
			_upmove = true;

		if (keyboardState->IsKeyUp(Input::Keys::D))
			_rightmove = false;
		if (keyboardState->IsKeyUp(Input::Keys::A))
			_leftmove = false;
		if (keyboardState->IsKeyUp(Input::Keys::S))
			_downmove = false;
		if (keyboardState->IsKeyUp(Input::Keys::W))
			_upmove = false;

		if (_rightmove)
		{
			_pacmanPosition->X += _cPacmanSpeed * elapsedTime;
			_pacmanDirection = 0;
		}
		else if (_leftmove)
		{
			_pacmanPosition->X -= _cPacmanSpeed * elapsedTime;
			_pacmanDirection = 2;
		}
		else if (_upmove)
		{
			_pacmanPosition->Y -= _cPacmanSpeed * elapsedTime;
			_pacmanDirection = 3;
		}
		else if (_downmove)
		{
			_pacmanPosition->Y += _cPacmanSpeed * elapsedTime;
			_pacmanDirection = 1;
		}
	
		if(_pacmanCurrentFrameTime>_cPacmanFrameTime)
		{
			_pacmanFrame++;
			if (_pacmanFrame >= 2)
				_pacmanFrame = 0;

			_pacmanCurrentFrameTime = 0;
		}
		_pacmanSourceRect->X = _pacmanSourceRect->Width * _pacmanFrame;

		_frameCount++;
	}
	
}

void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << _pacmanPosition->X << " Y: " << _pacmanPosition->Y;

	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::Draw(_pacmanTexture, _pacmanPosition, _pacmanSourceRect); // Draws Pacman

	if (_frameCount < 30)
	{
		// Draws Red Munchie
		SpriteBatch::Draw(_munchieInvertedTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

		
	}
	else
	{
		// Draw Blue Munchie
		SpriteBatch::Draw(_munchieBlueTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
		
		

		if (_frameCount >= 60)
			_frameCount = 0;
	}
	
	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	if(_paused)
	{
		std::stringstream menuStream;
		menuStream << "Paused!";
		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);
	}
	if(_StartScreen)
	{
		std::stringstream menuStream;
		menuStream << "Press space to play";
		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Cyan);
	}
	SpriteBatch::EndDraw(); // Ends Drawing
}