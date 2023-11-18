#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

#include "Constant.h"

#ifndef CHARGINGSTATION_H_
#define CHARGINGSTATION_H_

class ChargingStation{
    private:
        int cityId;
        string cityName;
        int distanceToLastCity;
        int numberOfChargers;
    public:
        ChargingStation(int i){
            cityId = i;
            cityName = nameMap[i];
            distanceToLastCity = distanceMap[i];
            numberOfChargers = chargersMap[i];
        }
        void showDetail();
        int distanceToSydney(int cityid);
};

void ChargingStation::showDetail() {
    cout << setw(12) << cityId;
    cout << setw(18) << cityName;
    cout << setw(22) << distanceToSydney(cityId);
    cout << setw(18) << numberOfChargers << endl;
}

int ChargingStation::distanceToSydney(int cityid) {
    int distance = 0;
    for(int i = 0; i <= cityid; i++){
        distance = distance + distanceMap[i];
    }
    return distance;
}

#endif /* CHARGINGSTATION_H_*/