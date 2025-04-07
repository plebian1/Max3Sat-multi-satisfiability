#include "Solver.h"

bool signum(int number)
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

int get_index(vector<int> v, int K)
{
	auto it = find(v.begin(), v.end(), K);
	int index = 0;
	// If element was found
	if (it != v.end())
	{

		// calculating the index
		// of K
		int index = it - v.begin();
		//cout << index << endl;
	}
	else {
		// If the element is not
		// present in the vector
		cout << "-1" << endl;
	}
	return index;
}

float average_over_vector(vector<int> &vec)
{
	int sum = 0;
	int vec_size = vec.size();
	for (int i = 0; i < vec_size; i++)
	{
		sum += vec[i];
	}
	float average = float(sum) / float(vec_size);
	return average;

}

int max_value_over_column(vector<vector<int>> array, int columnId, int rows)
{
	int max = 0;
	for (int i = 0; i < rows; i++)
	{

		if (array[i][columnId] > max)
		{
			max = array[i][columnId];
		}

	}
	return max;
}

int max_index_over_column_basic(vector<vector<int>> array, int columnId, int rows, int value)
{

	int index = 0;

	for (int i = 0; i < rows; i++)
	{

		if (array[i][columnId] == value)
		{
			index = i;
			break;
		}

	}
	return index;
}

int max_index_over_column(vector<vector<int>> array, int columnId, int rows, int value, int threshold)
{

	int index = -1;

	for (int i = 0; i < rows; i++)
	{

		if (array[i][columnId] == value && array[i][1] <= threshold)
		{
			index = i;
			break;
		}

	}

	if (index == -1 && value > 1)
	{
		value--;
		index = max_index_over_column(array, columnId, rows, value, threshold);
	}


	return index;
}

int smart_relaxation_random(vector<vector<int>>& relaxation_table, int cutoff, int no_clauses, int no_varies)
{

	// create vector with indexes of eligable variables, based on difference between columns 0 and 1

	//version where 1st create and resize vector instead of appending it
	int counter = 0;
	for (int i = 0; i < no_varies; i++)
	{

		if (relaxation_table[i][0] - relaxation_table[i][1] >= cutoff)
		{
			counter++;
		}

	}

	if (counter == 0)
	{
		return -1;
	}

	vector<int> eligable_indexes;
	eligable_indexes.resize(counter);
	counter = 0;
	for (int i = 0; i < no_varies; i++)
	{
		if (relaxation_table[i][0] - relaxation_table[i][1] >= cutoff)
		{
			eligable_indexes[counter] = i;

			counter++;
		}

	}

	int random = rand() % counter;
	return eligable_indexes[random];
}

int reverse_relaxation(vector<vector<int>>& relaxation_table, int no_clauses, int no_varies)
{

	// create vector with indexes of eligable variables, based on difference between columns 0 and 1

	//version where 1st create and resize vector instead of appending it
	int counter = 0;
	for (int i = 0; i < no_varies; i++)
	{

		if (relaxation_table[i][3] - relaxation_table[i][5] > 0)
		{
			counter++;
		}

	}

	if (counter == 0)
	{
		return -1;
	}


	vector<int> eligable_indexes;
	eligable_indexes.resize(counter);
	vector<int> eligable_diff;
	eligable_diff.resize(counter);
	counter = 0;
	for (int i = 0; i < no_varies; i++)
	{
		int diff = relaxation_table[i][3] - relaxation_table[i][5];
		if (diff > 0)
		{
			eligable_indexes[counter] = i;
			eligable_diff[counter] = diff;

			counter++;
		}

	}

	//highest result gives index for previous vector which gives index to return
	//if 1st created vector is empty return -1 marking failure

	int max = distance(eligable_diff.begin(), max_element(eligable_diff.begin(), eligable_diff.end()));
	int final_result = eligable_indexes[max];
	return final_result;
}

int smart_relaxation_version2(vector<vector<int>>& relaxation_table, int cutoff, int no_clauses, int no_varies)
{

	// create vector with indexes of eligable variables, based on difference between columns 0 and 1 as it's number of clauses fulfilled 

	//version where 1st create and resize vector instead of appending it
	int counter = 0;
	for (int i = 0; i < no_varies; i++)
	{

		if (relaxation_table[i][0] - relaxation_table[i][1] >= cutoff)
		{
			counter++;
		}

	}

	if (counter == 0)
	{
		return -1;
	}


	vector<int> eligable_indexes;
	eligable_indexes.resize(counter);
	vector<int> eligable_diff;
	eligable_diff.resize(counter);
	counter = 0;
	for (int i = 0; i < no_varies; i++)
	{
		if (relaxation_table[i][0] - relaxation_table[i][1] >= cutoff)
		{
			eligable_indexes[counter] = i;

			counter++;
		}

	}

	// create vector with indexes of eligable variables, based on sum  columns 1 and 5 as it's number of clauses fulfilled 1x and 2x times ->
	// wihtout need of division
	counter = 0;
	bool positive = false;
	for (int i : eligable_indexes)
	{

		int diff = relaxation_table[i][0] - relaxation_table[i][1] + (relaxation_table[i][2] - relaxation_table[i][4])/2 -  // change in 1x
				   ((relaxation_table[i][3] - relaxation_table[i][5])/2 + (relaxation_table[i][4] - relaxation_table[i][2]) );   // change in 2x
		eligable_diff[counter] = diff;

		/*int diff_3x = relaxation_table[i][5] - relaxation_table[i][3];
		eligable_diff[counter] = diff_3x;*/

		counter++;

	}

	//highest result gives index for previous vector which gives index to return
	//if 1st created vector is empty return -1 marking failure

	int max = distance(eligable_diff.begin(), max_element(eligable_diff.begin(), eligable_diff.end()));
	int final_result = eligable_indexes[max];
	/*int min = distance(eligable_diff.begin(), min_element(eligable_diff.begin(), eligable_diff.end()));
	int final_result = eligable_indexes[min];*/
	
	return final_result;
}

int smart_relaxation_version3(vector<vector<int>>& relaxation_table, int cutoff, int no_clauses, int no_varies, float limit, int value)
{

	// create vector with indexes of eligable variables, based on difference between columns 0 and 1 as it's number of clauses fulfilled 
	int minimal_accepted_value = floor(no_clauses * limit);
	//version where 1st create and resize vector instead of appending it
	int counter = 0;
	for (int i = 0; i < no_varies; i++)
	{

		if (relaxation_table[i][0] - relaxation_table[i][1] >= minimal_accepted_value - value)
		{
			counter++;
		}

	}

	if (counter == 0)
	{
		return -1;
	}


	vector<int> eligable_indexes;
	eligable_indexes.resize(counter);
	vector<int> eligable_diff;
	eligable_diff.resize(counter);
	counter = 0;
	for (int i = 0; i < no_varies; i++)
	{
		if (relaxation_table[i][0] - relaxation_table[i][1] >= minimal_accepted_value - value)
		{
			eligable_indexes[counter] = i;

			counter++;
		}

	}

	// create vector with indexes of eligable variables
	counter = 0;
	bool positive = false;
	for (int i : eligable_indexes)
	{

		int diff = relaxation_table[i][0] - relaxation_table[i][1] + (relaxation_table[i][2] - relaxation_table[i][4]) / 2 -  // change in 1x
			((relaxation_table[i][3] - relaxation_table[i][5]) / 2 + (relaxation_table[i][4] - relaxation_table[i][2]));   // change in 2x
		eligable_diff[counter] = diff;


		/*int diff_3x = relaxation_table[i][5] - relaxation_table[i][3];
		eligable_diff[counter] = diff_3x;*/

		counter++;

	}

	//highest result gives index for previous vector which gives index to return
	//if 1st created vector is empty return -1 marking failure

	int max = distance(eligable_diff.begin(), max_element(eligable_diff.begin(), eligable_diff.end()));
	int final_result = eligable_indexes[max];

	/*int min = distance(eligable_diff.begin(), min_element(eligable_diff.begin(), eligable_diff.end()));
	int final_result = eligable_indexes[min];*/
	return final_result;
}

int smart_relaxation_greedy(vector<vector<int>>& relaxation_table, int cutoff, int no_clauses, int no_varies)
{

	// create vector with indexes of eligable variables, based on difference between columns 0 and 1

	//version where 1st create and resize vector instead of appending it
	cutoff = 0;
	int counter = 0;
	int greedy_counter = 0;
	for (int i = 0; i < no_varies; i++)
	{

		if (relaxation_table[i][0] - relaxation_table[i][1] >= cutoff)
		{
			counter++;
		}
		if (relaxation_table[i][0] - relaxation_table[i][1] >= 1)
		{
			greedy_counter++;
		}

	}

	if (counter == 0)
	{
		return -1;
	}

	if (greedy_counter != 0)
	{
		counter = greedy_counter;
		cutoff = 1;
	}

	vector<int> eligable_indexes;
	eligable_indexes.resize(counter);
	vector<int> eligable_diff;
	eligable_diff.resize(counter);


	counter = 0;
	for (int i = 0; i < no_varies; i++)
	{
		if (relaxation_table[i][0] - relaxation_table[i][1] >= cutoff)
		{
			eligable_indexes[counter] = i;

			counter++;
		}

	}




	//count the differenece between columns 4 and 2 for each index in vector
	counter = 0;
	bool positive = false;
	for (int i : eligable_indexes)
	{

		int diff = relaxation_table[i][4] - relaxation_table[i][2];
		eligable_diff[counter] = diff;
		counter++;

	}

	//highest result gives index for previous vector which gives index to return
	//if 1st created vector is empty return -1 marking failure

	int max = distance(eligable_diff.begin(), max_element(eligable_diff.begin(), eligable_diff.end()));
	int max_result = *max_element(eligable_diff.begin(), eligable_diff.end());
	//cout << max_result << endl;
	int final_result = eligable_indexes[max];

	if (cutoff == 0 && max_result <= 0)  // if actions makes it worse, next step would most likely just reverse it and so on,
	{
		//cout << "HERE" << endl;
		return -1;
	}

	return final_result;
}

int smart_relaxation_restricted(vector<vector<int>> relaxation_table, int cutoff, int no_clauses, int no_varies, int restriction)
{

	// create vector with indexes of eligable variables, based on difference between columns 0 and 1

	//version where 1st create and resize vector instead of appending it
	int counter = 0;
	for (int i = 0; i < no_varies; i++)
	{

		if (relaxation_table[i][0] - relaxation_table[i][1] >= cutoff)
		{
			counter++;
		}

	}

	if (counter == 0)
	{
		return -1;
	}


	vector<int> eligable_indexes;
	eligable_indexes.resize(counter);
	vector<int> eligable_diff;
	eligable_diff.resize(counter);
	counter = 0;
	for (int i = 0; i < no_varies; i++)
	{
		if (relaxation_table[i][0] - relaxation_table[i][1] >= cutoff)
		{
			eligable_indexes[counter] = i;

			counter++;
		}

	}

	//count the differenece between columns 4 and 2 for each index in vector
	counter = 0;
	bool positive = false;
	for (int i : eligable_indexes)
	{

		int diff = relaxation_table[i][4] - relaxation_table[i][2];
		eligable_diff[counter] = diff;
		counter++;

	}

	//highest result gives index for previous vector which gives index to return
	//if 1st created vector is empty return -1 marking failure

	int max = distance(eligable_diff.begin(), max_element(eligable_diff.begin(), eligable_diff.end()));
	int final_result = eligable_indexes[max];
	return final_result;
}

void Solver::fihc(float fihc_propability, vector<bool>& solution, int& value, Ffe& ffe, mt19937_64 rng)
{
	/*int no_var = max3sat.get_varies();
	int no_clauses = max3sat.get_clauses();
	vector<vector<bool>> bool_table = max3sat.get_clause_table().clause_table_bool;
	vector<vector<int>> int_table = max3sat.get_clause_table().clause_table_int;*/

	vector<int> to_shuffle;
	for (int i = 0; i < no_var; i++)
	{
		to_shuffle.push_back(i);
	}
	float r = ((double)rand() / (RAND_MAX));

	if (r < fihc_propability) //fihc propability
	{
		shuffle(to_shuffle.begin(), to_shuffle.end(), rng);

		bool end_fihc = false;

		while (!end_fihc)
		{
			end_fihc = true;
			for (int i : to_shuffle)
			{
				solution[i] = !solution[i];
				int new_value = this->fast_fitness(value, i, solution);
				//int new_value = this->fitness(solution);
				//ffe.increment(4 * float(lut[i].size()) / no_clauses); // fast fitness is faster than normal fitness but it's faster based on instance size
				ffe.increment(1);
				if (new_value > value)
				{
					value = new_value;
					end_fihc = false;
				}
				else
				{
					solution[i] = !solution[i];
				}
			}
		}
	}
}

void Solver::fihc(float fihc_propability, vector<bool>& solution, int& value, Ffe& ffe, Ffe& ffe_partial, mt19937_64 rng)
{
	vector<int> to_shuffle;
	for (int i = 0; i < no_var; i++)
	{
		to_shuffle.push_back(i);
	}
	float r = ((double)rand() / (RAND_MAX));

	if (r < fihc_propability) //fihc propability
	{
		shuffle(to_shuffle.begin(), to_shuffle.end(), rng);

		bool end_fihc = false;

		while (!end_fihc)
		{
			end_fihc = true;
			for (int i : to_shuffle)
			{
				solution[i] = !solution[i];
				//int new_value = this->fast_fitness(value, i, solution);
				int new_value = this->fitness(solution);
				//ffe_partial.increment(2 * float(lut[i].size()) / no_clauses); // fast fitness is faster than normal fitness but it's faster based on instance size
				//ffe.increment(1);
				if (new_value > value)
				{
					value = new_value;
					end_fihc = false;
				}
				else
				{
					solution[i] = !solution[i];
				}
			}
		}
	}
}

void Solver::fihc_restricted(float fihc_propability, vector<bool>& solution, int& value, Ffe& ffe, mt19937_64 rng, int restricted)
{
	/*int no_var = max3sat.get_varies();
	int no_clauses = max3sat.get_clauses();
	vector<vector<bool>> bool_table = max3sat.get_clause_table().clause_table_bool;
	vector<vector<int>> int_table = max3sat.get_clause_table().clause_table_int;*/

	vector<int> to_shuffle;
	for (int i = 0; i < no_var; i++)
	{
		to_shuffle.push_back(i);
	}
	float r = ((double)rand() / (RAND_MAX));

	if (r < fihc_propability) //fihc propability
	{
		shuffle(to_shuffle.begin(), to_shuffle.end(), rng);

		bool end_fihc = false;

		while (!end_fihc)
		{
			end_fihc = true;
			for (int i : to_shuffle)
			{
				solution[i] = !solution[i];
				int new_value = this->fast_fitness(value, i, solution);

				ffe.increment(1); // fast fitness is faster than normal fitness but it's faster based on instance size
				if (new_value > value && this->check_single_clause(restricted, solution))
				{
					value = new_value;
					end_fihc = false;
				}
				else
				{
					solution[i] = !solution[i];
				}
			}
		}
	}
}

void Solver::fihc_relaxation_random(float fihc_propability, vector<bool>& solution, int& value, Ffe& ffe)
{
	
	vector<vector<int>> relaxation_table = this->create_fullfillment_tab(solution); //count fulfillment for given solution
	while (true)
	{

		int relaxation_cutoff = 1;
		int index_to_change = smart_relaxation_random(relaxation_table, relaxation_cutoff, no_clauses, no_var);

		if (index_to_change != -1)
		{
			//cout << index_to_change<<endl;
			solution[index_to_change] = !solution[index_to_change];
		}
		else
		{
			break;
		}
		this->fast_fullfillment(relaxation_table, solution, index_to_change);
	}
	value = this->fitness(solution);
}

void Solver::fihc_relaxation_version2(float fihc_propability, vector<bool>& solution, int& value, Ffe& ffe)
{
	vector<vector<int>> relaxation_table = this->create_fullfillment_tab(solution); //count fulfillment for given solution
	while (true)
	{
		
		int relaxation_cutoff = 1;
		int index_to_change = smart_relaxation_version2(relaxation_table, relaxation_cutoff, no_clauses, no_var);

		if (index_to_change != -1)
		{
			//cout << index_to_change<<endl;
			solution[index_to_change] = !solution[index_to_change];
		}
		else
		{
			break;
		}
		this->fast_fullfillment(relaxation_table, solution, index_to_change);
	}
	value = this->fitness(solution);
}

void Solver::fihc_relaxation_version3(float fihc_propability, vector<bool>& solution, int& value, Ffe& ffe, float limit)
{
	vector<vector<int>> relaxation_table = this->create_fullfillment_tab(solution); //count fulfillment for given solution
	while (true)
	{

		int relaxation_cutoff = 1;
		int index_to_change = smart_relaxation_version3(relaxation_table, relaxation_cutoff, no_clauses, no_var, limit, value);

		if (index_to_change != -1)
		{
			//cout << index_to_change<<endl;
			solution[index_to_change] = !solution[index_to_change];
		}
		else
		{
			break;
		}
		this->fast_fullfillment(relaxation_table, solution, index_to_change);
	}
	value = this->fitness(solution);
}

void Solver::fihc_relaxation(float fihc_propability, vector<bool>& solution, int& value, Ffe& ffe, Ffe& ffe_partial)
{
	
	while (true)
	{
		ffe_partial.increment(4);
		vector<vector<int>> relaxation_table = this->create_fullfillment_tab(solution); //count fulfillment for given solution
		int relaxation_cutoff = 1;
		int index_to_change = smart_relaxation_version2(relaxation_table, relaxation_cutoff, no_clauses, no_var);

		if (index_to_change != -1)
		{
			//cout << index_to_change<<endl;
			solution[index_to_change] = !solution[index_to_change];
		}
		else
		{
			break;
		}
	}
	value = this->fitness(solution);
	ffe.increment(1);
	ffe_partial.increment(1);


}

void Solver::fihc_relaxation_restricted(Max3Sat max3sat, float fihc_propability, vector<bool>& solution, int& value, Ffe& ffe, mt19937_64 rng, int restriction)
{
	int no_var = max3sat.get_varies();
	int no_clauses = max3sat.get_clauses();

	while (true)
	{
		vector<vector<int>> relaxation_table = this->create_fullfillment_tab( solution); //count fulfillment for given solution
		int relaxation_cutoff = 1;
		int index_to_change = smart_relaxation_restricted(relaxation_table, relaxation_cutoff, no_clauses, no_var, restriction);

		if (index_to_change != -1)
		{
			//cout << index_to_change<<endl;
			solution[index_to_change] = !solution[index_to_change];
		}
		else
		{
			break;
		}
	}
	value = this->fitness(solution);

}

void Solver::ils_random_my_rfihc(float fihc_propability, vector<bool>& solution, int& value, Ffe& ffe, mt19937_64 rng, float modifier)
{
	vector<bool> old_solution = solution;
	int old_value = value;



	
	int changed_count = 0;

	for (int i = 0; i < no_var; i++)
	{
		float r = ((double)rand() / (RAND_MAX));
		float upper_prob = (30.0 + (no_var / 5.0)) / no_var * modifier;
		//cout << "upper prob: " << upper_prob << endl;
		if (r < upper_prob)
		{
			solution[i] = uniform_int_distribution<>{ 0, 1 }(rng);
			changed_count++;
		}
	}

	//cout << changed_count << endl;
	//fihc

	this->fihc_relaxation_random(fihc_propability, solution, value, ffe);

	//keep changes or reverse

	if (value < old_value)
	{
		solution = old_solution;
		value = old_value;
	}
}

void Solver::ils1_my_rfihc(float fihc_propability, vector<bool>& solution, int& value, Ffe& ffe, mt19937_64 rng)
{
	vector<bool> old_solution = solution;
	int old_value = value;


	//random clause
	int single_clause = rand() % no_clauses;

	set<int> gene_list;

	// mask and randomizing gene
	for (int i : clause_connections_list[single_clause])
	{
		int a = int_table[i][0];
		int b = int_table[i][1];
		int c = int_table[i][2];
		gene_list.insert(a);
		gene_list.insert(b);
		gene_list.insert(c);
	}

	for (int i : gene_list)
	{
		solution[i] = uniform_int_distribution<>{ 0, 1 }(rng);
	}
	//fihc
	this->fihc_relaxation_random(fihc_propability, solution, value, ffe);

	//keep changes or reverse

	if (value < old_value)
	{
		solution = old_solution;
		value = old_value;
	}


}

void Solver::ils1_my_rfihc_sizable(float fihc_propability, vector<bool>& solution, int& value, Ffe& ffe, mt19937_64 rng, float ils_size)
{
	vector<bool> old_solution = solution;
	int old_value = value;

	set<int> gene_list;

	if (ils_size >= 1)
	{
		for (int i = 0; i<int(ils_size); i++)
		{
			//random clause
			int single_clause = rand() % no_clauses;


			// mask and randomizing gene
			for (int i : clause_connections_list[single_clause])
			{
				int a = int_table[i][0];
				int b = int_table[i][1];
				int c = int_table[i][2];
				gene_list.insert(a);
				gene_list.insert(b);
				gene_list.insert(c);
			}

		}
	}
	else if (ils_size < 1.0)
	{
		int single_clause = rand() % no_clauses;


		// mask and randomizing gene
		for (int i : clause_connections_list[single_clause])
		{
			int a = int_table[i][0];
			int b = int_table[i][1];
			int c = int_table[i][2];
			float r = ((double)rand() / (RAND_MAX));
			if(r < ils_size)
				gene_list.insert(a);
			r = ((double)rand() / (RAND_MAX));
			if (r < ils_size)
				gene_list.insert(b);
			r = ((double)rand() / (RAND_MAX));
			if (r < ils_size)
				gene_list.insert(c);
		}



	}

	//cout << gene_list.size()<<endl;

	for (int i : gene_list)
	{
		solution[i] = uniform_int_distribution<>{ 0, 1 }(rng);
	}
	//fihc
	this->fihc_relaxation_random(fihc_propability, solution, value, ffe);
	
	//keep changes or reverse

	if (value < old_value)
	{
		solution = old_solution;
		value = old_value;
	}
}

void Solver::ils1_my_directed_fihc_sizable(float fihc_propability, vector<bool>& solution, int& value, Ffe& ffe, mt19937_64 rng, float ils_size)
{
	vector<bool> old_solution = solution;
	int old_value = value;

	set<int> gene_list;

	if (ils_size >= 1)
	{
		for (int i = 0; i<int(ils_size); i++)
		{
			//random clause
			int single_clause = rand() % no_clauses;


			// mask and randomizing gene
			for (int i : clause_connections_list[single_clause])
			{
				int a = int_table[i][0];
				int b = int_table[i][1];
				int c = int_table[i][2];
				gene_list.insert(a);
				gene_list.insert(b);
				gene_list.insert(c);
			}

		}
	}
	else if (ils_size < 1.0)
	{
		int single_clause = rand() % no_clauses;


		// mask and randomizing gene
		for (int i : clause_connections_list[single_clause])
		{
			int a = int_table[i][0];
			int b = int_table[i][1];
			int c = int_table[i][2];
			float r = ((double)rand() / (RAND_MAX));
			if (r < ils_size)
				gene_list.insert(a);
			r = ((double)rand() / (RAND_MAX));
			if (r < ils_size)
				gene_list.insert(b);
			r = ((double)rand() / (RAND_MAX));
			if (r < ils_size)
				gene_list.insert(c);
		}



	}

	//cout << gene_list.size()<<endl;

	for (int i : gene_list)
	{
		solution[i] = uniform_int_distribution<>{ 0, 1 }(rng);
	}
	//fihc
	this->fihc_relaxation_version2(fihc_propability, solution, value, ffe);

	//keep changes or reverse

	if (value < old_value)
	{
		solution = old_solution;
		value = old_value;
	}
}

void Solver::ils1_my_directed_fihc_sizable_version2(float fihc_propability, vector<bool>& solution, int& value, Ffe& ffe, mt19937_64 rng, float ils_size, float limit)
{
	vector<bool> old_solution = solution;
	int old_value = value;

	set<int> gene_list;

	if (ils_size >= 1)
	{
		for (int i = 0; i<int(ils_size); i++)
		{
			//random clause
			int single_clause = rand() % no_clauses;


			// mask and randomizing gene
			for (int i : clause_connections_list[single_clause])
			{
				int a = int_table[i][0];
				int b = int_table[i][1];
				int c = int_table[i][2];
				gene_list.insert(a);
				gene_list.insert(b);
				gene_list.insert(c);
			}

		}
	}
	else if (ils_size < 1.0)
	{
		int single_clause = rand() % no_clauses;


		// mask and randomizing gene
		for (int i : clause_connections_list[single_clause])
		{
			int a = int_table[i][0];
			int b = int_table[i][1];
			int c = int_table[i][2];
			float r = ((double)rand() / (RAND_MAX));
			if (r < ils_size)
				gene_list.insert(a);
			r = ((double)rand() / (RAND_MAX));
			if (r < ils_size)
				gene_list.insert(b);
			r = ((double)rand() / (RAND_MAX));
			if (r < ils_size)
				gene_list.insert(c);
		}



	}

	//cout << gene_list.size()<<endl;

	for (int i : gene_list)
	{
		solution[i] = uniform_int_distribution<>{ 0, 1 }(rng);
	}
	//fihc
	this->fihc_relaxation_version3(fihc_propability, solution, value, ffe, limit);

	//keep changes or reverse

	if (value < old_value)
	{
		solution = old_solution;
		value = old_value;
	}
}

