#include <stdio.h>
#include <algorithm>
#include <vector>
#include <math.h>

using namespace std;

int n_rows, n_cols, k, total_row = 0;
vector< vector<int> > objs;
vector< vector<int> > vars_cons;
vector< vector<int> > cons_var;
vector <int> cand, ct_row, ct_col, cur_cost, nxt_ct;
vector< vector<int> > solutions;

int nxt_col()
{
	int i, nx = 0;
	/*
	for(i = 0; i < n_cols; i++)
		if(cand[i] == 0)
			return i;
	*/
	for(i = 1; i < n_cols; i++)
		if(cand[i] == 0 && (nxt_ct[i] > nxt_ct[nx] || cand[nx] != 0))
			nx = i;
	return nx;
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

vector <int> lb, tmp_ct;	
void lower_bound()
{
	fill(nxt_ct.begin(), nxt_ct.end(), 0);
	fill(lb.begin(), lb.end(), 0);
	int i, j, ii;
	for(ii = 0; ii < k; ii++)
	{
		lb[ii] = cur_cost[ii];
		double tmp_cst = 0;
		for(i = 0; i < n_rows; i++)
		{
			double mn = -1;
			int tmp_col = -1;
			if(ct_row[i])
				continue;
			for(j = 0; j < (int)cons_var[i].size(); j++)
			{
				int cr = cons_var[i][j];
				if(cand[cr] != 0)
					continue;
				double tmp_vl = (double)objs[ii][cr]/(vars_cons[cr].size()-ct_col[cr]);
				if(mn == -1 || tmp_vl < mn)
					mn = tmp_vl, tmp_col = cr;
			}
			tmp_cst += mn;
			tmp_ct[tmp_col]++;
		}
		for(i = 0; i < n_cols; i++)
			nxt_ct[i] += tmp_ct[i], tmp_ct[i] = 0;
		lb[ii] += ceil(tmp_cst);
	}
}

int add_sol(int adding_flag, vector <int> cand_test)
{
	int i, j, flag = 1;
	for(i = 0; i < (int)solutions.size(); i++)
	{
		int dom = 1, is_dom = 1;
		for(j = 0; j < k; j++)
		{
			int a = calc_cost(solutions[i], j);
			int b;
			if(adding_flag)
				b = calc_cost(cand_test, j);
			else
				b = cand_test[j];
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
		else if(is_dom == 1 && adding_flag)
		{
			swap(solutions[i], solutions[solutions.size()-1]);
			solutions.pop_back();
			i--;
		}
	}
	if(flag && adding_flag)
		solutions.push_back(cand_test);
	return flag;
}

void solve()
{
	if(!viable())
		return;
	if(total_row == n_rows)
	{
		add_sol(1, cand);
		return;
	}
	lower_bound();
	int nxt = nxt_col();
	if(!add_sol(0, lb))
		return;
	cand[nxt] = -1;
	solve();
	cand[nxt] = 1;
	for(int i = 0; i < k; i++)
		cur_cost[i] += objs[i][nxt];
	for(int i = 0; i < (int)vars_cons[nxt].size(); i++)
	{
		int tmp_cr = vars_cons[nxt][i];
		ct_row[tmp_cr]++;
		if(ct_row[tmp_cr] == 1)
			total_row++;
		for(int j = 0; j < (int)cons_var[tmp_cr].size(); j++)
			ct_col[cons_var[tmp_cr][j]]++;
	}
	solve();
	for(int i = 0; i < k; i++)
		cur_cost[i] -= objs[i][nxt];
	for(int i = 0; i < (int)vars_cons[nxt].size(); i++)
	{
		int tmp_cr = vars_cons[nxt][i];
		ct_row[tmp_cr]--;
		if(ct_row[tmp_cr] == 0)
			total_row--;
		for(int j = 0; j < (int)cons_var[tmp_cr].size(); j++)
			ct_col[cons_var[tmp_cr][j]]--;
	}
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
	lb.resize(k);
	cur_cost.resize(k);
	fill(cur_cost.begin(), cur_cost.end(), 0);
	for (int j = 0; j < k; ++j) {
		objs[j].resize(n_cols);
		for (int i = 0; i < n_cols; ++i) {
			scanf("%d", &val);
			objs[j][i] = val;
		}
	}

	vars_cons.resize(n_cols);
	cons_var.resize(n_rows);
	
	tmp_ct.resize(n_cols);
	nxt_ct.resize(n_cols);
	fill(nxt_ct.begin(), nxt_ct.end(), 0);
	ct_col.resize(n_cols);
	fill(ct_col.begin(), ct_col.end(), 0);
	ct_row.resize(n_rows);
	fill(ct_row.begin(), ct_row.end(), 0);
	
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
