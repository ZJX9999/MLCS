#include "answer.h"

// Initialize for sth.
Answer::Answer() {
	TolIndex = 0;
	TolEdgeNum = 0;
	MaxPathLength = 0;
	TolSearchCount = 0;
	RelaxedCount = 0;
	RemoveCount = 0;
	LongestPointNum = 0;
	FirstToMaxDepth = 0;
}

// Print for sth.
void Answer::Print() {
	std::cout << "------------------------------------------------------" << std::endl;
	std::cout << "* 点数: " << TolIndex << std::endl;
	std::cout << "* 边数: " << TolEdgeNum << std::endl;
	//std::cout << "* MLCS个数: " << MLCS.size() << std::endl;
	std::cout << "* MLCS长度: " << MaxPathLength << std::endl;
	/*std::cout << "* MLCS: " << std::endl;
	for(std::string& m : MLCS) {
		std::cout << "     " << m << std::endl;
	}*/
	std::cout << "------------------------------------------------------\n\n";
}