int Solver::count_backbone_overlap(vector<bool>& solution)
{
	int result = 0;

	for (int i = 0; i < no_var; i++)
	{
		if (backbone_list[i] != -1)
		{
			if (backbone_list[i] == 1 && solution[i] == true)
			{
				result += 1;
			}
			else if(backbone_list[i] == 0 && solution[i] == false)
			{
				result += 1;
			}
		}
	}

	return result;
}

int Solver::bf_mask_count(vector<bool>& solution, int& value, Ffe& ffe, mt19937_64 rng)
{
	int result = -1;

	vector<bool> old_solution = solution;
	int old_value = value;
	set<int> gene_list;

	unordered_set<int> unfulfilled_clauses = this->not_fulfilled_clauses(solution);
	set<int> genes_in_unfulfilled;
	vector<int> bruteforce_mask;
	set<int> bruteforce_mask_set;
	int min = 0;

	for (int i : unfulfilled_clauses)
	{
		genes_in_unfulfilled.insert(int_table[i][0]);
		genes_in_unfulfilled.insert(int_table[i][1]);
		genes_in_unfulfilled.insert(int_table[i][2]);
	}

	for (int i : unfulfilled_clauses)
	{
		for (int s = 0; s < 3; s++)
		{
			for (int j : vig_list[int_table[i][s]])
			{
				bruteforce_mask_set.insert(j);
			}
		}
	}
	for (int i : bruteforce_mask_set)
	{
		bruteforce_mask.push_back(i);
	}
	int bf_size = bruteforce_mask.size();



	//BEFORE REDUCTION
	value = bf_size;




	set<int> dependent_clauses;
	for (int i : bruteforce_mask)
	{
		for (int j : lut[i])
		{
			dependent_clauses.insert(j);
		}
	}
	vector<int> fulfillment = count_fullfillment(solution);
	//cutting
	while (true)
	{
		set<int> single_fulfilled_clauses_correct; // set of varies to delete from bruteforce mask
		for (int i : dependent_clauses)
		{
			if (fulfillment[i] == 1)
			{
				// get intersection from that clause varies and bruteforce mask
				vector<int> intersection;
				int position = -1;

				for (int j : bruteforce_mask)
				{
					if (int_table[i][0] == j)
					{
						intersection.push_back(j);
						position = 0;
					}
					if (int_table[i][1] == j)
					{
						intersection.push_back(j);
						position = 1;
					}
					if (int_table[i][2] == j)
					{
						intersection.push_back(j);
						position = 2;
					}


				}


				//check if that single clause is the one fulfilling it
				if (intersection.size() == 1)
				{
					int single_variable = intersection[0];

					if (bool_table[single_variable][position] == solution[single_variable])
					{
						single_fulfilled_clauses_correct.insert(single_variable);
					}
				}

			}
		}
		if (single_fulfilled_clauses_correct.size() == 0)
		{
			break;
		}

		for (int i : single_fulfilled_clauses_correct)
		{
			bruteforce_mask.erase(remove(bruteforce_mask.begin(), bruteforce_mask.end(), i), bruteforce_mask.end());
		}
		//new smaller clause list
		dependent_clauses.clear();

		for (int i : bruteforce_mask)
		{
			for (int j : lut[i])
			{
				dependent_clauses.insert(j);
			}
		}

	}

	bf_size = bruteforce_mask.size();


	return bf_size;
}

void Solver::ils_bf_mask_cut(vector<bool>& solution, int& value, Ffe& ffe, mt19937_64 rng)
{
	vector<bool> old_solution = solution;
	int old_value = value;
	set<int> gene_list;

	unordered_set<int> unfulfilled_clauses = this->not_fulfilled_clauses(solution);
	unordered_set<int> genes_in_unfulfilled;
	vector<int> bruteforce_mask;
	set<int> bruteforce_mask_set;
	int min = 0;

	for (int i : unfulfilled_clauses)
	{
		genes_in_unfulfilled.insert(int_table[i][0]);
		genes_in_unfulfilled.insert(int_table[i][1]);
		genes_in_unfulfilled.insert(int_table[i][2]);
	}

	for (int i : unfulfilled_clauses)
	{
		for (int s = 0; s < 3; s++)
		{
			for (int j : vig_list[int_table[i][s]])
			{
				bruteforce_mask_set.insert(j);
			}
		}
	}
	for (int i : bruteforce_mask_set)
	{
		bruteforce_mask.push_back(i);
	}
	int bf_size = bruteforce_mask.size();

	//extending base genes for bruteforce mask, should result in masks much harder to cut
	int added_base_genes = 5;
	for (int i = 0; i < added_base_genes; i++)
	{
		int rand_gene = rand() % bf_size;

		for (int j : vig_list[bruteforce_mask[rand_gene]])
		{
			bruteforce_mask_set.insert(j);
		}

	}
	bruteforce_mask.clear();
	for (int i : bruteforce_mask_set)
	{
		bruteforce_mask.push_back(i);
	}
	bf_size = bruteforce_mask.size();
	set<int> dependent_clauses;
	for (int i : bruteforce_mask)
	{
		for (int j : lut[i])
		{
			dependent_clauses.insert(j);
		}
	}

	vector<int> fulfillment = count_fullfillment(solution);
	//cutting
	while (true)
	{
		set<int> single_fulfilled_clauses_correct; // set of varies to delete from bruteforce mask
		for (int i : dependent_clauses)
		{
			if (fulfillment[i] == 1)
			{
				// get intersection from that clause varies and bruteforce mask
				vector<int> intersection;
				int position = -1;

				for (int j : bruteforce_mask)
				{
					if (int_table[i][0] == j)
					{
						intersection.push_back(j);
						position = 0;
					}
					if (int_table[i][1] == j)
					{
						intersection.push_back(j);
						position = 1;
					}
					if (int_table[i][2] == j)
					{
						intersection.push_back(j);
						position = 2;
					}


				}


				//check if that single clause is the one fulfilling it
				if (intersection.size() == 1)
				{
					int single_variable = intersection[0];

					if (bool_table[single_variable][position] == solution[single_variable])
					{
						single_fulfilled_clauses_correct.insert(single_variable);
					}
				}

			}
		}
		if (single_fulfilled_clauses_correct.size() == 0)
		{
			break;
		}

		for (int i : single_fulfilled_clauses_correct)
		{
			bruteforce_mask.erase(remove(bruteforce_mask.begin(), bruteforce_mask.end(), i), bruteforce_mask.end());
		}
		//new smaller clause list
		dependent_clauses.clear();

		for (int i : bruteforce_mask)
		{
			for (int j : lut[i])
			{
				dependent_clauses.insert(j);
			}
		}

	}




	bool abandon = true;
	for (int i : genes_in_unfulfilled)
	{
		for (int j : bruteforce_mask)
		{
			if (i == j)
			{
				abandon = false;
				break;
			}
		}

		if (abandon == false)
		{
			break;
		}

	}
	if (abandon)
	{
		return;
	}


	for (int tries = 0; tries < 10; tries++)
	{
		for (int i : bruteforce_mask)
		{
			solution[i] = uniform_int_distribution<>{ 0, 1 }(rng);
		}
		//fihc
		this->fihc_relaxation_random(1.0, solution, value, ffe);
		if (value == no_clauses)
		{
			break;
		}
		else
		{
			solution = old_solution;
			value = old_value;
		}


	}
	//keep changes or reverse

	/*if (value < old_value)
	{
		solution = old_solution;
		value = old_value;
	}*/


}

void Solver::ils1(float fihc_propability, vector<bool>& solution, int& value, Ffe& ffe, Ffe& ffe_partial, mt19937_64 rng)
{
	//vector<vector<int>> int_table = max3sat.get_clause_table().clause_table_int;

	vector<bool> old_solution = solution;
	int old_value = value;


	//random clause
	int single_clause = rand() % no_clauses;

	set<int> gene_list;
	
	// mask and randomizing gene
	for (int i : clause_connections_list[single_clause])
	{
		int a = int_table[i][0];
		int b = int_table[i][1];
		int c = int_table[i][2];
		gene_list.insert(a);
		gene_list.insert(b);
		gene_list.insert(c);
	}

	for (int i : gene_list)
	{
		solution[i] = uniform_int_distribution<>{ 0, 1 }(rng);
	}
	//fihc
	this->fihc_relaxation_version2(fihc_propability, solution, value, ffe);

	//keep changes or reverse

	if (value < old_value)
	{
		solution = old_solution;
		value = old_value;
	}

}

void Solver::ils1_std_fihc(float fihc_propability, vector<bool>& solution, int& value, Ffe& ffe, Ffe& ffe_partial ,mt19937_64 rng)
{
	vector<bool> old_solution = solution;
	int old_value = value;



	//random clause
	int single_clause = rand() % no_clauses;

	set<int> gene_list;

	// mask and randomizing gene
	for (int i : clause_connections_list[single_clause])
	{
		int a = int_table[i][0];
		int b = int_table[i][1];
		int c = int_table[i][2];
		gene_list.insert(a);
		gene_list.insert(b);
		gene_list.insert(c);
	}

	for (int i : gene_list)
	{
		solution[i] = uniform_int_distribution<>{ 0, 1 }(rng);
	}
	//fihc
	value = this->fitness(solution);
	ffe.increment(1);
	ffe_partial.increment(1);

	this->fihc(fihc_propability, solution, value, ffe, ffe_partial, rng);

	//keep changes or reverse
	//cout << "value: " << value << " old value: " << old_value << endl;
	if (value < old_value)
	{
		//cout << "HERE" << endl;
		solution = old_solution;
		value = old_value;
	}

}

bool Solver::check_single_clause(int clause_id, vector<bool>& solution)
{
	int a = int_table[clause_id][0];
	int b = int_table[clause_id][1];
	int c = int_table[clause_id][2];
	bool ret = false;
	if (solution[a] == bool_table[clause_id][0] || solution[b] == bool_table[clause_id][1] || solution[c] == bool_table[clause_id][2])
	{
		ret = true;
	}


	return ret;
}

void Solver::branch_and_bound_m3s(vector<bool>& solution, vector<int> bruteforce_mask, int& value, int max_possible_value, int deep)
{
	if (deep > 8) // limit to how deep can branch and bound go
	{
		//cout << "ABANDONED FROM DEPP, BRUTE FORCE MASK SIZE: " << bruteforce_mask.size() << endl;
		return;
	}

	set<int> dependent_clauses;
	for (int j : bruteforce_mask)
	{
		for (int c : lut[j])
		{
			dependent_clauses.insert(c);
		}
	}
	vector<int> fulfillment = count_fullfillment(solution);

	//cutting
	while (true)
	{
		set<int> single_fulfilled_clauses_correct; // set of varies to delete from bruteforce mask
		for (int i : dependent_clauses)
		{
			if (fulfillment[i] == 1)
			{
				// get intersection from that clause varies and bruteforce mask
				vector<int> intersection;
				int position = -1;

				for (int j : bruteforce_mask)
				{
					if (int_table[i][0] == j)
					{
						intersection.push_back(j);
						position = 0;
					}
					if (int_table[i][1] == j)
					{
						intersection.push_back(j);
						position = 1;
					}
					if (int_table[i][2] == j)
					{
						intersection.push_back(j);
						position = 2;
					
					}

				}
				//check if that single clause is the one fulfilling it
				if (intersection.size() == 1)
				{
					int single_variable = intersection[0];
					if (bool_table[single_variable][position] == solution[single_variable])
					{
						single_fulfilled_clauses_correct.insert(single_variable);
					}
				}


			}
		}
		if (single_fulfilled_clauses_correct.size() == 0)
		{
			break;
		}

		for (int i : single_fulfilled_clauses_correct)
		{
			bruteforce_mask.erase(remove(bruteforce_mask.begin(), bruteforce_mask.end(), i), bruteforce_mask.end());
		}
		//new smaller clause list
		dependent_clauses.clear();

		for (int i : bruteforce_mask)
		{
			for (int j : lut[i])
			{
				dependent_clauses.insert(j);
			}
		}

	}


	if (bruteforce_mask.size() > 10)
	{
		// B&B START HERE

		vector<int> bruteforce_mask_counter;
		bruteforce_mask_counter.resize(bruteforce_mask.size());

		for (int i = 0; i < bruteforce_mask_counter.size(); i++)
		{
			bruteforce_mask_counter[i] = 0;
		}


		for (int i : dependent_clauses)
		{
			int counter = 0;

			for (int j : bruteforce_mask)
			{
				if (int_table[i][0] == j)
				{
					bruteforce_mask_counter[counter]++;
				}
				if (int_table[i][1] == j)
				{
					bruteforce_mask_counter[counter]++;
				}
				if (int_table[i][2] == j)
				{
					bruteforce_mask_counter[counter]++;
				}

				counter++;
			}
		}

		int var_to_cut_ind = distance(bruteforce_mask_counter.begin(), max_element(bruteforce_mask_counter.begin(), bruteforce_mask_counter.end()));
		int var_to_cut = bruteforce_mask_counter[var_to_cut_ind];

		bruteforce_mask.erase(bruteforce_mask.begin() + var_to_cut_ind);
		vector<bool> new_solution = solution;

		this->branch_and_bound_m3s(solution, bruteforce_mask, value, max_possible_value, deep + 1);
		if (value == max_possible_value)
		{
			return;
		}

		// branch and bound z alternatywna wersja, if not maxed already
		new_solution[var_to_cut] = !new_solution[var_to_cut];

		this->branch_and_bound_m3s(new_solution, bruteforce_mask, value, max_possible_value, deep + 1);
		if (value == max_possible_value)
		{
			return;
		}
	}
	else if (bruteforce_mask.size() <= 10) // when mask is at max 5 finish with normal bruteforce at it's only 32 states to check
	{
		int min = bruteforce_mask.size();

		/*if(deep ==10)
			cout << "B&B BRUTE FORCE, MASK SIZE: " << min << endl;*/


		vector<bool> new_solution = solution; 
		int new_value = value;

		for (int i : bruteforce_mask)
		{
			new_solution[i] = 0;
		}
		new_value = this->fitness(new_solution);

		vector<vector<int>> relaxation_table = this->create_fullfillment_tab(new_solution);

		int table_size = pow(2, min);
		for (int i = 0; i < table_size - 1; i++)
		{

			int bit_to_change = gray_code_bit_flips[i];
			int index_to_change = bruteforce_mask[bit_to_change];
			new_solution[index_to_change] = !new_solution[index_to_change];

			int value_to_change = 0;
			value_to_change = relaxation_table[index_to_change][0] - relaxation_table[index_to_change][1];
			new_value += value_to_change;
			this->fast_fullfillment(relaxation_table, new_solution, index_to_change);

			if (new_value >= max_possible_value)
			{
				solution = new_solution;
				value = new_value;
				break;



			}
		}




	}


}

void Solver::brute_force(vector<bool>& solution, int& value, int max_possible_value, int& brute_force_count)
{
	// PART 1. choosing mask for brute force

	unordered_set<int> unfulfilled_clauses = this->not_fulfilled_clauses(solution);
	unordered_set<int> genes_in_unfulfilled;
	vector<int> bruteforce_mask;
	set<int> bruteforce_mask_set;
	int min = 0;

	//TRIPLE GEN DEPENDENCY

	for (int i : unfulfilled_clauses)
	{
		genes_in_unfulfilled.insert(int_table[i][0]);
		genes_in_unfulfilled.insert(int_table[i][1]);
		genes_in_unfulfilled.insert(int_table[i][2]);
	}


	for (int i : unfulfilled_clauses)
	{
		for (int s = 0; s < 3; s++)
		{
			for (int j : vig_list[int_table[i][s]])
			{
				bruteforce_mask_set.insert(j);
			}
		}
	}
	for (int i : bruteforce_mask_set)
	{
		bruteforce_mask.push_back(i);
	}

	int bf_size = bruteforce_mask.size();
	//cout << "BF SIZE INITIAL: " << bf_size << endl;;

	//extending base genes for bruteforce mask, should result in masks much harder to cut
	int added_base_genes = 0;
	for (int i = 0; i < added_base_genes; i++)
	{
		int rand_gene = rand() % bf_size;

		for (int j : vig_list[bruteforce_mask[rand_gene]])
		{
			bruteforce_mask_set.insert(j);
		}

	}
	bruteforce_mask.clear();
	for (int i : bruteforce_mask_set)
	{
		bruteforce_mask.push_back(i);
	}
	bf_size = bruteforce_mask.size();
	//cout << "BF SIZE EXTENDED: " << bf_size << endl;;





	set<int> dependent_clauses;
	for (int i : bruteforce_mask)
	{
		for (int j : lut[i])
		{
			dependent_clauses.insert(j);
		}
	}
		



	vector<int> fulfillment = count_fullfillment(solution);
	//int intersection_count = 0;
	//int correct_intersection_count = 0;
	//int sum_wrong = 0;
	//int cutting_iterator = 0;
	//cout << "MASK SIZE BEFORE CORRECTION: " << bruteforce_mask.size() << endl;

	//cutting
	while (true)
	{
		/*int intersection_count = 0;
		int intersection_count_2 = 0;
		int intersection_count_3 = 0;*/
		set<int> single_fulfilled_clauses_correct; // set of varies to delete from bruteforce mask
		for (int i : dependent_clauses)
		{
			if (fulfillment[i] == 1)
			{
				// get intersection from that clause varies and bruteforce mask
				vector<int> intersection;
				int position = -1;

				for (int j : bruteforce_mask)
				{
					//int add = 1;
					if (int_table[i][0] == j)
					{
						intersection.push_back(j);
						position = 0;
						//add = 0;
					}
					if (int_table[i][1] == j)
					{
						intersection.push_back(j);
						position = 1;
						//add = 0;
					}
					if (int_table[i][2] == j)
					{
						intersection.push_back(j);
						position = 2;
						//add = 0;
					}
					//sum_wrong += add;

				}


				//check if that single clause is the one fulfilling it
				if (intersection.size() == 1)
				{
					//intersection_count++;
					int single_variable = intersection[0];

					if (bool_table[single_variable][position] == solution[single_variable])
					{
						single_fulfilled_clauses_correct.insert(single_variable);
					}
					//correct_intersection_count++;
				}
				/*else if (intersection.size() == 2)
				{
					intersection_count_2++;

				}
				else if (intersection.size() == 3)
				{
					intersection_count_3++;
				}*/
				
				/*else if (intersection.size() != 1)
				{
					intersection_count++;
				}*/

				//single_fulfilled_clauses.push_back(i);

			}
		}
		if (single_fulfilled_clauses_correct.size() == 0)
		{
			//cout << "ENDER AFTER ITERATION: "<< cutting_iterator << endl<<endl;
			/*cout <<"LAST STEP 1 FULFILLED: "<< intersection_count << endl;
			cout <<"INTERSECTION 2:        " << intersection_count_2 << endl;
			cout <<"INTERSECTION 3:        " << intersection_count_3 << endl;*/
			break;
		}

		for (int i : single_fulfilled_clauses_correct)
		{
			bruteforce_mask.erase(remove(bruteforce_mask.begin(), bruteforce_mask.end(), i), bruteforce_mask.end());
		}

		//cout << "MASK SIZE AFTER CORRECTION: " << bruteforce_mask.size() << endl;

		//new smaller clause list
		dependent_clauses.clear();

		for (int i : bruteforce_mask)
		{
			for (int j : lut[i])
			{
				dependent_clauses.insert(j);
			}
		}

		//cutting_iterator++;

	}


	/*cout << "CORRECT INTERSECTION COUNT: " << correct_intersection_count << endl;
	cout << "INTERSECTION COUNT        : " << intersection_count << endl;
	cout << "WRONG FULFILLED       : " << sum_wrong << endl;
	cout << "DEPENDENT CLAUSES         : " << dependent_clauses.size() << endl;*/
	//cout<<"MASK SIZE AFTER CORRECTION: "<< bruteforce_mask.size() - single_fulfilled_clauses_correct.size() << endl;
	//cout <<endl<< "MASK SIZE AFTER CORRECTION: " << bruteforce_mask.size()<<endl;
	/*cout << "BRUTEFORCE MASK"<<endl;
	for (int i : bruteforce_mask)
	{
		cout << i << " ";
	}
	cout << endl;*/
	//cout << "min: " << min<<endl;


	bool abandon = true;
	for(int i : genes_in_unfulfilled)
	{
		for (int j : bruteforce_mask)
		{
			if (i == j)
			{
				abandon = false;
				break;
			}
		}

		if (abandon == false)
		{
			break;
		}
		
	}

	//cout << "CUTTED BRUTE FORCE MASK: " << bruteforce_mask.size() << endl;

	if (abandon)
	{
		//cout << "ABANDONED FROM MASK" << endl;
		return;
	}

	// B&B START HERE

	vector<int> bruteforce_mask_counter;
	bruteforce_mask_counter.resize(bruteforce_mask.size());

	for (int i = 0; i < bruteforce_mask_counter.size(); i++)
	{
		bruteforce_mask_counter[i] = 0;
	}


	for (int i : dependent_clauses)
	{
		int counter = 0;

		for (int j : bruteforce_mask)
		{
			if (int_table[i][0] == j)
			{
				bruteforce_mask_counter[counter]++;
			}
			if (int_table[i][1] == j)
			{
				bruteforce_mask_counter[counter]++;
			}
			if (int_table[i][2] == j)
			{
				bruteforce_mask_counter[counter]++;
			}

			counter++;
		}
	}

	int var_to_cut_ind = distance(bruteforce_mask_counter.begin(), max_element(bruteforce_mask_counter.begin(), bruteforce_mask_counter.end()));
	int var_to_cut = bruteforce_mask_counter[var_to_cut_ind];

	bruteforce_mask.erase(bruteforce_mask.begin() + var_to_cut_ind);
	vector<bool> new_solution = solution;
	int bnb_value = value;


	int deep = 0; //branch deep, max 10


	// branch and bound z wersja 1 usunietej zmiennej

	this->branch_and_bound_m3s(new_solution, bruteforce_mask, bnb_value, max_possible_value, deep + 1);
	if (bnb_value == max_possible_value)
	{
		solution = new_solution;
		value = bnb_value;
		return;
	}

	// branch and bound z alternatywna wersja, if not maxed already
	new_solution[var_to_cut] = !new_solution[var_to_cut];

	this->branch_and_bound_m3s(new_solution, bruteforce_mask, bnb_value, max_possible_value, deep + 1);
	if (bnb_value == max_possible_value)
	{
		solution = new_solution;
		value = bnb_value;
		return;
	}


	return; // i no longer need original bruteforce





	// PART 2. bruteforce itself, if the mask fits

	min = bruteforce_mask.size();
	if (min <= 20 && abandon ==false)  // 18 is arbitrary limit as it's bruteforce
	{
		//cout << "NOT ABANDONED  SIZE: " <<min<< endl;
		brute_force_count++;
		//vector<bool> new_solution = solution; //to narazie w sumie nie ma sensu specjalnego, ale wydaje mi sie ze bedzie mialo jak zrobie obcinanie B&B
int new_value = value; //tak jak powyzej

for (int i : bruteforce_mask)
{
	new_solution[i] = 0;
}
new_value = this->fitness(new_solution);

vector<vector<int>> relaxation_table = this->create_fullfillment_tab(new_solution); //count fulfillment for given solution
//this->fast_fullfillment(relaxation_table, solution, index_to_change);


int table_size = pow(2, min);
for (int i = 0; i < table_size - 1; i++)
{
	/*if (i % 1000 == 0)
	{
		cout << new_value << endl;
	}*/

	int bit_to_change = gray_code_bit_flips[i];
	int index_to_change = bruteforce_mask[bit_to_change];
	new_solution[index_to_change] = !new_solution[index_to_change];

	int value_to_change = 0;
	value_to_change = relaxation_table[index_to_change][0] - relaxation_table[index_to_change][1];
	new_value += value_to_change;
	this->fast_fullfillment(relaxation_table, new_solution, index_to_change);


	/*if (new_value >= value)
	{
		cout << i << ". " << new_value << endl;
	}*/


	if (new_value >= max_possible_value)
	{
		solution = new_solution;
		value = new_value;
		break;



	}
}




	}
	/*else if (abandon == true)
	{
		cout << "ABANDONED CUTS ";
		if (min > 18)
		{
			cout << " AND SIZE" << " SIZE: " << min;
		}
		cout << endl;
	}
	else if (min > 18)
	{
		cout << "ABANDONED SIZE " << " SIZE: " << min << endl;
	}
	*/


}

bool is_power_of_2(int n)
{
	if (n == 0)
		return false;

	return (ceil(log2(n)) == floor(log2(n)));
}

