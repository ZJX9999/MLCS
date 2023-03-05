#include <vector>
#include <queue>
#include "point.h"

class BucketQueue 
{
private:
    int cur;
    std::vector<std::queue<Point*>> buckets;
public:
    BucketQueue() : cur(-1) {};
    void push(Point* p);
    Point* top();
    bool pop();
    bool empty() {return cur == -1;};
};