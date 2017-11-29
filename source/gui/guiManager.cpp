/*
 * guiManager.cpp
 *
 *  Created on: 13 sept. 2017
 *      Author: Bertrand
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


#ifdef __RASP__
#include <pthread.h>
#include <semaphore.h>


#include <linux/input.h>
#define EVENT_DEVICE    "/dev/input/event0"
#endif

#include "SDL2_gfxPrimitives.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "SDL2_framerate.h"

#include "guiBase.h"
#include <guiMapHex.h>
#include <guiCalibration.h>


#include "configuration.h"

Uint32 lastButton = 0;
guiBase *pActiveWnd;
std::list<guiBase*> lstWnd;

#ifdef __RASP__
static sem_t guiSemEvent;
void* guiMouseThread(void * p) {
	int x = 0, y = 0, t = 0;
	int rx = 0, ry = 0, rt = 0;
	int error_read = 0;
	struct input_event ev;
	int fd;
	char name[256] = "Unknown";
	bool update = false;

	/* Open Device */
	fd = open(EVENT_DEVICE, O_RDONLY);
	if (fd == -1) {
		fprintf(stderr, "%s is not a vaild device\n", EVENT_DEVICE);
		pthread_exit(NULL);
		return 0;
	}

	/* Print Device Name */
	ioctl(fd, EVIOCGNAME(sizeof(name)), name);
	printf("Reading from:\n");
	printf("device file = %s\n", EVENT_DEVICE);
	printf("device name = %s\n", name);

	for (;;) {
		const ssize_t ev_size = sizeof(struct input_event);
		ssize_t size;

		/* TODO: use select() */

		size = read(fd, &ev, ev_size);
		if (size < ev_size) {
			fprintf(stderr, "Error size when reading\n");
			close(fd);
			sleep(1);
			fd = open(EVENT_DEVICE, O_RDONLY);
			error_read++;
			if (error_read > 10)
			goto err;
		} else {
			error_read = 0;
			if (ev.type == EV_ABS && ev.code == ABS_X) {
				/* TODO: convert value to pixels */
				//printf("X = %d\n", ev.value);
				x = ev.value;
				//update=true;
			}
			if (ev.type == EV_ABS && ev.code == ABS_Y) {
				/* TODO: convert value to pixels */
				//printf("Y = %d\n", ev.value);
				y = ev.value;
				update = true;
			} else if (ev.type == EV_KEY && (ev.code == BTN_TOUCH)) {
				//printf("touch: %i\n", ev.value);
				t = ev.value;
				if (!t)
				update = true;
			} else {
				//printf("%x %x %x\n",ev.type, ev.code,ev.value);
			}
		}

		if (update) {
			update = false;
#ifdef SCREEN_7P
			rx=600-y; // swap on purpose !!!
			ry=x;// swap on purpose !!!
			rt=t;
#endif

#ifdef SCREEN_5P
			rx = 480.0f * (y - 250.0f) / 3600.0f; // swap on purpose !!!
			ry = 800 - (800.0f * (x - 154.0f) / 3800.0f);// swap on purpose !!!
#endif
			SDL_Event e;

			e.type = SDL_MOUSEMOTION;
			e.motion.x = rx;
			e.motion.y = ry;
			e.motion.state = t;

			int ii;
			sem_getvalue(&guiSemEvent,&ii);
			if(ii<100)
			{
				sem_post(&guiSemEvent);
				sem_post(&guiSemEvent);
				sem_post(&guiSemEvent);
			}

			SDL_PushEvent(&e);
			//printf("%3.3i %3.3i %1.1i\n",pMouse->x, pMouse->y,pMouse->t);

		}
	}
	printf("Endof mousse thread!\n");
	return 0;

	err: printf("Fuck! error in mousse thread!\n");
	close(fd);
	return 0;
}
#endif

