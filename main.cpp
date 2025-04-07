#pragma warning(disable:4996)
#include "Clause_Table.h"
#include "Max3Sat.h"
#include "Solver.h"

using namespace std;

mutex m;
int thread_counter = 10;

float distribution_avg(vector<int> vec)
{
	float result;
	int sum = 0; 
	int count = 0;
	for (int i = 0; i < vec.size(); i++)
	{
		sum += vec[i] * i;
		count += vec[i];
	}

	result = float(sum) / float(count);


	return result;
}


float distribution_stdv(vector<int> vec, float avg)
{
	float result;
	float sum = 0;
	int count = 0;
	for (int i = 0; i < vec.size(); i++)
	{
		sum += pow(avg - i,2) * vec[i];
		count += vec[i];
	}

	result = sqrt(sum / float(count));
	return result;
}

float distribution_stdv_rightside(vector<int> vec, float avg)
{
	float result;
	float sum = 0;
	int count = 0;
	for (int i = 0; i < vec.size(); i++)
	{
		if (i > avg)
		{
			sum += pow(avg - i, 2) * vec[i];
			count += vec[i];
		}
	}
	result = sqrt(sum / float(count));
	return result;
}



void start_multi(string filename, string save_filename, int file_size, int process_id, int population_size, int max_iterations, int time_limit, int test_setoff, int test_files_setoff, string alfa)
{
	int thread_setoff = 0;
	while (true)
	{
		Max3Sat* instance = new Max3Sat();
		//cout << "HERE" << endl;
		//cout << filename << endl;
		//cout << save_filename << endl;
		//wcout << ExePath() <<endl;



		//LOADING FROM FILE FUNCTIONS
		// 
		//instance->load_from_wcnf_file(filename);
		//instance->divide_into_3sat();
		//cout << "FILENAME: " << filename << endl;

		if (thread_setoff >= 10)
		{
			filename = "..\\Max3Sat\\files\\P3_" + alfa + "\\" + to_string(file_size) + "\\m3s_" + to_string(file_size) + "_" + to_string(thread_setoff) + ".txt";
		}

		instance->load_from_file(filename, file_size);
		//instance->load_from_generator_file(filename, file_size);




		//cout << "HERE" << endl;
		Solver* solver = new Solver(*instance);

		solver->set_lut();

		//solver->set_variable_count(*instance);
		//solver->first_solution(*instance, 5);
		//solver->create_fullfillment_tab(*instance); 
		solver->create_simple_connections();


		solver->create_connectivity();


		//solver->create_clause_dep_con(*instance);

		solver->create_clause_connections();

		solver->create_clause_connections_list();

		//solver->create_clause_list_dependend_genes();



		//cout << "DIAGNOSIS START FOR FILE: " << process_id << endl;
		//solver->solutions_diagnosis(process_id, save_filename, test_files_setoff, alfa);
		//cout << "DIAGNOSIS COMPLETE FOR FILE: " << process_id << endl;


		//blackbox vig

		//int bb_sum = 0;
		//int iters = 1;
		//for (int i = 0; i < iters; i++)
		//{
		//	//cout << i << ". ";
		//	int a = solver->blackbox_VIG_deterministic(process_id);
		//	//bb_sum += a;

		//	if (a != -1)
		//		cout << "Proces " << process_id << " MISSED "<< a << endl;
		//}
		////cout <<"Proces "<<process_id<< " Srednia: " << float(bb_sum) / float(iters)<<endl;
		//

		//return;


		// vig into clauses

		//solver->vig_into_clauses(process_id);
		//return;


		//solver->create_gray_bit_flips(20);

		//int test_setoff = 3;
		float max_ffe = 10000000;
		float relaxation_propability = 1.0;
		float fihc_propability = 1.0;
		float cutoff = 1.7;
		//float max_time =10;
		//int population_size = 1000; //1000, 200, 1
		//int max_iterations = 200; //200, 1000, 2000000

		//solver->genetic(*instance, population_size, max_ffe, relaxation_propability, fihc_propability, cutoff);
		//solver->ils_tests(*instance, population_size, max_ffe, relaxation_propability);
		//solver->full_relaxation_test(population_size, max_ffe, relaxation_propability);


		int number_of_tests = 2 - test_setoff;
		vector<int> tests_results;
		tests_results.resize(number_of_tests);
		vector<int> tests_iterations;
		tests_iterations.resize(number_of_tests);

		vector<int> tests_full_ffe;
		tests_full_ffe.resize(number_of_tests);
		vector<int> tests_partial_ffe;
		tests_partial_ffe.resize(number_of_tests);
		vector<int> tests_brute_force_count;
		tests_brute_force_count.resize(number_of_tests);
		vector<int> tests_one_off_solutions;
		tests_one_off_solutions.resize(number_of_tests);
		vector<int> tests_best_neighbours;
		tests_best_neighbours.resize(number_of_tests);
		vector<int> tests_one_off_best_neighbours;
		tests_one_off_best_neighbours.resize(number_of_tests);


		vector<int> tests_brute_force_improved;
		tests_brute_force_improved.resize(number_of_tests);
		vector<int> ILS_BF_improved;
		ILS_BF_improved.resize(number_of_tests);
		vector<int> plateua_size;
		plateua_size.resize(number_of_tests);
		vector<double> ils_time;
		ils_time.resize(number_of_tests);
		vector<double> px_time;
		px_time.resize(number_of_tests);
		vector<double> bf_time;
		bf_time.resize(number_of_tests);
		vector<double> ils_bf_mask_time;
		ils_bf_mask_time.resize(number_of_tests);
		vector<double> time_sum;
		time_sum.resize(number_of_tests);
		vector<double> time_best;
		time_best.resize(number_of_tests);

		int clauses = instance->get_clauses();
		// CORELATION FIND
		vector<vector<float>> tail_vars;
		tail_vars.resize(number_of_tests);

		vector<vector<float>> tail_vars_no_cuts;
		tail_vars_no_cuts.resize(number_of_tests);

		vector<vector<float>> relaxation_sum_1x;
		relaxation_sum_1x.resize(number_of_tests);

		vector<vector<float>> relaxation_sum_2x;
		relaxation_sum_2x.resize(number_of_tests);

		vector<vector<float>> relaxation_sum_3x;
		relaxation_sum_3x.resize(number_of_tests);

		//COUNT
		vector<vector<int>> backbone_count;
		backbone_count.resize(number_of_tests);

		vector<vector<int>> backbone_count_one_off;
		backbone_count_one_off.resize(number_of_tests);

		vector<vector<int>> backbone_count_two_off;
		backbone_count_two_off.resize(number_of_tests);

		vector<vector<int>> backbone_count_perfect_solutions;
		backbone_count_perfect_solutions.resize(number_of_tests);

		// MIN MAX
		vector<vector<int>> relaxation_min_1x;
		relaxation_min_1x.resize(number_of_tests);
		vector<vector<int>> relaxation_max_1x;
		relaxation_max_1x.resize(number_of_tests);
		vector<vector<int>> relaxation_min_2x;
		relaxation_min_2x.resize(number_of_tests);
		vector<vector<int>> relaxation_max_2x;
		relaxation_max_2x.resize(number_of_tests);
		vector<vector<int>> relaxation_min_3x;
		relaxation_min_3x.resize(number_of_tests);
		vector<vector<int>> relaxation_max_3x;
		relaxation_max_3x.resize(number_of_tests);

		vector<vector<int>> reverse_relaxation_min_1x;
		reverse_relaxation_min_1x.resize(number_of_tests);
		vector<vector<int>> reverse_relaxation_max_1x;
		reverse_relaxation_max_1x.resize(number_of_tests);
		vector<vector<int>> reverse_relaxation_min_2x;
		reverse_relaxation_min_2x.resize(number_of_tests);
		vector<vector<int>> reverse_relaxation_max_2x;
		reverse_relaxation_max_2x.resize(number_of_tests);
		vector<vector<int>> reverse_relaxation_min_3x;
		reverse_relaxation_min_3x.resize(number_of_tests);
		vector<vector<int>> reverse_relaxation_max_3x;
		reverse_relaxation_max_3x.resize(number_of_tests);

		// REVERSE BACKBONE AVG
		vector<vector<float>> reverse_backbone_1x_avg;
		reverse_backbone_1x_avg.resize(number_of_tests);
		vector<vector<float>> reverse_backbone_2x_avg;
		reverse_backbone_2x_avg.resize(number_of_tests);
		vector<vector<float>> reverse_backbone_3x_avg;
		reverse_backbone_3x_avg.resize(number_of_tests);

		for (int i = 0; i < number_of_tests; i++)
		{
			cout << "TEST NR." << i + test_setoff << " PROCES NR." << process_id << endl;
			solver->test_suit_p3(i + test_setoff, process_id, time_limit);
			//cout << "HERE AFTER TEST" << endl;
			vector<int> best = solver->get_best_results();
			tests_results[i] = *max_element(best.begin(), best.end());
			tests_iterations[i] = solver->get_iterations();
			tests_full_ffe[i] = solver->get_full_ffe();
			tests_partial_ffe[i] = solver->get_partial_ffe();
			tests_brute_force_count[i] = solver->get_brute_force_count();
			tests_one_off_solutions[i] = solver->get_one_offs_count();
			tests_best_neighbours[i] = solver->get_best_neighbours();
			tests_one_off_best_neighbours[i] = solver->get_best_one_off_neighbours();
			tests_brute_force_improved[i] = solver->get_brute_force_improved();
			ILS_BF_improved[i] = solver->get_ILS_BF_improved();
			plateua_size[i] = solver->get_plateau_size();

			ils_time[i] = solver->get_ils_time();
			px_time[i] = solver->get_px_time();
			bf_time[i] = solver->get_bf_time();
			ils_bf_mask_time[i] = solver->get_ils_bf_mask_time();
			time_sum[i] = solver->get_full_time();
			time_best[i] = solver->get_best_time();

			//time_sum[i] = ils_time[i] + px_time[i] + bf_time[i] + ils_bf_mask_time[i];

			tail_vars[i] = solver->get_tail_var_avg();
			tail_vars_no_cuts[i] = solver->get_tail_var_avg_no_cuts();
			relaxation_sum_1x[i] = solver->get_relaxation_sum_avg_1x();
			relaxation_sum_2x[i] = solver->get_relaxation_sum_avg_2x();
			relaxation_sum_3x[i] = solver->get_relaxation_sum_avg_3x();

			backbone_count[i] = solver->get_count_for_backbone();
			backbone_count_one_off[i] = solver->get_count_for_backbone_one_off();
			backbone_count_two_off[i] = solver->get_count_for_backbone_two_off();
			backbone_count_perfect_solutions[i] = solver->get_count_for_backbone_perfect_solutions();


			relaxation_min_1x[i] = solver->get_backbone_1x_min();
			relaxation_max_1x[i] = solver->get_backbone_1x_max();
			relaxation_min_2x[i] = solver->get_backbone_2x_min();
			relaxation_max_2x[i] = solver->get_backbone_2x_max();
			relaxation_min_3x[i] = solver->get_backbone_3x_min();
			relaxation_max_3x[i] = solver->get_backbone_3x_max();

			reverse_relaxation_min_1x[i] = solver->get_reverse_backbone_1x_min();
			reverse_relaxation_max_1x[i] = solver->get_reverse_backbone_1x_max();
			reverse_relaxation_min_2x[i] = solver->get_reverse_backbone_2x_min();
			reverse_relaxation_max_2x[i] = solver->get_reverse_backbone_2x_max();
			reverse_relaxation_min_3x[i] = solver->get_reverse_backbone_3x_min();
			reverse_relaxation_max_3x[i] = solver->get_reverse_backbone_3x_max();

			reverse_backbone_1x_avg[i] = solver->get_reverse_backbone_1x_avg();
			reverse_backbone_2x_avg[i] = solver->get_reverse_backbone_2x_avg();
			reverse_backbone_3x_avg[i] = solver->get_reverse_backbone_3x_avg();


			int best_result = solver->get_main_result();
			/*if (best_result == clauses)
			{
				vector<bool> best_solution = solver->get_main_solution();

				string result_to_save;

				for (bool i : best_solution)
				{
					string to_append;
					if (i)
					{
						to_append = "1";
					}
					else
					{
						to_append = "0";
					}
					result_to_save.append(to_append);
				}
				ofstream outData;
				outData.open(save_filename);


				outData << result_to_save;
				cout << "PROCES " << process_id << " SIE ZAKONCZYL" << endl;
				return;
			}
			return;*/





			//cout << "HERE AFTER SAVES" << endl;
		}
		if (thread_setoff >= 10)
		{
			save_filename = "..\\Max3Sat\\results\\" + alfa + "_0\\" + to_string(file_size) + "\\results" + to_string(thread_setoff) + ".csv";
		}
		ofstream outData;
		outData.open(save_filename);

		//outData << "BACKBONE"<<";";
		//int backbone_size = solver->get_backbone_size();
		//for (int i = 0; i <= backbone_size; i++)
		//{
		//	outData << i << ";";
		//}
		//outData << endl;

		//for (int i = 0; i < number_of_tests; i++)
		//{
		////	/*outData <<"TAIL VARS"<<";";
		////	for (float j : tail_vars[i])
		////	{
		////		outData << int(j) << ";";
		////	}
		////	outData << endl;

		////	outData << "TAIL VARS NO CUTS" << ";";
		////	for (float j : tail_vars_no_cuts[i])
		////	{
		////		outData << int(j) << ";";
		////	}
		////	outData << endl;

		////	outData << "1X" << ";";
		////	for (float j : relaxation_sum_1x[i])
		////	{
		////		outData << int(j) << ";";
		////	}
		////	outData << endl;

		////	outData << "1X MIN" << ";";
		////	for (int j : relaxation_min_1x[i])
		////	{
		////		outData << j << ";";
		////	}
		////	outData << endl;

		////	outData << "1X MAX" << ";";
		////	for (int j : relaxation_max_1x[i])
		////	{
		////		outData << j << ";";
		////	}
		////	outData << endl;

		////	outData << "2X" << ";";
		////	for (float j : relaxation_sum_2x[i])
		////	{
		////		outData << int(j) << ";";
		////	}
		////	outData << endl;

		////	outData << "2X MIN" << ";";
		////	for (float j : relaxation_min_2x[i])
		////	{
		////		outData << j << ";";
		////	}
		////	outData << endl;

		////	outData << "2X MAX" << ";";
		////	for (float j : relaxation_max_2x[i])
		////	{
		////		outData << j << ";";
		////	}
		////	outData << endl;

		////	outData << "3X" << ";";
		////	for (float j : relaxation_sum_3x[i])
		////	{
		////		outData << int(j) << ";";
		////	}
		////	outData << endl;

		////	outData << "3X MIN" << ";";
		////	for (float j : relaxation_min_3x[i])
		////	{
		////		outData << j << ";";
		////	}
		////	outData << endl;

		////	outData << "3X MAX" << ";";
		////	for (float j : relaxation_max_3x[i])
		////	{
		////		outData << j << ";";
		////	}
		////	outData << endl;




		////	int start_nonzero = 0;
		////	int end_nonzero = 0;
		////	int iter = 0;
		////	for (float j : reverse_backbone_1x_avg[i])
		////	{
		////		if (j != 0 )
		////		{
		////			end_nonzero = iter;
		////			if (start_nonzero == 0)
		////			{
		////				start_nonzero = iter;
		////			}
		////		}
		////		iter++;
		////	}

		////	outData << "REVERSE BACKBONE 1X" << ";";
		////	for (int j = start_nonzero; j < end_nonzero; j++)
		////	{
		////		outData << int(reverse_backbone_1x_avg[i][j]) << ";";
		////	}
		////	outData << endl;

		////	outData << "REVERSE BACKBONE 1X MIN" << ";";
		////	for (int j = start_nonzero; j < end_nonzero; j++)
		////	{
		////		outData << int(reverse_relaxation_min_1x[i][j]) << ";";
		////	}
		////	outData << endl;

		////	outData << "REVERSE BACKBONE 1X MAX" << ";";
		////	for (int j = start_nonzero; j < end_nonzero; j++)
		////	{
		////		outData << int(reverse_relaxation_max_1x[i][j]) << ";";
		////	}
		////	outData << endl;



		////	start_nonzero = 0;
		////	end_nonzero = 0;
		////	iter = 0;
		////	for (float j : reverse_backbone_2x_avg[i])
		////	{
		////		if (j != 0)
		////		{
		////			end_nonzero = iter;
		////			if (start_nonzero == 0)
		////			{
		////				start_nonzero = iter;
		////			}
		////		}
		////		iter++;
		////	}
		////	outData << "REVERSE BACKBONE 2X" << ";";
		////	for (int j = start_nonzero; j < end_nonzero; j++)
		////	{
		////		outData << int(reverse_backbone_2x_avg[i][j]) << ";";
		////	}
		////	outData << endl;

		////	outData << "REVERSE BACKBONE 2X MIN" << ";";
		////	for (int j = start_nonzero; j < end_nonzero; j++)
		////	{
		////		outData << int(reverse_relaxation_min_2x[i][j]) << ";";
		////	}
		////	outData << endl;

		////	outData << "REVERSE BACKBONE 2X MAX" << ";";
		////	for (int j = start_nonzero; j < end_nonzero; j++)
		////	{
		////		outData << int(reverse_relaxation_max_2x[i][j]) << ";";
		////	}
		////	outData << endl;


		////	start_nonzero = 0;
		////	end_nonzero = 0;
		////	iter = 0;
		////	for (float j : reverse_backbone_3x_avg[i])
		////	{
		////		if (j != 0)
		////		{
		////			end_nonzero = iter;
		////			if (start_nonzero == 0)
		////			{
		////				start_nonzero = iter;
		////			}
		////		}
		////		iter++;
		////	}
		////	outData << "REVERSE BACKBONE 3X" << ";";
		////	for (int j = start_nonzero; j < end_nonzero; j++)
		////	{
		////		outData << int(reverse_backbone_3x_avg[i][j]) << ";";
		////	}
		////	outData << endl;

		////	outData << "REVERSE BACKBONE 3X MIN" << ";";
		////	for (int j = start_nonzero; j < end_nonzero; j++)
		////	{
		////		outData << int(reverse_relaxation_min_3x[i][j]) << ";";
		////	}
		////	outData << endl;

		////	outData << "REVERSE BACKBONE 3X MAX" << ";";
		////	for (int j = start_nonzero; j < end_nonzero; j++)
		////	{
		////		outData << int(reverse_relaxation_max_3x[i][j]) << ";";
		////	}
		////	outData << endl;

		////	outData << "COUNT 1 OFF" << ";";
		////	for (int j : backbone_count_one_off[i])
		////	{
		////		outData << j << ";";
		////	}
		////	outData << endl;

		////	outData << "COUNT MORE OFF" << ";";
		////	for (int j : backbone_count_two_off[i])
		////	{
		////		outData << j << ";";
		////	}
		////	outData << endl;*/





		//	outData << "COUNT" << ";";
		//	for (int j : backbone_count[i])
		//	{
		//		outData << j << ";";
		//	}
		//	outData << endl;

		//	outData << "PERFECT COUNT" << ";";
		//	for (int j : backbone_count_perfect_solutions[i])
		//	{
		//		outData << j << ";";
		//	}
		//	outData << endl;

		//	float avg = distribution_avg(backbone_count[i]);
		//	outData << "AVG" << ";" << avg;
		//	outData << endl;
		//	outData << "AVG NORM" << ";" << avg / backbone_count[i].size();
		//	outData << endl;
		//	float stdv = distribution_stdv(backbone_count[i], avg);
		//	outData << "STDV" << ";" << stdv;
		//	outData << endl;
		//	outData << "STDV NORM" << ";" << stdv / backbone_count[i].size();
		//	outData << endl;
		//	float stdv_right = distribution_stdv_rightside(backbone_count[i], avg);
		//	outData << "STDV RIGHT" << ";" << stdv_right;
		//	outData << endl;
		//	outData << "STDV RIGHT NORM" << ";" << stdv_right / backbone_count[i].size();
		//	outData << endl;
		//	float score = avg*0.8 + stdv*0.2;
		//	outData << "score" << ";" << score;
		//	outData << endl;
		//	outData << "score NORM" << ";" << score / backbone_count[i].size();
		//	outData << endl;
		//	float score_right = avg * 0.8 + stdv_right * 0.2;
		//	outData << "score right" << ";" << score_right;
		//	outData << endl;
		//	outData << "score right NORM" << ";" << score_right / backbone_count[i].size();
		//	outData << endl;

		//	float best_10, best_5, best_2;
		//	float best_10_sum = 0, best_5_sum = 0, best_2_sum = 0;
		//	float best_10_count = 0, best_5_count = 0, best_2_count = 0;
		//	float best_10_norm, best_5_norm, best_2_norm;
		//	float best_10_avg_dist, best_5_avg_dist, best_2_avg_dist;

		//	int backbone_count_sum = 0;
		//	for (int j : backbone_count[i])
		//	{
		//		backbone_count_sum += j;
		//	}
		//	cout << backbone_count_sum;

		//	int temp_sum = 0;
		//	int a = 0;
		//	for (int j : backbone_count[i])
		//	{
		//		if (temp_sum >= float(backbone_count_sum) * 0.9) // top 10%
		//		{
		//			best_10_sum += a * j;
		//			best_10_count+=j;
		//		}
		//		if (temp_sum >= float(backbone_count_sum) * 0.95) // top 5%
		//		{
		//			best_5_sum += a * j;
		//			best_5_count += j;
		//		}
		//		if (temp_sum >= float(backbone_count_sum) * 0.98) // top 2%
		//		{
		//			best_2_sum += a * j;
		//			best_2_count += j;
		//		}

		//		temp_sum += j;
		//		a++;
		//	}
		//	best_10 = best_10_sum / best_10_count;
		//	best_5 = best_5_sum / best_5_count;
		//	best_2 = best_2_sum / best_2_count;
		//	best_10_norm = best_10 / backbone_count[i].size();
		//	best_5_norm = best_5 / backbone_count[i].size();
		//	best_2_norm = best_2 / backbone_count[i].size();
		//	best_10_avg_dist = best_10 - avg;
		//	best_5_avg_dist = best_5 - avg;
		//	best_2_avg_dist = best_2 - avg;

		//	float best_10_end_dist = 0, best_5_end_dist = 0, best_2_end_dist = 0;
		//	best_10_end_dist = backbone_count[i].size() - best_10;
		//	best_5_end_dist = backbone_count[i].size() - best_5;
		//	best_2_end_dist = backbone_count[i].size() - best_2;

		//	outData << "TOP 10% AVG" << ";" << best_10;
		//	outData << endl;
		//	outData << "TOP 10% AVG NORM" << ";" << best_10_norm;
		//	outData << endl;
		//	outData << "TOP 10% AVG DIST FROM WHOLE AVG" << ";" << best_10_avg_dist;
		//	outData << endl;
		//	outData << "TOP 10% AVG DIST FROM FULL BACKBONE" << ";" << best_10_end_dist;
		//	outData << endl;

		//	outData << "TOP 5% AVG" << ";" << best_5;
		//	outData << endl;
		//	outData << "TOP 5% AVG NORM" << ";" << best_5_norm;
		//	outData << endl;
		//	outData << "TOP 5% AVG DIST FROM WHOLE AVG" << ";" << best_5_avg_dist;
		//	outData << endl;
		//	outData << "TOP 5% AVG DIST FROM FULL BACKBONE" << ";" << best_5_end_dist;
		//	outData << endl;

		//	outData << "TOP 2% AVG" << ";" << best_2;
		//	outData << endl;
		//	outData << "TOP 2% AVG NORM" << ";" << best_2_norm;
		//	outData << endl;
		//	outData << "TOP 2% AVG DIST FROM WHOLE AVG" << ";" << best_2_avg_dist;
		//	outData << endl;
		//	outData << "TOP 2% AVG DIST FROM FULL BACKBONE" << ";" << best_2_end_dist;
		//	outData << endl;

		//}
		//return;


		outData << "nr_testu" << ";" << "najlepsza_ocena" << ";" << "najlepsza_ocena %" << ";" << "liczba wykonanych iteracji" << ";"
			//<< "brute force count" << ";" << "one offs count" << ";" << "perfect solutions" << ";" << "one offs perfect solution" << ";" << "brute force improved" <<";"
			<< "ILS BF improved" << ";"
			<< "Plateau size" << ";" << "ILS TIME" << ";" << "PX TIME" << ";" << "SLIDING TIME" << ";" << "ILS BF MASK TIME" << ";" << "TIME SUM" << ";" << "BEST TIME" << endl;
		for (int i = 0; i < number_of_tests; i++)
		{
			outData << i + test_setoff << ";" << float(tests_results[i]) << ";" << float(tests_results[i]) / float(clauses) << ";" << tests_iterations[i] << ";"
				//<< tests_brute_force_count[i] << ";" << tests_one_off_solutions[i]<< ";" << tests_best_neighbours[i] << ";" << tests_one_off_best_neighbours[i] << ";" << tests_brute_force_improved[i] <<";"
				<< ILS_BF_improved[i] << ";"
				<< plateua_size[i] << ";" << ils_time[i] / time_sum[i] << ";" << px_time[i] / time_sum[i] << ";" << bf_time[i] / time_sum[i] << ";" << ils_bf_mask_time[i] / time_sum[i] << ";" << double(time_sum[i]) << ";" << double(time_best[i]) << endl;
		}

		//bool end_this_thread = false;
		m.lock();
		cout << "thread number " << process_id << " file: " << thread_counter << endl;
		if (thread_counter < 30)
		{
			thread_counter++;
			thread_setoff = thread_counter - 1;
			m.unlock();
		}
		else
		{
			m.unlock();
			break;
		}
	}
	cout << "PROCES " << process_id << " SIE ZAKONCZYL" << endl;

}

