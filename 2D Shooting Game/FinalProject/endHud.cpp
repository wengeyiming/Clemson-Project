#include "endHud.h"
#include "ioMod.h"

EndHud& EndHud::getInstance() {
	static EndHud instance;
	return instance;
}

EndHud::EndHud() : 
	red(Gamedata::getInstance().getXmlInt("EndHud/red")),
	green(Gamedata::getInstance().getXmlInt("EndHud/green")),
	blue(Gamedata::getInstance().getXmlInt("EndHud/blue")),
	alpha(Gamedata::getInstance().getXmlInt("EndHud/alpha")),
	positionX(Gamedata::getInstance().getXmlInt("EndHud/positionX")),
	positionY(Gamedata::getInstance().getXmlInt("EndHud/positionY")),
	width(Gamedata::getInstance().getXmlInt("EndHud/width")),
	height(Gamedata::getInstance().getXmlInt("EndHud/height")),
	rect({positionX,positionY,width,height})
{
	
}

void EndHud::closeInstructions() const {

}

void EndHud::showVictoryInstructions(SDL_Renderer* renderer) const {
	// First set the blend mode so that alpha blending will work;
    // the default blend mode is SDL_BLENDMODE_NONE!
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    // Now set the color for the hud:
    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255/2 );

    // Render rect
    SDL_RenderFillRect( renderer, &rect );

    // Now set the color for the outline of the hud:
    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255/2 );
    SDL_RenderDrawRect( renderer, &rect );

    int distance = Gamedata::getInstance().getXmlInt("EndHud/informationDistance");
    IoMod::getInstance().writeText(Gamedata::getInstance().getXmlStr("EndHud/informationWin"), positionX+width/3, (positionY+height)/2+1*distance);
    IoMod::getInstance().writeText(Gamedata::getInstance().getXmlStr("EndHud/informationR"), positionX+width/3, (positionY+height)/2+3*distance);
  
    // Render the rect to the screen
    SDL_RenderPresent(renderer);


}


void EndHud::showLostInstructions(SDL_Renderer* renderer) const {
    // First set the blend mode so that alpha blending will work;
    // the default blend mode is SDL_BLENDMODE_NONE!
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    // Now set the color for the hud:
    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255/2 );

    // Render rect
    SDL_RenderFillRect( renderer, &rect );

    // Now set the color for the outline of the hud:
    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255/2 );
    SDL_RenderDrawRect( renderer, &rect );

    int distance = Gamedata::getInstance().getXmlInt("EndHud/informationDistance");
    IoMod::getInstance().writeText(Gamedata::getInstance().getXmlStr("EndHud/informationLost"), positionX+width/3, (positionY+height)/2+1*distance);
    IoMod::getInstance().writeText(Gamedata::getInstance().getXmlStr("EndHud/informationR"), positionX+width/3, (positionY+height)/2+3*distance);
    
    // Render the rect to the screen
    SDL_RenderPresent(renderer);


}


