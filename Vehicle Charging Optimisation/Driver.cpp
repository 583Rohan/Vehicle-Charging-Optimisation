#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

#include "Constant.h"
#include "ChargingStation.h"
#include "Vehicle.h"
#include "DemandGenerator.h"
#include "ChargingAllocation.h"

void displayAllChargingStations();
int main(){
    //Creating demand list and filling the file using constructor  (Constructor doing this)
    DemandGenerator a;
    //Reading demand list and making the vector of vehicle objects (Constructor doing this)
    ChargingAllocation b;

    //Printing all vehicles read from txt file
    b.displayAllVehicles();
    cout << endl << endl;

    //Printing all Charging station details
    displayAllChargingStations();
    cout << endl << endl;

    //Using function to allocate vehicles with charging stations
    b.assignDefaultChargingStations();

    //Display all vehicle information along with charging Stations
    b.displayAllVehiclesRechargeInfo();
    cout << endl << endl;

    //Using functions to calculate Queue lengths and Avg waiting time
    b.calculateQueueLengths();
    b.calculateAverageWaitingTime();

    //Display all charging station information along with queue lngth and avg waiting time
    b.displayAllCSdetails();
    cout << endl << endl;

    //Printing Overall waiting time
    cout << "Overall Waiting time is: " << b.calculateOverallWaitingTime() << " hours";
    cout << endl << endl;


    cout << "algo result: ";
    //Apply My program
    b.reduce();
    
    //Displaying updated charging info
    b.MCdisplayAllVehiclesRechargeInfo();
    cout << endl << endl;   

    //displaying updated charging station info 
    b.MCdisplayAllCSdetails();
    cout << endl << endl;

    //displaying final waiting time
    cout << "Improved overall Waiting time is: " << b.MCcalculateOverallWaitingTime() << " hours";
    cout << endl << endl;
    return 0;

}

//This function declares the objects of charging stations and displays the information
void displayAllChargingStations() {
    cout << "Charging station information:" << endl;
    cout << setw(12) << "Location Id" << setw(18) << "Location name" 
         << setw(22) << "Distance to Sydney" << setw(18) << "no of Chargers" << endl;
    cout << "------------------------------------------------------------------------" << endl;
    for (int i = 0; i < NUM_CITIES; i++) {
        ChargingStation station(i);
        station.showDetail();
    }
}


