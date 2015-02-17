#include <stdio.h>
#include <string.h>
#include "Binary_Search_Tree.h"
#include <stdint.h>

#define __STDC_FORMAT_MACROS
#include <inttypes.h>


void Print_Seq_Bin_1(uint64_t *tmp, int size) {

	for(int i = 0; i < size; i++) {
		printf("%" PRIu64 "  ", tmp[i]);
	}

	printf("\n");
}
	
void Binary_Search_Tree_Read_1_Read_2::Display_Info(double time_spent) {

	fprintf(stderr, "Final:| reads: %ld | duplicates: %ld | percent: %.2f | total_seconds: %.2f | reads/sec: %.2f\n", reads, duplicates, (double)duplicates/(double)reads, time_spent, (double)reads/(double)time_spent);

}




void Binary_Search_Tree_Read_1_Read_2::Delete_And_Print(FILE *output_1, FILE *output_2) {

	Delete_And_Print_Private(&root, output_1, output_2);
}

void Write_To_File(FILE *f_out, char *id, char *seq, char *qual) {
	fprintf(f_out, "%s%s+\n%s", id, seq, qual);

}
/*Prints out tree left than right recursive*/
void Binary_Search_Tree_Read_1_Read_2::Delete_Public() {
	Delete_Private(&root_eff);
}

void Binary_Search_Tree_Read_1_Read_2::Delete_Private(Reads_Node_Eff **node) {
	if (*node == NULL) {
		return;
	}

	Delete_Private(&((*node)->left));
	Delete_Private(&((*node)->right));
	
	delete *node;
}

void Binary_Search_Tree_Read_1_Read_2::Delete_And_Print_Private(Reads_Node **node, FILE *output_1, FILE *output_2) {

	if (*node == NULL) {
		return;
	}

	Delete_And_Print_Private(&((*node)->left), output_1, output_2);
	Delete_And_Print_Private(&((*node)->right), output_1, output_2);
	
	Write_To_File(output_1, (*node)->id_1, (*node)->seq_1, (*node)->qual_1);
	Write_To_File(output_2, (*node)->id_2, (*node)->seq_2, (*node)->qual_2);
	delete *node;
}


int Sum_Qual_Score(char *qual) {

        int i = 0;
        double qual_sum = 0;
        while (qual[i] != '\0' && qual[i] != '\n') {
		qual_sum += (qual[i] - 33);
                i++;
        }
	

        return qual_sum;

}

/*Memory efficective loop using different data structure (-M option)*/
long int Binary_Search_Tree_Read_1_Read_2::Reads_Add_Tree_Public(uint64_t *seq_bin, char *id_1, char *seq_1, char *qual_1, char *id_2, char *seq_2, char *qual_2, FILE *f_read1, FILE *f_read2, bool qual_check, int size) {
	reads++;
	Reads_Add_Tree_Private(&(root_eff), seq_bin, id_1, seq_1, qual_1, id_2, seq_2, qual_2, f_read1, f_read2, qual_check, size);
	return reads;
}
/*"Normal" data structure - faster, but takes much more memory (default)*/
long int Binary_Search_Tree_Read_1_Read_2::Reads_Add_Tree_Public(uint64_t *seq_bin, char *id_1, char *seq_1, char *qual_1, char *id_2, char *seq_2, char *qual_2, bool qual_check, int size) {
	//Print_Seq_Bin_1(seq_bin, size);
	reads++;
	Reads_Add_Tree_Private(&(root), seq_bin, id_1, seq_1, qual_1, id_2, seq_2, qual_2, qual_check, size);
	return reads;
}


/*Greater than, less than, or equal to (for array of doubles)*/
int RIGHT = -1;
int LEFT = 1;
int EQUAL = 0;

int left_right_equal(uint64_t *seq_bin, uint64_t *test_node, int size) {
	int i = 0;	
	for (i; i < size; i++) {
		if (seq_bin[i] > test_node[i]) {
			return RIGHT;
		} else if (seq_bin[i] < test_node[i]) {
			return LEFT;
		}
	}

	return EQUAL;

	
}

/*Add tree follows the same logic for both
 * If null add a new node with infomration (relative to memory saving options)
 * If memory saving option is on -write to file imediately and save location in file, seq_bin, and qual (-M)
 * Otherwise, save char * of id, seq, quality, seq_bin, and qual sum (defualt)
 * Otherwise transverse the tree (right greater, left less than)
 * If it is equal - if check qual is on - take the summation of the quality score and take the greater quality (defualt)
 * to turn this feature off (Faster run times) -q
 */

