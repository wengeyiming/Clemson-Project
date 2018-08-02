#include "hud.h"
#include "ioMod.h"

Hud& Hud::getInstance() {
	static Hud instance;
	return instance;
}

Hud::Hud() : 
	red(Gamedata::getInstance().getXmlInt("Hud/red")),
	green(Gamedata::getInstance().getXmlInt("Hud/green")),
	blue(Gamedata::getInstance().getXmlInt("Hud/blue")),
	alpha(Gamedata::getInstance().getXmlInt("Hud/alpha")),
	positionX(Gamedata::getInstance().getXmlInt("Hud/positionX")),
	positionY(Gamedata::getInstance().getXmlInt("Hud/positionY")),
	width(Gamedata::getInstance().getXmlInt("Hud/width")),
	height(Gamedata::getInstance().getXmlInt("Hud/height")),
	rect({positionX,positionY,width,height})
{
	
}

void Hud::closeInstructions() const {

}

void Hud::showInstructions(SDL_Renderer* renderer) const {
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

    int distance = Gamedata::getInstance().getXmlInt("Hud/informationDistance");
    IoMod::getInstance().writeText(Gamedata::getInstance().getXmlStr("Hud/informationW"), positionX+width/3, (positionY+height)/3);
    IoMod::getInstance().writeText(Gamedata::getInstance().getXmlStr("Hud/informationS"), positionX+width/3, (positionY+height)/3+1*distance);
    IoMod::getInstance().writeText(Gamedata::getInstance().getXmlStr("Hud/informationA"), positionX+width/3, (positionY+height)/3+2*distance);
    IoMod::getInstance().writeText(Gamedata::getInstance().getXmlStr("Hud/informationD"), positionX+width/3, (positionY+height)/3+3*distance); 
    IoMod::getInstance().writeText(Gamedata::getInstance().getXmlStr("Hud/informationSpace"), positionX+width/3, (positionY+height)/3+4*distance);    
    IoMod::getInstance().writeText(Gamedata::getInstance().getXmlStr("Hud/informationG"), positionX+width/3, (positionY+height)/3+5*distance);
    IoMod::getInstance().writeText(Gamedata::getInstance().getXmlStr("Hud/informationR"), positionX+width/3, (positionY+height)/3+6*distance); 
    IoMod::getInstance().writeText(Gamedata::getInstance().getXmlStr("Hud/informationWin"), positionX+width/5, (positionY+height)/3+8*distance); 
  
    // Render the rect to the screen
    SDL_RenderPresent(renderer);


}





