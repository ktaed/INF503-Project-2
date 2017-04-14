#include <iostream>
#include <random>
#include <time.h>

using namespace std;

class SmithWaterman
{
	struct AlignOut
	{
		double score;
		char ResA[200];
		char AlSymb[200];
		char ResB[200];
	};
	
	public:
	
	SmithWaterman(double Nmatch, double Nmismatch, double Ngap)
	{
		this->match = Nmatch;
		this->mismatch = Nmismatch;
		this->gap = Ngap;
		BestScore = 0;
	}		

	~SmithWaterman()
	{
		free(BestAlign);
		free(BestResA);
		free(BestResB);
	}
			
	AlignOut Align(char *SeqA, char *SeqB)
	{
		double left, up, diag, MaxScore = 0;
		int i, j, Maxi, Maxj;
		sizeA = 0;
		sizeB = 0;

		while (SeqA[sizeA] != '\0') {sizeA++;}
		while (SeqB[sizeB] != '\0') {sizeB++;}

		char ResultA[sizeA+sizeB], Alignment[sizeA+sizeB], ResultB[sizeA+sizeB];		
		double DScore[sizeA+1][sizeB+1];
		char Trace[sizeA+1][sizeB+1];
		
		for (i=0; i<sizeA+1; i++) {DScore[i][0] = i*gap; Trace[i][0] = 'U';}
		for (j=0; j<sizeB+1; j++) {DScore[0][j] = j*gap; Trace[0][j] = 'L';}
				
		for (i=1; i <= sizeA; i++)
		{
			for (j=1; j <= sizeB; j++)
			{
				left = DScore[i][j-1] + gap;
				up = DScore[i-1][j] + gap;
				if (SeqA[i-1] == SeqB[j-1]){
					diag = DScore[i-1][j-1] + match;
				} else {
					diag = DScore[i-1][j-1] + mismatch;
				}
				DScore[i][j] = max(max(left,up),max((double) 0,diag));
				
				if (DScore[i][j] >= MaxScore)
				{
					MaxScore = DScore[i][j];
					Maxi = i;
					Maxj = j;
				}
				
				if (DScore[i][j] == left)
				{
					Trace[i][j] = 'L';
				} 
				if (DScore[i][j] == up)
				{
					Trace[i][j] = 'U';
				}
				if (DScore[i][j] == diag)
				{
					Trace[i][j] = 'D';
				}
				if (DScore[i][j] == 0)
				{
					Trace[i][j] = 'S';
				}
			}
		}

		i = Maxi;
		j = Maxj;

		int k = 0;
				
		while (((i != 0) || (j != 0)) && (Trace[i][j] != 'S'))
		{
			switch (Trace[i][j])
			{
				case 'U':
					ResultA[k] = SeqA[i-1];
					ResultB[k] = '_';
					Alignment[k] = ' ';
					i--;
					break;
				case 'L':
					ResultA[k] = '_';
					ResultB[k] = SeqB[j-1];
					Alignment[k] = ' ';
					j--;
					break;
				case 'D':
					ResultA[k] = SeqA[i-1];
					ResultB[k] = SeqB[j-1];
					if (ResultA[k] == ResultB[k]) 
					{
						Alignment[k] = '|';
					} else {
						Alignment[k] = 'X';
					}
					i--; j--;
					break;
			}
		k++;
		}
		
		AlignOut output;
		
		for (i=0; i < k; i++){
			
			output.ResA[i] = ResultA[k-i-1];
			output.ResB[i] = ResultB[k-i-1];
			output.AlSymb[i] = Alignment[k-i-1];
		}
		
		output.ResA[k] = '\0';
		output.ResB[k] = '\0';
		output.AlSymb[k] = '\0';
		output.score = MaxScore;
		
		return output;
	}
	
	void checkBest(char *SeqA, char *SeqB)
	{
		AlignOut current = Align(SeqA, SeqB);
		
		if (BestScore == 0)
		{
			BestAlign = new char[sizeA+sizeB];
			BestResA = new char[sizeA+sizeB];
			BestResB = new char[sizeA+sizeB];
		}
		
		if (current.score >= BestScore)
		{
			BestScore = current.score;
			
			for (int i=0; i < sizeA+sizeB; i++)
			{
				BestResA[i] = current.ResA[i];
				BestResB[i] = current.ResB[i];
				BestAlign[i] = current.AlSymb[i];
			}
		}

	}

	void showBest()
	{
//		int i=0;
//		while (BestAlign[i] != '\0') {i++;}
		
		cout << endl << "Best score: "<< BestScore << endl << endl;
//		cout << "Alignment Length: " << i << endl << endl;
		cout << BestResA << endl;
		cout << BestAlign << endl;
		cout << BestResB << endl << endl;
	}		
			
	void changeScoreMat(double newMatch, double newMismatch, double newGap)
	{
		this->match = newMatch;
		this->mismatch = newMismatch;
		this->gap = newGap;
	}

	private:
	
	char *BestResA, *BestResB, *BestAlign;
	double match, mismatch, gap, BestScore;
	int sizeA, sizeB;
	
};

class randomNum 
{ 
    std::mt19937 rng;
public:
    randomNum() : rng(std::random_device()()) {}
    int operator()(int low, int high) { 
        int uni = low + rng() % (high - low +1);
        return uni;
    }
};

	
char *fragMaker(int size)
{
	randomNum randy;
	char lets[] = "ACGT";
	char *A = new char[size+1];
	for (int i=0; i<size; i++)
	{
		A[i] = lets[randy(0,3)];
	}
	A[size] = '\0';
	return A;
};

	

int main(int argc, char *argv[])
{
	unsigned int start=time(0), finish;
	
	int ntsize = 100;
	
	int numSubs = 1;
	
	if (argc > 1)  
	{
		numSubs = std::atoi(argv[1]);
	}
	
	SmithWaterman frag(2, -1, -3);
	
	char *Q = fragMaker(ntsize);	

	for (int i=0; i<numSubs; i++)
	{
		char *S = fragMaker(ntsize);
		frag.checkBest(S, Q);
	}
	
	frag.showBest();
	
	finish = time(0);
	cout << "Elapsed time: "<<(finish-start)/60<<" minutes and ";
	cout <<(finish - start) % 60 << " seconds."<<endl;
	
	return 0;
}