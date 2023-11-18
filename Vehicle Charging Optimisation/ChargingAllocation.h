#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector>

using namespace std;

#include "Constant.h"
#include "Vehicle.h"

#ifndef CHARGINGALLOCATION_H_
#define CHARGINGALLOCATION_H_

class ChargingAllocation{
    private:
        vector<Vehicle> Vehicles;
        int queueLengths[NUM_CITIES];
        double avgWaitingTime[NUM_CITIES];
        double defaultOWT;

        //variable to store monte Carlo details
        int MCqueueLengths[NUM_CITIES];
        double MCavgWaitingTime[NUM_CITIES];
        double MC_OWT;

        //others
        int numberOfStops;
        
    public:
        ChargingAllocation();
        void assignDefaultChargingStations();
        void calculateQueueLengths();
        void calculateAverageWaitingTime();
        void displayAllVehicles();
        void displayAllVehiclesRechargeInfo();
        void displayAllCSdetails();
        double calculateOverallWaitingTime();
        void MCcalculateQueueLengths();
        void MCcalculateAverageWaitingTime();
        double MCcalculateOverallWaitingTime();
        void reduce();
        void MCdisplayAllVehiclesRechargeInfo();
        void MCdisplayAllCSdetails();
};

//Constructor
ChargingAllocation::ChargingAllocation(){
    numberOfStops = 0;
    ifstream fin;
    fin.open("ChargingDemands.txt");
    if(fin.fail()){
        cout<<"File couldn't be opened";
        exit(1);
    }
    string line;
    int i = 0;
    int vecId, destiId, capId, remainId;
    int currId = 0;
    char extra;
    while(getline(fin, line)) {
        istringstream iss(line);

        iss >> extra;      //Opening Bracket
        iss >> vecId;      //Vehicle Id
        iss >> extra;      //comma
        iss >> destiId;    //destination ID
        iss >> extra;      //comma
        iss >> capId;      //capacity value
        iss >> extra;      //comma
        iss >> remainId;   //remaining capacity
        iss >> extra;      //Closing Bracket

        Vehicles.push_back(Vehicle(vecId, currId, destiId, capId, remainId));
    }
    fin.close();
}


//assigning functions
void ChargingAllocation::assignDefaultChargingStations(){
    int chargepoint = 0;
    for(int i = 0; i < Vehicles.size(); i++){
        chargepoint = 0;
        while(Vehicles[i].getCurrentCity() < Vehicles[i].getDestinationCity() && chargepoint < Vehicles[i].getDestinationCity()){  
            chargepoint = Vehicles[i].furthestReach(chargepoint);
            if(chargepoint < Vehicles[i].getDestinationCity())
            {
                //Add charging station to vector in vehicle class
                Vehicles[i].addChargingStation(chargepoint);
                //fill battery
                Vehicles[i].rechargeBattery();
                numberOfStops++;
            } 
            else{
                break;
            }
        }
    }
}


//Calculation functions
void ChargingAllocation::calculateQueueLengths() {
    // Initialize all cities with 0 queue length.
    for (int i = 0; i < NUM_CITIES; i++) {
        queueLengths[i] = 0;
    }

    // Loop through each vehicle.
    for (int i = 0; i < Vehicles.size(); i++) {
        //For every vehicle, for every charging station
        //if charging station is 1, queue length at charging station 1 increases by 1
        for(int j = 0; j < Vehicles[i].getChargingStations().size(); j++){
            queueLengths[Vehicles[i].getChargingStations()[j]]++;
        }
    }
}
void ChargingAllocation::calculateAverageWaitingTime() {
    for (int i = 0; i < NUM_CITIES; i++) {
        avgWaitingTime[i] = 0.5 * (double(queueLengths[i]) / double(chargersMap[i]));
    }
}
double ChargingAllocation::calculateOverallWaitingTime(){
    double a = 0;
    for(int i = 0; i < NUM_CITIES; i++){
        a = a + (queueLengths[i]*avgWaitingTime[i]);
    }
    a = a/numberOfStops;
    defaultOWT = a;
    return a;
}


