#ifndef __POINT_H__
#define __POINT_H__

#include <vector>
#include <map>
#include <initializer_list>
#include <string>

#define CordType int

class Point
{
public:
    int Index;
    char value;
    CordType* coords;
    int Depth;
    int MaxLeftStep;
    std::vector<int> PredcArray;
    bool IfLongest;
    int sz;

Point(CordType* c, int size): coords(c), sz(size){}
void addInfo(int id, char alpha, int d, int pre);

Point(int id, int num, char alpha);
Point(int id, int num, int ival, int depth, char alpha, std::map<int, std::vector<std::vector<int>>>& DpTabs);

Point(std::initializer_list<CordType> vals);
int getDepth() const { return Depth; }
int getMaxLeftStep() const { return MaxLeftStep; }
int size() const { return sz; }
CordType& operator[](int dim){ return coords[dim]; }
CordType at(int dim) const { return coords[dim]; };
void cal_MLS_by_dpts(std::map<int, std::vector<std::vector<int>>>& DpTabs);

void setDepth(int d) { Depth = d; };


void print(std::string flag) const;
~Point();
};

#endif
