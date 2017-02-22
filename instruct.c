/*
 *	Instructions command for rogue clone. Alan Cox 1992
 */

#ifndef CURSES
#include <curses.h>
#endif CURSES
#include "rogue.h"
#include <string.h>
#include <stdio.h>

static char *instructions="/usr/games/rogue.instr";

void Instructions()
{
	char buffer[DROWS+1][DCOLS+1];
	char buf[256];
	FILE *f;
	short row;
	int i,j;
	f=fopen(instructions,"r");
	if(f==NULL)
	{
		message("Help file not on line.",0);
		return;
	}
	for (row = 0; row < DROWS; row++) {
		for (j = 0; j < DCOLS; j++) {
			buffer[row][j] = mvinch(row, j);
		}
		buffer[row][j] = 0;
		mvaddstr(row, 0, buffer[row]);
		clrtoeol();
	}
	move(0,0);
	for(i=0;i<DROWS;i++)
	{
		move(i,0);
		clrtoeol();
	}	
	refresh();
	for(i=0;i<DROWS;i++)
	{
		if(fgets(buf,250,f)==NULL)
			break;
		if(strchr(buf,'\n')!=NULL)
			*strchr(buf,'\n')=0;
		move(i,0);
		clrtoeol();
		mvaddstr(i,0,buf);
	}
	refresh();
	rgetchar();
	move(0,0);
	clrtoeol();
	for(i=0;i<DROWS;i++)
	{
		move(i,0);
		clrtoeol();
	}
	refresh();
	for(i=0;i<DROWS;i++)
	{
		mvaddstr(i,0,buffer[i]);
	}
	refresh();
}