//display functions
void ChargingAllocation::displayAllVehicles() {
    cout << setw(5) << "ID" 
         << setw(20) << "Current Location" 
         << setw(20) << "Destination" 
         << setw(20) << "Battery Capacity"
         << setw(20) << "Remaining Range" << endl;
    cout << "--------------------------------------------------------------------------------------" << endl;

    for(int i = 0; i < Vehicles.size(); i++) {
        Vehicles[i].showDetails();
    }
}
void ChargingAllocation::displayAllVehiclesRechargeInfo() {
    cout << "Initial charging allocation ..." << endl;
    cout << setw(12) << "Vehicle Id" 
         << setw(20) << "Destination" 
         << setw(20) << "Capacity Range"
         << setw(20) << "First recharge"
         << setw(20) << "Second recharge"
         //Number of recharges need to be coded here according to number of max recharges
         << endl;
    cout << "----------------------------------------------------------------------------------------------" << endl;
    for(int i = 0; i < Vehicles.size(); i++) {
        Vehicles[i].showRechargeDetails();
    }
}
void ChargingAllocation::displayAllCSdetails() {
    cout << setw(12) << "Location Id" << setw(18) << "Location name" << setw(22) << "Distance to Sydney"
         << setw(18) << "no of Chargers" << setw(16) << "Queue length" << setw(20) << "Waiting hours" << endl;
    cout << "--------------------------------------------------------------------------------------" << endl;
    for (int i = 0; i < NUM_CITIES; i++) {
        cout << setw(12) << i 
             << setw(18) << nameMap[i] 
             << setw(22) << distanceMap[i] 
             << setw(18) << chargersMap[i]
             << setw(16) << queueLengths[i]
             << setw(18) << fixed << setprecision(2) << avgWaitingTime[i] << " hours" << endl;
    }
}

//Time Reduction
void ChargingAllocation::reduce(){
    int j = 0;
    for (int i = 0; i < NUM_CITIES; i++) {
        MCavgWaitingTime[i] = j;
    }
    // Initialize all cities with 0 queue length.
    for (int i = 0; i < NUM_CITIES; i++) {
        MCqueueLengths[i] = 0;
    }
            
    cout << endl << endl;
    for(int i = 0; i < Vehicles.size(); i++){
        Vehicles[i].reAllocate(MCavgWaitingTime);
        MCcalculateQueueLengths();
        MCcalculateAverageWaitingTime();
    }

}

void ChargingAllocation::MCcalculateQueueLengths() {
    for (int i = 0; i < NUM_CITIES; i++) {
        MCqueueLengths[i] = 0;
    }
    // Loop through each vehicle.
    for (int i = 0; i < Vehicles.size(); i++) {
        //For every vehicle, for every charging station
        //if charging station is 1, queue length at charging station 1 increases by 1
        for(int j = 0; j < Vehicles[i].getRandomChargingStations().size(); j++){
            MCqueueLengths[Vehicles[i].getRandomChargingStations()[j]]++;
        }
        
    }
}
void ChargingAllocation::MCcalculateAverageWaitingTime() {
    for (int i = 0; i < NUM_CITIES; i++) {
        MCavgWaitingTime[i] = 0.5 * (double(MCqueueLengths[i]) / double(chargersMap[i]));
    }
}
double ChargingAllocation::MCcalculateOverallWaitingTime(){
    double a = 0;
    for(int i = 0; i < NUM_CITIES; i++){
        a = a + (MCqueueLengths[i]*MCavgWaitingTime[i]);
    }
    a = a/numberOfStops;
    MC_OWT = a;
    return a;
}

//Algorithm display functions
void ChargingAllocation::MCdisplayAllVehiclesRechargeInfo() {
    cout << "Final charging allocation ..." << endl;
    cout << setw(12) << "Vehicle Id" 
         << setw(20) << "Destination" 
         << setw(20) << "Capacity Range"
         << setw(20) << "First recharge"
         << setw(20) << "Second recharge"
         //Number of recharges need to be coded here according to number of max recharges
         << endl;
    cout << "----------------------------------------------------------------------------------------------" << endl;
    for(int i = 0; i < Vehicles.size(); i++) {
        Vehicles[i].MCshowRechargeDetails();
    }
}
void ChargingAllocation::MCdisplayAllCSdetails() {
    cout << setw(12) << "Location Id" << setw(18) << "Location name" << setw(22) << "Distance to Sydney"
         << setw(18) << "no of Chargers" << setw(16) << "Queue length" << setw(20) << "Waiting hours" << endl;
    cout << "--------------------------------------------------------------------------------------" << endl;
    for (int i = 0; i < NUM_CITIES; i++) {
        cout << setw(12) << i 
             << setw(18) << nameMap[i] 
             << setw(22) << distanceMap[i] 
             << setw(18) << chargersMap[i]
             << setw(16) << MCqueueLengths[i]
             << setw(18) << fixed << setprecision(2) << MCavgWaitingTime[i] << " hours" << endl;
    }
}

#endif /* CHARGINGALLOCATION_H_*/