#include <vector>
#include <queue>
#include <unordered_set>
#include <set>
#include <unordered_map>
#include <map>
#include <functional>
#include "point.h"
#include "hash.h"
#include "loader.h"
#include "utils.h"
#include "answer.h"
#include "string"
#include "bucketqueue.h"
#include <chrono>

using namespace std;

bool PointCmp(Point* p1, Point* p2) {
    return (p1->Depth + p1->MaxLeftStep) < (p2->Depth + p2->MaxLeftStep);
}

static int seqnum = 0;
static int alphanum = 0;
static int min_len_of_MLCS = 0;
static map<char, int> cmap;
static vector<vector<vector<int>>> SucTabs;
static map<int, vector<vector<int>>> DpTabs;
static vector<vector<vector<int>>> CountTabs;

static BucketQueue OpenTable;

static set<int> CloseTable;
static vector<Point*> PointsDom;            // 所有结点集合
static PointMap CreatedMap;
static Point* endPoint;
static Point* startPoint;
static Answer Ans;

void calAllTables(DataLoader *ds) {
    build_alphabet_map(ds->alphabets(), cmap);
    cal_suc_tabs(ds->data(), cmap, SucTabs, false);
    cal_dp_tabs(ds->data(), cmap, DpTabs);
    cal_count_tabs(ds->data(), cmap, CountTabs);
}

bool matchExplore(Point* p) {
    if(CloseTable.count(p->Index)) {
		return false;
	}
	CloseTable.insert(p->Index);
	return true;
}

void connectEndNode(Point* p) {
    if(p->Depth >= Ans.MaxPathLength) {
        Ans.TolEdgeNum++;
        endPoint->PredcArray.push_back(p->Index);

    }
}

void ConstructRelationGraph()
{
   startPoint = new Point(-3, seqnum, 0, 0, '#', DpTabs);
   startPoint->cal_MLS_by_dpts(DpTabs);
   OpenTable.push(startPoint);
   while(!OpenTable.empty()) {
       Point* p = OpenTable.top();
       OpenTable.pop();
       if(!matchExplore(p)) continue;
       bool hasSuc = false;
       for(auto& m : cmap) {
           CordType* SucCoords = GetSucCoords(p->coords, p->sz, SucTabs, m.first, m.second);
           if(SucCoords) {
                Point* suc = new Point(SucCoords, p->sz);
                suc->cal_MLS_by_dpts(DpTabs);
                if(p->Depth + suc->MaxLeftStep + 1 < Ans.MaxPathLength) continue;
                if(CreatedMap.find(suc)) {
                    int oldIndex = CreatedMap[suc];
                    Point* oldPoint = PointsDom[oldIndex];

                    if(p->Depth + 1 > oldPoint->Depth) {
                        int sz = oldPoint->PredcArray.size();
                        Ans.TolEdgeNum = Ans.TolEdgeNum + sz - 1;
                        Ans.RelaxedCount++;
                        oldPoint->PredcArray.clear();
                        oldPoint->PredcArray.push_back(p->Index);
                        oldPoint->Depth = p->Depth + 1;

                        OpenTable.push(oldPoint);
                        CloseTable.erase(oldIndex);
                    }
                    else if(p->Depth + 1 == oldPoint->Depth) {
                        Ans.TolEdgeNum++;
                        oldPoint->PredcArray.push_back(p->Index);
                    }
                    delete suc;
                }
                else {
                    suc->addInfo(Ans.TolIndex, m.first, p->Depth + 1, p->Index);
                    CreatedMap.insert(suc, suc->Index);
                    Ans.TolEdgeNum++;
                    PointsDom.push_back(suc);
                    OpenTable.push(suc);
                    Ans.TolIndex++;
                    Ans.MaxPathLength = max(Ans.MaxPathLength, suc->Depth + calc_ub_by_cts(suc, CountTabs));
                }
                hasSuc = true;
           }
       }
       if(hasSuc) {
            Ans.MaxPathLength = max(Ans.MaxPathLength, p->Depth + 1);
            continue;
       }
       connectEndNode(p);
   }
}

void FindLongestPath(Point* p, int step, string& mlcs) {
    if(p != endPoint) mlcs[step] = p->value;
    for(int pre : p->PredcArray) {
        if(pre == -3) {
            Ans.MLCS.push_back(mlcs);
        }
        else if(PointsDom[pre]->Depth == step) {
            FindLongestPath(PointsDom[pre], step-1, mlcs);
        }
    }
}

int main(int argc, char* argv[]) {
    DataLoader *ds = nullptr;
    if(argc == 2){
        ds = new DataLoader(argv[1]);
    }
    auto start_t = chrono::system_clock::now();
    seqnum = ds->data().size();
    endPoint = new Point(-2, seqnum, -1, 0, '#', DpTabs);
    alphanum = ds->alphabets().size();
    calAllTables(ds);
    ConstructRelationGraph();
    string mlcs(Ans.MaxPathLength, '#');
    FindLongestPath(endPoint, Ans.MaxPathLength, mlcs);
    auto end_t = chrono::system_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end_t - start_t);
    Ans.Print();
    cout << "time(sec) : " << double(duration.count()) * chrono::microseconds::period::num / chrono::microseconds::period::den << endl;
    return 0;
}
