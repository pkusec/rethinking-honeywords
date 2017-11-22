#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<algorithm>
using namespace std;

const int LINE_LEN = 100000;
const int WORD_LEN = 30;
const int THRESHOLD_K = 20;

int account_size = 0;

class MyString
{
public:
	int cnt;
	int l;
	char w[WORD_LEN];
};

class anpair
{
public:
	char a;
	int n;
};

class BaseStructure
{
public:
	int l;
	anpair w[WORD_LEN];
	BaseStructure()
	{
		l = 0;
	}
	void add(anpair anp)
	{
		w[l].a = anp.a;
		w[l].n = anp.n;
		l++;
	}
	void print()
	{
		for (int i = 0; i < l; i++) printf("%c%d", w[i].a, w[i].n);
		printf("\n");
	}
};

class SegTrieNode
{
public:
	char c;
	int count;
	SegTrieNode* firstSon;
	SegTrieNode* rightBrother;
	SegTrieNode()
	{
		count = 0;
		firstSon = NULL; rightBrother = NULL;
	}
};

class SegTrie
{
public:
	SegTrieNode* root;
	SegTrie()
	{
		root = new SegTrieNode();
	}
	bool insert(char* str)
	{
		int l = strlen(str);
		SegTrieNode* curNode = root;
		SegTrieNode* tmpNode = NULL;
		for (int i = 0; i < l; i++)
		{
			tmpNode = curNode->firstSon;
			while (tmpNode != NULL)
			{
				if (tmpNode->c == str[i]) break;
				tmpNode = tmpNode->rightBrother;
			}
			if (tmpNode == NULL)
			{
				tmpNode = new SegTrieNode();
				tmpNode->c = str[i];
				tmpNode->rightBrother = curNode->firstSon;
				curNode->firstSon = tmpNode;
			}
			curNode = tmpNode;
		}
		bool f = true;
		if (curNode->count == 0) f = false;
		else f = true;
		curNode->count++;
		return f;
	}
};

class SegTable
{
public:
	int n;
	int cnt;
	int sum;
	MyString* t;
	SegTable()
	{
		n = 0; cnt = 0; sum = 0;
	}
	void makeSpace()
	{
		t = new MyString[n]();
	}
};

// get random int from [a,b), a and b shouldn't be too big!
int get_random_int(int a, int b)
{
	int ra = rand();
	int r = (int)(a + (double)(b-a) * ra / ((double)RAND_MAX + 1)); 
	return r;
}

bool isDigit(char ch)
{
	return ch >= '0' && ch <= '9';
}

bool isAlpha(char ch)
{
	return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'); 
}

bool isSpecial(char ch)
{
	return !isDigit(ch) && !isAlpha(ch);
}

SegTrie DTree, LTree, STree;
SegTable DT[WORD_LEN], LT[WORD_LEN], ST[WORD_LEN];

bool stringCompare(const MyString & x, const MyString & y)
{
	return x.cnt > y.cnt;
}

void printSegTrieNode(SegTable* tb, SegTrieNode* node, char* path, int level)
{
	path[level] = node->c;
	if (node->count > 0)
	{
		int count = tb[level].cnt;
		for (int i = 1; i <= level; i++) tb[level].t[tb[level].cnt].w[i-1] = path[i];
		tb[level].t[count].cnt = node->count;
		tb[level].t[count].l = level;
		tb[level].cnt++;
		tb[level].sum += node->count;
	}
	SegTrieNode* tmpNode = node->firstSon;
	while (tmpNode != NULL)
	{
		printSegTrieNode(tb, tmpNode, path, level+1);
		tmpNode = tmpNode->rightBrother;
	}
}

void printIntoSegTable(SegTable *tb, SegTrie* tree)
{
	char path[WORD_LEN];
	printSegTrieNode(tb, tree->root, path, 0);
	for (int i = 0; i < WORD_LEN; i++)
		sort(tb[i].t, tb[i].t+tb[i].cnt, stringCompare);
}

