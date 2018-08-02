#include <iostream>
#include <cmath>
#include "ioMod.h"
#include "multibullets.h"
#include "imageFactory.h"
#include "collisionStrategy.h"


MultiBullets::~MultiBullets() { delete strategy; }

MultiBullets::MultiBullets(const std::string& n) :
	name(n),
	myVelocity(
		Gamedata::getInstance().getXmlInt(name+"/speedX"),
		Gamedata::getInstance().getXmlInt(name+"/speedY")
	),
	bulletImages( ImageFactory::getInstance().getImages(name)),
	bulletList(),
	freeList(),
	strategy(NULL)
 {
	const string thisStratety = Gamedata::getInstance().getXmlStr("collisionStrategy");
	if(thisStratety == "PerPixel") {
		strategy = new PerPixelCollisionStrategy;
	} else if(thisStratety == "MidPoint") {
		strategy = new MidPointCollisionStrategy;
	} else if(thisStratety == "Rectangular") {
		  std::cout << "Rectangular strategy!" << std::endl;
		strategy = new RectangularCollisionStrategy;
	}
 }

MultiBullets::MultiBullets(const MultiBullets& b) :
	name(b.name),
	myVelocity(b.myVelocity),
	bulletImages(b.bulletImages),
	bulletList(b.bulletList),
	freeList(b.freeList),
	strategy(b.strategy)
 {}

void MultiBullets::shoot(const Vector2f& pos, const Vector2f& objVel) {
	if( freeList.empty() ) {
		MultiBullet b (name, pos, objVel);
		bulletList.push_back(b);
	} else {
		MultiBullet b = freeList.front();
		freeList.pop_front();
		b.reset();
		b.setVelocity(objVel);
		b.setPosition(pos);
		bulletList.push_back(b);
	}
}

bool MultiBullets::collided(const Drawable* obj)  {
	/*
	for( const auto& bullet : bulletList) {
		if( strategy->execute(bullet, *obj) ) {
			return true;
		}
	}
	*/
	std::list<MultiBullet>::iterator ptr = bulletList.begin();
	while(ptr != bulletList.end()) {
		if( strategy->execute(*ptr, *obj) ) {
			freeList.push_back(*ptr);
			ptr = bulletList.erase(ptr);
			return true;
		} else {
			++ptr;
		}
	}

	return false;
}

void MultiBullets::draw() const {
	for( const auto& bullet : bulletList) {
		bullet.draw();
	}
}

void MultiBullets::update(Uint32 tricks) {
	std::list<MultiBullet>::iterator ptr = bulletList.begin();
	while(ptr != bulletList.end()) {
		ptr->update(tricks);
		if(ptr->goneTooFar()) {
			freeList.push_back(*ptr);
			ptr = bulletList.erase(ptr);
		} else {
			++ptr;
		}
	}
}