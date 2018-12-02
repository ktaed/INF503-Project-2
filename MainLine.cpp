#include <fstream>
#include <iostream>
#include "Levenshtein.cpp"
#include "PrefixTrie.cpp"
#include <string.h>
#include <vector>
#include <utility>
#include <time.h>
using namespace std;

// The main program
int main(int argc, char *argv[])
{

	unsigned int i, j, k;
	unsigned int N;
	unsigned int wordSize, clustMin;
	unsigned int G = 0;
    unsigned int read_total = 0;
	int elapsed_time = time(0);

	int SNPLimit = atoi(argv[3]);
    char** headers, **reads;
	ifstream f;
    string genome, rc_genome,temp, line;

    // Loading the Antracis genome into string
	f.open(argv[2]);
	while (!f.eof())
	{
	    getline(f, line);
	    if (line.length() && line[0] != '>')
            genome += line;
	}
    f.close();
    rc_genome = rev_comp(genome);
    // Assignment of genome length
    G = genome.length();

    // Counting the total reads for space allocation and get read length
	f.open(argv[1], std::fstream::in); //Open File
	while(!f.eof()){
        getline(f, line);
        if (line[0] == '>')
            read_total++;
        else if (line.length())
            N = line.length();
	}
	f.close();

    // Space allocation for reads
	headers = new char*[read_total];
    reads = new char*[read_total];

    // Read in the 100-mers read set file
    f.open(argv[1], std::fstream::in); //Open File
	for(i=0;i<read_total;i++){
        getline(f, line);
        headers[i] = new char[line.length()];
        strcpy(headers[i], line.c_str());

        getline(f, line);
        reads[i] = new char[line.length()];
        strcpy(reads[i], line.c_str());

	}
	f.close();
	
	// Calculate seed length and minimum cluster size
	if (argc >4 ){
        wordSize = atoi(argv[4]);
	}else{
        wordSize = (N/(SNPLimit+1))-1;
	}
	if (argc >5 ){
        clustMin = atoi(argv[5]);
	}else{
        clustMin = (N%wordSize)+1;
	}
	
	if (argc >6 ){
        clustMin = atoi(argv[5]);
	}else{
        clustMin = (N%wordSize)+1;
	}
	if (argc >7 ){
        clustMin = atoi(argv[5]);
	}else{
        clustMin = (N%wordSize)+1;
	}
	// Building the prefix trie from reference genome
	PrefixTrie PT((char*)genome.c_str(), G, wordSize);
	PrefixTrie RC_PT((char*)rc_genome.c_str(), G, wordSize);
    cout <<  PT.node_count() <<"\n";
	// -----------> Logic below can be threaded
	// vectors for potential alignment sites and actual alignment sites
    vector<unsigned int>  potential_starts, rc_potential_starts, reset_positions;
    vector<pair<unsigned int, int>> position_starts, rc_position_starts;

    // Create a positionArray to mark seed locations
	uint8_t* positionArray = new uint8_t[G-wordSize+1];
	uint8_t* rc_positionArray = new uint8_t[G-wordSize+1];

	int *locations, *rc_locations;

    for(unsigned int ind = 0; ind < read_total; ind++){
        // Zero out position array used to find clusters
        for (j=0; j<G-wordSize+1; j++){
            positionArray[j] = 0;
            rc_positionArray[j] = 0;
        }

//        for (j=0; j<reset_positions.size(); j++){positionArray[reset_positions[j]] = 0;}
//        reset_positions.clear();
//        // Finds seeds of the desired length then inserts them into
//        // the position array incrementing to account for duplicate seeds
        for (i=0;i<wordSize;i++)
        {
            for (j=i; j < N - wordSize + 1; j+=wordSize)
            {
                locations = PT.found(reads[ind]+(j), wordSize);
                rc_locations = RC_PT.found(reads[ind]+(j), wordSize);
                if(locations){
                    for( k=1; k < locations[0];k++)
                        positionArray[locations[k]]++;
//                        reset_positions.push_back(locations[k]);
                }
                if (rc_locations){
                    for( k=1; k < rc_locations[0];k++)
                        rc_positionArray[rc_locations[k]]++;
//                        reset_positions.push_back(rc_locations[k]);
                }
                }
            }


        // Goes through positionArray to find sections that meet the minimum seed criteria
        // breaks inner loop when cluster threshold is met
        for (j=0; j < G - N + 1; j++)
        {
            int tempCount =0;
            for (k=0; k<N-wordSize+1; k++)
            {
                if (positionArray[j+k]){
                    tempCount += positionArray[j+k];
                }
                if(tempCount >= clustMin){
                    potential_starts.push_back(j);
                    break;
                }
            }

            tempCount =0;
            for (k=0; k<N-wordSize+1; k++)
            {
                if (rc_positionArray[j+k]){
                    tempCount += rc_positionArray[j+k];
                }
                if(tempCount >= clustMin){
                    rc_potential_starts.push_back(j);
                    break;
                }
            }
        }

        // Gets edit distance for potential alignment sites adding the sites that meet mismatch criteria
        // to a vector is then used to build required output string associating read to mapped locations
        for(j=0;j < potential_starts.size();j++){
            temp = genome.substr(potential_starts[j], N);
            if (temp.length() == N){
                int mm = levenshtein((char*)temp.c_str(), N, reads[ind], N, SNPLimit);
                if (mm <= SNPLimit)
                    position_starts.push_back(make_pair(potential_starts[j], mm));
            }

        }
        for(j=0;j < rc_potential_starts.size();j++){
            temp = rc_genome.substr(rc_potential_starts[j], N);
            if (temp.length() == N){
                int mm = levenshtein((char*)temp.c_str(), N, reads[ind], N, SNPLimit);
                if (mm <= SNPLimit)
                    rc_position_starts.push_back(make_pair(rc_potential_starts[j], mm));
            }
        }
        potential_starts.clear();
        rc_potential_starts.clear();

        // builds string then prints it to stdout for piping, threaded version would need to use
        // thread-safe memory space or write to different files.
        temp = headers[ind];
        temp += "$";
        for(j=0;j< position_starts.size();j++){
            temp += to_string(position_starts[j].first) +":" + to_string(position_starts[j].second) +",";
            }
        if (j){
            temp = temp.substr(0, temp.length()-1);
        }
        temp += "~";
        for(j=0;j< rc_position_starts.size();j++){
            temp += to_string(rc_position_starts[j].first) +":" + to_string(rc_position_starts[j].second) +",";
            }
        if (j){
            temp = temp.substr(0, temp.length()-1);
        }

        cout << temp <<endl;
        position_starts.clear();
        rc_position_starts.clear();
}
	elapsed_time = time(0)- elapsed_time;
	int time_remaining = elapsed_time % 86400;
        cout <<"days:"<<  elapsed_time /86400<<endl;
	elapsed_time = time_remaining;
	time_remaining = elapsed_time % 3600;
	cout<<"hours:"<<  elapsed_time /3600<<endl;
	elapsed_time = time_remaining;
	time_remaining = elapsed_time % 60;
	cout<<"minute:"<<  elapsed_time /60<<endl;
	elapsed_time = time_remaining;
	cout<<"second:"<<  elapsed_time <<endl;

	return 0;
}


