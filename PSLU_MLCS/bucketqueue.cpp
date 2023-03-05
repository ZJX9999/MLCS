#include "bucketqueue.h"

void BucketQueue::push(Point* p) {
    int id = p->Depth + p->MaxLeftStep;
    while(buckets.size() <= id) {
        buckets.push_back(std::queue<Point*>());
    }
    buckets[id].push(p);
    cur = std::max(id, cur);
}

Point* BucketQueue::top() {
    if(cur < 0) return nullptr;
    return buckets[cur].front();
}
bool BucketQueue::pop() {
    if(cur < 0) return false;
    buckets[cur].pop();
    while(cur >= 0 && buckets[cur].empty()) cur--;
    return true;
}