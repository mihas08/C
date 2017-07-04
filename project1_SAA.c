#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <ctime>

using namespace std;


typedef struct elem {
int key;
elem *next;
} *pelem;
typedef struct stack {
pelem start;
} *pstack;

void push(pstack s, int n) {
	pelem p = s->start;
	s->start = (pelem)malloc(sizeof elem);
	s->start->key = n;
	s->start->next = p;
}

int pop(pstack s) {
	int n = -1;
	pelem p = s->start;
	if (p != NULL) {
		n = p->key;
		s->start = p->next;
		free(p);
	}
	return n;
}

int is_empty(pstack s) {
	return s->start == NULL;
}

pstack create_stack() {
	pstack t = (pstack)malloc(sizeof stack);
	memset(t, 0, sizeof stack);
	return t;
}

void selection_sort(pstack s) {
	pstack ret = create_stack();
	pstack temp = create_stack();
	while (!(s)) {
		int current = pop(s);
		while (!(s)) {
			int n = pop(s);
			if (n < current) {
				int swap = current;
				current = n;
				n = swap;
			}
			push(temp, n);
		}
		push(ret, current);
		s->start = temp->start;
		temp->start = NULL;
	}
	s->start = ret->start;
}

void file_output()
{
	int chislo;
	srand(time(NULL));
	ofstream file;
	file.open("file.txt");
	if (file.is_open())
	{
		for (int i = 0; i<50; i++)
		{
			chislo = rand() % 1000;
			file << chislo << endl;
		}
		file.close();
	}
	else cout << "File cant be open";
}

void file_input()
{
	int a;
	pstack s1 = create_stack();
	ifstream file_1;
	file_1.open("file.txt", ios::in);
	if (file_1)
	{
		while (!file_1.eof())
		{
			file_1 >> a;
			push(s1, a);
		}
		file_1.close();
		cout << endl << " File load." << endl << endl;
	}
	else
		cout << "File error!" << endl;
}


void display_file(int type){
	int value;
	string name;
	switch (type){
	case 1:
		name = "inputf.txt";
		break;
	case 2:
		name = "outputf.txt";
	}
	ifstream file;
	file.open(name.c_str(), ios::in);
	if (file){
		int i(1);
		cout << name << endl;
		cout << setw(8) << "Element";
		cout << setw(14) << "Stoinost";
		while (!file.eof()){
			file >> value;
			cout << endl << setw(5) << i;
			cout << setw(14) << value;
			i++;
		}
		file.close();
	}
	else cout << "File error!";

}

int main(){
	pstack s1 = create_stack();
	cout << "MENU" << endl;
	cout << "[1] - Vavedi danni ot file ( inputf.txt )." << endl;
	cout << "[2] - Sortirane na steka." << endl;
	cout << "[3] - Zapishi deka v file ( outputf.txt )." << endl;
	cout << "[4] - Izvejdane na sadyrjanieto na vhodqshtia file." << endl;
	cout << "[5] - Izvejdane na sadyrjanieto na izhodnia file." << endl;
	cout << "[0] - Izhod ot programata." << endl;
	char chc;
	while (true){
		cout << "Vavedi operacia -> ";
		cin >> chc;
		switch (chc)
		{
		case '0':
		{
					exit(0);
					break;
		}
		case '1':
		{
					file_input();
					break;
		}

                case '2':
		{
					 pstack s1 = create_stack();
					 selection_sort(s1);

					break;
		}
		case '3':
		{
					file_output();
					break;
		}
		case '4':
		{
					display_file(1);
					break;
		}
		case '5':
		{
					display_file(2);
					break;
		}

		cout << "Greshen izbor" << endl;
		}
		cout << endl;
	}
	return 1;
}
