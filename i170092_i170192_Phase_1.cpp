#include<iostream>
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<unistd.h>
#include<fstream>
using namespace std;
ofstream out;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

int game[9][9] ={  
          {6, 6,0 , 2, 3, 9, 1, 8, 7},
            {5, 1, 9, 7, 2, 8, 6, 3, 4},
            {8, 3, 7, 6, 1, 4, 2, 9, 5},
            {1, 4, 3, 8, 6, 5, 7, 2, 9},
            {9, 5, 8, 2, 4, 7, 3, 6, 1},
            {7, 6, 2, 3, 9, 1, 4, 5, 8},
            {3, 7, 1, 9, 5, 6, 8, 4, 2},
            {4, 9, 6, 1, 8, 2, 5, 7, 3},
            {2, 8, 5, 4, 7, 3, 9, 1, 6}
        };

/* structure for passing data to threads */
typedef struct {
	int row; //To select from which row to start
	int col; //To select from which column to start
	int boxno;
} parameters;
//pthread ids
pthread_t rowcheckid[9];
pthread_t colcheckid[9];
pthread_t boxid[9];
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
		this->repeat=false;
		this->box=false;
		this->boxno=-1;
	}
	void insert(int v, int r, int c, pthread_t id, bool repeat = false,bool box=false,int boxno=-1) {
		this->index = v;
		this->row = r;
		this->column = c;
		this->id = id;
		this->repeat = repeat;
		this->box=box;
		this->boxno=boxno;
	}
	void clear() {
		this->index = -1;
		this->row = -1;
		this->column = -1;
		this->repeat = false;
		
	}
	void displayboxes(ofstream &out) {
		if (repeat == false) {
			out <<"The invalid box is:"<<boxno<< ".The invalid value of the box is " << index << " The row is "
					<< row + 1 << " And the column is " << column + 1
					<< " The thread id is:" << this->id << endl;
		}
		if (repeat == true) {
			out <<"The invalid box is:"<<boxno<< ".The value that repeated is:" << index << " at row:"
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
					<< row + 1 << " And the column is " << column+1
					<< " The thread id is:" << this->id << endl;
		}
		if (repeat == true) {
			out << "The value that repeated in column " << column +1 << " is:"
					<< index << ".And the row is:" << row +1 
					<< " The thread id is:" << this->id << endl;
		}
	}

};
/* Variable arrays declaration*/
vector<pairr> invalidrows;
vector<int> irows;
/*column arrays*/
vector<int> icols;
vector<pairr> invalidcolumns;
/*invalid boxes array*/
vector<int> invalidboxes;
vector<pairr> invalidrowsboxes;

