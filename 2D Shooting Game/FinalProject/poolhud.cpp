#include "poolhud.h"
#include "ioMod.h"
#include "renderContext.h"
#include <sstream>  

PoolHud::PoolHud(const Player& p) : 
    player(p),
    renderer(RenderContext::getInstance()->getRenderer()),
    hudFrame(
        {
            Gamedata::getInstance().getXmlInt("PoolHud/positionX"),
            Gamedata::getInstance().getXmlInt("PoolHud/positionY"),
            Gamedata::getInstance().getXmlInt("PoolHud/width"),
            Gamedata::getInstance().getXmlInt("PoolHud/height")
        }
    ),
    showPoolHud(true)
{
}


void PoolHud::update() {
}

void PoolHud::draw() const {
    if( !showPoolHud ) {
        return;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255/2);

    SDL_RenderFillRect( renderer, &hudFrame );

    std::stringstream strm;
    strm << Gamedata::getInstance().getXmlStr("PoolHud/bulletList") << player.bulletCount();
    IoMod::getInstance().writeText(strm.str(), hudFrame.x+20, 22);
    strm.clear();
    strm.str("");
    strm << Gamedata::getInstance().getXmlStr("PoolHud/freeList") << player.freeCount();
    IoMod::getInstance().writeText(strm.str(), hudFrame.x+20, 50);

    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255/2 );
    SDL_RenderDrawRect( renderer, &hudFrame );
}
