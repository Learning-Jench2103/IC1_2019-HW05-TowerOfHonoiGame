#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <limits>

using namespace std;

void printScreen();
void catchData();
bool checkMovingLegal(int, int);
void moveDisk(int, int);
void showBestSteps_graph();
void showBestSteps_text(int, int, int, int);
void continuousComputeCurrentStep(int, int, int, int);

int diskNum = 0;
int states[9][3];
int towerInitial = 0;
int towerTarget = 0;
int counter = 0;

int main() {
	//initialize all states[][]
	for (int i = 0; i < 9; i++) {
		states[i][0] = 0;
		states[i][1] = 0;
		states[i][2] = 0;
	}
	
	//input the amount of plates
	while (diskNum <= 0||diskNum > 9) {
		cout << "Please input a number: ";
		cin >> diskNum;
		if (cin.fail()) {	//if the input is not a integer, clean the cin buffer
			cin.clear();
			cin.ignore(1000, '\n');
		}
		if (diskNum <= 0|| diskNum > 9) {
			cout << "Input Error!! 0~9 only!" << endl;
		}
	}

	//initialize all plates' position
	for (int i = 1; i <= diskNum; i++) {
		states[i][0] = diskNum + 1 - i;
	}

	//select 1 of 3 options
	int option = 0;
	while (option < 1||option > 3) {
		cout << "Choose a option(1:Do it myself  2:Show the steps by graphing  3:Show the steps by texts)¡G";
		cin >> option;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(1000, '\n');
		}
		if (option < 1 || option > 3) {
			cout << "Input Error! 1~3 only!" << endl;
		}
	}

	switch (option) {
	case 1://play by user
		while (1) {
			system("cls");	//clean the whole screen
			printScreen();
			if (states[diskNum][2] == 1) {
				cout << "You Are Win!!" << endl << endl;
				Sleep(1000);	//pause for 1000ms
				break;
			}
			catchData();
			towerInitial--;
			towerTarget--;
			if (!checkMovingLegal(towerInitial, towerTarget))
				continue;
			moveDisk(towerInitial, towerTarget);
		}
	case 2://show the answer by graphing
		showBestSteps_graph();
	case 3://show the answer by texts
		counter = 1;
		system("cls");	//clean the whole screen
		cout << "All Steps:" << endl << endl;
		showBestSteps_text(diskNum, 1, 2, 3);
		cout << endl << "WIN!!" << endl << endl;
	}
	system("pause");
	return 0;
}

void printScreen() {
	//top of the window
	cout << endl <<" ";
	for (int i = 0; i < (diskNum * 6 + 20); i++) {
		cout << "=";
	}
	cout << endl << " |" << setw((__int64)diskNum * 6 + 19) << "|"<< endl;

	//first column
	cout << " |" << setw((__int64)diskNum + 5) << "||" << setw((__int64)diskNum * 2 + 5) << "||" << setw((__int64)diskNum * 2 + 5) << "||" << setw((__int64)diskNum + 4) << "|" << endl;
	
	for (int i = diskNum; i > 0; i--) {	//i = column
		//the first tower
		cout << " |   ";
		for (int j = 0; j < (diskNum - states[i][0]); j++) {
			cout << " ";
		}
		for (int j = 0; j < states[i][0]; j++) {
			cout << "=";
		}
		cout << "||";
		for (int j = 0; j < states[i][0]; j++) {
			cout << "=";
		}
		for (int j = 0; j < (diskNum - states[i][0]); j++) {
			cout << " ";
		}
		cout << "   ";

		//the second tower
		for (int j = 0; j < (diskNum - states[i][1]); j++) {
			cout << " ";
		}
		for (int j = 0; j < states[i][1]; j++) {
			cout << "=";
		}
		cout << "||";
		for (int j = 0; j < states[i][1]; j++) {
			cout << "=";
		}
		for (int j = 0; j < (diskNum - states[i][1]); j++) {
			cout << " ";
		}
		cout << "   ";

		//the third tower
		for (int j = 0; j < (diskNum - states[i][2]); j++) {
			cout << " ";
		}
		for (int j = 0; j < states[i][2]; j++) {
			cout << "=";
		}
		cout << "||";
		for (int j = 0; j < states[i][2]; j++) {
			cout << "=";
		}
		cout << setw(((__int64)diskNum - states[i][2]) + 4) << "|" << endl;
	}

	//end of the column
	cout << " | ";
	for (int i = 0; i < ((__int64)diskNum * 6 + 16); i++) {
		cout << "-";
	}
	cout << " |" << endl;

	//print the number of towers
	cout << " |" << setw((__int64)diskNum + 5) << "01" << setw((__int64)diskNum * 2 + 5) << "02" << setw((__int64)diskNum * 2 + 5) << "03" << setw((__int64)diskNum + 4) << "|" << endl;
	
	//bottom of the window
	cout << " ";
	for (int i = 0; i < (diskNum * 6 + 20); i++) {
		cout << "=";
	}
	cout << endl << endl;
}

