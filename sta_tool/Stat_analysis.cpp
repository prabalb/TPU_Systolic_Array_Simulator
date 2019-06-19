#include "CAD.h"
#include "stat.h"
#include <queue>
#include <iterator>
#include <map>
#include <fstream>
#include <ctime>

int last_clock = 0;
//srand(time(0));

Timing_Analysis::Timing_Analysis(CADutil *cd, char *voltage, char *mode, string ans)//, char* PC_FILE)
	: CAD_ob(cd), phase(0)//, global_match(0), global_non_match(0)
{

	count=0;
	phase_one_set_UC.clear();
	phase_two_set_UC.clear();

	phase_one_set_C.clear();
	phase_two_set_C.clear();

	set_node_calculation_order();

	for (set<int>::iterator it = cd->input_list.begin(); it != cd->input_list.end(); it++)
	{
//		delay[*it] = 0;
	        WIRE_DELAY_MEAN[*it] = 0.0;
                WIRE_DELAY_SD[*it] = 0.0;
	}
	for (vector<D_flipflop>::iterator it = cd->D_flipflops.begin(); it != cd->D_flipflops.end(); it++)
	{
//		delay[it->Q_index] = 0;
		WIRE_DELAY_MEAN[it->Q_index] = 0.0;
                WIRE_DELAY_SD[it->Q_index] = 0.0;
	}

if(!strcmp(mode,"1"))
{
        if(!strcmp(voltage,"0.85"))
	 {delays=parseDelays("./16_0.85_70_nostress_fin.txt");
	  sds=parseSDs("./16_0.85_70_nostress_fin.txt");}
        else if(!strcmp(voltage,"0.76"))
	 {delays=parseDelays("./22_0.76_110_nostress.txt");
	  sds=parseSDs("./22_0.76_110_nostress.txt");}
        else if(!strcmp(voltage,"0.72"))
	 {delays=parseDelays("./22_0.72_110_nostress.txt");
	  sds=parseSDs("./22_0.72_110_nostress.txt");}
        else if(!strcmp(voltage,"0.68"))
	 {delays=parseDelays("./22_0.68_110_nostress.txt");
	  sds=parseSDs("./22_0.68_110_nostress.txt");}
        else if(!strcmp(voltage,"0.64"))
	 {delays=parseDelays("./22_0.64_110_nostress.txt");
	  sds=parseSDs("./22_0.64_110_nostress.txt");}
        else if(!strcmp(voltage,"0.6"))
	 {delays=parseDelays("./22_0.6_110_nostress.txt");
	  sds=parseSDs("./22_0.6_110_nostress.txt");}
        else if(!strcmp(voltage,"0.56"))
	 {delays=parseDelays("./22_0.56_110_nostress.txt");
	  sds=parseSDs("./22_0.56_110_nostress.txt");}
        else if(!strcmp(voltage,"1"))
	 {delays=parseDelays("./45_1_110_nostress.txt");
	  sds=parseSDs("./45_1_110_nostress.txt");}
	// AATREYI's FILES^M
	 else if(!strcmp(voltage,"0.25"))        
         {delays=parseDelays("./16_0.25_70_nostress_fin.txt");
          sds=parseSDs("./16_0.25_70_nostress_fin.txt");}
	 else if(!strcmp(voltage,"0.35"))        
         {delays=parseDelays("./16_0.35_70_nostress_fin.txt");
          sds=parseSDs("./16_0.35_70_nostress_fin.txt");}
	 else if(!strcmp(voltage,"0.45"))        
         {delays=parseDelays("./16_0.45_70_nostress_fin.txt");
          sds=parseSDs("./16_0.45_70_nostress_fin.txt");}
        else if(!strcmp(voltage,"0.55"))
         {delays=parseDelays("./16_0.55_70_nostress_fin.txt");
          sds=parseSDs("./16_0.55_70_nostress_fin.txt");}
        else if(!strcmp(voltage,"0.65"))
         {delays=parseDelays("./16_0.65_70_nostress_fin.txt");
          sds=parseSDs("./16_0.65_70_nostress_fin.txt");}
        else if(!strcmp(voltage,"0.75"))
         {delays=parseDelays("./16_0.75_70_nostress_fin.txt");
          sds=parseSDs("./16_0.75_70_nostress_fin.txt");}
        else if(!strcmp(voltage,"0.85"))
         {delays=parseDelays("./16_0.85_70_nostress_fin.txt");
          sds=parseSDs("./16_0.85_70_nostress_fin.txt");}
	// AATREYI's FILES^M

}else{

	if(!strcmp(voltage,"0.85"))
	 {delays=parseDelays("./16_0.85_70_stress_fin.txt");
	  sds=parseSDs("./16_0.85_70_stress_fin.txt");}
        else if(!strcmp(voltage,"0.76"))
	 {delays=parseDelays("./22_0.76_110_stress.txt");
	  sds=parseSDs("./22_0.76_110_stress.txt");}
        else if(!strcmp(voltage,"0.72"))
	 {delays=parseDelays("./22_0.72_110_stress.txt");
	  sds=parseSDs("./22_0.72_110_stress.txt");}
        else if(!strcmp(voltage,"0.68"))
	 {delays=parseDelays("./22_0.68_110_stress.txt");
	  sds=parseSDs("./22_0.68_110_stress.txt");}
        else if(!strcmp(voltage,"0.64"))
	 {delays=parseDelays("./22_0.64_110_stress.txt");
	  sds=parseSDs("./22_0.64_110_stress.txt");}
        else if(!strcmp(voltage,"0.6"))
	 {delays=parseDelays("./22_0.6_110_stress.txt");
	  sds=parseSDs("./22_0.6_110_stress.txt");}
        else if(!strcmp(voltage,"0.56"))
	 {delays=parseDelays("./22_0.56_110_stress.txt");
	  sds=parseSDs("./22_0.56_110_stress.txt");}

	// AATREYI's FILES
	else if(!strcmp(voltage,"0.25"))        
         {delays=parseDelays("./16_0.25_70_stress_fin.txt");
          sds=parseSDs("./16_0.25_70_stress_fin.txt");}
	else if(!strcmp(voltage,"0.35"))        
         {delays=parseDelays("./16_0.35_70_stress_fin.txt");
          sds=parseSDs("./16_0.35_70_stress_fin.txt");}
        else if(!strcmp(voltage,"0.45"))	
	 {delays=parseDelays("./16_0.45_70_stress_fin.txt");
	  sds=parseSDs("./16_0.45_70_stress_fin.txt");}
        else if(!strcmp(voltage,"0.55"))
	 {delays=parseDelays("./16_0.55_70_stress_fin.txt");
	  sds=parseSDs("./16_0.55_70_stress_fin.txt");}
        else if(!strcmp(voltage,"0.65"))
	 {delays=parseDelays("./16_0.65_70_stress_fin.txt");
	  sds=parseSDs("./16_0.65_70_stress_fin.txt");}
        else if(!strcmp(voltage,"0.75"))
	 {delays=parseDelays("./16_0.75_70_stress_fin.txt");
	  sds=parseSDs("./16_0.75_70_stress_fin.txt");}
        else if(!strcmp(voltage,"0.85"))
	 {delays=parseDelays("./16_0.85_70_stress_fin.txt");
	  sds=parseSDs("./16_0.85_70_stress_fin.txt");}
	// AATREYI's FILES
} 
	if((!strcmp(mode,"1")) && (!ans.compare("y"))){
		if(!strcmp(voltage,"0.8"))
		 {delaye=parseDelays("./22_0.8_110_stress.txt");
		  sde=parseSDs("./22_0.8_110_stress.txt");}
		else if(!strcmp(voltage,"0.76"))
		 {delaye=parseDelays("./22_0.76_110_stress.txt");
		  sde=parseSDs("./22_0.76_110_stress.txt");}
		else if(!strcmp(voltage,"0.72"))
		 {delaye=parseDelays("./22_0.72_110_stress.txt");
		  sde=parseSDs("./22_0.72_110_stress.txt");}
		else if(!strcmp(voltage,"0.68"))
		 {delaye=parseDelays("./22_0.68_110_stress.txt");
		  sde=parseSDs("./22_0.68_110_stress.txt");}
		else if(!strcmp(voltage,"0.64"))
		 {delaye=parseDelays("./22_0.64_110_stress.txt");
		  sde=parseSDs("./22_0.64_110_stress.txt");}
		else if(!strcmp(voltage,"0.6"))
		 {delaye=parseDelays("./22_0.6_110_stress.txt");
		  sde=parseSDs("./22_0.6_110_stress.txt");}
		else if(!strcmp(voltage,"0.56"))
		 {delaye=parseDelays("./22_0.56_110_stress.txt");
		  sde=parseSDs("./22_0.56_110_stress.txt");}
		else if(!strcmp(voltage,"1"))
		 {delaye=parseDelays("./45_1_110_stress.txt");
		  sde=parseSDs("./45_1_110_stress.txt");}
		// AATREYI's FILES^M
	         else if(!strcmp(voltage,"0.25"))        
                  {delaye=parseDelays("./16_0.25_70_stress_fin.txt");
                  sde=parseSDs("./16_0.25_70_stress_fin.txt");}
	 	else if(!strcmp(voltage,"0.35"))        
        	 {delaye=parseDelays("./16_0.35_70_stress_fin.txt");
         	 sde=parseSDs("./16_0.35_70_stress_fin.txt");}
		else if(!strcmp(voltage,"0.45"))        
	         {delaye=parseDelays("./16_0.45_70_stress_fin.txt");
          	  sde=parseSDs("./16_0.45_70_stress_fin.txt");}
        	else if(!strcmp(voltage,"0.55"))
         	 {delaye=parseDelays("./16_0.55_70_stress_fin.txt");
         	  sde=parseSDs("./16_0.55_70_stress_fin.txt");}
        	else if(!strcmp(voltage,"0.65"))
        	 {delaye=parseDelays("./16_0.65_70_stress_fin.txt");
        	  sde=parseSDs("./16_0.65_70_stress_fin.txt");}
        	else if(!strcmp(voltage,"0.75"))
        	 {delaye=parseDelays("./16_0.75_70_stress_fin.txt");
        	  sde=parseSDs("./16_0.75_70_stress_fin.txt");}
        	else if(!strcmp(voltage,"0.85"))
        	 {delaye=parseDelays("./16_0.85_70_stress_fin.txt");
        	  sde=parseSDs("./16_0.85_70_stress_fin.txt");}
		// AATREYI's FILES^M

	}

	parseDelayargs("./test.txt");
 //pramesh

//	for(int i=0;i<29;i++){
//		for(int j=0;j<9;j++)
//			cout<<delay_args[i][j]/delay_args[i][0]<<" ";
//		cout<<endl;
//	}
}

