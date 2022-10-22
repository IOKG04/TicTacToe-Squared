#include <stdio.h>
#include <stdlib.h>

struct _Field{
    signed char state;		// -1: Undecided | 0: Draw | 1: Player-1 | 2: Player-2
    int depth;
    struct _Field *parent;
    struct _Field **children;
};
typedef struct _Field Field;

int fw(Field *f, char new_state){
    f->state = new_state;
    return new_state;
}
signed char fr(Field *f){
    return f->state;
}
Field *ff(Field* master, int depth, int *indexes){
    Field *outp = master;
    for(int i = 0; i < depth; ++i){
	outp = outp->children[indexes[i]];
    }
    return outp;
}

signed char field_update(Field *f){
    // get counts
    int unc = 0;
    int drc = 0;
    int p1c = 0;
    int p2c = 0;
    for(int i = 0; i < 9; ++i){
	if(fr(f->children[i]) == -1) ++unc;
	if(fr(f->children[i]) == 0) ++drc;
	if(fr(f->children[i]) == 1) ++p1c;
	if(fr(f->children[i]) == 2) ++p2c;
    }

    if(unc > 7) return -1;

    if(fr(f->children[0]) != -1 && f->children[0] != 0){
	if(fr(f->children[1]) == fr(f->children[0]) && fr(f->children[2]) == fr(f->children[0])){fw(f, fr(f->children[0])); return fr(f);}
	if(fr(f->children[3]) == fr(f->children[0]) && fr(f->children[6]) == fr(f->children[0])){fw(f, fr(f->children[0])); return fr(f);}
    }
    if(fr(f->children[4]) != -1 && f->children[4] != 0){
	if(fr(f->children[3]) == fr(f->children[4]) && fr(f->children[5]) == fr(f->children[4])){fw(f, fr(f->children[4])); return fr(f);}
	if(fr(f->children[1]) == fr(f->children[4]) && fr(f->children[7]) == fr(f->children[4])){fw(f, fr(f->children[4])); return fr(f);}
	if(fr(f->children[0]) == fr(f->children[4]) && fr(f->children[8]) == fr(f->children[4])){fw(f, fr(f->children[4])); return fr(f);}
	if(fr(f->children[2]) == fr(f->children[4]) && fr(f->children[6]) == fr(f->children[4])){fw(f, fr(f->children[4])); return fr(f);}
    }
    if(fr(f->children[8]) != -1 && f->children[8] != 0){
	if(fr(f->children[6]) == fr(f->children[8]) && fr(f->children[7]) == fr(f->children[8])){fw(f, fr(f->children[8])); return fr(f);}
	if(fr(f->children[2]) == fr(f->children[8]) && fr(f->children[5]) == fr(f->children[8])){fw(f, fr(f->children[8])); return fr(f);}
    }

    if(unc == 0){
	if(p1c > p2c){fw(f, 1); return 1;}
	if(p2c > p1c){fw(f, 2); return 2;}
	if(p1c == p2c){fw(f, 0); return 0;}
    }

    return -1;
}

int ffree(Field *f){
    if(f->depth > 1){
	ffree(f->children[0]);
	ffree(f->children[1]);
	ffree(f->children[2]);
	ffree(f->children[3]);
	ffree(f->children[4]);
	ffree(f->children[5]);
	ffree(f->children[6]);
	ffree(f->children[7]);
	ffree(f->children[8]);
    }
    free(f->children);
    free(f);
    return 0;
}
Field *fnew(int depth){
    Field *outp = malloc(sizeof(Field));
    outp->state = -1;
    outp->depth = depth;
    if(depth > 0){
	outp->children = malloc (sizeof(Field*) * 9);
	outp->children[0] = fnew(depth - 1);
	outp->children[0]->parent = outp;
	outp->children[1] = fnew(depth - 1);
	outp->children[1]->parent = outp;
	outp->children[2] = fnew(depth - 1);
	outp->children[2]->parent = outp;
	outp->children[3] = fnew(depth - 1);
	outp->children[3]->parent = outp;
	outp->children[4] = fnew(depth - 1);
	outp->children[4]->parent = outp;
	outp->children[5] = fnew(depth - 1);
	outp->children[5]->parent = outp;
	outp->children[6] = fnew(depth - 1);
	outp->children[6]->parent = outp;
	outp->children[7] = fnew(depth - 1);
	outp->children[7]->parent = outp;
	outp->children[8] = fnew(depth - 1);
	outp->children[8]->parent = outp;
    }
    return outp;
}

