#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "BurgerTimeGame.h"
#include "Minigin.h"

int main(int, char* []) {
	dae::Minigin engine;
	engine.m_pGame = new BurgerTimeGame(engine.m_WindowWidth, engine.m_WindowHeight);
	engine.Run();
	return 0;
}