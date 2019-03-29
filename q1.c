#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Process
{
   int pid;
   int bt;
} Process;

/* File reading implementation */
void read_data(char **data)
{
    *data = malloc(sizeof(char)*1000);
    FILE *fp = fopen("CPU_BURST.txt", "r");
    if (fp == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }
    fscanf(fp,"%[^\n]", *data);
    fclose(fp);
}

/* Array Generator */
void generate_burst_list(int *arr, int *size)
{
	char *str; int i, u;
    i = u = 0;
    int temp_arr[1000];
    read_data(&str);
    int init_size = strlen(str);
	char delim[] = ",";
	char *ptr = strtok(str, delim);
	while (ptr != NULL)
	{
        temp_arr[i] = atoi(ptr);
        if(temp_arr[i] <= 0){
            printf("The file has invalid contents at %d which is less than 0, Array generation failed\n", i+1);
            exit(1);
        }
		ptr = strtok(NULL, delim);
        i++;
	}
    u = i;
    *size = i;
    for(int j = 0; j < u; j++)
        arr[j] = temp_arr[j];
}

/* Calculations for the job implementation */
void waiting_time(Process *proc, int size, int *base){
    base[0] = 0;
    for (int i = 1; i < size ; i++ )
        base[i] = proc[i-1].bt + base[i-1];
}

void turn_around_time(Process *proc, int size,
                        int *base, int *base1){
    for (int i = 0; i < size ; i++)
        base1[i] = proc[i].bt + base[i];
}

void get_time(Process *proc, int size, int *t1, int *t2){
    int base[size], base1[size], wt, tat;
    wt = tat = 0;
    waiting_time(proc, size, base);
    turn_around_time(proc, size, base, base1);
    for (int i = 0; i < size; i++)
    {
        wt = wt + base[i];
        tat = tat + base1[i];
    }
    *t1 = wt;
    *t2 = tat;
    printf("Process ID       Burst Time        Waiting time      Turn Around Time\n");
    for(int i = 0; i < size; i++){
        printf ("  %d",proc[i].pid);
        printf("\t         \t%d", proc[i].bt);
        printf("\t    \t%d", base[i]);
        printf("\t            \t%d",base1[i]);
        printf("\n");
    }
}

/* Sorting implementation */
int burst_sorter(const void *ptr1, const void *ptr2){
    int x = ((Process *)ptr1)->bt;
    int y = ((Process *)ptr2)->bt;
    return x > y;
}

void job_sort(Process *proc, int size){
    qsort((void *) proc, size, sizeof(proc[0]), burst_sorter);
}

/* SJF base implementation */
void shortest_job_first(int *waiting_time, int *turn_around){
    int data[256], size;
    generate_burst_list(data, &size);
    Process proc[size];
    for(int i = 0; i < size; i++){
        proc[i].pid = i+1;
        proc[i].bt = data[i];
    }
    job_sort(proc,size);
    printf("Order of process execution is as follows\n");
    for(int i = 0; i < size; i++)
        printf("Process[%d]\n", proc[i].pid);
    printf("\n");
    get_time(proc,size,waiting_time,turn_around);
}

int main(){
    int w_time, t_time;
    w_time = t_time = 0;
    shortest_job_first(&w_time, &t_time);
    printf("Average Waiting time is %.2f\n", (float)w_time/4);
    printf("Average Turn around time is %.2f\n", (float)t_time/4);
}