string vector_to_string(vector<bool>& vec)
{
	string result = "";
	for (bool i : vec)
	{
		if (i == true)
			result += '1';
		else if (i == false)
			result += '0';
	}
	return result;
}

vector<bool> string_to_vector(string s, int varies)
{
	vector<bool> result(varies,false);

	for (int i = 0; i < s.size(); i++)
	{
		if (s[i] == '1')
		{
			result[i] = true;
		}
	}


	return result;
}

void Solver::solutions_diagnosis(int process_id, string save_filename, int test_files_setoff, string alfa)
{

	int file_size = no_var;
	//string path = "..\\Max3Sat\\solution_files\\"+alfa+"\\" + to_string(file_size) + "\\results" + to_string(process_id + test_files_setoff) + ".txt";  //CORRECT BASIC
	//string path = "..\\Max3Sat\\solution_files\\pwlgen\\" + to_string(file_size) + "\\8" + "\\results" + to_string(process_id + test_files_setoff) + ".txt";
	string path = "..\\Max3Sat\\solution_files\\4.27_gen\\" + to_string(file_size) + "\\8\\results" + to_string(process_id + test_files_setoff) + ".txt";


	string zero;
	fstream handler;
	string holder;
	handler.open(path.c_str());
	handler >> holder; //holder holds the 1st solution
	vector<bool> diagnosed_solution;
	
	for (int i = 0; i < no_var; i++)
	{
		int a = holder[i];
		//cout << a << endl;
		if (a == 49)
		{
			diagnosed_solution.push_back(true);
		}
		else if (a == 48)
		{
			diagnosed_solution.push_back(false);
		}
	}

	//cout << this->fitness(diagnosed_solution) << endl;

	vector<vector<int>> final_relaxation_table = this->create_fullfillment_tab(diagnosed_solution);
	//vector<vector<bool>> used_tab;
	unordered_set<string> used_tab;

	vector<string> current_solutions_tab;
	vector<vector<vector<int>>> relaxation_tables;

	this->main_solution = diagnosed_solution;
	this->main_result = this->fitness(diagnosed_solution);

	used_tab.insert(vector_to_string(diagnosed_solution));

	//BFS
	//int relaxation_tab_iter = 0;
	//vector<int> gene_tab;
	//for (int v = 0; v < no_var; v++)
	//{
	//	gene_tab = final_relaxation_table[v];
	//	if ((gene_tab[1] == 0) || (gene_tab[1] - gene_tab[0] == 0))
	//	{
	//		//best_neighbours++;
	//		diagnosed_solution[relaxation_tab_iter] = !diagnosed_solution[relaxation_tab_iter];

	//		current_solutions_tab.push_back(vector_to_string(diagnosed_solution));
	//		used_tab.insert(vector_to_string(diagnosed_solution));

	//		this->fast_fullfillment(final_relaxation_table, diagnosed_solution, relaxation_tab_iter);
	//		relaxation_tables.push_back(final_relaxation_table);

	//		diagnosed_solution[relaxation_tab_iter] = !diagnosed_solution[relaxation_tab_iter];
	//		this->fast_fullfillment(final_relaxation_table, diagnosed_solution, relaxation_tab_iter);
	//	}

	//	relaxation_tab_iter++;
	//}

	//int current_start = 0;
	//int current_size = relaxation_tables.size();
	//cout << "START SIZE: " << current_size << endl;
	//while (true)
	//{
	//	int new_adds = 0;
	//	for (int i = current_start; i < current_size; i++)
	//	{
	//		int relaxation_tab_iter2 = 0;
	//		//vector<int> gene_tab;
	//		for (int v = 0; v < no_var; v++)
	//		{
	//			gene_tab = relaxation_tables[i][v];
	//			if ((gene_tab[1] == 0) || (gene_tab[1] - gene_tab[0] == 0))
	//			{
	//				//best_neighbours++;
	//				diagnosed_solution = string_to_vector(current_solutions_tab[i], no_var);
	//				diagnosed_solution[relaxation_tab_iter2] = !diagnosed_solution[relaxation_tab_iter2];
	//				string solution_string = vector_to_string(diagnosed_solution);

	//				if (used_tab.find(solution_string) != used_tab.end()) // jezeli jest juz na liscie
	//				{
	//					diagnosed_solution[relaxation_tab_iter2] = !diagnosed_solution[relaxation_tab_iter2];
	//				}
	//				else // jezeli nie ma jeszcze na liscie
	//				{
	//					current_solutions_tab.push_back(solution_string);
	//					used_tab.insert(solution_string);

	//					this->fast_fullfillment(relaxation_tables[i], diagnosed_solution, relaxation_tab_iter2);
	//					relaxation_tables.push_back(relaxation_tables[i]);

	//					diagnosed_solution[relaxation_tab_iter2] = !diagnosed_solution[relaxation_tab_iter2];
	//					this->fast_fullfillment(relaxation_tables[i], diagnosed_solution, relaxation_tab_iter2);
	//					new_adds++;
	//				}
	//			}
	//			relaxation_tab_iter2++;
	//		}
	//	}
	//	if (new_adds == 0 || used_tab.size() > 500000)
	//	{
	//		break;
	//	}
	//	current_start = current_size;
	//	current_size += new_adds;
	//	cout << "CURRENT SIZE: " << used_tab.size() << endl;
	//}



	//DFS
	

	int iterator = 0;
	int deep = 0;


	for (vector<int> gene_tab : final_relaxation_table)
	{
		if (gene_tab[1] == 1)
		{
			one_off_best_neighbours++;
		}
		else if ((gene_tab[1] == 0))
		{
			best_neighbours++;
			//cout << best_neighbours << " ";
		  dfs_plateau(used_tab, final_relaxation_table, diagnosed_solution, iterator, deep);

		}
		iterator++;
	}
	plateau_size = used_tab.size();
	cout << plateau_size << endl;

	int sum_x1 = 0,  sum_x2 = 0, sum_x3 = 0;
	int max_x1 = 0, max_x2 = 0, max_x3 = 0;
	int min_x1 = INT_MAX, min_x2 = INT_MAX, min_x3 = INT_MAX;
	int avg_x1, avg_x2, avg_x3;
	vector<int> over_fullfilled;
	// zliczenie kaluzul 1x, 2x, 3x
	for (string sol_string: used_tab)
	{
		vector<bool> sol = string_to_vector(sol_string,no_var);
		over_fullfilled = count_fullfillment(sol);
		int x1 = 0, x2 = 0 , x3 = 0;

		for (int i : over_fullfilled)
		{
			if (i == 1)
			{
				x1++;
			}
			else if (i == 2)
			{
				x2++;
			}
			else if (i == 3)
			{
				x3++;
			}
		}

		if (x1 > max_x1)
		{
			max_x1 = x1;
		}
		if (x2 > max_x2)
		{
			max_x2 = x2;
		}
		if (x3 > max_x3)
		{
			max_x3 = x3;
		}

		if (x1 < min_x1)
		{
			min_x1 = x1;
		}
		if (x2 < min_x2)
		{
			min_x2 = x2;
		}
		if (x3 < min_x3)
		{
			min_x3 = x3;
		}

		sum_x1 += x1;
		sum_x2 += x2;
		sum_x3 += x3;
	}
	avg_x1 = sum_x1 / plateau_size;
	avg_x2 = sum_x2 / plateau_size;
	avg_x3 = sum_x3 / plateau_size;


	backbone_list.resize(no_var);
	

	//backbone find

	vector<vector<bool>> used_tab_bool;
	used_tab_bool.resize(used_tab.size());
	int a = 0;
	for (string sol : used_tab)
	{
		used_tab_bool[a] = string_to_vector(sol, no_var);
		a++;
	}

	int backbone_count = 0;
	for (int j = 0; j < no_var; j++)
	{
		bool identical = true;
		bool value1 = used_tab_bool[0][j];
		for (int i = 1; i < plateau_size; i++)
		{
			if (used_tab_bool[i][j] != value1)
			{
				identical = false;
				break;
			}
		}
		if (identical)
		{
			backbone_count++;
			if (value1)
			{
				backbone_list[j] = 1;
			}
			else
			{
				backbone_list[j] = 0;
			}

		}
		else
		{
			backbone_list[j] = -1;
		}
	}
	backbone_size = backbone_count;
	

	//cout << "BACKBONE COUNT: " << backbone_count << "/"<<no_var<< endl;

	/*cout << "MAX 1X: " << max_x1 << " MIN 1X: " << min_x1 << endl;
	cout << "MAX 2X: " << max_x2 << " MIN 2X: " << min_x2 << endl;
	cout << "MAX 3X: " << max_x3 << " MIN 3X: " << min_x3 << endl;

	cout << "AVG 1X: " << avg_x1 << endl;
	cout << "AVG 2X: " << avg_x2 << endl;
	cout << "AVG 3X: " << avg_x3 << endl;*/


	/*ofstream outData;
	outData.open(save_filename);
	outData << "BACKBONE COUNT" << ";" 
		<< "MAX 1X" << ";" << "MIN 1X" << ";" << "AVG 1X" << ";"
		<< "MAX 2X" << ";" << "MIN 2X" << ";" << "AVG 2X" << ";"
		<< "MAX 3X" << ";" << "MIN 3X" << ";" << "AVG 3X" << ";"
		<< endl;
	outData << backbone_count << ";"
		<< max_x1 << ";" << min_x1 << ";" << avg_x1 << ";"
		<< max_x2 << ";" << min_x2 << ";" << avg_x2 << ";"
		<< max_x3 << ";" << min_x3 << ";" << avg_x3 << ";"
		<< endl;*/



}

void Solver::solutions_diagnosis_inside(vector<bool>& vec)
{

	int file_size = no_var;
	//string path = "..\\Max3Sat\\solution_files\\" + to_string(file_size) + "\\results" + to_string(process_id+test_files_setoff) + ".txt";
	
	
	vector<bool> diagnosed_solution = vec;

	//cout << this->fitness(diagnosed_solution) << endl;

	vector<vector<int>> final_relaxation_table = this->create_fullfillment_tab(diagnosed_solution);
	//vector<vector<bool>> used_tab;
	unordered_set<string> used_tab;

	used_tab.insert(vector_to_string(diagnosed_solution));
	int iterator = 0;
	int deep = 0;


	for (vector<int> gene_tab : final_relaxation_table)
	{
		if (gene_tab[1] == 1)
		{
			one_off_best_neighbours++;
		}
		else if ((gene_tab[1] == 0))
		{
			best_neighbours++;
			//cout << best_neighbours << " ";
			dfs_plateau(used_tab, final_relaxation_table, diagnosed_solution, iterator, deep);

		}
		iterator++;
	}
	plateau_size = used_tab.size();
	cout <<"PLATEAU SIZE: "<< plateau_size << endl;

	int sum_x1 = 0, sum_x2 = 0, sum_x3 = 0;
	int max_x1 = 0, max_x2 = 0, max_x3 = 0;
	int min_x1 = INT_MAX, min_x2 = INT_MAX, min_x3 = INT_MAX;
	int avg_x1, avg_x2, avg_x3;
	vector<int> over_fullfilled;
	// zliczenie kaluzul 1x, 2x, 3x
	for (string sol_string : used_tab)
	{
		vector<bool> sol = string_to_vector(sol_string, no_var);
		over_fullfilled = count_fullfillment(sol);
		int x1 = 0, x2 = 0, x3 = 0;

		for (int i : over_fullfilled)
		{
			if (i == 1)
			{
				x1++;
			}
			else if (i == 2)
			{
				x2++;
			}
			else if (i == 3)
			{
				x3++;
			}
		}

		if (x1 > max_x1)
		{
			max_x1 = x1;
		}
		if (x2 > max_x2)
		{
			max_x2 = x2;
		}
		if (x3 > max_x3)
		{
			max_x3 = x3;
		}

		if (x1 < min_x1)
		{
			min_x1 = x1;
		}
		if (x2 < min_x2)
		{
			min_x2 = x2;
		}
		if (x3 < min_x3)
		{
			min_x3 = x3;
		}

		sum_x1 += x1;
		sum_x2 += x2;
		sum_x3 += x3;
	}
	avg_x1 = sum_x1 / plateau_size;
	avg_x2 = sum_x2 / plateau_size;
	avg_x3 = sum_x3 / plateau_size;


	backbone_list.resize(no_var);


	//backbone find

	vector<vector<bool>> used_tab_bool;
	used_tab_bool.resize(used_tab.size());
	int a = 0;
	for (string sol : used_tab)
	{
		used_tab_bool[a] = string_to_vector(sol, no_var);
		a++;
	}

	int backbone_count = 0;
	int backbone_var_count = 0;
	for (int j = 0; j < no_var; j++)
	{
		bool identical = true;
		bool value1 = used_tab_bool[0][j];
		for (int i = 1; i < plateau_size; i++)
		{
			if (used_tab_bool[i][j] != value1)
			{
				identical = false;
				break;
			}
		}
		if (identical)
		{
			backbone_count += lut[j].size();
			backbone_var_count++;
			if (value1)
			{
				backbone_list[j] = 1;
			}
			else
			{
				backbone_list[j] = 0;
			}

		}
		else
		{
			backbone_list[j] = -1;
		}
	}
	backbone_size = backbone_var_count;


	cout << "BACKBONE VAR COUNT: " << backbone_var_count << "/" << no_var<< endl;
	cout << "BACKBONE CLAUSE COUNT: " << backbone_count << "/"<<no_clauses*3<< endl;

	/*cout << "MAX 1X: " << max_x1 << " MIN 1X: " << min_x1 << endl;
	cout << "MAX 2X: " << max_x2 << " MIN 2X: " << min_x2 << endl;
	cout << "MAX 3X: " << max_x3 << " MIN 3X: " << min_x3 << endl;

	cout << "AVG 1X: " << avg_x1 << endl;
	cout << "AVG 2X: " << avg_x2 << endl;
	cout << "AVG 3X: " << avg_x3 << endl;*/


	/*ofstream outData;
	outData.open(save_filename);
	outData << "BACKBONE COUNT" << ";"
		<< "MAX 1X" << ";" << "MIN 1X" << ";" << "AVG 1X" << ";"
		<< "MAX 2X" << ";" << "MIN 2X" << ";" << "AVG 2X" << ";"
		<< "MAX 3X" << ";" << "MIN 3X" << ";" << "AVG 3X" << ";"
		<< endl;
	outData << backbone_count << ";"
		<< max_x1 << ";" << min_x1 << ";" << avg_x1 << ";"
		<< max_x2 << ";" << min_x2 << ";" << avg_x2 << ";"
		<< max_x3 << ";" << min_x3 << ";" << avg_x3 << ";"
		<< endl;*/



}

void Solver::create_gray_bit_flips(int size)
{
	//gray_code_bit_flips.push_back(0);

	int table_size = pow(2, size);

	for (int i = 1; i < table_size; i++)
	{
		//cout << i << ". ";
		if (i % 2 == 1)
		{
			gray_code_bit_flips.push_back(0);
			//cout << 0;
		}
		else if (is_power_of_2(i))
		{
			int num = int(log2(i));
			gray_code_bit_flips.push_back(num);
			//cout << num;
		}
		else
		{
			int highest_lower_pow2 =pow(2, int(floor(log2(i))));

			int new_num = i % highest_lower_pow2;
			int to_save = gray_code_bit_flips[new_num-1];
			//cout <<to_save;
			gray_code_bit_flips.push_back(to_save);
		}


	}

	/*for (int i : gray_code_bit_flips)
	{
		cout << i << " ";
	}*/


}

//float average_over_column(vector<vector<int>> array, int columnId, int rows)
//{
//
//	float sum = 0;
//	int iter = 0;
//	for (int i = 0; i < rows; i++)
//	{
//		sum += array[i][columnId];
//		iter++;
//
//	}
//
//	return sum / float(iter);
//}

Solver::Solver()
{
}

Solver::Solver(Max3Sat max3sat)
{
	bool_table = max3sat.get_clause_table().clause_table_bool;
	int_table = max3sat.get_clause_table().clause_table_int;
	no_var = max3sat.get_varies();
	no_clauses = max3sat.get_clauses();

	bool_table_original = max3sat.get_bool_table_original();
	int_table_original = max3sat.get_int_table_original();
	no_var_og = max3sat.get_varies_og();
	no_clauses_og = max3sat.get_clauses_og();

}

void Solver::remove_clauses()
{
	vector<bool> solution = this->main_solution;
	vector<int> unsat_clauses;
	//int result = 0;
	for (int i = 0; i < no_clauses; i++)
	{

		int a = int_table[i][0];
		int b = int_table[i][1];
		int c = int_table[i][2];

		if (solution[a] == bool_table[i][0] || solution[b] == bool_table[i][1] || solution[c] == bool_table[i][2])
		{
			//cout << solution[a] <<" "<< solution[b]<< " " << solution[c] << endl;
			//cout << bool_table[i][0] << " " << bool_table[i][1] << " " << bool_table[i][2] << endl;
			//result++;
		}
		else
		{
			unsat_clauses.push_back(i);
		}
	}
	//cout << "RESULT BEFORE CUTS: " << result<< "/" <<no_clauses << endl;

	for (int i = 0; i < unsat_clauses.size(); i++)
	{
		int_table.erase(int_table.begin() + unsat_clauses[i] - i);
		bool_table.erase(bool_table.begin() + unsat_clauses[i] - i);
		no_clauses--;
	}

	//result = 0;
	for (int i = 0; i < no_clauses; i++)
	{

		int a = int_table[i][0];
		int b = int_table[i][1];
		int c = int_table[i][2];

		if (solution[a] == bool_table[i][0] || solution[b] == bool_table[i][1] || solution[c] == bool_table[i][2])
		{
			//cout << solution[a] <<" "<< solution[b]<< " " << solution[c] << endl;
			//cout << bool_table[i][0] << " " << bool_table[i][1] << " " << bool_table[i][2] << endl;
			//result++;
		}
	}
	//cout << "RESULT AFTER CUTS: " << result << "/" << no_clauses << endl;
	this->clear_lut();
	this->set_lut();
	this->create_simple_connections();
	this->create_connectivity();
	this->create_clause_connections();
	this->create_clause_connections_list();


}

void Solver::set_variable_count()
{
	/*int no_var = max3sat.get_varies();
	int no_clauses = max3sat.get_clauses();
	vector<vector<bool>> bool_table = max3sat.get_clause_table().clause_table_bool;
	vector<vector<int>> int_table = max3sat.get_clause_table().clause_table_int;*/


	this->variables_count.resize(no_var);
	this->main_solution.resize(no_var);

	for (int i = 0; i < no_var; i++)
	{
		this->variables_count[i].resize(4); // liczba wystapien, dodatnie, ujemne, diff 
		variables_count[i][0] = 0;
		variables_count[i][1] = 0;
		variables_count[i][2] = 0;
		variables_count[i][3] = 0;
	}

	// Zliczanie zmiennych w klauzulach

	for (int i = 0; i < no_clauses; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int index = int_table[i][j];
			variables_count[index][0] += 1;

			if (bool_table[i][j] == true)
			{
				variables_count[abs(index)][1] += 1;
			}
			else
			{
				variables_count[abs(index)][2] += 1;
			}


		}


	}


	for (int i = 0; i < no_var; i++)
	{
		variables_count[i][3] = abs(variables_count[i][1] - variables_count[i][2]);
	}

	//cout << "VARIABLE COUNT" << endl;
	//for (int i = 0; i < no_var; i++)
	//{
	//	cout <<i<<". " <<variables_count[i][0] << endl;
	//}

}

void Solver::clear_lut()
{
	for (int i = 0; i < no_var; i++)
	{
		lut[i].resize(0);
	}
}

void Solver::set_lut()
{
	/*int no_var = max3sat.get_varies();
	int no_clauses = max3sat.get_clauses();
	vector<vector<bool>> bool_table = max3sat.get_clause_table().clause_table_bool;
	vector<vector<int>> int_table = max3sat.get_clause_table().clause_table_int;*/

	for (int i = 0; i < no_clauses; i++)
	{

		for (int j = 0; j < 3; j++)
		{
			int index = int_table[i][j];
			lut[index].push_back(i);

		}
	}



}

int Solver::fitness_wcnf(vector<bool>& solution)
{
	int result = 0;
	for (vector<int> clause : int_table_original)
	{
		for (int i : clause)
		{
			if (solution[i - 1] == signum(i))
			{
				result++;
				break;
			}
		}
	}


	return result;
}

int Solver::fitness(vector<bool> &solution)
{
	/*int no_var = max3sat.get_varies();
	int no_clauses = max3sat.get_clauses();
	vector<vector<bool>> bool_table = max3sat.get_clause_table().clause_table_bool;
	vector<vector<int>> int_table = max3sat.get_clause_table().clause_table_int;*/

	/*for (int i = 0; i < no_clauses; i++)
	{
		cout<< int_table[i][0]<< " "<< int_table[i][1]<<" "<< int_table[i][2]<<" "<< bool_table[i][0] << " " << bool_table[i][1] << " " << bool_table[i][2]<<endl;
	}*/



	int result = 0;
	for (int i = 0; i < no_clauses; i++)
	{

		int a = int_table[i][0];
		int b = int_table[i][1];
		int c = int_table[i][2];

		if (solution[a] == bool_table[i][0] || solution[b] == bool_table[i][1] || solution[c] == bool_table[i][2])
		{
			//cout << solution[a] <<" "<< solution[b]<< " " << solution[c] << endl;
			//cout << bool_table[i][0] << " " << bool_table[i][1] << " " << bool_table[i][2] << endl;
			result++;
		}
	}
	return result;




}

int Solver::fast_fitness(int result, int change, vector<bool> &solution)
{

	int prev = 0;
	int current = 0;
	int diff = 0;

	//int no_var = max3sat.get_varies();
	//int no_clauses = max3sat.get_clauses();
	////vector<vector<bool>> bool_table = max3sat.get_clause_table().clause_table_bool;
	////vector<vector<int>> int_table = max3sat.get_clause_table().clause_table_int;

	// przechodzimy po wszystkich elementach w lut dla danej zmiennej, czyli po klauzulach w ktorych ona wystepuje

	for (int i : lut[change])
	{
		// i to numer klauzuli

		// sprawdz stan dla poprzedniego stanu i aktualnego, jezeli sie roznia dodaj lub odejmij 1 od diff


		int a = int_table[i][0];
		int b = int_table[i][1];
		int c = int_table[i][2];

		if (solution[a] == bool_table[i][0] || solution[b] == bool_table[i][1] || solution[c] == bool_table[i][2])
		{
			current++;
		}

		if (a == change)
		{
			bool_table[i][0] = !bool_table[i][0];

		}
		else if (b == change)
		{
			bool_table[i][1] = !bool_table[i][1];

		}
		else if (c == change)
		{
			bool_table[i][2] = !bool_table[i][2];

		}

		if (solution[a] == bool_table[i][0] || solution[b] == bool_table[i][1] || solution[c] == bool_table[i][2])
		{
			prev++;
		}

		// przywracanie
		if (a == change)
		{
			bool_table[i][0] = !bool_table[i][0];

		}
		else if (b == change)
		{
			bool_table[i][1] = !bool_table[i][1];

		}
		else if (c == change)
		{
			bool_table[i][2] = !bool_table[i][2];

		}




	}
	diff = current - prev;
	result = result + diff;

	return result;
}

unordered_set<int> Solver::not_fulfilled_clauses(vector<bool> solution)
{
	/*int no_var = max3sat.get_varies();
	int no_clauses = max3sat.get_clauses();
	vector<vector<bool>> bool_table = max3sat.get_clause_table().clause_table_bool;
	vector<vector<int>> int_table = max3sat.get_clause_table().clause_table_int;*/

	unordered_set<int> not_fulfilled;
	/*for (int i = 0; i < no_clauses; i++)
	{
		cout<< int_table[i][0]<< " "<< int_table[i][1]<<" "<< int_table[i][2]<<" "<< bool_table[i][0] << " " << bool_table[i][1] << " " << bool_table[i][2]<<endl;
	}*/

	for (int i = 0; i < no_clauses; i++)
	{

		int a = int_table[i][0];
		int b = int_table[i][1];
		int c = int_table[i][2];

		if (solution[a] == bool_table[i][0] || solution[b] == bool_table[i][1] || solution[c] == bool_table[i][2])
		{
			//cout << solution[a] <<" "<< solution[b]<< " " << solution[c] << endl;
			//cout << bool_table[i][0] << " " << bool_table[i][1] << " " << bool_table[i][2] << endl;
			
		}
		else
		{
			not_fulfilled.insert(i);
		}
	}



	return not_fulfilled;
}

void Solver::first_solution(Max3Sat max3sat, int cutoff)
{
	mt19937_64 rng(random_device{}());
	/*int no_var = max3sat.get_varies();
	int no_clauses = max3sat.get_clauses();
	vector<vector<bool>> bool_table = max3sat.get_clause_table().clause_table_bool;
	vector<vector<int>> int_table = max3sat.get_clause_table().clause_table_int;*/

	for (int i = 0; i < no_var; i++)
	{
		if (variables_count[i][1] >= variables_count[i][2] + cutoff)
		{
			main_solution[i] = true;
		}
		else if (variables_count[i][1] <= variables_count[i][2] - cutoff)
		{
			main_solution[i] = false;
		}
		else
		{
			main_solution[i] = uniform_int_distribution<>{ 0, 1 }(rng);
		}
	}

}

