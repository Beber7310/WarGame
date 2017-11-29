/*
 * guiMap.cpp
 *
 *  Created on: 5 sept. 2017
 *      Author: Bertrand
 */

#include <guiMapHex.h>
#include "configuration.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

guiMapHex::guiMapHex() {
	_imgHex = NULL;
}

guiMapHex::guiMapHex(char* mapfile, bool editormode) {
	sizeX = 33;
	sizeY = 16;

	//Hexagonal orientation, with 32 tile width, 28 height, and 16 side length. There's also an offset of 2 on the Y axis in the tileset's properties
	_imgHex = IMG_LoadTexture(_renderer, "res/map/hextiles.png");
	SDL_SetTextureBlendMode(_imgHex, SDL_BLENDMODE_BLEND);
	SDL_QueryTexture(_imgHex, NULL, NULL, &_RectHex.w, &_RectHex.h);
	_RectHex.x = 0;
	_RectHex.y = 0;

	mapCode = (tileHex*) malloc(sizeX * sizeY * sizeof(tileHex));

	for (int ii = 0; ii < sizeX * sizeY; ii++) {
		mapCode[ii].tile = 0; //ii % 25;
		mapCode[ii].walkable = true; //ii % 25;
		mapCode[ii].hilight = 0;
	}

	_editorMode = editormode;
	_editorSelectedTile = 0;

	Neighbour[0] = -1;
	Neighbour[1] = -1;
	Neighbour[2] = -1;
	Neighbour[3] = -1;
	Neighbour[4] = -1;
	Neighbour[5] = -1;

	_imgBlackdot = IMG_LoadTexture(_renderer, "res/blackdot.png");
	SDL_SetTextureBlendMode(_imgBlackdot, SDL_BLENDMODE_BLEND);
}

guiMapHex::~guiMapHex() {
	// TODO Auto-generated destructor stub
}

void guiMapHex::renderPalette() {
	SDL_Rect rect_src;
	SDL_Rect rect_dst;

	rect_dst.w = 32;
	rect_dst.h = 48;

	rect_src.w = 32;
	rect_src.h = 48;

	for (int ii = 0; ii < 33; ii++) {

		rect_src.x = 32 * (ii % 8);
		rect_src.y = 48 * (ii / 8);

		rect_dst.x = 900 + (ii % 8) * 32;
		rect_dst.y = 100 + (ii / 8) * 48;

		SDL_RenderCopy(_renderer, _imgHex, &rect_src, &rect_dst);
	}

}

void guiMapHex::render() {
	guiBase * pTemp;
	SDL_Rect rect_src;
	SDL_Rect rect_dst;

	computeClipping();

	rect_dst.w = 32;
	rect_dst.h = 48;

	rect_src.w = 32;
	rect_src.h = 48;

	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x += 2) {

			int ii = x + y * sizeX;

			rect_src.x = 32 * (mapCode[ii].tile % 8);
			rect_src.y = 48 * (mapCode[ii].tile / 8);

			rect_dst.x = x * 24;
			rect_dst.y = y * 28 - 17;
			if ((x & 0x01) == 0x01)
				rect_dst.y += 14;

			SDL_RenderCopy(_renderer, _imgHex, &rect_src, &rect_dst);

			if (mapCode[ii].hilight > 0) {
				rect_src.x = 32 * (mapCode[ii].hilight % 8);
				rect_src.y = 48 * (mapCode[ii].hilight / 8);
				SDL_RenderCopy(_renderer, _imgHex, &rect_src, &rect_dst);
			}

		}

		for (int x = 1; x < sizeX; x += 2) {

			int ii = x + y * sizeX;
			rect_src.x = 32 * (mapCode[ii].tile % 8);
			rect_src.y = 48 * (mapCode[ii].tile / 8);

			rect_dst.x = x * 24;
			rect_dst.y = y * 28 - 17;
			if ((x & 0x01) == 0x01)
				rect_dst.y += 14;

			SDL_RenderCopy(_renderer, _imgHex, &rect_src, &rect_dst);

			if (mapCode[ii].hilight > 0) {
				rect_src.x = 32 * (mapCode[ii].hilight % 8);
				rect_src.y = 48 * (mapCode[ii].hilight / 8);
				SDL_RenderCopy(_renderer, _imgHex, &rect_src, &rect_dst);
			}
		}
	}

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

	if (_editorMode)
		renderPalette();

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

