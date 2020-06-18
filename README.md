# Sudoku

OS Project Building Sudoku validator implementing core operating system concepts.    
Phase 1

For file writing ofstream object ‘out’ is  created.

Mutex object ‘mutex1’ is initialized.

Array of integer type ‘game’ 9x9 is created. 
int game[9][9] = 
{ { 6, 6, 6, 6, 3, 9, 1, 8, 7 },
		{ 5, 1, 9, 7, 2, 8, 6, 3, 4 },
 { 8, 3, 7, 6, 1, 4, 2, 9, 5 }, 
{ 1, 4, 3,8, 6, 5, 7, 2, 9 }, 
{ 9, 5, 8, 8, 4, 7, 3, 6, 1 },		
{ 7, 6, 2, 3, 9, 1, 4, 5, 8 }, 
{ 3, 7, 1, 9, 5, 6, 8, 4, 2 }, 
{ 4, 9, 6, 1, 8, 2, 5, 7, 3 }, 
{ 2, 8, 5, 4, 7, 3, 9, 1, 6 } };
Structure with object ‘parameters’ is created for passing data to thread includes row (to select from which row to start) and col(to select from which column to start).
	
Thread IDs are created rowcheckid[9], colcheckid[9], boxid[9].

Structure called ‘pairr’ is created with data members row, column, index, repeat, box, boxno and id. 
Its member functions are: Insert, Clear, displayBoxes, displayR, displayC.

void insert(int v, int r, int c, pthread_t id, bool repeat = false,bool box=false,int boxno=-1) 
{
	Sets values to object data members according to arguments passed.
}

void clear() 
{
	Sets all integers to -1 and bool to false.	
}

void displayboxes(ofstream &out) 
{
	Writes in file in case of repeat and otherwise.
	Invalid box no, Invalid value, Invalid row, Invalid col.		
}

void displayr(ofstream &out) & void displayc(ofstream &out)
{
	Writes in file in case of repeat and otherwise.
	Invalid value, Invalid row, Invalid col.
      }
The following global Vectors are created:

Row Arrays:
vector<pairr> invalidrows;
vector<int> irows;

Column Arrays:
vector<int> icols;
vector<pairr> invalidcolumns;

Invalid Boxes Arrays:
vector<int> invalidboxes;
vector<pairr> invalidrowsboxes;

A global object ‘temp’ of datatype pair is created.

A global Array ‘indexes’ of size 9 having values 0-8 are created.