vector< std::string > Timing_Analysis::tokenize(std::string token,std::string line)
{
  std::string numbers_str = line;
  std::vector < std::string > numbers; 
  std::string  temp;

  while (numbers_str.find(token, 0) != std::string::npos)
  { 
    size_t  pos = numbers_str.find(token, 0); 
    temp = numbers_str.substr(0, pos);      
    numbers_str.erase(0, pos + 1);          
    numbers.push_back(temp);                
  }

  numbers.push_back(numbers_str);           
 
return numbers;

}

vector<double> Timing_Analysis::parseDelays (string  filename)
{
	vector < double > delays;
	//cout << filename << endl;
	std::vector < std::string > fields;	
	string line;
	string lines[3];
	//string lines[29]; // 6 nand, 12 nor, 11 inv
	ifstream myfile (filename.c_str());
	if (myfile.is_open())
	{
		int i=0;
		
		while ( myfile.good() )
		{
			getline (myfile,line);
			lines[i]=line;
			i++;
			if(i==3)break;
		//	if (i==29)break;
		}
		
		myfile.close();
	}

	else cout << "Unable to open file"; 

	for(int j=0;j<3;j++)
//	for(int j=0;j<29;j++)
	{
		fields=tokenize("\t",lines[j]);
//		cout << "fields: " << fields[1].c_str() << endl;
		delays.push_back(atof(fields[1].c_str()));
	}

	return delays;
}

void Timing_Analysis::parseDelayargs (string  filename)
{
//	vector < double > delays;

	std::vector < std::string > fields;	
	string line;
	//string lines[3];
	string lines[18]; // 6 nand, 12 nor, 11 inv
	ifstream myfile (filename.c_str());

	if (myfile.is_open())
	{
		int i=0;
		
		while ( myfile.good() )
		{
			getline (myfile,line);
			lines[i]=line;
			i++;
		//	if(i==3)break;
			if (i==18)
			break;
		}
		
		myfile.close();

	}

	else cout << "Unable to open file"; 

//	for(int j=0;j<3;j++)
	for(int j=0;j<18;j++)
	{

		fields=tokenize("\t",lines[j]);
//		cout << "fields: " << fields[1].c_str() << endl;
		for(int k=0;k<6;k++)
			delay_args[j][k] = atof(fields[k].c_str());
//		delays.push_back(atof(fields[1].c_str()));

	}

//	return delays;
}

vector<double> Timing_Analysis::parseSDs (string  filename)
{
	vector < double > SDs;
	std::vector < std::string > fields;	
	string line;
	string lines[3];
//	string lines[29];
	ifstream myfile (filename.c_str());
	if (myfile.is_open())
	{
		int i=0;

		while ( myfile.good() )
		{
			getline (myfile,line);
			lines[i]=line;
			i++;
			if(i==3)break;
			//if(i==29)break;
		}
		
		myfile.close();
	}

	else cout << "Unable to open file"; 

	for(int j=0;j<3;j++)
//	for(int j=0;j<29;j++)
	{
		fields=tokenize("\t",lines[j]);
//		cout << "fields: " << fields[2].c_str() << endl;
		SDs.push_back(atof(fields[2].c_str()));
	}

	return SDs;
}

