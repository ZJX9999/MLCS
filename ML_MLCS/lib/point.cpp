#include <iostream>
#include "point.h"
#include <cstring>

Point::Point(int id, int num, char alpha) : Index(id), sz(num), value(alpha), Depth(0) 
{
    coords = new CordType[num];
    memset(coords, 0, num * sizeof(int));
}
Point::Point(int id, int num, int ival, int depth, char alpha, 
            std::map<int, std::vector<std::vector<int>>>& DpTabs) 
            : Index(id), value(alpha), Depth(depth), IfLongest(false), sz(num)
{
    coords = new CordType[num];
    for(int i = 0; i < num; i++){
    	coords[i] = ival;
    }
    sz = num;
    //cal_MLS_by_dpts(DpTabs);
}

Point::Point(std::initializer_list<CordType> vals)
{
    if(vals.size() <= 0){
    	coords = nullptr;
        sz = 0;
    }
    else{
        CordType *p;
        coords = new CordType[vals.size()];
        p = coords;
        for(auto val : vals) *p++ = val;
        sz = vals.size();
    }
}

void Point::addInfo(int id, char alpha, int d, int pre)
{
    Index = id;
    value = alpha;
    Depth = d;
    PredcArray.push_back(pre);
}

void Point::cal_MLS_by_dpts(std::map<int, std::vector<std::vector<int>>>& DpTabs)
{
    MaxLeftStep = INT16_MAX;
    for(auto it = DpTabs.begin(); it != DpTabs.end(); ++it) {
        int i1 = it->first / sz;
        int i2 = it->first % sz;
        MaxLeftStep = std::min(MaxLeftStep, it->second[coords[i1]][coords[i2]]);
    }
}

void Point::print(std::string flag) const
{
    //std::cout << "Index: " << Index << std::endl;
    std::cout << flag << std::endl;
    std::cout << "value: " << value << std::endl;
    std::cout << "coords: ";
    for(int i = 0; i < sz; i++) {
        std::cout << coords[i] << ' ';
    }
    std::cout << std::endl;
    std::cout << "Depth: " << Depth << std::endl;
    std::cout << "MaxLeftStep: " << MaxLeftStep << std::endl;
    std::cout << "PredcArray: ";
    for(int i = 0; i < PredcArray.size(); i++) {
        std::cout << PredcArray[i] << ' ';
    }
    std::cout << std::endl << std::endl;
}

Point::~Point()
{
    if(coords){
    	delete[] coords;
    	coords = nullptr;
        sz = 0;
    }
}

