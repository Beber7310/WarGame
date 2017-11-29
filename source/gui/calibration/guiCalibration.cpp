/*
 * guiMap.cpp
 *
 *  Created on: 5 sept. 2017
 *      Author: Bertrand
 */

#include <guiCalibration.h>
#include "configuration.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

guiCalibration::guiCalibration() {
	_imgBlackdot = IMG_LoadTexture(_renderer, "res/blackdot.png");
	SDL_SetTextureBlendMode(_imgBlackdot, SDL_BLENDMODE_BLEND);
}

guiCalibration::~guiCalibration() {
	// TODO Auto-generated destructor stub
}

void guiCalibration::render() {
	guiBase * pTemp;
	SDL_Rect rect_src;
	SDL_Rect rect_dst;

	computeClipping();

	rect_dst.w = 32;
	rect_dst.h = 48;

	rect_src.w = 32;
	rect_src.h = 48;

	rect_dst.h = 32;
	rect_dst.w = 32;
	rect_dst.x = 100;
	rect_dst.y = 100;
	SDL_RenderCopy(_renderer, _imgBlackdot, NULL, &rect_dst);

	rect_dst.x = 700;
	rect_dst.y = 100;
	SDL_RenderCopy(_renderer, _imgBlackdot, NULL, &rect_dst);

	rect_dst.x = 700;
	rect_dst.y = 380;
	SDL_RenderCopy(_renderer, _imgBlackdot, NULL, &rect_dst);

	rect_dst.x = 100;
	rect_dst.y = 380;
	SDL_RenderCopy(_renderer, _imgBlackdot, NULL, &rect_dst);

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
void guiCalibration::event(int x, int y, int button) {

	guiBase * pTemp;
	int idx;
	std::list<guiBase*>::iterator it;

	pTemp = GetFirstChild(&it);
	while (pTemp) {
		if ((x > pTemp->_relWndRect.x) && (y > pTemp->_relWndRect.y) && (x < (pTemp->_relWndRect.x + pTemp->_relWndRect.w)) && (y < (pTemp->_relWndRect.y + pTemp->_relWndRect.h))) {
			pTemp->event(x - pTemp->_relWndRect.x, y - pTemp->_relWndRect.y, button);
		}
		pTemp = GetNextChild(&it);
	}
}