void Timing_Analysis::set_node_calculation_order()
{
//	cout << "Finding optimal node calculation order." << endl;
//cout << "Here\n";
	pair<multimap<int, int>::iterator, multimap<int, int>::iterator> edges;
	multimap<int, int>::iterator input1;
	multimap<int, int>::iterator input2;
	//cout << "Problem\n";
	queue<int> uncalculated_nodes;
	int node_to_calc;
//cout << "Error\n";
    // debug
    //std::cout << "Size of node_name (inside set_node_calculation_order()) : " << CAD_ob->nodes_name.size() << std::endl;
    //for (map<string, int>::iterator it = CAD_ob->nodes_name.begin(); it != CAD_ob->nodes_name.end(); ++it)
	//{
    //  std::cout << "net name: " << it->first << ", value: " << it->second << std::endl;
    //}

	for (map<string, int>::iterator it = CAD_ob->nodes_name.begin(); it != CAD_ob->nodes_name.end(); ++it)
	{
		if (CAD_ob->GATES.find(it->second) != CAD_ob->GATES.end())
		{		
            //std::cout << "within set_node_calculation_order, setting " << it->first << " to false ..." << std::endl;
			CAD_ob->node_ready[it->second] = false;		// Set all gate outputs to not ready	
			uncalculated_nodes.push(it->second);		// Create a queue of all the gates
		} else {
          //std::cout << "DBG: " << it->first << std::endl;
        }
	}

    // debug
    if(0) {
      std::cout << "====DEBUG_START====" << std::endl;
      for (map<string, int>::iterator it = CAD_ob->nodes_name.begin(); it != CAD_ob->nodes_name.end(); ++it) {
        if(CAD_ob->node_ready[it->second] == true) {
          std::cout << it->first << std::endl;
        }
        if(it->first == "route_ip_ivc_orc_24") {
          if(CAD_ob->node_ready.find(it->second) != CAD_ob->node_ready.end()) {
            std::cout << "found: " << it->first << ", " << CAD_ob->node_ready[it->second] << std::endl;
          }
        }
      }
      std::cout << "====DEBUG_END====" << std::endl;
    }

	while(!uncalculated_nodes.empty())				// Repeat until all gates are ordered
	{					
		node_to_calc = uncalculated_nodes.front();
		uncalculated_nodes.pop();

		edges = CAD_ob->edges_from.equal_range(node_to_calc);	// Get inputs to gate
		if (CAD_ob->edges_from.count(node_to_calc) == 1)	// Check if inputs have been ordered
		{			
			input1 = edges.first;		// Inverter
			input2 = edges.first;		// Pretend that there are two inputs from the same location
		}
		else
		{
			input2 = edges.first;		// NAND or NOR gate
			++input2;
			input1 = edges.first;
		}

		if (CAD_ob->node_ready[input1->second] && CAD_ob->node_ready[input2->second])	// If both inputs have been ordered
		{
			node_calculation_order.push_back(node_to_calc);
			CAD_ob->node_ready[node_to_calc] = true;
		}
		else if (CAD_ob->constant_output.find( CAD_ob->index_to_node[node_to_calc] ) != CAD_ob->constant_output.end())
		{
			cout << "Constant output" << endl;
			sleep (1);
		}
		else
		{
			uncalculated_nodes.push(node_to_calc);		// If node is not ready to be ordered, replace in queue
			if (0 && uncalculated_nodes.size() <= 9706)
			{
              std::cout << "Can't eject: " << CAD_ob->index_to_node[node_to_calc] << endl;
			}
		}
        /*
        std::cout << "size of uncalculated_nodes: " << uncalculated_nodes.size() << std::endl;
        if(uncalculated_nodes.size() <= 1647) {
          std::queue<int> q = uncalculated_nodes;
          std::cout << "============================" << std::endl;
          while(!q.empty()) {
            std::cout << CAD_ob->index_to_node[q.front()] << ", " << CAD_ob->node_ready[q.front()] << std::endl;
            q.pop();
          }
          std::cout << "============================" << std::endl;
        }
        */
	}

	for (map<string, int>::iterator it = CAD_ob->nodes_name.begin(); it != CAD_ob->nodes_name.end(); ++it)
	{
		if (CAD_ob->GATES.find(it->second) != CAD_ob->GATES.end())
		{		
            CAD_ob->node_ready[it->second] = false;
		}
	}
}

void Timing_Analysis::get_pcs(char *PC_FILE)
{
	ifstream pc_file(PC_FILE);
	cout << *PC_FILE << endl;
	pc;
	set<int> empty_set;
	empty_set.clear();
	string buf;
	
	if (pc_file.fail())
	{
		cout << "Error opening PC file\n";
		exit(0);
	}

	while (!pc_file.eof())
	{
		pc_file >> buf;
		pc = atoi(buf.c_str());
		pc_to_prev_set_C[pc] = empty_set;
		pc_to_prev_set_UC[pc] = empty_set;
		global_match[pc] = 0;
		global_non_match[pc] = 0;
	}
}

void Timing_Analysis::call_delay(int* ts, int size)
{
	for(int i = 0; i < size; i++)
	{
                if(CAD_ob->GATES.find(ts[i]) != CAD_ob->GATES.end())
		{
                                                //Get wire delay (mean and SD) between various nodes
                        get_delay(ts[i], CAD_ob->GATES[ts[i]]);
                        WIRE_DELAY_MEAN[ts[i]] = Timing_Analysis::ret_mean;
                        WIRE_DELAY_SD[ts[i]] = Timing_Analysis::ret_sd;
                }
                else
		{
                        WIRE_DELAY_MEAN[ts[i]] = 0.0;               //If primary Input
                        WIRE_DELAY_SD[ts[i]] = 0.0;
                }
        }

	report_max_delay(ts, size);
} 