vector<int> Solver::count_fullfillment(vector<bool> solution)
{
	/*int no_var = max3sat.get_varies();
	int no_clauses = max3sat.get_clauses();
	vector<vector<bool>> bool_table = max3sat.get_clause_table().clause_table_bool;
	vector<vector<int>> int_table = max3sat.get_clause_table().clause_table_int;*/



	vector<int> over_fullfilled;
	over_fullfilled.resize(no_clauses);

	for (int clauseid = 0; clauseid < no_clauses; clauseid++)
	{
	
		int result = 0;
		int a = int_table[clauseid][0];
		int b = int_table[clauseid][1];
		int c = int_table[clauseid][2];



		if (solution[a] == bool_table[clauseid][0])
		{
			result++;
		}

		if (solution[b] == bool_table[clauseid][1])
		{
			result++;
		}

		if (solution[c] == bool_table[clauseid][2])
		{
			result++;

		}

		over_fullfilled[clauseid] = result;


	}

	return over_fullfilled;
}

vector<vector<int>> Solver::create_fullfillment_tab(vector<bool>& solution)
{

	
	vector<vector<int>> over_fulfilled_varies;
	/*int no_var = max3sat.get_varies();
	int no_clauses = max3sat.get_clauses();
	vector<vector<bool>> bool_table = max3sat.get_clause_table().clause_table_bool;
	vector<vector<int>> int_table = max3sat.get_clause_table().clause_table_int;*/

	vector<int> over_fullfilled;
	//over_fullfilled.resize(no_clauses);


	over_fullfilled = count_fullfillment( solution); // X ffe
	
	

	over_fulfilled_varies.resize(no_var);

	for (int i = 0; i < no_var; i++)
	{
		over_fulfilled_varies[i].resize(6);
		for (int j = 0; j < 6; j++)
		{
			over_fulfilled_varies[i][j] = 0;
		}
	}

	// struct description: [0]- 0 fulfilled clauses, [1] - fulfilled in 1 time fulfilled clauses, [2] - fulfilled in 2 times fulfilled clauses,
	// struct description cd: [3]- fulfilled in 3 times fulfilled clauses, [4] -  not fulfilled in 1 time fulfilled clauses, [5] - not fulfilled in 2 times fulfilled clauses,
	// after a change values moves like this: 0->1, 1->0, 2->4, 4->2, 3->5, 5->3

	for (int i = 0; i < no_clauses; i++) // 1 ffe
	{
		int a = int_table[i][0];
		int b = int_table[i][1];
		int c = int_table[i][2];

		if (over_fullfilled[i] == 1)
		{
			if (solution[a] == bool_table[i][0])
			{
				over_fulfilled_varies[a][1] += 1;
				over_fulfilled_varies[b][4] += 1;
				over_fulfilled_varies[c][4] += 1;
			}
			else if (solution[b] == bool_table[i][1])
			{
				over_fulfilled_varies[a][4] += 1;
				over_fulfilled_varies[b][1] += 1;
				over_fulfilled_varies[c][4] += 1;
			}
			else if (solution[c] == bool_table[i][2])
			{
				over_fulfilled_varies[a][4] += 1;
				over_fulfilled_varies[b][4] += 1;
				over_fulfilled_varies[c][1] += 1;
			}
		}
		else if (over_fullfilled[i] == 2)
		{
			if (solution[a] == bool_table[i][0] && solution[b] == bool_table[i][1])
			{
				over_fulfilled_varies[a][2] += 1;
				over_fulfilled_varies[b][2] += 1;
				over_fulfilled_varies[c][5] += 1;
			}
			else if (solution[b] == bool_table[i][1] && solution[c] == bool_table[i][2])
			{
				over_fulfilled_varies[a][5] += 1;
				over_fulfilled_varies[b][2] += 1;
				over_fulfilled_varies[c][2] += 1;
			}
			else if (solution[a] == bool_table[i][0] && solution[c] == bool_table[i][2])
			{
				over_fulfilled_varies[a][2] += 1;
				over_fulfilled_varies[b][5] += 1;
				over_fulfilled_varies[c][2] += 1;
			}
		}
		else if (over_fullfilled[i] == 3)
		{
			over_fulfilled_varies[a][3] += 1;
			over_fulfilled_varies[b][3] += 1;
			over_fulfilled_varies[c][3] += 1;
		}
		else if (over_fullfilled[i] == 0)
		{
			over_fulfilled_varies[a][0] += 1;
			over_fulfilled_varies[b][0] += 1;
			over_fulfilled_varies[c][0] += 1;
		}
	}

	/*for (int i = 0; i < no_var; i++)
	{
		cout << i<<" ";
		for (int j : over_fulfilled_varies[i])
		{
			cout << j << " ";
		}
		cout << endl;
	}*/



	return over_fulfilled_varies;
}

int Solver::encode_clause(vector<bool>& solution, int clause_id)
{
	int a = int_table[clause_id][0];
	int b = int_table[clause_id][1];
	int c = int_table[clause_id][2];

	int result = 0;

	if (solution[a] == bool_table[clause_id][0])
	{
		result += 4;
	}
	if (solution[b] == bool_table[clause_id][1])
	{
		result += 2;

	}

	if (solution[c] == bool_table[clause_id][2])
	{
		result += 1;

	}

	if (result == 3)
	{
		result = 4;
	}
	else if (result == 4)
	{
		result = 3;
	}

	return result;
}

void Solver::fast_fullfillment( vector<vector<int>>& old_tab, vector<bool> solution, int change) // accepts new solution
{

	vector<int> varies_to_change = this->vig_list[change];
	vector<int> changed_clauses = lut[change];

	for (int clause_id : changed_clauses)
	{
		solution[change] = !solution[change];
		int old_encode = this->encode_clause(solution, clause_id);
		solution[change] = !solution[change];
		int new_encode = this->encode_clause(solution, clause_id);

		int a = int_table[clause_id][0];
		int b = int_table[clause_id][1];
		int c = int_table[clause_id][2];
		//cout << "OLD ENCODE = " << old_encode << " NEW ENCODE = " << new_encode << endl;

		if (old_encode == 7)
		{
			old_tab[a][3] -= 1;
			old_tab[b][3] -= 1;
			old_tab[c][3] -= 1;

			if (new_encode == 6)
			{
				old_tab[a][2] += 1;
				old_tab[b][2] += 1;
				old_tab[c][5] += 1;

			}
			else if (new_encode == 5)
			{
				old_tab[a][2] += 1;
				old_tab[b][5] += 1;
				old_tab[c][2] += 1;

			}
			else if (new_encode == 4)
			{
				old_tab[a][5] += 1;
				old_tab[b][2] += 1;
				old_tab[c][2] += 1;
			}

		}
		else if (old_encode == 6)
		{
			if (new_encode == 7)
			{
				old_tab[a][3] += 1;
				old_tab[b][3] += 1;
				old_tab[c][3] += 1;

				old_tab[a][2] -= 1;
				old_tab[b][2] -= 1;
				old_tab[c][5] -= 1;

			}
			else if (new_encode == 3)
			{
				old_tab[a][2] -= 1;
				old_tab[a][1] += 1;

				old_tab[b][2] -= 1;
				old_tab[b][4] += 1;

				old_tab[c][5] -= 1;
				old_tab[c][4] += 1;
			}
			else if (new_encode == 2)
			{
				old_tab[a][2] -= 1;
				old_tab[a][4] += 1;

				old_tab[b][2] -= 1;
				old_tab[b][1] += 1;
				
				old_tab[c][5] -= 1;
				old_tab[c][4] += 1;
			}

		}
		else if (old_encode == 5)
		{
			if (new_encode == 7)
			{
				old_tab[a][3] += 1;
				old_tab[b][3] += 1;
				old_tab[c][3] += 1;

				old_tab[a][2] -= 1;
				old_tab[b][5] -= 1;
				old_tab[c][2] -= 1;

			}
			else if (new_encode == 3)
			{
				old_tab[a][2] -= 1;
				old_tab[a][1] += 1;

				old_tab[b][5] -= 1;
				old_tab[b][4] += 1;
				
				old_tab[c][2] -= 1;
				old_tab[c][4] += 1;
			}
			else if (new_encode == 1)
			{
				old_tab[a][2] -= 1;
				old_tab[a][4] += 1;

				old_tab[b][5] -= 1;
				old_tab[b][4] += 1;
				
				old_tab[c][2] -= 1;
				old_tab[c][1] += 1;
			}

		}
		else if (old_encode == 4)
		{
			if (new_encode == 7)
			{
				old_tab[a][3] += 1;
				old_tab[b][3] += 1;
				old_tab[c][3] += 1;

				old_tab[a][5] -= 1;
				old_tab[b][2] -= 1;
				old_tab[c][2] -= 1;

			}
			else if (new_encode == 2)
			{
				old_tab[a][5] -= 1;
				old_tab[a][4] += 1;

				old_tab[b][2] -= 1;
				old_tab[b][1] += 1;
				
				old_tab[c][2] -= 1;
				old_tab[c][4] += 1;
			}
			else if (new_encode == 1)
			{
				old_tab[a][5] -= 1;
				old_tab[a][4] += 1;
				
				old_tab[b][2] -= 1;
				old_tab[b][4] += 1;
				
				old_tab[c][2] -= 1;
				old_tab[c][1] += 1;
			}

		}
		else if (old_encode == 3)
		{
			if (new_encode == 0)
			{
				old_tab[a][0] += 1;
				old_tab[b][0] += 1;
				old_tab[c][0] += 1;

				old_tab[a][1] -= 1;
				old_tab[b][4] -= 1;
				old_tab[c][4] -= 1;

			}
			else if (new_encode == 5)
			{
				old_tab[a][1] -= 1;
				old_tab[a][2] += 1;
				old_tab[b][4] -= 1;
				old_tab[b][5] += 1;
				old_tab[c][4] -= 1;
				old_tab[c][2] += 1;
			}
			else if (new_encode == 6)
			{
				old_tab[a][1] -= 1;
				old_tab[a][2] += 1;
				old_tab[b][4] -= 1;
				old_tab[b][2] += 1;
				old_tab[c][4] -= 1;
				old_tab[c][5] += 1;
			}

		}
		else if (old_encode == 2)
		{
			if (new_encode == 0)
			{
				old_tab[a][0] += 1;
				old_tab[b][0] += 1;
				old_tab[c][0] += 1;

				old_tab[a][4] -= 1;
				old_tab[b][1] -= 1;
				old_tab[c][4] -= 1;

			}
			else if (new_encode == 6)
			{
				old_tab[a][4] -= 1;
				old_tab[a][2] += 1;
				old_tab[b][1] -= 1;
				old_tab[b][2] += 1;
				old_tab[c][4] -= 1;
				old_tab[c][5] += 1;
			}
			else if (new_encode == 4)
			{
				old_tab[a][4] -= 1;
				old_tab[a][5] += 1;
				old_tab[b][1] -= 1;
				old_tab[b][2] += 1;
				old_tab[c][4] -= 1;
				old_tab[c][2] += 1;
			}

		}
		else if (old_encode == 1)
		{
			if (new_encode == 0)
			{
				old_tab[a][0] += 1;
				old_tab[b][0] += 1;
				old_tab[c][0] += 1;

				old_tab[a][4] -= 1;
				old_tab[b][4] -= 1;
				old_tab[c][1] -= 1;

			}
			else if (new_encode == 5)
			{
				old_tab[a][4] -= 1;
				old_tab[a][2] += 1;
				old_tab[b][4] -= 1;
				old_tab[b][5] += 1;
				old_tab[c][1] -= 1;
				old_tab[c][2] += 1;
			}
			else if (new_encode == 4)
			{
				old_tab[a][4] -= 1;
				old_tab[a][5] += 1;
				old_tab[b][4] -= 1;
				old_tab[b][2] += 1;
				old_tab[c][1] -= 1;
				old_tab[c][2] += 1;
			}

		}
		else if (old_encode == 0)
		{
			old_tab[a][0] -= 1;
			old_tab[b][0] -= 1;
			old_tab[c][0] -= 1;

			if (new_encode == 3)
			{

				old_tab[a][1] += 1;
				old_tab[b][4] += 1;
				old_tab[c][4] += 1;

			}
			else if (new_encode == 2)
			{
				old_tab[a][4] += 1;
				old_tab[b][1] += 1;
				old_tab[c][4] += 1;
			}
			else if (new_encode == 1)
			{
				old_tab[a][4] += 1;
				old_tab[b][4] += 1;
				old_tab[c][1] += 1;
			}

		}

	}
	




}

void Solver::create_simple_connections()
{

	/*int no_var = max3sat.get_varies();
	int no_clauses = max3sat.get_clauses();
	vector<vector<bool>> bool_table = max3sat.get_clause_table().clause_table_bool;
	vector<vector<int>> int_table = max3sat.get_clause_table().clause_table_int;*/
	simple_connections.resize(0);
	simple_connections_list.resize(0);
	vig_list.resize(0);
	simple_connections.resize(no_var);
	simple_connections_list.resize(no_var);
	vig_list.resize(no_var);
	for (int i = 0; i < no_var; i++)
	{
		simple_connections[i].resize(no_var);
		simple_connections_list[i].resize(no_var);
	}

	int iter = 0;
	for (vector<int> i : int_table)
	{

		int a = i[0];
		int b = i[1];
		int c = i[2];

		simple_connections[a][b] += 1;
		simple_connections[a][c] += 1;
		simple_connections[b][c] += 1;
		simple_connections[c][b] += 1;
		simple_connections[c][a] += 1;
		simple_connections[b][a] += 1;


		simple_connections_list[a][b].push_back(iter);
		simple_connections_list[a][c].push_back(iter);
		simple_connections_list[b][c].push_back(iter);
		simple_connections_list[c][b].push_back(iter);
		simple_connections_list[c][a].push_back(iter);
		simple_connections_list[b][a].push_back(iter);
		iter++;
	}
	for (int i = 0; i < no_var; i++)
	{
		for (int j = 0; j < no_var; j++)
		{
			if (simple_connections[i][j] >= 1)
			{
				vig_list[i].push_back(j);
			}
		}
	}


	// print
	/*cout << "   ";
	for (int j = 0; j < no_var; j++)
	{
		cout << setw(2)<< j<<" ";
	}

	for (int i = 0; i < no_var; i++)
	{
		cout << endl;
		cout << setw(2) << i << " ";
		for (int j = 0; j < no_var; j++)
		{
			cout << setw(2) << simple_connections[i][j] << " ";
		}
	}*/


}

void Solver::create_simple_connections_bool()
{
	/*int no_var = max3sat.get_varies();
	int no_clauses = max3sat.get_clauses();
	vector<vector<bool>> bool_table = max3sat.get_clause_table().clause_table_bool;
	vector<vector<int>> int_table = max3sat.get_clause_table().clause_table_int;*/
	simple_connections_bool.resize(0);
	simple_connections_bool.resize(no_var);
	for (int i = 0; i < no_var; i++)
	{
		simple_connections_bool[i].resize(no_var);
	}

	for (vector<int> i : int_table)
	{
		int a = i[0];
		int b = i[1];
		int c = i[2];

		simple_connections_bool[a][b] = true;
		simple_connections_bool[a][c] = true;
		simple_connections_bool[b][c] = true;
		simple_connections_bool[c][b] = true;
		simple_connections_bool[c][a] = true;
		simple_connections_bool[b][a] = true;
	}


	// print

	/*for (int j = 0; j < no_var; j++)
	{
		cout << j << " ";
	}

	for (int i = 0; i < no_var; i++)
	{
		cout << endl;
		cout << i << " ";
		for (int j = 0; j < no_var; j++)
		{
			cout << simple_connections_bool[i][j] << " ";
		}
	}*/
}

void Solver::create_connectivity()
{

	/*int no_var = max3sat.get_varies();
	int no_clauses = max3sat.get_clauses();
	vector<vector<bool>> bool_table = max3sat.get_clause_table().clause_table_bool;
	vector<vector<int>> int_table = max3sat.get_clause_table().clause_table_int;*/
	connectivity.resize(0);
	connectivity.resize(no_var);
	for (int i = 0; i < no_var; i++)
	{
		connectivity[i] = 0;
		for (int j = 0; j < no_var; j++)
		{
			connectivity[i] += simple_connections[i][j];
		}
	}

	//print

	/*for (int i = 0; i < no_var; i++)
	{
		cout << i << ". " << connectivity[i] << endl;
	}*/

}

void Solver::create_clause_connections()
{
	/*int no_var = max3sat.get_varies();
	int no_clauses = max3sat.get_clauses();
	vector<vector<bool>> bool_table = max3sat.get_clause_table().clause_table_bool;
	vector<vector<int>> int_table = max3sat.get_clause_table().clause_table_int;*/

	//init
	this->clause_connections.resize(0);
	this->clause_connections.resize(no_clauses);
	for (int i = 0; i < no_clauses; i++)
	{
		this->clause_connections[i].resize(no_clauses);
		for (int j = 0; j < no_clauses; j++)
		{
			clause_connections[i][j] = 0;
		}
	}


	for (int i = 0; i < no_clauses; i++)
	{
		int a = int_table[i][0];
		int b = int_table[i][1];
		int c = int_table[i][2];

		for (int j : lut[a])
		{
			clause_connections[i][j] += 1;
		}
		for (int j : lut[b])
		{
			clause_connections[i][j] += 1;
		}
		for (int j : lut[c])
		{
			clause_connections[i][j] += 1;
		}

	}
	//print

	/*for (int i = 0; i < no_clauses; i++)
	{
		cout << endl;
		for (int j = 0; j < no_clauses; j++)
		{
			 cout<<clause_connections[i][j]<<" ";
		}
	}*/


}

void Solver::create_clause_connections_list()
{
	/*int no_var = max3sat.get_varies();
	int no_clauses = max3sat.get_clauses();*/
	clause_connections_list.resize(0);
	clause_connections_list.resize(no_clauses);

	for (int i = 0; i < no_clauses; i++)
	{
		for (int j =0; j< no_clauses; j++ )
		{
			int val = clause_connections[i][j];
			if (val != 0)
			{
				clause_connections_list[i].insert(j);
			}
		}
	}

	/*for (set<int> sth : clause_connections_list)
	{
		cout << sth.size();
		cout << endl;
		for (int i : sth)
		{
			cout << i << " ";
		}
		cout << endl;
		
	}*/


}

void Solver::create_clause_dep_con()
{
	/*int no_var = max3sat.get_varies();
	int no_clauses = max3sat.get_clauses();
	vector<vector<bool>> bool_table = max3sat.get_clause_table().clause_table_bool;
	vector<vector<int>> int_table = max3sat.get_clause_table().clause_table_int;*/
	clause_dependence.resize(no_clauses);
	//clause_connectivity.resize(no_clauses);

	// dependence
	for (int i = 0; i < no_clauses; i++)
	{
		clause_dependence[i] = 0;
		int a = int_table[i][0];
		int b = int_table[i][1];
		int c = int_table[i][2];

		clause_dependence[i] += variables_count[a][0];
		clause_dependence[i] += variables_count[b][0];
		clause_dependence[i] += variables_count[c][0];
	}



}

void Solver::create_clause_list_dependend_genes()
{
	for (int v = 0; v < no_var; v++)
	{
		set<int> dependent_clauses;
		vector<int> varies_list = vig_list[v];
		for (int i : varies_list)
		{
			for (int j : lut[i])
			{
				dependent_clauses.insert(j);
			}
		}
		this->clause_list_of_dependent_genes.push_back(dependent_clauses);

	}


	
}

void Solver::dfs_plateau(unordered_set<string>& used_tab, vector<vector<int>>& fulfillment_tab, vector<bool>& solution, int change, int& deep)
{
	if (used_tab.size() > 1000000)
	{
		return;
	}

	deep++;
	if (deep > 3000) // STACK OVERFLOW XDDDD
	{
		deep--;
		return;
	}

	/*if (deep < 200)
	{
		cout << "DEEP " << deep << endl;
	}*/
	solution[change] = !solution[change];

	/*int fit = this->fitness(solution);
	if (fit != 640)
	{
		cout << fit;
	}*/

	bool end_path = false;

	//check if this was already checked
	string solution_string = vector_to_string(solution);
	if (used_tab.find(solution_string)  != used_tab.end())
	{
		end_path = true;
	}

	/*for (vector<bool> i : used_tab)
	{
		if (solution == i)
		{
			end_path = true;
			break;
		}
	
	}*/

	if (end_path)
	{
		//cout << "HERE END PATH!" << endl;
		deep--;
		solution[change] = !solution[change];
		return;
	}

	used_tab.insert(solution_string);


	this->fast_fullfillment(fulfillment_tab, solution, change);
	vector<int> plateau_neighbours;

	int iter = 0;
	for (vector<int> i : fulfillment_tab)
	{
		if (i[1] - i[0] == 0)
		{
			plateau_neighbours.push_back(iter);
		}
		iter++;
	}
	iter = 0;
	//cout << "neigh count: " << plateau_neighbours.size() << endl;
	for (int i : plateau_neighbours)
	{
		//if (iter != 0)
		//{
		//	//cout << "Neighbours: " << iter << endl;
		//}
		dfs_plateau(used_tab, fulfillment_tab, solution, i, deep);
		iter++;
	}



	//cout << "HERE END FUNCTION" << endl;
	deep--;
	solution[change] = !solution[change];
	this->fast_fullfillment(fulfillment_tab, solution, change);
	return;

}

vector<bool> Solver::get_main_solution()
{
	return this->main_solution;
}

int Solver::get_main_result()
{
	return this->main_result;
}

vector<int> Solver::get_best_results()
{

	return this->best_results;
}

vector<int> Solver::get_worst_results()
{
	return this->worst_results;
}

int Solver::get_brute_force_count()
{
	return this->brute_force_count;
}

int Solver::get_one_offs_count()
{
	return this->one_off_solutions;
}

int Solver::get_best_neighbours()
{
	return this->best_neighbours;
}

int Solver::get_best_one_off_neighbours()
{
	return this->one_off_best_neighbours;
}

bool Solver::get_brute_force_improved()
{
	return this->brute_force_improved;
}

bool Solver::get_ILS_BF_improved()
{
	return this->ILS_BF_improved;
}

double Solver::get_ils_time()
{
	return this->ils_time;
}

double Solver::get_px_time()
{
	return this->px_time;
}

double Solver::get_bf_time()
{
	return this->brute_force_time;
}

double Solver::get_ils_bf_mask_time()
{
	return this->ils_bf_mask_time;
}

double Solver::get_best_time()
{
	return this->best_time;
}

double Solver::get_full_time()
{
	return this->full_time;
}

int Solver::get_plateau_size()
{
	return this->plateau_size;
}

int Solver::get_iterations()
{
	return this->iterations;
}

int Solver::get_full_ffe()
{
	return this->full_ffe;
}

float Solver::get_partial_ffe()
{
	return this->partial_ffe;
}

vector<float> Solver::get_tail_var_avg()
{
	return this->tail_var_avg;
}

vector<float> Solver::get_tail_var_avg_no_cuts()
{
	return this->tail_var_avg_no_cuts;
}

vector<float> Solver::get_relaxation_sum_avg_1x()
{
	return relaxation_sum_1x_avg;
}

vector<float> Solver::get_relaxation_sum_avg_2x()
{
	return relaxation_sum_2x_avg;
}

vector<float> Solver::get_relaxation_sum_avg_3x()
{
	return relaxation_sum_3x_avg;
}

vector<int> Solver::get_count_for_backbone()
{
	return this->count_for_backbone;
}

vector<int> Solver::get_count_for_backbone_one_off()
{
	return this->count_for_backbone_one_off;
}

vector<int> Solver::get_count_for_backbone_two_off()
{
	return this->count_for_backbone_two_off;
}

vector<int> Solver::get_count_for_backbone_perfect_solutions()
{
	return this->count_for_backbone_perfect_solutions;
}

vector<int> Solver::get_backbone_1x_min()
{
	return this->relaxation_1x_min;
}

vector<int> Solver::get_backbone_1x_max()
{
	return this->relaxation_1x_max;
}

vector<int> Solver::get_backbone_2x_min()
{
	return this->relaxation_2x_min;
}

vector<int> Solver::get_backbone_2x_max()
{
	return this->relaxation_2x_max;
}

vector<int> Solver::get_backbone_3x_min()
{
	return this->relaxation_3x_min;
}

vector<int> Solver::get_backbone_3x_max()
{
	return this->relaxation_3x_max;
}

vector<int> Solver::get_reverse_backbone_1x_min()
{
	return this->reverse_relaxation_1x_min;
}

vector<int> Solver::get_reverse_backbone_1x_max()
{
	return this->reverse_relaxation_1x_max;
}