void read_dic(char* dic_file)
{
	int line_cnt = 0;
	char line[LINE_LEN];
	FILE* fpwd = fopen(dic_file, "r");
	while (fgets(line, LINE_LEN, fpwd))
	{
		int l = strlen(line);
		line_cnt++;
		if (line_cnt % 10000==0) printf("%d\n", line_cnt);
		if (line[l-1] == '\n') {line[l-1] = '\0'; l--;}
		if (line[l-1] == '\r') {line[l-1] = '\0'; l--;}
		if (l>=WORD_LEN) continue;

		anpair anp;
		BaseStructure bs;
		char w[WORD_LEN];
		if (isDigit(line[0])) anp.a = 'D';
		else if (isAlpha(line[0])) anp.a = 'L';
		else anp.a = 'S';
		w[0] = line[0];
		anp.n = 1;

		int ptr = 1; int wptr = 1;
		while (line[ptr] != '\0')
		{
			if (isDigit(line[ptr]))
			{
				if (anp.a == 'D')
				{
					anp.n++;
					w[wptr] = line[ptr];
				}
				else
				{
					bs.add(anp);
					anp.a = 'D'; anp.n = 1;
					w[wptr] = '\0';
					if (isAlpha(w[0])) {if (!LTree.insert(w)) LT[wptr].n++;}
					else {if (!STree.insert(w)) ST[wptr].n++;}
					w[0] = line[ptr]; wptr = 0;
				}
			}
			else if (isAlpha(line[ptr]))
			{
				if (anp.a == 'L')
				{
					anp.n++;
					w[wptr] = line[ptr];
				}
				else
				{
					bs.add(anp);
					anp.a = 'L'; anp.n = 1;
					w[wptr] = '\0';
					if (isDigit(w[0])) {if (!DTree.insert(w)) DT[wptr].n++;}
					else {if (!STree.insert(w)) ST[wptr].n++;}
					w[0] = line[ptr]; wptr = 0;
				}
			}
			else
			{
				if (anp.a == 'S')
				{
					anp.n++;
					w[wptr] = line[ptr];
				}
				else
				{
					bs.add(anp);
					anp.a = 'S'; anp.n = 1;
					w[wptr] = '\0';
					if (isDigit(w[0])) {if (!DTree.insert(w)) DT[wptr].n++;}
					else {if (!LTree.insert(w)) LT[wptr].n++;}
					w[0] = line[ptr]; wptr = 0;
				}
			}
			ptr++; wptr++;
		}
		w[wptr] = '\0';
		if (isDigit(w[0])) {if (!DTree.insert(w)) DT[wptr].n++;}
		else if (isAlpha(w[0])) {if (!LTree.insert(w)) LT[wptr].n++;}
		else {if (!STree.insert(w)) ST[wptr].n++;}
		bs.add(anp);
	}
	fclose(fpwd);

	printf("read in.\n");

	for (int i = 0; i < WORD_LEN; i++)
	{
		DT[i].makeSpace(); LT[i].makeSpace(); ST[i].makeSpace(); 
	}

	printIntoSegTable(LT, &LTree);
	printIntoSegTable(DT, &DTree);
	printIntoSegTable(ST, &STree);
	printf("write in table.\n");
}

