using namespace std;

// Levenshtein edit distance function modified to break calculation when SNP limit met
// Was slowly morphed from smith waterman to needleman wunsch to levenshtein to maximize the alignment speed
int levenshtein(char *SeqA, int sizeA, char *SeqB, int sizeB, int maxSNP)
	{
		unsigned int left, up, diag;
		int i, j;
        int gap = 1, mismatch = 1, match = 0;
		unsigned int DScore[sizeA+1][sizeB+1];

		for (i=0; i<sizeA+1; i++) {DScore[i][0] = i*gap;}
		for (j=0; j<sizeB+1; j++) {DScore[0][j] = j*gap;}

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

                if (left > up)
                    DScore[i][j] = up;
                else
                    DScore[i][j] = left;
                if (DScore[i][j] >= diag)
                    DScore[i][j] = diag;

                if (i==j && DScore[i][j] > maxSNP)
                    return DScore[i][j];
			}
		}
        return DScore[sizeA][sizeB];
	}

string rev_comp(string genome){
    string rev_comp;
    rev_comp.reserve(genome.length());
    long i;
    for(i = genome.length()-1; i >= 0; i--){
        if (genome[i] == 'A'){
            rev_comp += "T";
        }
        else if (genome[i] == 'C'){
            rev_comp += "G";
        }
        else if (genome[i] == 'G'){
            rev_comp += "C";
        }
        else{
            rev_comp += "A";
        }
    }
    return rev_comp;
}