char *frc(Field *f){
    if(f->state == -1) return " \x00";
    if(f->state == 0) return "d\x00";
    if(f->state == 1) return "O\x00";
    if(f->state == 2) return "X\x00";
    return "?\x00";
}

Field *input_field(Field *master){
    Field *outp = master;

    // get undecided field 1
    printf("Please enter as [Field index]\n");
    int fieldcoord;
    goto INP_F1;
REINP_F1:
    printf("FIeld you entered isn't undecided or doesn't exist, please retry\n");
INP_F1:
    scanf("%i", &fieldcoord);
    if(fieldcoord > 8 || fieldcoord < 0) goto REINP_F1;
    if(fr(master->children[fieldcoord]) != -1) goto REINP_F1;
    outp = outp->children[fieldcoord];
    
    return outp;
}

int max_depth;
int turn;

int main(int argc, char **argv){
    // get depth ; init master
    max_depth = 2;
    Field *master = fnew(max_depth);

    // loop
    turn = 0;
    while(master->state == -1){
	printf("---NEW-TURN---\n\n\nCurrent field:\n\n");

	// print master (not generic as TTT^2 is complicated enough)
	printf("             |             |\n");
	printf("  %s | %s | %s  |  %s | %s | %s  |  %s | %s | %s\n", frc(master->children[0]->children[0]), frc(master->children[0]->children[1]), frc(master->children[0]->children[2]), frc(master->children[1]->children[0]), frc(master->children[1]->children[1]), frc(master->children[1]->children[2]), frc(master->children[2]->children[0]), frc(master->children[2]->children[1]), frc(master->children[2]->children[2]));
	printf(" ---+---+--- | ---+---+--- | ---+---+---\n");
	printf("  %s | %s | %s  |  %s | %s | %s  |  %s | %s | %s\n", frc(master->children[0]->children[3]), frc(master->children[0]->children[4]), frc(master->children[0]->children[5]), frc(master->children[1]->children[3]), frc(master->children[1]->children[4]), frc(master->children[1]->children[5]), frc(master->children[2]->children[3]), frc(master->children[2]->children[4]), frc(master->children[2]->children[5]));
	printf(" ---+---+--- | ---+---+--- | ---+---+---\n");
	printf("  %s | %s | %s  |  %s | %s | %s  |  %s | %s | %s\n", frc(master->children[0]->children[6]), frc(master->children[0]->children[7]), frc(master->children[0]->children[8]), frc(master->children[1]->children[6]), frc(master->children[1]->children[7]), frc(master->children[1]->children[8]), frc(master->children[2]->children[6]), frc(master->children[2]->children[7]), frc(master->children[2]->children[8]));
	printf("             |             |\n");
	printf("-----------------------------------------\n");
	printf("             |             |\n");
	printf("  %s | %s | %s  |  %s | %s | %s  |  %s | %s | %s\n", frc(master->children[3]->children[0]), frc(master->children[3]->children[1]), frc(master->children[3]->children[2]), frc(master->children[4]->children[0]), frc(master->children[4]->children[1]), frc(master->children[4]->children[2]), frc(master->children[5]->children[0]), frc(master->children[5]->children[1]), frc(master->children[5]->children[2]));
	printf(" ---+---+--- | ---+---+--- | ---+---+---\n");
	printf("  %s | %s | %s  |  %s | %s | %s  |  %s | %s | %s\n", frc(master->children[3]->children[3]), frc(master->children[3]->children[4]), frc(master->children[3]->children[5]), frc(master->children[4]->children[3]), frc(master->children[4]->children[4]), frc(master->children[4]->children[5]), frc(master->children[5]->children[3]), frc(master->children[5]->children[4]), frc(master->children[5]->children[5]));
	printf(" ---+---+--- | ---+---+--- | ---+---+---\n");
	printf("  %s | %s | %s  |  %s | %s | %s  |  %s | %s | %s\n", frc(master->children[3]->children[6]), frc(master->children[3]->children[7]), frc(master->children[3]->children[8]), frc(master->children[4]->children[6]), frc(master->children[4]->children[7]), frc(master->children[4]->children[8]), frc(master->children[5]->children[6]), frc(master->children[5]->children[7]), frc(master->children[5]->children[8]));
	printf("             |             |\n");
	printf("-----------------------------------------\n");
	printf("             |             |\n");
	printf("  %s | %s | %s  |  %s | %s | %s  |  %s | %s | %s\n", frc(master->children[6]->children[0]), frc(master->children[6]->children[1]), frc(master->children[6]->children[2]), frc(master->children[7]->children[0]), frc(master->children[7]->children[1]), frc(master->children[7]->children[2]), frc(master->children[8]->children[0]), frc(master->children[8]->children[1]), frc(master->children[8]->children[2]));
	printf(" ---+---+--- | ---+---+--- | ---+---+---\n");
	printf("  %s | %s | %s  |  %s | %s | %s  |  %s | %s | %s\n", frc(master->children[6]->children[3]), frc(master->children[6]->children[4]), frc(master->children[6]->children[5]), frc(master->children[7]->children[3]), frc(master->children[7]->children[4]), frc(master->children[7]->children[5]), frc(master->children[8]->children[3]), frc(master->children[8]->children[4]), frc(master->children[8]->children[5]));
	printf(" ---+---+--- | ---+---+--- | ---+---+---\n");
	printf("  %s | %s | %s  |  %s | %s | %s  |  %s | %s | %s\n", frc(master->children[6]->children[6]), frc(master->children[6]->children[7]), frc(master->children[6]->children[8]), frc(master->children[7]->children[6]), frc(master->children[7]->children[7]), frc(master->children[7]->children[8]), frc(master->children[8]->children[6]), frc(master->children[8]->children[7]), frc(master->children[8]->children[8]));
	printf("             |             |\n");

	Field *changed_field_m, *changed_field_s;
	// player 1 turn
	if((turn % 2) == 0){
	    printf("It is O's turn, please select field to place O at\n\nExample TTT field:\n");
	    printf("012\n345\n678\n\n");
	    changed_field_m = input_field(master);
	    changed_field_s = input_field(changed_field_m);
	    fw(changed_field_s, 1);
	}
	// player 2 turn
	else{
	    printf("It is X's turn, please select field to place X at\n\nExample TTT field:\n");
	    printf("012\n345\n678\n\n");
	    changed_field_m = input_field(master);
	    changed_field_s = input_field(changed_field_m);
	    fw(changed_field_s, 2);
	}
	field_update(changed_field_m);

	field_update(master);
	++turn;
    }

    // print win message
    if(master->state == 0) printf("\nNo one won, it's a draw\n");
    if(master->state == 1) printf("\nPlayer 1 won in %i turns\n", turn);
    if(master->state == 2) printf("\nPlayer 2 won in %i turns\n", turn);

    printf("Final board:\n");
    printf("             |             |\n");
    printf("  %s | %s | %s  |  %s | %s | %s  |  %s | %s | %s\n", frc(master->children[0]->children[0]), frc(master->children[0]->children[1]), frc(master->children[0]->children[2]), frc(master->children[1]->children[0]), frc(master->children[1]->children[1]), frc(master->children[1]->children[2]), frc(master->children[2]->children[0]), frc(master->children[2]->children[1]), frc(master->children[2]->children[2]));
    printf(" ---+---+--- | ---+---+--- | ---+---+---\n");
    printf("  %s | %s | %s  |  %s | %s | %s  |  %s | %s | %s\n", frc(master->children[0]->children[3]), frc(master->children[0]->children[4]), frc(master->children[0]->children[5]), frc(master->children[1]->children[3]), frc(master->children[1]->children[4]), frc(master->children[1]->children[5]), frc(master->children[2]->children[3]), frc(master->children[2]->children[4]), frc(master->children[2]->children[5]));
    printf(" ---+---+--- | ---+---+--- | ---+---+---\n");
    printf("  %s | %s | %s  |  %s | %s | %s  |  %s | %s | %s\n", frc(master->children[0]->children[6]), frc(master->children[0]->children[7]), frc(master->children[0]->children[8]), frc(master->children[1]->children[6]), frc(master->children[1]->children[7]), frc(master->children[1]->children[8]), frc(master->children[2]->children[6]), frc(master->children[2]->children[7]), frc(master->children[2]->children[8]));
    printf("             |             |\n");
    printf("-----------------------------------------\n");
    printf("             |             |\n");
    printf("  %s | %s | %s  |  %s | %s | %s  |  %s | %s | %s\n", frc(master->children[3]->children[0]), frc(master->children[3]->children[1]), frc(master->children[3]->children[2]), frc(master->children[4]->children[0]), frc(master->children[4]->children[1]), frc(master->children[4]->children[2]), frc(master->children[5]->children[0]), frc(master->children[5]->children[1]), frc(master->children[5]->children[2]));
    printf(" ---+---+--- | ---+---+--- | ---+---+---\n");
    printf("  %s | %s | %s  |  %s | %s | %s  |  %s | %s | %s\n", frc(master->children[3]->children[3]), frc(master->children[3]->children[4]), frc(master->children[3]->children[5]), frc(master->children[4]->children[3]), frc(master->children[4]->children[4]), frc(master->children[4]->children[5]), frc(master->children[5]->children[3]), frc(master->children[5]->children[4]), frc(master->children[5]->children[5]));
    printf(" ---+---+--- | ---+---+--- | ---+---+---\n");
    printf("  %s | %s | %s  |  %s | %s | %s  |  %s | %s | %s\n", frc(master->children[3]->children[6]), frc(master->children[3]->children[7]), frc(master->children[3]->children[8]), frc(master->children[4]->children[6]), frc(master->children[4]->children[7]), frc(master->children[4]->children[8]), frc(master->children[5]->children[6]), frc(master->children[5]->children[7]), frc(master->children[5]->children[8]));
    printf("             |             |\n");
    printf("-----------------------------------------\n");
    printf("             |             |\n");
    printf("  %s | %s | %s  |  %s | %s | %s  |  %s | %s | %s\n", frc(master->children[6]->children[0]), frc(master->children[6]->children[1]), frc(master->children[6]->children[2]), frc(master->children[7]->children[0]), frc(master->children[7]->children[1]), frc(master->children[7]->children[2]), frc(master->children[8]->children[0]), frc(master->children[8]->children[1]), frc(master->children[8]->children[2]));
    printf(" ---+---+--- | ---+---+--- | ---+---+---\n");
    printf("  %s | %s | %s  |  %s | %s | %s  |  %s | %s | %s\n", frc(master->children[6]->children[3]), frc(master->children[6]->children[4]), frc(master->children[6]->children[5]), frc(master->children[7]->children[3]), frc(master->children[7]->children[4]), frc(master->children[7]->children[5]), frc(master->children[8]->children[3]), frc(master->children[8]->children[4]), frc(master->children[8]->children[5]));
    printf(" ---+---+--- | ---+---+--- | ---+---+---\n");
    printf("  %s | %s | %s  |  %s | %s | %s  |  %s | %s | %s\n", frc(master->children[6]->children[6]), frc(master->children[6]->children[7]), frc(master->children[6]->children[8]), frc(master->children[7]->children[6]), frc(master->children[7]->children[7]), frc(master->children[7]->children[8]), frc(master->children[8]->children[6]), frc(master->children[8]->children[7]), frc(master->children[8]->children[8]));
    printf("             |             |\n");

    // free and return
    ffree(master);
    return 0;
}
