#include "invMan.h"
#define adminPassword "9527"
using namespace std;
item * inventory = new item [1000];
int mainMenu(string , bool);
int searchItem();
bool checkDateStr(string dateStr);
int addItem(string );
int removeItem(string );
int editItem(string);
const string currentDateTime(){ //Custom made time-date generation function for logging purposes
    time_t now = time(0);
    struct tm  timeStruct;
    char buf[80];
    timeStruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &timeStruct);
    return buf;
}
char getch(){ //Keystroke Capture function
    char buf=0;
    struct termios old={0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
    return buf;
 }

ifstream userFile, invFile;
fstream logFile("LOG.TXT", fstream::in | fstream::out | fstream::app);
int main()
{
	//Check if these three files exists
	// 1. USERS.TXT Stores valid user names
	// 2. LOG.TXT Stores system access log
	// 3. ITEMS.TXT Stores current inventory
	int dummy;
	cout << "Starting system..." << endl;
	cout << "Checking user file USERS.TXT	[";
	userFile.open("USERS.TXT");
	invFile.open("ITEMS.TXT"); // Open all three file streams to check existence
    if(userFile.fail()){
		cout << "FAILED]" << endl;
		perror("USERS.TXT");
		cout << "Press <Enter> to exit" << endl;
		cin >> dummy;
		getchar();
		return 0;
	}
    else cout << "OK]" << endl;
    cout << "Checking user file LOG.TXT	[";
    if(logFile.fail()){
		cout << "FAILED]" << endl;
		perror("LOG.TXT");
		cout << "Press <Enter> to exit" << endl;
		cin >> dummy;
		getchar();
		return 0;
	}
    else cout << "OK]"<< endl;
    cout << "Checking user file ITEMS.TXT	[";
    if(invFile.fail()){
		cout << "FAILED]" << endl;
		perror("ITEMS.TXT");
		cout << "Press <Enter> to exit" << endl;
		cin >> dummy;
		getchar();
		return 0;
	}
    else cout << "OK]"<< endl;
    cout << "File existence check passed..." << endl;
    cout << "Retrieving User List from file" << endl;
    struct user users[1000]; //Maximum of 1000 users
    int userCount=0, adminCount = 0;
    while(!userFile.eof()&&userCount<1000&&++userCount){ //Retrieve user names and privilege by going through the files
        string temp, parameter; //Temporary variables
        getline(userFile,temp); //Get the whole line
        istringstream tempStream(temp);//Turn the line of string to stream to separate things
        getline(tempStream, users[userCount].name, ' '); //Separate according to space character
        getline(tempStream, parameter, ' ');//Again
        if(parameter == "A"){ // If the user has admin privilege, there will be an 'A' after the name
            adminCount++;
            users[userCount].admin = true; //Flag the user if the user is meant to have admin privileges
        }else if(parameter == ""){
            users[userCount].admin = false;
        }else {
            cout << "FATAL ERROR: User File corrupted, aborting initialization..." << endl;
            cout << "Press <Enter> to exit..."<<endl;
            getchar();
            return 0;
        }
    } //read users to user list
	cout << userCount << " User(s) retrieved" << endl; //User Count has no need to add 1
	cout << adminCount << " Admin(s) are found" << endl;
	cout << "Enter Username to Sign in:";
	string signInName;
	cin >> signInName;
	bool found = false;
	bool admin = false;
	for(int i=0;i<userCount;i++){ // Simple Linear Search to check the user list
		if(signInName==users[i].name){
			found=true;
			if(users[i].admin) admin=true;
			break;
		}
	}if(found == false){
		cout << "Invalid Username, this attempt will be reported" << endl; //Log the event if the user name is invalid
        logFile << "User named <" << signInName << "> unsuccessful login attempt at " << currentDateTime() << endl; //Report the incident
        logFile.close();
        userFile.close();
        cout << "Press <Enter> to exit" << endl;
		getchar();
		return 0;
	}
	//If login success
	cout << "Welcome <" << signInName << "> redirecting you to main menu..." << endl;
	logFile << "User named <" << signInName << "> successful login at " << currentDateTime() << endl; //Log the event
	int itemNum = readDataFromFile(inventory);
	for(int i=0;i<itemNum;i++){ //Check for expired merchandise
        if(inventory[i].shelfLife!=-1){
            time_t now;
            struct tm dateOfImport;
            char buf[80];
            istringstream dateStrStream(inventory[i].importDate);
            dateStrStream >> std::get_time(&dateOfImport, "%Y-%m-%d"); //Extract import date and convert to time structure
            const char *dateStrTemp = inventory[i].importDate.c_str();
            strptime(dateStrTemp, "%Y-%m-%d", &dateOfImport);
            now = time(0);
            double expireTime = (-1*difftime(mktime(localtime(&now)), mktime(&dateOfImport)))/3600/24 + (double)inventory[i].shelfLife; //Comparison
            if(expireTime<0) cout << inventory[i].name << " is expired for " << -1*expireTime << " day(s)" << endl;
        }
	}
	mainMenu(signInName, admin); //Pass the function
	return 0;
}
int mainMenu(string userName, bool admin){
    int dummy;
    if(admin==true){ //Check for admin
        dummy = getchar(); //Clear the input
        cout << "Admin privilege found" << endl;
        cout << "Please Enter password:";
        char passChar;
        string password="";
        while(passChar!=10&&(passChar = getch())) {
            if(passChar!=10) cout << "*";
            password+=passChar;
        }
        if(password.substr(0,password.length() - 1)==adminPassword){
            logFile << "User named <" << userName << "> successful ADMIN login attempt at " << currentDateTime() << endl; //Log failure
            cout << endl << "Password correct, access granted..." << endl;
        }else{
            cout << "Wrong password, access denied..."<< endl;
            logFile << "User named <" << userName << "> unsuccessful ADMIN login attempt at " << currentDateTime() << endl;//Log success
            logFile.close();
            cout << "Press <Enter> to exit..." << endl;
            getchar();
            return 0;
        }
    }
	string permission="",command="";
	if(admin==true) permission="ADELSQadelsq";//Privilege settings, limits the commands that the user can execute
	else permission="ELSQelsq";
	cout << "Inventory Management System v1 \"Storm Glass\" Main Menu" << endl; //Here comes the main dish
	cout << "Login time: " << currentDateTime() << endl;
	while(command!="q"&&command!="Q"){
        do{
            if(admin==true){
                    cout << endl << underline << "A" << underline_off << "dd Item" << endl;
                    cout << underline << "D" << underline_off << "elete Item" << endl;
            }
            cout << underline << "E" << underline_off << "dit Item" << endl;
            cout << underline << "L" << underline_off << "ist all Items" << endl;
            cout << underline << "S" << underline_off << "earch Item" << endl;
            cout << underline << "Q" << underline_off << "uit the Program" << endl;
            cout << "Please enter you choice (Type the underlined characters to select the function):";
            cin >> command;
        }while(permission.find(command)==std::string::npos||command.length()!=1);
        if(command=="a"||command=="A"){
                cout << "Launching Importation Subroutine..." << endl;
                int itemNo = addItem(userName);
                writeDataToFile(inventory, itemNo);
            }else if(command=="d"||command=="D"){
                cout << "Launching Discard Subroutine..." << endl;
                int itemNo = removeItem(userName);
                writeDataToFile(inventory, itemNo);
            }else if(command=="e"||command=="E"){
                cout << "Launching Edit Subroutine..." << endl;
                int itemNo = editItem(userName);
                writeDataToFile(inventory, itemNo);
            }else if(command=="s"||command=="S"){
                cout << "Launching Query Subroutine..." << endl;
                searchItem();
            }else if(command=="l"||command=="L"){
                cout << "Launching Listing Subroutine..." << endl;
                int itemNo = readDataFromFile(inventory);
                printAllItem(inventory, itemNo);
            }else if(command=="q"||command=="Q"){
                cout << "Signing out and quitting..." << endl;
                logFile << "User named <" << userName << "> signed out from the system at " << currentDateTime() << endl;
                logFile.close();
                cout << "Thank you for using the commodity management system" << endl;
                cout << "Press <Enter> to Exit..." <<endl;
                getchar();
            }else{
                cout << "Invalid Choice, Try again..." << endl;
            }
	}
	return 0;
}
int searchItem(){
	int itemNo = readDataFromFile(inventory);
    cout << "Total of <" << itemNo << "> items in inventory." << endl; //Print item count
	string str;
	cout << "Please enter your search item name or category: " << endl;
	cin.ignore();
	getline(cin,str);
	int count = 0;
	for(int i=0; i<itemNo; i++){
		if(inventory[i].name.find(str)!=std::string::npos || inventory[i].tag.find(str)!=std::string::npos){ //If keyword is present in name or category, display
			cout << "Name:\t\t" << inventory[i].name << endl;
			cout << "Category:\t" << inventory[i].tag << endl;
			cout << "Amount:\t\t" << inventory[i].amount << endl;
			if((inventory[i].shelfLife>=0)) cout << "Shelf life:\t" << inventory[i].shelfLife << " day(s)" <<endl;
			else cout << "Shelf life:\t" << "Not applicable" << endl;
			cout << "Import date:\t" << inventory[i].importDate << endl << endl;
			count++;
		}
	}
	if (count > 0){
		cout << count << " result(s) found." << endl;
	}
	else{
		cout << "Cannot find \"" << str << "\"" << endl;
	}
	cout << "Search again? (Type 'y' for yes, otherwise no) " << endl;
	char c;
	cin >> c;
	if (c == 'Y' || c == 'y'){
		searchItem();
	}
	else{
		return 0;
	}
	return count;
}

bool checkDateStr(string dateStr){//Check if date is valid
    istringstream dateStrStream(dateStr);
    struct tm d;
    dateStrStream >> std::get_time(&d, "%Y-%m-%d"); // Conversion to time structure
    const char *dateStrTemp = dateStr.c_str();
	return strptime(dateStrTemp, "%Y-%m-%d", &d);//Check for a successful conversion, invalid if cannot convert
}

int addItem(string userName){
	int i = readDataFromFile(inventory);
	char c = 'y';
	while (c == 'y'|c == 'Y'){
		string itemName;
		cout << "Name: ";
		cin.ignore();
		getline(cin,itemName);
		while(itemExist(inventory, itemName, i)){//Check for duplicate Items
			cout << "Item \"" << itemName << "\" already exists. You can" << endl;
			cout << underline << "E" << underline_off << "dit Item" << endl;
			cout << underline << "R" << underline_off << "e-enter Item Name" << endl;
			cout << "Please enter your choice: ";
			string c;
			cin >> c;

			if(c=="r"||c=="R"){
				cout << "Launching Importation Subroutine..." << endl;
				cout << "Name: ";
				cin.ignore();
				getline(cin,itemName);

			}
			else if(c=="e"||c=="E"){
				cout << "Launching Edit Subroutine..." << endl;
				editItem(userName);
			}else{
				cout << "Invalid choice, try again..." << endl;
			}
		}
		inventory[i].name = itemName; //Enter Item Properties
		cout << "Category: ";
		getline(cin,inventory[i].tag);
		cout << "Amount: ";
		cin >> inventory[i].amount;
		cout << "Shelf life (in days/ -1 if it won't expire): ";
		cin >> inventory[i].shelfLife;
		cout << "Import Date (yyyy-mm-dd): ";
		string dateStr="";
		cin.ignore();
		do{
            getline(cin,dateStr);
            if (checkDateStr(dateStr)){
                inventory[i].importDate = dateStr;
                break;
            }else{
                cout << "Invalid date, please reenter..." << endl;
                cout << "Import Date (yyyy-mm-dd): ";

            }
		}while(!checkDateStr(dateStr));
		writeDataToFile(inventory, i);
		logFile << "User <" << userName << "> added <" << itemName << "> to the inventory" << endl;
		cout << "Continue to add? (Type 'y' for yes, otherwise no)" << endl;
		cin >> c;
		i++;
	}

	return i;

}
int removeItem(string userName){
	int itemNo = readDataFromFile(inventory);
	string str;
	cout << "Please enter the name of the item to be deleted: " << endl;
	cin.ignore();
	getline(cin,str);
	char c;
	int delIndex = -1;
	for(int i=0; i<itemNo; i++){//Linear Search
		if(str == inventory[i].name){
			delIndex = i;
			cout << "Name:\t\t" << inventory[i].name << endl;
			cout << "Category:\t" << inventory[i].tag << endl;
			cout << "Amount:\t\t" << inventory[i].amount << endl;
			cout << "Shelf life (in days):\t" << inventory[i].shelfLife << endl;
			cout << "Import date:\t" << inventory[i].importDate << endl << endl;
		}
	}
	if (delIndex >= 0){
		cout << "Are you sure you want to permanently delete this item? ('y' if yes, otherwise no)" << endl;
		logFile << "User <" << userName << "> removed <" << str << "> from the inventory" << endl;
		cin >> c;
		if (c=='y'){
			for (int i=delIndex; i<itemNo; i++){
				inventory[i].name = inventory[i+1].name;
				inventory[i].tag = inventory[i+1].tag;
				inventory[i].amount = inventory[i+1].amount;
				inventory[i].shelfLife = inventory[i+1].shelfLife;
				inventory[i].importDate = inventory[i+1].importDate;
			}
			itemNo--;
			cout << "Item \"" << str << "\" deleted" << endl;
		}
	}
	else{
		cout << "Cannot find \"" << str << "\"" << endl;
		cout << "Search again? ('y' if yes, otherwise no)" << endl;
		cin >> c;
		if (c=='y')
			removeItem(userName);
	}

	cout << "Remaining number of Items: " << itemNo << endl;
	return itemNo;
}
int editItem(string userName){
	int itemNo = readDataFromFile(inventory);
	string str;
	cout << "Please enter the name of the item to be edited: " << endl;
	cin.ignore();
	getline(cin,str);

	int count = 0;
	for(int i=0; i<itemNo; i++){
		if(str == inventory[i].name){
			count++;
		}
	}
	if (count == 1){
		for (int i=0; i<itemNo; i++){
			if (str == inventory[i].name){
				cout << "Name:\t\t" << inventory[i].name << endl;
				cout << "Category:\t" << inventory[i].tag << endl;
				cout << "Amount:\t\t" << inventory[i].amount << endl;
				cout << "Shelf life (in days):\t" << inventory[i].shelfLife << endl;
				cout << "Import date:\t" << inventory[i].importDate << endl << endl;
				cout << "Please enter new information below:";
				cout << "Category: ";
				getline(cin,inventory[i].tag);
				cout << "Amount: ";
				cin >> inventory[i].amount;
				cout << "Shelf Life (in days):";
				cin >> inventory[i].shelfLife;
				logFile << "User <" << userName << "> changed details of <" << inventory[i].name << "> in the inventory" << endl;
				writeDataToFile(inventory, itemNo); //Writing record back to file
				break;
			}
		}
	}
	else{
		cout << "Cannot find \"" << str << "\"" << endl;
	}
	cout << "Enter again? (Type 'y' for yes, otherwise no) " << endl;
	char c;
	cin >> c;
	if (c == 'Y' || c == 'y'){
		editItem(userName);
	}
	else{
		return itemNo;
	}
	return itemNo;
}