void Timing_Analysis::propagate_logic(char* mode, vector<string> exc)//, int seq)
{
	pair<multimap<int, int>::iterator, multimap<int, int>::iterator> edges;
	multimap<int, int>::iterator input1;
	multimap<int, int>::iterator input2;
	queue<int> uncalculated_nodes;
	int node_to_calc;
        
        unchanged_state=0;
      	changed_state=0;
	//srand (seq);

				// Set state of flipflops
	for (vector<D_flipflop>::iterator it = CAD_ob->D_flipflops.begin(); it != CAD_ob->D_flipflops.end(); it++)
	{
		if (CAD_ob->get_logic_state(it->Q_index) == it->Q_next)
		{
			unchanged_state++;
			current_set_UC.insert(it->Q_index);
		}
		else 
		{
			changed_state++;
			current_set_C.insert(it->Q_index);
		}
			
		CAD_ob->set_logic_state(it->Q_index, it->Q_next);	// Set next state to current state
	}

	for (vector<int>::iterator it = node_calculation_order.begin(); it != node_calculation_order.end(); ++it)
	{
		//compute_logic(*it, CAD_ob->GATES[*it]);	// Calculate node

		compute_logic(*it, CAD_ob->GATES[*it], CAD_ob->GATE_FOUT[*it], mode, exc);//hchen for multi-size
	}

				// Calculate next state of flipflops
	for (vector<D_flipflop>::iterator it = CAD_ob->D_flipflops.begin(); it != CAD_ob->D_flipflops.end(); it++)
	{	
		switch(it->type)
		{
		case DFX:
			if (CAD_ob->get_logic_state(it->SA_index))
			{
				if (CAD_ob->get_logic_state(it->DA_index))	it->Q_next = 1;
				else						it->Q_next = 0;
			}
			else
			{
				if (CAD_ob->get_logic_state(it->DB_index))	it->Q_next = 1;
				else						it->Q_next = 0;
			}
			break;
		    case DFC:
	        if (CAD_ob->get_logic_state(it->SA_index)) //CDN = 1, no reset
		{
			if (CAD_ob->get_logic_state(it->DA_index))	it->Q_next = 1;
			else						it->Q_next = 0;
		}
	        else //CDN = 0, reset
		{
			it->Q_next = 0;
		}
	        break;
	    }
	
	}

	max_delay = 0;
	max_mean = 0;  //Added by Aatreyi
    	max_sd = 0;	//Added by Aatreyi
    	sz = 0;		//Added by Aatreyi

	int which = -1;	// added by Kenyon
	int change = 0;
	int maxPath = -1;
	string name = "output_path.txt"; 
	string maxPathName = "critical_path.txt";
	for (set<int>::iterator it = CAD_ob->output_list.begin(); it != CAD_ob->output_list.end(); it++)
	{
//cout<<CAD_ob->GATENAMES[*it]<<"\t"<< CAD_ob->TOTAL_DELAY_MEAN[*it]<<endl;	
		//max_delay = (max_delay > WIRE_DELAY_MEAN[*it])? max_delay : WIRE_DELAY_MEAN[*it];
	//Added by Aatreyi (start)
	//int change = 0;
		for (set<int>::iterator itr = current_set_C.begin(); itr != current_set_C.end(); itr++)
        	{
			//cout<<CAD_ob->GATENAMES[*itr]<<"\t"<<CAD_ob->TOTAL_DELAY_MEAN[*itr]<<endl;	//pramesh
			if(*it==*itr){
				change=1;
				break;
			}
		}
		if(change==1){

			//cout<<CAD_ob->GATENAMES[*it]<<"\t"<<CAD_ob->TOTAL_DELAY_MEAN[*it]<<endl;    //pramesh


			if (CAD_ob->TOTAL_DELAY_MEAN[*it] > max_mean){
				max_mean = CAD_ob->TOTAL_DELAY_MEAN[*it];                 //Calculates the mean and sd of the critical path rather than a deterministic				                	        
				//max_sd = CAD_ob->COMBI_DELAY_SD[*it];
				//which = int(*it);
				maxPath = int(*it);
   			 }
			which = int(*it);
			change=0;
			print_path(name.c_str(), which);
		}
   		//Added by Aatreyi (end)
		//which = (max_mean > COMBI_DELAY_MEAN[*it])? which : (int) *it;	// added by Kenyon
		//max_delay = (max_delay > delay[*it])? max_delay : delay[*it];
	}

	print_path(maxPathName.c_str(), maxPath);

	/*for (set<int>::iterator it = CAD_ob->output_list.begin(); it != CAD_ob->output_list.end(); it++)
        {
        //      max_delay = (max_delay > WIRE_DELAY_MEAN[*it])? max_delay : WIRE_DELAY_MEAN[*it];
        //                      //max_delay = (max_delay > delay[*it])? max_delay : delay[*it];
                if((0.95*max_mean)< CAD_ob->COMBI_DELAY_MEAN[*it] &&  CAD_ob->COMBI_DELAY_MEAN[*it]< max_mean){
                        crit_paths_mean.push_back( CAD_ob->COMBI_DELAY_MEAN[*it]);
                        crit_paths_sd.push_back( CAD_ob->COMBI_DELAY_SD[*it]);
                        sz++;	
                }
        }
*/

	//string name = "output_path.txt";	// added by Kenyon
	//print_path(name.c_str(), which);	// added by Kenyon

	for (vector<D_flipflop>::iterator it = CAD_ob->D_flipflops.begin(); it != CAD_ob->D_flipflops.end(); it++)
	{	
		switch(it->type)
		{
		case DFX:
		if (CAD_ob->get_logic_state(it->SA_index))
		{
			max_delay = (max_delay > WIRE_DELAY_MEAN[it->DA_index])? max_delay : WIRE_DELAY_MEAN[it->DA_index];
			//max_delay = (max_delay > delay[it->DA_index])? max_delay : delay[it->DA_index];
		}
		else
		{
			max_delay = (max_delay > WIRE_DELAY_MEAN[it->DB_index])? max_delay : WIRE_DELAY_MEAN[it->DB_index];
			//max_delay = (max_delay > delay[it->DB_index])? max_delay : delay[it->DB_index];
		}
		break;
	    case DFC:
	      if (CAD_ob->get_logic_state(it->SA_index))
		{
			max_delay = (max_delay > WIRE_DELAY_MEAN[it->DA_index])? max_delay : WIRE_DELAY_MEAN[it->DA_index];
			//max_delay = (max_delay > delay[it->DA_index])? max_delay : delay[it->DA_index];
		}
		else
		{//reset for SA/CDN = 0, SA_index is delay path
			max_delay = (max_delay > WIRE_DELAY_MEAN[it->SA_index])? max_delay : WIRE_DELAY_MEAN[it->SA_index];
			//max_delay = (max_delay > delay[it->DB_index])? max_delay : delay[it->DB_index];
		}
	    }


	}
/*
	count++;

	if(count%3 == 1)
	{
		current_set_C.clear();
		current_set_UC.clear();
		prev_set_C.clear();
		prev_set_UC.clear();
	}
	if(count%3 == 2)
	{
		prev_set_C = current_set_C;
		prev_set_UC = current_set_UC;
		current_set_C.clear();
		current_set_UC.clear();
	}
	else if (count%3 == 0)
	{
		evaluate_union_intersection();
	}
*/
/*	if (delay_occurence.count(max_delay) == 1)
	{
		delay_occurence[max_delay]++;
	}
	else
	{
		delay_occurence[max_delay] = 1;
	}*/
}