void catchData() {	//let user input which towers the disk be moved from and to.
	bool towerInitial_illegal = false;
	do {
		towerInitial_illegal = false;
		cout << "Choose which Tower that the disk you want to move is on: ";
		cin >> towerInitial;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(1000, '\n');
		}
		if (towerInitial > 3 || towerInitial < 1) {
			cout << "Input illegal!" << endl << endl;
			towerInitial_illegal = true;
		}
	} while (towerInitial_illegal);

	bool towerTarget_illegal = false;
	do {
		towerTarget_illegal = false;
		cout << "Choose the tower you want to move the disk to: ";
		cin >> towerTarget;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(1000, '\n');
		}
		if (towerTarget > 3 || towerTarget < 1) {
			cout << "Input illegal!" << endl << endl;
			towerTarget_illegal = true;
		}
	} while (towerTarget_illegal);
}

bool checkMovingLegal(int initial, int target) {	//check if the moving is valid
	int counter_initial = 1;
	int counter_target = 1;
	while (states[counter_initial][initial] != 0) {
		counter_initial++;
	}	//after this loop, "counter_initial" would be 1 more than where the uppermost disk is
	counter_initial--;
	
	while (states[counter_target][target] != 0) {
		counter_target++;
	}	//after this loop, "counter_target" would be 1 more than where the uppermost disk is
	counter_target--;
	
	//by the rule, the disk been moved must be smaller than the disks on the target tower
	if (states[counter_target][target] > states[counter_initial][initial]||states[counter_target][target] == 0) {
		if (states[counter_initial][initial] != 0) {	//the position been moved should has a disk
			return true;
		}
		else {
			cout << "Moving illegal" << endl << endl;
			system("pause");
			return false;
		}
	}
	else {
		cout << "Moving illegal" << endl << endl;
		system("pause");
		return false;
	}
}

void moveDisk(int initial, int target) {	//move disks by revising states[][]
	int counter_initial = 1;
	int counter_target = 1;
	//find the topest plates
	while (states[counter_initial][initial] != 0) {
		counter_initial++;
	}
	counter_initial--;
	//find the lowest position on the target tower that don't have a plate
	while (states[counter_target][target] != 0) {
		counter_target++;
	}
	states[counter_target][target] = states[counter_initial][initial];
	states[counter_initial][initial] = 0;
}

void showBestSteps_graph() {
	for (int i = 0; i < 9; i++) {
		states[i][0] = 0;
		states[i][1] = 0;
		states[i][2] = 0;
	}

	for (int i = 1; i <= diskNum; i++) {
		states[i][0] = diskNum + 1 - i;
	}

	system("cls");	//clean the whole screen
	cout << "Let us watch the best solution." << endl;
	cout << "Step " << counter << ": " << endl;
	counter++;
	printScreen();
	Sleep(1000);	//pause for 1000ms

	//print the steps
	continuousComputeCurrentStep(diskNum, 1, 2, 3);
	//printing ends
	cout << "WIN!!" << endl << endl;
	system("pause");
}

void continuousComputeCurrentStep(int disk, int ini, int buffer, int tar) {

	if (disk == 0) {
		return;
	}

	/*
	Once try to move a plate from A tower to B tower,
	players must move the plate on it from tower A to another tower C in advance.
	*/
	continuousComputeCurrentStep(disk-1, ini, tar, buffer);

	towerInitial = ini - 1;
	towerTarget = tar - 1;
	
	moveDisk(towerInitial, towerTarget);

	system("cls");	//clean the whole screen
	cout << "Let us watch the best solution." << endl;
	cout << "Step " << counter << ": " << "Plate " << disk << " move from tower " << ini << " to tower " << tar << "." << endl;
	counter++;	//count one more steps
	printScreen();

	system("pause");

	continuousComputeCurrentStep(disk-1, buffer, ini, tar);
}

void showBestSteps_text(int disk, int ini, int buffer, int tar) {

	if (disk == 0) {
		return;
	}

	/*
	Once try to move a plate from A tower to B tower,
	players must move the plate on it from tower A to another tower C in advance.
	*/
	showBestSteps_text(disk - 1, ini, tar, buffer);

	cout << "Step " << setw(3) << counter << ": " << "Plate " << disk << " move from tower " << ini << " to tower " << tar << "." << endl;
	counter++;	//count one more steps

	showBestSteps_text(disk - 1, buffer, ini, tar);
}