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

#define MAXMBALL 20

#ifndef EXTERN
#define EXTERN extern
#endif

typedef	uint32_t uint32;

/*
 *	util.c
 */
void	errorf(char *, ...);
void	input(void);
void	loop(int (*)(void));
void	init(void);

/*
 *	viz.c
 */
void	drawgrid(void);
double	mbfn(double, double, double, double);
double	fn(double, double);
double	lerp(double, double, double, double);
void	msqcell(double, double);
void	msq(void);
void	drawcont(void);
int	msqstep(void);
int	meta(void);
void	setmball(void);
void	start(void);

EXTERN	SDL_Window	*scr;
EXTERN	SDL_Renderer	*rnd;
EXTERN	uint32		msdelay;
EXTERN	int		quit;
EXTERN	int		drawtoggle;
EXTERN	int		gridtoggle;
EXTERN	int		pause;
EXTERN	int		done;
EXTERN	int		drawfill;
EXTERN	double		lg;
EXTERN	double		wg;
EXTERN	double		nmball;
EXTERN	double		fneq;
