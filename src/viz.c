#include "msq.h"

#define COL(v)	\
	((v) >> (8*3)) & 0xff,	\
	((v) >> (8*2)) & 0xff,	\
	((v) >> (8*1)) & 0xff,	\
	((v) >> (8*0)) & 0xff

#define U(v)	(int)(((v+lg*wg/2) / (lg*wg)) * W)
#define UW	U(-lg*wg/2+wg)

#define COL_W	0xffffffff
#define COL_R	0xff0000ff
#define COL_G	0x00ff00ff
#define COL_B	0x00007fff
#define COL_Y	0x000000ff
#define	FNEQ	0.75
#define	BOUND	(0.2*lg*wg/2)

#define MAXCONT	8192
struct {
	int npt;
	int x[5], y[5];
} cont[MAXCONT];
int ncont;

#define NMBALL 10
struct {
	double ha, va;
	double cx, cy;
} mball[NMBALL];

void drawgrid(void)
{
	double i, j;

	SDL_SetRenderDrawColor(rnd, COL(COL_B));
	for(i = -lg*wg/2; i < lg*wg/2; i += wg) {
		SDL_RenderDrawLine(rnd, U(i), 0, U(i), H);
		SDL_RenderDrawLine(rnd, 0, U(i), W, U(i));
	}
	SDL_SetRenderDrawColor(rnd, COL(COL_W));
	SDL_RenderDrawLine(rnd, U(0), 0, U(0), H);
	SDL_RenderDrawLine(rnd, 0, U(0), W, U(0));
}

double mbfn(double x, double y, double cx, double cy)
{
	return 1.0 / sqrt((y-cy)*(y-cy) + (x-cx)*(x-cx));
}

double fn(double x, double y)
{
	int i;
	double r;

	r = 0;
	for(i = 0; i < NMBALL; i++)
		r += mbfn(x, y, mball[i].cx, mball[i].cy);
	return r;
}

double lerp(double p0, double p1, double v0, double v1)
{
	return p0 + (FNEQ-v0)/(v1-v0) * (p1-p0);
}

void contadd(double x, double y)
{
	cont[ncont].x[cont[ncont].npt] = x;
	cont[ncont].y[cont[ncont].npt] = y;
	cont[ncont].npt++;
}

void msqcell(double i, double j)
{
	int cx0, cx1, cy0, cy1, sum;
	double x0, x1, y0, y1;

	cx0 = ((x0 = fn(i, j)) >= FNEQ);
	cx1 = ((x1 = fn(i+wg, j)) >= FNEQ);
	cy0 = ((y0 = fn(i, j+wg)) >= FNEQ);
	cy1 = ((y1 = fn(i+wg, j+wg)) >= FNEQ);
	sum = cx0+cx1+cy0+cy1;
	if(sum == 4 || sum == 0)
		return;
	cont[ncont].npt = 0;
	if(cx0 != cx1)
		contadd(U(lerp(i, i+wg, x0, x1)), U(j));
	if(cy0 != cy1)
		contadd(U(lerp(i, i+wg, y0, y1)), U(j+wg));
	if(cy0 != cx0)
		contadd(U(i), U(lerp(j, j+wg, x0, y0)));
	if(cy1 != cx1)
		contadd(U(i+wg), U(lerp(j, j+wg, x1, y1)));
	if(drawfill) {
		/*if(!cx0)
			contadd();
		if(!cx1)
			contadd();
		if(!cy0)*/

	}
	ncont++;
}

void msq(void)
{
	int k;
	double i, j;

	for(j = -lg*wg/2; j < lg*wg/2; j += wg)
		for(i = -lg*wg/2; i < lg*wg/2; i += wg)
			msqcell(i, j);
}

void drawcont(void)
{
	int i;

	for(i = 0; i < ncont; i++)
		SDL_RenderDrawLine(rnd, cont[i].x[0], cont[i].y[0], cont[i].x[1], cont[i].y[1]);
}

int msqstep(void)
{
	static int first = 1;
	static double i, j;
	int cx0, cx1, cy0, cy1;
	SDL_Rect rect;

	if(first) {
		i = j = -lg*wg/2;
		first = 0;
	}
	input();
	if(pause || done)
		return quit;
	msqcell(i, j);
	if(drawtoggle || (i == lg*wg/2-wg && j == lg*wg/2-wg)) {
		SDL_SetRenderDrawColor(rnd, COL(0));
		SDL_RenderClear(rnd);
		if(gridtoggle)
			drawgrid();
		SDL_SetRenderDrawColor(rnd, COL(COL_R));
		drawcont();
		rect.x = U(i);
		rect.y = U(j);
		rect.w = rect.h = UW;
		cx0 = (fn(i, j) >= FNEQ);
		cx1 = (fn(i+wg, j) >= FNEQ);
		cy0 = (fn(i, j+wg) >= FNEQ);
		cy1 = (fn(i+wg, j+wg) >= FNEQ);
		SDL_SetRenderDrawColor(rnd, COL(COL_W));
		SDL_RenderDrawRect(rnd, &rect);
		filledCircleRGBA(rnd, U(i), U(j), UW/5, COL(cx0 ? COL_G : COL_R));
		filledCircleRGBA(rnd, U(i+wg), U(j), UW/5, COL(cx1 ? COL_G : COL_R));
		filledCircleRGBA(rnd, U(i), U(j+wg), UW/5, COL(cy0 ? COL_G : COL_R));
		filledCircleRGBA(rnd, U(i+wg), U(j+wg), UW/5, COL(cy1 ? COL_G : COL_R));
		SDL_RenderPresent(rnd);
	}
	if(i < lg*wg/2-wg)
		i += wg;
	else {
		i = -lg*wg/2;
		j += wg;
		if(j >= lg*wg/2)
			done = 1;
	}
	return quit;
}

int meta(void)
{
	int i;

	input();
	msq();
	if(drawtoggle) {
		SDL_SetRenderDrawColor(rnd, COL(0));
		SDL_RenderClear(rnd);
		if(gridtoggle)
			drawgrid();
		SDL_SetRenderDrawColor(rnd, COL(COL_R));
		drawcont();
		ncont = 0;
		SDL_RenderPresent(rnd);
	}
	for(i = 0; i < NMBALL; i++) {
		if(mball[i].cx+mball[i].ha >= (lg*wg/2-BOUND) || mball[i].cx+mball[i].ha <= (-lg*wg/2+BOUND))
			mball[i].ha *= -1.0;
		else
			mball[i].cx += mball[i].ha;

		if(mball[i].cy+mball[i].va >= (lg*wg/2-BOUND) || mball[i].cy+mball[i].va <= (-lg*wg/2+BOUND))
			mball[i].va *= -1.0;
		else
			mball[i].cy += mball[i].va;
	}
	return quit;
}

void setmball(void)
{
	int i;

	for(i = 0; i < NMBALL; i++) {
		mball[i].cx = (double)(rand()%(int)(lg*wg/2-BOUND)) * (rand()&1 ? -1 : 1);
		mball[i].cy = (double)(rand()%(int)(lg*wg/2-BOUND)) * (rand()&1 ? -1 : 1);
		mball[i].ha = (double)(rand()%10-5)/25.0;
		mball[i].va = (double)(rand()%10-5)/25.0;
	}
}

void start(void)
{
	drawtoggle = gridtoggle = drawfill = 1;
	pause = pauseafter = quit = done = 0;
	setmball();
	loop(10, &meta);
}