vector<int> Solver::get_reverse_backbone_2x_min()
{
	return this->reverse_relaxation_2x_min;
}

vector<int> Solver::get_reverse_backbone_2x_max()
{
	return this->reverse_relaxation_2x_max;
}

vector<int> Solver::get_reverse_backbone_3x_min()
{
	return this->reverse_relaxation_3x_min;
}

vector<int> Solver::get_reverse_backbone_3x_max()
{
	return this->reverse_relaxation_3x_max;
}

vector<float> Solver::get_reverse_backbone_1x_avg()
{
	return this->reverse_backbone_1x_avg;
}

vector<float> Solver::get_reverse_backbone_2x_avg()
{
	return this->reverse_backbone_2x_avg;
}

vector<float> Solver::get_reverse_backbone_3x_avg()
{
	return this->reverse_backbone_3x_avg;
}

int Solver::get_backbone_size()
{
	return this->backbone_size;
}

int Solver::blackbox_VIG(int process_id)
{
	mt19937_64 rng(random_device{}());
	vector<vector<int>> max_VIG; // tworzone VIG
	max_VIG.resize(no_var);
	for (int i = 0; i < no_var; i++)
	{
		max_VIG[i].resize(no_var);
		for (int j = 0; j < no_var; j++)
		{
			max_VIG[i][j] = 0;
		}

	}

	vector<vector<bool>> used_solutions;

	for (int iterations = 0; iterations < 200; iterations++)
	{
		vector<bool> new_pop;
		new_pop.resize(no_var);
		for (int j = 0; j < no_var; j++)
		{
			new_pop[j] = uniform_int_distribution<>{ 0, 1 }(rng);
		}

		




		vector<bool> base_solution = new_pop;
		int base_value = this->fitness(new_pop);


		vector<vector<int>> partial_VIG; // tworzone VIG

		partial_VIG.resize(no_var);
		for (int i = 0; i < no_var; i++)
		{
			partial_VIG[i].resize(no_var);
			for (int j = 0; j < no_var; j++)
			{
				partial_VIG[i][j] = 0;
			}
		}





		vector<int> single_swap_values;
		single_swap_values.resize(no_var);
		//wartosci pojedynczych zmian
		for (int i = 0; i < no_var; i++)
		{
			new_pop[i] = !new_pop[i];
			single_swap_values[i] = this->fitness(new_pop);
			new_pop[i] = !new_pop[i];
		}

		// sprawdzanie wszystkich par


		for (int i = 0; i < no_var - 1; i++)
		{
			new_pop[i] = !new_pop[i];
			for (int j = i + 1; j < no_var; j++)
			{
				new_pop[j] = !new_pop[j];
				int combined_value = this->fitness(new_pop);
				int diff1, diff2, diff_combined, diff_final;
				diff1 = single_swap_values[i] - base_value;
				diff2 = single_swap_values[j] - base_value;
				diff_combined = combined_value - base_value;
				diff_final = abs(diff1 + diff2 - diff_combined);
				if (diff_final != 0)
				{
					partial_VIG[i][j] += diff_final;
					partial_VIG[j][i] += diff_final;
				}
				new_pop[j] = !new_pop[j];
			}
			new_pop[i] = !new_pop[i];
		}


		// flipowanie wszystkisch bitow i powtarzenie oprecji

		new_pop = base_solution;

		for (int i = 0; i < no_var; i++)
		{
			new_pop[i] = !new_pop[i];
		}

		base_value = this->fitness(new_pop);

		for (int i = 0; i < no_var; i++)
		{
			new_pop[i] = !new_pop[i];
			single_swap_values[i] = this->fitness(new_pop);
			new_pop[i] = !new_pop[i];
		}


		for (int i = 0; i < no_var - 1; i++)
		{
			new_pop[i] = !new_pop[i];
			for (int j = i + 1; j < no_var; j++)
			{
				new_pop[j] = !new_pop[j];
				int combined_value = this->fitness(new_pop);
				int diff1, diff2, diff_combined, diff_final;
				diff1 = single_swap_values[i] - base_value;
				diff2 = single_swap_values[j] - base_value;
				diff_combined = combined_value - base_value;
				diff_final = abs(diff1 + diff2 - diff_combined);
				if (diff_final != 0)
				{
					partial_VIG[i][j] += diff_final;
					partial_VIG[j][i] += diff_final;
				}
				new_pop[j] = !new_pop[j];
			}
			new_pop[i] = !new_pop[i];
		}



		for (int i = 0; i < no_var; i++)
		{
			for (int j = 0; j < no_var; j++)
			{
				max_VIG[i][j] = max(max_VIG[i][j], partial_VIG[i][j]);
			}

		}



		int sum_vig = 0;

		for (int i = 0; i < no_var; i++)
		{
			for (int j = 0; j < no_var; j++)
			{
				sum_vig += max_VIG[i][j];
			}

		}


		/*cout << "ITERATION: " << iterations << endl;
		cout << "SUMA WSZYSTKIEGO W VIG: " << sum_vig / 2 << endl;
		cout << "BRAKUJE: " << no_clauses * 3 - sum_vig / 2 << endl;
		cout << "% VIG " << float((sum_vig / 2)) / float((no_clauses * 3)) << " %" << endl;
		cout << endl;*/


		if ((no_clauses * 3 - sum_vig/2) == 0)
		{
			//cout << "ITERATION: " << iterations << endl;
			return iterations;
		}
		

		/*vector<vector<int>> diff_vig;

		diff_vig.resize(no_var);
		for (int i = 0; i < no_var; i++)
		{
			diff_vig[i].resize(no_var);
			for (int j = 0; j < no_var; j++)
			{
				diff_vig[i][j] = simple_connections[i][j] - max_VIG[i][j];
				if (diff_vig[i][j] != 0)
				{
					cout << i << " " << j << endl;
					cout << "ORIGINAL VALUE: "<< simple_connections[i][j] << " DIFF: "<< diff_vig[i][j]<<endl;
				}
			}


		}*/

	}

	return -1;
}

int Solver::blackbox_VIG_deterministic(int process_id)
{
	mt19937_64 rng(random_device{}());
	vector<vector<int>> max_VIG; // tworzone VIG
	max_VIG.resize(no_var);
	for (int i = 0; i < no_var; i++)
	{
		max_VIG[i].resize(no_var);
		for (int j = 0; j < no_var; j++)
		{
			max_VIG[i][j] = 0;
		}

	}

	int base2 = ceil(log2(no_var)) + 1;
	vector<vector<bool>> detecting_solutions;
	detecting_solutions.resize(base2);

	vector<bool> solution0;
	solution0.resize(no_var);
	for (int i = 0; i < no_var; i++)
	{
		solution0[i] = 0;
	}
	detecting_solutions[0] = solution0;

	vector<bool> current_solution;
	for (int i = 1; i < base2; i++)
	{
		int interval = pow(2, i);
		int half = interval / 2;
		current_solution = detecting_solutions[i - 1];

		for (int j = 0; j < no_var; j++)
		{
			int remainder = j % interval;

			if (remainder >= half)
			{
				current_solution[j] = !current_solution[j];
			}

			//cout << current_solution[j];
		}
		//cout << endl;
		detecting_solutions[i] = current_solution;

	}


	int missing = -1;


	for (int iterations = 0; iterations < base2; iterations++)
	{
		vector<bool> new_pop;

		new_pop = detecting_solutions[iterations];

		vector<bool> base_solution = new_pop;
		int base_value = this->fitness(new_pop);
		vector<vector<int>> partial_VIG; // tworzone VIG
		partial_VIG.resize(no_var);
		for (int i = 0; i < no_var; i++)
		{
			partial_VIG[i].resize(no_var);
			for (int j = 0; j < no_var; j++)
			{
				partial_VIG[i][j] = 0;
			}
		}

		vector<int> single_swap_values;
		single_swap_values.resize(no_var);
		//wartosci pojedynczych zmian
		for (int i = 0; i < no_var; i++)
		{
			new_pop[i] = !new_pop[i];
			single_swap_values[i] = this->fitness(new_pop);
			new_pop[i] = !new_pop[i];
		}

		// sprawdzanie wszystkich par


		for (int i = 0; i < no_var - 1; i++)
		{
			new_pop[i] = !new_pop[i];
			for (int j = i + 1; j < no_var; j++)
			{
				new_pop[j] = !new_pop[j];
				int combined_value = this->fitness(new_pop);
				int diff1, diff2, diff_combined, diff_final;
				diff1 = single_swap_values[i] - base_value;
				diff2 = single_swap_values[j] - base_value;
				diff_combined = combined_value - base_value;
				diff_final = abs(diff1 + diff2 - diff_combined);
				if (diff_final != 0)
				{
					partial_VIG[i][j] += diff_final;
					partial_VIG[j][i] += diff_final;
				}
				new_pop[j] = !new_pop[j];
			}
			new_pop[i] = !new_pop[i];
		}


		// flipowanie wszystkisch bitow i powtarzenie oprecji

		new_pop = base_solution;

		for (int i = 0; i < no_var; i++)
		{
			new_pop[i] = !new_pop[i];
		}

		base_value = this->fitness(new_pop);

		for (int i = 0; i < no_var; i++)
		{
			new_pop[i] = !new_pop[i];
			single_swap_values[i] = this->fitness(new_pop);
			new_pop[i] = !new_pop[i];
		}


		for (int i = 0; i < no_var - 1; i++)
		{
			new_pop[i] = !new_pop[i];
			for (int j = i + 1; j < no_var; j++)
			{
				new_pop[j] = !new_pop[j];
				int combined_value = this->fitness(new_pop);
				int diff1, diff2, diff_combined, diff_final;
				diff1 = single_swap_values[i] - base_value;
				diff2 = single_swap_values[j] - base_value;
				diff_combined = combined_value - base_value;
				diff_final = abs(diff1 + diff2 - diff_combined);
				if (diff_final != 0)
				{
					partial_VIG[i][j] += diff_final;
					partial_VIG[j][i] += diff_final;
				}
				new_pop[j] = !new_pop[j];
			}
			new_pop[i] = !new_pop[i];
		}



		for (int i = 0; i < no_var; i++)
		{
			for (int j = 0; j < no_var; j++)
			{
				max_VIG[i][j] = max(max_VIG[i][j], partial_VIG[i][j]);
			}

		}



		int sum_vig = 0;

		for (int i = 0; i < no_var; i++)
		{
			for (int j = 0; j < no_var; j++)
			{
				sum_vig += max_VIG[i][j];
			}

		}

		if ((no_clauses * 3 - sum_vig / 2) == 0)
		{
			//cout << "ITERATION: " << iterations << endl;
			return -1;
		}


		missing = no_clauses * 3 - sum_vig / 2;

		cout << "ITERATION: " << iterations << endl;
		cout << "SUMA WSZYSTKIEGO W VIG: " << sum_vig / 2 << endl;
		cout << "BRAKUJE: " << no_clauses * 3 - sum_vig / 2 << endl;
		cout << "% VIG " << float((sum_vig / 2)) / float((no_clauses * 3)) << " %" << endl;
		cout << endl;


		

		vector<vector<int>> diff_vig;

		diff_vig.resize(no_var);
		for (int i = 0; i < no_var; i++)
		{
			diff_vig[i].resize(no_var);
			for (int j = 0; j < no_var; j++)
			{
				diff_vig[i][j] = simple_connections[i][j] - max_VIG[i][j];
				if (diff_vig[i][j] != 0)
				{
					cout << i << " " << j << endl;
					cout << "ORIGINAL VALUE: "<< simple_connections[i][j] << " DIFF: "<< diff_vig[i][j]<<endl;
				}
			}


		}

	}


	return missing;



}

vector<int> vectors_overlap(vector<int> &vector1, vector<int> &vector2)
{
	vector<int> overlap;

	for (int i : vector1)
	{
		for (int j : vector2)
		{
			if (i == j)
			{
				overlap.push_back(i);
				break;
			}
		}
	}
	return overlap;
}

bool vector_contains_int(int num, vector<int> &vec)
{
	for (int i : vec)
	{
		if (i == num)
		{
			return true;
		}
	}
	return false;
}

void Solver::vig_into_clauses(int process_id)
{
	mt19937_64 rng(random_device{}());
	vector<vector<int>> unsigned_clauses;
	vector<vector<int>> vig_copy = simple_connections;
	vector<vector<int>> vig_list_copy = vig_list;
	vector<int> banned_list;
	//banned_list.push_back(25);

	
	int clause_pool = 0;

	vector<int> dependency_count; // z iloma zmiennymi jest powi¹zane
	dependency_count.resize(no_var);
	bool main_loop_break = true;

	while (true)
	{
		main_loop_break = true;
		banned_list.clear();
		//cout << "BANNED LIST RESET" << endl;
		for (int sth = 0; sth < no_var; sth++)
		{
			int min_val = no_var, min_ind = -1;

			for (int i = 0; i < no_var; i++)
			{
				dependency_count[i] = 0;
				for (int j = 0; j < no_var; j++)
				{

					if (vig_copy[i][j] != 0)
					{
						dependency_count[i]++;
					}
				}
				if (dependency_count[i] < min_val && min_val != 0 && vector_contains_int(i, banned_list) == false)
				{
					min_val = dependency_count[i];
					min_ind = i;
				}

			}
			/*if (min_val != 0)
			{
				cout << "Najmniej zaleznosci ma zmienna: " << min_ind << endl;
				cout << "Jest to " << min_val << endl;
				clause_pool += min_val;
				for (int i : vig_list[min_ind])
				{
					if(vig_copy[min_ind][i] !=0)
						cout << i << " ";
				}
				cout << endl;
				for (int i : vig_list[min_ind])
				{
					if (vig_copy[min_ind][i] !=0)
						cout << vig_copy[min_ind][i] << " ";
				}

				cout << endl;
			}*/




			bool to_break = true;
			bool one_more_iter =  false;
			while (true)
			{
				/*cout << "ANOTHER ITER" << endl;

				for (int i : vig_list_copy[min_ind])
				{
					cout << i << " ";
				}
				cout << endl;
				for (int i : vig_list_copy[min_ind])
				{
					cout << vig_copy[min_ind][i] << " ";
				}
				cout << endl;*/

				/*int iter = 0;
				for (int i : vig_copy[min_ind])
				{
					if (i > 0)
						cout << iter << " ";
					iter++;
				}
				cout << endl;*/

				vector<int> temp_copy = vig_list_copy[min_ind];
				for (int i : temp_copy)
				{
					/*for (int j : vig_list[i])
					{
						cout << j << " ";
					}
					cout << endl;*/
					/*cout << i << ". OVERLAP: ";
					for (int o : vectors_overlap(vig_list_copy[min_ind], vig_list_copy[i]))
					{
						cout << o << " ";
					}
					cout << endl;*/

					vector<int> overlap_val = vectors_overlap(vig_list_copy[min_ind], vig_list_copy[i]);
					int overlap_size = overlap_val.size();

					//basic clauses extraction
					if (overlap_size == 1 && vector_contains_int(i, vig_list_copy[min_ind]))
					{
						main_loop_break = false;
						to_break = false;
						//cout << "HERE" << endl;
						vector<int> single_clause;
						single_clause.resize(3);
						single_clause[0] = min_ind;
						single_clause[1] = i;
						single_clause[2] = overlap_val[0];
						unsigned_clauses.push_back(single_clause);

						//cout << overlap_val[0] <<" "<< vig_copy[min_ind][overlap_val[0]] << endl;

						vig_copy[min_ind][i]--;				//1
						vig_copy[min_ind][overlap_val[0]]--;//2
						vig_copy[i][min_ind]--;				//3
						vig_copy[i][overlap_val[0]]--;		//4
						vig_copy[overlap_val[0]][min_ind]--;//5
						vig_copy[overlap_val[0]][i]--;		//6
						//cout << overlap_val[0] <<" "<< vig_copy[min_ind][overlap_val[0]] << endl;

						//1
						if (vig_copy[min_ind][i] == 0)
						{
							vig_list_copy[min_ind].erase(remove(vig_list_copy[min_ind].begin(), vig_list_copy[min_ind].end(), i), vig_list_copy[min_ind].end());
							//cout <<1<< ". removed: " << i << " from " << min_ind << endl;
						}
						//2
						if (vig_copy[min_ind][overlap_val[0]] == 0)
						{
							vig_list_copy[min_ind].erase(remove(vig_list_copy[min_ind].begin(), vig_list_copy[min_ind].end(), overlap_val[0]), vig_list_copy[min_ind].end());
							//cout <<2<< ". removed: " << overlap_val[0] << " from " << min_ind << endl;
						}
						//3
						if (vig_copy[i][min_ind] == 0)
						{
							vig_list_copy[i].erase(remove(vig_list_copy[i].begin(), vig_list_copy[i].end(), min_ind), vig_list_copy[i].end());
							//cout <<3<< ". removed: " << min_ind << " from " << i << endl;
						}
						//4
						if (vig_copy[i][overlap_val[0]] == 0)
						{
							vig_list_copy[i].erase(remove(vig_list_copy[i].begin(), vig_list_copy[i].end(), overlap_val[0]), vig_list_copy[i].end());
							//cout <<4<< ". removed: " << overlap_val[0] << " from " << i << endl;
						}
						//5
						if (vig_copy[overlap_val[0]][min_ind] == 0)
						{
							vig_list_copy[overlap_val[0]].erase(remove(vig_list_copy[overlap_val[0]].begin(), vig_list_copy[overlap_val[0]].end(), min_ind), vig_list_copy[overlap_val[0]].end());
							//cout <<5<< ". removed: " << min_ind << " from " << overlap_val[0]<< endl;
						}
						//6
						if (vig_copy[overlap_val[0]][i] == 0)
						{
							vig_list_copy[overlap_val[0]].erase(remove(vig_list_copy[overlap_val[0]].begin(), vig_list_copy[overlap_val[0]].end(), i), vig_list_copy[overlap_val[0]].end());
							//cout <<6<< ". removed: " << i << " from " << overlap_val[0] << endl;
						}
						//cout << endl;


						/*for (int h : vig_list_copy[min_ind])
						{
							cout << h << " ";
						}
						cout << endl;
						for (int h : vig_list_copy[min_ind])
						{
							cout << vig_copy[min_ind][h] << " ";
						}
						cout << endl;*/


					}
					
					//testing based of dependency
					else if (overlap_size >= 2 && vector_contains_int(i, vig_list_copy[min_ind]))
					{
						int third_var = -1;

						vector<bool> new_pop;
						new_pop.resize(no_var);
						for (int j = 0; j < no_var; j++)
						{
							new_pop[j] = 0;
						}
						int base_val, change1_val, change2_val, final_val;

						base_val = this->fitness(new_pop);
						new_pop[min_ind] = !new_pop[min_ind];
						change1_val = this->fitness(new_pop);
						new_pop[i] = !new_pop[i];
						final_val = this->fitness(new_pop);
						new_pop[min_ind] = !new_pop[min_ind];
						change2_val = this->fitness(new_pop);
						new_pop[i] = !new_pop[i];

						int diff1, diff2, diff_final, diff_dependency;
						diff1 = base_val - change1_val;
						diff2 = base_val - change2_val;
						diff_final = base_val - final_val;
						diff_dependency = diff1 + diff2 - diff_final;


					

						for (int over : overlap_val)
						{

							if (simple_connections[min_ind][i] == 1 && simple_connections[min_ind][over] == 1 && simple_connections[i][over] == 1)
							{
								new_pop[over] = !new_pop[over];

								base_val = this->fitness(new_pop);
								new_pop[min_ind] = !new_pop[min_ind];
								change1_val = this->fitness(new_pop);
								new_pop[i] = !new_pop[i];
								final_val = this->fitness(new_pop);
								new_pop[min_ind] = !new_pop[min_ind];
								change2_val = this->fitness(new_pop);

								new_pop[i] = !new_pop[i];
								new_pop[over] = !new_pop[over];

								int diff_dependency_inside;

								diff1 = base_val - change1_val;
								diff2 = base_val - change2_val;
								diff_final = base_val - final_val;
								diff_dependency_inside = diff1 + diff2 - diff_final;

								if (diff_dependency_inside != diff_dependency) // to znaczy, ze tworza wspolnie klauzule
								{
									third_var = over;
									break;
								}
							}

						}

						if (third_var != -1)
						{

							main_loop_break = false;
							to_break = false;
							//cout << "HERE" << endl;
							vector<int> single_clause;
							single_clause.resize(3);
							single_clause[0] = min_ind;
							single_clause[1] = i;
							single_clause[2] = third_var;
							unsigned_clauses.push_back(single_clause);

							//cout << overlap_val[0] <<" "<< vig_copy[min_ind][overlap_val[0]] << endl;

							vig_copy[min_ind][i]--;				//1
							vig_copy[min_ind][third_var]--;//2
							vig_copy[i][min_ind]--;				//3
							vig_copy[i][third_var]--;		//4
							vig_copy[third_var][min_ind]--;//5
							vig_copy[third_var][i]--;		//6
							//cout << overlap_val[0] <<" "<< vig_copy[min_ind][overlap_val[0]] << endl;

							//1
							if (vig_copy[min_ind][i] == 0)
							{
								vig_list_copy[min_ind].erase(remove(vig_list_copy[min_ind].begin(), vig_list_copy[min_ind].end(), i), vig_list_copy[min_ind].end());
								//cout <<1<< ". removed: " << i << " from " << min_ind << endl;
							}
							//2
							if (vig_copy[min_ind][third_var] == 0)
							{
								vig_list_copy[min_ind].erase(remove(vig_list_copy[min_ind].begin(), vig_list_copy[min_ind].end(), third_var), vig_list_copy[min_ind].end());
								//cout <<2<< ". removed: " << overlap_val[0] << " from " << min_ind << endl;
							}
							//3
							if (vig_copy[i][min_ind] == 0)
							{
								vig_list_copy[i].erase(remove(vig_list_copy[i].begin(), vig_list_copy[i].end(), min_ind), vig_list_copy[i].end());
								//cout <<3<< ". removed: " << min_ind << " from " << i << endl;
							}
							//4
							if (vig_copy[i][third_var] == 0)
							{
								vig_list_copy[i].erase(remove(vig_list_copy[i].begin(), vig_list_copy[i].end(), third_var), vig_list_copy[i].end());
								//cout <<4<< ". removed: " << overlap_val[0] << " from " << i << endl;
							}
							//5
							if (vig_copy[third_var][min_ind] == 0)
							{
								vig_list_copy[third_var].erase(remove(vig_list_copy[third_var].begin(), vig_list_copy[third_var].end(), min_ind), vig_list_copy[third_var].end());
								//cout <<5<< ". removed: " << min_ind << " from " << overlap_val[0]<< endl;
							}
							//6
							if (vig_copy[third_var][i] == 0)
							{
								vig_list_copy[third_var].erase(remove(vig_list_copy[third_var].begin(), vig_list_copy[third_var].end(), i), vig_list_copy[third_var].end());
								//cout <<6<< ". removed: " << i << " from " << overlap_val[0] << endl;
							}

						}


					}

					//more complicated for rare case
					if (main_loop_break && overlap_size >= 2 && vig_copy[min_ind][i] >= 2 && vig_copy[min_ind][i] == simple_connections[min_ind][i] && vig_copy[min_ind][i] == overlap_size && vector_contains_int(i, vig_list_copy[min_ind]))
					{
						one_more_iter = false;
						main_loop_break = false;
						to_break = false;
						//cout << "HERE" << endl;
						//clause 1
						vector<int> single_clause;
						single_clause.resize(3);
						single_clause[0] = min_ind;
						single_clause[1] = i;
						single_clause[2] = overlap_val[0];
						unsigned_clauses.push_back(single_clause);

						vig_copy[min_ind][i]--;				//1
						vig_copy[min_ind][overlap_val[0]]--;//2
						vig_copy[i][min_ind]--;				//3
						vig_copy[i][overlap_val[0]]--;		//4
						vig_copy[overlap_val[0]][min_ind]--;//5
						vig_copy[overlap_val[0]][i]--;		//6


						//clause 2
						vector<int> single_clause2;
						single_clause2.resize(3);
						single_clause2[0] = min_ind;
						single_clause2[1] = i;
						single_clause2[2] = overlap_val[1];
						unsigned_clauses.push_back(single_clause2);

						vig_copy[min_ind][i]--;				//1
						vig_copy[min_ind][overlap_val[1]]--;//2
						vig_copy[i][min_ind]--;				//3
						vig_copy[i][overlap_val[1]]--;		//4
						vig_copy[overlap_val[1]][min_ind]--;//5
						vig_copy[overlap_val[1]][i]--;		//6

						/*for (int cl : single_clause)
						{
							cout << cl << " ";
						}
						cout << endl;
						for (int cl : single_clause2)
						{
							cout << cl << " ";
						}
						cout << endl;*/


						//1
						if (vig_copy[min_ind][i] == 0)
						{
							vig_list_copy[min_ind].erase(remove(vig_list_copy[min_ind].begin(), vig_list_copy[min_ind].end(), i), vig_list_copy[min_ind].end());
							//cout <<1<< ". removed: " << i << " from " << min_ind << endl;
						}
						//2
						if (vig_copy[min_ind][overlap_val[0]] == 0)
						{
							vig_list_copy[min_ind].erase(remove(vig_list_copy[min_ind].begin(), vig_list_copy[min_ind].end(), overlap_val[0]), vig_list_copy[min_ind].end());
							//cout <<2<< ". removed: " << overlap_val[0] << " from " << min_ind << endl;
						}
						//3
						if (vig_copy[i][min_ind] == 0)
						{
							vig_list_copy[i].erase(remove(vig_list_copy[i].begin(), vig_list_copy[i].end(), min_ind), vig_list_copy[i].end());
							//cout <<3<< ". removed: " << min_ind << " from " << i << endl;
						}
						//4
						if (vig_copy[i][overlap_val[0]] == 0)
						{
							vig_list_copy[i].erase(remove(vig_list_copy[i].begin(), vig_list_copy[i].end(), overlap_val[0]), vig_list_copy[i].end());
							//cout <<4<< ". removed: " << overlap_val[0] << " from " << i << endl;
						}
						//5
						if (vig_copy[overlap_val[0]][min_ind] == 0)
						{
							vig_list_copy[overlap_val[0]].erase(remove(vig_list_copy[overlap_val[0]].begin(), vig_list_copy[overlap_val[0]].end(), min_ind), vig_list_copy[overlap_val[0]].end());
							//cout <<5<< ". removed: " << min_ind << " from " << overlap_val[0]<< endl;
						}
						//6
						if (vig_copy[overlap_val[0]][i] == 0)
						{
							vig_list_copy[overlap_val[0]].erase(remove(vig_list_copy[overlap_val[0]].begin(), vig_list_copy[overlap_val[0]].end(), i), vig_list_copy[overlap_val[0]].end());
							//cout <<6<< ". removed: " << i << " from " << overlap_val[0] << endl;
						}

						//7
						if (vig_copy[overlap_val[1]][min_ind] == 0)
						{
							vig_list_copy[overlap_val[1]].erase(remove(vig_list_copy[overlap_val[1]].begin(), vig_list_copy[overlap_val[1]].end(), min_ind), vig_list_copy[overlap_val[1]].end());
							//cout <<5<< ". removed: " << min_ind << " from " << overlap_val[0]<< endl;
						}
						//8
						if (vig_copy[overlap_val[1]][i] == 0)
						{
							vig_list_copy[overlap_val[1]].erase(remove(vig_list_copy[overlap_val[1]].begin(), vig_list_copy[overlap_val[1]].end(), i), vig_list_copy[overlap_val[1]].end());
							//cout <<6<< ". removed: " << i << " from " << overlap_val[0] << endl;
						}

						//9
						if (vig_copy[min_ind][overlap_val[1]] == 0)
						{
							vig_list_copy[min_ind].erase(remove(vig_list_copy[min_ind].begin(), vig_list_copy[min_ind].end(), overlap_val[1]), vig_list_copy[min_ind].end());
							//cout <<2<< ". removed: " << overlap_val[0] << " from " << min_ind << endl;
						}
						//10
						if (vig_copy[i][overlap_val[1]] == 0)
						{
							vig_list_copy[i].erase(remove(vig_list_copy[i].begin(), vig_list_copy[i].end(), overlap_val[1]), vig_list_copy[i].end());
							//cout <<4<< ". removed: " << overlap_val[0] << " from " << i << endl;
						}





					}

				}
				//vig_list_copy[min_ind] = temp_copy;

				if (one_more_iter)
				{
					break;
				}

				if (to_break)
				{
					one_more_iter = true;
				}
				to_break = true;

			}



			banned_list.push_back(min_ind);


			

		}


		/*int expected = 0, checksum = 0;

		expected = no_clauses - unsigned_clauses.size();

		for (vector<int> w : vig_copy)
		{
			for (int k : w)
			{
	
				checksum += k;

			}
		}
		cout << "EXPECTED " << expected << endl;
		cout << "CHECKSUM " << checksum / 6 << endl;*/


		if (main_loop_break)
		{
			break;
		}

	}
	/*for (vector<int> i : unsigned_clauses)
	{
		for (int j : i)
			cout << j << " ";
		cout << endl;
	}*/


	//VALIDATION
	int validated = 0;

	vector<int> created_clauses_to_original;
	
	for (vector<int> test : unsigned_clauses)
	{
		bool vald = false;
		int iter = 0;
		for (vector<int> orig : int_table)
		{
			
			if ((test[0] == orig[0] || test[0] == orig[1] || test[0] == orig[2]) && (test[1] == orig[0] || test[1] == orig[1] || test[1] == orig[2]) && (test[2] == orig[0] || test[2] == orig[1] || test[2] == orig[2]))
			{
				validated++;
				created_clauses_to_original.push_back(iter);
				break;
			}
			iter++;
		}

	}


	if (unsigned_clauses.size() != no_clauses || validated != no_clauses)
	{
		cout << "PROCESS " << process_id<<endl;
		cout << "ZNALEZIONE KLAUZULE: " << unsigned_clauses.size() << endl;
		cout << "VALIDATED: " << validated << endl;
	}

	if (validated != unsigned_clauses.size())
	{
		cout << "NOT VALIDATED!!! PROCESS: " << process_id << endl;
		return;
	}


	// signing clauses

	vector<vector<bool>> signed_clauses;
	signed_clauses.resize(no_clauses);
	for (int c = 0; c < no_clauses; c++)
	{
		signed_clauses[c].resize(3);

		signed_clauses[c][0] = false;
		signed_clauses[c][1] = false;
		signed_clauses[c][2] = false;

	}

	int iter = 0;

	for (vector<int> test : unsigned_clauses)
	{
		int a = test[0];
		int b = test[1];
		int c = test[2];

		vector<bool> rand_solution;
		rand_solution.resize(no_var);

		for (int j = 0; j < no_var; j++)
		{
			rand_solution[j] = 1;// uniform_int_distribution<>{ 0, 1 }(rng);
		}


		rand_solution[a] = 0;
		rand_solution[b] = 0;
		rand_solution[c] = 0;

		for (int k = 0; k < 3; k++)
		{
	

			int temp_var;

			if (k == 0) // 001
			{

			}
			else if (k == 1) // 010
			{
				temp_var = c;
				c = b;
				b = temp_var;
			}
			else if (k == 2) // 100
			{
				temp_var = c;
				c = a;
				a = temp_var;
			}


			int base_val, change1_val, change2_val, final_val;

			base_val = this->fitness(rand_solution);
			rand_solution[a] = !rand_solution[a];
			change1_val = this->fitness(rand_solution);
			rand_solution[b] = !rand_solution[b];
			final_val = this->fitness(rand_solution);
			rand_solution[a] = !rand_solution[a];
			change2_val = this->fitness(rand_solution);
			rand_solution[b] = !rand_solution[b];

			int diff1, diff2, diff_final, diff_dependency;
			diff1 = base_val - change1_val;
			diff2 = base_val - change2_val;
			diff_final = base_val - final_val;
			diff_dependency = abs(diff1 + diff2 - diff_final);


			rand_solution[c] = !rand_solution[c];


			base_val = this->fitness(rand_solution);
			rand_solution[a] = !rand_solution[a];
			change1_val = this->fitness(rand_solution);
			rand_solution[b] = !rand_solution[b];
			final_val = this->fitness(rand_solution);
			rand_solution[a] = !rand_solution[a];
			change2_val = this->fitness(rand_solution);
			rand_solution[b] = !rand_solution[b];

			int diff_dependency2;
			diff1 = base_val - change1_val;
			diff2 = base_val - change2_val;
			diff_final = base_val - final_val;
			diff_dependency2 = abs(diff1 + diff2 - diff_final);

			//cout << "DIFF DIFF: " << diff_dependency2 - diff_dependency<<endl;


			if (diff_dependency2 > diff_dependency)
			{
				signed_clauses[iter][2-k] = false;
			}
			else if (diff_dependency2 < diff_dependency)
			{
				signed_clauses[iter][2 - k] = true;
			}
			else if (diff_dependency2 == diff_dependency)
			{
				cout << "ERROR CHECK" << endl;
			}
			else
			{
				cout<<"WTF???!??!!"<<endl;
			}





			//reverse changes
			rand_solution[c] = !rand_solution[c];
			if (k == 0)
			{

			}
			else if (k == 1)
			{
				temp_var = c;
				c = b;
				b = temp_var;
			}
			else if (k == 2)
			{
				temp_var = c;
				c = a;
				a = temp_var;
			}


		}


		iter++;
	}

	/*cout << "TRUTH TABLES" << endl;
	int eq_test = 0, eq_test2 = 0;
	for (vector<bool> test : signed_clauses)
	{
		cout << test[0] << " " << test[1] << " " << test[2] << endl;

		if (test[0] == 1)
		{
			eq_test++;
		}
		else
		{
			eq_test2++;
		}
		if (test[1] == 1)
		{
			eq_test++;
		}
		else
		{
			eq_test2++;
		}
		if (test[2] == 1)
		{
			eq_test++;
		}
		else
		{
			eq_test2++;
		}
	}
	cout <<"EQ TEST: "<< eq_test<<endl;
	cout << "EQ TEST2: " << eq_test2<<endl;*/

	/*iter = 0;
	for (int i : created_clauses_to_original)
	{
		for (int j = 0; j < 3; j++)
		{
			if (signed_clauses[iter][j] == 0)
			{
				cout << " -";
			}
			else
			{
				cout << "  ";
			}
			cout << unsigned_clauses[iter][j];
		}

		cout << endl;

		for (int j = 0; j < 3; j++)
		{
			if (bool_table[i][j] == 0)
			{
				cout << "-";
			}
			else
			{
				cout << " ";
			}
			cout << int_table[i][j];
		}
		cout << endl;
		cout << endl;
		iter++;
	}*/

	iter = 0;
	validated = 0;
	int  not_validated = 0;
	for (int i : created_clauses_to_original)
	{
		for (int h = 0; h < 3; h++)
		{
			int position = -1;
			for (int j = 0; j < 3; j++)
			{
				if (unsigned_clauses[iter][h] == int_table[i][j])
				{
					position = j;
					break;
				}

			}
			if (signed_clauses[iter][h] == bool_table[i][position])
			{
				validated++;
			}
			else
			{
				not_validated++;
				//cout << "clause number: " << iter << endl;
				/*cout << "true: " << bool_table[i][position] << endl;
				cout << "taken: " << signed_clauses[iter][h] << endl;*/
				//cout << endl;
			}

		}

		iter++;
	}

	cout << "VALIDATED: " << validated << endl;
	cout << "NOT VALIDATED: " << not_validated << endl;




}

