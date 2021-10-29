#include <stdio.h>
#include <algorithm>
#include <vector>
#include <math.h>

using namespace std;

int n_rows, n_cols, k, total_row = 0;
vector< vector<int> > objs;
vector< vector<int> > vars_cons;
vector< vector<int> > cons_var;
vector <int> cand;
vector< vector<int> > solutions;

int nxt_col()
{
	int i;
	for(i = 0; i < n_cols; i++)
		if(cand[i] == 0)
			return i;
	return -1;
}

int viable()
{
	int i, j;
	for(i = 0; i < n_rows; i++)
	{
		int flag = 0;
		for(j = 0; j < (int)cons_var[i].size(); j++)
			if(cand[cons_var[i][j]] != -1)
				flag = 1;
		if(!flag)
			return 0;
	}
	return 1;
}

int calc_cost(vector <int> vec, int func)
{
	int i, cost = 0;
	for(i = 0; i < n_cols; i++)
	{
		if(vec[i] != 1)
			continue;
		cost += objs[func][i];
	}
	return cost;
}

int add_sol(vector <int> cand_test)
{
	int i, j, flag = 1;
	for(i = 0; i < (int)solutions.size(); i++)
	{
		int dom = 1, is_dom = 1;
		for(j = 0; j < k; j++)
		{
			int a = calc_cost(solutions[i], j);
			int b;
			b = calc_cost(cand_test, j);
			if(a > b)
				dom = 0;
			else if(b > a)
				is_dom = 0;
		}
		if(dom == 1 && is_dom == 1)
			break;
		else if(dom == 1)
		{
			flag = 0;
			break;
		}
		else if(is_dom == 1)
		{
			swap(solutions[i], solutions[solutions.size()-1]);
			solutions.pop_back();
			i--;
		}
	}
	if(flag)
		solutions.push_back(cand_test);
	return flag;
}

void solve()
{
	if(!viable())
		return;
	int nxt = nxt_col();
	if(nxt == -1)
	{
		add_sol(cand);
		return;
	}
	cand[nxt] = -1;
	solve();
	cand[nxt] = 1;
	solve();
	cand[nxt] = 0;
	return;
}

void print_sol()
{
	int sz = solutions.size(), i;
	printf("%d\n", sz);
	for(i = 0; i < sz; i++)
	{
		for(int j = 0; j < n_cols; j++)
			printf("%d ", solutions[i][j] == 1 ? 1 : 0);
		printf("\n");
	}
}

void branch_and_bound()
{
	cand.resize(n_cols+1);
	fill(cand.begin(), cand.end(), 0);
	solve();
	print_sol();
}

void read()
{
	scanf("%d", &n_rows);
	scanf("%d", &n_cols);

	int val;

	scanf("%d", &k);
	objs.resize(k);
	for (int j = 0; j < k; ++j) {
		objs[j].resize(n_cols);
		for (int i = 0; i < n_cols; ++i) {
			scanf("%d", &val);
			objs[j][i] = val;
		}
	}

	vars_cons.resize(n_cols);
	cons_var.resize(n_rows);
	int n_c;
	for (int c = 0; c < n_rows; ++c) {
		scanf("%d", &n_c);
		cons_var[c].resize(n_c);
		for (int i = 0; i < n_c; ++i) {
			scanf("%d", &cons_var[c][i]);
			cons_var[c][i]--; //opcional, tornar 0-indexed
			vars_cons[cons_var[c][i]].push_back(c);
		}
	}
}

int main()
{
	read();
	branch_and_bound();
	return 0;
}
