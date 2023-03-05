#include "hash.h"

unsigned int RSHash::operator() (const Point* point) const{
    unsigned int b = 378551 ;
    unsigned int a = 63689 ;
    unsigned int hash = 0 ;

    for(int i = 0; i < point->size(); i++){
        hash = hash * a + point->at(i);
        a *= b;
    }

    return hash % mod;
}

bool PointKeyEqual::operator() (const Point* point1, const Point* point2) const{
	if(point1->size() != point2->size()){
		return false;
	}
	else{
        for(int i = 0; i < point1->size(); i++){
            if(point1->at(i) != point2->at(i)) return false;
        }
        return true;
    }
}

PointSet::PointSet(unsigned int mod, float load_factor){
	points = new ps_type(mod, RSHash(mod), PointKeyEqual());
	points->max_load_factor(load_factor); // Make sure rehash doesn't occur.
}


Point* PointSet::find(Point* point) {
    auto iter = points->find(point);
    if(iter != points->end()){
        return *iter;
    } 
    else{
        return nullptr;
    }
}

void PointSet::clear(bool del_point){
    if(del_point){
        for(auto p : *points){
            delete p;
        }
    }
    points->clear();
}

std::vector<int> PointSet::buckets_size(bool store, std::string path){
    std::vector<int> ret(points->bucket_count());

    for(unsigned int i = 0; i < points->bucket_count(); i++){
    	ret[i] = points->bucket_size(i);
    }

    if(store){
    	std::ofstream ofs(path);
        for(auto val : ret){
        	ofs << val << std::endl;
        }
    	ofs.close();
    }

    return ret;
}

int PointSet::save(std::string path){
    std::ofstream ofs(path);
    
    for(auto p : *points){
        for(int i = 0; i < p->size(); i++){
            ofs << p->at(i);
            if(i != p->size() - 1) ofs << ',';
            else ofs << std::endl;
        }
    }

    ofs.close();

    return points->size();
}

PointSet::~PointSet(){
    for(auto p : *points){
        delete p;
    }
    delete points;
}

PointMap::PointMap(unsigned int mod, float load_factor){
	points = new ps_type(mod, RSHash(mod), PointKeyEqual());
	points->max_load_factor(load_factor); // Make sure rehash doesn't occur.
}

std::vector<int> PointMap::buckets_size(bool store, std::string path){
    std::vector<int> ret(points->bucket_count());

    for(unsigned int i = 0; i < points->bucket_count(); i++){
    	ret[i] = points->bucket_size(i);
    }

    if(store){
    	std::ofstream ofs(path);
        for(auto val : ret){
        	ofs << val << std::endl;
        }
    	ofs.close();
    }

    return ret;
}

PointMap::~PointMap(){
    for(auto& p : *points){
        delete p.first;
    }
    delete points;
}
