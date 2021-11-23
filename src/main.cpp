#include <iostream>
#include <math.h>
#include "EasyDriver.h"
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <chrono>
#include <thread>

using namespace std;

enum motor_choice{
    ALT,
    AZI
};

//Easydriver objects
EasyDriver ALT_drive(67,68,44,26,46,144,200);
EasyDriver AZI_drive(65,27,47,45,69,144,200);

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
        //Azimuth is 0 dimensions and Elevation is 1 dimension == azi, elev
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

// >80 degrees danger detection
bool is_danger(vector<vector<double>> future_pos, int time, bool &goingDown){
    cout << "Verifying danger zone" << endl;
    if(future_pos[time][1] > 80){
        goingDown = true;
        AZI_drive.rotate(180);
        cout << "is_danger future pos: " << future_pos[time][1] << endl;
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
vector<vector<double>> getIMU(){
    system("./start_IMU.sh");
    return get_body("IMU_Data");
}

//function to get change in current position to future position
vector<double> get_change_pos(vector<vector<double>> future, vector<vector<double>> current, int time_index, bool &goingDown){
    vector<double> delta;
    double hold1, hold2;
    
    cout << "Calculating change in position...in progress" << endl;
    
    if(future[time_index][1] > 80){
        cout << "Danger Zone Detected: Sleeping for " << time_index << "ms" << endl;
        delta.push_back(0.0);
        delta.push_back(0.0);
        goingDown = true;
        AZI_drive.rotate(180);
        return delta;
    }else{
        cout << "Calculated change in position" << endl;
        hold1 = future[time_index][0] - current[0][0];
        hold2 = future[time_index][1] - current[0][1];
        delta.push_back(hold1);
        delta.push_back(hold2);
        return delta;
    }
}

//change position function
void change_pos(int time, vector<vector<double>> future, vector<vector<double>> &current, bool &goingDown)
{   
    vector<double> hold;
    cout << "change pos called" << endl;
    hold = get_change_pos(future, current, time, goingDown);

    //return if the holds give no change (ie. danger zone detected)
    if(hold[0] == hold[1])
        return;
    
    //changing altitude
    cout << "Changing alt" << endl;
    ALT_drive.rotate(5.95*hold[1]);
    cout << "Hold vector: " << hold[1] << " " << hold[1] << endl;
    cout << "Rotating " << hold[1] << " degrees" <<endl;
    
    cout << "Changing azi" << endl;
    AZI_drive.rotate(17.06*hold[0]);
    // current[0][0] = current[0][0] + hold[0]; 
    // current[0][1] = current[0][1] + hold[1];
    cout << "Rotating " << hold[0] << " degrees" << endl;
    
    return;  

}

    /*   
        Pins for BBB. P8 is the header and the number is the pin # located on the beaglebone pinout guide

        Alt/Elevation driver:
            MS1 = P8_8 = GPIO 67
            MS2 = P8_10 = GPIO 68 
            STEP = P8_12 = GPIO 44
            SLP = P8_14 = GPIO 26
            DIR = P8_16 = GPIO 46

        Azi driver:
            MS1 = P8_18 = GPIO 65
            MS2 = P8_17 = GPIO 27
            STEP = P8_15 = GPIO 47
            SLP = P8_11 = GPIO 45
            DIR = P8_9 = GPIO 69

        IMU:
            SCL = P8_19
            SDA = P8_20
            RST =  P8_12

   */

int main(int argc, char *argv[]){
    //hold vars for menu
    int choice, time = 0;
    bool isOn = true, goingDown = false;
    double azi, elev;
    string body;
    
    //vectors for position trackings
    vector<vector<double>> future_pos;
    vector<vector<double>> current_pos;
    vector<double> delta_pos;
    vector<double> hold;
    vector<vector<double>> input_angle;
    
    //declare sys start
    cout << "System Initialized" << endl;
    
    //get IMU data (azi, y, elev)
    current_pos = getIMU();
    //get rid of y angle to get only (azi,elev)
    current_pos.erase(0);
    
    //azi + 90 degrees to calibrate to north
    current_pos[0][1] += 90;
        //if azi is greater than 365 then sub 365
        if(current_pos[0][1] > 365){
	        current_pos[0][1] -= 365;
        }
    cout << "IMU Initialized" << endl;

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
                //print menu and take choice
                print_body_menu();
                cin >> choice;
                //get future position from selected celestial body
                future_pos = select_body(choice);
                print_elev_azi_vector(future_pos);
                change_pos(time, input_angle, current_pos, goingDown);
                //sleep for a minute
                this_thread::sleep_for(chrono::minutes(1));
                //increase time to represent sleep
                time++;
                break;
            case 2:
                //take desired coords
                cout << "Please input new elevation in degrees:" << endl;
                cin >> elev;
                cout << "Please input new azimuth in degrees:" << endl;
                cin >> azi;
                //load into single vector
                hold.push_back(elev);
                hold.push_back(azi);
                //load into a double vector for ease of use
                input_angle.push_back(hold);
                cout << "input loaded" << endl;
                //change pos
                change_pos(time, input_angle, current_pos, goingDown);
                //update current position
                current_pos[time][0] += input_angle[0][0];
                current_pos[time][1] += input_angle[0][1];
                //print current position
                print_elev_azi_vector(current_pos);
                //clear input vector
                hold.clear();
                input_angle.clear();
                break;
            default:
                cout << "Invalid option, please input the number corresponding to the choice" << endl;
                break;
        }

    }
    return 0;
}
