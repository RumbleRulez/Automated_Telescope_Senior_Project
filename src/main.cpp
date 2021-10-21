#include <iostream>
#include <math.h>
//#include <EasyDriver.h>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

vector<vector<double>> get_body(string body_name){
    vector<vector<double>> data;
    vector<double> alt_hold;
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
        //#####Azimuth is i dimensions and Elevation is j dimension
        while(getline(sent, word,',')){
            alt_hold.push_back(stod(word));
        }
        data.push_back(alt_hold);
        alt_hold.clear();
    }

    //close file
    file.close();

    return data;

}
//print vector
void print_elev_azi_vector(vector<vector<double>> v){    
    for(int i = 0; i < v.size(); i++){
        for(int j = 0; j < v[i].size(); j++){
            cout << v[i][j] << " ";
        }
        cout << endl;
    }
    return;
}   
//print top level menu
void print_top_menu(){
    cout << "Welcome to the automated telescope tracking interface. \nPlease press the number corresponding to the desired action:" << endl;
    cout << "1. Track a Celestial body by name" << endl;
    cout << "2. Track telescope to coordinate" << endl;
    cout << "0. Quit" << endl;

    return;
}
//print menu for celestial body
void print_body_menu(){
    cout << "1.Earth's moon" << endl;

    return;
}

vector<vector<double>> select_body(int in, vector<vector<double>> v){
    switch(in){
        case 0:
            return get_body("Moon");
            break;
        case 1:
            return get_body("Mars");
            break;
        default:
            cout << "Not a valid choice" << endl;
            break;
    }
}

// vector<vector<double>> getIMU(){

// }

// double get_change_pos(vector<double> future, vector<double> current, int time_index){
//     
// }

//void change_elev(vector<vector<double>> data, int time, ){
//
//}

//void change_azi(){

//}

//bool is_danger(){}

int main(int argc, char *argv[]){
   // NEED PINS FOR EASYDRIVER
   // EasyDriver::EasyDriver drive = new EasyDriver();

    //hold vars for menu
    int top_choice;
    bool isOn = true;
    double azi, elev;
    string body;

    vector<vector<double>> future_pos;
    vector<vector<double>> current_pos;

    //declare sys start
    cout << "System Initialized" << endl;
    //get IMU data
    //get_IMU();

    //loop to keep alive
    while(isOn == true){
    
        //print choice menu
        //and take in choice for top level
        print_top_menu();
        cin >> top_choice;

        switch(top_choice){
            case 0:
                cout << "Thank you for using this product!" << endl;
                isOn  = false;
                break;
            case 1:
                print_body_menu();
                
                future_pos = get_body("testData");
                print_elev_azi_vector(future_pos);
                break;
            case 2:
                cout << "Please input new elevation in degrees:" << endl;
                cin >> elev;
                cout << "Please input new azimuth in degrees:" << endl;
                cin >> azi;
                break;
            default:
                cout << "Invalid option, please input the number corresponding to the choice" << endl;
                break;
        }

    }

    return 0;
}