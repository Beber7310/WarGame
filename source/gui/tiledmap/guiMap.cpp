/*
 * guiMap.cpp
 *
 *  Created on: 5 sept. 2017
 *      Author: Bertrand
 */

#include <guiMap.h>
#include "configuration.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

guiMap::guiMap() {
	_imgTileset = NULL;
}

guiMap::guiMap(char* mapfile, bool editormode) {
	sizeX = 33;
	sizeY = 16;

	//Hexagonal orientation, with 32 tile width, 28 height, and 16 side length. There's also an offset of 2 on the Y axis in the tileset's properties
	_imgTileset = IMG_LoadTexture(_renderer, "res/map/tileset.png");
	SDL_SetTextureBlendMode(_imgTileset, SDL_BLENDMODE_BLEND);
	SDL_QueryTexture(_imgTileset, NULL, NULL, &_RectHex.w, &_RectHex.h);
	_RectHex.x = 0;
	_RectHex.y = 0;

	mapCode = (tile*) malloc(sizeX * sizeY * sizeof(tile));

	for (int ii = 0; ii < sizeX * sizeY; ii++) {
		mapCode[ii].tile =0;//ii % 25;
		mapCode[ii].walkable = true; //ii % 25;
		mapCode[ii].hilight = 0;
	}

	_editorMode = editormode;
	_editorSelectedTile = 0;



	//SDL_QueryTexture(_imgTileset, NULL, NULL, &_RectHex.w, &_RectHex.h);
}

guiMap::~guiMap() {
	// TODO Auto-generated destructor stub
}

void guiMap::renderPalette() {
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

		SDL_RenderCopy(_renderer, _imgTileset, &rect_src, &rect_dst);
	}

}

void guiMap::render() {
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
			rect_src.y = 32 * (mapCode[ii].tile / 8);

			rect_dst.x = x * 32;
			rect_dst.y = y * 32;

			SDL_RenderCopy(_renderer, _imgTileset, &rect_src, &rect_dst);

			if (mapCode[ii].hilight > 0) {
				rect_src.x = 32 * (mapCode[ii].hilight % 8);
				rect_src.y = 32 * (mapCode[ii].hilight / 8);
				SDL_RenderCopy(_renderer, _imgTileset, &rect_src, &rect_dst);
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

			SDL_RenderCopy(_renderer, _imgTileset, &rect_src, &rect_dst);

			if (mapCode[ii].hilight > 0) {
				rect_src.x = 32 * (mapCode[ii].hilight % 8);
				rect_src.y = 48 * (mapCode[ii].hilight / 8);
				SDL_RenderCopy(_renderer, _imgTileset, &rect_src, &rect_dst);
			}
		}
	}

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

int guiMap::getTileByPos(int x, int y) {
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

int guiMap::getPaletteByPos(int x, int y) {
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

void guiMap::clearHightlight() {
	for (int ii = 0; ii < sizeX * sizeY; ii++) {

		mapCode[ii].hilight = 0;
	}
}

void guiMap::event(int x, int y, int button) {

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