int guiMapHex::getTileByPos(int x, int y) {
	int xtile;
	int ytile;

	xtile = x / 24;

	if ((xtile & 0x01) == 1) {
		ytile = (y - 14) / 28;
	} else {
		ytile = y / 28;
	}

	if (xtile >= sizeX)
		return -1;
	if (ytile >= sizeY)
		return -1;

	return xtile + sizeX * ytile;
}

int guiMapHex::getPaletteByPos(int x, int y) {
	int xtile;
	int ytile;

	xtile = (x - 900) / 32;
	ytile = (y - 100) / 48;

	printf("%i %i\n", xtile, ytile);

	if (xtile > 7)
		return -1;
	if (ytile > 5)
		return -1;
	if (xtile < 0)
		return -1;
	if (ytile < 0)
		return -1;

	printf("%i %i %i\n", xtile, ytile, xtile + 8 * ytile);
	return xtile + 8 * ytile;
}

void guiMapHex::clearHightlight() {
	for (int ii = 0; ii < sizeX * sizeY; ii++) {

		mapCode[ii].hilight = 0;
	}
}

int guiMapHex::findNeighbour(int add) {

	findNeighbour2(add, 3);

}

int guiMapHex::findNeighbour2(int add, int dist) {
	int x, y;

	if (dist <= 0)
		return 0;

	x = add % sizeX;
	y = add / sizeX;

	if (x > 0) {
		int l = x - 1 + (y * sizeX);
		//if (mapCode[l].hilight == 0)
		{
			mapCode[l].hilight = 36;
			findNeighbour2(l, dist - 1);
		}
	}

	if (x < (sizeX - 1)) {
		int l = x + 1 + (y * sizeX);
		//if (mapCode[l].hilight == 0)
		{
			mapCode[l].hilight = 36;
			findNeighbour2(l, dist - 1);
		}
	}

	if (y > 0) {
		int l = x + ((y - 1) * sizeX);
		//if (mapCode[l].hilight == 0)
		{
			mapCode[l].hilight = 36;
			findNeighbour2(l, dist - 1);
		}
	}

	if (y < (sizeY - 1)) {
		int l = x + ((y + 1) * sizeX);
		//if (mapCode[l].hilight == 0)
		{
			mapCode[l].hilight = 36;
			findNeighbour2(l, dist - 1);
		}
	}

	if ((x & 0x01) == 0) {
		if (y > 0) {
			if (x < sizeX - 1) {
				int l = x + 1 + ((y - 1) * sizeX);
				//if (mapCode[l].hilight == 0)
				{
					mapCode[l].hilight = 36;
					findNeighbour2(l, dist - 1);
				}
			}
			if (x > 0) {
				int l = x - 1 + ((y - 1) * sizeX);
				//if (mapCode[l].hilight == 0)
				{
					mapCode[l].hilight = 36;
					findNeighbour2(l, dist - 1);
				}
			}
		}
	} else {
		if (y < sizeY - 1) {
			if (x < sizeX - 1) {
				int l = x + 1 + ((y + 1) * sizeX);
				//if (mapCode[l].hilight == 0)
				{
					mapCode[l].hilight = 36;
					findNeighbour2(l, dist - 1);
				}
			}
			if (x > 0) {
				int l = x - 1 + ((y + 1) * sizeX);
				//if (mapCode[l].hilight == 0)
				{
					mapCode[l].hilight = 36;
					findNeighbour2(l, dist - 1);
				}
			}
		}
	}
	return 0;
}

void guiMapHex::event(int x, int y, int button) {

	guiBase * pTemp;
	int idx;
	std::list<guiBase*>::iterator it;

	if (_editorMode) {
		if ((button == 1) || (button == 2)) {

			idx = getTileByPos(x, y);
			if (idx >= 0)
				mapCode[idx].tile = _editorSelectedTile;

			idx = getPaletteByPos(x, y);
			if (idx >= 0)
				_editorSelectedTile = idx;

		}
	} else {

		idx = getTileByPos(x, y);
		if (idx >= 0) {
			clearHightlight();
			findNeighbour(idx);
		}
	}

	pTemp = GetFirstChild(&it);
	while (pTemp) {
		if ((x > pTemp->_relWndRect.x) && (y > pTemp->_relWndRect.y) && (x < (pTemp->_relWndRect.x + pTemp->_relWndRect.w)) && (y < (pTemp->_relWndRect.y + pTemp->_relWndRect.h))) {
			pTemp->event(x - pTemp->_relWndRect.x, y - pTemp->_relWndRect.y, button);
		}
		pTemp = GetNextChild(&it);
	}
}
