#include "Max3Sat.h"
#include <sstream>
#pragma warning(disable:4996)

int count_the_lines(string filename)
{
	int lines = 0;
	FILE* infile = fopen(filename.c_str(), "r");
	int ch;
	
	while (EOF != (ch = getc(infile)))
		//cout << "HERE" << endl;
		if ('\n' == ch)
			++lines;
	fclose(infile);
	//cout << lines << endl;
	return lines;
}

bool sign(int number)
{
	if (number >= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}



Max3Sat::Max3Sat()
{
	varies = 0;
	clauses = 0;
}

void Max3Sat::load_from_file(string filename, int varies)
{
	

	string zero;
	fstream handler;
	string holder;
	int lines = count_the_lines(filename);
	handler.open(filename.c_str());


	int a, b, c, ab;
	bool sign_a = true;
	int clauses = lines; 
	Clause_Table clause_table = Clause_Table();
	clause_table.clause_table_int.resize(clauses);
	clause_table.clause_table_bool.resize(clauses);
	for (int i = 0; i < clauses; i++)
	{
		clause_table.clause_table_int[i].resize(3);
		clause_table.clause_table_bool[i].resize(3);
		handler >> holder;

		for (int j = 0; j < 3; j++)
		{

			handler >> zero;

			if (zero == "-0")
			{
				a = 0;
				sign_a = false;
			}
			else if (zero == "0")
			{
				a = 0;
				sign_a = true;
			}
			else
			{
				a = stoi(zero);

				sign_a = sign(a);
				a = abs(a);
			}

			clause_table.clause_table_int[i][j] = a;
			clause_table.clause_table_bool[i][j] = sign_a;
	
		}

		handler >> holder;


	}

	this->varies = varies;
	this->clauses = lines;
	this->clause_table = clause_table;
}


void Max3Sat::load_from_generator_file(string filename, int varies)
{


	string zero;
	fstream handler;
	string holder;
	int lines = count_the_lines(filename);
	handler.open(filename.c_str());


	int a, b, c, ab;
	bool sign_a = true;
	int clauses = lines;
	Clause_Table clause_table = Clause_Table();
	clause_table.clause_table_int.resize(clauses);
	clause_table.clause_table_bool.resize(clauses);
	for (int i = 0; i < clauses; i++)
	{
		clause_table.clause_table_int[i].resize(3);
		clause_table.clause_table_bool[i].resize(3);
		//handler >> holder;

		for (int j = 0; j < 3; j++)
		{

			handler >> zero;
			a = stoi(zero);
			sign_a = sign(a);
			a = abs(a);
			

			clause_table.clause_table_int[i][j] = a-1;
			clause_table.clause_table_bool[i][j] = sign_a;

		}

		//handler >> holder;


	}

	this->varies = varies;
	this->clauses = lines;
	this->clause_table = clause_table;
}



void Max3Sat::load_from_wcnf_file(string filename)
{
	string zero;
	fstream handler;
	string holder;
	int lines = count_the_lines(filename);
	cout << lines << endl;
	handler.open(filename.c_str());
	int vars =-1;
	int clauses = -1;

	while (true)
	{
		handler >> holder;
		if (holder == "\"nvars\":")
		{
			handler >> holder;
			holder.pop_back();
			vars = stoi(holder);
		}

		if (holder == "\"ncls\":")
		{
			handler >> holder;
			holder.pop_back();
			clauses = stoi(holder);
		}

		if (vars != -1 && clauses != -1)
		{
			break;
		}
	}
	cout << "vars: " << vars<<endl;
	cout << "clauses: " << clauses<<endl;
	this->varies_og = vars;
	this->clauses_og = clauses;

	handler >> holder;
	string line = "";
	getline(handler, line);

	while (getline(handler, line))
	{
		if (line[0] != 'c')
		{

			break;
		}
		/*else
		{
			cout << line << endl;
		}*/
	}
	int_table_original.push_back(vector<int>());
	int temp_num;
	istringstream iss(line);
	while (iss >> temp_num)
	{
		if(temp_num !=0)
			int_table_original[0].push_back(temp_num);
	}



	for (int i = 1; i < clauses; i++)
	{
		int_table_original.push_back(vector<int>());

		handler >> holder;
		if (holder == "h")
		{
			handler >> holder;
		}

		while (holder != "0")
		{
			int_table_original[i].push_back(stoi(holder));
			handler >> holder;
		}

	}
	cout <<"size: "<< int_table_original.size()<<endl;

	/*for (int sth : int_table_original[clauses-1])
	{
		cout << sth << " ";
	}
	cout << endl;*/




}

int Max3Sat::get_varies()
{
	return this->varies;
}

int Max3Sat::get_clauses()
{
	return this->clauses;
}

Clause_Table Max3Sat::get_clause_table()
{
	return this->clause_table;
}

Clause_Table* Max3Sat::get_clause_table2()
{
	return &this->clause_table;
}

vector<vector<bool>> Max3Sat::get_bool_table_original()
{
	return this->bool_table_original;
}

vector<vector<int>> Max3Sat::get_int_table_original()
{
	return this->int_table_original;
}

int Max3Sat::get_varies_og()
{
	return this->varies_og;
}

int Max3Sat::get_clauses_og()
{
	return this->clauses_og;
}

void divide( vector<int> &clause,int &var_count, vector<vector<int>> &final_int_table)
{
	vector<int> clause1;
	vector<int> clause2;
	int size = clause.size();
	int half = -1;
	if (size % 2 == 0)
	{
		half = size / 2;
	}
	else
	{
		half = size / 2 + 1;
	}

	//divide part
	for (int i = 0; i < half; i++)
	{
		clause1.push_back(clause[i]);
	}
	for (int i = half; i < size; i++)
	{
		clause2.push_back(clause[i]);
	}

	//add new part
	clause1.push_back(var_count+1);
	clause2.push_back(-1*(var_count + 1));

	var_count++;

	//check for size
	if (clause1.size() > 3)
	{
		divide(clause1, var_count, final_int_table);
	}
	else
	{
		final_int_table.push_back(clause1);
	}

	if (clause2.size() > 3)
	{
		divide(clause2, var_count, final_int_table);
	}
	else
	{
		final_int_table.push_back(clause2);
	}

}


void Max3Sat::divide_into_3sat()
{
	vector<vector<int>> int_table_final;
	int var_count = varies_og;

	int count_2 = 0;
	int count_3 = 0;
	int count_4p = 0;


	for (vector<int> clause : int_table_original)
	{
		if (clause.size() == 2)
		{
			count_2++;
			vector<int> clause1 = clause;
			vector<int> clause2 = clause;

			clause1.push_back(var_count);
			clause2.push_back(-1 * var_count);
			var_count++;
			int_table_final.push_back(clause1);
			int_table_final.push_back(clause2);
		}
		else if (clause.size() == 3)
		{
			count_3++;
			int_table_final.push_back(clause);
		}
		else if (clause.size() > 3)
		{
			count_4p += clause.size() - 3;
			/*cout << "original clause: " << endl;
			for (int j : clause)
			{
				cout << j << " ";
			}
			cout << endl;*/

			divide(clause, var_count, int_table_final);
		}


	}

	/*cout << "new clauses: " << endl;
	for (vector<int> clause : final_int_table)
	{
		for (int j : clause)
		{
			cout << j << " ";
		}
		cout << endl;
	}*/

	/*cout << "count 2: " << count_2<<endl;
	cout << "count 3: " << count_3 << endl;
	cout << "count 4p: " << count_4p << endl;*/


	this->varies = var_count;
	this->clauses = int_table_final.size();
	cout << "final varies count: " << varies << endl;
	cout << "final clause count: " << clauses << endl;


	//divide into bool table and decrement by 1;


	Clause_Table clause_table = Clause_Table();
	clause_table.clause_table_int.resize(clauses);
	clause_table.clause_table_bool.resize(clauses);

	for (int i = 0; i < clauses; i++)
	{
		clause_table.clause_table_bool[i].resize(3);
		clause_table.clause_table_int[i].resize(3);
		for (int j = 0; j < 3; j++)
		{
			clause_table.clause_table_bool[i][j] = sign(int_table_final[i][j]);
			clause_table.clause_table_int[i][j] = abs(int_table_final[i][j]) - 1;
		}

	}

	this->clause_table = clause_table;





}
