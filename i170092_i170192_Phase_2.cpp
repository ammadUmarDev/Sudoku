//
//
// PROJECT BY : I170192 & I170092.
//
//
#include<iostream>
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<unistd.h>
#include<fstream>
using namespace std;
//object for writing
ofstream out;
int treegrowth = 0;
int j = 0;
int totalthreads = 0;
//initializing mutex
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex3 = PTHREAD_MUTEX_INITIALIZER;
//game array 9x9
int game[9][9] = { { 6, 2, -1, 5, 3, 9, 1, 8, 7 },
		{ 5, 1, -2, 7, 2, 8, 6, 3, 4 }, { 8, 3, 7, 6, 1, 4, 2, 9, 5 }, { 1, 4,
				3, 8, 6, 5, 7, 2, 9 }, { 9, 5, 8, 2, 4, 7, 3, 6, 1 }, { 7, 6, 2,
				3, 9, 9, 4, 5, 8 }, { 3, 7, 1, 9, 5, 6, 8, 4, 2 }, { 4, 9, 6, 1,
				8, 2, 5, 7, 3 }, { 1, 8, 5, 4, 7, 3, 9, 1, 1 } };
//New structure pair! Is created that holds index(rowno and colon)
struct pairr1 {
	int row;
	int column;
	//Set row & column to -2.
	pairr1() {
		this->row = -2;
		this->column = -2;
	}
	//Resets row & column to -2.
	void clear() {
		this->row = -2;
		this->column = -2;
	}
	//Inserts row & column to arguments.
	void insert(int row, int col) {
		this->row = row;
		this->column = col;
	}
};
/* structure for passing data to threads */
typedef struct {
	int row; //To select from which row to start
	int col; //To select from which column to start
	int boxno;
} parameters;
/*structure for pthread row and column*/
typedef struct {
	pairr1 temp;
	int index;
} indstr;
//pthread ids
pthread_t rowcheckid[9];
pthread_t colcheckid[9];
pthread_t boxid[9];
pthread_t mainn;
pthread_t rowcheckid1[9];
pthread_t colcheckid1[9];
pthread_t boxid1[9];
//Structure called ‘pairr’ is created with data members row, column, index, repeat, box, boxno and id.
//Its member functions are: Insert, Clear, displayBoxes, displayR, displayC.
struct pairr {
	int row;
	int column;
	int index;
	bool repeat;
	bool box;
	int boxno;
	pthread_t id;
public:
	pairr() {
		this->index = 0;
		this->row = 0;
		this->column = 0;
		this->repeat = false;
		this->box = false;
		this->boxno = -1;
	}
	//Sets values to object data members according to arguments passed.
	void insert(int v, int r, int c, pthread_t id, bool repeat = false,
			bool box = false, int boxno = -1) {
		this->index = v;
		this->row = r;
		this->column = c;
		this->id = id;
		this->repeat = repeat;
		this->box = box;
		this->boxno = boxno;
	}
	//Sets all integers to -1 and bool to false.
	void clear() {
		this->index = -1;
		this->row = -1;
		this->column = -1;
		this->repeat = false;

	}
	//The following writes in file in case of repeat and otherwise.
	//	Invalid value, Invalid row, Invalid col.
	void displayboxes(ofstream &out) {
		if (repeat == false) {
			out << "The invalid box is:" << boxno
					<< ".The invalid value of the box is " << index
					<< " The row is " << row + 1 << " And the column is "
					<< column + 1 << " The thread id is:" << this->id << endl;
		}
		if (repeat == true) {
			out << "The invalid box is:" << boxno
					<< ".The value that repeated is:" << index << " at row:"
					<< row + 1 << " .And the column is:" << column + 1
					<< " The thread id is:" << this->id << endl;
		}
	}
	void displayr(ofstream &out) {

		if (repeat == false) {
			out << "The invalid row value is " << index << " The row is "
					<< row + 1 << " And the column is " << column + 1
					<< " The thread id is:" << this->id << endl;
		}
		if (repeat == true) {
			out << "The value that repeated in row " << row + 1 << " is:"
					<< index << ".And the column is:" << column + 1
					<< " The thread id is:" << this->id << endl;
		}
	}
	void displayc(ofstream &out) {

		if (repeat == false) {
			out << "The invalid column value is " << index << " The row is "
					<< row + 1 << " And the column is " << column + 1
					<< " The thread id is:" << this->id << endl;
		}
		if (repeat == true) {
			out << "The value that repeated in column " << column + 1 << " is:"
					<< index << ".And the row is:" << row + 1
					<< " The thread id is:" << this->id << endl;
		}
	}

};
// The following global Vectors are created:
vector<pairr> invalidrows;
vector<int> irows;
/*column arrays*/
vector<int> icols;
vector<pairr> invalidcolumns;
/*invalid boxes array*/
vector<int> invalidboxes;
vector<pairr> invalidrowsboxes;
/*-1 indexs vector*/
vector<pairr1> invlaidindexes;
/*parameters*/
parameters*data1;
parameters*data2;
parameters*data3;
parameters*data4;
parameters*data5;
parameters*data6;
parameters*data7;
parameters*data8;
parameters*data9;
/*parameters*/
//A global object ‘temp’ of datatype pair is created.
pairr temp;
int indexes[9] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
//Displays game array in file.
void displaygame(ofstream &out) {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			out << game[i][j] << " ";
		}
		out << endl;
	}
}
//Returns true if row has number equal to value passed.
bool checkrow(int value, int row) {
	for (int i = 0; i < 9; i++) {
		if (game[row][i] == value) {
			return true;
		}
	}
	return false;
}
//Returns true if col has number equal to value passed.
bool checkcol(int value, int col) {

	for (int i = 0; i < 9; i++) {
		if (game[i][col] == value) {
			return true;
		}
	}
	return false;
}
//Displays game array.
void displaygame() {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			cout << game[i][j] << " ";
		}
