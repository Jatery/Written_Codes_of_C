#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
	int total;
	int addr[10][2];
	int N;
} Group;

typedef struct {
	int value;
	int groupIndex;
} Node;

int min(int a, int b) {
	return a < b ? a : b;
}

int max(int a, int b) {
	return a > b ? a : b;
}

void solve(Node arr[9][9], int *index, Group groups[]) {
	if(*index == 81) {
		return;
	}
	if(arr[*index / 9][*index % 9].value > 0) {
		(*index)++;
		solve(arr, index, groups);
		if(*index != 81)
			(*index)--;
		return;
	}
	int total = groups[arr[*index / 9][*index % 9].groupIndex].total;
	int N = groups[arr[*index / 9][*index % 9].groupIndex].N;
	if(total <= 0)
		return;	
	for(int i = min(total - N * (N - 1) / 2, 9); i >= max(total - (20 - N) * (N - 1) / 2, 1) && *index != 81; --i) {
		bool flag = 1;
		for(int j = 0; j < 9 && flag; ++j) {
			if(arr[*index / 9][j].value == i)
				flag = 0;
			else if(arr[j][*index % 9].value == i)
				flag = 0;
		}
		for(int j = 0; j < 3 && flag; ++j)
			for(int k = 0; k < 3 && flag; ++k) {
				if(arr[((*index / 9) / 3) * 3 + j][((*index % 9) / 3) * 3 + k].value == i)
					flag = 0;
			}
		if(flag) {
			groups[arr[*index / 9][*index % 9].groupIndex].total -= i;
			groups[arr[*index / 9][*index % 9].groupIndex].N--;
			arr[*index / 9][*index % 9].value = i;
			(*index)++;
			solve(arr, index, groups);
			if(*index != 81) {
				(*index)--;
				arr[*index / 9][*index % 9].value = 0;
				groups[arr[*index / 9][*index % 9].groupIndex].total += i;
				groups[arr[*index / 9][*index % 9].groupIndex].N++;
			}
		}
	}
}

int main() {
	FILE *fpIn, *fpOut;
	int w;
	scanf("%d", &w);
	while (w--) {
		char inputPathName[] = "testcase/b074_0 .in";
		inputPathName[strstr(inputPathName, " ") - inputPathName] = w + '0';
		fpIn = fopen(inputPathName, "r");
		char outputPathName[] = "testcase/b074_0 .out";
		outputPathName[strstr(outputPathName, " ") - outputPathName] = w + '0';
		fpOut = fopen(outputPathName, "w+");
		assert(fpIn != NULL);
		int n, m;
		fscanf(fpIn, "%d", &n);
		while(n--) {
			fscanf(fpIn, "%d", &m);
			Node arr[9][9];
			for(int i = 0; i < 9; ++i)
				for(int j = 0; j < 9; ++j) {
					fscanf(fpIn, "%d", &arr[i][j].value);
				}
			Group groups[85];
			for(int i = 0; i < m; ++i) {
				groups[i].N = 0;
			}
			for(int i = 0; i < 9; ++i) {
				for(int j = 0; j < 9; ++j) {
					fscanf(fpIn, "%d", &arr[i][j].groupIndex);
					groups[arr[i][j].groupIndex].addr[groups[arr[i][j].groupIndex].N][0] = i;
					groups[arr[i][j].groupIndex].addr[groups[arr[i][j].groupIndex].N][1] = j;
					groups[arr[i][j].groupIndex].N++;
				}
			}

#ifdef DEBUG
			for(int i = 0; i < 9; ++i) {
				for(int j = 0; j < 9; ++j) {
					printf("%3d", arr[i][j].groupIndex);
				}
				printf("\n");
			}
#endif

			for(int i = 0; i < m; ++i) {
				fscanf(fpIn, "%d", &groups[i].total);
				int N = groups[i].N;
				for(int j = 0; j < N; ++j) {
					assert(arr[groups[i].addr[j][0]][groups[i].addr[j][1]].groupIndex == i);
					if(arr[groups[i].addr[j][0]][groups[i].addr[j][1]].value > 0)
						groups[i].N--;
					groups[i].total -= arr[groups[i].addr[j][0]][groups[i].addr[j][1]].value;
				}
			}
				
#ifdef DEBUG
			for(int i = 0; i < 9; ++i) {
				for(int j = 0; j < 9; ++j) {
					printf("%3d", groups[arr[i][j].groupIndex].total);
				}
				printf("\n");
			}
#endif
			int index = 0;
			solve(arr, &index, groups);
			if(index != 81) {
				fprintf(fpOut, "No solution.\n");
			}
			for(int i = 0; i < 9; ++i) {
				for(int j = 0; j < 9; ++j) {
					fprintf(fpOut, "%d ", arr[i][j].value);
				}
				fprintf(fpOut, "\n");
			}
			fprintf(fpOut, "\n");
#ifdef DEBUG
			for(int i = 0; i < 9; ++i) {
				for(int j = 0; j < 9; ++j) {
					if(arr[i][j].groupIndex == -1) {
						printf("Error: %d %d\n", i, j);
						exit(0);
					}
				}
			}
#endif
		}	
		fclose(fpIn);
		fclose(fpOut);
	}
}

