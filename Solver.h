#pragma once
#include "Max3Sat.h"
#include <map>
#include<thread>
#include<mutex>
#include <vector>
#include <numeric>
#include<chrono>
#include <functional>
#include <set>
#include <iomanip> 
#include<unordered_set>

using namespace std;

struct Ffe
{
	float ffe_value = 0;

	void increment(float inc)
	{
		ffe_value += inc;
	}
	int get_ffe()
	{
		return ffe_value;
	}

};




class Solver {
private:
	vector<vector<int>> variables_count; //array with counts of variables
	vector<vector<int>> over_fulfilled_varies_main; // array with count of variables fullfillment for given solution

	// VIG !!!!!!
	vector<vector<int>> simple_connections; // array of connections between variables, count of how many times 2 varies are in the same clause, basicly VIG

	vector<vector<vector<int>>> simple_connections_list; // array of lists contaning clauses id
	vector<vector<int>> vig_list; // array of dependent variables for given variable

	vector<set<int>> clause_list_of_dependent_genes; // list of all clauses dependent on genes that depend on single gene. Returns list for given 1 gene.

	vector<vector<bool>> simple_connections_bool;
	vector<vector<int>> complex_connections; // array of connections between variables, more comples count of their relations IDEA TODO
	vector<int> connectivity; // array with count of how much one variable is connected to others
	vector<int> clause_dependence; //  array 
	vector<int> clause_fulfillment_frequency;
	vector<vector<int>> clause_connections; // maxtrix showing which clauses depend on which
	vector<set<int>> clause_connections_list; // list of genes dependent on that clause
	//vector<int> clause_connectivity; // turns out it's just 2x dependency for some reason so as it's linear it makes no sense to keep it
	vector<bool> main_solution;
	int main_result;
	map<int, vector<int>> lut; //mapa LUT ,  list of clauses in which given variable exists
	vector<int> best_results;
	vector<int> worst_results;
	int iterations;
	int full_ffe;
	bool brute_force_improved;
	bool ILS_BF_improved;
	float partial_ffe;

	int brute_force_count; // count of times brute force was used
	int one_off_solutions; // count of 1 off perfect solutions encountered during algorithm

	int best_neighbours; // neighbours with perfect solution that are also perfect solutions
	int one_off_best_neighbours; // neighbours with perfect solution that are 1 off perfect solutions


	double ils_time;
	double px_time;
	double brute_force_time;
	double ils_bf_mask_time;
	double full_time;
	double best_time;

	int plateau_size;
	int backbone_size;

	vector<int> backbone_list;

	//sums
	vector<long long> tail_var_sum;
	vector<long long> tail_var_sum_no_cuts;
	vector<long long> relaxation_sum_1x;
	vector<long long> relaxation_sum_2x;
	vector<long long> relaxation_sum_3x;


	vector<int> count_for_backbone_one_off;
	vector<int> count_for_backbone_two_off;
	vector<int> count_for_backbone_perfect_solutions;
	vector<int> count_for_backbone;

	//averages
	vector<float> tail_var_avg;
	vector<float> tail_var_avg_no_cuts;
	vector<float> relaxation_sum_1x_avg;
	vector<float> relaxation_sum_2x_avg;
	vector<float> relaxation_sum_3x_avg;

	//new things
	vector<int> relaxation_1x_min;
	vector<int> relaxation_2x_min;
	vector<int> relaxation_1x_max;
	vector<int> relaxation_2x_max;

	vector<int> relaxation_3x_min;
	vector<int> relaxation_3x_max;

	//reverse min max
	vector<int> reverse_relaxation_1x_min;
	vector<int> reverse_relaxation_2x_min;
	vector<int> reverse_relaxation_1x_max;
	vector<int> reverse_relaxation_2x_max;

	vector<int> reverse_relaxation_3x_min;
	vector<int> reverse_relaxation_3x_max;


