#include "GamePair.h"

GamePair::GamePair(Client *One, Client *Two)
	:One(One),
	Two(Two)
{

}
GamePair::~GamePair()
{
	delete One;
	One=NULL;
	delete Two;
	Two=NULL;
}