void *rowcheck(void *arg) 
{
	Mutex locks

	int chck[10] = {0};

	Loop(i=0 to 9)
 	{
		a= row number passed as argument.
		i= column currently being checked.
		
		Checking invalid values
if (game[a][i] less 0 or greater than 9) 
		{
			if (chck2==false) 
			{
				if(chck1==false)
				{
					insert entire row into vector irow;
				}
				temp.insert(game[a][i], a, i, rowcheckid[a]);
				inserts invalid values into ‘temp’ object

				invalidrows.push_back(temp);
inserts invalid values from temp into invalidrows vector.

			}
		}
		
		No invalid values now check for repetition.	
	
		if (chck[game[a][i]] == 0) 
		{
			NO repetition, Save value for later in chck array
			chck[game[a][i]] = game[a][i];
		} 

		else if (value matches value saved in chck array)
	       {
			REPETITION EXISTS.
			if (chck1 == false) 
			{
		 	Inserts invalid values into ‘temp’ object
			Inserts invalid values from temp into invalidrows vector.
			} 

			else 
			{
			Inserts invalid values into ‘temp’ object
			Inserts invalid values from temp into invalidrows vector.
			}
		}
	}
	Mutex Unlocks
	Thread exits
}
void *colcheck(void *arg) 
{
	Mutex locks

	int chck[10] = {0};

	Loop(i=0 to 9)
 	{
		a= col number passed as argument.
		i= column currently being checked.
		
		Checking invalid values
if (game[i][a] less 0 or greater than 9) 
		{
			if (chck2==false) 
			{
				if(chck1==false)
				{
					insert entire col into vector icol;
				}
				temp.insert(game[i][a], i, a, colcheckid[a]);
				inserts invalid values into ‘temp’ object

				invalidcolumns.push_back(temp);
inserts invalid values from temp into invalidcolumns vector.

			}
		}
		
		No invalid values now check for repetition.	
	
		if (chck[game[i][a]] == 0) 
		{
			NO repetition, Save value for later in chck array
			chck[game[i][a]] = game[i][a];
		} 

		else if (value matches value saved in chck array)
	       {
			REPETITION EXISTS.
			if (chck1 == false) 
			{
		 	Inserts invalid values into ‘temp’ object
			Inserts invalid values from temp into invalidcolumns vector.
			} 

			else 
			{
			Inserts invalid values into ‘temp’ object
			Inserts invalid values from temp into invalidcolumns vector.
			}
		}
	}
	Mutex Unlocks
	Thread exits
}
void *checkboxes(void *args) 
{
	Mutex locks

	int chck[10] = {0};

	parameters *data = arguments passed;

	int row=data->row;
	int column =data->col;
	int boxnoo=data->boxno;

	Loop(i=row to row+3)
 	{
				
		for(j=column to column+3)
		{	
			if (game[i][j] less 0 or greater 9) 
			{
				if (chck2==false) 
				{
					if(chck==false)
					{
						insert entire box into vector invalidboxes
					}
					Inserts invalid values into ‘temp’ object
					Inserts invalid values from temp into invalidrowsboxes vector.
				}
			}
		
			if(chck1[game[i][j]]==0)
			{	
				NO repetition, Save value for later in chck1 array
				chck1[game[i][j]]=game[i][j];
			}
			
			else if(value matches value saved in chck array)
			{
				REPETITION EXISTS.
				if(chck==false)
				{
					Inserts invalid values into ‘temp’ object
					Inserts invalid values from temp into invalidrowsboxes vector.
				}
				else
				{	
					Inserts invalid values into ‘temp’ object
					Inserts invalid values from temp into invalidrowsboxes vector.
				}
			}

		}
	
	}
	Mutex Unlocks
	Thread exits
}