int mousseMgt(guiBase* mainWin) {
	SDL_Event e;
	static int firstX, firstY;

	if (SDL_PollEvent(&e)) {



		if (e.type == SDL_QUIT)
			return -1;

		if ((e.type == SDL_MOUSEMOTION) || (e.type == SDL_MOUSEBUTTONDOWN) || (e.type == SDL_MOUSEBUTTONUP)) {

			if (e.motion.state > 0 && lastButton == 0) {
				// printf("event 1!\n");
				mainWin->event(e.motion.x, e.motion.y, 1);
				firstX = e.motion.x;
				firstY = e.motion.y;
			}

			if (e.motion.state > 0 && lastButton > 0) {
				// printf("event 2!\n");
				mainWin->event(e.motion.x, e.motion.y, 2);
			}

			if (e.motion.state == 0 && lastButton > 0) {
				// printf("event 3!\n");
				mainWin->event(e.motion.x, e.motion.y, 3);
				if ((abs(firstX - e.motion.x) < 100) && (abs(firstY - e.motion.y) < 100)) {
					// printf("event 4!\n");
					mainWin->event(e.motion.x, e.motion.y, 4);
				}
			}

			if (e.motion.state == 0 && lastButton == 0) {

				mainWin->event(e.motion.x, e.motion.y, 0);
			}

			lastButton = e.motion.state;
			if(e.type == SDL_MOUSEBUTTONUP)
				lastButton=0;


#ifdef __RASP__
			SDL_WarpMouseInWindow(NULL, e.motion.x, e.motion.y);
#endif

		}

	}
	return 0;
}

void setActiveWindows(guiBase* pWin) {
	pActiveWnd = pWin;
	lstWnd.push_front(pActiveWnd);
}

void popActiveWindows() {
	lstWnd.pop_front();
	pActiveWnd = lstWnd.front();

}

int rendertask() {

#ifdef __RASP__
	pthread_t my_mouseThread;
	SDL_ShowCursor(SDL_DISABLE);
	sem_init(&guiSemEvent,0,100);
#endif

	if (SDL_Init(SDL_INIT_VIDEO)) {
		printf("SDL_Init Error: %s", SDL_GetError());
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow("SDL2_gfx test", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
	//SDL_Window *window = SDL_CreateWindow("SDL2_gfx test", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN);

	if (window == NULL) {
		printf("SDL_CreateWindow Error: %s", SDL_GetError());
		SDL_Quit();
		return 2;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	//SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	if (renderer == NULL) {
		SDL_DestroyWindow(window);
		printf("SDL_CreateRenderer Error: %s", SDL_GetError());
		SDL_Quit();
		return 3;
	}

	printf("%s\n", SDL_GetCurrentVideoDriver());

	if (TTF_Init()) {
		fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
	}

	int flags = IMG_INIT_JPG | IMG_INIT_PNG;
	int initted = IMG_Init(flags);
	if (initted & flags != flags) {
		printf("could not init SDL_Image");
		printf("Reason: %s \n", IMG_GetError());
	}

#ifdef __RASP__
	int ret = pthread_create(&my_mouseThread, NULL, &guiMouseThread, NULL);
	if (ret != 0) {
		printf("Error: pthread_create() failed\n");
	}
#endif

	/*	guiHome::staticInit(renderer);
	 guiHome mainHome;
	 mainHome.setRect(0, 0, 600, 1024);
	 setActiveWindows(&mainHome);
	 */

	guiBase mainWnd;
	guiBase::staticInit(renderer);
	mainWnd.setRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	setActiveWindows(&mainWnd);

#ifdef EDITOR
	guiMapHex mainMap("toto",true);
#else
	guiMapHex mainMap("toto",false);
#endif

	guiCalibration wCalib;

	mainWnd.AddChild((guiBase*) &wCalib);

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	guiBase *localpActiveWnd = pActiveWnd;
	while (!mousseMgt(localpActiveWnd)) {
		localpActiveWnd = pActiveWnd;
		if (strlen(SDL_GetError()) > 3) {
			printf("Error catch in main loop: %s\n", SDL_GetError());
			SDL_ClearError();
		}
#ifdef __RASP__
		sem_wait(&guiSemEvent);
#endif
		SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0xFF, 0x00);
		SDL_RenderClear(renderer);

		localpActiveWnd->render();

		SDL_RenderPresent(renderer);
	}

	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	return 0;
}

int refresh(void) {
#ifdef __RASP__
	sem_post(&guiSemEvent);
#endif
}

