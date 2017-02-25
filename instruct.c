/*
 *	Instructions command for rogue clone. Alan Cox 1992
 */

#include "rogue.h"
#include "extern.h"

const int instructionnum = 23;
char *instructinos[instructionnum] = {"Movement                                                y  k  u",
"        With SHIFT or CTRL to move until you hit         \\ | /",
"        an obstacle                                     h -+- l",
"                                                         / | \\",
"        .        Rest                                   b  j  n",
"        m <direction>    Move onto something without taking it",
"        ,                Pick up item at your location.",
"",
"Command Keys",
"s          search                    i                inventory",
"f          fight                     F                fight (to the death)",
"e          Eat                       q                Quaff",
"r          Read a scroll             d                Drop",
"P          Put on ring               R                Remove a ring",
"CTRL-P     See previous message      >                Descend level",
"<          Ascend level              )                List weapons",
"]          List armour               =                List rings",
"^          Identify trap             I                Short inventory",
"T          Take off                  W                Wear armour",
"w          Wield weapon              c                Call something a name",
"z          Zap something             t<direction>     Throw something",
"v          Version                   Q                Quit",
"CTRL-A     Show average hit points   S                Save game"};

void Instructions() {
	char buffer[DROWS+1][DCOLS+1];
/*	char buf[256];
	FILE *f;*/
	int row;
	int i,j;
	for (row = 0; row < DROWS; row++) {
		for (j = 0; j < DCOLS; j++) {
			buffer[row][j] = (char)mvinch(row, j);
		}
		buffer[row][j] = 0;
		mvaddstr(row, 0, buffer[row]);
		clrtoeol();
	}
	move(0,0);
	for(i=0; i<DROWS; i++) {
		move(i,0);
		clrtoeol();
	}	
	refresh();
	for(i=0; i < instructionnum; i++) {
		move(i,0);
		clrtoeol();
		mvaddstr(i,0,instructinos[i]);
	}
	refresh();
	rgetchar();
	move(0,0);
	clrtoeol();
	for(i=0;i<DROWS;i++) {
		move(i,0);
		clrtoeol();
	}
	refresh();
	for(i=0;i<DROWS;i++) {
		mvaddstr(i,0,buffer[i]);
	}
	refresh();
}
