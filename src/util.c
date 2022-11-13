#include "msq.h"

void errorf(char *fmt, ...)
{
	va_list ap;

	fprintf(stderr, "error: ");
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	fprintf(stderr, "\n");
	SDL_Quit();
	exit(1);
}

void input(void)
{
	SDL_Event e;

	while(SDL_PollEvent(&e))
		switch(e.type) {
		case SDL_QUIT:
			quit = 1;
			break;
		case SDL_KEYDOWN:
			switch(e.key.keysym.sym) {
			case SDLK_v:
				drawtoggle = !drawtoggle;
				break;
			case SDLK_g:
				gridtoggle = !gridtoggle;
				break;
			case SDLK_SPACE:
				pause = !pause;
				break;
			case SDLK_q:
				quit = 1;
				break;
			case SDLK_r:
				setmball();
				break;
			case SDLK_f:
				drawfill = !drawfill;
				break;
			case SDLK_b:
				if(nmball < MAXMBALL-1)
					nmball++;
				setmball();
				break;
			case SDLK_n:
				if(nmball > 0)
					nmball--;
				setmball();
				break;
			case SDLK_j:
				if(lg > 10)
					lg -= 10;
				break;
			case SDLK_k:
				if(lg < 200)
					lg += 10;
				break;
			case SDLK_w:
				if(msdelay >= 10)
					msdelay -= 10;
				break;
			case SDLK_s:
				if(msdelay < 1000)
					msdelay += 10;
				break;
			case SDLK_o:
				fneq += 0.25;
				break;
			case SDLK_p:
				fneq -= 0.25;
				break;
			}
			break;
		}
}

void loop(int (*run)(void))
{
	int t, dt, err;

	err = 0;
	for(;;) {
		t = SDL_GetTicks();
		if((*run)())
			break;

		dt = SDL_GetTicks()-t;
		if(dt < msdelay) {
			err -= msdelay-dt;
			if(err < 0) {
				SDL_Delay(-err);
				err = 0;
			}
		} else if(dt > msdelay)
			err += dt-msdelay;
	}
}

void init(void)
{
	SDL_Init(SDL_INIT_VIDEO);
	scr = SDL_CreateWindow("msq", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, W, H, SDL_WINDOW_OPENGL);
	rnd = SDL_CreateRenderer(scr, -1, 0);
}