void Solver::test_suit2_px(int test_id, int process_id, int max_time)
{

	mt19937_64 rng(random_device{}());
	vector<vector<bool>> population_tab; // array to hold all current populations
	vector<int> population_value; //fitness of each individual in population
	//population_tab.resize(population_size);
	//population_value.resize(population_size);
	this->best_results.clear();
	this->worst_results.clear();
	this->iterations = 0;
	full_ffe = 0;
	partial_ffe = 0;


	auto start = chrono::high_resolution_clock::now();
	
	Ffe ffe, ffe_partial;

	int iteration = 0;
	while (true)
	{
		//init new unit
		vector<bool> new_pop;
		new_pop.resize(no_var);
		for (int j = 0; j < no_var; j++)
		{
			new_pop[j] = uniform_int_distribution<>{ 0, 1 }(rng);
		}
		//Fitness
		population_tab.push_back(new_pop);
		population_value.push_back(this->fitness(new_pop));
		//FIHC
		this->fihc(1.0, new_pop, population_value[iteration], ffe, ffe_partial, rng);

		//
		//auto start2 = chrono::high_resolution_clock::now();
		
		if (test_id == 0)
		{

			for (int i = 0; i < population_tab.size(); i++)
			{
				this->ils1_my_rfihc_sizable(1.0, population_tab[i], population_value[i], ffe, rng, 0.75);
			}

		}
		else if (test_id == 1)
		{

			for (int i = 0; i < population_tab.size(); i++)
			{
				this->ils1_my_rfihc_sizable(1.0, population_tab[i], population_value[i], ffe, rng, 0.25);
			}

		}
		else if (test_id == 2)
		{

			for (int i = 0; i < population_tab.size(); i++)
			{
				this->ils_random_my_rfihc(1.0, population_tab[i], population_value[i], ffe, rng, 0.75);
			}


		}
		else if (test_id == 3)
		{

			for (int i = 0; i < population_tab.size(); i++)
			{
				this->ils_random_my_rfihc(1.0, population_tab[i], population_value[i], ffe, rng, 0.25);
			}

		}
		//else if (test_id == 4)
		//{
		//	for (int i = 0; i < population_tab.size(); i++)
		//	{
		//		this->ils1_my_rfihc_sizable(1.0, population_tab[i], population_value[i], ffe, rng, 1.0);
		//		vector<vector<int>> relaxation_table = this->create_fullfillment_tab(population_tab[i]); //count fulfillment for given solution
		//		//cout << "BEFORE" << endl;
		//		for (int r = 0; r < 200; r++)
		//		{
		//			ffe_partial.increment(1);

		//			int relaxation_cutoff = 0;
		//			int index_to_change = smart_relaxation_greedy(relaxation_table, relaxation_cutoff, no_clauses, no_var);
		//			if (index_to_change != -1)
		//			{
		//				//cout << index_to_change<<endl;
		//				population_tab[i][index_to_change] = !population_tab[i][index_to_change];
		//			}
		//			else
		//			{
		//				break;
		//			}
		//			this->fast_fullfillment(relaxation_table, population_tab[i], index_to_change);


		//		}
		//	}


		//}
		//else if (test_id == 5)
		//{
		//	for (int i = 0; i < population_tab.size(); i++)
		//	{
		//		this->ils1_my_rfihc_sizable(1.0, population_tab[i], population_value[i], ffe, rng, 0.5);
		//		vector<vector<int>> relaxation_table = this->create_fullfillment_tab(population_tab[i]); //count fulfillment for given solution
		//		//cout << "BEFORE" << endl;
		//		for (int r = 0; r < 200; r++)
		//		{
		//			ffe_partial.increment(1);
		//			
		//			int relaxation_cutoff = 0;
		//			int index_to_change = smart_relaxation_greedy(relaxation_table, relaxation_cutoff, no_clauses, no_var);
		//			if (index_to_change != -1)
		//			{
		//				//cout << index_to_change<<endl;
		//				population_tab[i][index_to_change] = !population_tab[i][index_to_change];
		//			}
		//			else
		//			{
		//				break;
		//			}
		//			this->fast_fullfillment(relaxation_table, population_tab[i], index_to_change);


		//		}
		//	}


		//}
		//else if (test_id == 6)
		//{
		//	for (int i = 0; i < population_tab.size(); i++)
		//	{
		//		this->ils_random_my_rfihc(1.0, population_tab[i], population_value[i], ffe, rng, 1.0);
		//		vector<vector<int>> relaxation_table = this->create_fullfillment_tab(population_tab[i]); //count fulfillment for given solution
		//		//cout << "BEFORE" << endl;
		//		for (int r = 0; r < 200; r++)
		//		{
		//			ffe_partial.increment(1);

		//			int relaxation_cutoff = 0;
		//			int index_to_change = smart_relaxation_greedy(relaxation_table, relaxation_cutoff, no_clauses, no_var);
		//			if (index_to_change != -1)
		//			{
		//				//cout << index_to_change<<endl;
		//				population_tab[i][index_to_change] = !population_tab[i][index_to_change];
		//			}
		//			else
		//			{
		//				break;
		//			}
		//			this->fast_fullfillment(relaxation_table, population_tab[i], index_to_change);


		//		}
		//	}


		//}
		//else if (test_id == 7)
		//{
		//	for (int i = 0; i < population_tab.size(); i++)
		//	{
		//		this->ils_random_my_rfihc(1.0, population_tab[i], population_value[i], ffe, rng, 0.5);
		//		vector<vector<int>> relaxation_table = this->create_fullfillment_tab(population_tab[i]); //count fulfillment for given solution
		//		//cout << "BEFORE" << endl;
		//		for (int r = 0; r < 200; r++)
		//		{
		//			ffe_partial.increment(1);

		//			int relaxation_cutoff = 0;
		//			int index_to_change = smart_relaxation_greedy(relaxation_table, relaxation_cutoff, no_clauses, no_var);
		//			if (index_to_change != -1)
		//			{
		//				//cout << index_to_change<<endl;
		//				population_tab[i][index_to_change] = !population_tab[i][index_to_change];
		//			}
		//			else
		//			{
		//				break;
		//			}
		//			this->fast_fullfillment(relaxation_table, population_tab[i], index_to_change);


		//		}
		//	}


		//}


		//auto end2 = chrono::high_resolution_clock::now();
		//chrono::duration<double> elapsed2 = end2 - start2;
		//cout << "ILS TIME: " << elapsed2.count() << endl;
		/*if (iterations % 100 == 0)
		{
			cout << "iterations: " << iterations;
		}*/


		//PX phase
		
		int temp_value = 0;
		int ffe_temp = ffe.get_ffe();
		//cout << "BEFORE PX" << endl;
		//start2 = chrono::high_resolution_clock::now();
		for (int i = 0; i < population_tab.size() - 1; i++)
		{
			vector<bool> temp_vec = this->px(population_tab[iteration], population_tab[i], 0, ffe);
			population_tab[iteration] = temp_vec;

		}
		//end2 = chrono::high_resolution_clock::now();
		//elapsed2 = end2 - start2;
		//cout << "PX  TIME: " << elapsed2.count() << endl;

		population_value[iterations] = this->fitness(population_tab[iteration]);


		int diff = ffe.get_ffe() - ffe_temp;
		ffe_partial.increment(diff);
		population_value[iteration] = this->fitness(population_tab[iteration]);
		ffe.increment(1);
		ffe_partial.increment(1);


		this->partial_ffe = ffe_partial.get_ffe();
		this->full_ffe = ffe.get_ffe();
		this->iterations = iteration;



		// saving best
		int max_el = *max_element(population_value.begin(), population_value.end());
		int min_el = *min_element(population_value.begin(), population_value.end());
		best_results.push_back(max_el);
		worst_results.push_back(min_el);

		if (max_el == no_clauses)
		{
			int ind = get_index(population_value, max_el);
			this->main_solution = population_tab[ind];
			/*for (int i = 0; i < no_var; i++)
			{
				cout << population_tab[ind][i];
			}*/
			break;
		}



		iteration++;
		auto finish = chrono::high_resolution_clock::now();
		chrono::duration<double> elapsed = finish - start;
		//STOP CONDITION
		
		if (elapsed.count() > max_time)
		{
			//cout << "iteration: " << iteration - 1 << " - time -";
			//cout << elapsed.count() << endl;
			break;
		}
	}



}

