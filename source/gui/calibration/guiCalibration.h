/*
 * guiMap.h
 *
 *  Created on: 5 sept. 2017
 *      Author: Bertrand
 */

#ifndef SRC_guiCalib_H_
#define SRC_guiCalib_H_

class guiCalibration;
class guiFont;

#include "SDL2_gfxPrimitives.h"
#include <SDL_image.h>
#include <list>
#include "guiManager.h"

#include <stdio.h>
#include <SDL_ttf.h>

class guiCalibration :guiBase {
public:
	guiCalibration();

	virtual ~guiCalibration();

	virtual void render();
	void event(int x, int y, int button);

	SDL_Texture * 	_imgBlackdot;

};

#endif /* SRC_guiCalib_H_ */
