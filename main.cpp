#include <stdio.h>
#include <algorithm>
#include <vector>

using namespace std;

int n_rows, n_cols, k;
vector< vector<int> > objs;
vector< vector<int> > vars_cons;
vector< vector<int> > cons_var;
vector <int> cand;
vector< vector<int> > solutions;

/*
Bounds
	- Criar estruturas para guardar a melhor bound global ate agora;
	- Criar funcoes para calcular lower e upper bounds;
	- Criar funcoes para atualizar bounds;
	...
 */

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
		for(j = 0; j < (int)vars_cons[i].size(); j++)
			if(cand[vars_cons[i][j]] != -1)
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

void add_sol()
{
	int i, j, flag = 1;
	for(i = 0; i < (int)solutions.size(); i++)
	{
		int dom = 1, is_dom = 1;
		for(j = 0; j < k; j++)
		{
			int a = calc_cost(solutions[i], j);
			int b = calc_cost(cand, j);
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
		solutions.push_back(cand);
}

void solve()
{
	if(!viable())
		return;
	int nxt = nxt_col();
	if(nxt == -1)
	{
		add_sol();
	}
	// calcular bounds para a solucao atual e verificar se e pruned;
	
	cand[nxt] = -1;
	solve();
	cand[nxt] = 1;
	solve();
	return;
}

void branch_and_bound()
{
	cand.resize(n_cols);
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

	vars_cons.resize(n_rows);
	cons_var.resize(n_cols);
	int n_c;
	for (int c = 0; c < n_rows; ++c) {
		scanf("%d", &n_c);
		vars_cons[c].resize(n_c);
		for (int i = 0; i < n_c; ++i) {
			scanf("%d", &vars_cons[c][i]);
			vars_cons[c][i]--; //opcional, tornar 0-indexed
			cons_var[vars_cons[c][i]].push_back(c);
		}
	}
}

int main()
{
	read();
	branch_and_bound();
	return 0;
}