void Solver::test_suit_p3(int test_id, int process_id, int max_time)
{
	mt19937_64 rng(random_device{}());
	vector<vector<vector<bool>>> population_pyramid; // array to hold all current populations
	vector<vector<int>> population_value_pyramid; //fitness of each individual in population
	vector<vector<bool>> brute_force_blacklist;
	//population_tab.resize(population_size);
	//population_value.resize(population_size);
	this->best_results.clear();
	this->worst_results.clear();
	this->iterations = 0;
	this->main_result = 0;
	this->main_solution.resize(0);
	full_ffe = 0;
	partial_ffe = 0;
		
	brute_force_count = 0;
	one_off_solutions = 0;
	best_neighbours = 0;
	one_off_best_neighbours = 0;
	this->brute_force_improved = false;
	this->ILS_BF_improved = false;

	ils_time = 0;
	px_time = 0;
	brute_force_time = 0;
	ils_bf_mask_time = 0;

	plateau_size = 0;

	auto start = chrono::high_resolution_clock::now();

	Ffe ffe, ffe_partial;

	int iteration = 0;

	population_pyramid.resize(1);
	population_value_pyramid.resize(1);
	

	int known_max = no_clauses;

	//set for 1 off solutions;
	unordered_set<string> one_offs;
	unordered_set<string> two_offs;
	unordered_set<string> perfect_solutions;

	bool fixed_clauses = false;

	while (true)
	{
		//init new unit
		vector<bool> new_pop;
		new_pop.resize(no_var);
		for (int j = 0; j < no_var; j++)
		{
			new_pop[j] = uniform_int_distribution<>{ 0, 1 }(rng);
		}
		//Fitness
		population_pyramid[0].push_back(new_pop);
		population_value_pyramid[0].push_back(this->fitness(new_pop));
		//FIHC
		this->fihc(1.0, new_pop, population_value_pyramid[0][iteration], ffe, ffe_partial, rng);

		auto start2 = chrono::high_resolution_clock::now();
		
		/*if (test_id == 0)
		{

			for (int i = 0; i < population_pyramid[0].size(); i++)
			{
				this->ils1_my_rfihc_sizable(1.0, population_pyramid[0][i], population_value_pyramid[0][i], ffe, rng, 0.37);
			}

		}
		else if (test_id == 1)
		{
			for (int i = 0; i < population_pyramid[0].size(); i++)
			{
				this->ils1_my_rfihc_sizable(1.0, population_pyramid[0][i], population_value_pyramid[0][i], ffe, rng, 0.25);
			}
		}
		else if (test_id == 2)
		{

			for (int i = 0; i < population_pyramid[0].size(); i++)
			{
				this->ils1_my_rfihc_sizable(1.0, population_pyramid[0][i], population_value_pyramid[0][i], ffe, rng, 0.17);
			}

		}
		else if (test_id == 3)
		{

			for (int i = 0; i < population_pyramid[0].size(); i++)
			{
				this->ils_random_my_rfihc(1.0, population_pyramid[0][i], population_value_pyramid[0][i], ffe, rng, 0.37);
			}

		}
		else if (test_id == 4)
		{

			for (int i = 0; i < population_pyramid[0].size(); i++)
			{
				this->ils_random_my_rfihc(1.0, population_pyramid[0][i], population_value_pyramid[0][i], ffe, rng, 0.25);
			}

		}
		else if (test_id == 5)
		{

			for (int i = 0; i < population_pyramid[0].size(); i++)
			{
				this->ils_random_my_rfihc(1.0, population_pyramid[0][i], population_value_pyramid[0][i], ffe, rng, 0.17);
			}

		}*/
		//STANDARD OLD TEST
		if (test_id == 0)
		{
			int pyramid_size = population_pyramid.size();
			for (int level_counter = 0; level_counter < pyramid_size; level_counter++)
			{
				int level_size = population_pyramid[level_counter].size() - 1;
				for (int i = 0; i < level_size; i++)
				{
					this->ils1_my_rfihc_sizable(1.0, population_pyramid[level_counter][i], population_value_pyramid[level_counter][i], ffe, rng, 0.25);

				}
			}

			/*for (int i = 0; i < population_pyramid[0].size(); i++)
			{
				this->ils1_my_rfihc_sizable(1.0, population_pyramid[0][i], population_value_pyramid[0][i], ffe, rng, 0.25);
			}*/

		}
		// TEST WHERE 1x and 2x are more important than fitness after 0.995 fitness level
		else if (test_id == 1)
		{

			int pyramid_size = population_pyramid.size();
			for (int level_counter = 0; level_counter < pyramid_size; level_counter++)
			{
				int level_size = population_pyramid[level_counter].size() - 1;
				for (int i = 0; i < level_size; i++)
				{
					this->ils1_my_directed_fihc_sizable(1.0, population_pyramid[level_counter][i], population_value_pyramid[level_counter][i], ffe, rng, 0.25);

				}
			}

			/*for (int i = 0; i < population_pyramid[0].size(); i++)
			{
				this->ils1_my_directed_fihc_sizable(1.0, population_pyramid[0][i], population_value_pyramid[0][i], ffe, rng, 0.25);
			}*/
		}
		// TEST WHERE 1x and 2x are more important than fitness after 0.990 fitness level
		else if (test_id == 3)
		{

			int pyramid_size = population_pyramid.size();
			for (int level_counter = 0; level_counter < pyramid_size; level_counter++)
			{
				int level_size = population_pyramid[level_counter].size() - 1;
				for (int i = 0; i < level_size; i++)
				{
					this->ils1_my_directed_fihc_sizable(1.0, population_pyramid[level_counter][i], population_value_pyramid[level_counter][i], ffe, rng, 0.25);

				}
			}


			/*for (int i = 0; i < population_pyramid[0].size(); i++)
			{
				this->ils1_my_directed_fihc_sizable(1.0, population_pyramid[0][i], population_value_pyramid[0][i], ffe, rng, 0.25);
			}*/

		}
		else if (test_id == 2) // OPTYMALIZATOR MIESZANY
		{


			int vev_iter = 0;
			int pyramid_size = population_pyramid.size();
			for (int level_counter = 0; level_counter < pyramid_size; level_counter++)
			{
				int level_size = population_pyramid[level_counter].size() - 1;
				for (int i = 0; i < level_size; i++)
				{
					if (vev_iter % 2 == 0)
					{
						this->ils1_my_rfihc_sizable(1.0, population_pyramid[level_counter][i], population_value_pyramid[level_counter][i], ffe, rng, 0.25);
					}
					else
					{
						this->ils1_my_directed_fihc_sizable(1.0, population_pyramid[level_counter][i], population_value_pyramid[level_counter][i], ffe, rng, 0.25);
					}
					vev_iter++;
				}
			}


		}
		auto finish2 = chrono::high_resolution_clock::now();
		chrono::duration<double> elapsed2 = finish2 - start2;
		ils_time += elapsed2.count();
	
		//PX, P3 phase

		int temp_value = 0;
		int ffe_temp = ffe.get_ffe();

		auto start3 = chrono::high_resolution_clock::now();

		//int temp_start_fitness = this->fitness(population_pyramid[0][iteration]);
		//int pyramid_size = population_pyramid.size();
		//vector<bool> temp_main_solution = population_pyramid[0].back();
		//for (int level_counter=0; level_counter < pyramid_size; level_counter++)
		//{
		//	int level_size = population_pyramid[level_counter].size() - 1;
		//	for (int i = 0; i < level_size; i++)
		//	{
		//		vector<bool> temp_vec = this->px(temp_main_solution, population_pyramid[level_counter][i], 0, ffe);
		//		
		//		int value_after_px = this->fitness(temp_vec);
		//		temp_main_solution = temp_vec;
		//		if (value_after_px > temp_start_fitness) // P3 phase, we add it to the next level
		//		{
		//			//check size of pyramid
		//			if (level_counter + 1 == pyramid_size)
		//			{
		//				population_pyramid.resize(level_counter + 2);
		//				population_value_pyramid.resize(level_counter + 2);
		//				pyramid_size++;
		//			}
		//			//int mid_level_counter = population_pyramid[level_counter].size();
		//			population_pyramid[level_counter+1].push_back(temp_vec);
		//			population_value_pyramid[level_counter + 1].push_back(value_after_px);
		//			temp_start_fitness = value_after_px;

		//			break;
		//		}
		//		else
		//		{
		//			population_pyramid[level_counter][level_size] = temp_vec;
		//		}
		//	}


		//}

		int temp_start_fitness = this->fitness(population_pyramid[0][iteration]);
		int pyramid_size = population_pyramid.size();
		vector<bool> temp_main_solution = population_pyramid[0].back();
		for (int level_counter = 0; level_counter < pyramid_size; level_counter++)
		{
			int level_size = population_pyramid[level_counter].size() - 1;
			for (int i = 0; i < level_size; i++)
			{
				int fit_before = temp_start_fitness;
				vector<bool> temp_vec = this->px2(temp_main_solution, population_pyramid[level_counter][i], temp_start_fitness);
				temp_main_solution = temp_vec;
				if (temp_start_fitness > fit_before) // P3 phase, we add it to the next level
				{
					//check size of pyramid
					if (level_counter + 1 == pyramid_size)
					{
						population_pyramid.resize(level_counter + 2);
						population_value_pyramid.resize(level_counter + 2);
						pyramid_size++;
					}
					//int mid_level_counter = population_pyramid[level_counter].size();
					population_pyramid[level_counter + 1].push_back(temp_vec);
					population_value_pyramid[level_counter + 1].push_back(temp_start_fitness);

					break;
				}
				else
				{
					population_pyramid[level_counter][level_size] = temp_vec;
				}
			}


		}
		population_value_pyramid[0][iteration] = this->fitness(population_pyramid[0][iteration]);

		auto finish3 = chrono::high_resolution_clock::now();
		chrono::duration<double> elapsed3 = finish3 - start3;
		px_time  += elapsed3.count();
		

		float limit = 0; //POSSIBLY CHANGE IF FUTURE WARNING
		auto start4 = chrono::high_resolution_clock::now();
		/*if (test_id == 2)
		{
			limit = 0.990;

		}
		else if (test_id == 1)
		{
			limit = 0.995;
		}
		else if (test_id == 3)
		{
			limit = 0;
		}*/


		if (test_id == 1 || test_id == 2 || test_id == 3)
		{
			bool to_break = false;
			int pyramid_iter = 0;
			for (int level_counter = 0; level_counter < pyramid_size; level_counter++)
			{
				int level_size = population_pyramid[level_counter].size() - 1;
				for (int i = 0; i < level_size; i++)
				{
					if (population_value_pyramid[level_counter][i] >= floor(float(no_clauses) * limit ) )//&& pyramid_iter %2==1)
					{
						//string solution_string = vector_to_string(population_pyramid[level_counter][i]);
						//one_offs.insert(solution_string);


						//100 iterations of improving 1x and 2x regardless of fitness as long as it's over 636/633
						vector<vector<int>> relaxation_table = this->create_fullfillment_tab(population_pyramid[level_counter][i]); //count fulfillment for given solution
						for(int r = 0; r < 25; r++)
						{

							int relaxation_cutoff = 1;
							int index_to_change = smart_relaxation_version3(relaxation_table, relaxation_cutoff, no_clauses, no_var, limit, population_value_pyramid[level_counter][i]);

							if (index_to_change != -1)
							{
								//cout << index_to_change<<endl;
								population_pyramid[level_counter][i][index_to_change] = !population_pyramid[level_counter][i][index_to_change];
								population_value_pyramid[level_counter][i] += (relaxation_table[index_to_change][0] - relaxation_table[index_to_change][1]);
							}
							else
							{
								break;
							}
							this->fast_fullfillment(relaxation_table, population_pyramid[level_counter][i], index_to_change);
							if (population_value_pyramid[level_counter][i] == no_clauses)
							{
								break;
							}
							pyramid_iter++;
						}
						population_value_pyramid[level_counter][i] = this->fitness(population_pyramid[level_counter][i]);




						if (population_value_pyramid[level_counter][i] == no_clauses)
						{
							//cout << "ILS IMPROVED" << endl;
							this->ILS_BF_improved = true;
							to_break = true;
							break;
						}


					}
					

				}
				if (to_break)
				{
					break;
				}
			}



		}
		auto finish4 = chrono::high_resolution_clock::now();
		chrono::duration<double> elapsed4 = finish4 - start4;
		brute_force_time += elapsed4.count(); 



		/*auto start4 = chrono::high_resolution_clock::now();

		bool to_break = false;
		pyramid_size = population_pyramid.size();
		for (int level_counter = 0; level_counter < pyramid_size; level_counter++)
		{
			int level_size = population_pyramid[level_counter].size() - 1;
			for (int i = 0; i < level_size; i++)
			{
				if (population_value_pyramid[level_counter][i] == no_clauses - 1)
				{
					bool already_checked = false;

					//HASH MAP TODO!!!!!
					int blacklist_counter = 0;
					for (vector<bool> bf_bl : brute_force_blacklist)
					{
						blacklist_counter++;
						if (population_pyramid[level_counter][i] == bf_bl)
						{
							already_checked = true;
							break;
						}

						if (blacklist_counter > 5000) // artificial break
						{
							break;
						}
					}
					

					if (already_checked == false)
					{
						brute_force_blacklist.push_back(population_pyramid[level_counter][i]);

						this->brute_force(population_pyramid[level_counter][i], population_value_pyramid[level_counter][i], no_clauses, brute_force_count);

						if (population_value_pyramid[level_counter][i] == no_clauses)
						{
							cout << "BRUTE FORCE IMPROVED" << endl;
							this->brute_force_improved = true;
							to_break = true;
							break;
						}
					}
				}

			}
			if (to_break)
			{
				break;
			}
		}

		auto finish4 = chrono::high_resolution_clock::now();
		chrono::duration<double> elapsed4 = finish4 - start4;
		brute_force_time += elapsed4.count();*/



		//ILS ON BF EXTENDED MASK, RUN X times
		auto start5 = chrono::high_resolution_clock::now();
		bool to_break = false;
		pyramid_size = population_pyramid.size();

		//if ( main_result == no_clauses || iteration > 500 ) //
		//{
		//	if (!fixed_clauses)
		//	{
		//		remove_clauses();

		//		fixed_clauses = true;
		//	}




			for (int level_counter = 0; level_counter < pyramid_size; level_counter++)
			{
				int level_size = population_pyramid[level_counter].size() - 1;
				for (int i = 0; i < level_size; i++)
				{
					if (population_value_pyramid[level_counter][i] == no_clauses - 1)  // FOR GENERATED FILES CHANGE no_clauses for known_max !!!!!!!!!!!!!
					{
						string solution_string = vector_to_string(population_pyramid[level_counter][i]);
						//one_offs.insert(solution_string);

						this->ils_bf_mask_cut(population_pyramid[level_counter][i], population_value_pyramid[level_counter][i], ffe, rng);
						if (population_value_pyramid[level_counter][i] == no_clauses)
						{
							//cout << "ILS IMPROVED" << endl;
							this->ILS_BF_improved = true;
							to_break = true;
							break;
						}


					}
					//else if (population_value_pyramid[level_counter][i] >= floor(float(no_clauses) * 0.99) && population_value_pyramid[level_counter][i] < known_max - 1) // FOR GENERATED FILES CHANGE no_clauses for known_max !!!!!!!!!!!!!
					//{
					//	string solution_string = vector_to_string(population_pyramid[level_counter][i]);
					//	two_offs.insert(solution_string);
					//}
					//else if (population_value_pyramid[level_counter][i] == known_max)
					//{
					//	string solution_string = vector_to_string(population_pyramid[level_counter][i]);
					//	perfect_solutions.insert(solution_string);
					//}

				}
				if (to_break)
				{
					break;
				}
			}




		//}

		auto finish5 = chrono::high_resolution_clock::now();
		chrono::duration<double> elapsed5 = finish5 - start5;
		ils_bf_mask_time += elapsed5.count();





		this->one_off_solutions = brute_force_blacklist.size();
		this->iterations = iteration;



		// saving best
		
		//pyramid_size = population_pyramid.size();

		/*if (iteration % 100 == 0)
		{
			cout << "WYSOKOSC PIRAMIDY P3: " << pyramid_size<<endl;

			for (int i = 0; i < pyramid_size; i++)
			{

				cout << "Srednia wartosc poziomu " << i << ": " << average_over_vector(population_value_pyramid[i])<<endl;
			}

		}*/



		int index_to_save = -1;

		int overall_max = this->main_result;
		int overall_max_level = -1;
		for (int level_counter = 0; level_counter < pyramid_size; level_counter++)
		{
			int max_el = *max_element(population_value_pyramid[level_counter].begin(), population_value_pyramid[level_counter].end());

			if (max_el > overall_max)
			{
				overall_max = max_el;
				overall_max_level = level_counter;
				//if (overall_max == no_clauses)
				//{
				//	//cout << "LEVEL COUNTER" << level_counter << endl;
				//	int a = distance(population_value_pyramid[level_counter].begin(), max_element(population_value_pyramid[level_counter].begin(), population_value_pyramid[level_counter].end()));
				//	//cout << "DISTANCE: " << a << endl;
				//	index_to_save = a;
				//	this->main_solution = population_pyramid[overall_max_level][index_to_save];
				//}

				int a = distance(population_value_pyramid[level_counter].begin(), max_element(population_value_pyramid[level_counter].begin(), population_value_pyramid[level_counter].end()));
				//cout << "DISTANCE: " << a << endl;
				index_to_save = a;
				this->main_solution = population_pyramid[overall_max_level][index_to_save];
				auto finishx = chrono::high_resolution_clock::now();
				chrono::duration<double> elapsedx = finishx - start;
				this->best_time = elapsedx.count();
			}
		}
		known_max = overall_max;
		
		this->main_result = overall_max;
		best_results.push_back(overall_max);

		
		

		if (overall_max == no_clauses ) //PAMIETAJ ZEBY TO KIEDY WYLACZYC ZEBY TO SIE NORMALNIE PRZERYWALO!!!!!!!!!!! DZIÊKI!
		{
			//int ind = get_index(population_value_pyramid[overall_max_level], overall_max);
			//cout << "LEVEL COUNTER" << overall_max_level << endl;
			//cout << "DISTANCE: " << ind << endl;
			auto finishx = chrono::high_resolution_clock::now();
			chrono::duration<double> elapsedx = finishx - start;
			this->full_time = elapsedx.count();
			//this->main_solution = population_pyramid[overall_max_level][index_to_save];
			this->main_result = overall_max;

			break;
			/*for (int i = 0; i < no_var; i++)
			{
				cout << population_tab[ind][i];
			}*/


			vector<vector<int>> final_relaxation_table = this->create_fullfillment_tab(main_solution); //count fulfillment for given solution
			
			unordered_set<string> used_tab;
			used_tab.insert(vector_to_string(main_solution));
			int iterator = 0;
			int deep = 0;


			for (vector<int> gene_tab : final_relaxation_table)
			{
				if (gene_tab[1] == 1)
				{
					one_off_best_neighbours++;
				}
				else if ((gene_tab[1] == 0))
				{
					best_neighbours++;
					//cout << best_neighbours << " ";
					dfs_plateau(used_tab, final_relaxation_table, main_solution, iterator, deep);
					
				}
				iterator++;
			}
			plateau_size = used_tab.size();
		
			//cout << used_tab.size() << endl;

			break;
		}

		iteration++;
		auto finish = chrono::high_resolution_clock::now();
		chrono::duration<double> elapsed = finish - start;
		//STOP CONDITION

		if (elapsed.count() > max_time)
		{
			this->full_time = elapsed.count();
			cout << "iteration: " << iteration - 1 << " - time: ";
			cout << elapsed.count() << endl;
			break;
		}

		/*if (two_offs.size() + one_offs.size() >= 100000)
		{
			break;
		}*/
	}

	/*cout << "BACKBONE LIST: " << endl;
	int check = 0;
	for (int i : backbone_list)
	{
		cout << i << " ";
		if (i != -1)
		{
			check++;
		}

	}
	cout << endl;
	cout <<"CHECK: "<<check<< endl;*/
	return;
	
	//cout <<"BEST SOLUTION FOUND: "<< known_max <<"/"<< no_clauses<< endl;

	//this->solutions_diagnosis_inside(this->main_solution);

	tail_var_sum.clear();
	tail_var_sum.resize(backbone_size + 1);
	count_for_backbone.clear();
	count_for_backbone.resize(backbone_size + 1);
	count_for_backbone_one_off.clear();
	count_for_backbone_one_off.resize(backbone_size + 1);
	count_for_backbone_two_off.clear();
	count_for_backbone_two_off.resize(backbone_size + 1);

	count_for_backbone_perfect_solutions.clear();
	count_for_backbone_perfect_solutions.resize(backbone_size + 1);

	// DATA ANALYSIS PART

	

	/*relaxation_1x_min.resize(backbone_size + 1, -1);
	relaxation_2x_min.resize(backbone_size + 1, -1);
	relaxation_1x_max.resize(backbone_size + 1);
	relaxation_2x_max.resize(backbone_size + 1);

	relaxation_3x_min.resize(backbone_size + 1, -1);
	relaxation_3x_max.resize(backbone_size + 1);

	reverse_relaxation_1x_min.resize(no_clauses + 1, -1);
	reverse_relaxation_2x_min.resize(no_clauses + 1, -1);
	reverse_relaxation_1x_max.resize(no_clauses + 1);
	reverse_relaxation_2x_max.resize(no_clauses + 1);

	reverse_relaxation_3x_min.resize(no_clauses + 1, -1);
	reverse_relaxation_3x_max.resize(no_clauses + 1);


	reverse_backbone_1x.resize(no_clauses + 1);
	reverse_backbone_2x.resize(no_clauses + 1);
	reverse_backbone_1x_count.resize(no_clauses + 1);
	reverse_backbone_2x_count.resize(no_clauses + 1);

	reverse_backbone_3x.resize(no_clauses + 1);
	reverse_backbone_3x_count.resize(no_clauses + 1);


	reverse_backbone_1x_avg.resize(no_clauses + 1);
	reverse_backbone_2x_avg.resize(no_clauses + 1);
	reverse_backbone_3x_avg.resize(no_clauses + 1);



	tail_var_sum_no_cuts.resize(backbone_size+1);
	relaxation_sum_1x.resize(backbone_size+1);
	relaxation_sum_2x.resize(backbone_size+1);
	relaxation_sum_3x.resize(backbone_size+1);*/


	//cout << "ONE OFFS SIZE: " << one_offs.size() << endl;
	//cout << "MORE OFFS SIZE: " << two_offs.size() << endl;
	cout << "ONE AND MORE OFFS SIZE: " << one_offs.size() + two_offs.size() << endl;
	int no_cuts = -1;
	for (string s : one_offs)
	{
		vector<bool> current_vector = string_to_vector(s, no_var);
		int backbone_index = this->count_backbone_overlap(current_vector);


		//int tail_vars = this->bf_mask_count(current_vector, no_cuts, ffe, rng);
		////tail vars
		//tail_var_sum[backbone_index] += tail_vars;
		//tail_var_sum_no_cuts[backbone_index] += no_cuts;
		////fulfillment x1,x2,x3
		//int x1=0, x2=0, x3=0;

		//vector<vector<int>> temp_fulfillment = create_fullfillment_tab(current_vector);

		//for (vector<int> vec : temp_fulfillment)
		//{
		//	x1 += vec[1];
		//	x2 += vec[2];
		//	x3 += vec[3];

		//	
		//}
		//x2 = x2 / 2;
		//x3 = x3 / 3;


		//if (relaxation_1x_min[backbone_index] == -1)
		//{
		//	relaxation_1x_min[backbone_index] = x1;
		//}
		//else if (relaxation_1x_min[backbone_index] > x1 )
		//{
		//	relaxation_1x_min[backbone_index] = x1;
		//}
		//if (relaxation_1x_max[backbone_index] < x1)
		//{
		//	relaxation_1x_max[backbone_index] = x1;
		//}


		//if (relaxation_2x_min[backbone_index] == -1)
		//{
		//	relaxation_2x_min[backbone_index] = x2;
		//}
		//else if (relaxation_2x_min[backbone_index] > x2)
		//{
		//	relaxation_2x_min[backbone_index] = x2;
		//}
		//if (relaxation_2x_max[backbone_index] < x2)
		//{
		//	relaxation_2x_max[backbone_index] = x2;
		//}

		//if (relaxation_3x_min[backbone_index] == -1)
		//{
		//	relaxation_3x_min[backbone_index] = x3;
		//}
		//else if (relaxation_3x_min[backbone_index] > x3)
		//{
		//	relaxation_3x_min[backbone_index] = x3;
		//}
		//if (relaxation_3x_max[backbone_index] < x3)
		//{
		//	relaxation_3x_max[backbone_index] = x3;
		//}


		////reverse relaxation min max
		//if (reverse_relaxation_1x_min[x1] == -1)
		//{
		//	reverse_relaxation_1x_min[x1] = backbone_index;
		//}
		//else if (reverse_relaxation_1x_min[x1] > backbone_index)
		//{
		//	reverse_relaxation_1x_min[x1] = backbone_index;
		//}
		//if (reverse_relaxation_1x_max[x1] < backbone_index)
		//{
		//	reverse_relaxation_1x_max[x1] = backbone_index;
		//}

		//if (reverse_relaxation_2x_min[x2] == -1)
		//{
		//	reverse_relaxation_2x_min[x2] = backbone_index;
		//}
		//else if (reverse_relaxation_2x_min[x2] > backbone_index)
		//{
		//	reverse_relaxation_2x_min[x2] = backbone_index;
		//}
		//if (reverse_relaxation_2x_max[x2] < backbone_index)
		//{
		//	reverse_relaxation_2x_max[x2] = backbone_index;
		//}

		//if (reverse_relaxation_3x_min[x3] == -1)
		//{
		//	reverse_relaxation_3x_min[x3] = backbone_index;
		//}
		//else if (reverse_relaxation_3x_min[x3] > backbone_index)
		//{
		//	reverse_relaxation_3x_min[x3] = backbone_index;
		//}
		//if (reverse_relaxation_3x_max[x3] < backbone_index)
		//{
		//	reverse_relaxation_3x_max[x3] = backbone_index;
		//}


		//relaxation_sum_1x[backbone_index] += x1;
		//relaxation_sum_2x[backbone_index] += x2;
		//relaxation_sum_3x[backbone_index] += x3;

		//reverse_backbone_1x[x1] += backbone_index;
		//reverse_backbone_2x[x2] += backbone_index;
		//reverse_backbone_1x_count[x1]++;
		//reverse_backbone_2x_count[x2]++;

		//reverse_backbone_3x[x3] += backbone_index;
		//reverse_backbone_3x_count[x3]++;



		count_for_backbone_one_off[backbone_index]++;
		count_for_backbone[backbone_index]++;
	}
	for (string s : two_offs)
	{
		vector<bool> current_vector = string_to_vector(s, no_var);
		int backbone_index = this->count_backbone_overlap(current_vector);

		//int tail_vars = this->bf_mask_count(current_vector, no_cuts, ffe, rng);
		////tail vars
		//tail_var_sum[backbone_index] += tail_vars;
		//tail_var_sum_no_cuts[backbone_index] += no_cuts;
		////fulfillment x1,x2,x3
		//int x1 = 0, x2 = 0, x3 = 0;

		//vector<vector<int>> temp_fulfillment = create_fullfillment_tab(current_vector);

		//for (vector<int> vec : temp_fulfillment)
		//{
		//	x1 += vec[1];
		//	x2 += vec[2];
		//	x3 += vec[3];

		//}
		//x2 = x2 / 2;
		//x3 = x3 / 3;

		//if (relaxation_1x_min[backbone_index] == -1)
		//{
		//	relaxation_1x_min[backbone_index] = x1;
		//}
		//else if (relaxation_1x_min[backbone_index] > x1)
		//{
		//	relaxation_1x_min[backbone_index] = x1;
		//}
		//if (relaxation_1x_max[backbone_index] < x1)
		//{
		//	relaxation_1x_max[backbone_index] = x1;
		//}


		//if (relaxation_2x_min[backbone_index] == -1)
		//{
		//	relaxation_2x_min[backbone_index] = x2;
		//}
		//else if (relaxation_2x_min[backbone_index] > x2)
		//{
		//	relaxation_2x_min[backbone_index] = x2;
		//}
		//if (relaxation_2x_max[backbone_index] < x2)
		//{
		//	relaxation_2x_max[backbone_index] = x2;
		//}

		//if (relaxation_3x_min[backbone_index] == -1)
		//{
		//	relaxation_3x_min[backbone_index] = x3;
		//}
		//else if (relaxation_3x_min[backbone_index] > x3)
		//{
		//	relaxation_3x_min[backbone_index] = x3;
		//}
		//if (relaxation_3x_max[backbone_index] < x3)
		//{
		//	relaxation_3x_max[backbone_index] = x3;
		//}


		////reverse relaxation min max
		//if (reverse_relaxation_1x_min[x1] == -1)
		//{
		//	reverse_relaxation_1x_min[x1] = backbone_index;
		//}
		//else if (reverse_relaxation_1x_min[x1] > backbone_index)
		//{
		//	reverse_relaxation_1x_min[x1] = backbone_index;
		//}
		//if (reverse_relaxation_1x_max[x1] < backbone_index)
		//{
		//	reverse_relaxation_1x_max[x1] = backbone_index;
		//}

		//if (reverse_relaxation_2x_min[x2] == -1)
		//{
		//	reverse_relaxation_2x_min[x2] = backbone_index;
		//}
		//else if (reverse_relaxation_2x_min[x2] > backbone_index)
		//{
		//	reverse_relaxation_2x_min[x2] = backbone_index;
		//}
		//if (reverse_relaxation_2x_max[x2] < backbone_index)
		//{
		//	reverse_relaxation_2x_max[x2] = backbone_index;
		//}

		//if (reverse_relaxation_3x_min[x3] == -1)
		//{
		//	reverse_relaxation_3x_min[x3] = backbone_index;
		//}
		//else if (reverse_relaxation_3x_min[x3] > backbone_index)
		//{
		//	reverse_relaxation_3x_min[x3] = backbone_index;
		//}
		//if (reverse_relaxation_3x_max[x3] < backbone_index)
		//{
		//	reverse_relaxation_3x_max[x3] = backbone_index;
		//}


		//relaxation_sum_1x[backbone_index] += x1;
		//relaxation_sum_2x[backbone_index] += x2;
		//relaxation_sum_3x[backbone_index] += x3;

		//reverse_backbone_1x[x1] += backbone_index;
		//reverse_backbone_2x[x2] += backbone_index;
		//reverse_backbone_1x_count[x1]++;
		//reverse_backbone_2x_count[x2]++;

		//reverse_backbone_3x[x3] += backbone_index;
		//reverse_backbone_3x_count[x3]++;

		count_for_backbone_two_off[backbone_index]++;
		count_for_backbone[backbone_index]++;
	}
	//int perfect_count =0;
	for (string s : perfect_solutions)
	{
		vector<bool> current_vector = string_to_vector(s, no_var);
		int backbone_index = this->count_backbone_overlap(current_vector);

		if (backbone_index < backbone_size )
		{
			count_for_backbone_perfect_solutions[backbone_index]++;
			//perfect_count++;
		}

	}

	//cout << "PERFECT COUNT: " << perfect_count<<endl;



	//cout << "CUTS TRY:" << tail_var_sum[85] << endl;
	//cout <<"NO CUTS TRY:"<< tail_var_sum_no_cuts[85] << endl;

	/*tail_var_avg.resize(backbone_size+1);
	tail_var_avg_no_cuts.resize(backbone_size+1);
	relaxation_sum_1x_avg.resize(backbone_size+1);
	relaxation_sum_2x_avg.resize(backbone_size+1);
	relaxation_sum_3x_avg.resize(backbone_size+1);

	for (int x = 0; x <= backbone_size; x++)
	{
		if (count_for_backbone[x] !=0)
		{
			tail_var_avg[x] = float(tail_var_sum[x]) / float(count_for_backbone[x]);
			tail_var_avg_no_cuts[x] = float(tail_var_sum_no_cuts[x]) / float(count_for_backbone[x]);
			relaxation_sum_1x_avg[x] = float(relaxation_sum_1x[x]) / float(count_for_backbone[x]);
			relaxation_sum_2x_avg[x] = float(relaxation_sum_2x[x]) / float(count_for_backbone[x]);
			relaxation_sum_3x_avg[x] = float(relaxation_sum_3x[x]) / float(count_for_backbone[x]);


		}
		else
		{
			tail_var_avg[x] = 0;
			tail_var_avg_no_cuts[x] = 0;
			relaxation_sum_1x_avg[x] = 0;
			relaxation_sum_2x_avg[x] = 0;
			relaxation_sum_3x_avg[x] = 0;
		}


	}
	for (int x = 0; x <= no_clauses; x++)
	{
		if (reverse_backbone_1x_count[x] != 0)
		{
			reverse_backbone_1x_avg[x] = float(reverse_backbone_1x[x]) / float(reverse_backbone_1x_count[x]);
		}
		else
		{
			reverse_backbone_1x_avg[x] = 0;
		}

		if (reverse_backbone_2x_count[x] != 0)
		{
			reverse_backbone_2x_avg[x] = float(reverse_backbone_2x[x]) / float(reverse_backbone_2x_count[x]);
		}
		else
		{
			reverse_backbone_2x_avg[x] = 0;
		}

		if (reverse_backbone_3x_count[x] != 0)
		{
			reverse_backbone_3x_avg[x] = float(reverse_backbone_3x[x]) / float(reverse_backbone_3x_count[x]);
		}
		else
		{
			reverse_backbone_3x_avg[x] = 0;
		}
	}*/
	

	//cout << "CUTS AVG:" << tail_var_avg[85] << endl;
	//cout << "NO CUTS AVG:" << tail_var_avg_no_cuts[85] << endl;



}

