/*
 * guiBase.h
 *
 *  Created on: 5 sept. 2017
 *      Author: Bertrand
 */

#ifndef SRC_GUIBASE_H_
#define SRC_GUIBASE_H_

class guiBase;
class guiFont;

#include "SDL2_gfxPrimitives.h"
#include <SDL_image.h>
#include <list>
#include "guiManager.h"

#include <stdio.h>
#include <SDL_ttf.h>


class guiBase {
public:
	guiBase();
	guiBase(int x, int y, int w, int h);

	virtual ~guiBase();

	SDL_Rect _relWndRect;
	SDL_Rect _absWndRect;
	SDL_Rect _clpWndRect;
	std::list<guiBase*> _lstWnd;
	char* _sortName;

	void AddChild(guiBase * pWin);
	guiBase * GetFirstChild(std::list<guiBase*>::iterator* it);
	guiBase * GetNextChild(std::list<guiBase*>::iterator* it);
	void sort();
	void sortReverse();
	void computeClipping();
	virtual void render();
	virtual void event(int x, int y, int button);
	void setRect(int x, int y, int w, int h);
	static void staticInit(SDL_Renderer *renderer);


	static guiBase * _gblPlayer;
	static SDL_Texture * _textWallPaper;
	static SDL_Renderer *_renderer;

	guiBase * _pParent;



protected:
	/*
	static TTF_Font *_police1;
	static TTF_Font *_police2;
	static TTF_Font *_police3;
	*/

	static guiFont* _font1;
	static guiFont* _font2;


};

#endif /* SRC_GUIBASE_H_ */
