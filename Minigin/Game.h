#pragma once
class Game
{
public:
	Game() = default;
	virtual ~Game() = default;
	Game(const Game& other) = delete;
	Game(Game&& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game& operator=(Game&& other) = delete;
	
	virtual void LoadGame() = 0;

protected:
	unsigned int m_WindowWidth;
	unsigned int m_WindowHeight{};
};

