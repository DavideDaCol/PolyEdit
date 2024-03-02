#include <ncurses.h>
#include <cstring>
using namespace std;

int main(int argc, char** argv){
  initscr(); //starts ncurses screen
  cbreak();
  noecho();

  int x=0, y=0;
  getmaxyx(stdscr, y, x); // term size
  refresh();

  const char * title = "PolyEdit: PolyML Editor";
  const char * subtitle = "press ctrl+c to exit";

  mvprintw(y/2,x/2-strlen(title)/2,title); //printw needs a const string
  mvprintw(y/2+1,x/2-strlen(subtitle)/2,subtitle);
  refresh();

  char buff = getch(); 
  clear();
  printw("%c", buff); //TODO: learn this syntax
  getch(); // waits for a keypress to exit
  endwin(); //deallocates memory!
  return 0;
}
