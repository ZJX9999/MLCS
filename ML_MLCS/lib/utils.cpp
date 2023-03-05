#include "utils.h"

void build_alphabet_map(std::string& alphabets, std::map<char, int>& cmap) {
    for(unsigned int i = 0; i < alphabets.length(); i++){
        cmap[alphabets[i]] =  i;
    }
}

void cal_suc_tabs(std::vector<std::string>& seqs, 
                  std::map<char, int>& cmap, 
                  sts_type& SucTabs, bool disp)
{
    for(auto& seq : seqs){
        int len = seq.length();
        st_type SucTab(cmap.size(), std::vector<int>(len + 1));
            // calculate successor table
        for (unsigned int i = 0; i < cmap.size(); i++) {
            SucTab[i][len] = -1;
        }

        for (int j = len - 1; j >= 0; j--) {
            for (unsigned int i = 0; i < cmap.size(); i++) {
                SucTab[i][j] = SucTab[i][j + 1];
            }
            SucTab[cmap[seq[j]]][j] = j + 1;
        }
        SucTabs.push_back(SucTab);

        if(disp){
            std::cout << "\n  |         ";
            for(int i = 0; i < len; i++) std::cout << seq[i] << "    ";
            std::cout << std::endl;
            for(int i = 0; i < len + 2; i++) std::cout << "----" << "    ";
            std::cout << std::endl;
            for (auto& m : cmap) {
                std::cout << m.first << " |     ";
                for (int j = 0; j <= len; j++) {
                    std::cout << SucTab[m.second][j] << "    ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
    }
}

/*Point *successor(Point* p, sts_type SucTabs, char alpha, int i, dpts_type& DpTabs) 
{
    Point* q = new Point(SucTabs.size(), alpha);

    for(unsigned int j = 0; j < SucTabs.size(); ++j) {
        (*q)[j] = SucTabs[j][i][p->at(j)];
        if(q->at(j) < 0) {
            delete q;
            return nullptr;
        }
    }
    q->cal_MLS_by_dpts(DpTabs);
    return q;
}*/

CordType* GetSucCoords(CordType* PreCoords, int size, sts_type& SucTabs,  char alpha, int i)
{
    CordType* coords = new CordType[size];
    for(unsigned int j = 0; j < SucTabs.size(); ++j) {
        coords[j] = SucTabs[j][i][PreCoords[j]];
        if(coords[j] < 0) {
            delete coords;
            return nullptr;
        }
    }
    return coords;
}

void cal_dp_tabs(std::vector<std::string>& seqs, 
                  std::map<char, int>& cmap,
                  dpts_type& DpTabs)
{
    for(unsigned int i1 = 0; i1 < seqs.size(); i1++) {
        for(unsigned int i2 = i1 + 1; i2 < seqs.size(); i2++) {
            dpt_type DpTab(seqs[i1].length() + 1, std::vector<int>(seqs[i2].length() + 1, 0));
            for (int x = seqs[i1].length() - 1; x >= 0; x--) {
                for (int y = seqs[i2].length() - 1; y >= 0; y--) {
                    if (seqs[i1][x] == seqs[i2][y])
                        DpTab[x][y] = DpTab[x + 1][y + 1] + 1;
                    else
                        DpTab[x][y] = std::max(DpTab[x + 1][y], DpTab[x][y + 1]);
                }
            }
            DpTabs[i1 * seqs.size() + i2] = DpTab;
        }
    }
}

void cal_count_tabs(std::vector<std::string>& seqs, 
	std::map<char, int>& cmap, cts_type& CountTabs)
{
    for(std::string seq : seqs) {
        int len = seq.size();
        ct_type CountTab(cmap.size(), std::vector<int>(len + 1));
        for (unsigned int i = 0; i < cmap.size(); i++) {
            CountTab[i][len] = 0;
        }

        for (int j = len - 1; j >= 0; j--) {
            for (unsigned int i = 0; i < cmap.size(); i++) {
                CountTab[i][j] = CountTab[i][j + 1];
            }
            CountTab[cmap[seq[j]]][j]++;
        }
        CountTabs.push_back(CountTab);
    }
}

int calc_ub_by_cts(const Point *p, cts_type& CountTabs) {
    int ub = 0;
    for(unsigned int i = 0; i < CountTabs[0].size(); i++){
        int minval = INT32_MAX;
        for(unsigned int j = 0; j < CountTabs.size(); j++){
            minval = std::min(CountTabs[j][i][p->at(j)], minval);
        }
        ub = std::max(ub, minval);
    }

    return ub;
}