void Solver::test_suit_final(int test_id, int process_id, int max_time)
{
	mt19937_64 rng(random_device{}());
	vector<vector<vector<bool>>> population_pyramid; // array to hold all current populations
	vector<vector<int>> population_value_pyramid; //fitness of each individual in population

	vector<vector<bool>> population_tab; // array to hold all current populations
	vector<int> population_value; //fitness of each individual in population

	//population_tab.resize(population_size);
	//population_value.resize(population_size);
	this->best_results.clear();
	this->worst_results.clear();
	this->iterations = 0;
	full_ffe = 0;
	partial_ffe = 0;


	auto start = chrono::high_resolution_clock::now();

	Ffe ffe, ffe_partial;

	int iteration = 0;

	population_pyramid.resize(1);
	population_value_pyramid.resize(1);

	int max_el = 0;

	while (true)
	{
		//init new unit
		vector<bool> new_pop;
		new_pop.resize(no_var);
		for (int j = 0; j < no_var; j++)
		{
			new_pop[j] = uniform_int_distribution<>{ 0, 1 }(rng);
		}
		//Fitness
		population_pyramid[0].push_back(new_pop);
		population_value_pyramid[0].push_back(this->fitness(new_pop));

		population_tab.push_back(new_pop);
		population_value.push_back(this->fitness(new_pop));
		//FIHC
		

		//auto start2 = chrono::high_resolution_clock::now();

		if (test_id == 0)
		{
			this->fihc(1.0, population_tab[iteration], population_value[iteration], ffe, ffe_partial, rng);
			for (int i = 0; i < population_tab.size(); i++)
			{
				this->ils1_std_fihc(1.0, population_tab[i], population_value[i], ffe, ffe_partial, rng);
			}
		}
		/*else if (test_id == 1)
		{
			this->fihc(1.0, new_pop, population_value_pyramid[0][iteration], ffe, ffe_partial, rng);
		}*/
		else if (test_id == 1)
		{
			this->fihc(1.0, new_pop, population_value_pyramid[0][iteration], ffe, ffe_partial, rng);
			for (int i = 0; i < population_pyramid[0].size(); i++)
			{
				this->ils1_std_fihc(1.0, population_pyramid[0][i], population_value_pyramid[0][i], ffe, ffe_partial, rng);
			}
		}
		else if (test_id == 2)
		{
			this->fihc(1.0, new_pop, population_value[iteration], ffe, ffe_partial, rng);
			for (int i = 0; i < population_tab.size(); i++)
			{
				this->ils1_std_fihc(1.0, population_tab[i], population_value[i], ffe, ffe_partial, rng);
			}
		}

		if (test_id == 1)
		{
			int temp_start_fitness = this->fitness(population_pyramid[0][iteration]);
			int pyramid_size = population_pyramid.size();
			vector<bool> temp_main_solution = population_pyramid[0].back();
			for (int level_counter = 0; level_counter < pyramid_size; level_counter++)
			{
				int level_size = population_pyramid[level_counter].size() - 1;
				for (int i = 0; i < level_size; i++)
				{
					vector<bool> temp_vec = this->px(temp_main_solution, population_pyramid[level_counter][i], 0, ffe);

					int value_after_px = this->fitness(temp_vec);
					temp_main_solution = temp_vec;
					if (value_after_px > temp_start_fitness) // P3 phase, we add it to the next level
					{
						//check size of pyramid
						if (level_counter + 1 == pyramid_size)
						{
							population_pyramid.resize(level_counter + 2);
							population_value_pyramid.resize(level_counter + 2);
							pyramid_size++;
						}
						//int mid_level_counter = population_pyramid[level_counter].size();
						population_pyramid[level_counter + 1].push_back(temp_vec);
						population_value_pyramid[level_counter + 1].push_back(value_after_px);
						temp_start_fitness = value_after_px;

						break;
					}
					else
					{
						population_pyramid[level_counter][level_size] = temp_vec;
					}
				}


			}

			population_value_pyramid[0][iteration] = this->fitness(population_pyramid[0][iteration]);
		}
		else if (test_id == 2)
		{
			for (int i = 0; i < population_tab.size() - 1; i++)
			{
				vector<bool> temp_vec = this->px(population_tab[iteration], population_tab[i], 0, ffe);
				population_tab[iteration] = temp_vec;

			}
			//end2 = chrono::high_resolution_clock::now();
			//elapsed2 = end2 - start2;
			//cout << "PX  TIME: " << elapsed2.count() << endl;

			population_value[iteration] = this->fitness(population_tab[iteration]);
		}




		// saving best

		int pyramid_size = population_pyramid.size();

		
		int overall_max = 0;
		int overall_max_level = -1;

		for (int level_counter = 0; level_counter < pyramid_size; level_counter++)
		{
			int max_el = *max_element(population_value_pyramid[level_counter].begin(), population_value_pyramid[level_counter].end());

			if (max_el > overall_max)
			{
				overall_max = max_el;
				overall_max_level = level_counter;
			}
		}

		// saving best
		if (test_id == 1)
		{
			best_results.push_back(overall_max);
		}

		
		if (test_id == 0 || test_id == 2)
		{
			max_el = *max_element(population_value.begin(), population_value.end());
			best_results.push_back(max_el);

		}




		if (overall_max == no_clauses || max_el == no_clauses)
		{
			int ind = get_index(population_value_pyramid[overall_max_level], overall_max);
			this->main_solution = population_pyramid[overall_max_level][ind];
			/*for (int i = 0; i < no_var; i++)
			{
				cout << population_tab[ind][i];
			}*/
			break;
		}

		this->iterations = iteration;

		iteration++;
		auto finish = chrono::high_resolution_clock::now();
		chrono::duration<double> elapsed = finish - start;
		//STOP CONDITION

		if (elapsed.count() > max_time)
		{
			cout << "iteration: " << iteration - 1 << " - time -";
			cout << elapsed.count() << endl;
			break;
		}

	}

}

void Solver::test_fulfillment()
{

	mt19937_64 rng(random_device{}());
	vector<vector<bool>> population_tab; // array to hold all current populations
	vector<vector<bool>> population_tab_new; // array to hold all current populations
	vector<int> population_value; //fitness of each individual in population
	int population_size = 100;
	population_tab.resize(population_size);
	population_tab_new.resize(population_size);
	population_value.resize(population_size);
	for (int i = 0; i < population_size; i++)
	{
		population_tab[i].resize(no_var);
	}

	for (int i = 0; i < population_size; i++)
	{
		//cout << "HERE " << i<< endl;
		for (int j = 0; j < no_var; j++)
		{
			population_tab[i][j] = uniform_int_distribution<>{ 0, 1 }(rng);
			//population_tab[i][j] = 1;
		}

	}

	Ffe ffe;// 
	

	// assign value to each individual
	for (int i = 0; i < population_size; i++)
	{
		population_value[i] = this->fitness(population_tab[i]);
		ffe.increment(1);
	}
	int era = 0;

	while (era < 100)
	{
		for (int i = 0; i < population_size; i++)
		{


			//cout << "ERA " << era << endl;

			//this->fihc_relaxation(fihc_propability, population_tab[i], population_value[i], ffe);
			//population_value[i] = this->fitness(population_tab[i]);
			//ffe.increment(1);

			auto start = chrono::high_resolution_clock::now();
			this->ils1(1.0, population_tab[i], population_value[i], ffe, ffe, rng);
			auto finish = chrono::high_resolution_clock::now();
			chrono::duration<double> elapsed = finish - start;
			cout << "MY ILS: " << elapsed.count() << endl;

			population_value[i] = this->fitness(population_tab[i]);

			start = chrono::high_resolution_clock::now();
			this->ils1_std_fihc(1.0, population_tab[i], population_value[i], ffe, ffe, rng);
			finish = chrono::high_resolution_clock::now();
			elapsed = finish - start;
			cout <<"STD ILS: "<< elapsed.count()<<endl;

			population_value[i] = this->fitness(population_tab[i]);

			start = chrono::high_resolution_clock::now();
			vector<vector<int>> relaxation_table = this->create_fullfillment_tab(population_tab[i]);
			for (int r = 0; r < 200; r++)
			{
				int relaxation_cutoff = 0;

				int index_to_change = smart_relaxation_greedy(relaxation_table, relaxation_cutoff, no_clauses, no_var);
				if (index_to_change != -1)
				{
					//cout << index_to_change<<endl;
					population_tab[i][index_to_change] = !population_tab[i][index_to_change];
					this->fast_fullfillment(relaxation_table, population_tab[i], index_to_change);
					/*vector<vector<int>> relaxation_table2 = this->create_fullfillment_tab(population_tab[i]);
					int rows = relaxation_table2.size();
					int columns = 6;

					for (int w = 0; w < rows; w++)
					{
						for (int k = 0; k < columns; k++)
						{
							if (relaxation_table2[w][k] - relaxation_table[w][k] !=0)
							{
								cout << "ERROR IN ROW "<<w << " IN COLUMN: " << k << " VALUE IS " << relaxation_table2[w][k] - relaxation_table[w][k] << endl;
							}
						}

					}*/


				}
				else
				{
					break;
					cout << "HERE BREAK r: " << r << endl;
				}

			}
			finish = chrono::high_resolution_clock::now();
			elapsed = finish - start;
			cout << "MY ILS SLIDING: " << elapsed.count() << endl;


			population_value[i] = this->fitness(population_tab[i]);


			/*population_value[i] = this->fitness(population_tab[i]);
			ffe.increment(1);*/

			era++;


		}

	}
}

void Solver::full_relaxation_test(int population_size, float max_ffe, float fihc_propability)
{
	mt19937_64 rng(random_device{}());
	vector<vector<bool>> population_tab; // array to hold all current populations
	vector<vector<bool>> population_tab_new; // array to hold all current populations
	vector<int> population_value; //fitness of each individual in population
	population_tab.resize(population_size);
	population_tab_new.resize(population_size);
	population_value.resize(population_size);
	for (int i = 0; i < population_size; i++)
	{
		population_tab[i].resize(no_var);
	}


	// 1st get all random population

	for (int i = 0; i < population_size; i++)
	{
		//cout << "HERE " << i<< endl;
		for (int j = 0; j < no_var; j++)
		{
			population_tab[i][j] = uniform_int_distribution<>{ 0, 1 }(rng);
			//population_tab[i][j] = 1;
		}

	}

	Ffe ffe;// 

	// assign value to each individual
	for (int i = 0; i < population_size; i++)
	{
		population_value[i] = this->fitness(population_tab[i]);
		ffe.increment(1);
	}

	int era = 0;

	while (ffe.get_ffe() < max_ffe)
	{
		for (int i = 0; i < population_size; i++)
		{
			if (i % 20 == 0)
				cout << "ILS " << i << endl;

			
			//this->ils1(fihc_propability, population_tab[i], population_value[i], ffe, rng);
			
			//this->fihc_relaxation(fihc_propability, population_tab[i], population_value[i], ffe);
			//population_value[i] = this->fitness(population_tab[i]);
			//ffe.increment(1);

			for(int r = 0; r<20;r++)
			{
				ffe.increment(4);
				vector<vector<int>> relaxation_table = this->create_fullfillment_tab(population_tab[i]); //count fulfillment for given solution
				int relaxation_cutoff = 0;
				int index_to_change = smart_relaxation_version2(relaxation_table, relaxation_cutoff, no_clauses, no_var);
				if (index_to_change != -1)
				{
					//cout << index_to_change<<endl;
					population_tab[i][index_to_change] = !population_tab[i][index_to_change];
				}
				else
				{
					break;
					cout << "HERE BREAK r: "<<r << endl;
				}
			}

			population_value[i] = this->fitness(population_tab[i]);
			ffe.increment(1);

			this->fihc_relaxation_version2(fihc_propability, population_tab[i], population_value[i], ffe);
			/*population_value[i] = this->fitness(population_tab[i]);
			ffe.increment(1);*/







		}


		int max_el = *max_element(population_value.begin(), population_value.end());
		int min_el = *min_element(population_value.begin(), population_value.end());
		cout << "era: " << era << " ffe: " << ffe.get_ffe() << endl;
		cout << "max: " << max_el << " / " << no_clauses << endl;
		cout << "min: " << min_el << endl;

		best_results.push_back(max_el);
		worst_results.push_back(min_el);


		//reassign to new population to default tab



		if (max_el == no_clauses)
		{
			cout << "best solution: ";
			int ind = get_index(population_value, max_el);
			this->main_solution = population_tab[ind];
			for (int i = 0; i < no_var; i++)
			{
				cout << population_tab[ind][i];
			}
			break;
		}

		era++;

	}
}

vector<bool> int_number_to_bool_vector(int number)
{
	vector<bool> final_number;

	
	while (number > 0)
	{
		final_number.push_back(number % 2);
		number = number / 2;
	}
	return final_number;

}

vector<bool> Solver::px(vector<bool> solution1, vector<bool> solution2, int cutoff, Ffe& ffe) //CHYBA DZIAlA
{
	/*int no_var = max3sat.get_varies();
	int no_clauses = max3sat.get_clauses();
	vector<vector<bool>> bool_table = max3sat.get_clause_table().clause_table_bool;
	vector<vector<int>> int_table = max3sat.get_clause_table().clause_table_int;*/
	vector<int> diff_indexes; // vector of indexes in which parents differ
	vector<int> diff_clauses; // clauses with at least 2 of aboce indexes
	vector<bool> results;

	// 1. Differing genes
	for (int i = 0; i < no_var; i++)
	{
		if (solution1[i] != solution2[i])
		{
			diff_indexes.push_back(i);
		}

		//setting results to basic as solution1
		results.push_back(solution1[i]);

	}

	/*for (int i : diff_indexes)
	{
		cout << i << " ";
	}*/
	//cout << endl;

	vector<vector<int>> groups;
	while (true)
	{
		vector<int> group1;
		int length = diff_indexes.size();
		if (length != 0)
		{

			group1.push_back(diff_indexes[0]);
			diff_indexes.erase(diff_indexes.begin());
			length--;

			int index = 0;
			while (true) // XD
			{
				for (int i = 0; i < length; i++)
				{
					int index1 = group1[index];
					int index2 = diff_indexes[i];
					if (simple_connections[index1][index2] > 0)
					{
						group1.push_back(index2);
						diff_indexes.erase(diff_indexes.begin() + i);
						length--;
						i--;
					}
				}
				index++;
				if (group1.size() < (index + 1))
				{
					break;
				}
			}
		}

		if (group1.size() > 0)
		{
			groups.push_back(group1);
		}
		else
		{

			break;
		}



	}
	if (diff_indexes.size() != 0)
	{
		groups.push_back(diff_indexes);
	}

	if (groups.size() == 1) // all variables are dependent so we return initial parent 1
	{
		return solution1;
	}
	/*else if (groups.size()>3)
	{
		cout << "GROUPS SIZE: " << groups.size() << endl;
	}*/
	//else

	/*cout << "GROUPS SIZE: " << groups.size() << endl;
	for (int i : groups[0])
	{
		cout << i << " ";
	}
	cout << endl;
	for (int i : groups[1])
	{
		cout << i << " ";
	}
	cout << endl;*/


	vector<vector<bool>> solutions;
	int no_groups = groups.size();
	int no_solutions = pow(2, no_groups) - 1;
	solutions.resize(no_solutions);
	for (int j = 0; j < no_solutions; j++)
	{


		for (int i = 0; i < no_var; i++)
		{
			solutions[j].push_back(solution1[i]);
		}
	}

	// Po tym kroku mamy tablice 2^n -1 rozwiazan gotowych do modyfikacji, gdzie pierwsze rozwiazanie jest rozwiazaniem bazowym

	for (int i = 1; i < no_solutions; i++)
	{
		vector<bool> temp_sth = int_number_to_bool_vector(i);
		int iterator = 0;
		for (bool j : temp_sth)
		{
			if (j)
			{
				for (int g : groups[iterator])
				{
					solutions[i][g] = solution2[g];
				}
			}


			iterator++;
		}

	}


	/*for (int i = 1; i <= no_solutions; i++)
	{

		solutions[i];
	}*/

	/*for (int i : groups[0])
	{
		solutions[0][i] = solution2[i];

	}
	for (int i : diff_indexes)
	{
		solutions[1][i] = solution2[i];

	}*/


	int main_fit = this->fitness(results);
	ffe.increment(1);
	for (int i = 0; i < no_solutions; i++)
	{
		int temp_fit = this->fitness(solutions[i]);
		if (temp_fit >= main_fit)
		{
			for (int j = 0; j < no_var; j++)
			{
				results[j] = solutions[i][j];
			}
			main_fit = temp_fit;
		}
		ffe.increment(1);

	}

	return results;
}

vector<bool> Solver::px2(vector<bool> solution1, vector<bool> solution2, int &solution1_value)
{
	// Zalozenie: dawca posiada spelnione klauzule ktorych nie posiada biorca. 
	// Operuje na mojej macierzy przejsc (zwanej kiedys relaksacja)
	// 1. tworze normalna maske PX (CHYBA)
	// 2. Sprawdzam ktore geny w masce, naleza do niespelnionych klauzul w biorcy i je oznaczam
	// 3. losouje gen z wyzej oznaczonej puli i przeklejam go z dawcy do biorcy
	// 4. jezeli fitness sie poprawil to zapisujemy wynik do zapamietania na przyszlosc. W kazdym wypadku wynik zostaje w operowanym osobniku
	// 5. usuwam zmieniony gen z maski
	// 6. Wracam do punktu 2, mam teraz prawdopodobnie niespelnione inne klauzule, wiec bedzie nowa inna maska oznaczonych genow
	// 7. Powtarzam tak dlugo jak maska ma jakiekolwiek geny
	
	unordered_set<int> diff_indexes;
	vector<bool> results;
	int results_temp_value = solution1_value;

	for (int i = 0; i < no_var; i++)
	{
		if (solution1[i] != solution2[i])
		{
			diff_indexes.insert(i);
		}

		//setting results to basic as solution1
		results.push_back(solution1[i]);

	}

	unordered_set<int> nf1 = this->not_fulfilled_clauses(solution1);
	unordered_set<int> nf2 = this->not_fulfilled_clauses(solution2);
	vector<int> considered_clauses; //clauses that aren't fulfilled in receiver but are fulfilled in doner;
	for (int i : nf1)
	{	
		if ((nf2.find(i) != nf2.end()) == false)
		{
			considered_clauses.push_back(i);
		}
	}

	set<int> considered_genes; // genes in the above clauses
	for (int i : considered_clauses)
	{
		/*int a = int_table[i][0];
		int b = int_table[i][1];
		int c = int_table[i][2];
		*/
		considered_genes.insert(int_table[i][0]);
		considered_genes.insert(int_table[i][1]);
		considered_genes.insert(int_table[i][2]);

	}

	vector<int> marked_genes; //intersection of PX mask (indexes where 2 given solutions differ) and considered genes

	for (int i : diff_indexes)
	{
		if ((considered_genes.find(i) != considered_genes.end()) == false)
		{
			marked_genes.push_back(i);
		}
	}

	if (marked_genes.size() == 0) // STOP CONDITION
	{
		return results;
	}


	int random_index = rand() % marked_genes.size();
	int random_gene = marked_genes[random_index];
	
	solution1[random_gene] = !solution1[random_gene];
	int new_value;
	new_value = this->fast_fitness(results_temp_value, random_gene, solution1);
	if (new_value >= results_temp_value)
	{
		results = solution1;
		results_temp_value = new_value;
	}



	if (diff_indexes.find(random_gene) != diff_indexes.end())
	{
		diff_indexes.erase(marked_genes[random_index]);
	}


	// KONIEC 1 ITERACJI, TERAZ PETLA PO TYM AZ SIE SKONCZY MASKA

	while (true)
	{
		//auto start = chrono::high_resolution_clock::now();


		nf1 = this->not_fulfilled_clauses(solution1);
		considered_clauses.clear(); //clauses that aren't fulfilled in receiver but are fulfilled in doner;
		for (int i : nf1)
		{
			if ((nf2.find(i) != nf2.end()) == false)
			{
				considered_clauses.push_back(i);
			}
		}

		/*auto finish = chrono::high_resolution_clock::now();
		chrono::duration<double> elapsed = finish - start;
		cout << "TIME 1 : " << elapsed.count() << endl;*/

		//auto start2 = chrono::high_resolution_clock::now();

		considered_genes.clear(); // genes in the above clauses
		for (int i : considered_clauses)
		{
			/*int a = int_table[i][0];
			int b = int_table[i][1];
			int c = int_table[i][2];
			*/
			considered_genes.insert(int_table[i][0]);
			considered_genes.insert(int_table[i][1]);
			considered_genes.insert(int_table[i][2]);

		}

		/*auto finish2 = chrono::high_resolution_clock::now();
		chrono::duration<double> elapsed2 = finish2 - start2;
		cout << "TIME 2 : " << elapsed2.count() << endl;*/


		//auto start3 = chrono::high_resolution_clock::now();


		marked_genes.clear(); //intersection of PX mask (indexes where 2 given solutions differ) and considered genes
		for (int i : diff_indexes)
		{
			if ((considered_genes.find(i) != considered_genes.end()) == false)
			{
				marked_genes.push_back(i);
			}
		}

		/*auto finish3 = chrono::high_resolution_clock::now();
		chrono::duration<double> elapsed3 = finish3 - start3;
		cout << "TIME 3 : " << elapsed3.count() << endl;*/


		if (marked_genes.size() == 0) // STOP CONDITION
		{
			break;
		}

		random_index = rand() % marked_genes.size();
		random_gene = marked_genes[random_index];
		solution1[random_gene] = !solution1[random_gene];


		//auto start4 = chrono::high_resolution_clock::now();

		new_value = this->fast_fitness(results_temp_value, random_gene, solution1);

		/*auto finish4 = chrono::high_resolution_clock::now();
		chrono::duration<double> elapsed4 = finish4 - start4;
		cout << "TIME 4 : " << elapsed4.count() << endl;
		cout << endl;*/

		if (new_value >= results_temp_value)
		{
			results = solution1;
			results_temp_value = new_value;
		}


		if (diff_indexes.find(random_gene) != diff_indexes.end())
		{
			diff_indexes.erase(marked_genes[random_index]);
		}

	}





	solution1_value = results_temp_value; //save as a return info xD

	return results;
}


