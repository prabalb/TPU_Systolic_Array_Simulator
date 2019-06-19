#ifndef _CAD_H_ 
#define _CAD_H_


#include <boost/config.hpp>
#include <iostream>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/algorithm/string.hpp>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <iterator>
#include <boost/graph/topological_sort.hpp>
#include <utility>
#include <list>
#include <math.h>
#include <assert.h>
#include "DFF.h"
#include <gmp.h>
#include <gmpxx.h>

using namespace std;
using namespace boost;

typedef property<edge_weight_t, double> DistanceProperty;		//property defined for the edges
typedef adjacency_list<vecS, vecS, directedS, property<vertex_name_t, string>, DistanceProperty> MyGraphType;	//Declaring a directed graph

typedef property_map<MyGraphType, edge_weight_t>::type WeightMap;

typedef graph_traits < MyGraphType >::vertex_descriptor vertex_descriptor;		//Defining vertex and edge descriptors
typedef graph_traits < MyGraphType >::edge_descriptor edge_descriptor;

typedef std::map<string, vertex_descriptor> NameVertexMap;

typedef std::vector< vertex_descriptor > container;

class CADutil{				//CADutil class - Used for parsing the verilog file 
	
	char* v;	
	char* dl;		
	NameVertexMap nodes;
	NameVertexMap::iterator pos;
	WeightMap distances;
	map<int, int> STATE;
	
	public:

	map<string, int> nodes_name;
	map<int, string> index_to_node;
	map<int, int> GATES;
	map<int, string> GATENAMES;		// added by Kenyon
	
	map<int, float> GATE_FOUT;             //hchen for gate fan-out
	map<int, float> GATE_DRIVE;
	map<int, float> GATE_LOAD;

	multimap<int, int> edges_from; 
	MyGraphType G_ref;
	map<int, double> GATE_DELAY_MEAN, GATE_DELAY_SD, COMBI_DELAY_MEAN, COMBI_DELAY_SD, TOTAL_DELAY_MEAN;


	map<int, bool> node_ready;		// States whether nodes are ready for computation
	vector<D_flipflop> D_flipflops;		// Contains all the flip flops in the circuit.
	//vector<int> input_list;
	set<int> input_list;
	//vector<int> output_list;
	set<int> output_list;
	map<string, int> input_size;
	map<string, int> output_size;
	map<string, int> constant_output;	
	
	CADutil(char* s, char* d_new);
	void parse_syn_v(MyGraphType& G_ref);
	void parse_inverter(std::string line, int inv_type, string inv_name, MyGraphType& G_ref);	// parameter inv_name added by Kenyon
//	void parse_inverter(std::string line, MyGraphType& G_ref);
	void del_spaces(std::string &s);
	void parse_nand_and_nor(std::string s_nand_nor, int type, string gate_name, MyGraphType& G_ref); //parameter added by Kenyon
	void parse_delay_file();
	void set_logic_state(int key, int value) {  /*cout << "Setting " << index_to_node[key] << " to " << value << endl;*/ STATE[key] = value;}
	int get_logic_state(int key) { /*assert(STATE.find(key) != STATE.end())*/; return STATE[key]; }
	void parse_input_file(char * fname);
	void report_inputs();
	void report_adder_out();
	void initialize_logic_state();
	void report_current_logic_states();
	void create_input_set(string inp1, string inp2, bool sub);

	void parse_input(string);
	void parse_output(string);
	void parse_D_flipflop(string, MyGraphType&, DFF_type);
	//void parse_D_flipflop(string, int, MyGraphType&);
	void set_input(string, mpz_class);
	void print_outputs(ofstream &);
	void print_inputs(ofstream &);
	void print_inputs();
	void print_outputs();
	void report_current_logic_states(ofstream &);
	void parse_constant_output(string);
	void report_logic_depth();
	int find_depth(int);
	int get_type(string s);
	float get_size(int type);
};

#endif