pairr temp;
int indexes[9] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
void *rowcheck(void *arg) {
	pthread_mutex_lock(&mutex1);
	bool chck1 = false;
	bool chck2=false;
	pairr temp;
	int chck[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int a;
	a = *((int *) arg);
	//cout<<a<<endl;
	for (int i = 0; i < 9; i++) {

		if (game[a][i] <= 0||game[i][a] >9) {
			//chck1=false;
			if (chck2==false) {
				if(chck1==false)
				{
					irows.push_back(a);
				}
				temp.insert(game[a][i], a, i, rowcheckid[a]);
				invalidrows.push_back(temp);
				chck1 = false;
				chck2=true;
				temp.clear();

			}
		}
		if (chck[game[a][i]] == 0) {
			chck[game[a][i]] = game[a][i];
		} 
		else if (game[a][i] == chck[game[a][i]] && chck[game[a][i]] != 0) {
			//cout<<"There is reptition in row "<<a<<" of:"<<game[a][i]<<endl;
			if (chck1 == false) {
				irows.push_back(a);
				chck1 = true;
				temp.insert(game[a][i], a, i, rowcheckid[a], true);
				invalidrows.push_back(temp);
				//	repeteddigitrows.push_back(temp);
				temp.clear();

			} 
			else {
				temp.insert(game[a][i], a, i, rowcheckid[a], true);
				invalidrows.push_back(temp);
				//repeteddigitrows.push_back(temp);
				temp.clear();
			}
		}

	}
	pthread_mutex_unlock(&mutex1);
	pthread_exit(NULL);
}
void *colcheck(void *arg) {
	pthread_mutex_lock(&mutex1);
	bool chck1 = false;
	bool chck2=false;
	pairr temp;
	int chck[10] = {0,0,0,0,0,0,0,0,0,0};
	int a;
	a = *((int *) arg);
	//cout<<a<<endl;
	for (int i = 0; i < 9; i++) {
		if (game[i][a] == 0||game[i][a] >9||game[i][a] < 0) {
			//chck1=false;
			if (chck2==false) {
				if(chck1==false)
				{
					icols.push_back(a);
				}
				temp.insert(game[i][a], i, a, colcheckid[a]);
				invalidcolumns.push_back(temp);
				chck1 = false;
				chck2=true;
				temp.clear();

			}
		}
		if (chck[game[i][a]] == 0) {
			chck[game[i][a]] = game[i][a];
		}
		else if (game[i][a] == chck[game[i][a]] && chck[game[i][a]] !=0) {
			//cout<<"There is reptition in column "<<a<<" of:"<<game[i][a]<<endl;
			if (chck1 == false) {
				icols.push_back(a);
				chck1 = true;
				temp.insert(game[i][a], i, a, colcheckid[a], true);
				invalidcolumns.push_back(temp);
			//	repeatedcols.push_back(temp);
				temp.clear();

			}
			else{
				temp.insert(game[i][a], i, a, colcheckid[a], true);
			//	repeatedcols.push_back(temp);
				invalidcolumns.push_back(temp);
				temp.clear();
			}
		}

	}
	

	pthread_mutex_unlock(&mutex1);
	pthread_exit(NULL);
}
void *checkboxes(void *args) {
	pthread_mutex_lock(&mutex1);
	pairr temp;
	parameters *data=(parameters *)args;
	int row=data->row;
	int column =data->col;
	int boxnoo=data->boxno;
	//cout<<boxnoo<<endl;
	bool chck=false;
	bool chck2=false;
	int chck1[10]={0,0,0,0,0,0,0,0,0,0};
	for(int i=row;i<row+3;i++)
	{				
		for(int j=column;j<column+3;j++)
		{	
			if (game[i][j] == 0||game[i][j] >9||game[i][j] < 0) {
			//chck1=false;
				if (chck2==false) {
					if(chck==false)
					{
					invalidboxes.push_back(boxnoo);
					}
					temp.insert(game[i][j],row,column,boxid[(boxnoo)-1],false,true,boxnoo);
					invalidrowsboxes.push_back(temp);
					chck = false;
					chck2=true;
					temp.clear();
					}
			}		
			if(chck1[game[i][j]]==0)
			{
				chck1[game[i][j]]=game[i][j];
			}
			else if(chck1[game[i][j]]==game[i][j] && chck1[game[i][j]]!=0)
			{//cout<<"entered";

				if(chck==false)
				{
					//cout<<"Entered"<<endl;
					invalidboxes.push_back(boxnoo);
					temp.insert(game[i][j],row,column,boxid[(boxnoo)-1],true,true,boxnoo);
					invalidrowsboxes.push_back(temp);
					chck=true;
					temp.clear();
				}
				else
				{	
					//cout<<"Else entered"<<endl;
					temp.insert(game[i][j],row,column,boxid[(boxnoo)-1],true,true,boxnoo);
					invalidrowsboxes.push_back(temp);
					temp.clear();
				}
			}

		}
	
	}
	//cout<<"done"<<endl;
	pthread_mutex_unlock(&mutex1);
	pthread_exit(NULL);

	
	//cout<<data->row<<endl;

}
int main() {
	out.open("ibrar.txt");
	//Parameters Declaration1
	parameters*data1 = (parameters *) malloc(sizeof(parameters));
	data1->row = 0;
	data1->col = 0;
	data1->boxno = 1;
	//Parameters Declaration2
	parameters*data2 = (parameters *) malloc(sizeof(parameters));
	data2->row = 0;
	data2->col = 3;
	data2->boxno = 2;
	//Parameters Declaration3
	parameters*data3 = (parameters *) malloc(sizeof(parameters));
	data3->row = 0;
	data3->col = 6;
	data3->boxno = 3;
	//Parameters Declaration4
	parameters*data4 = (parameters *) malloc(sizeof(parameters));
	data4->row = 3;
	data4->col = 0;
	data4->boxno = 4;
	//Parameters Declaration5
	parameters*data5 = (parameters *) malloc(sizeof(parameters));
	data5->row = 3;
	data5->col = 3;
	data5->boxno = 5;
	//Parameters Declaration6im a mess i'm a loser ima hater i'm a user
	parameters*data6 = (parameters *) malloc(sizeof(parameters));
	data6->row = 3;
	data6->col = 6;
	data6->boxno = 6;
	//Parameters Declaration7
	parameters*data7 = (parameters *) malloc(sizeof(parameters));
	data7->row = 6;
	data7->col = 0;
	data7->boxno = 7;
	//Parameters Declaration8
	parameters*data8 = (parameters *) malloc(sizeof(parameters));
	data8->row = 6;
	data8->col = 3;
	data8->boxno = 8;
	//Parameters Declaration9
	parameters*data9 = (parameters *) malloc(sizeof(parameters));
	data9->row = 6;
	data9->col = 6;
	data9->boxno = 9;
	//pthread return status variable
	void * rows;//Not used in 1st phase
	void * col;//Not used in 2md phase
	//pthread creations
	cout << "Welcome To The World Of MultiThreading " << endl;
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
	 pthread_create(&boxid[0],NULL,checkboxes,data1);
	 pthread_create(&boxid[1],NULL,checkboxes,data2);
	 pthread_create(&boxid[2],NULL,checkboxes,data3);
	 pthread_create(&boxid[3],NULL,checkboxes,data4);
	 pthread_create(&boxid[4],NULL,checkboxes,data5);
	 pthread_create(&boxid[5],NULL,checkboxes,data6);
	 pthread_create(&boxid[6],NULL,checkboxes,data7);
	 pthread_create(&boxid[7],NULL,checkboxes,data8);
	 pthread_create(&boxid[8],NULL,checkboxes,data9);
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
	/*File writing*/
	out<<"Invalid Rows:"<<endl;
	for (int i = 0; i < irows.size(); i++) {
		out << "Invalid Rows is:" << irows[i] + 1 << endl;
	}
	for (int i = 0; i < invalidrows.size(); i++) {
		temp = invalidrows[i];
		temp.displayr(out);
		temp.clear();

	}
	out<<endl<<endl;
	out << "Invalid Columns:"<< endl;
	for (int i = 0; i < icols.size(); i++) {
		out << "Invalid columns is:" << icols[i] + 1 << endl;
	}
	for (int i = 0; i < invalidcolumns.size(); i++) {
		temp = invalidcolumns[i];
		temp.displayc(out);
		temp.clear();
	}

	out<<endl<<endl;
	out << "Invalid Boxes:"<< endl;
	 for(int i=0;i<invalidboxes.size();i++)
	 {
		 out<<"invalid box is:"<<invalidboxes[i]<<endl;
	 }
	 for(int i=0;i<invalidrowsboxes.size();i++)
	 {
		 temp=invalidrowsboxes[i];
		 temp.displayboxes(out);
	 }

}