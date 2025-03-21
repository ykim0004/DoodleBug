#include <iostream>
#include <typeinfo>
#include <cstdlib>
#include <time.h>
#include <vector>
using namespace std;

char** createBoard() { //didnt need to be dynamically created (practice)
  char** c = new char*[20]; //an array of 20char pointers
  for (int i = 0; i < 20; i++) {
    c[i] = new char[20]; //
    
    for (int j = 0; j < 20; j++) {//test
      c[i][j] = ' ';
   //   cout <<c[i][j]<< " ";
    }
  }
  return c;
};

char** board = createBoard(); 

class position{ 
public:
  int row;
  int col;

  position(): row( rand()%20 ), col(rand()%20) { }

  void print() {cout << "row: "   << row << " col: " << col <<  endl;}
  
  friend bool operator==(const position& x, const position& y) {
    return x.row == y.row && x.col == y.col;
  }

  friend position spawnD(char** board);

  // friend class Organism; (not needed?)
};

void printB(char**x) { //way of accessing the board
  cout <<endl<<"   01234567890123456789\n";
  for (int i = 0; i < 20; i++) {
    cout << i%10 <<": ";
    for (int j = 0; j <20; j++)
      cout << x[i][j];
    cout <<endl;
  }
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%Helpers%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


char typeAt(const position& p) { 
  if (board[p.row][p.col] == ' ') { 
    cout << "Space" <<endl;
    return ' ';
  }
  else if (board[p.row][p.col] == 'X') {
    cout << "Doodle" <<endl;
    return 'X';
  }
  else if (board[p.row][p.col] == 'o') {
    cout << "Ant" <<endl;
    return 'o';
  }
}

bool occupiedAt(char** board, const position& p) {
  return board[p.row][p.col] != ' ';
} //works! 


position spawnD(char** board) { //prints x at a position on the board randomized by p
  position p; 
//  p.print(); //p works
  while (board[p.row][p.col] != ' ') { //if position is occupied, reroll board.
    position temp; //gets new position
   // temp.print();
    p = temp; //overload not needed, default copy assignment
  }  //p coord is empty. // upon spawn, record these valid positions to each of the organisms 
  board[p.row][p.col] = 'X';
  return p;
}

position spawnA(char** board) { 
  position p; 
//  p.print();
  while (occupiedAt(board, p)) { 
    position temp; 
    p = temp; //shallow
  }  

  board[p.row][p.col] = 'o';

  return p;
}

vector<position> antsAround(const position& p) { //scan: returns an array of ants around a given point 
    vector<position>ants;
    position temp;
    if (board[p.row][p.col-1] == 'o' ) { //left entry
      temp.row = p.row;
      temp.col = p.col-1;
      ants.push_back( temp);
    }
    if (board[p.row][p.col+1] == 'o' ) { //right
      temp.row = p.row;
      temp.col = p.col+1;
      ants.push_back(temp);

      }
    if (board[p.row-1][p.col] == 'o' ) { //up
      temp.row = p.row-1;
      temp.col = p.col;
      ants.push_back(temp);

    }
    if (board[p.row+1][p.col] == 'o' ) {//down
      temp.row = p.row+1;
      temp.col = p.col;
      ants.push_back(temp);
    }
    //print test
    for (int i = 0; i < ants.size(); i++) {
      ants[i].print();
    }
    return ants;
  }

//NOTE: ANY ADDITIONAL CREATURES OUTSIDE THE 125'S COORDINATES WILL HAVE TO BE READJUSTED SINCE CONSTRUCTORS CALL SPAWNDA (which  RANDOMLY chooses a place) 
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Classes %%%%%%%%%%%%%%%%%

  

//                              Organismmm
class Organism {
public: //CHANGE after
  position current;
  int age{0}; //initialize to 0; at the end of every tstep increment by 1. 
  char type;
  bool fertile = false;
  void timeStep() {age++;}
  void markTerritory() {
    board[current.row][current.col] = type;
  }
  bool onBoundaryU() {   //Move directions only reset the position to blank space
   // if (current.row == 0) cout <<"on boundary U " <<endl;
   // else cout << "Not on boundary U" <<endl;
    return current.row == 0;
  }
  bool onBoundaryD() {
  //  if (current.row == 19 ) cout <<"on boundary D " <<endl;
  //  else cout << "Not on boundary D" <<endl;
    return current.row == 19;
  }

  bool onBoundaryL() {
  //  if (current.col == 0 ) cout <<"on boundary L " <<endl;
  //  else cout << "Not on boundary L" <<endl;
    return current.col == 0 ;
  }
  bool onBoundaryR() {
    // if (current.row == 0 || current.row == 19 || current.col == 0  || current.col == 19) cout <<"on boundary R " <<endl;
 //   else cout << "Not on boundary R" <<endl;
    return current.col == 19;
  }
  void moveUp() { //move functions should change the position of the organism
    if(onBoundaryU()) return;
    else { //can legitimately move upwards
      board[current.row][current.col] = ' '; //update this
      --current.row; //movement in Base class will delete current position and advane pointer to new pos
      board[current.row][current.col] = type; 
    }
  }
  void moveDown() {
    if(onBoundaryD()) return;
    else {
      board[current.row][current.col] = ' ';
      ++current.row;
      board[current.row][current.col] = type; 
      }
  }
  void moveLeft() {
    if(onBoundaryL()) return;
    else {
       board[current.row][current.col] = ' ';
       --current.col;
       board[current.row][current.col] = type; 
       }
  }
  void moveRight() {
    if(onBoundaryR()) return;
     else {
       board[current.row][current.col] = ' ';
       ++current.col;
       board[current.row][current.col] = type; 
      }
  }

//PUBLIC: DONT make too big of a change!

  // Organism(): current(spawnA(board)) { //FIXX vtable error 
  //   cout << "Organism position: ";
  //   current.print();
  // }
  Organism() {}
  
  virtual  ~Organism() {}
  virtual void move(){
  int x = rand() % 4;
  if (x == 0) moveUp();
  else if (x == 1) moveDown();
  else if(x == 2) moveLeft();
  else if (x == 3) moveRight();
  }; //non virtual, movement same for all orgs 
  virtual void die() {}; //error without {}  (pg875/906)
  virtual void breed() {};  //omitting {} will cause vtable reference missing error  

  void toggleFertile() { //to use immediately after breed()
    cout << "fertile status before: "<< fertile <<endl;
    if (fertile) fertile = false;
    else fertile = true;
    cout << "fertile status after: " << fertile <<endl;
  } //check logic of this later

  
//getters and setters
  char getType() {
     cout << "organism type: " << type<<endl;
    return type;
  }
  int getAge() { cout <<"Age: " << age << endl; return age;}
  position getPos() {
    current.print();
    return current;
  }
  void setPos(const position p) {
    current = p;
  }
};

//   ANTTTTTTT
class ant:public Organism { //Every ant object needs to have a valid position on board (use pass by reference). make use of spawn/breed feature
  // const char type = 'o';
public:
  ant(): Organism() { 
    type = 'o';
    setPos(spawnA(board)); 
  //  cout <<"Ant constructor called \n";
  //  getPos(); //verify position is same before and after the call. 
  }

  // void breed() {
  //   if (age == 3) 
  //     cout << 1;
  // }

  void move() override {
    Organism::move();
 
  }


};
                  ///  DOODLEBUG
class doodlebug:public Organism {
//define constructor
  
  int starve = 0;
public:
  doodlebug(): Organism() {
    type = 'X';
    setPos(spawnD(board));
 //   cout <<"Doodle constructor called \n";
    // getPos();
  }

  virtual ~doodlebug() {
    cout << "db destroyed \n"; 
  }


  void breed() {
    cout << "db breeding\n";
    
    age = 0;
  }
  //void die ( if old, or starving) 
 
//override move feature; does everything as ants; + extra if condition
  void move() override {
    Organism::move();
    if (age == 8) {
      breed();
    //  cout << age <<endl;
    }
    // cout << "doodleMove called" <<endl;
  //  board[current.row][current.col] = type; 
  }
};  

                 
int main() {
  int t = 0;
  srand(time(0));
//  position x, y, z;
//  x.print(); y.print(); z.print(); //to check if two positions are the same: overload == 
//  cout << (x == y) <<endl; //parentheses needed due to evaluation precedence  
  std::cout << "Ants vs. Doodlebugs\n";
  // char** board = createBoard();
//  printB(board);
  vector<Organism*> life;
   for (int i = 0; i < 5; i++) {
     life.push_back(new doodlebug());
   } //populate with doodlebugs
   
  for (int i = 0; i < 20; i++) {
     life.push_back(new ant());
   }


cout << "                         Initial board:" << t;  
  printB(board);


while ( true) {
//DEBUG
  for (int i = 0; i < life.size(); i ++) {cout << i+1 << ": "; life[i]->getType();} 
  for (int i = 0; i < life.size(); i++) {
    life[i]->move(); //break into critter types
    life[i]->timeStep();

  
//Death phase  die() for db 
    if (life[i]->type == 'X') {
    //  ++(life[i]->age);
     // antsAround( (life[i])->current); //expensive (seg default)
//EAT implementation
      //int s = near.size();
      // if (s != 0) {  
      //   (life[i])->current = near[ s % size];
      // }
      if( life[i]->age  == 3) {
        board[life[i]->current.row][life[i]->current.col] = ' ';
        cout << "db dying... \n";
        life.erase(life.begin() + i);
      }
     
    }

//Death for ant
    // if (life[i]->type == 'o') {
    //   ++(life[i]->age);
    //   if( life[i]->age  == 3) {
    //     board[life[i]->current.row][life[i]->current.col] = ' ';
    //     cout << "db dying... \n";
    //     life.erase(life.begin() + i);
    //   }
     
    // }
  
  
  }
  
  cout << "Size of life[]: " << life.size() <<endl;
  cin.ignore();
  cout << "End of time " << t << ": ";
  printB(board);
  cout << "\n-----------------\n";
  t++;
}
  
  // ant x; // TEST: works! 
  // doodlebug y; // both have valid positions, but are not projected on the map. 
  // doodlebug d; //breed function should include respective symbol based off derived class. 
  // typeAt(y.getPos()); 
  // typeAt(d.getPos());
  // typeAt(x.getPos());
}

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&Next steps
/*
0) remove semi modular (board) argument from all functions, you dont need position class, just row col ints directly
1) Implement move feature; 
  every critter has to move
2) Record current valid positions 
3) Put spawn function in the constructors of Ants and Doodles and then use this to record their initial position

find a way to incorporate positions for each of the 25 organisms.

Upon construcion of each organism, you need to make sure their positions are valid. 

Move: 
  Scan (creates an array of non empty organisms found. )
/
//to be used by void eatAnt() 
char typeAt(const position& p) { 
  if (board[p.row][p.col] == ' ') { //careful for accidental assignment!
    cout << "Space" <<endl;
    return ' ';
  }
  else if (board[p.row][p.col] == 'X') {
    cout << "Doodle" <<endl;
    return 'X';
  }
  else if (board[p.row][p.col] == 'o') {
    cout << "Ant" <<endl;
    return 'o';
  }
}

*/


/*

%%%Sequence of actions in main()
Main() {
		Create board
		Spawn initial organisms
		For each organism O: {
		-Move() //DB prioritizes ant panels over freespace
			
    -breed() //ove



_______
Inside DB move:
	-scan for antsAround(p) 
	
	
	-if (antsAround not empty) {
	Call Eat() {
	     -Randomly choose an entry (thus choosing an ant) 
	    -swap position with o. 
	    - toggle eaten member to on.. 
	}
	
	Else // you havent eaten yet after 3 turns , then db dies{
		Starve++; //initially 0
		If (starve == 3) {
			Delete this; //the DB caller
		} //
	//debug: Check size of array to see there's one less
	
	
	
	//end phase of time step:
	Check all db's; starve member.
	If (starve == 3);
		Delete this. 
	}
	
	
}

___
antsAround() db member function
//returns array of positions containing ants



Breed():
  at end of 3rd and 8th iteration, toggle fertility to yes. 

  Ant:; 3, 6 ,9 ,12 ,15 
  Doodle: End of 8 , 16, 24. 
  Then at each iteration, check if organism is fertile(). 
  -If(fertile), then breed().
  -Reset fertility to false AND  
  -Toggle age back to 0.
  -starving boolean (db only): 




  }





*/