	vector<long long> reverse_backbone_1x;
	vector<long long> reverse_backbone_2x;
	vector<int> reverse_backbone_1x_count;
	vector<int> reverse_backbone_2x_count;
	vector<float> reverse_backbone_1x_avg;
	vector<float> reverse_backbone_2x_avg;

	vector<long long> reverse_backbone_3x;
	vector<int> reverse_backbone_3x_count;
	vector<float> reverse_backbone_3x_avg;

	// TEMPORARY PATCH
	int no_var;
	int no_clauses;
	vector<vector<bool>> bool_table;
	vector<vector<int>> int_table;
	vector<int> gray_code_bit_flips;
	vector<vector<bool>> bool_table_original;
	vector<vector<int>> int_table_original;
	int no_var_og;
	int no_clauses_og;
	//vector<vector<int>> brute_force_masks_blacklist;



public:
	Solver();
	Solver(Max3Sat max3sat);

	void remove_clauses();

	void fihc( float fihc_propability, vector<bool>& solution, int& value, Ffe& ffe, mt19937_64 rng);
	void fihc( float fihc_propability, vector<bool>& solution, int& value, Ffe& ffe, Ffe& ffe_partial, mt19937_64 rng);
	void fihc_restricted(float fihc_propability, vector<bool>& solution, int& value, Ffe& ffe, mt19937_64 rng, int restriction);
	void fihc_relaxation_random(float fihc_propability, vector<bool>& solution, int& value, Ffe& ffe);
	void fihc_relaxation_version2(float fihc_propability, vector<bool>& solution, int& value, Ffe& ffe);
	void fihc_relaxation_version3(float fihc_propability, vector<bool>& solution, int& value, Ffe& ffe, float limit);
	void fihc_relaxation(float fihc_propability, vector<bool>& solution, int& value, Ffe& ffe, Ffe& ffe_partial);
	void fihc_relaxation_restricted(Max3Sat max3sat, float fihc_propability, vector<bool>& solution, int& value, Ffe& ffe, mt19937_64 rng, int restriction);


	void ils_random_my_rfihc(float fihc_propability, vector<bool>& solution, int& value, Ffe& ffe, mt19937_64 rng, float modifier);

	void ils1_my_rfihc(float fihc_propability, vector<bool>& solution, int& value, Ffe& ffe, mt19937_64 rng);
	void ils1_my_rfihc_sizable(float fihc_propability, vector<bool>& solution, int& value, Ffe& ffe, mt19937_64 rng, float ils_size);
	void ils1_my_directed_fihc_sizable(float fihc_propability, vector<bool>& solution, int& value, Ffe& ffe, mt19937_64 rng, float ils_size);
	void ils1_my_directed_fihc_sizable_version2(float fihc_propability, vector<bool>& solution, int& value, Ffe& ffe, mt19937_64 rng, float ils_size, float limit);


	int count_backbone_overlap(vector<bool>& solution);
	int bf_mask_count(vector<bool>& solution, int& value, Ffe& ffe, mt19937_64 rng);

	void ils_bf_mask_cut(vector<bool>& solution, int& value, Ffe& ffe, mt19937_64 rng);

	void ils1( float fihc_propability, vector<bool>& solution, int& value, Ffe& ffe, Ffe& ffe_partial, mt19937_64 rng);
	
	void ils1_std_fihc( float fihc_propability, vector<bool>& solution, int& value, Ffe& ffe, Ffe& ffe_partial, mt19937_64 rng);
	
	void branch_and_bound_m3s(vector<bool>& solution, vector<int> bruteforce_mask, int& value, int max_possible_value, int deep);
	void brute_force(vector<bool>& solution, int& value, int max_possible_value, int& brute_force_count);


	void solutions_diagnosis(int process_id, string save_filename, int test_files_setoff, string alfa);
	void solutions_diagnosis_inside(vector<bool>& vec);

	void create_gray_bit_flips(int size);

