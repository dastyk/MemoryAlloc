#ifndef TESTSTRUCTS_H
#define TESTSTRUCTS_H

#include <random>

struct Enemies //"Object pool" test struct
{
	Enemies() { };
	void Init() { HP = 10; damage = 0; timeToLive = rand() % 20; };
	void Tick() { timeToLive--; };
	bool Alive() { return timeToLive; };
	int HP;
	int timeToLive;
	int damage;
};

#endif // !TESTSTRUCTS_H