//void Timing_Analysis::compute_logic(int v, int type)
//{
//	double d_mean, d_sd, delay1 = 0.0, delay2 = 0.0, delay2_sd, delay1_sd;
//	pair<multimap<int, int>::iterator, multimap<int, int>::iterator> ppp;
//	multimap<int, int>::iterator lb, ub, nlb;
//	ppp = CAD_ob->edges_from.equal_range(v);
//	bool new_state;    
//	bool orig_state = CAD_ob->get_logic_state(v);
//	bool verbose = false;
////	double delay_in;   
////if(type) 
////cout<<"type is` "<<type<<" mean is "<<delays[type]<<"  sd is "<<sds[type]<<endl;
//	if(CAD_ob->edges_from.count(v) == 1) 
//	{
//		lb = ppp.first;	
//		        
//		bool state_1 = CAD_ob->get_logic_state((*lb).second);
//
////		delay_in = delay[(*lb).second];
//
//		if (state_1)
//		{
//		        new_state = 0;
//		} 
//		else
//		{
//		        new_state = 1;		        
//		}
//
//		if (verbose) 
//		        cout << "NODE (" << CAD_ob->index_to_node[v] << "," << v << ") input= (" << CAD_ob->index_to_node[(*lb).second] << "," << (*lb).second << ") State1 = " << state_1 << "  new state " << new_state << endl;
//
//		//cout << "Node " << v << " changes state " << endl;
//		if (orig_state == new_state)
//		{
//			//cout << "Node " << v << " does not change state " << endl;
//			unchanged_state++;
//			current_set_UC.insert(current_set_UC.begin(), v);
//			
//        	        Timing_Analysis::WIRE_DELAY_MEAN[v]=0.0;
//        	        Timing_Analysis::WIRE_DELAY_SD[v]=0.0;
//		}
//		else
//		{
//			changed_state++;
//			current_set_C.insert(current_set_C.begin(), v);
//        	        Timing_Analysis::statistical_delay_sum(WIRE_DELAY_MEAN[(*lb).second], WIRE_DELAY_SD[(*lb).second], delays[type], sds[type]);
//                        WIRE_DELAY_MEAN[v]=Timing_Analysis::ret_mean;
//                        WIRE_DELAY_SD[v]=Timing_Analysis::ret_sd;
////                        WIRE_DELAY_MEAN[v]=WIRE_DELAY_MEAN[(*lb).second]+delays[type];
////                        delay[v]=delay[(*lb).second]+delays[type];
////			//cout << "Node " << v << " changes state " << endl;
////			//cout<<WIRE_DELAY_MEAN[v]<<"  "<<WIRE_DELAY_SD[v]<<endl;
////			cout<<delay[v]<<endl;
//		}
//		        
//		} 
//	else
//	{
//	        assert(CAD_ob->edges_from.count(v) == 2);
//	        lb = ppp.first;
//	        ub = lb;
//	        lb++;
//	        nlb = lb;
//        
//	        bool state_1 = CAD_ob->get_logic_state((*ub).second) ;
//	        bool state_2 = CAD_ob->get_logic_state((*nlb).second);
//
//		//delay_in = (delay[lb->second] > delay[ub->second])? delay[lb->second] : delay[ub->second];
//
//	        if (type >= 17 && type <= 28) 
//		{ // NOR
//			if (state_1 || state_2)
//				new_state = 0;
//			else
//	                	new_state = 1;
//			//new_state = ~(state_1||state_2 );
//	        }
//		else if (type >= 11 && type <= 28)
//		{ // NAND
//			if (state_1 && state_2)
//	                	new_state = 0;
//			else
//				new_state = 1;
//			//new_state = ~(state_1 && state_2);    
//        	}
//        
//		if (verbose)
//			cout << "NODE (" << CAD_ob->index_to_node[v] << "," << v << ")" << " input1 = " << state_1 << " v1 = (" << CAD_ob->index_to_node[(*ub).second] << "," <<  (*ub).second << ") input2  " << state_2 << " v2 = (" <<  CAD_ob->index_to_node[(*nlb).second] << "," << (*nlb).second << ") new state = " << new_state << " type = " << type << endl;              
//		if (orig_state == new_state)
//		{
//			//cout << "Node " << v << " does not change state " << endl;
//			unchanged_state++;
//			current_set_UC.insert(current_set_UC.begin(), v);
//			
//        	        Timing_Analysis::WIRE_DELAY_MEAN[v]=0.0;
//        	        Timing_Analysis::WIRE_DELAY_SD[v]=0.0;
//		}
//		else
//		{
//			changed_state++;
//			current_set_C.insert(current_set_C.begin(), v);
//
//                	delay1 = Timing_Analysis::WIRE_DELAY_MEAN[(*ub).second];
//                	delay1_sd = Timing_Analysis::WIRE_DELAY_SD[(*ub).second];
//                	delay2 = Timing_Analysis::WIRE_DELAY_MEAN[(*nlb).second];
//                	delay2_sd = Timing_Analysis::WIRE_DELAY_SD[(*nlb).second];
//           		
//			 Timing_Analysis::stat_max(Timing_Analysis::WIRE_DELAY_MEAN[(*nlb).second], Timing_Analysis::WIRE_DELAY_SD[(*nlb).second], Timing_Analysis::WIRE_DELAY_MEAN[(*ub).second], Timing_Analysis::WIRE_DELAY_SD[(*ub).second]);
//            		Timing_Analysis::statistical_delay_sum(Timing_Analysis::in_delay_mean, Timing_Analysis::in_delay_sd, delays[type], sds[type]);
//                        WIRE_DELAY_MEAN[v]=Timing_Analysis::ret_mean;
//                        WIRE_DELAY_SD[v]=Timing_Analysis::ret_sd;
//
////                       double maxxx= WIRE_DELAY_MEAN[(*nlb).second]>WIRE_DELAY_MEAN[(*ub).second] ? WIRE_DELAY_MEAN[(*nlb).second] : WIRE_DELAY_MEAN[(*ub).second];
////                        WIRE_DELAY_MEAN[v]=maxxx+delays[type];
////                              maxxx= delay[(*nlb).second]>delay[(*ub).second] ? delay[(*nlb).second] : delay[(*ub).second];
////                        delay[v]=maxxx+delays[type];
////			cout<<WIRE_DELAY_MEAN[v]<<"  "<<WIRE_DELAY_SD[v]<<endl;
////			cout<<delay[v]<<endl;
//			//cout << "Node " << v << " changes state " << endl;
//		}
//	}
//    
//	CAD_ob->set_logic_state(v, new_state);
//
//
////	if (orig_state == new_state)
////	{
////		//cout << "Node " << v << " does not change state " << endl;
////		unchanged_state++;
////		current_set_UC.insert(current_set_UC.begin(), v);
////		
////		delay[v] = 0;
//////		delay[v] = delay_in;
////	}
////	else
////	{
////		changed_state++;
////		current_set_C.insert(current_set_C.begin(), v);
////                delay[v] = delay_in + delays[type];
////		//cout << "Node " << v << " changes state " << endl;
////	}
////
////
//
//
//}
/*************************************hchen for multi-size**************************/
double Timing_Analysis::calc_delay(int type, double fanout)
{
	double delay;
	if(fanout<delay_args[type][0])
		delay = delay_args[type][3]*fanout + delay_args[type][5];
	else
		delay = delay_args[type][4]*fanout + delay_args[type][6];
	return delay;

}
double Timing_Analysis::calc_sd(int type, double delay)
{
	double sd;
	sd = delay/delay_args[type][1]*delay_args[type][2];
	return sd;
}
/**********************************************************************************/

