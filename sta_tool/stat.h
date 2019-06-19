#include <boost/config.hpp>
//#include </usr/include/boost/config.hpp>
#include <iostream>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
//#include </usr/include/boost/graph/adjacency_list.hpp>
#include <boost/property_map/property_map.hpp>
//#include </usr/include/boost/property_map.hpp>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <iterator>
#include <boost/graph/topological_sort.hpp>
//#include </usr/include/boost/graph/topological_sort.hpp>
#include <utility>
#include <list>
#include <set>
#include <math.h>
#include "DFF.h"

extern int clock_count;
extern int last_clock;

using namespace std;


double phi_func(double x);
double psi_func(double x);

class Timing_Analysis {                                           //Class for Timing Analysis and parsing of the delay file


	vector<double> delays, delaye; // new objects
	vector<double> sds, sde; // new objects
/*************************hchen for multi-size********************************/
	double delay_args[29][10];  /*delay_args   0:region0_end; 
                                                   1:region1_end; 
                                                   2:dl_orig; 
                                                   3:sd_orig; 
                                                   4:a0; 
                                                   5:a1;
                                                   6:a2;
                                                   7:b0;
                                                   8:b1;
                                                   9:b2;*/
/*****************************************************************************/
	
//---------------
	double max_delay;
	long long count;
	CADutil *CAD_ob; 
	double in_delay_mean, in_delay_sd, ret_mean, ret_sd, max_mean, max_sd;	//Last two added by Aatreyi
	map<int, double> WIRE_DELAY_MEAN, WIRE_DELAY_SD;        //the various delay values     
	map<int, int > WIRE_DELAY_PATH;			// Added by Kenyon
	vector<double> crit_paths_mean, crit_paths_sd;  //Added by Aatreyi
	int sz;											//Added by Aatreyi
	
	//void compute_logic(int, int);
	void compute_logic(int, int, double, char*, vector<string>);     //hchen for multi-size     //Last two added by Aatreyi
	int changed_state;
	int unchanged_state;
	int phase; // number of input
	int pc;
	set<int> phase_one_set_UC; // unchanged state
	set<int> phase_two_set_UC;
	
	set<int> phase_one_set_C; // changed state
	set<int> phase_two_set_C;
	
	set <int> current_set_C;
	set <int> prev_set_C;
	
	set <int> current_set_UC;
	set <int> prev_set_UC;

	map< int, set<int> > pc_to_set_C;
	map< int, set<int> > pc_to_prev_set_C;
	map< int, set<int> > pc_to_set_UC;
	map< int, set<int> > pc_to_prev_set_UC;
	
	void compare_sets(set<int> set1, set<int> set2, int &m, int &um);
	void set_node_calculation_order();
	map<int, unsigned long long> global_match;
	map<int, unsigned long long> global_non_match;
	vector<int> node_calculation_order;

	map<int,double> delay;
	map<double,int> delay_occurence;
	
	public:
	
       	vector < string > tokenize(std::string token,std::string line);

	vector< double > parseDelays( string  filename);

/************************hchen for multi-size*******************************/
	void parseDelayargs( string  filename);
	double calc_delay(int type, double fanout);
	double calc_sd(int type, double delay);
/***************************************************************************/

	vector< double > parseSDs( string  filename);
	//--------------------------------------
	Timing_Analysis(CADutil *cd, char*, char*, string);
	void call_delay(int* ts, int N);
	void get_delay(int a, int b);
	void statistical_delay_sum(double a, double b, double c, double d);
	void stat_max(double a, double b, double c, double d);
	void report_max_delay(int* ts, int N);
//	void propagate_logic(int *ts, int size);
	void propagate_logic(char* mode, vector<string> exc);//, int seq);//int); //Arguments added by Aatreyi
	void evaluate_union_intersection(); 
	void read_architecture_data(char *fname, int *ts, int size);
	void print_changed_nodes(char*);
	void print_unchanged_nodes();
	void get_pcs(char*);
	void cycle_delay();

	void print_delay(char *);
	void print_path(const char *, int);
};




