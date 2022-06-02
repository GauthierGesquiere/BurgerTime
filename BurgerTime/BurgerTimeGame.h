#pragma once
#include "Game.h"

class BurgerTimeGame final : public Game
{
public:
	BurgerTimeGame(unsigned int width, unsigned int height);
	~BurgerTimeGame() override = default;
	BurgerTimeGame(const BurgerTimeGame& other) = delete;
	BurgerTimeGame(BurgerTimeGame&& other) = delete;
	BurgerTimeGame& operator=(const BurgerTimeGame& other) = delete;
	BurgerTimeGame& operator=(BurgerTimeGame&& other) = delete;


	void LoadGame() override;

private:
	void InitInput();
};

