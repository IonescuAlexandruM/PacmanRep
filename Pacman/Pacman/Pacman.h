#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

//declaring the number of types of enemies on the screen
#define MUNCHIECOUNT 50
#define GHOSTCOUNT 4
#define HWALLSCOUNT 14
#define VWALLSCOUNT 12

// Just need to include main header file
#include "S2D/S2D.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;


//Structure of pacman
struct Player
{
	const float speed;
	const int cframeTime ;
	float speedMultiplier;
	int currentFrameTime;
	int direction;
	int frame;
	bool dead;
	Rect* sourceRect;
	Texture2D* texture;
	Vector2* position;
	int frameTime;
	int frameCount;
	int score;
	int lives;
	int numberofMunchies;
		
};
//Structure of munchie
struct Enemy
{
	const int cframeTime;
	const float speed;
	int frameCount;
	Vector2* position;
	Vector2* outsideposition;
	Rect* sourceRect;
	Texture2D* texture;
	int currentFrameTime;
	int direction;
	float cherrySpeed;
	bool dead;


};

//Structure of movement
struct Movement
{
	int right;
	int left;
	int up;
	int down;

};


//STRUCTURE FOR MENU
struct MenuState
{
	Texture2D* background;
	Rect* rectangle;
	Vector2* stringPosition;
	Vector2* startPosition;
	bool pausedScreen;
	bool startScreen;
	bool p;
	bool space;
	

};

//Structure for enemy movement
struct MovingEnemy
{
	Vector2* position;
	Texture2D* texture;
	Rect* sourceRect;
	int direction;
	float speed;

};





// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public Game
{
private:
/*	// Data to represent Pacman
	Vector2* _pacmanPosition;
	Rect* _pacmanSourceRect;
	Texture2D* _pacmanTexture;

	// Data to represent Munchie
	int _munchieFrameCount;
	Vector2* _munchiePosition;
	Rect* _munchieSourceRect;
	Texture2D* _munchieBlueRedTexture;
	


	//Data to represent cherry
	int _cherryFrameCount;
	Vector2* _cherryPosition;
	Rect* _cherrySourceRect;
	Texture2D* _cherryTexture;


	// Position for String
	Vector2* _stringPosition;

	//Speed
	const float _cPacmanSpeed;

	//Data for menu
	Texture2D* _menuBackground;
	Vector2* _menuStringPosition;
	Rect* _menuRectangle;
	bool _paused;
	bool _pKeyDown;
	bool _spaceKeyDown;
	bool _StartScreen;
	
	
	//Moving only XY axes
	bool _rightmove;
	bool _leftmove;
	bool _upmove;
	bool _downmove;

	//Animating pacman
	int _pacmanDirection;
	int _pacmanFrame;
	int _pacmanCurrentFrameTime;
	const int _cPacmanFrameTime;

	//Animating munchie
	const int _cMunchieFrameTime;
	int _munchieFrame;
	int _munchieCurrentFrameTime;

	//Animating cherry
	const int _ccherryFrameTime;
	int _cherryCurrentFrameTime;
	int _cherryFrame;



*/
	Player* _pacman;
	Enemy* _munchies[MUNCHIECOUNT];
	Enemy* _cherry;

	Enemy* _hwalls[HWALLSCOUNT];
	Enemy* _vwalls[VWALLSCOUNT];
	Movement* _move;

	MenuState* _menu;
	MovingEnemy* _ghosts[GHOSTCOUNT];
	SoundEffect* _pop;
	SoundEffect* _wow;
	SoundEffect* _backsound;
	
	
	
	void UpdateGhost(MovingEnemy*, int elapsedTime);
	void UpdateOneGhost(MovingEnemy*, int elapsedTime);

public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);

	//Input methods
	void Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState);

	//Check methods
	void CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey);
	void CheckStart(Input::KeyboardState* state, Input::Keys startKey);
	void CheckGhostCollisions();
	void CheckCherryCollisions();
	void CheckMunchieCollisions();
	void CheckHorizontalWallsCollision(int elapsedTime);
	void CheckVerticalWallsCollision(int elapsedTime);
	bool CheckCollision(int x1, int y1, int width1, int height1, int x2, int y2, int widht2, int height2);

	//Update methods
	void UpdatePacman(int elapsedTime);
	void UpdateCherry(int elapsedTime);
	void UpdateMunchies(Enemy*, int elapsedTime);
	
	
};