void Timing_Analysis::compute_logic(int v, int type, double fanout, char *mode, vector<string> exc)//hchen for multi-size
{
	double d_mean, d_sd, delay1 = 0.0, delay2 = 0.0, delay2_sd, delay1_sd, gate_delay, gate_sd,z; //last two added by Aatreyi
	pair<multimap<int, int>::iterator, multimap<int, int>::iterator> ppp;
	multimap<int, int>::iterator lb, ub, nlb;
	ppp = CAD_ob->edges_from.equal_range(v);
	bool new_state;    
	int mark=0;
	bool orig_state = CAD_ob->get_logic_state(v);
	bool verbose = false;
	//srand (time(0));
	
	//Added by Aatreyi (start)
	for (int i=0; i<exc.size(); i++){
		if (CAD_ob->GATENAMES[v].compare(exc[i])==0){
			mark=1;
			break;
		}
	}
	if(mark==1){

		if (type>=0 && type<=5){		//inverter
		CAD_ob->GATE_DELAY_MEAN[v]=delaye[0];
		CAD_ob->GATE_DELAY_SD[v]=sde[0];
		}
		else if (type>=12 && type<=17){		//NOR
			CAD_ob->GATE_DELAY_MEAN[v]=delaye[2];			
			CAD_ob->GATE_DELAY_SD[v]=sde[2];
		}
		else {					//NAND
			CAD_ob->GATE_DELAY_MEAN[v]=delaye[1];
			CAD_ob->GATE_DELAY_SD[v]=sde[1];
		}
	}else{
		if (type>=0 && type<=5){
			CAD_ob->GATE_DELAY_MEAN[v]=delays[0];
			CAD_ob->GATE_DELAY_SD[v]=sds[0];
		}
		else if (type>=12 && type<=17){
			CAD_ob->GATE_DELAY_MEAN[v]=delays[2];
			CAD_ob->GATE_DELAY_SD[v]=sds[2];
		}
		else {
			CAD_ob->GATE_DELAY_MEAN[v]=delays[1];
			CAD_ob->GATE_DELAY_SD[v]=sds[1];
		}

	}

	//Function to calculate a deterministic delay value from the distribution in case of process variation
	if ((mark==1) || (!strcmp(mode,"2")==1)){
		z=-30+(float(rand()%61));
		z=z/10;
		//pramesh added
                //srand(time(0));
                //int randomizer=rand()%
                float LO=-1;
                float HI=3;
                float r3 = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
                //cout<<"randomizer is "<<r3<<endl;
		CAD_ob->GATE_DELAY_MEAN[v]=CAD_ob->GATE_DELAY_MEAN[v] + (r3* CAD_ob->GATE_DELAY_SD[v]);    //z_score=(value - mean)/SD
		//cout<<endl<<"z:"<<z<<endl;
		//cout<<CAD_ob->GATENAMES[v]<<"\t"<<CAD_ob->GATE_DELAY_MEAN[v]<<endl;
	}	
	//Added by Aatreyi (end)

//	double delay_in;   
//if(type) 
//<<"type is` "<<type<<" mean is "<<delays[type]<<"  sd is "<<sds[type]<<endl;
	if(CAD_ob->edges_from.count(v) == 1) 
	{
		lb = ppp.first;	
		        
		bool state_1 = CAD_ob->get_logic_state((*lb).second);

//		delay_in = delay[(*lb).second];

		if (state_1)
		{
		        new_state = 0;
		} 
		else
		{
		        new_state = 1;		        
		}

		if (verbose) 
		        cout << "NODE (" << CAD_ob->index_to_node[v] << "," << v << ") input= (" << CAD_ob->index_to_node[(*lb).second] << "," << (*lb).second << ") State1 = " << state_1 << "  new state " << new_state << endl;

		//cout << "Node " << v << " changes state " << endl;
		if (orig_state == new_state)
		{
			//cout << "Node " << v << " does not change state " << endl;
			unchanged_state++;
			current_set_UC.insert(current_set_UC.begin(), v);
		        Timing_Analysis::WIRE_DELAY_MEAN[v] = 0;
                        Timing_Analysis::WIRE_DELAY_SD[v] = 0;
        	        CAD_ob->TOTAL_DELAY_MEAN[v]=0.0;
			CAD_ob->COMBI_DELAY_MEAN[v]=0.0;
        	        CAD_ob->COMBI_DELAY_SD[v]=0.0;
		}
		else
		{
//cout<<CAD_ob->GATENAMES[v]<<endl;
			changed_state++;
			current_set_C.insert(current_set_C.begin(), v);
			                 Timing_Analysis::WIRE_DELAY_MEAN[v] = calc_delay(type,fanout);
                                         Timing_Analysis::WIRE_DELAY_SD[v] = calc_sd(type, Timing_Analysis::WIRE_DELAY_MEAN[v]);
                         
                         

//			cout<<"gate_delay is "<<gate_delay<<", gate_sd is "<<gate_sd<<endl;	
        	        //Timing_Analysis::statistical_delay_sum(WIRE_DELAY_MEAN[(*lb).second], WIRE_DELAY_SD[(*lb).second], gate_delay, gate_sd);
                        //WIRE_DELAY_MEAN[v]=Timing_Analysis::ret_mean;
                        //WIRE_DELAY_SD[v]=Timing_Analysis::ret_sd;
//                        WIRE_DELAY_MEAN[v]=WIRE_DELAY_MEAN[(*lb).second]+delays[type];
//                        delay[v]=delay[(*lb).second]+delays[type];
//			//cout << "Node " << v << " changes state " << endl;
//			//cout<<WIRE_DELAY_MEAN[v]<<"  "<<WIRE_DELAY_SD[v]<<endl;
//			cout<<delay[v]<<endl;

			//Added by Aatreyi(start)
            		Timing_Analysis::statistical_delay_sum(Timing_Analysis::WIRE_DELAY_MEAN[(*lb).second], Timing_Analysis::WIRE_DELAY_SD[(*lb).second], CAD_ob->GATE_DELAY_MEAN[v], CAD_ob->GATE_DELAY_SD[v]);
			 CAD_ob->COMBI_DELAY_MEAN[v]=ret_mean;
			 CAD_ob->COMBI_DELAY_SD[v]=ret_sd;
			//Added by Aatreyi(end)
			
		}
		        
	} 
	else
	{
	        assert(CAD_ob->edges_from.count(v) == 2);
	        lb = ppp.first;
	        ub = lb;
	        lb++;
	        nlb = lb;
        
	        bool state_1 = CAD_ob->get_logic_state((*ub).second) ;
	        bool state_2 = CAD_ob->get_logic_state((*nlb).second);
		

		//delay_in = (delay[lb->second] > delay[ub->second])? delay[lb->second] : delay[ub->second];

	        if (type >= 12 && type <= 17) 
		{ // NOR
			if (state_1 || state_2)
				new_state = 0;
			else
	                	new_state = 1;
			//new_state = ~(state_1||state_2 );
	        }
		else if (type >= 6 && type <= 11)
		{ // NAND
			if (state_1 && state_2)
	                	new_state = 0;
			else
				new_state = 1;
			//new_state = ~(state_1 && state_2);    
        	}
        
		if (verbose)
			cout << "NODE (" << CAD_ob->index_to_node[v] << "," << v << ")" << " input1 = " << state_1 << " v1 = (" << CAD_ob->index_to_node[(*ub).second] << "," <<  (*ub).second << ") input2  " << state_2 << " v2 = (" <<  CAD_ob->index_to_node[(*nlb).second] << "," << (*nlb).second << ") new state = " << new_state << " type = " << type << endl;              
		if (orig_state == new_state)
		{
			//cout << "Node " << v << " does not change state " << endl;
			unchanged_state++;
			current_set_UC.insert(current_set_UC.begin(), v);
			Timing_Analysis::WIRE_DELAY_MEAN[v] = 0;
                        Timing_Analysis::WIRE_DELAY_SD[v] = 0;
        	        CAD_ob->TOTAL_DELAY_MEAN[v]=0.0;
			CAD_ob->COMBI_DELAY_MEAN[v]=0.0;
        	        CAD_ob->COMBI_DELAY_SD[v]=0.0;
		}
		else
		{
//cout<<CAD_ob->GATENAMES[v]<<endl;
			changed_state++;
			current_set_C.insert(current_set_C.begin(), v);

			if(!strcmp(mode,"1")){
                                
                                         Timing_Analysis::WIRE_DELAY_MEAN[v] = calc_delay(type,fanout);
                                         Timing_Analysis::WIRE_DELAY_SD[v] = calc_sd(type,Timing_Analysis::WIRE_DELAY_MEAN[v]);
                                         
                                
                        }else{
                                        delay1 = Timing_Analysis::WIRE_DELAY_MEAN[(*ub).second];
                			delay1_sd = Timing_Analysis::WIRE_DELAY_SD[(*ub).second];
                			delay2 = Timing_Analysis::WIRE_DELAY_MEAN[(*nlb).second];
                			delay2_sd = Timing_Analysis::WIRE_DELAY_SD[(*nlb).second];
           		
					//Timing_Analysis::stat_max(Timing_Analysis::WIRE_DELAY_MEAN[(*nlb).second], Timing_Analysis::WIRE_DELAY_SD[(*nlb).second], Timing_Analysis::WIRE_DELAY_MEAN[(*ub).second], Timing_Analysis::WIRE_DELAY_SD[(*ub).second]);

					Timing_Analysis::WIRE_DELAY_MEAN[v] = calc_delay(type,fanout);
					Timing_Analysis::WIRE_DELAY_SD[v] = calc_sd(type,Timing_Analysis::WIRE_DELAY_MEAN[v]);
                        }



                	
//			cout<<"gate_delay is "<<gate_delay<<", gate_sd is "<<gate_sd<<endl;	
        	        //Timing_Analysis::statistical_delay_sum(WIRE_DELAY_MEAN[(*lb).second], WIRE_DELAY_SD[(*lb).second], gate_delay, gate_sd);

                        //WIRE_DELAY_MEAN[v]=Timing_Analysis::ret_mean;
                        //WIRE_DELAY_SD[v]=Timing_Analysis::ret_sd;

//                       double maxxx= WIRE_DELAY_MEAN[(*nlb).second]>WIRE_DELAY_MEAN[(*ub).second] ? WIRE_DELAY_MEAN[(*nlb).second] : WIRE_DELAY_MEAN[(*ub).second];
//                        WIRE_DELAY_MEAN[v]=maxxx+delays[type];
//                              maxxx= delay[(*nlb).second]>delay[(*ub).second] ? delay[(*nlb).second] : delay[(*ub).second];
//                        delay[v]=maxxx+delays[type];
//			cout<<WIRE_DELAY_MEAN[v]<<"  "<<WIRE_DELAY_SD[v]<<endl;
//			cout<<delay[v]<<endl;
			//cout << "Node " << v << " changes state " << endl;
		//Added by Aatreyi(start)
		//Timing_Analysis::stat_max(Timing_Analysis::WIRE_DELAY_MEAN[(*nlb).second], Timing_Analysis::WIRE_DELAY_SD[(*nlb).second], Timing_Analysis::WIRE_DELAY_MEAN[(*ub).second], Timing_Analysis::WIRE_DELAY_SD[(*ub).second]);
		if(Timing_Analysis::WIRE_DELAY_MEAN[(*nlb).second]>Timing_Analysis::WIRE_DELAY_MEAN[(*ub).second]){
			Timing_Analysis::in_delay_mean=Timing_Analysis::WIRE_DELAY_MEAN[(*nlb).second];
			Timing_Analysis::in_delay_sd=Timing_Analysis::WIRE_DELAY_SD[(*nlb).second];
			lb=nlb;
		}else{
			Timing_Analysis::in_delay_mean=Timing_Analysis::WIRE_DELAY_MEAN[(*ub).second];
                        Timing_Analysis::in_delay_sd=Timing_Analysis::WIRE_DELAY_SD[(*ub).second];
                        lb=ub;
		}

		//lb=in_delay_mean>Timing_Analysis::WIRE_DELAY_MEAN[(*ub).second]?nlb:ub;
           	Timing_Analysis::statistical_delay_sum(Timing_Analysis::in_delay_mean, Timing_Analysis::in_delay_sd, CAD_ob->GATE_DELAY_MEAN[v], CAD_ob->GATE_DELAY_SD[v]);
		 //Timing_Analysis::statistical_delay_sum(Timing_Analysis::WIRE_DELAY_MEAN[(*lb).second], Timing_Analysis::WIRE_DELAY_SD[(*lb).second], CAD_ob->GATE_DELAY_MEAN[v], CAD_ob->GATE_DELAY_SD[v]);
		CAD_ob->COMBI_DELAY_MEAN[v]=ret_mean;
//cout<<CAD_ob->GATENAMES[v]<<"\t"<<Timing_Analysis::WIRE_DELAY_MEAN[(*lb).second]<<"\t"<< CAD_ob->GATE_DELAY_MEAN[v]<<"\t"<< CAD_ob->COMBI_DELAY_MEAN[v]<<endl;
	 	CAD_ob->COMBI_DELAY_SD[v]=ret_sd;
		//Added by Aatreyi(end)
		
		}
	}
   	int flag=0;
	if (changed_state!=0)
	WIRE_DELAY_PATH[v]=(*lb).second;	// added by Kenyon
	for (set<int>::iterator it = CAD_ob->input_list.begin(); it != CAD_ob->input_list.end(); it++){
		if(CAD_ob->GATENAMES[*it].compare(CAD_ob->GATENAMES[v])==0){
			flag=1;
			break;
		}
	}
	if(flag==1){	
		 CAD_ob->TOTAL_DELAY_MEAN[v]=CAD_ob->GATE_DELAY_MEAN[v];
	}else{
		 CAD_ob->TOTAL_DELAY_MEAN[v]= CAD_ob->COMBI_DELAY_MEAN[v]+ CAD_ob->TOTAL_DELAY_MEAN[(*lb).second]; //Delay of the path so far as the sum of earlier gate delay and wire delay
	}	
	CAD_ob->set_logic_state(v, new_state);


//	if (orig_state == new_state)
//	{
//		//cout << "Node " << v << " does not change state " << endl;
//		unchanged_state++;
//		current_set_UC.insert(current_set_UC.begin(), v);
//		
//		delay[v] = 0;
////		delay[v] = delay_in;
//	}
//	else
//	{
//		changed_state++;
//		current_set_C.insert(current_set_C.begin(), v);
//                delay[v] = delay_in + delays[type];
//		//cout << "Node " << v << " changes state " << endl;
//	}
//
//


}

