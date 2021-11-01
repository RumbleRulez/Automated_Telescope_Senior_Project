#include <iostream>
#include <math.h>

//keep this uncommented when not compiling on BBB
//calls unix and microcontroller libraries that windows hates
#include "EasyDriver.h"

#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

enum{
    alt,
    azi
}motor_choice;

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
        //Azimuth is j dimensions and Elevation is i dimension
        while(getline(sent, word,',')){
            alt_hold.push_back(stod(word));
        }
        //push new vector in and then clear the hold vector
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

bool is_danger(vector<vector<double>> future_pos, int time){
    if(future_pos[0][time] > 80.0){
        return true;
    }else{
        return false;
    }
}

//print menu for celestial body
void print_body_menu(){
    cout << "0.Earth's moon" << endl;
    cout << "1.Mars" << endl;
    return;
}

//body selection from submenu
vector<vector<double>> select_body(int in){
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

//function to get IMU initial data
// vector<double> getIMU(){

// }

//function to get change in current position to future position
vector<double> get_change_pos(vector<vector<double>> future, vector<double> current, int time_index){
    vector<double> delta;
    if(!is_danger(future, time_index)){
        delta.push_back(future[time_index][0] - current[0]);
        delta.push_back(future[time_index][1] - current[1]);

        return delta;
    }else{
        cout << "Danger Zone Detected: Sleeping for " << time_index << "ms" << endl;
        delta.push_back(0);
        delta.push_back(0);
        return delta;
    }
}

void change_pos(EasyDriver &driver, int time,vector<vector<double>> future, vector<double> current)
{   
    
    driver.rotate(future[0][time]);
    cout << "Rotating " << future[0][time] <<endl;

    return;    
}

//void track(){}

int main(int argc, char *argv[]){
    /*   
        Pins for BBB. P8 is the header and the number is the pin # located on the beaglebone pinout guide

        Alt/Elevation driver:
            MS1 = P8_8
            MS2 = P8_10
            STEP = P8_12
            SLP = P8_14
            DIR = P8_16

        Azi driver:
            MS1 = P8_18
            MS2 = P8_17
            STEP = P8_15
            SLP = P8_11
            DIR = P8_9

   */
    EasyDriver ALT_drive(8,10,12,14,16,1,360);
    cout << "ALT Driver Initlized" << endl;
    EasyDriver AZI_drive(18,17,15,11,9,1,360);
    cout << "AZI Driver Initlized" << endl;

    //hold vars for menu
    int choice;
    bool isOn = true;
    double azi, elev;
    string body;

    vector<vector<double>> future_pos;
    vector<double> current_pos;
    vector<double> delta_pos;
    vector<double> input_angle;
    //declare sys start
    cout << "System Initialized" << endl;
    //get IMU data
    //get_IMU();

    //loop to keep alive
    while(isOn == true){
    
        //print choice menu
        //and take in choice for top level
        print_top_menu();
        cin >> choice;

        switch(choice){
            case 0:
                cout << "Thank you for using this product!" << endl;
                isOn  = false;
                break;
            case 1:
                print_body_menu();
                cin >> choice;
                future_pos = select_body(choice);
                print_elev_azi_vector(future_pos);
                break;
            case 2:
                cout << "Please input new elevation in degrees:" << endl;
                cin >> elev;
                cout << "Please input new azimuth in degrees:" << endl;
                cin >> azi;
                current_pos.push_back(0);
                current_pos.push_back(0);
                input_angle.push_back(elev);
                input_angle.push_back(azi);
                delta_pos = get_change_pos(future_pos,current_pos, 0);
                cout << delta_pos[0] << delta_pos[1];
                break;
            case 3:
                future_pos = get_body("testData");
                cout << "Data aquired" << endl;
                current_pos.push_back(1000);
                current_pos.push_back(2000);
                cout << "current pos initalized" << endl;
                delta_pos = get_change_pos(future_pos, current_pos, 0);
                print_elev_azi_vector(future_pos);
                cout << "delta calculated" << endl;
                cout << delta_pos[0] << " " << delta_pos[1] << endl;
                break;
            default:
                cout << "Invalid option, please input the number corresponding to the choice" << endl;
                break;
        }

    }
    return 0;
}