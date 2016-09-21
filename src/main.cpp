#include "ui/CursesWindow.h"

WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);

int windowFrameworkTest();
int window_demo();
int panel_basic_demo();

int main(int argc, char *argv[])
{
	return windowFrameworkTest();
	//return window_demo();
//	return panel_basic_demo();
}

int windowFrameworkTest() {
	initscr();			/* Start curses mode 		*/
	cbreak();			/* Line buffering disabled, Pass on
					 * everty thing to me 		*/
	keypad(stdscr, TRUE);		/* I need that nifty F1 */
	noecho();

	addstr("Press any key to start.");
	getch();

	//Create and show encapsulated curses window
	Window* myWin = new CursesWindow(30, 30, -10, -10);
	myWin->setTitle("Winning");
	doupdate();

	getch();

	//Test that that filling with some text lays out properly within horizontal bounds, with margins
	myWin->setPadding(4, 4, 4, 4);
	myWin->fillWithText("01234 6789 123 567 9 123 5 789. 2345 789012345 789012345678901234567890_2345678901234567 90\n\n0123 56\t89 1234\t6789");
	doupdate();

	getch(); //wait for keypress to resize

	//Test resize, no move
	myWin->resize(20,20);
	doupdate();

	getch(); //wait for keypress to resize and move

	//Test resize, move
	myWin->resize(35, 10, 2, 2);
	doupdate();

	getch(); //finish after another keypress

	//Test resizing to size smaller than title
	myWin->resize(5,5);
	doupdate();

	getch();

	//test that text lays out properly w/i vertical bounds and stops when it can't fit in window any longer
	myWin->setPadding(0, 0, 0, 0);
	myWin->fillWithText("01234 6 8 0 2 4 6");
	doupdate();

	getch();

	//test that changing padding to be too large for any text clears text
	myWin->setPadding(50, 50, 50, 50);
	myWin->fillWithText("01234 6 8 0 2 4 6");
	doupdate();

	getch();

	//Test clearing title
	myWin->setTitle("");
	doupdate();

	getch();

	//Test just move
	myWin->move(2, 10);
	myWin->setTitle("Move");
	doupdate();

	getch();

	//Test move out of bounds
	myWin->move(-2, 10);
	myWin->setTitle("OOB");
	doupdate();

	getch();

	//Test child in top-left corner
	myWin->resize(20,20, 5, 5);
	myWin->setTitle("Parent");
	Window* child1 = myWin->makeChildCentered(12, 12);
	child1->setTitle("Child1");
	Window* child2 = myWin->makeChildCentered(6, 100);
	child2->setTitle("Child2");
	Window* child3 = myWin->makeChildCentered(100, 6);
	child3->setTitle("Child3");
	Window* child4 = myWin->makeChild(11, 12, 19, 19);
	child4->setTitle("Child4");
	doupdate();

	getch();

	delete myWin;

	endwin();

	return 0;
}

int window_demo() {
	WINDOW *my_win;
	int startx, starty, width, height;
	int ch;

	initscr();			/* Start curses mode 		*/
	cbreak();			/* Line buffering disabled, Pass on
					 * everty thing to me 		*/
	keypad(stdscr, TRUE);		/* I need that nifty F1 	*/

	height = 3;
	width = 10;
	starty = (LINES - height) / 2;	/* Calculating for a center placement */
	startx = (COLS - width) / 2;	/* of the window		*/
	printw("Press F1 to exit");
	refresh();
	my_win = create_newwin(height, width, starty, startx);

	while((ch = getch()) != KEY_F(1))
	{	switch(ch)
		{	case KEY_LEFT:
				destroy_win(my_win);
				my_win = create_newwin(height, width, starty,--startx);
				break;
			case KEY_RIGHT:
				destroy_win(my_win);
				my_win = create_newwin(height, width, starty,++startx);
				break;
			case KEY_UP:
				destroy_win(my_win);
				my_win = create_newwin(height, width, --starty,startx);
				break;
			case KEY_DOWN:
				destroy_win(my_win);
				my_win = create_newwin(height, width, ++starty,startx);
				break;
		}
	}

	endwin();			/* End curses mode		  */
	return 0;
}

int panel_basic_demo() {
	WINDOW *my_wins[3];
	PANEL  *my_panels[3];
	int lines = 10, cols = 40, y = 2, x = 4, i;

	initscr();
	cbreak();
	noecho();

	/* Create windows for the panels */
	my_wins[0] = newwin(lines, cols, y, x);
	my_wins[1] = newwin(lines, cols, y + 1, x + 5);
	my_wins[2] = newwin(lines, cols, y + 2, x + 10);

	/*
	 * Create borders around the windows so that you can see the effect
	 * of panels
	 */
	for(i = 0; i < 3; ++i)
		box(my_wins[i], 0, 0);

	/* Attach a panel to each window */ 	/* Order is bottom up */
	my_panels[0] = new_panel(my_wins[0]); 	/* Push 0, order: stdscr-0 */
	my_panels[1] = new_panel(my_wins[1]); 	/* Push 1, order: stdscr-0-1 */
	my_panels[2] = new_panel(my_wins[2]); 	/* Push 2, order: stdscr-0-1-2 */

	/* Update the stacking order. 2nd panel will be on top */
	update_panels();

	/* Show it on the screen */
	doupdate();

	getch();
	endwin();

	return 0;
}

WINDOW *create_newwin(int height, int width, int starty, int startx)
{	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);		/* 0, 0 gives default characters
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}

void destroy_win(WINDOW *local_win)
{
	/* box(local_win, ' ', ' '); : This won't produce the desired
	 * result of erasing the window. It will leave it's four corners
	 * and so an ugly remnant of window.
	 */
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	/* The parameters taken are
	 * 1. win: the window on which to operate
	 * 2. ls: character to be used for the left side of the window
	 * 3. rs: character to be used for the right side of the window
	 * 4. ts: character to be used for the top side of the window
	 * 5. bs: character to be used for the bottom side of the window
	 * 6. tl: character to be used for the top left corner of the window
	 * 7. tr: character to be used for the top right corner of the window
	 * 8. bl: character to be used for the bottom left corner of the window
	 * 9. br: character to be used for the bottom right corner of the window
	 */
	wrefresh(local_win);
	delwin(local_win);
}