void Timing_Analysis::compare_sets(set<int> set1, set<int> set2, int &m, int &um)
{
	set<int>::iterator one_it = set1.begin();
	set<int>::iterator two_it = set2.begin();

	while ((one_it != set1.end()) && (two_it != set2.end()))
	{
		if (*one_it == *two_it)
		{
			m++;
			one_it++;
			two_it++;
		}
		else if (*one_it > *two_it)
		{
			two_it++;
			um++;
		}
		else if (*two_it > *one_it)
		{
			one_it++;
			um++;
		}
	}

	while (one_it != set1.end())
	{
		one_it++;
		um++;
	}

	while (two_it != set2.end())
	{
		two_it++;
		um++;
	}
}

void Timing_Analysis::evaluate_union_intersection() 
{
	int matched = 0;
	int unmatched = 0;

	compare_sets(current_set_UC, prev_set_UC, matched, unmatched);
	compare_sets(current_set_C, prev_set_C, matched, unmatched);

	global_match[pc] += matched;
	global_non_match[pc] += unmatched;
}


void Timing_Analysis::get_delay(int v, int type){		//To get the Wire delays between nodes that are connected to a particular output node
        
	double d_mean, d_sd, delay1 = 0.0, delay2 = 0.0, delay2_sd, delay1_sd;
        pair<multimap<int, int>::iterator, multimap<int, int>::iterator> ppp;
        multimap<int, int>::iterator lb, ub, nlb;
        ppp = CAD_ob->edges_from.equal_range(v);
        
        if(CAD_ob->edges_from.count(v) == 1){
            type = 0;
            d_mean = 0.0;
            d_sd = 0.0;
            
            lb = ppp.first;
            if(Timing_Analysis::WIRE_DELAY_MEAN.find((*lb).second) != Timing_Analysis::WIRE_DELAY_MEAN.end()){
                d_mean = Timing_Analysis::WIRE_DELAY_MEAN[(*lb).second];
                d_sd = Timing_Analysis::WIRE_DELAY_SD[(*lb).second];
            } else {	
                cout<<"PROBLEM WITH TOPOLOGICAL SORT1"<<'\n';
                exit(0);
            }
            Timing_Analysis::statistical_delay_sum(d_mean, d_sd, CAD_ob->GATE_DELAY_MEAN[type], CAD_ob->GATE_DELAY_SD[type]);
        } else {
            lb = ppp.first;
            nlb = ppp.first;
            if(Timing_Analysis::WIRE_DELAY_MEAN.find((*lb).second) != Timing_Analysis::WIRE_DELAY_MEAN.end()){
                delay1 = Timing_Analysis::WIRE_DELAY_MEAN[(*lb).second];
                delay1_sd = Timing_Analysis::WIRE_DELAY_SD[(*lb).second];
            } else {	
                cout<<"PROBLEM WITH TOPOLOGICAL SORT2"<<'\n';
                exit(0);
            }
            ++lb;
            ub = lb;
	            
            /*if(Timing_Analysis::WIRE_DELAY_MEAN.find((*ub).second) != Timing_Analysis::WIRE_DELAY_MEAN.end()){
                delay2 = Timing_Analysis::WIRE_DELAY_MEAN[(*ub).second];
                delay2_sd = Timing_Analysis::WIRE_DELAY_SD[(*ub).second];
            } else {
                cout<<"PROBLEM WITH TOPOLOGICAL SORT3"<<'\n';
                exit(0);
            }*/
            Timing_Analysis::stat_max(Timing_Analysis::WIRE_DELAY_MEAN[(*nlb).second], Timing_Analysis::WIRE_DELAY_SD[(*nlb).second], Timing_Analysis::WIRE_DELAY_MEAN[(*ub).second], Timing_Analysis::WIRE_DELAY_SD[(*ub).second]);
            Timing_Analysis::statistical_delay_sum(Timing_Analysis::in_delay_mean, Timing_Analysis::in_delay_sd, CAD_ob->GATE_DELAY_MEAN[type], CAD_ob->GATE_DELAY_SD[type]);
        }
        
}


