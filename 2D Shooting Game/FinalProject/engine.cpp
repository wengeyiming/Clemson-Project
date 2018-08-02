#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "smartSprite.h"
#include "multisprite.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
#include "collisionStrategy.h"
#include "player.h"
#include "lifeSprite.h"
#include "bossSprite.h"
#include "sound.h"

auto Less = [](const Drawable* ith, const Drawable* ithplusfirsteth) {
  return ith->getScale() < ithplusfirsteth->getScale();
};


class SpriteLess {
public:
  bool operator()(const Drawable* lhs, const Drawable* rhs) const {
    return lhs->getScale() < rhs->getScale();
  }
};

Engine::~Engine() { 
  for ( SmartSprite* sprite : backgroundVector ) {
    delete sprite;
  }
  for ( SmartSprite* sprite : spritesVector ) {
    delete sprite;
  }
  for ( CollisionStrategy* strategy : strategiesVector ) {
    delete strategy;
  }  

  for ( LifeSprite* life : lifeVector ) {
    delete life;
  }

  for ( MultiSprite* coin : coinVector ) {
    delete coin;
  }

  delete player;
  delete boss;
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  hud( Hud::getInstance() ),
  endHud ( EndHud::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  background("background", Gamedata::getInstance().getXmlInt("background/factor")),
  foreground("foreground", Gamedata::getInstance().getXmlInt("foreground/factor")),
  viewport( Viewport::getInstance() ),
  player(new Player("SoldierAttack")),
  boss(new BossSprite("Scarlet")),
  poolhud(*player),
  spritesVector(),
  strategiesVector(),
  lifeVector(),
  coinVector(),
  backgroundVector(),
  collision(false),
  makeVideo(false)
{
  int n = Gamedata::getInstance().getXmlInt("numberOfSmart");
  spritesVector.reserve(n);
  Vector2f pos = player->getPosition();
  int w = player->getScaledWidth();
  int h = player->getScaledHeight();
  for (int i = 0; i < n; ++i) {
    spritesVector.push_back( new SmartSprite("Fish", pos, w, h) );
    player->attach( spritesVector[i] );

    SmartSprite* s = new SmartSprite("Fish1", pos, w, h);
    float scale = Gamedata::getInstance().getRandFloat(0.3,0.5);
    s->setScale(scale);
    backgroundVector.push_back(s);
  }

  strategiesVector.push_back( new RectangularCollisionStrategy );
  strategiesVector.push_back( new PerPixelCollisionStrategy );
  strategiesVector.push_back( new MidPointCollisionStrategy );


  int lifeNumber = Gamedata::getInstance().getXmlInt("numberOfLife");
  lifeVector.reserve(lifeNumber);
  for (int i = 0; i < lifeNumber; ++i) {
    lifeVector.push_back(new LifeSprite("Heart",pos,Vector2f(0,0),pos,w,h));
  }  
  coinVector.push_back(new MultiSprite("Coin"));
  coinVector.push_back(new MultiSprite("Coin1"));
  coinVector.push_back(new MultiSprite("Coin2"));
  //coinVector.push_back(new MultiSprite("ManWalk"));
  Viewport::getInstance().setObjectToTrack(player);
  std::cout << "Loading complete" << std::endl;

}

void Engine::draw() const {
  background.draw();
  for(unsigned int i = 0; i < backgroundVector.size();i++) {
      backgroundVector.at(i)->draw();
  }
  foreground.draw();
  player->draw();
  boss->draw();
  poolhud.draw();
  for(unsigned int i = 0; i < spritesVector.size();i++) {
      spritesVector.at(i)->draw();
  }

  for(unsigned int i = 0; i < lifeVector.size();i++) {
      lifeVector.at(i)->draw();
  }

  for(unsigned int i = 0; i < coinVector.size();i++) {
      coinVector.at(i)->draw();
  }

  if ( collision ) {
    IoMod::getInstance().writeText("Oops: Collision", 500, 90);
  }
  viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::checkForCollisions() {
  auto it = spritesVector.begin();
  auto lit = lifeVector.end()-1;


  while ( it != spritesVector.end() ) {
    if ( strategiesVector[currentStrategy]->execute(*player, **it) ) {
      SmartSprite* doa = *it;
      player->detach(doa);
      delete doa;
      it = spritesVector.erase(it);
      if(lifeVector.size() > 0 && !player->getGodMode()) {
        LifeSprite* dob = *lit;
        delete dob;
        lit = lifeVector.erase(lit);
        lit--;
        if(lifeVector.size() == 0) {
          std::cout << "you are lost" << std::endl;
          isLost = true;
        }
      } 
      
    }
    
    else if(player->bulletCollided(*it)) 
    {

        SmartSprite* doa = *it;
        doa->explode();

    }
    
    else ++it;
  }

  auto cit = coinVector.begin();
  while ( cit != coinVector.end() ) { 
    if ( strategiesVector[currentStrategy]->execute(*player, **cit) ) {
      MultiSprite* dob = *cit;
      delete dob;
      cit = coinVector.erase(cit);
    } 
    else {
      ++cit;
    }
    if(coinVector.size() == 0) {
        std::cout << "you are win!" << std::endl;
        isLost = true;
        isWin = true;
    }
    
  }

  if(boss->bulletCollided(player) && !player->getGodMode()) {
      if(lifeVector.size() > 0) {
        LifeSprite* dob = *lit;
        delete dob;
        lit = lifeVector.erase(lit);
        lit--;
        if(lifeVector.size() == 0) {
          std::cout << "you are lost" << std::endl;
          isLost = true;
        }
      }     
  }

  if(player->bulletCollided(boss)) {
    boss->decreaseLife();

  }
}

void Engine::update(Uint32 ticks) {
  if(!isLost) {
    checkForCollisions();

    poolhud.update();
    player->update(ticks);
    boss->update(ticks);



    for ( SmartSprite* sprite : backgroundVector ) {
      sprite->update( ticks );
    }

    for ( SmartSprite* sprite : spritesVector ) {
      sprite->update( ticks );
    }

    int number = 0;
    for ( LifeSprite* life : lifeVector ) {
      life->update( ticks, player->getPosition(), number );
      number++;
    }

    for ( MultiSprite* coin : coinVector ) {
      coin->update( ticks );
    }    

    background.update();
    foreground.update();
    viewport.update(); // always update viewport last
  }

}

bool Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;
  SDLSound sound;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_M] ) {
          currentStrategy = (1 + currentStrategy) % strategiesVector.size();
        }
        if ( keystate[SDL_SCANCODE_SPACE] ) {
          if(!isLost) {
            sound[0];
            static_cast<Player*>(player)->shoot();
          }
        }       
        if ( keystate[SDL_SCANCODE_R] ) {
          if(isLost) {
            clock.unpause();
            return true;
          }
        }
        if (keystate[SDL_SCANCODE_G]) {
           static_cast<Player*>(player)->changeGodMode();
        }    
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
        if (keystate[SDL_SCANCODE_F1]) {
          if ( !clock.isPaused() ) {
            clock.pause();
            hud.showInstructions(renderer);        
          } else {
            clock.unpause();
          } 
        }

          if ( keystate[SDL_SCANCODE_0] ) {
            sound[0];
          }
          if ( keystate[SDL_SCANCODE_1] ) {
            sound[1];
          }
          if ( keystate[SDL_SCANCODE_2] ) {
            sound[2];
          }
          if ( keystate[SDL_SCANCODE_3] ) {
            sound[3];
          }
          if ( keystate[SDL_SCANCODE_4] ) {
            sound[4];
          }
          if ( keystate[SDL_SCANCODE_5] ) {
            sound[5];
          }
          if ( keystate[SDL_SCANCODE_6] ) {
            sound[6];
          }

      }
    }

    if(isLost) {
      if(!isWin) {
        endHud.showLostInstructions(renderer);
      } else {
        endHud.showVictoryInstructions(renderer);
      }
    }

    // In this section of the event loop we allow key bounce:
    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {     
      clock.incrFrame();
      if (keystate[SDL_SCANCODE_A]) {
        if(!isLost) {
          static_cast<Player*>(player)->left();
        }
      }
      if (keystate[SDL_SCANCODE_D]) {
        if(!isLost) {
          static_cast<Player*>(player)->right();
        }
      }
      if (keystate[SDL_SCANCODE_W]) {
        static_cast<Player*>(player)->up();
      }
      if (keystate[SDL_SCANCODE_S]) {
        static_cast<Player*>(player)->down();
      }

      static_cast<BossSprite*>(boss)->shoot();

      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
  return false;
}