	void set_variable_count();
	void clear_lut();
	void set_lut();
	bool check_single_clause(int clause_id, vector<bool>&solution);

	int fitness_wcnf(vector<bool>& solution);
	int fitness(vector<bool> &solution);
	int fast_fitness(int result, int change,vector<bool> &solution);
	unordered_set<int> not_fulfilled_clauses(vector<bool> solution);
	void first_solution(Max3Sat max3sat, int cutoff); //useless
	vector<int> count_fullfillment(vector<bool> solution);
	vector<vector<int>> create_fullfillment_tab( vector<bool>& solution);


	int encode_clause(vector<bool>& solution, int clause_id);
	void fast_fullfillment( vector<vector<int>> &old_tab, vector<bool> solution, int change); 


	void create_simple_connections();
	void create_simple_connections_bool();
	void create_connectivity();
	void create_clause_connections();
	void create_clause_connections_list();
	void create_clause_dep_con();
	void create_clause_list_dependend_genes();


	void dfs_plateau(unordered_set<string>& used_tab, vector<vector<int>>& fulfillment_tab, vector<bool>& solution, int change, int& deep);

	vector<bool> get_main_solution();
	int get_main_result();
	vector<int> get_best_results();
	vector<int> get_worst_results();

	int get_brute_force_count();
	int get_one_offs_count();
	int get_best_neighbours();
	int get_best_one_off_neighbours();
	bool get_brute_force_improved();
	bool get_ILS_BF_improved();

	double get_ils_time();
	double get_px_time();
	double get_bf_time();
	double get_ils_bf_mask_time();
	double get_best_time();
	double get_full_time();
	int get_plateau_size();


	int get_iterations();
	int get_full_ffe();
	float get_partial_ffe();

	//backbone gets
	vector<float> get_tail_var_avg();
	vector<float> get_tail_var_avg_no_cuts();
	vector<float> get_relaxation_sum_avg_1x();
	vector<float> get_relaxation_sum_avg_2x();
	vector<float> get_relaxation_sum_avg_3x();

	vector<int> get_count_for_backbone();
	vector<int> get_count_for_backbone_one_off();
	vector<int> get_count_for_backbone_two_off(); 
	vector<int> get_count_for_backbone_perfect_solutions();

	vector<int> get_backbone_1x_min();
	vector<int> get_backbone_1x_max();
	vector<int> get_backbone_2x_min();
	vector<int> get_backbone_2x_max();
	vector<int> get_backbone_3x_min();
	vector<int> get_backbone_3x_max();


	vector<int> get_reverse_backbone_1x_min();
	vector<int> get_reverse_backbone_1x_max();
	vector<int> get_reverse_backbone_2x_min();
	vector<int> get_reverse_backbone_2x_max();
	vector<int> get_reverse_backbone_3x_min();
	vector<int> get_reverse_backbone_3x_max();


	vector<float> get_reverse_backbone_1x_avg();
	vector<float> get_reverse_backbone_2x_avg();
	vector<float> get_reverse_backbone_3x_avg();


	int get_backbone_size();

	// optymalizacja px itd

	int blackbox_VIG(int process_id);
	int blackbox_VIG_deterministic(int process_id);

	void vig_into_clauses(int process_id);

	
	void test_suit(int population_size, int max_iterations, float fihc_propability, int test_id, int process_id);
	void test_suit2_px(int test_id, int process_id, int max_time);
	void test_suit_p3(int test_id, int process_id, int max_time);
	void test_suit_final(int test_id, int process_id, int max_time);
	void test_fulfillment();
	void full_relaxation_test(int population_size, float max_ffe, float fihc_propability);
	vector<bool> px( vector<bool> solution1, vector<bool> solution2, int cutoff, Ffe& ffe); // partial crossover, returns offspring
	vector<bool> px2(vector<bool> solution1, vector<bool> solution2, int & solution1_value);


};