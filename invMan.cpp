#include "invMan.h"
using namespace std;
int readDataFromFile(item inventory[]){//Purpose is to locate the index of the last entry
	ifstream inv;
	inv.open("ITEMS.TXT");
	int i=0;
	string line, amountStr, shelfLifeStr;
	while(getline(inv,line)){
		istringstream iss(line);
		if (!getline(iss,inventory[i].name,','))
			break;
		if (!getline(iss,inventory[i].tag,','))
			break;
		if (!getline(iss,amountStr,','))
			break;
		inventory[i].amount = atoi(amountStr.c_str());
		if (!getline(iss, shelfLifeStr,','))
			break;
        inventory[i].shelfLife = atoi(shelfLifeStr.c_str());
		if (!getline(iss,inventory[i].importDate))
			break;
		++i;
	}
	inv.close();
    	return i;
}
int writeDataToFile(item inventory[], int itemNo){
	ofstream fout;//Open file Stream
	fout.open("ITEMS.TXT");
	for(int i=0; i<itemNo; i++){
		fout << inventory[i].name << "," // Write all the properties
		     << inventory[i].tag << ","
		     << inventory[i].amount << ","
		     << inventory[i].shelfLife << ","
		     << inventory[i].importDate << endl;
	}
	fout.close();
}
bool itemExist(item inventory[], string s, int length){
	for (int i=0; i<length; i++){
		if (s == inventory[i].name){ //Linear Search
			return true;
		}
	}
	return false;
}
void printAllItem(item inventory[], int itemNo){ //Print all items
	for (int i=0; i<itemNo; i++){//Use a loop to print all of the inventory
		cout << "Name:\t\t" << inventory[i].name << endl;
		cout << "Category:\t" << inventory[i].tag << endl;
		cout << "Amount:\t\t" << inventory[i].amount << endl;
		if((inventory[i].shelfLife>=0)) cout << "Shelf life:\t" << inventory[i].shelfLife<< endl;
		else cout << "Shelf life:\t" << "Not applicable" << endl;
		cout << "Import date:\t" << inventory[i].importDate << endl << endl;
	}
	cout << "Total number of Items: " << itemNo << endl;
}
