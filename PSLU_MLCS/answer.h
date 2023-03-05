#ifndef __ANSWER_H__
#define __ANSWER_H__

#include <iostream>
#include <map>
#include <vector>
#include <string>

class Answer
{
public:
    int TolIndex;                   // 结点总数
	int TolEdgeNum;                 // 边总数
	int MaxPathLength;              // 最大深度
	int TolSearchCount;             // 总搜索次数
	int RelaxedCount;               // 松弛次数
	int RemoveCount;                // 删除点数目
	std::map<int, int> RelaxedTolCount;  // 松弛总计
	std::vector<std::string> MLCS;            // 答案
	std::vector<int> RemoveInfo;         // 删除点的记录信息
	int LongestPointNum;            // 最长路上的结点数目
	int FirstToMaxDepth;            // 第一次找到一条最长路创建了多少结点

    Answer();
    void Print();
};

#endif