void Binary_Search_Tree_Read_1_Read_2::Reads_Add_Tree_Private(Reads_Node_Eff **node, uint64_t *seq_bin, char *id_1, char *seq_1, char *qual_1, char *id_2, char *seq_2, char *qual_2, FILE *f_read1, FILE *f_read2, bool qual_check, int size) {
	if (*node == NULL) {
		int sum_qual = 0;

		if (qual_check) {
			sum_qual = Sum_Qual_Score(qual_1) + Sum_Qual_Score(qual_2);


			/*File location data structure*/
			fpos_t pos_1, pos_2;
		
			/*Gets the current location of the file (pre-write)*/
			fgetpos(f_read1, &pos_1);
			fgetpos(f_read2, &pos_2);
	
			
			*node = new Reads_Node_Eff;
			(*node)->Add_Info(seq_bin, sum_qual, pos_1, pos_2, size); 
	
			/*Since this is the -M option write to file imediately*/
			Write_To_File(f_read1, id_1, seq_1, qual_1);
			Write_To_File(f_read2, id_2, seq_2, qual_2);
		} else {
			*node = new Reads_Node_Eff;
			(*node)->Add_Info(seq_bin, size); 
		}	
		
		return;
	} else {
		/*Figures out movemnt of the tree*/
		int movement = left_right_equal(seq_bin, (*node)->seq_bin, size);

		if (movement == RIGHT) {
			Reads_Add_Tree_Private(&((*node)->right), seq_bin, id_1, seq_1, qual_1, id_2, seq_2, qual_2, f_read1, f_read2, qual_check, size);
		} else if (movement == LEFT) {
			Reads_Add_Tree_Private(&((*node)->left), seq_bin, id_1, seq_1, qual_1, id_2, seq_2, qual_2, f_read1, f_read2, qual_check, size);
		} else if (movement == EQUAL) {
			if (qual_check) {
				int sum_qual = 0;
				sum_qual = Sum_Qual_Score(qual_1) + Sum_Qual_Score(qual_2);
				/*take sthe higher quality check*/
				if (sum_qual > (*node)->sum_qual) {
					fsetpos(f_read1, &((*node)->file_loc1));
					fsetpos(f_read2, &((*node)->file_loc2));
					
					(*node)->Add_Info(sum_qual); 
				
					Write_To_File(f_read1, id_1, seq_1, qual_1);
					Write_To_File(f_read2, id_2, seq_2, qual_2);
					/*moves the FILE * back to the end*/
					fseek(f_read1, 0, SEEK_END);
					fseek(f_read2, 0, SEEK_END);
	
				}
			}
			/*Since it is equal to - there is a duplicate - so increment*/
			duplicates++;
		
			return;
		}
		
	}


}

void Binary_Search_Tree_Read_1_Read_2::Reads_Add_Tree_Private(Reads_Node **node, uint64_t *seq_bin, char *id_1, char *seq_1, char *qual_1, char *id_2, char *seq_2, char *qual_2, bool qual_check, int size) {

	if (*node == NULL) {
		int sum_qual = 0;
	
		if (qual_check) {
			sum_qual = Sum_Qual_Score(qual_1) + Sum_Qual_Score(qual_2);
		}
		/*Adds  R1 and R2 seq_bin, id, seq, qual, and sum_qual)*/
		*node = new Reads_Node;
		(*node)->Add_Info(seq_bin, id_1, seq_1, qual_1, id_2, seq_2, qual_2, sum_qual, size); 
		return;
	} else {
		/*Figures out movement*/
		int movement = left_right_equal(seq_bin, (*node)->seq_bin, size);
		if (movement == RIGHT) {
			Reads_Add_Tree_Private(&((*node)->right), seq_bin, id_1, seq_1, qual_1, id_2, seq_2, qual_2, qual_check, size);
		} else if (movement == LEFT) {
			Reads_Add_Tree_Private(&((*node)->left), seq_bin, id_1, seq_1, qual_1, id_2, seq_2, qual_2, qual_check, size);
		} else if (movement == EQUAL) {
			
			if (qual_check) {
				int sum_qual = 0;
				sum_qual = Sum_Qual_Score(qual_1) + Sum_Qual_Score(qual_2);
				if (sum_qual > (*node)->sum_qual) {
					(*node)->Add_Info(seq_bin, id_1, seq_1, qual_1,id_2, seq_2, qual_2, sum_qual, size);
				}
			}
			duplicates++;

			return;
		}
		
	}


}

