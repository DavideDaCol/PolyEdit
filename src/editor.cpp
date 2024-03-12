#include <iostream>
#include <ncurses.h>
#include <fstream>
#include <cstring>
#include <vector>
#include <string>
using namespace std;

int printAndLoadBuffer(int argc, char* filename, vector < string > file, fstream &in, fstream &out);

int main(int argc, char** argv){
  initscr();
  cbreak();
  noecho();

  int x=0, y=0;
  vector < string > file {};
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
  int rows = printAndLoadBuffer(argc,argv[1],file,in,out);
  getch(); // waits for a keypress to exit
  endwin(); //deallocates memory!
  return 0;
}

int printAndLoadBuffer(int argc, char* filename, vector < string > file, fstream &in, fstream &out){
  int rows=0;
  if(argc == 2 ){ // gets file path
    string buffer;
    in.open(filename,ios::in);
    while(getline(in,buffer)){
      addstr(buffer.c_str()); // ncurses wants c strings
      file.push_back(buffer); // loads modifier buffer
      rows++;
      move(rows,0);
    }
  } else out.open("noname.txt",ios::out);
  return rows;
}
