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

    //close file
    file.close();
}





int main(int argc, char *argv[]){
   // NEED PINS FOR EASYDRIVER
   // EasyDriver::EasyDriver drive = new EasyDriver();
    



    return 0;
}