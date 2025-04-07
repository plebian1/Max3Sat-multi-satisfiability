#include <iomanip>
#include <string>
#include <iostream>
#include <cmath>
#include <stdlib.h>     
#include <time.h>
#include <fstream>
#include <algorithm> 
#include <cstdio>
#include <windows.h>
#include <chrono>
#include <random>
#include "Clause_Table.h"
#pragma once

using namespace std;

class Max3Sat {
private:
	Clause_Table clause_table;
	int varies;
	int clauses;
	int varies_og;
	int clauses_og;

	vector<vector<bool>> bool_table_original;
	vector<vector<int>> int_table_original;



public:
	Max3Sat();
	void load_from_file(string filename, int varies);
	void load_from_generator_file(string filename, int varies);
	void load_from_wcnf_file(string filename);
	int get_varies();
	int get_clauses();
	Clause_Table get_clause_table();
	Clause_Table* get_clause_table2();

	//wcnf files
	vector<vector<bool>> get_bool_table_original();
	vector<vector<int>> get_int_table_original();
	int get_varies_og();
	int get_clauses_og();

	void divide_into_3sat();





};