#include <fstream>
#include <iostream>
#include <random>
#include <smithwat.cpp>
#include <prefixtrie.cpp>

using namespace std;

class randomNum { 

    std::mt19937 rng;

public:
    randomNum() : rng(std::random_device()()) {}
    int operator()(int low, int high) { 
        int uni = low + rng() % (high-low);
        return uni;
        }
};

int main(int argc, char *argv[])
{
	unsigned int i, j, k, pos;
	unsigned int N = 100, wordSize = 23, G = 0;
	unsigned int QSize = 100;
	int largestCluster;
	randomNum randy;
	char temp[101];
	
	if (argc > 1)
	{
		N = std::atoi(argv[1]);
	}
	
	// Load genome
	fstream f;
	f.open("./DENV2.txt", std::fstream::in);
//	f.open("./test_genome.fasta", std::fstream::in);
	while (!f.eof()) 
	{
		f >> temp[0];
		G++;
	}
	
//	cout << "Genome Size: "<<G<<endl;
	
	f.clear();
	f.seekg(0, std::ios::beg);
	char genome[G];
	for (i=0; i<G; i++)
	{
		f >> genome[i];
	}
	
	PrefixTrie PT(genome, G, wordSize);
	
//	cout << "Trie Size: " << PT.size() << endl;
	
	char **DATASet = new char*[100];
	
	fstream DATAFile; // declare File
	DATAFile.open("./sample_DENV2.fasta", std::fstream::in); //Open File
//	DATAFile.open("./sample_reads.fasta", std::fstream::in); //Open File
	k=0;
	while (!DATAFile.eof())
	{
		DATAFile.getline (temp,QSize+1);
		if (temp[0] != '>') {
			DATASet[k] = new char[QSize];
			for (i=0; i<QSize; i++) {DATASet[k][i] = temp[i];}
			k++;
		};
	}
//	cout<<"Got "<< k-1<<" test reads."<<endl;
	
	int bitArray[G-wordSize+1];
	int *locations;
	
	for (i=0;i<100;i++)
	{
		for (j=0; j<G-wordSize+1; j++){bitArray[j] = 0;}
		for (j=0; j<QSize-wordSize; j++)
		{
			for (k=0; k < wordSize; k++){temp[k] = DATASet[i][k+j];}
			locations = PT.found(temp, wordSize);
			if (locations)
			{
				for (k=1; k<locations[0]; k++){bitArray[locations[k]] = 1;}
			}
		}
		largestCluster = 0;
		int tempCount = 0;
		int clusterLoc = 0;
		for (j=0; j < G - QSize; j++)
		{
			tempCount =0;
			for (k=0; k< QSize - wordSize + 1; k++)
			{
				tempCount = tempCount + bitArray[k+j];
			}
			if (tempCount > largestCluster)
			{
				clusterLoc = j;
				largestCluster = tempCount;
			}
		}
		cout << "For seq "<<i<<", the largest cluster "<<largestCluster<<" found at "<<clusterLoc<<endl;
	}
		
	return 0;
}

	