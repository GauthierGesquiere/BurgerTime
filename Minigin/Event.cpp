#include "MiniginPCH.h"
#include "Event.h"

dae::Event::~Event()
{
	delete pDataType;
}
