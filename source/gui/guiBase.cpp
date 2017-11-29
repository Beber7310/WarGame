/*
 * guiBase.cpp
 *
 *  Created on: 5 sept. 2017
 *      Author: Bertrand
 */

#include "configuration.h"
#include <stdio.h>
#include "string.h"
#include "guiBase.h"

SDL_Renderer *guiBase::_renderer;


guiBase *guiBase::_gblPlayer;


guiBase::guiBase() {
	_pParent = NULL;

	_relWndRect.x = 0;
	_relWndRect.y = 0;
	_relWndRect.w = SCREEN_WIDTH;
	_relWndRect.h = SCREEN_HEIGHT;

	_absWndRect.x = 0;
	_absWndRect.y = 0;
	_absWndRect.w = SCREEN_WIDTH;
	_absWndRect.h = SCREEN_HEIGHT;

	_clpWndRect.x = 0;
	_clpWndRect.y = 0;
	_clpWndRect.w = SCREEN_WIDTH;
	_clpWndRect.h = SCREEN_HEIGHT;

	_sortName = NULL;
}

guiBase::guiBase(int x, int y, int w, int h) {
	_pParent = NULL;

	_relWndRect.x = x;
	_relWndRect.y = y;
	_relWndRect.w = w;
	_relWndRect.h = h;

	_absWndRect.x = 0;
	_absWndRect.y = 0;
	_absWndRect.w = 200;
	_absWndRect.h = 200;
	_sortName = NULL;
}

void guiBase::staticInit(SDL_Renderer *renderer) {
	/*
	 _police1 = TTF_OpenFont("res/font1.otf", 65);
	 if (!_police1) {
	 printf("TTF_OpenFont Error: %s\n", SDL_GetError());

	 }

	 _police2 = TTF_OpenFont("res/font2.otf", 30);
	 if (!_police2) {
	 printf("TTF_OpenFont Error: %s\n", SDL_GetError());

	 }

	 _police3 = TTF_OpenFont("res/font1.otf", 65);
	 if (!_police3) {
	 printf("TTF_OpenFont Error: %s\n", SDL_GetError());

	 }
	 */
/*
	_textWallPaper = IMG_LoadTexture(renderer, WALLPAPER);
	if (!_textWallPaper) {
		printf("Error catch in staticInit: %s\n", SDL_GetError());
		SDL_ClearError();
	}
	_renderer = renderer;

	//_font1=new guiFont(_renderer,"res/font2.otf");
	_font2 = new guiFont(_renderer, "res/Mermaid1001.ttf");
//	_font3=new guiFont(_renderer,"res/font3.otf");
*/
	_renderer = renderer;
}

void guiBase::setRect(int x, int y, int w, int h) {
	_relWndRect.x = x;
	_relWndRect.y = y;
	_relWndRect.w = w;
	_relWndRect.h = h;
}

guiBase::~guiBase() {
	// TODO Auto-generated destructor stub
}

void guiBase::sort() {
	// TODO Auto-generated destructor stub
	_lstWnd.sort([](const guiBase* a, const guiBase* b) {return (strcasecmp( a->_sortName, b->_sortName)<0);});

}
void guiBase::sortReverse() {
	// TODO Auto-generated destructor stub
	_lstWnd.sort([](const guiBase* a, const guiBase* b) {return (strcasecmp( a->_sortName, b->_sortName)>0);});

}

void guiBase::AddChild(guiBase * pWin) {
	pWin->_pParent = this;
	_lstWnd.push_back(pWin);
}

guiBase *
guiBase::GetFirstChild(std::list<guiBase*>::iterator* it) {
	*it = _lstWnd.begin();
	return _lstWnd.front();
}
guiBase *
guiBase::GetNextChild(std::list<guiBase*>::iterator* it) {
	(*it)++;
	if ((*it) != _lstWnd.end())
		return (guiBase*) ((**it));
	else
		return NULL;
}

void guiBase::render() {
	guiBase * pTemp;

	computeClipping();

	//boxRGBA(_renderer, _absWndRect.x, _absWndRect.y, _absWndRect.x + _absWndRect.w, _absWndRect.y + _absWndRect.h, 0x0, 0x0, 0x00, 0xFF);
	//SDL_RenderCopy(_renderer, _textWallPaper, NULL, NULL);

	std::list<guiBase*>::iterator it;
	pTemp = GetFirstChild(&it);
	while (pTemp) {
		pTemp->_absWndRect.x = _absWndRect.x + pTemp->_relWndRect.x;
		pTemp->_absWndRect.y = _absWndRect.y + pTemp->_relWndRect.y;
		pTemp->_absWndRect.w = pTemp->_relWndRect.w;
		pTemp->_absWndRect.h = pTemp->_relWndRect.h;

		pTemp->render();
		pTemp = GetNextChild(&it);
	}

	SDL_RenderSetClipRect(_renderer, NULL);
}

void guiBase::event(int x, int y, int button) {

	guiBase * pTemp;
	std::list<guiBase*>::iterator it;
	pTemp = GetFirstChild(&it);
	while (pTemp) {
		if ((x > pTemp->_relWndRect.x) && (y > pTemp->_relWndRect.y) && (x < (pTemp->_relWndRect.x + pTemp->_relWndRect.w)) && (y < (pTemp->_relWndRect.y + pTemp->_relWndRect.h))) {
			pTemp->event(x - pTemp->_relWndRect.x, y - pTemp->_relWndRect.y, button);
		}
		pTemp = GetNextChild(&it);
	}
}

void guiBase::computeClipping() {
	_clpWndRect.x = _absWndRect.x;
	_clpWndRect.y = _absWndRect.y;
	_clpWndRect.w = _absWndRect.w;
	_clpWndRect.h = _absWndRect.h;

	if (_pParent) {
		_clpWndRect.x = std::max(_absWndRect.x, _pParent->_clpWndRect.x);
		_clpWndRect.y = std::max(_absWndRect.y, _pParent->_clpWndRect.y);
	}

	int res = SDL_RenderSetClipRect(_renderer, &_clpWndRect);
	if (res)
		printf("%s", SDL_GetError());
}

