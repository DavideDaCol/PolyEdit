#include <iostream>
#include <ncurses.h>
#include <fstream>
#include <cstring>
#include <string>
using namespace std;

int main(int argc, char** argv){
  initscr(); //starts ncurses screen
  cbreak();
  noecho();

  int x=0, y=0;
  getmaxyx(stdscr, y, x); // term size
  refresh();

  const char title[24] = "PolyEdit: PolyML Editor";
  const char subtitle[33] = "press i to start, ctrl+c to exit";

  mvprintw(y/2,x/2-strlen(title)/2,title); //printw needs a const string
  mvprintw(y/2+1,x/2-strlen(subtitle)/2,subtitle);
  refresh();

  char buff; 
  do{
    buff=getch();
  } while(buff!='i');

  clear();

  fstream in,out;

  if(argc == 2 ){ // gets file path
    int rows=0, cols=0;
    string buffer;
    in.open(argv[1],ios::in);
    while(getline(in,buffer)){
      addstr(buffer.c_str());
      rows++;
      move(rows,0);
    } 
  } else out.open("noname.txt",ios::out);
  getch(); // waits for a keypress to exit
  endwin(); //deallocates memory!
  return 0;
}
