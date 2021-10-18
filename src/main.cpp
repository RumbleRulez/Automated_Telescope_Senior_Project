#include <iostream>
#include <math.h>
//#include <EasyDriver.h>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

vector<double> get_body(string body_name){
    vector<double> data;
    //file stream var
    ifstream file;
    //open file
    file.open(body_name + ".csv");
    //various hold vars
    string hold = "";
    string word;

    //while file is not empty
    while(!file.eof()){
        //clear the hold string
        hold.clear();

        //grab next line
        getline(file,hold);

        stringstream sent(hold);

        //while there is a delimiter then grab next one and shove it
        //into vector
        while(getline(sent, word,',')){
            data.push_back(stod(word));
        }
    }

    return data;
    //close file
    file.close();
}

void print_top_menu(){
    cout << "Welcome to the automated telescope tracking interface. \nPlease press the number corresponding to the desired action:" << endl;
    cout << "1. Track a Celestial body by name" << endl;
    cout << "2. Track telescope to coordinate" << endl;
    cout << "0. Quit" << endl;

    return;
}
void print_body_menu(){
    cout << "1.Earth's moon" << endl;

    return;
}


int main(int argc, char *argv[]){
   // NEED PINS FOR EASYDRIVER
   // EasyDriver::EasyDriver drive = new EasyDriver();
    
    cout << "System Initialized" << endl;
    print_body_menu();
    print_top_menu();

    return 0;
}