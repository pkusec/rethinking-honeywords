#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string>
#include<map>
using namespace std;

const int WORD_LEN = 30;
const int LINE_LEN = 10000;

int account_size = 0;
int dic_size = 0;
map<string, int> train_dic;

void read_training_set(char* training_set)
{
	char line[LINE_LEN];
	FILE* fin = fopen(training_set, "r");
	while (fgets(line, LINE_LEN, fin))
	{
		dic_size++;
		int l = strlen(line);
		if (line[l-1] == '\n') {line[l-1] = '\0'; l--;}
		if (line[l-1] == '\r') {line[l-1] = '\0'; l--;}
		if (l >= WORD_LEN) continue;

		map<string, int>::iterator it;
		it = train_dic.find(line);
		if (it == train_dic.end())
			train_dic.insert(make_pair(line, 1));
		else
			it->second++;

		if (dic_size % 10000 == 0) printf("%d\n", dic_size);
	}
	fclose(fin);
}

void calc_honeywords_pr(char* honeywords_file, char* pr_file)
{
	char line[LINE_LEN];
	FILE* fhoney = fopen(honeywords_file, "r");
	FILE* fout = fopen(pr_file, "w");
	while (fgets(line, LINE_LEN, fhoney))
	{
		int l = strlen(line);
		if (line[l-1] == '\n') {line[l-1] = '\0'; l--;}
		if (line[l-1] == '\r') {line[l-1] = '\0'; l--;}
		int ptr = 0;
		int honey_cnt = 0;

		while (ptr <= l-1)
		{
			char tmpstr[LINE_LEN];
			int head = ptr, tail;
			while (line[ptr] != '\t' && line[ptr] != '\0') ptr++;
			tail = ptr; ptr++;
			for (int i = head; i < tail; i++) tmpstr[i-head] = line[i];
			tmpstr[tail-head] = '\0';

			int freq = 0;
			map<string, int>::iterator it;
			it = train_dic.find(tmpstr);
			if (it != train_dic.end())
				freq = it->second;

			// smooth
			fprintf(fout, "%g\t", (double)(freq+1.0)/ dic_size);

			honey_cnt++;
		}

		account_size++;
		fprintf(fout, "\n");
		if (account_size % 100000 == 0) printf("account: %d\n", account_size);
	}
	fclose(fhoney);
	fclose(fout);
}


int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		printf("calc parameters wrong!\n");
		return 0;
	}

	char prefix[LINE_LEN], pr_file[LINE_LEN];
	strcpy(prefix, argv[1]);
	int dotpos = strlen(prefix)-1;
	while (prefix[dotpos] != '_' && dotpos > 0) dotpos--;
	prefix[dotpos] = '\0';
	strcpy(pr_file, prefix);
	strcat(pr_file, "_pr.txt");
	read_training_set(argv[2]);
	calc_honeywords_pr(argv[1], pr_file);

	return 0;
}
