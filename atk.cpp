#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<map>
#include<string>
#include<queue>
#include<time.h>
using namespace std;
 
const int WORD_LEN = 30;
const int LINE_LEN = 10000;
const int LINE_NUM = 1e8;
const int THRESHOLD_K = 20;

class Account
{
public:
	double p[THRESHOLD_K];
	int pos[THRESHOLD_K];
	bool hit[THRESHOLD_K];
	double sum;
	int pivot;
	int line;
	Account()
	{
		memset(p, 0, sizeof(p));
		sum = 0;
		pivot = 0;
	}
};

bool operator<(const Account & x, const Account & y)
{
	return x.p[x.pivot]>y.p[y.pivot];
}

priority_queue<Account> heap;
Account* acnt;
int pwd_dic_size = 0;
int account_size = 0;
int hit_cnt = 0;
int wrong_cnt = 0;

void read_honeywords_pr_file(char* pr_file, char* honeywords_file, char* checker_file, int THRESHOLD_X)
{
	char line[LINE_LEN];
	FILE* fpr = fopen(pr_file, "r");
	FILE* fhny = fopen(honeywords_file, "r");
	FILE* fchk = fopen(checker_file, "r");
	while (fgets(line, LINE_LEN, fpr))
	{
		int l = strlen(line);
		if (line[l-1] == '\n') {line[l-1] = '\0'; l--;}
		if (line[l-1] == '\r') {line[l-1] = '\0'; l--;}

		int ptr = 0;
		int honey_cnt = 0;
		double tmpp[THRESHOLD_K];
		while (ptr <= l-1)
		{
			sscanf(line+ptr, "%lf", &tmpp[honey_cnt]);
			acnt[account_size].sum += tmpp[honey_cnt];
			while (line[ptr] != '\t' && line[ptr] != '\0') ptr++;
			ptr++;
			honey_cnt++;
			if (honey_cnt >= THRESHOLD_K) break;
		}

		fgets(line, LINE_LEN, fchk);
		l = strlen(line);
		if (line[l-1] == '\n') {line[l-1] = '\0'; l--;}
		if (line[l-1] == '\r') {line[l-1] = '\0'; l--;}
		int real_pos = atoi(line);

		fgets(line, LINE_LEN, fhny);
		l = strlen(line);
		if (line[l-1] == '\n') {line[l-1] = '\0'; l--;}
		if (line[l-1] == '\r') {line[l-1] = '\0'; l--;}
		char tmpstr[THRESHOLD_K][WORD_LEN];
		ptr = 0;
		honey_cnt = 0;
		while (ptr <= l-1)
		{
			int head = ptr, tail;
			while (line[ptr] != '\t' && line[ptr] != '\0') ptr++;
			tail = ptr; ptr++;
			for (int i = head; i < tail; i++) tmpstr[honey_cnt][i-head] = line[i];
			tmpstr[honey_cnt][tail-head] = '\0';
			honey_cnt++;
		}

		if (acnt[account_size].sum == 0)
		{
			for (int i = 0; i < THRESHOLD_K; i++) tmpp[i]+=1;
			acnt[account_size].sum += THRESHOLD_K;
		}

		bool vis[THRESHOLD_K] = {false};
		for (int i = 0; i < THRESHOLD_X; i++)
		{
			int same[THRESHOLD_K];
			double max = 0;
			int cnt = 0;
			int pos = 0;
			for (int j = 0; j < THRESHOLD_K; j++)
			{
				if (tmpp[j]>max && !vis[j])
				{
					pos = j;
					max = tmpp[j];
					same[0] = j;
					cnt = 1;
				}
				else if (tmpp[j] == max && !vis[j])
				{
					same[cnt] = j;
					cnt++;
				}
			}
			if (tmpp[pos] == max && cnt > 1)
			{
				int rrr = rand()%cnt;
				pos = same[rrr];
			}

			acnt[account_size].p[i] = 0;
			for (int j = 0; j < THRESHOLD_K; j++)
				if (j != pos && !vis[j]) acnt[account_size].p[i] += tmpp[j];

			acnt[account_size].p[i] /= acnt[account_size].sum;

			acnt[account_size].pos[i] = pos;
			if (strcmp(tmpstr[pos], tmpstr[real_pos])==0)
				acnt[account_size].hit[i] = true;
			else
				acnt[account_size].hit[i] = false;
			vis[pos] = true;
		}
		acnt[account_size].line = account_size;
		if (acnt[account_size].sum != 0) heap.push(acnt[account_size]);
		account_size++;
		if (account_size % 100000 == 0) printf("%d\n", account_size);
	}
	fclose(fpr);
	fclose(fhny);
	fclose(fchk);
}