int main() 
{
	ALL OUTPUTS ARE WRITTEN IN FILE

	out.open("ibrar.txt");

	Parameters Declaration 1-9
	parameters*data1-9 = (parameters *) malloc(sizeof(parameters));
	data1-9 ->row = values;
	data1-9 ->col = values;
	data1-9 ->boxno = 1;
	
	Threads for rows, cols, boxes created:
	pthread_create(&rowcheckid[0-9], NULL, rowcheck, &indexes[0-9]);
	pthread_create(&colcheckid[0-9], NULL, colcheck, &indexes[0-9]);
	pthread_create(&boxid[0-9],NULL,checkboxes,data1);
	 
	Threads joined:
	pthread_join(rowcheckid[0-9], NULL);
	pthread_join(colcheckid[0-9], NULL);
	pthread_join(boxid[0-9], NULL);
	
	
	Write file for Invalid Rows:
	for (i=0 < size of irow vector) 
	{
		Write invalid row numbers:
		out << "Invalid Rows is:" << irows[i] + 1 << endl;
	}
	for (i=0 < size of invalidrows vector) 
	{
		Displays Invalid values in invalidrows vector.
	}
		
	Write file for Invalid Cols:
	for (i=0 < size of icol vector) 
	{
		Write invalid col numbers:
		out << "Invalid Rows is:" << icols[i] + 1 << endl;
	}
	for (i=0 < size of invalidcolumns vector) 
	{
		Displays Invalid values in invalidcolumns vector.
	}

	Write file for Invalid Boxes:
	for (i=0 < size of invalidboxes vector) 
	{
		Write invalid box numbers:
		out << "Invalid Box is:" << invalidboxes [i] + 1 << endl;
	}
	for (i=0 < size of invalidrowsboxes vector) 
	{
		Displays Invalid values in invalidrowsboxes vector.
	}

OS Project Report
Phase 2

In Phase 2 code is built upon previous Work in done Phase 1 according to logic required..
int j = 0;
bool mainchck = false;
New structure pair! Is created that holds index(rowno and colon):
struct pairr1
 {
	int row;
	int column;
	pairr1() 
{
		Set row & column to -2.
	}
	void clear()
 {
		Resets row & column to -2.	}
	void insert(int row, int col) 
{
		Inserts row & column to arguments.	
}
};

The following  new global Vectors are created:
vector<pairr1> invlaidindexes;

bool checkrow(int value, int row) 
{
	Returns true if row has number equal to value passed.
}

bool checkcol(int value, int col) 
{
Returns true if col has number equal to value passed.
}
void functioninvalidrows(pairr temp2)
 {
	Sets int row, column, value  to values in temp2.
	Loop of i  = 0-8
	{
			if (game[row][i] == value) 
				game[row][i] = -1;
			In sort function sets invalid numbers in row to -1 comparing it to arg temp 2
	}
}
void indexstore()
 {
	Stored indexes with value -1 into invlaidindexes.push_back(temp3) vector.
}

void functioninvalidcolumns(pairr temp2) {
	Sets int row, column, value  to values in temp2.
	Loop of i  = 0-8
{
			if (game[i][column] == value)
				game[i][column] = -1;
In sort function sets invalid numbers in col to -1 comparing it to arg temp 2
	}
}
bool searchrow=true;
bool searchcolumn=true;
void * search(void *args)
{	
             Mutex locks

	Set searchcolumn & searchrow=true;
	indstr *data=(indstr *)args;
	searchrow=checkrow(data->index,data->temp.row);
	searchcolumn=checkcol(data->index,data->temp.column);
             Mutex Unlocks
             Thread exits
}
void displaygame() 
{
	Displays game array.
}

void intro()
{	
	Prints SODUKO in a fancy way.	
}

bool sudokusolver() 
 {
	Epic finction fixes suduko.
Return false invlaidindexes.size()==0
	Else
{
	Another nine thread IDs initialized pthread_t sid[9];
	pairr1 temp2;
	temp2=invlaidindexes[j];
	indstr *data;
	if(j==invlaidindexes.size())
		return true;
	else
	{
int i=1;
		for(i;i<9;i++)
		{
			data=(indstr *) malloc(sizeof(indstr));
			data->index=i;
			data->temp=temp2;
			pthread_create(&sid[i],NULL,search,data);
			pthread_join(sid[i],NULL);
			totalthreads+=1;
			if(searchrow==false && searchcolumn==false)
			{
				game[temp2.row][temp2.column]=i;
				j++;
				if(sudoku())
					return true;
				else
					game[temp2.row][temp.column]=-1; 
			}
		}
	}
	return false;
}
}

void * mainfunc(void *args)
 {
	Thread created by main ();
	Here game array is fixed.
             Mutex locks

	Old matrice is displayed :  displaygame(out);
	Invalid rows numbers are wriiten in the file “ibrar.txt”
	Displays entire invalid row in the file
	Invalid col numbers are wriiten in the file “ibrar.txt”
	Displays entire invalid col in the file
	Invalid box numbers are wriiten in the file “ibrar.txt”
	Displays entire invalid box in the file	
	Passes invalid cols and rows to functioninvalidrows and functioninvalidcolumns through object temp1
	functioninvalidrows(temp1);
	functioninvalidcolumns(temp1);
	call indexstore();
	call sudoku();
	New matrice is displayed :  displaygame(out);	
Mutex unlocks and thread exists
}


int main()
 {
	Previously Open file for writing, Parameters Declaration 1-9, Threads for rows, cols, boxes created and	Threads joined.
	
	Call intro()  to write fancy Suduko on file.
Create  Main thread
	pthread_create(&mainn[0], NULL, mainfunc, NULL);
	Join Main Thread 
	pthread_join(mainn[0], NULL);
}

Computer Specification:

Implementation in another scenario:
Concepts of Multithreading  can be applied to any game allowing code to run in parallel, one example can be tic tac toe game where similarly two players can simultaneous play, an array of 3*3 where combinations can be checked by threads running in parallel. Or a 3D game where multiple parts of code run in parallel.
