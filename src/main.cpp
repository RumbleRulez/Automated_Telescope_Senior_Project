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

enum motor_choice{
    ALT,
    AZI
};

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
        //Azimuth is 1 dimensions and Elevation is 0 dimension == elev,azi
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
bool is_danger(vector<vector<double>> future_pos, int time){
    if(future_pos[time][0] > 80.0){
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
vector<double> getIMU(){
    double alt, azi;
    


}

//function to get change in current position to future position
vector<double> get_change_pos(vector<vector<double>> future, vector<double> current, int time_index){
    vector<double> delta;
    double hold1, hold2;
    if(!is_danger(future, time_index)){
        hold1 = future[time_index][0] - current[0];
        hold2 = future[time_index][1] - current[1];
        delta.push_back(hold1);
        delta.push_back(hold2);
        cout << delta.size() << endl;
        cout << hold1 << endl;
        cout << hold2 << endl;
        cout << "get change works" << endl;

        return delta;
    }else{
        cout << "Danger Zone Detected: Sleeping for " << time_index << "ms" << endl;
        delta.push_back(0.0);
        delta.push_back(0.0);
        return delta;
    }
}

void change_pos(EasyDriver &driver, int time,vector<vector<double>> future, vector<double> &current, motor_choice motor)
{   
    vector<double> hold;    
    hold = get_change_pos(future, current, time);

    if(hold[0] && hold[1])
        return;

    if(motor == 0){
        driver.rotate(hold[0]);
        current[0] += hold[0]; 
        current[1] += hold[1];
        cout << "Rotating " << hold[0] <<endl;

        return;  
    }else{
        current[0] += hold[0]; 
        current[1] += hold[1];
        driver.rotate(hold[1]);
        cout << "Rotating " << hold[0] <<endl;
        return;
    }
}

int main(int argc, char *argv[]){
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

   */
    EasyDriver ALT_drive(67,68,44,26,46,144,200);
    cout << "ALT Driver Initialized" << endl;
    EasyDriver AZI_drive(65,27,47,45,69,144,200);
    cout << "AZI Driver Initialized" << endl;

    //hold vars for menu
    int choice, time = 0;
    bool isOn = true;
    double azi, elev;
    string body;

    //vectors for position trackings
    vector<vector<double>> future_pos;
    vector<double> current_pos;
    vector<double> delta_pos;
    vector<double> hold;
    vector<vector<double>> input_angle;
    
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
                hold.push_back(elev);
                hold.push_back(azi);
                input_angle.push_back(hold);
                ALT_drive.rotate(30);
                AZI_drive.rotate(30);
                // change_pos(ALT_drive, time, input_angle, current_pos, ALT);
                // change_pos(AZI_drive, time, input_angle, current_pos, AZI);

                break;
            case 3:
                future_pos = get_body("testData");
                cout << "Data aquired" << endl;
                current_pos.push_back(0);
                current_pos.push_back(0);
                cout << "current pos initalized" << endl;
                delta_pos = get_change_pos(future_pos, current_pos, 0);
                print_elev_azi_vector(future_pos);
                cout << "delta calculated" << endl;
                cout << delta_pos[0] << " " << delta_pos[1] << endl;
                cout << current_pos[0] << current_pos[1] << endl;
                delta_pos = get_change_pos(future_pos, current_pos, 1);
                cout << "delta calculated" << endl;
                cout << delta_pos[0] << " " << delta_pos[1] << endl;
                cout << current_pos[0] << " " <<current_pos[1] << endl;
                cout << "Analog pin 0 value:" << readADC(0) << endl;
                break;
            default:
                cout << "Invalid option, please input the number corresponding to the choice" << endl;
                break;
        }

    }
    return 0;
}