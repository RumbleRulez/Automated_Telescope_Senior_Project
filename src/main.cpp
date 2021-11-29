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
        goingDown = !goingDown;
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
vector<double> get_change_pos(vector<vector<double>> future,vector<vector<double>> current, int time_index, bool &goingDown){
    vector<double> delta;
    double hold1, hold2;

    cout << "Calculating change in position...in progress" << endl;
    
    if(is_danger(future,time_index, goingDown)){
        cout << "Danger Zone Detected: Sleeping..." << endl;
        delta.push_back(0.0);
        delta.push_back(0.0);
        return delta;
    }else{
        if(goingDown){
            cout << "Calculated change in position" << endl;
            hold1 = future[time_index][0] - current[0][0];
            hold2 = -1*(future[time_index][1] - current[0][1]);
            delta.push_back(hold1);
            delta.push_back(hold2);
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
}

//change position function
void change_pos(int time, vector<vector<double>> future, vector<vector<double>> &current, bool &goingDown)
{   
    vector<vector<double>> temp_curr;
    vector<double> delta;
    cout << "change pos called" << endl;
    delta = get_change_pos(future, current, time, goingDown);
    cout << "delta vector: " << delta[0] << " " << delta[1] << endl;
    //return if the holds give no change (ie. danger zone detected)
    if(delta[0] == delta[1])
        return;
    

    
    //changing altitude
    cout << "Changing alt" << endl;
    ALT_drive.rotate(8.928571428*delta[1]);
    cout << "Rotating " << delta[1] << " degrees" <<endl;
    
    cout << "Changing azi" << endl;
    AZI_drive.rotate(17.06349206*delta[0]);
    cout << "Rotating " << delta[0] << " degrees" << endl;



    // current[0][0] = current[0][0] + hold[0]; 
    // current[0][1] = current[0][1] + hold[1];
    
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
    //cout << "System Initialized" << endl;
    
    //get IMU data
    //current_pos = getIMU();
    
    
    // cout << "IMU Initialized" << endl;
    // cout << "Current Pos: ";
    // print_elev_azi_vector(current_pos);
    // cout << "\n";

    //loop to keep alive
    while(isOn){
    

        AZI_drive.sleep();
        ALT_drive.sleep();

        //cout << "Current Pos: ";
        //print_elev_azi_vector(current_pos);
        
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
                for(int i = 0; i < 20; i++){
                    AZI_drive.wake();
                    ALT_drive.wake();
                    //print menu and take choice
                    print_body_menu();
                    cin >> choice;
                    //get future position from selected celestial body
                    future_pos = select_body(choice);
                    //print_elev_azi_vector(future_pos);
                    change_pos(time, input_angle, current_pos, goingDown);
                    AZI_drive.sleep();
                    ALT_drive.sleep();
                    //sleep for a minute
                    this_thread::sleep_for(chrono::minutes(1));
                    //increase time to represent sleep
                    time++;
                }
                break;
            case 2:
                AZI_drive.wake();
                ALT_drive.wake();
                //take desired coords
                cout << "Please input new azimuth in degrees:" << endl;
                cin >> azi;
                cout << "Please input new elevation in degrees:" << endl;
                cin >> elev;
                //load into single vector
                hold.push_back(azi);
                hold.push_back(elev);
                //load into a double vector for ease of use
                input_angle.push_back(hold);
                cout << "input loaded" << endl;
                //update current pos
                cout << "getting current position" << endl;
                current_pos = getIMU();
                //if azi is greater than 360 then sub 360 -- passes 0 point
                //azi + 90 degrees to calibrate to north
                // current_pos[0][0] += 90;
                // if(current_pos[0][0] > 360){
	            //     current_pos[0][0] -= 360;
                // }
                print_elev_azi_vector(current_pos);
                
                //change pos
                change_pos(0, input_angle,current_pos,goingDown);
                //print current position
                current_pos = getIMU();
                // current_pos[0][0] += 90;
                // if(current_pos[0][0] > 360){
	            //     current_pos[0][0] -= 360;
                // }
                print_elev_azi_vector(current_pos);
                //clear input vector
                current_pos.clear();
                hold.clear();
                input_angle.clear();
                AZI_drive.sleep();
                ALT_drive.sleep();
                break;
            default:
                cout << "Invalid option, please input the number corresponding to the choice" << endl;
                break;
        }

    }
    return 0;
}
