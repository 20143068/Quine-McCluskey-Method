#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
using namespace std;

int diff_bit(string a, string b, int bit); // return the number of different bits
vector<string> duplication(vector<string>& a, vector <string>& b);
void QM_Method(vector<int> input);
void combine(vector<string>& combine_mt, vector<string>& binary_mt, int bitnum);
bool isChecked(string pi, string binary, int bit);

vector<string> nepi;
vector<string> pi;
vector<string> epi;

int main(void)
{
	//input format : bit number, the number of minterm, minterm
	//vector<int> input = { 4, 7, 0, 1, 2, 3, 10, 11, 12 };
	//vector<int> input = { 4, 11, 0, 1, 2, 4, 5, 6, 7, 8, 9, 10, 15};
	//vector<int> input = { 3, 4, 0, 1, 2, 3};
	//vector<int> input = { 4, 6, 0, 1, 2, 5, 6, 7};
	vector<int> input = { 5, 14, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 14, 29, 30, 31};
	QM_Method(input);
	return 0;
}

void QM_Method(vector<int> input)
{
	int bitnum = input[0];  // bit number 
	int num_of_mt = input[1]; // the number of minterm
	int max_mt = input.back();
	string s;
	vector<int> check(num_of_mt, 0); // array for checking combined minterms
	vector<int> minterm;
	vector<int> origin_minterm;
	vector<string> binary_minterm;
	vector<string> answer;

	for (int i = 0; i < num_of_mt; i++)  
	{
		minterm.push_back(input[i + 2]);
		origin_minterm.push_back(input[i + 2]);
	}

	for (int i = 0; i < num_of_mt; i++) // decimal to binary
	{
		int origin_minterm = minterm[i];

		while (minterm[i] > 1)
		{
			if (minterm[i] % 2 == 1)
				s += '1';
			else
				s += '0';
			minterm[i] /= 2;
		}

		if (origin_minterm == 0)
			s += '0';
		else
			s += '1';

		if (s.size() != bitnum)
		{
			while (bitnum != s.size())
			{
				s += '0';
			}
		}
		reverse(s.begin(), s.end());
		binary_minterm.push_back(s);
		s.clear();
	}

	vector<string> combine_minterm = binary_minterm;

	for (int i = 0; i < bitnum; i++)
	{
		combine(combine_minterm, binary_minterm, bitnum);
	}

	for (int i = 0; i < combine_minterm.size(); i++)
		pi.push_back(combine_minterm[i]);

	vector<vector<int> > epi_check_board(binary_minterm.size(), vector<int>(pi.size(), 0));

	for (int i = 0; i < binary_minterm.size(); i++)
	{
		for (int j = 0; j < pi.size(); j++)
		{
			if (isChecked(pi[j], binary_minterm[i], bitnum))
				epi_check_board[i][j] = 1;
		}
	}

	for (int i = 0; i < binary_minterm.size(); i++)
	{
		int ans = 0;;
		int sum = 0;

		for (int j = 0; j < pi.size(); j++)
			sum += epi_check_board[i][j];

		if (sum == 1)
		{
			for (int k = 0; k < pi.size(); k++)
			{
				if (epi_check_board[i][k] == 1)
				{
					epi.push_back(pi[k]);
					break;
				}
			}
		}			
	}

	nepi = duplication(pi, epi);
	// ascending sort
	for (int i = 0; i < epi.size(); i++)
	{
		for (int j = 0; j < bitnum; j++)
		{
			if (epi[i][j] == '-')
				epi[i][j] = '2';
		}
	}

	for (int i = 0; i < nepi.size(); i++)
	{
		for (int j = 0; j < bitnum; j++)
		{
			if (nepi[i][j] == '-')
				nepi[i][j] = '2';
		}
	}
	sort(epi.begin(), epi.end());
	sort(nepi.begin(), nepi.end());

	for (int i = 0; i < epi.size(); i++)
	{
		for (int j = 0; j < bitnum; j++)
		{
			if (epi[i][j] == '2')
				epi[i][j] = '-';
		}
	}

	for (int i = 0; i < nepi.size(); i++)
	{
		for (int j = 0; j < bitnum; j++)
		{
			if (nepi[i][j] == '2')
				nepi[i][j] = '-';
		}
	}
	// print answer //
	answer.push_back("EPI");

	epi.erase(unique(epi.begin(), epi.end()), epi.end());
	for (int i = 0; i < epi.size(); i++)
		answer.push_back(epi[i]);

	answer.push_back("NEPI");
	for (int i = 0; i < nepi.size(); i++)
		answer.push_back(nepi[i]);

	for (int i = 0; i < answer.size(); i++)
		cout << answer[i] << ' ';
}

int diff_bit(string a, string b, int bit)  // count the number of different bits
{
	int diff_bit_num = 0;

	for (int i = 0; i < bit; i++)
	{
		if (a[i] != b[i] && (a[i] != '-' || b[i] != '-' ))
			diff_bit_num++;
	}

	return diff_bit_num;
}

bool isChecked(string pi, string binary, int bit)  // check combined minterms
{
	int same_bit_num = 0;
	int count = 0;
	for (int i = 0; i < bit; i++)
	{
		if (pi[i] == '-')
			count++;
	}

	for (int i = 0; i < bit; i++)
	{
		if (pi[i] == binary[i] && (pi[i] != '-'))
			same_bit_num++;
	}

	if ((bit - count) == same_bit_num)
		return true;

	else
		return false;
}

void combine(vector<string>& combine_mt, vector<string>& binary_mt, int bitnum)
{
	vector<string> temp_mt;
	vector<string> priv_mt = combine_mt;
	int size = combine_mt.size();
	vector<int> check(size, 0);
	check.resize(size, 0);
	for (int i = 0; i < size - 1; i++)  // combine minterms
	{
		for (int j = i + 1; j < size; j++)
		{
			if (diff_bit(combine_mt[i], combine_mt[j], bitnum) == 1)  // combine when the number of different bit is 1 except '-'
			{
				for (int k = 0; k < bitnum; k++)
				{
					if (combine_mt[i][k] != combine_mt[j][k])
					{
						check[i] += 1; check[j] += 1;
						string temp = combine_mt[i];
						temp[k] = '-';
						temp_mt.push_back(temp);
						
					}
				}
			}
		}
	}

	if (temp_mt.size() == 0)
		return;

	sort(temp_mt.begin(), temp_mt.end());
	temp_mt.erase(unique(temp_mt.begin(), temp_mt.end()), temp_mt.end());
	combine_mt = temp_mt;
	
	for (int i = 0; i < check.size(); i++)
	{
		if(check[i] == 0)
			pi.push_back(priv_mt[i]);
	}

	check.clear();
}

vector<string> duplication(vector<string>& a, vector <string>& b) // remove redundant elements of two vectors
{
	vector<string>::iterator iter;
	vector<string>::iterator iter_b;
	vector<string> c = a; 

	for (iter_b = b.begin(); iter_b != b.end(); iter_b++) {
		for (iter = c.begin(); iter != c.end();) {
			if (*iter == *iter_b)
				iter = c.erase(iter); //중복 제거
			else
				iter++;
		}
	}
	return c;
}
