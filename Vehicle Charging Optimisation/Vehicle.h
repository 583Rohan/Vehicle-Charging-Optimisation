#include <iostream>
#include <string>
#include <iomanip>
#include <vector>

using namespace std;

#include "Constant.h"
#include "ChargingStation.h"

#ifndef VEHICLE_H_
#define VEHICLE_H_

class Vehicle{
    private:
        int vehicleId;
        int currentCityId;
        int destinationId;
        int capacityId;
        int remainRange;
        int oldBattery;
        vector<int> default_allocated_Charging_Stations;
        vector<int> random_allocated_Charging_Stations;
    public:
        Vehicle(int a, int b, int c, int d, int e){
            vehicleId = a;
            currentCityId = b;
            destinationId = c;
            capacityId = d;
            remainRange = e;
            oldBattery = e;
        }
        void showDetails();
        int furthestReach(int currId);
        int distanceBetweenCity(int bId, int dId);
        void addChargingStation(int stationId);
        int getCurrentCity();
        int getDestinationCity();
        void rechargeBattery();
        vector<int> getChargingStations();
        void showRechargeDetails();
        vector<int> getRandomChargingStations();
        int earliestStation(int destiId);
        void reAllocate(double arr[]);
        void MCshowRechargeDetails();

};

void Vehicle::showDetails(){
    cout << setw(5) << vehicleId 
         << setw(20) << nameMap[currentCityId] 
         << setw(20) << nameMap[destinationId]
         << setw(20) << capacityId 
         << setw(20) << remainRange << endl;
}
void Vehicle::showRechargeDetails(){
    // Print Vehicle Id, Destination Name, Capacity Range, and Remaining Range
    cout << setw(12) << vehicleId
         << setw(20) << nameMap[destinationId]
         << setw(20) << capacityId;

    // Print all recharging stations
    for(int i = 0; i < default_allocated_Charging_Stations.size(); i++) {
        cout << setw(20) << nameMap[default_allocated_Charging_Stations[i]];
    }

    // If there were less than N charging stations, print "----" for the remaining columns
    for(int i = default_allocated_Charging_Stations.size(); i < MAX_NUM_RECHARGES; i++) {
        cout << setw(20) << "----";
    }

    // New line for the next vehicle
    cout << endl;
}
int Vehicle::distanceBetweenCity(int bId, int dId){
    int distance = 0;
    for(int i = bId + 1; i <= dId; i++){
        distance = distance + distanceMap[i];
    }
    return distance;
}
int Vehicle::furthestReach(int currId){
    //copying the two variables
    int leftRange = remainRange;
    int startPointId = currId;
    
    while(leftRange >= distanceBetweenCity(startPointId, startPointId+1)){
        leftRange = leftRange - distanceBetweenCity(startPointId, startPointId+1);
        startPointId++;
    }
    return startPointId; //THis is the farthest city reached
}
void Vehicle::addChargingStation(int stationId){
    default_allocated_Charging_Stations.push_back(stationId);
}
int  Vehicle::getCurrentCity(){
    return currentCityId;
}
int Vehicle::getDestinationCity(){
    return destinationId;
}
void Vehicle::rechargeBattery(){
    remainRange = capacityId;
}
vector<int> Vehicle::getChargingStations() {
    return default_allocated_Charging_Stations;
}
vector<int> Vehicle::getRandomChargingStations() {
    return random_allocated_Charging_Stations;
}
int Vehicle::earliestStation(int destiId){
    int range = capacityId;  // vehicle's capacity
    
    // If the destination itself is within range, no need to recharge
    if (range >= distanceBetweenCity(currentCityId, destiId)) {
        return currentCityId;  // or return -1 if you don't want to recharge
    }

    // Move backwards from the destination city
    while (destiId > 0 && range >= distanceBetweenCity(destiId-1, destiId)) {
        range -= distanceBetweenCity(destiId-1, destiId);
        destiId--;
    }

    return destiId;  // This will now be the station where the vehicle should recharge
}
void Vehicle::reAllocate(double arr[]){//array of avg waiting times
    remainRange = oldBattery;
    if(default_allocated_Charging_Stations.empty()){
        return;
    }
    else if(default_allocated_Charging_Stations.size() == 1){
        int a = furthestReach(currentCityId);
        int b = earliestStation(destinationId);

        if(b < 0){
            b = 0;
        }
        int minWaitingTime = 1000;
        int selectedNumber = b;
        for (int i = a; i <= b; ++i) {
           if (arr[i] <= minWaitingTime) {
                minWaitingTime = arr[i];
                selectedNumber = i;
            }
        }
        random_allocated_Charging_Stations.push_back(selectedNumber);
        return;
    }
    else if(default_allocated_Charging_Stations.size() == 2){
        int tempDes = earliestStation(destinationId);
        if(tempDes < 0){
            tempDes = 0;
        }
        int a = earliestStation(tempDes);
        int b = furthestReach(0);       //is it possible that earliest is after furthest reach
                                        //What if they are equal
        int minWaitingTime = 1000;
        int selectedNumber = a;
        for (int i = a; i <= b; ++i) {
           if (arr[i] < minWaitingTime) {
                minWaitingTime = arr[i];
                selectedNumber = i;
            }
        }
        random_allocated_Charging_Stations.push_back(selectedNumber);
        tempDes = selectedNumber;
        remainRange = capacityId;
        int c = furthestReach(selectedNumber); //What if furthest reach is greater than destination
        minWaitingTime = 1000;

        //instead of temp des, start from previous charging station
        //selectedNumber = tempDes;
        for (int i = tempDes; i <= c; ++i) {
           if (arr[i] < minWaitingTime) {
                minWaitingTime = arr[i];
                selectedNumber = i;
            }
        }
        random_allocated_Charging_Stations.push_back(selectedNumber);
        return;
    }
}
void Vehicle::MCshowRechargeDetails(){
    // Print Vehicle Id, Destination Name, Capacity Range, and Remaining Range
    cout << setw(12) << vehicleId
         << setw(20) << nameMap[destinationId]
         << setw(20) << capacityId;

    // Print all recharging stations
    for(int i = 0; i < random_allocated_Charging_Stations.size(); i++) {
        cout << setw(20) << nameMap[random_allocated_Charging_Stations[i]];
    }

    // If there were less than N charging stations, print "----" for the remaining columns
    for(int i = random_allocated_Charging_Stations.size(); i < MAX_NUM_RECHARGES; i++) {
        cout << setw(20) << "----";
    }

    // New line for the next vehicle
    cout << endl;
}


#endif /* VEHICLE_H_ */