void gen(char* pwd, char* dst)
{
	anpair anp;
	BaseStructure bs;
	if (isDigit(pwd[0])) anp.a = 'D';
	else if (isAlpha(pwd[0])) anp.a = 'L';
	else anp.a = 'S';
	anp.n = 1;

	int ptr = 1;
	while (pwd[ptr] != '\0')
	{
		if (isDigit(pwd[ptr]))
		{
			if (anp.a == 'D') anp.n++;
			else
			{
				bs.add(anp);
				anp.a = 'D'; anp.n = 1;
			}
		}
		else if (isAlpha(pwd[ptr]))
		{
			if (anp.a == 'L') anp.n++;
			else
			{
				bs.add(anp);
				anp.a = 'L'; anp.n = 1;
			}
		}
		else
		{
			if (anp.a == 'S') anp.n++;
			else
			{
				bs.add(anp);
				anp.a = 'S'; anp.n = 1;
			}
		}
		ptr++;
	}
	bs.add(anp);

	dst[0] = '\0';
	for (int i = 0; i < bs.l; i++)
	{
		char a = bs.w[i].a;
		int n = bs.w[i].n;
		int r = 0;
		if (a == 'D')
		{
			if (DT[n].cnt == 0) return;
			r = get_random_int(0, DT[n].cnt);
			strcat(dst, DT[n].t[r].w);
		}
		else if (a == 'L')
		{
			if (LT[n].cnt == 0) return;
			r = get_random_int(0, LT[n].cnt);
			strcat(dst, LT[n].t[r].w);
		}
		else
		{
			if (ST[n].cnt == 0) return;
			r = get_random_int(0, ST[n].cnt);
			strcat(dst, ST[n].t[r].w);
		}
	}
}

void gen_model_syntax(char* pwd, int k, char** pwdset)
{
	char newpwd[WORD_LEN];
	strcpy(pwdset[0], pwd);
	for (int i = 1; i < k; i++)
	{
		gen(pwd, newpwd);
		strcpy(pwdset[i], newpwd);
	}
}

int shuffle(int* idx)
{
	int pos = 0;
	for (int i = 0; i < THRESHOLD_K; i++)
	{
		int r = get_random_int(i, THRESHOLD_K);
		int t = idx[r];
		idx[r] = idx[i];
		idx[i] = t;
	}
	for (int i = 0; i < THRESHOLD_K; i++)
		if (idx[i] == 0) {pos = i; break;}
	return pos;
}

void honeywords_gen_model_syntax(char* pwd_file, char* honeywords_file, char* checker_file)
{
	char line[LINE_LEN];
	FILE* fpwd = fopen(pwd_file, "r");
	FILE* fhny = fopen(honeywords_file, "w");
	FILE* fchk = fopen(checker_file, "w");
	read_dic(pwd_file);
	while (fgets(line, LINE_LEN, fpwd))
	{
		int l = strlen(line);
		if (line[l-1] == '\n') {line[l-1] = '\0'; l--;}
		if (line[l-1] == '\r') {line[l-1] = '\0'; l--;}
		if (l >= WORD_LEN) continue;

		char *pwdset[THRESHOLD_K];
		for (int j = 0; j < THRESHOLD_K; j++)
			pwdset[j] = new char[WORD_LEN];

		gen_model_syntax(line, THRESHOLD_K, pwdset);
		
		int idx[THRESHOLD_K];
		for (int i = 0; i < THRESHOLD_K; i++) idx[i] = i;
		int pw_pos = shuffle(idx);

		for (int i = 0; i < THRESHOLD_K; i++)
		{
			fprintf(fhny, "%s\t", pwdset[idx[i]]);
		}
		fprintf(fhny, "\n");
		fprintf(fchk, "%d\n", pw_pos);
		
		for (int j = 0; j < THRESHOLD_K; j++)
			delete pwdset[j];
	}
	fclose(fpwd);
	fclose(fhny);
	fclose(fchk);
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("gen parameters wrong!\n");
		return 0;
	}

	srand(time(NULL));

	char prefix[LINE_LEN], honeywords_file[LINE_LEN], checker_file[LINE_LEN];
	strcpy(prefix, argv[1]);
	int dotpos = strlen(prefix)-1;
	while (prefix[dotpos] != '.' && dotpos > 0) dotpos--;
	prefix[dotpos] = '\0';
	strcpy(honeywords_file, prefix);
	strcat(honeywords_file, "_honeywords.txt");
	strcpy(checker_file, prefix);
	strcat(checker_file, "_checker.txt");

	honeywords_gen_model_syntax(argv[1], honeywords_file, checker_file);

	return 0;
}