void Timing_Analysis::statistical_delay_sum(double d_mean, double d_sd, double a_mean, double a_sd){	//Adds the wire delays and the Gate delays to get
        Timing_Analysis::ret_mean = d_mean + a_mean;								//final delay values (both mean and SD)
        Timing_Analysis::ret_sd = sqrt(pow(d_sd, 2) + pow(a_sd, 2));
}


void Timing_Analysis::report_max_delay(int* ts, int N){						//Reports the maximum delay and the delay node
       
	 
	double max_mean = 0.0;
        double max_sd = 0.0;
        int delay_node;
        string x;
        cout.precision(15);
        cout.setf(ios::fixed,ios::floatfield);
        
	for(int i = 1; i <= N; i++){
                if(Timing_Analysis::WIRE_DELAY_MEAN[ts[i]] > max_mean){
                        max_mean = Timing_Analysis::WIRE_DELAY_MEAN[ts[i]];
                        max_sd = Timing_Analysis::WIRE_DELAY_SD[ts[i]];
                        delay_node = ts[i];
                }
        }
        
	typedef map<string, int>::const_iterator CI;
        for(CI p = CAD_ob->nodes_name.begin(); p != CAD_ob->nodes_name.end(); ++p){
                if((p->second) == delay_node)
                        x = p->first;
        }

        cout<<"Maximum Delay in the circuit is: ("<<max_mean<<","<<max_sd<<") "<<"For Node "<<x<<'\n';
}


void Timing_Analysis::stat_max(double mu_1, double sd_1, double mu_2, double sd_2)	//Gives the maximum wire delay value
{

        if(mu_1 == 0.0){

                Timing_Analysis::in_delay_mean = mu_2;
                Timing_Analysis::in_delay_sd = sd_2;
		}
        else if(mu_2 == 0.0){

                Timing_Analysis::in_delay_mean = mu_1;
                Timing_Analysis::in_delay_sd = sd_1;
		}
                /*else if(sd_1 == 0.0 && sd_2 == 0.0){
                        cout<<"BOTH SDs ARE ZERO"<<'\n';
                        exit(0);
			}*/
		 else if(sd_1 == 0.0 && sd_2 == 0.0){

                        Timing_Analysis::in_delay_mean = (mu_1 + mu_2);
                        Timing_Analysis::in_delay_sd = 0.0;
                        }

        else{

        double a = sqrt(pow(sd_1, 2) + pow(sd_2, 2));
        double alpha = (mu_1 - mu_2);

        double mu_12 = mu_1 * phi_func(alpha) + mu_2 * phi_func(-alpha) + a * psi_func(alpha);
        double part_1 = (pow(mu_1, 2) + pow(sd_1, 2)) * phi_func(alpha) + (pow(mu_2, 2) + pow(sd_2, 2)) * phi_func(-alpha) +
                (mu_1 + mu_2) * a * psi_func(alpha);
        double part_2 = pow(mu_12, 2);
        double ret = part_1 - part_2;

                Timing_Analysis::in_delay_mean = mu_12;
                Timing_Analysis::in_delay_sd = sqrt(ret);
        }

}

double phi_func(double x) {
        
	double arg = x;
        double mod_arg = arg/sqrt(2);
        double value = 0.5 * (1 + erf(mod_arg));
        return value;
}

double psi_func(double x) {
        double ret = 0.3989422804 * exp(-0.5 * x * x);
        return ret;
}

void Timing_Analysis::print_changed_nodes(char *output_file)
{
//	ofstream outFile (output_file, ios::binary | ios::trunc | ios::out);

//	cout << outFile.is_open() << endl;;
//	outFile << " PC\t\tglobal_match\tglobal_non_match\tcommonality\n";

	double commonality;

	for (map<int, unsigned long long>::iterator it = global_match.begin(); it != global_match.end(); it++)
	{
		commonality = (double)(it->second)/((double)(global_non_match[it->first] + (double)(it->second)));
//		outFile << it->first << "\t\t" << it->second << "\t\t" << global_non_match[it->first] << "\t\t" << commonality << endl;
	}

}

void Timing_Analysis::print_delay(char *output_file)
{
//	ofstream outFile (output_file, ios::binary | ios::trunc | ios::out);

	for (map<double, int>::iterator it = delay_occurence.begin(); it != delay_occurence.end(); it++)
	{
//		outFile << it->first << ", " << it->second << endl;
	}
}

/// This function added by Kenyon

void Timing_Analysis::print_path(const char *output_file, int v)
{
	int i=1;

	int orgIndex = v;
//	ofstream outFile (output_file, ios::binary | ios::app);
	if(last_clock != clock_count) {
		last_clock = clock_count;
//		outFile << "===============" << endl;
		//cout<<"===============" << endl;
     }

	//outFile << "Number of gates in GATES: " << CAD_ob->GATES.size() << endl;
	//outFile << "Number of gates in GATENAMES: " << CAD_ob->GATENAMES.size() << endl;

/*	for (i = 0; i < WIRE_DELAY_PATH[v].size(); i++)
		outFile << CAD_ob->GATENAMES[WIRE_DELAY_PATH[v][i]] << ' ';
	outFile << std::endl;
}*/ 
	do{
	for (set<int>::iterator it = CAD_ob->input_list.begin(); it != CAD_ob->input_list.end(); it++)
	{
	     if (v==*it){
				i=0;
				break;
	     }
      	}
//	outFile << CAD_ob->GATENAMES[v]<<' ';
	v=WIRE_DELAY_PATH[v];
	}while(i==1);
//	outFile<<endl;
//	outFile<<endl;
//	outFile<<"Delay: "<< CAD_ob->TOTAL_DELAY_MEAN[orgIndex];
//	outFile<<endl;
//	outFile<<endl;

}
/// END part added by Kenyon

void Timing_Analysis::cycle_delay(void)
{
	static int counter=0;
	//outFile <<  max_mean<</* ",          "<< "UC: "<<unchanged_state<<",        "
          //      <<"C: "<<changed_state<<",       "<<"change rate is: "
            //    << double(changed_state)/double(unchanged_state+changed_state)<< */endl;
        /*if(sz!=0){
                for (int i=0; i<sz; i++){
                        outFile << crit_paths_mean[i] << " , " << crit_paths_sd[i] << endl;
                }
        }
*/
        //outFile << "\n";
if(counter++>0) cout<<max_mean<<endl;
        crit_paths_mean.clear();
        crit_paths_sd.clear();

}
