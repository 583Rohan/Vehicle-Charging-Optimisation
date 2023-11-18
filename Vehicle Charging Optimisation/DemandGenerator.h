#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <ctime>

using namespace std;

#include "Constant.h"

#ifndef DEMANDGENERATOR_H_
#define DEMANDGENERATOR_H_

class DemandGenerator{
    private:
        int vehicleId;
        int destinationId;
        int capacityRange;
        int remainRange;
        int noOfDemands;
    public:
        DemandGenerator();
};
DemandGenerator::DemandGenerator(){ 
    srand(time(0));
    noOfDemands = rand() % 51 + 150;
    ofstream fout;
    fout.open("ChargingDemands.txt");
    if(fout.fail()){
        cout<<"File couldn't be opened";
        exit(1);
    }
    for(int i = 0; i < noOfDemands; i++){
        vehicleId = i+500;
        destinationId = rand() % NUM_CITIES;
        capacityRange = rand() % (MAX_CAPACITY - MIN_CAPACITY + 1) + MIN_CAPACITY;
        remainRange = rand() % (capacityRange - MIN_REMAIN_RANGE + 1) + MIN_REMAIN_RANGE;
        fout << "[" << vehicleId << "," << destinationId <<"," << capacityRange << "," << remainRange << "]" << endl;
    }
    fout.close();
}
#endif /* DEMANDGENERATOR_H_ */