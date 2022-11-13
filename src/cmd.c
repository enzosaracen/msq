#define EXTERN
#include "msq.h"

int main(int argc, char **argv)
{
	if(argc != 4) {
		lg = 100;
		wg = 0.5;
		msdelay = 50;
	} else {
		lg = atoi(argv[1]);
		wg = atof(argv[2]);
		msdelay = atoi(argv[3]);
	}
	srand(time(NULL));
	init();
	start();
	SDL_Quit();
}
