#ifndef renderings_included
#define renderings_included

void drawTriangle(u_char rowOff, u_char colOff, u_char width, u_char height,
	       u_char rowJump, u_char colJump, u_char opperation, u_int colorBGR);

void drawBox(u_char rowOff, u_char colOff, u_char width, u_char height,
	     u_char rowJump, u_char colJump, u_int colorBGR);

void drawEatenDonut(u_char rowOff, u_char colOff, u_char width, u_char height,
		u_char stepSize, u_int colorBGR);

void drawHourGlass();

void drawDonut();

//void drawEatenDonut();

void drawJurrasicPark();
#endif
