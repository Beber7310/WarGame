/*
 * guiMap.h
 *
 *  Created on: 5 sept. 2017
 *      Author: Bertrand
 */

#ifndef SRC_guiMap_H_
#define SRC_guiMap_H_

class guiMapHex;
class guiFont;

#include "SDL2_gfxPrimitives.h"
#include <SDL_image.h>
#include <list>
#include "guiManager.h"

#include <stdio.h>
#include <SDL_ttf.h>

typedef struct
{
	int tile;
	bool walkable;
	int hilight;

}tileHex;

class guiMapHex :guiBase {
public:
	guiMapHex();
	guiMapHex(char* mapfile,bool editormode);
	virtual ~guiMapHex();
	tileHex* mapCode;

	void clearHightlight();
	virtual void render();
	void renderPalette();

	int getTileByPos(int x,int y);
	int getPaletteByPos(int x, int y) ;

	int findNeighbour(int idx);
	int findNeighbour2(int add,int  dist);

	void event(int x, int y, int button);
	bool _editorMode;
	int _editorSelectedTile;

	int sizeX,sizeY;
	SDL_Texture * 	_imgHex;
	SDL_Texture * 	_imgBlackdot;
	SDL_Rect 		_RectHex;


	int Neighbour[6];
};

#endif /* SRC_guiMap_H_ */
