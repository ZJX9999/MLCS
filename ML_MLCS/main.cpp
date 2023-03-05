#include <vector>
#include <queue>
#include <unordered_set>
#include <set>
#include <unordered_map>
#include <map>
#include <functional>
#include <torch/script.h>
#include "point.h"
#include "hash.h"
#include "loader.h"
#include "utils.h"
#include "answer.h"
#include "string"
#include "bucketqueue.h"
#include <chrono>

using namespace std;


class PointCmp {
public:
	bool operator()(const Point* p1,const Point* p2)const {
		return p1->score < p2->score;
	}
};

static int seqnum = 0;
static int alphanum = 0;
static int min_len_of_MLCS = 0;
static map<char, int> cmap;
static vector<vector<vector<int>>> SucTabs;
static map<int, vector<vector<int>>> DpTabs;
static vector<vector<vector<int>>> CountTabs;

//static BucketQueue OpenTable;
static priority_queue<Point*, std::vector<Point*>, PointCmp > OpenTable;

static set<int> CloseTable;
static vector<Point*> PointsDom;            // 所有结点集合
static PointMap CreatedMap;
static Point* endPoint;
static Point* startPoint;
static Answer Ans;

static torch::jit::script::Module module;

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
        
        auto in = at::zeros({4, seqnum * 2});
        CordType* sucs[4];
        for(auto& m : cmap) {
            sucs[m.second] = GetSucCoords(p->coords, p->sz, SucTabs, m.first, m.second);
            if(sucs[m.second]) {
                int min_val = INT_MAX;
                for(int i = 0; i < seqnum; ++i) {
                    min_val = min(min_val, p->coords[i]);
                    in[m.second][i] = p->coords[i];
                    in[m.second][i + seqnum] = sucs[m.second][i];
                }
                for(int i = 0; i < seqnum; ++i) {
                    in[m.second][i] -= min_val;
                    in[m.second][i + seqnum] -= min_val;
                }
            }
        }
        vector<torch::jit::IValue> input;
        input.push_back((torch::jit::IValue)in);
        at::Tensor output = module.forward(input).toTensor();

       for(auto& m : cmap) {
           //CordType* SucCoords = GetSucCoords(p->coords, p->sz, SucTabs, m.first, m.second);
           if(sucs[m.second]) {
                Point* suc = new Point(sucs[m.second], p->sz);
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
                        oldPoint->score = output[m.second][0].item<float>();

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
                    suc->score = (float)output[m.second][0].item<float>();
                    CreatedMap.insert(suc, suc->Index);
                    Ans.TolEdgeNum++;
                    PointsDom.push_back(suc);
                    
                    OpenTable.push(suc);
                    Ans.TolIndex++;
                    Ans.MaxPathLength = max(Ans.MaxPathLength, suc->Depth + calc_ub_by_cts(suc, CountTabs));
                }
                hasSuc = true;
                sucs[m.second] = NULL;
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
    try {
        module = torch::jit::load("../mlcs.pt");
    }
    catch (const c10::Error& e) {
        std::cerr << "error loading the model\n";
        return -1;
    }
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
