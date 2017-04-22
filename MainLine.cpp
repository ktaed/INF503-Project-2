#include <fstream>
#include <iostream>
#include <random>
#include <prefixtrie.cpp>

using namespace std;

int main(int argc, char *argv[])
{
	unsigned int i, j, k, pos;
	unsigned int N = 100, wordSize = 23, G = 0;
	randomNum randy;
	char temp;
	
	if (argc > 1)
	{
		N = std::atoi(argv[1]);
	}
	
	// Load DENV2
	fstream f;
	f.open("./test_genome.fasta", std::fstream::in);
	while (!f.eof()) 
	{
		f >> temp;
		G++;
	}
	
	cout << "Genome Size: "<<G<<endl;
	
	f.clear();
	f.seekg(0, std::ios::beg);
	char genome[G];
	for (i=0; i<G; i++)
	{
		f >> genome[i];
	}
	
	PrefixTrie PT(genome, G, wordSize);
	
	cout << "Trie Size: " << PT.size() << endl;
	
/*	for (i=0; i < N; i++)
	{
		pos = randy(0, G-merSize);	
		for (j = 0; j < merSize; j++)
		{
			randyMer[j] = DENV2[j+pos];
		}
		PT.add(randyMer, merSize);
//		randyMer[1] = 'A';
//		if (i%(N/100)==0)
//		{
//			if (PT.found(randyMer, merSize))
//			{
//				cout << i << " Hooray!!" << endl;
//			}
//		}
	}
	
	cout<< endl << "For "<<N<<" random fragments, "<<PT.size()<<" nodes were required."<< endl;
*/	
	return 0;
}

	