void functioninvalidrows(pairr temp2) {
	int row = temp2.row;
	int column = temp2.column;
	int value = temp2.index;
	for (int i = 0; i < 9; i++) {
		if (i != column || i == column) {
			if (game[row][i] == value) {
				game[row][i] = -1;
			}
		}
	}
}
//Finction fixes suduko.
bool sudokusolver() {
	if (invlaidindexes.size() == 0) {
		return false;
	} else {
		bool row;
		bool col;
		pairr1 temp2;
		temp2 = invlaidindexes[j];
		if (j == invlaidindexes.size()) {
			return true;
		} else {
			int i = 0;
			for (i; i < 9; i++) {
				totalthreads += 1;
				row = checkrow(i + 1, temp2.row);
				col = checkcol(i + 1, temp2.column);
				if (row == false && col == false) {
					treegrowth += 1;
					game[temp2.row][temp2.column] = i + 1;
					j++;
					if (sudokusolver()) {
						return true;
					} else {
						game[temp2.row][temp2.column] = -1;
					}
				}

			}
		}
		return false;
	}
}
//In sort function sets invalid numbers in col to -1 comparing it to arg temp 2
void functioninvalidcolumns(pairr temp2) {
	int row = temp2.row;
	int column = temp2.column;
	int value = temp2.index;
	for (int i = 0; i < 9; i++) {
		if (i != row || i == row) {
			if (game[i][column] == value) {
				game[i][column] = -1;
			}
		}
	}
}
bool searchrow = true;
bool searchcolumn = true;
void * search(void *args) {
	pthread_mutex_lock(&mutex2);
	searchcolumn = true;
	searchrow = true;
	indstr *data = (indstr *) args;
	searchrow = checkrow(data->index, data->temp.row);
	searchcolumn = checkcol(data->index, data->temp.column);
	pthread_mutex_unlock(&mutex2);
	pthread_exit(NULL);
}
//Stored indexes with value -1 into invlaidindexes.push_back(temp3) vector.
void indexstore() {
	pairr1 temp3;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (game[i][j] == -1) {
				temp3.row = i;
				temp3.column = j;
				invlaidindexes.push_back(temp3);
				temp.clear();

			}
		}
	}

}
void *rowcheck(void *arg) {
	bool chck1 = false;
	bool chck2 = false;
	pairr temp;
	int chck[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int a;
	a = *((int *) arg);
	pthread_mutex_lock(&mutex1);
	for (int i = 0; i < 9; i++) {

		if (game[a][i] <= 0 || game[a][i] > 9) {
			if (chck2 == false) {
				if (chck1 == false) {
					irows.push_back(a);
				}
				temp.insert(game[a][i], a, i, rowcheckid[a]);
				invalidrows.push_back(temp);
				chck1 = false;
				chck2 = true;
				temp.clear();

			}
		}
		if (chck[game[a][i]] == 0) {
			chck[game[a][i]] = game[a][i];
		} else if (game[a][i] == chck[game[a][i]] && chck[game[a][i]] != 0) {
			if (chck1 == false) {
				irows.push_back(a);
				chck1 = true;
				temp.insert(game[a][i], a, i, rowcheckid[a], true);
				invalidrows.push_back(temp);
				temp.clear();

			} else {
				temp.insert(game[a][i], a, i, rowcheckid[a], true);
				invalidrows.push_back(temp);
				temp.clear();
			}
		}

	}
	pthread_mutex_unlock(&mutex1);
	pthread_exit(NULL);
}
void *colcheck(void *arg) {

	bool chck1 = false;
	bool chck2 = false;
	pairr temp;
	int chck[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int a;
	a = *((int *) arg);
	pthread_mutex_lock(&mutex1);
	for (int i = 0; i < 9; i++) {
		if (game[i][a] == 0 || game[i][a] > 9 || game[i][a] < 0) {
			if (chck2 == false) {
				if (chck1 == false) {
					icols.push_back(a);
				}
				temp.insert(game[i][a], i, a, colcheckid[a]);
				invalidcolumns.push_back(temp);
				chck1 = false;
				chck2 = true;
				temp.clear();

			}
		}
		if (chck[game[i][a]] == 0) {
			chck[game[i][a]] = game[i][a];
		} else if (game[i][a] == chck[game[i][a]] && chck[game[i][a]] != 0) {
			if (chck1 == false) {
				icols.push_back(a);
				chck1 = true;
				temp.insert(game[i][a], i, a, colcheckid[a], true);
				invalidcolumns.push_back(temp);
				temp.clear();

			} else {
				temp.insert(game[i][a], i, a, colcheckid[a], true);
				invalidcolumns.push_back(temp);
				temp.clear();
			}
		}

	}

	pthread_mutex_unlock(&mutex1);
	pthread_exit(NULL);
}
void *checkboxes(void *args) {

	pairr temp;
	parameters *data = (parameters *) args;
	int row = data->row;
	int column = data->col;
	int boxnoo = data->boxno;
	bool chck = false;
	bool chck2 = false;
	int chck1[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	pthread_mutex_lock(&mutex1);
	for (int i = row; i < row + 3; i++) {
		for (int j = column; j < column + 3; j++) {
			if (game[i][j] == 0 || game[i][j] > 9 || game[i][j] < 0) {
				if (chck2 == false) {
					if (chck == false) {
						invalidboxes.push_back(boxnoo);
					}
					temp.insert(game[i][j], row, column, boxid[(boxnoo) - 1],
							false, true, boxnoo);
					invalidrowsboxes.push_back(temp);
					chck = false;
					chck2 = true;
					temp.clear();
				}
			}
			if (chck1[game[i][j]] == 0) {
				chck1[game[i][j]] = game[i][j];
			} else if (chck1[game[i][j]] == game[i][j]
					&& chck1[game[i][j]] != 0) {

				if (chck == false) {
					invalidboxes.push_back(boxnoo);
					temp.insert(game[i][j], row, column, boxid[(boxnoo) - 1],
							true, true, boxnoo);
					invalidrowsboxes.push_back(temp);
					chck = true;
					temp.clear();
				} else {
					temp.insert(game[i][j], row, column, boxid[(boxnoo) - 1],
							true, true, boxnoo);
					invalidrowsboxes.push_back(temp);
					temp.clear();
				}
			}

		}

	}
	pthread_mutex_unlock(&mutex1);
	pthread_exit(NULL);

}
bool sudoku() {
	//Return false invlaidindexes.size()==0
	if (invlaidindexes.size() == 0) {
		return false;
	} else {
		pthread_t sid[9];
		pairr1 temp2;
		temp2 = invlaidindexes[j];
		indstr *data;
		if (j == invlaidindexes.size()) {
			return true;
		} else {
			int i = 1;
			for (i; i < 9; i++) {
				data = (indstr *) malloc(sizeof(indstr));
				data->index = i;
				data->temp = temp2;
				pthread_create(&sid[i], NULL, search, data);
				pthread_join(sid[i], NULL);
				totalthreads += 1;
				if (searchrow == false && searchcolumn == false) {
					game[temp2.row][temp2.column] = i;
					j++;
					treegrowth += 1;
					if (sudoku()) {
						treegrowth += 1;
						return true;
					} else {
						game[temp2.row][temp.column] = -1;
					}
				}

			}
			
		}
		return false;
	}
}
//Prints SODUKO in a fancy way.
void intro() {
	out
			<< "---------------------------------------------------------------------------------\n";

	out
			<< "*********************************************************************************\n";
	out
			<< "*********************************************************************************\n";
	out
			<< "*********************************************************************************\n";
	out
			<< "*********************************************************************************\n";
	out << "\n";
	out << "\n";
	out << "\n";
	out
			<< "            *******  **    **  ******     ******   **    **  **    **\n";
	out
			<< "            *******  **    **  *******   ********  **   **   **    **\n";
	out
			<< "            **       **    **  **    **  **    **  **  **    **    **\n";
	out
			<< "            **       **    **  **    **  **    **  ** **     **    **\n";
	out
			<< "            *******  **    **  **    **  **    **  ****      **    **\n";
	out
			<< "            *******  **    **  **    **  **    **  ****      **    **\n";
	out
			<< "                ***  **    **  **    **  **    **  ** **     **    **\n";
	out
			<< "                ***  **    **  **    **  **    **  **  **    **    **\n";
	out
			<< "            *******  ********  *******   *******   **   **   ********\n";
	out
			<< "            *******   ******   ******     *****    **    **   ******\n";
	out << "\n";
	out << "\n";
	out
			<< "                           PROJECT BY: I170192 & I170092                 \n";
	out << "\n";
	out << "\n";
	out
			<< "*********************************************************************************\n";
	out
			<< "*********************************************************************************\n";
	out
			<< "*********************************************************************************\n";
	out
			<< "*********************************************************************************\n";

	out
			<< "---------------------------------------------------------------------------------\n";
	out << "\n";
}
//Declares Parameters, Creates threads, Join threads for Validation
void validator() {
	parameters*data1;
	parameters*data2;
	parameters*data3;
	parameters*data4;
	parameters*data5;
	parameters*data6;
	parameters*data7;
	parameters*data8;
	parameters*data9;
	int size[3];
	size[0] = invalidrows.size();
	size[1] = invalidcolumns.size();
	size[2] = invalidrowsboxes.size();
	//Parameters Declaration1
	data1 = (parameters *) malloc(sizeof(parameters));
	data1->row = 0;
	data1->col = 0;
	data1->boxno = 1;
	//Parameters Declaration2
	data2 = (parameters *) malloc(sizeof(parameters));
	data2->row = 0;
	data2->col = 3;
	data2->boxno = 2;
	//Parameters Declaration3
	data3 = (parameters *) malloc(sizeof(parameters));
	data3->row = 0;
	data3->col = 6;
	data3->boxno = 3;
	//Parameters Declaration4
	data4 = (parameters *) malloc(sizeof(parameters));
	data4->row = 3;
	data4->col = 0;
	data4->boxno = 4;
	//Parameters Declaration5
	data5 = (parameters *) malloc(sizeof(parameters));
	data5->row = 3;
	data5->col = 3;
	data5->boxno = 5;
	//Parameters Declaration6im a mess i'm a loser ima hater i'm a user
	data6 = (parameters *) malloc(sizeof(parameters));
	data6->row = 3;
	data6->col = 6;
	data6->boxno = 6;
	//Parameters Declaration7
	data7 = (parameters *) malloc(sizeof(parameters));
	data7->row = 6;
	data7->col = 0;
	data7->boxno = 7;
	//Parameters Declaration8
	data8 = (parameters *) malloc(sizeof(parameters));
	data8->row = 6;
	data8->col = 3;
	data8->boxno = 8;
	//Parameters Declaration9
	data9 = (parameters *) malloc(sizeof(parameters));
	data9->row = 6;
	data9->col = 6;
	data9->boxno = 9;
	/*Threads for rows*/
	pthread_create(&rowcheckid1[0], NULL, rowcheck, &indexes[0]);
	pthread_create(&rowcheckid1[1], NULL, rowcheck, &indexes[1]);
	pthread_create(&rowcheckid1[2], NULL, rowcheck, &indexes[2]);
	pthread_create(&rowcheckid1[3], NULL, rowcheck, &indexes[3]);
	pthread_create(&rowcheckid1[4], NULL, rowcheck, &indexes[4]);
	pthread_create(&rowcheckid1[5], NULL, rowcheck, &indexes[5]);
	pthread_create(&rowcheckid1[6], NULL, rowcheck, &indexes[6]);
	pthread_create(&rowcheckid1[7], NULL, rowcheck, &indexes[7]);
	pthread_create(&rowcheckid1[8], NULL, rowcheck, &indexes[8]);
	/*Threads for columns*/
	pthread_create(&colcheckid1[0], NULL, colcheck, &indexes[0]);
	pthread_create(&colcheckid1[1], NULL, colcheck, &indexes[1]);
	pthread_create(&colcheckid1[2], NULL, colcheck, &indexes[2]);
	pthread_create(&colcheckid1[3], NULL, colcheck, &indexes[3]);
	pthread_create(&colcheckid1[4], NULL, colcheck, &indexes[4]);
	pthread_create(&colcheckid1[5], NULL, colcheck, &indexes[5]);
	pthread_create(&colcheckid1[6], NULL, colcheck, &indexes[6]);
	pthread_create(&colcheckid1[7], NULL, colcheck, &indexes[7]);
	pthread_create(&colcheckid1[8], NULL, colcheck, &indexes[8]);
	/*boxes treads*/
	pthread_create(&boxid1[0], NULL, checkboxes, data1);
	pthread_create(&boxid1[1], NULL, checkboxes, data2);
	pthread_create(&boxid1[2], NULL, checkboxes, data3);
	pthread_create(&boxid1[3], NULL, checkboxes, data4);
	pthread_create(&boxid1[4], NULL, checkboxes, data5);
	pthread_create(&boxid1[5], NULL, checkboxes, data6);
	pthread_create(&boxid1[6], NULL, checkboxes, data7);
	pthread_create(&boxid1[7], NULL, checkboxes, data8);
	pthread_create(&boxid1[8], NULL, checkboxes, data9);
	totalthreads += 27;
	/*Row joins*/
	pthread_join(rowcheckid1[0], NULL);
	pthread_join(rowcheckid1[1], NULL);
	pthread_join(rowcheckid1[2], NULL);
	pthread_join(rowcheckid1[3], NULL);
	pthread_join(rowcheckid1[4], NULL);
	pthread_join(rowcheckid1[5], NULL);
	pthread_join(rowcheckid1[6], NULL);
	pthread_join(rowcheckid1[7], NULL);
	pthread_join(rowcheckid1[8], NULL);
	/*Columns joins*/
	pthread_join(colcheckid1[0], NULL);
	pthread_join(colcheckid1[1], NULL);
	pthread_join(colcheckid1[2], NULL);
	pthread_join(colcheckid1[3], NULL);
	pthread_join(colcheckid1[4], NULL);
	pthread_join(colcheckid1[5], NULL);
	pthread_join(colcheckid1[6], NULL);
	pthread_join(colcheckid1[7], NULL);
	pthread_join(colcheckid1[8], NULL);
	/*boxes joins*/
	pthread_join(boxid1[0], NULL);
	pthread_join(boxid1[1], NULL);
	pthread_join(boxid1[2], NULL);
	pthread_join(boxid1[3], NULL);
	pthread_join(boxid1[4], NULL);
	pthread_join(boxid1[5], NULL);
	pthread_join(boxid1[6], NULL);
	pthread_join(boxid1[7], NULL);
	pthread_join(boxid1[8], NULL);
	if (invalidrows.size() == size[0] && invalidcolumns.size() == size[1]
			&& invalidrowsboxes.size() == size[2]) {
		out << endl;
		out << "The suduko is solved corrected validated by the validator ";
	}

}
//Writes into file "ibrar.txt"
void * mainfunc(void *args) {
	pthread_mutex_lock(&mutex3);
	//Displays Sudoku
	intro();
	//Display invalid matrix
	out << "The old matrix unsolved is:" << endl;
	displaygame(out);
	out << endl << endl;
	out << "Invalid Rows:" << endl;
	//Invalid rows numbers are wriiten in the file “ibrar.txt”
	for (int i = 0; i < irows.size(); i++) {
		out << "Invalid Rows is:" << irows[i] + 1 << endl;
	}
	data7 = (parameters *) malloc(sizeof(parameters));
	for (int i = 0; i < invalidrows.size(); i++) {
		temp = invalidrows[i];
		temp.displayr(out);
		temp.clear();
	}
	out << endl << endl;
	out << "Invalid Columns:" << endl;
	//Invalid col numbers are wriiten in the file “ibrar.txt”
	for (int i = 0; i < icols.size(); i++) {
		out << "Invalid columns is:" << icols[i] + 1 << endl;
	}
	for (int i = 0; i < invalidcolumns.size(); i++) {
		temp = invalidcolumns[i];
		temp.displayc(out);
		temp.clear();
	}
	out << endl << endl;
	out << "Invalid Boxes:" << endl;
	//Invalid box numbers are wriiten in the file “ibrar.txt”
	for (int i = 0; i < invalidboxes.size(); i++) {
		out << "invalid box is:" << invalidboxes[i] << endl;
	}
	for (int i = 0; i < invalidrowsboxes.size(); i++) {
		temp = invalidrowsboxes[i];
		temp.displayboxes(out);
	}
	for (int i = 0; i < invalidrows.size(); i++) {

		pairr temp1;
		temp1 = invalidrows[i];
		functioninvalidrows(temp1);
	}
	for (int i = 0; i < invalidcolumns.size(); i++) {
		pairr temp1;
		temp1 = invalidcolumns[i];
		functioninvalidcolumns(temp1);
	}
	//Validating matrix
	indexstore();
	sudoku();
	sudokusolver();
	//Display invalid matrix
	out << endl << endl << "The New solved sudoko is:" << endl;
	displaygame(out);
	//	validator();
	out << endl << "The Total number of threads formed are:" << totalthreads
			<< endl;
	out << endl << "The total levels of the tree are:" << treegrowth << endl;
	validator();
	out << endl << "******GAME OVER*****" << endl;
	pthread_mutex_unlock(&mutex3);
	pthread_exit(NULL);
}

int main() {
	out.open("ibrar.txt");
	//Parameters Declaration1
	data1 = (parameters *) malloc(sizeof(parameters));
	data1->row = 0;
	data1->col = 0;
	data1->boxno = 1;
	//Parameters Declaration2
	data2 = (parameters *) malloc(sizeof(parameters));
	data2->row = 0;
	data2->col = 3;
	data2->boxno = 2;
	//Parameters Declaration3
	data3 = (parameters *) malloc(sizeof(parameters));
	data3->row = 0;
	data3->col = 6;
	data3->boxno = 3;
	//Parameters Declaration4
	data4 = (parameters *) malloc(sizeof(parameters));
	data4->row = 3;
	data4->col = 0;
	data4->boxno = 4;
	//Parameters Declaration5
	data5 = (parameters *) malloc(sizeof(parameters));
	data5->row = 3;
	data5->col = 3;
	data5->boxno = 5;
	//Parameters Declaration6im a mess i'm a loser ima hater i'm a user
	data6 = (parameters *) malloc(sizeof(parameters));
	data6->row = 3;
	data6->col = 6;
	data6->boxno = 6;
	//Parameters Declaration7
	data7 = (parameters *) malloc(sizeof(parameters));
	data7->row = 6;
	data7->col = 0;
	data7->boxno = 7;
	//Parameters Declaration8
	data8 = (parameters *) malloc(sizeof(parameters));
	data8->row = 6;
	data8->col = 3;
	data8->boxno = 8;
	//Parameters Declaration9
	data9 = (parameters *) malloc(sizeof(parameters));
	data9->row = 6;
	data9->col = 6;
	data9->boxno = 9;
	/*Threads for rows*/
	pthread_create(&rowcheckid[0], NULL, rowcheck, &indexes[0]);
	pthread_create(&rowcheckid[1], NULL, rowcheck, &indexes[1]);
	pthread_create(&rowcheckid[2], NULL, rowcheck, &indexes[2]);
	pthread_create(&rowcheckid[3], NULL, rowcheck, &indexes[3]);
	pthread_create(&rowcheckid[4], NULL, rowcheck, &indexes[4]);
	pthread_create(&rowcheckid[5], NULL, rowcheck, &indexes[5]);
	pthread_create(&rowcheckid[6], NULL, rowcheck, &indexes[6]);
	pthread_create(&rowcheckid[7], NULL, rowcheck, &indexes[7]);
	pthread_create(&rowcheckid[8], NULL, rowcheck, &indexes[8]);
	/*Threads for columns*/
	pthread_create(&colcheckid[0], NULL, colcheck, &indexes[0]);
	pthread_create(&colcheckid[1], NULL, colcheck, &indexes[1]);
	pthread_create(&colcheckid[2], NULL, colcheck, &indexes[2]);
	pthread_create(&colcheckid[3], NULL, colcheck, &indexes[3]);
	pthread_create(&colcheckid[4], NULL, colcheck, &indexes[4]);
	pthread_create(&colcheckid[5], NULL, colcheck, &indexes[5]);
	pthread_create(&colcheckid[6], NULL, colcheck, &indexes[6]);
	pthread_create(&colcheckid[7], NULL, colcheck, &indexes[7]);
	pthread_create(&colcheckid[8], NULL, colcheck, &indexes[8]);
	/*boxes treads*/
	pthread_create(&boxid[0], NULL, checkboxes, data1);
	pthread_create(&boxid[1], NULL, checkboxes, data2);
	pthread_create(&boxid[2], NULL, checkboxes, data3);
	pthread_create(&boxid[3], NULL, checkboxes, data4);
	pthread_create(&boxid[4], NULL, checkboxes, data5);
	pthread_create(&boxid[5], NULL, checkboxes, data6);
	pthread_create(&boxid[6], NULL, checkboxes, data7);
	pthread_create(&boxid[7], NULL, checkboxes, data8);
	pthread_create(&boxid[8], NULL, checkboxes, data9);
	totalthreads += 27;
	/*Main thread*/
	pthread_create(&mainn, NULL, mainfunc, NULL);
	/*Row joins*/
	pthread_join(rowcheckid[0], NULL);
	pthread_join(rowcheckid[1], NULL);
	pthread_join(rowcheckid[2], NULL);
	pthread_join(rowcheckid[3], NULL);
	pthread_join(rowcheckid[4], NULL);
	pthread_join(rowcheckid[5], NULL);
	pthread_join(rowcheckid[6], NULL);
	pthread_join(rowcheckid[7], NULL);
	pthread_join(rowcheckid[8], NULL);
	/*Columns joins*/
	pthread_join(colcheckid[0], NULL);
	pthread_join(colcheckid[1], NULL);
	pthread_join(colcheckid[2], NULL);
	pthread_join(colcheckid[3], NULL);
	pthread_join(colcheckid[4], NULL);
	pthread_join(colcheckid[5], NULL);
	pthread_join(colcheckid[6], NULL);
	pthread_join(colcheckid[7], NULL);
	pthread_join(colcheckid[8], NULL);
	/*boxes joins*/
	pthread_join(boxid[0], NULL);
	pthread_join(boxid[1], NULL);
	pthread_join(boxid[2], NULL);
	pthread_join(boxid[3], NULL);
	pthread_join(boxid[4], NULL);
	pthread_join(boxid[5], NULL);
	pthread_join(boxid[6], NULL);
	pthread_join(boxid[7], NULL);
	pthread_join(boxid[8], NULL);
	/*Main Thread join*/
	pthread_join(mainn, NULL);
	totalthreads += 1;

}
