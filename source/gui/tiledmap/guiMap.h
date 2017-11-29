/*
 * guiMap.h
 *
 *  Created on: 5 sept. 2017
 *      Author: Bertrand
 */

#ifndef SRC_guiMap_H_
#define SRC_guiMap_H_

class guiMap;
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

}tile;

class guiMap :guiBase {
public:
	guiMap();
	guiMap(char* mapfile,bool editormode);
	virtual ~guiMap();
	tile* mapCode;

	void clearHightlight();
	virtual void render();
	void renderPalette();

	int getTileByPos(int x,int y);
	int getPaletteByPos(int x, int y) ;

	void event(int x, int y, int button);
	bool _editorMode;
	int _editorSelectedTile;

	int sizeX,sizeY;
	SDL_Texture * 	_imgTileset;


	SDL_Rect 		_RectHex;

};

#endif /* SRC_guiMap_H_ */
