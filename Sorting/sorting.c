#include "sorting.h"

long* Generate_Seq1(int N);
long* Generate_Seq2(int N);

long *Load_File(char *Filename, int *Size) {
    *Size += 1;
    FILE* fp = fopen(Filename, "r");
    char* line = NULL;
    size_t len = 0;
    getline(&line, &len, fp);
    *Size = atoi(line);
    int i;
    long* Load_File = malloc((*Size)*sizeof(*Load_File));
    for(i = 0; i < *Size; i++) {
        getline(&line, &len, fp);
        Load_File[i] = atol(line);
    }
    fclose(fp);
    return Load_File;
}
int Save_File(char *Filename, long *Array, int Size) {
    FILE* fp = fopen(Filename, "w");
    int i;
    int Save_File = 0;
    fprintf(fp, "%d\n", Size);
    for(i = 0; i < Size; i++) {
        if (fprintf(fp, "%ld\n", Array[i]) > 0) {
            Save_File++;
        }
    }
    fclose(fp);
    return Save_File;
}
void Shell_Insertion_Sort(long *Array, int Size, double *N_Comp, double *N_Move) {
    long* seq = Generate_Seq1(Size);
    int seq_size = 0;
    int i = 0;
    while(seq[i] < Size) {
        seq_size++;
        i++;
    }
    int k = seq[seq_size - 1];
    int l = 1;
    int j;
    do {
        for(j = k; j < Size; j++) {
            int temp = Array[j];
            i = j;
            *N_Comp += 1;
            while (i >= k && Array[i-k] > temp) {
                Array[i] = Array[i-k];
                *N_Move += 1;
                *N_Comp += 1;
                i = i-k;
            }
            Array[i] = temp;
        }
        l++;
        k = seq[seq_size - l];
    } while(k >= 1);
    free(seq);
}

void Improved_Bubble_Sort(long *Array, int Size, double *N_Comp, double *N_Move) {
    long* seq = Generate_Seq2(Size);
    int seq_size = 1;
    int i = 0;
    while(seq[i] > 1) {
        seq_size++;
        i++;
    }
    int m = 0;
    int k;
    int j;
    do {
        k = seq[m];
        j = Size-k;
        int last;
        do {
            last = -1;
            for(i = 0; i < j; i++) {
                *N_Comp += 1;
                if(Array[i+k] < Array[i] && i+k<Size){
                    int temp = Array[i];
                    Array[i] = Array[i+k];
                    Array[i+k] = temp;
                    last = i;
                    *N_Move += 1;
                }
            }
            j = last-1;
        } while(last >= 0);
        m++;
    } while(m < seq_size);
    free(seq);
}

long* Generate_Seq1(int N) {
    long* seq = malloc((N)*sizeof(*seq));
    int i = 0;
    int p2 = 0;
    int p3 = 0;
    seq[0] = 1;
    for(i = 1; i < N; i++) {
        if (seq[p2]*2 == seq[i - 1]) {
            p2++;
        }
        if (seq[p3]*3 == seq[i - 1]) {
            p3++;
        }
        if (seq[p2]*2 < seq[p3]*3){
            seq[i] = seq[p2]*2;
            p2++;
        }
        else {
            seq[i] = seq[p3]*3;
            p3++;
        }
    }
    return seq;
}

long* Generate_Seq2(int N) {
    long* seq = malloc((N)*sizeof(*seq));
    long gap = N;
    int i = 0;
    while(gap > 1) {
        float temp = gap / 1.3;
        gap = (long)temp;
        if(gap == 9 || gap == 10) {
            gap = 11;
        }
        seq[i] = gap;
        i++;
    }
    return seq;
}

void Save_Seq1(char *Filename, int N) {
    FILE* fp = fopen(Filename, "w");
    long* seq = malloc((N)*sizeof(*seq));
    int i;
    int p2 = 0;
    int p3 = 0;
    seq[0] = 1;
    for(i = 1; i < N; i++) {
        if (seq[p2]*2 == seq[i - 1]) {
            p2++;
        }
        if (seq[p3]*3 == seq[i - 1]) {
            p3++;
        }
        if (seq[p2]*2 < seq[p3]*3){
            seq[i] = seq[p2]*2;
            p2++;
        }
        else {
            seq[i] = seq[p3]*3;
            p3++;
        }
    }
    i = 0;
    int seq_size = 0;
    while(seq[i] < N) {
        seq_size++;
        i++;
    }
    fprintf(fp, "%d\n", seq_size);
    for (i = seq_size; i >= 0; i--) {
        fprintf(fp, "%ld\n", seq[i]);
    }
    free(seq);
}

void Save_Seq2(char* Filename, int N) {
    FILE* fp = fopen(Filename, "w");
    long* seq = malloc((N)*sizeof(*seq));
    long gap = N;
    int i = 0;
    while(gap > 1) {
        float temp = gap / 1.3;
        gap = (long)temp;
        if(gap == 9 || gap == 10) {
            gap = 11;
        }
        seq[i] = gap;
        i++;
    }
    fprintf(fp, "%d\n", i);
    int j;
    for(j = 0; j < i; j++) {
        fprintf(fp, "%ld\n", seq[j]);
    }
    fclose(fp);
}