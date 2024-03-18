#include <iostream>
#include <ncurses.h>
#include <fstream>
#include <cstring>
#include <vector>
#include <string>

//TODO: verify behavior: doesn't match online sources
#define KEYUP 3
#define KEYDOWN 2
#define KEYRIGHT 5
#define KEYLEFT 4

using namespace std;

int printAndLoadBuffer(int argc, char* filename, vector < string > file, fstream &in, fstream &out);
bool isDirection(char buff);

int main(int argc, char** argv){
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  int x=0, y=0, xcurr=0, ycurr=0;
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
  do{
    buff=getch();
    if(isDirection(buff)){ // TODO: move direction into dedicated function or file
      switch (buff) //if the input is an arrow key, move cursor (if in bounds)
      {
      case KEYUP:
        if(ycurr>0){
          --ycurr;
        }
        break;
      case KEYDOWN:
        if(ycurr<rows){
          ++ycurr;
        }
        break;
      case KEYLEFT:
        if(xcurr>0){
          xcurr--;
        }
        break;
      case KEYRIGHT:
        if(xcurr<x){
          xcurr++;
        }
        break;
      
      default:
        break;
      }
      wmove(stdscr,ycurr,xcurr);
      wrefresh(stdscr);
    } else {
      // edit file contents
    }
  } while(buff!=':'); 
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

bool isDirection(char buff){
  int calc = int(buff); //can probably get rid of this
  if(calc==KEYUP||calc==KEYDOWN||calc==KEYLEFT||calc==KEYRIGHT){
    return true;
  } else return false;
}