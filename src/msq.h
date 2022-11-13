#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2_gfxPrimitives.h>

#define H	500
#define W	500

#ifndef EXTERN
#define EXTERN extern
#endif

typedef	uint32_t uint32;

/*
 *	lib.c
 */
void	errorf(char *, ...);
void	input(void);
void	loop(uint32, int (*)(void));
void	init(void);

/*
 *	viz.c
 */
double	fn(double, double);
void	drawgrid(void);
void	msq(void);
int	msqstep(void);
void	msqcell(double, double);
void	setmball(void);
void	start(void);

EXTERN	SDL_Window	*scr;
EXTERN	SDL_Renderer	*rnd;
EXTERN	int		quit;
EXTERN	int		drawtoggle;
EXTERN	int		gridtoggle;
EXTERN	int		pause;
EXTERN	int		pauseafter;
EXTERN	int		done;
EXTERN	int		msdelay;
EXTERN	double		lg;
EXTERN	double		wg;
