#define EXTERN
#include "msq.h"

int main(void)
{
	srand(time(NULL));
	init();
	start();
	SDL_Quit();
}
