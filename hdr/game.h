#include <SDL.h>
#include <vector>

struct Vector2
{
	float x;
	float y;
};

struct Ball
{
	Vector2 pos;	// position
	Vector2 vel;	// velocity
};

class Game
{
private:
	bool 			m_IsRunning;
	std::vector<Ball> mBalls;
	Vector2			mPaddle1Pos;
	Vector2 		mPaddle2Pos;
	SDL_Window* 	mWindow;
	SDL_Renderer* 	mRenderer;

	unsigned int 	mTicksCount;
	int 			mPaddle1Dir;
	int 			mPaddle2Dir;
	int 			mAttempts;
public:
	Game();

	bool Initialize();
	void RunLoop();
	void Shutdown();
private:

	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
};
