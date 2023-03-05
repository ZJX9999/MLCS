#ifndef __HASH_H__
#define __HASH_H__

#include <math.h>

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <fstream>

#include "point.h"

class RSHash{

public:
	RSHash(unsigned int mod = 10000) : mod(mod) { }

	unsigned int operator() (const Point* point) const;

private:
	unsigned int mod;

};

struct PointKeyEqual{
    bool operator() (const Point* point1, const Point* point2) const;
};

class PointSet{

public:
	using ps_type = std::unordered_set<Point*, RSHash, PointKeyEqual>;

    PointSet(unsigned int mod = 10000, float load_factor = 10000);
    ~PointSet();

    PointSet(const PointSet&) = delete;
    PointSet(PointSet&&) = delete;
    PointSet& operator=(const PointSet&) = delete;
    PointSet& operator=(PointSet&&) = delete;

    bool insert(Point* point){ return points->insert(point).second; }
    Point* find(Point* point);
    unsigned int size() const { return points->size(); }
    void clear(bool del_point = false);
    ps_type* get_points() { return points; }
    std::vector<int> buckets_size(bool store = false, std::string path = "./data.txt");
    int save(std::string path = "./points.txt");

private:
	ps_type* points;

};

#define MapType int

class PointMap{

public:
	using ps_type = std::unordered_map<Point*, MapType, RSHash, PointKeyEqual>;
	using pair_type = std::pair<Point*, MapType>;

    PointMap(unsigned int mod = 10000, float load_factor = 10000);
    ~PointMap();

    PointMap(const PointMap&) = delete;
    PointMap(PointMap&&) = delete;
    PointMap& operator=(const PointMap&) = delete;
    PointMap& operator=(PointMap&&) = delete;

    bool insert(pair_type& data){ return points->insert(data).second; }
    bool insert(Point* p, MapType i){ return points->insert({p, i}).second; }
    bool find(Point* point){ return points->find(point) != points->end(); }
    MapType operator[] (Point* point) { return points->operator[](point); }
    unsigned int size() const { return points->size(); }
    ps_type* get_points() { return points; }
    std::vector<int> buckets_size(bool store = false, std::string path = "./data.txt");

private:
	ps_type* points;

};

#endif