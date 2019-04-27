#ifndef INVMAN_H
#define INVMAN_H
#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <sstream>
#include <string>
#include <iomanip>
#include <fstream>
#include <cerrno>
#include <clocale>
#include <unistd.h>
#include <termios.h>
#define underline "\033[4m"
#define underline_off "\033[0m"
struct item{ 		//Defining Commodity (For each import)
    std::string name; 	//Item name
    std::string tag; 	//Item category
    int amount; 	//How many
    int shelfLife; 	//Shelf life (in Days)
    std::string importDate; 	//Date of import
};
struct user{
    std::string name;
    bool admin;
};
int readDataFromFile(item inventory[]);
int writeDataToFile(item inventory[], int itemNo);
bool itemExist(item inventory[], std::string s, int itemNo);
void printAllItem(item inventory[], int itemNo);
#endif