void attack(char* cracknum_file, char* res_file, int THRESHOLD_X, int THRESHOLD_WRONG)
{
	FILE* fout = fopen(res_file, "w");
	FILE* fcrack = fopen(cracknum_file, "w");
	while (wrong_cnt < THRESHOLD_WRONG && !heap.empty())
	{
		Account tmpNode = heap.top();
		heap.pop();
		int line = tmpNode.line;
		int line_pos = tmpNode.pos[tmpNode.pivot];
		bool hit = tmpNode.hit[tmpNode.pivot];

		for (int i = acnt[line].pivot+1; i < THRESHOLD_X; i++)
			acnt[line].p[i] *= acnt[line].sum;
		acnt[line].sum = acnt[line].p[acnt[line].pivot]*acnt[line].sum;
		if (acnt[line].sum == 0)
		{
			acnt[line].sum = THRESHOLD_K - acnt[line].pivot - 1;
			for (int i = acnt[line].pivot+1; i < THRESHOLD_X; i++) 
				acnt[line].p[i] = THRESHOLD_K - acnt[line].pivot - 2;
		}
		for (int i = acnt[line].pivot+1; i < THRESHOLD_X; i++)
			acnt[line].p[i] /= acnt[line].sum;
		if (hit)
		{
			hit_cnt++;
			fprintf(fout, "%d\t%d\t%d\t%g\n", hit_cnt, line, line_pos, acnt[line].p[acnt[line].pivot]);
		}
		else
		{
			wrong_cnt++;
			fprintf(fout, "%d\t%d\t%d\t%g\n", hit_cnt, line, line_pos, acnt[line].p[acnt[line].pivot]);
			fprintf(fcrack, "%d\n", hit_cnt);
			acnt[line].pivot++;
			if (acnt[line].pivot < THRESHOLD_X)
			{
				heap.push(acnt[line]);
			}
		}
		if (wrong_cnt % 10000 == 0) printf("wrong attempts: %d\n", wrong_cnt);
	}
	fclose(fout);
	fclose(fcrack);
}

int main(int argc, char* argv[])
{
	if (argc != 6)
	{
		printf("atk parameters wrong!\n");
		return 0;
	}

	srand(time(NULL));

	acnt = new Account[LINE_NUM]();

	char prefix[LINE_LEN], cracknum_file[LINE_LEN], res_file[LINE_LEN];
	strcpy(prefix, argv[1]);
	int dotpos = strlen(prefix)-1;
	while (prefix[dotpos] != '_' && dotpos > 0) dotpos--;
	prefix[dotpos] = '\0';
	strcpy(cracknum_file, prefix);
	strcat(cracknum_file, "_cracknum.txt");
	strcpy(res_file, prefix);
	strcat(res_file, "_result.txt");

	int THRESHOLD_X = atoi(argv[4]);
	int THRESHOLD_WRONG = atoi(argv[5]);
	read_honeywords_pr_file(argv[1], argv[2], argv[3], THRESHOLD_X);
	attack(cracknum_file, res_file, THRESHOLD_X, THRESHOLD_WRONG);

	printf("hit:%d, wrong:%d, test size:%d\n", hit_cnt, wrong_cnt, account_size);
	delete acnt;

	return 0;
}
