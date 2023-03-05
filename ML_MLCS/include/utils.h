#ifndef __LCSCAL_INCLUDE_UTILS_H__
#define __LCSCAL_INCLUDE_UTILS_H__

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <cmath>
#include <utility>

#include "point.h"

void build_alphabet_map(std::string& alphabets, std::map<char, int>& cmap);

using sts_type = std::vector<std::vector<std::vector<int>>>;
using st_type = std::vector<std::vector<int>>;
void cal_suc_tabs(std::vector<std::string>& seqs, 
                  std::map<char, int>& cmap, 
                  sts_type& SucTabs, bool disp);

using dpt_type = std::vector<std::vector<int>>;
using dpts_type = std::map<int, dpt_type>;
void cal_dp_tabs(std::vector<std::string>& seqs, 
                  std::map<char, int>& cmap,
                  dpts_type& DpTabs);

Point *successor(Point* p, sts_type SucTabs,  char alpha, int i, dpts_type& DpTabs);
CordType* GetSucCoords(CordType* PreCoords, int size, sts_type& SucTabs,  char alpha, int i);

using ct_type = std::vector< std::vector<int> >;
using cts_type = std::vector< std::vector< std::vector<int> > >;
void cal_count_tabs(std::vector<std::string>& seqs, 
	std::map<char, int>& cmap, cts_type& CountTabs);

int calc_ub_by_cts(const Point *p, cts_type& CountTabs);

#endif