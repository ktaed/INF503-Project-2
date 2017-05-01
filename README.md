# INF503-Project-2
Daniel Edem Kpormegbey  
Sareh Assiri  
Isaac Shaffer  
Tsosie E Schneider  

MainLine.cpp --- Main Program  takes 5 command line parameters. First 3 are required.
   Input
	<reads_file> <reference_genome> <mismatch_tolerance> <seed_size> <cluster_count>
   Output
	string to stdout in the format 
	<read_header>$<comma seperated list mapping position to mismatch with ':'>~<comma seperated list mapping reverse complement position to mismath with ':'>
prefixtrie.cpp --- PrefixTrie class  
Levenshtein.cpp  --- Levenshtein distance function  

sample_DENV2.fasta --- 100 sequences of 100 bp in fasta format from DENV2 with 1% error and all 'A' line  
sample_reads.fasta --- 100 sequences of 100 bp in fasta format from the test_reads.fasta file.  
DENV2.txt --- Dengue genome  
inf503-notes.pdf --- Some algorithmic background and underlying concepts.  
