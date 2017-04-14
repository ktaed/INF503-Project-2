#include <fstream>
#include <iostream>
#include <random>

using namespace std;

struct Node
{
	Node *A;
	Node *C;
	Node *G;
	Node *T;
	bool Leaf;
};

class randomNum { 

    std::mt19937 rng;

public:
    randomNum() : rng(std::random_device()()) {}
    int operator()(int low, int high) { 
        int uni = low + rng() % (high-low);
        return uni;
        }
};

class PrefixTrie
{
	private:
	Node *root;
	unsigned int nodeCount;
	
	public:
	PrefixTrie()
	{
		this->nodeCount = 1;
		this->root = newNode();
	}
	
	~PrefixTrie()
	{
		prune(this->root);
		this->root = NULL;
	}

	Node *newNode()
	{
		Node *current = new Node;
		current->A = NULL;
		current->C = NULL;
		current->G = NULL;
		current->T = NULL;
		current->Leaf = 0;
		return current;
	}

	void prune(Node *current)
	{
		if (current->A != NULL)
		{
			prune(current->A);
		}
		if (current->C != NULL)
		{
			prune(current->C);
		}
		if (current->G != NULL)
		{
			prune(current->G);
		}
		if (current->T != NULL)
		{
			prune(current->T);
		}
		delete current;
		this->nodeCount--;
	}
	
	void add(char *seq, int seqSize)
	{
		Node *current = this->root;
		for (int i = 0; i < seqSize; i++)
		{
			switch (seq[i])
			{
				case 'A':
					if (current->A == NULL)
					{
						current->A = newNode();
						this->nodeCount++;
						current = current->A;
					} else {
						current = current->A;
					}
					break;
				
				case 'C':
					if (current->C == NULL)
					{
						current->C = newNode();
						this->nodeCount++;
						current = current->C;
					} else {
						current = current->C;
					}
					break;
				
				case 'G':
					if (current->G == NULL)
					{
						current->G = newNode();
						this->nodeCount++;
						current = current->G;
					} else {
						current = current->G;
					}
					break;
				
				case 'T':
					if (current->T == NULL)
					{
						current->T = newNode();
						this->nodeCount++;
						current = current->T;
					} else {
						current = current->T;
					}
					break;			
			}// end switch
		}// end while
		current->Leaf = 1;
	}
	
	unsigned int size()
	{
		return this->nodeCount;
	}
	
	bool found(char *seq, int seqSize)
	{
		Node *current = this->root;
		
		int i = 0;
		
		while ((current != NULL) && (i < seqSize))
		{
			switch (seq[i])
			{
				case 'A':	
					current = current->A;
					break;

				case 'C':
					current = current->C;
					break;

				case 'G':
					current = current->G;
					break;
					
				case 'T':
					current = current->T;
					break;
			}//end switch
			i++;
		}// end while
		if (current != NULL)
		{
			return ((current->Leaf) && (i == seqSize));
		} else {
			return 0;
		}
	}	
};

int main(int argc, char *argv[])
{
	unsigned int i, j, k, pos;
	unsigned int N = 100, merSize = 36, G = 0;
	randomNum randy;
	char randyMer[merSize];
	char temp;
	PrefixTrie PT;
	
	if (argc > 1)
	{
		N = std::atoi(argv[1]);
	}
	
	// Load DENV2
	fstream f;
	f.open("./DENV2.txt", std::fstream::in);
	while (!f.eof()) 
	{
		f >> temp;
		G++;
	}
	
	f.clear();
	f.seekg(0, std::ios::beg);
	char DENV2[G];
	for (i=0; i<G; i++)
	{
		f >> DENV2[i];
	}
	
	for (i=0; i < N; i++)
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
	
	return 0;
}

	