wstring ExePath() {
	TCHAR buffer[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
	return std::wstring(buffer).substr(0, pos);
}

int main()
{
	srand(time(NULL));



	//vector<thread> thr;
	int thread_number = 6;
	//thr.resize(thread_number);

	int file_size;
	int population_size; //1000, 200, 1
	int max_iterations; //200, 1000, 2000000
	int time_limit;

	string zero;
	fstream handler;
	string holder;
	int test_setoff;
	int test_files_setoff;
	int beta;
	string alfa;
	string filename = "..\\Max3Sat\\settings.txt";
	handler.open(filename.c_str());
	handler >> population_size;
	handler >> max_iterations;
	handler >> file_size;
	handler >> thread_number;
	handler >> time_limit;
	handler >> test_setoff;
	handler >> test_files_setoff;
	handler >> beta;
	handler >> alfa;
	vector<thread> thr;
	thr.resize(thread_number);

	thread_counter = thread_number;

	string path = "..\\Max3Sat\\files\\P3_"+alfa +"\\" + to_string(file_size) + "\\m3s_" + to_string(file_size) + "_";
	//string path = "..\\Max3Sat\\files\\" +alfa+ "\\" + to_string(file_size) + "\\"+to_string(beta) + "\\" + to_string(file_size) + "_";
	//string path = "..\\Max3Sat\\files\\wcnf\\file_2.wcnf";
	string endpath = "..\\Max3Sat\\results\\" +alfa+"_0\\"+ to_string(file_size)+"\\results";
	//string endpath = "..\\Max3Sat\\results\\" + alfa + "\\" + to_string(file_size) +"\\" + to_string(beta) + "\\results";
	for (int i = 0; i < thread_number; i++)
	{
		thr[i] = thread(start_multi, path + to_string(i+ test_files_setoff) + ".txt", endpath + to_string(i+ test_files_setoff) + ".csv", file_size, i, population_size, max_iterations, time_limit, test_setoff, test_files_setoff, alfa);
		//thr[i] = thread(start_multi, path, endpath + to_string(i + test_files_setoff) + ".csv", file_size, i, population_size, max_iterations, time_limit, test_setoff, test_files_setoff);
	}

	for (int i = 0; i < thread_number; i++)
	{
		thr[i].join();
	}

	/*beta = 5;
	path = "..\\Max3Sat\\files\\" + alfa + "\\" + to_string(file_size) + "\\" + to_string(beta) + "\\" + to_string(file_size) + "_";
	endpath = "..\\Max3Sat\\results\\" + alfa + "\\" + to_string(file_size) + "\\" + to_string(beta) + "\\results";
	for (int i = 0; i < thread_number; i++)
	{
		thr[i] = thread(start_multi, path + to_string(i + test_files_setoff) + ".txt", endpath + to_string(i + test_files_setoff) + ".csv", file_size, i, population_size, max_iterations, time_limit, test_setoff, test_files_setoff, alfa);
	}
	for (int i = 0; i < thread_number; i++)
	{
		thr[i].join();
	}*/



}