#include <fstream>
#include <iostream>
#include <random>
#include "SmithWat.cpp"
#include "PrefixTrie.cpp"
#include <string.h>
#include <vector>
#include <utility>

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
	unsigned int N;
	unsigned int wordSize, clustMin;
	unsigned int G = 0;
	unsigned int QSize;
	int SNPLimit = atoi(argv[3]);
//	int largestCluster;
//	SmithWat::AlignOut alignment;
//	SmithWat::SmithWaterman SW(2,-1,-3);
//	randomNum randy;

    char** headers, **reads;
	// Load genome
	ifstream f;
    string genome,temp, line;

	f.open(argv[2]);
//	f.open("./test_genome.fasta", std::fstream::in);
	while (!f.eof())
	{
	    getline(f, line);
	    if (line.length() && line[0] != '>')
            genome += line;
//        cout << line;
	}
    f.close();
//    cout << genome.length() << endl;
//    cout << genome << endl;////	cout << "Genome Size: "<<G<<endl;
//
//	f.clear();
//	f.seekg(0, std::ios::beg);
//	string genome = temp;
//    strcpy(genome, temp.c_str());
    G = genome.length();


	fstream DATAFile; // declare File
	DATAFile.open(argv[1], std::fstream::in); //Open File
	unsigned int read_total = 0;
	while(!DATAFile.eof()){
        getline(DATAFile, line);
        if (line[0] == '>')
            read_total++;
        else if (line.length())
            N = line.length();
	}
	DATAFile.close();

    DATAFile.open(argv[1], std::fstream::in); //Open File
    headers = new char*[read_total];
    reads = new char*[read_total];
	for(i=0;i<read_total;i++){
        getline(DATAFile, line);
        headers[i] = new char[line.length()];

        strcpy(headers[i], line.c_str());
        getline(DATAFile, line);
        reads[i] = new char[line.length()];
        strcpy(reads[i], line.c_str());

	}
	DATAFile.close();

	wordSize = (N/(SNPLimit+1))-1;
	clustMin = (N%wordSize)+1;
	PrefixTrie PT((char*)genome.c_str(), G, wordSize);
    vector<unsigned int>  potential_starts;
    vector<pair<unsigned int, int>> position_starts;
	uint8_t* positionArray = new uint8_t[G-wordSize+1];
	int *locations;
    for(unsigned int ind = 0; ind < read_total; ind++){
        for (j=0; j<G-wordSize+1; j++){positionArray[j] = 0;}

        for (i=0;i<wordSize;i++)
        {
            for (j=i; i+j<N-wordSize; j+=wordSize)
            {
    //			for (k=0; k < wordSize; k++){temp[k] = DATASet[i][k+j];}
                locations = PT.found(reads[ind]+(i+j), wordSize);
                if(locations){
                    for( k=1; k < locations[0];k++)
                        positionArray[locations[k]] = 1;
                }
            }
        }
//    		largestCluster = 0;
        for (j=0; j < G - N; j++)
        {
            int tempCount =0;
            for (k=0; k<N; k++)
            {
                if (positionArray[j+k])
                    tempCount++;
                if(tempCount >= clustMin){
                    potential_starts.push_back(j);
                    break;
                }
            }
        }
        for(j=0;j < potential_starts.size();j++){
//                cout << potential_starts[j] <<endl;
            temp = genome.substr(potential_starts[j], N);
            if (temp.length() == N){
//                cout << temp <<endl;
//                cout << reads[ind] <<endl;
                int mm = nw_align((char*)temp.c_str(), N, reads[ind], N, SNPLimit);
                if (mm <= SNPLimit)
                    position_starts.push_back(make_pair(potential_starts[j], mm));
            }
        }
        potential_starts.clear();
        temp = headers[ind];
        temp += "$";
        for(j=0;j< position_starts.size();j++){
            temp += to_string(position_starts[j].first) +":" + to_string(position_starts[j].second) +",";
            }
            cout << temp.substr(0, temp.length()-1) <<endl;
            position_starts.clear();

        }
//    cout << position_starts.size() << endl;
//    }
//    for (i=0;i<position_starts.size();i++)
//        cout << position_starts[i].first<<"\t"<<position_starts[i].second <<endl;
	return 0;
}

