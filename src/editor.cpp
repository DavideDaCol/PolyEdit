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

vector < string > printAndLoadBuffer(int argc, char* filename, vector < string > file, int &rows, fstream &in, fstream &out, fstream &err);
bool isDirection(char buff);
void arrowMove(char dir,WINDOW* window, vector < string > file, int &xcurr, int &ycurr, int x, int y, int rows);
vector < string > fileEdit(char edit, vector < string > file, int &xcurr, int &ycurr, int &rows, fstream &err);

int main(int argc, char** argv){
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  int x=0, y=0, xcurr=0, ycurr=0, finiter=0;
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

  fstream in,out,err;
  err.open("log.txt",ios::out);
  int rows = 0;
  vector < string > mod = printAndLoadBuffer(argc,argv[1],file,rows,in,out,err);
  move(0,0);
  do{
    buff=getch();
    if(isDirection(buff)){ // TODO: move direction into dedicated function or file
       arrowMove(buff,stdscr,mod,xcurr,ycurr,x,y,rows);//if the input is an arrow key, move cursor (if in bounds)
    } else {
      mod = fileEdit(buff,mod,xcurr,ycurr,rows,err);
    }
  } while(buff!=':');
  string final;
  if(argc == 2){
      out.open(argv[1],ios::out);
    }
  for (int i=0; i<rows; i++){
    out << mod[i];
    out << endl;
  }
  endwin(); //deallocates memory!
  return 0;
}

vector < string > printAndLoadBuffer(int argc, char* filename, vector < string > file, int &rows, fstream &in, fstream &out, fstream &err){
  if(argc == 2 ){ // gets file path
    string buffer;
    in.open(filename,ios::in);
    while(getline(in,buffer)){
      addstr(buffer.c_str()); // ncurses wants c strings
      file.push_back(buffer); // loads modifier buffer
      rows++;
      move(rows,0);
    }
    in.close();
  } else out.open("noname.txt",ios::out);
  return file;
}

bool isDirection(char buff){
  int calc = int(buff); //can probably get rid of this
  if(calc==KEYUP||calc==KEYDOWN||calc==KEYLEFT||calc==KEYRIGHT){
    return true;
  } else return false;
}

void arrowMove(char dir,WINDOW* window, vector < string > file, int &xcurr, int &ycurr, int x, int y, int rows){
  switch (dir) //if the input is an arrow key, move cursor (if in bounds)
      {
      case KEYUP:
        if(ycurr>0){
          --ycurr;
          if(xcurr>file[ycurr].length()){ // wraps cursor to the end of the line
            xcurr=file[ycurr].length()-1;
          }
        }
        break;
      case KEYDOWN:
        if(ycurr<rows-1){
          ++ycurr;
          if(xcurr>file[ycurr].length()){ //same as KEYUP
            xcurr=file[ycurr].length()-1;
          }
        }
        break;
      case KEYLEFT:
        if(xcurr>0){
          xcurr--;
        }
        break;
      case KEYRIGHT:
        if(xcurr<file[ycurr].length()){ 
          xcurr++;
        }
        break;
      
      default:
        break;
      }
      wmove(window,ycurr,xcurr);
      wrefresh(window);
}

vector < string > fileEdit(char edit,  vector < string > file, int &xcurr, int &ycurr, int &rows, fstream &err){
  switch (edit)
  {
    case '\n':
      //TODO: add newline implementation (need to add rows as parameter)
    break;

    case 7: //backspace in cooked mode
      if(xcurr>0){ //moves cursor and erases the character under the cursor
        xcurr--;
        file[ycurr].erase(xcurr,1);
        mvdelch(ycurr,xcurr);
        refresh();
      } else { // edge case: need to go up one line
        string temp = file[ycurr];
        file.erase(file.begin()+ycurr);
        rows--;
        ycurr--;
        file[ycurr].append(temp);
        deleteln();
        mvaddstr(ycurr,xcurr,file[ycurr].c_str());
      }
    break;

    default: //if normal charachter, add first to the file in corresponding line and then to the screen
    file[ycurr].insert(xcurr,1,edit);
    insch(edit);
    xcurr++;
    move(ycurr,xcurr);
    refresh();
    break;
  }
  return file;
}
