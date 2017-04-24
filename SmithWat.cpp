using namespace std;

int nw_align(char *SeqA, int sizeA, char *SeqB, int sizeB, int maxSNP)
	{
		double left, up, diag, MaxScore = 0;
		int i, j, Maxi, Maxj;
        int gap = -1, mismatch = -1, match = 0;
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

                if (left > up)
                    DScore[i][j] = left;
                else
                    DScore[i][j] = up;
                if (DScore[i][j] <= diag)
                    DScore[i][j] = diag;


				if (DScore[i][j] == diag)
				{
					Trace[i][j] = 'D';
				}
				else if (DScore[i][j] == up)
				{
					Trace[i][j] = 'U';
				}
				else
				{
					Trace[i][j] = 'L';
				}
			}
		}

		i = sizeA;
		j = sizeB;

		int SNPCount = 0;

		while (i || j)
		{
			switch (Trace[i][j])
			{
				case 'U':
					SNPCount++;
					i--;
					break;
				case 'L':
					SNPCount++;
					j--;
					break;
				case 'D':
					if (SeqA[i-1] != SeqB[j-1])
					{
						SNPCount++;
					}
					i--;
					j--;
					break;
			}
			if (SNPCount > maxSNP){
//                for(int l = 0;l<sizeA;l++)
//                    cout << SeqA[l];
//                cout << endl;
//                for(int l = 0;l<sizeB;l++)
//                    cout << SeqB[l];
//                cout << endl;
//                cout << SeqA
                break;
			}
		}
//		cout << "hhmmm\n";
		return SNPCount;
	}
