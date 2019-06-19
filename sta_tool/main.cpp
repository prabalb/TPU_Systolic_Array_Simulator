#include "CAD.h"
#include "stat.h"
#include <iostream>
#include <gmpxx.h>
#include <ctime>
#include <string>
#include <stdio.h>
#include <cstring>

void initialize_command_list(map<string,int>&);
char* get_output_filename(char*, char *, int, char *, char *);
char* get_commonality_filename(char *, char *, int);

int clock_count;


int main(int argc, char *argv[])		// <Verilog File> <command file> <delay file>
{

	srand(time(0));
	clock_count = 0;
	int num;	//Added by Aatreyi
	int program_time = time(NULL);
//	string ans;	//Added by Aatreyi
	string e_gate;       //Added by Aatreyi
//	std::string e_gate_together;//pramesh
	vector<string> exc; //Added by Aatreyi
	if (argc < 3)
	{
		printf("Not enough input arguments.\n");
		return 0;
	}
	mpz_class input_value;

string pad1="set activ ";
string pad2="set weight ";
string pad3="set prev_mac_o ";
string pad4="pl";
string i1(argv[6]); // prev active 
string w1(argv[7]); // prev weight
string a1(argv[8]); // prev prev_mac_o
string i2(argv[9]); // curr activ
string w2(argv[10]); // curr weight
string a2(argv[11]); // curr prev_mac_o

  /*
  ofstream mfile (argv[2]);
  if (mfile.is_open())
  {
    mfile << pad1<<i1<<endl;
    mfile << pad2<<w1<<endl;
    mfile << pad3<<a1<<endl;
    mfile << pad4<<endl;
    mfile << pad1<<i2<<endl;
    mfile << pad2<<w2<<endl;
    mfile << pad3<<a2<<endl;
    mfile << pad4<<endl;
    mfile << "quit"<<endl;
  
  mfile.close();
  }
  */


	CADutil *CAD = new CADutil(argv[1], argv[2]);
	char* mode = argv[3];							//Added by Aatreyi	(start)				
	char* ans = argv[5];							//Added by Aatreyi	(start)				
        if(!strcmp(mode,"1")){
  //              cout<<"Any exception gate(y/n)?"<<endl;
               // char* ans=argv[5];
//		cin>>ans;
		if(!strcmp(ans,"y")){
		//cout<<"Number of Gates?"<<endl;
                //cin>>num;
		//cout<<"gates"<<endl;
//		cin>>e_gate_together;
	
//                   char* inputv=argv[5]
                   int inputv=atoi(argv[4]);            
                   //cout<<"input is "<<inputv<<endl;

    string line_temp;
    string line;
  ifstream myfile (argv[12]);
// cout<<"file is "<<argv[13]<<endl; 
 if (myfile.is_open())
  {
    int line_counter;
    while ( getline (myfile,line_temp) )
    {
      if (line_counter++==inputv){
      // cout << "pramesh file input line"<<line_temp << '\n';
        line=line_temp;
          }
    }
    myfile.close();
  }
//   cout<<line.length()<<" length of line "<<line<<endl;
		   int ii=0;
		   int jj=0;

   		   while(ii<line.length())
                               {
			       if (line[ii]==' '){
			           //cout<<"j i "<<jj<<" "<<ii-jj;
			           exc.push_back(line.substr(jj,ii-jj));
			           //cout<<"the line is "<<line.substr(jj,ii-jj)<<endl;
			           jj=ii+1;
			       }
			       ii++;
			   }

	           exc.push_back(line.substr(jj,ii-jj));//for last element
			//cout<<"\nprinting from vector to verify "<<exc.size()<<endl;
                // for(int i=0;i<exc.size();i++){
                  //              cout<<exc[i]<<endl;
                    //                    }

			//cout<<"\nprinting from vector to verify"<<endl;
////pramesh added to this point

//cout<<exc;





//		for (int i=0; i<num; i++){
//			cout<<"Gate "<<i+1<<":"<<endl;
//			cin>>e_gate;
//			exc.push_back(e_gate);
//		}
	        }		
	}								//Added by Aatreyi (end)

	CAD->parse_syn_v(CAD->G_ref);			// Parse the Verilog File
	//CAD->parse_delay_file();			// Parse the delay file	
	CAD->initialize_logic_state();			// Set all internal states to zero
	Timing_Analysis time(CAD,argv[2],mode,ans);//, argv[2])	
//cout << "check"<<endl;
	char *output_filename, *commonality_file;
	//ofstream outFile, nodeFile;
//	ofstream pathFile;		// added by Kenyon
	ifstream *inFile;
    char* voltage;

	if (argc > 2)		// Set up input and output files
	{
		output_filename = get_output_filename(argv[1], const_cast<char*>("dummy.txt"), program_time, argv[3], argv[4]);
//cout<<"no problem"<<endl;
//		commonality_file = get_commonality_filename(argv[1], argv[2], program_time);
		//outFile.open(output_filename, ios::out | ios::trunc | ios::binary);
		//nodeFile.open("node_output_file.txt", ios::out | ios::trunc | ios::binary);
		//pathFile.open("output_path.txt", ios::out | ios::trunc | ios::binary);
		//pathFile.close();
		//inFile = new ifstream(argv[2]);
		inFile = NULL;
	
	}
	else
	{
		output_filename = NULL;
		commonality_file = NULL;
		inFile = NULL;
	}
	string line;				// Set up commands
	vector<string> commands;
	//char buf[2048];
	map<string,int> command_list;
	initialize_command_list(command_list);
	bool quit = false;

    std::vector<std::string> instructions;
    instructions.resize(9);
    instructions[0] = pad1 + i1;
    instructions[1] = pad2 + w1;
    instructions[2] = pad3 + a1;
    instructions[3] = pad4;
    instructions[4] = pad1 + i2;
    instructions[5] = pad2 + w2;
    instructions[6] = pad3 + a2;
    instructions[7] = pad4;
    instructions[8] = std::string("quit");


    int counter = 0;
	while (!quit)
	{					// Read a command from file or from terminal
		//if (inFile == NULL || inFile->eof())
		//{
		//	//cout << "  >> ";
		//	cin.getline(buf, 2048);
		//}
		//else
		//{
		//	inFile->getline(buf, 2048);
		//}
			
		//line = buf;
		line = instructions[counter++];
		boost::split(commands, line, boost::is_any_of("\t "));

		if (command_list.find(commands[0]) == command_list.end())		// First word is invalid.
		{
		//	cout << endl << line << endl << "! Invalid command, please try again." << endl << endl;
			continue;
		}

		switch( command_list[commands[0]] )
		{
		case 0:							// set
			try
			{
				input_value = commands.at(2).c_str();
				CAD->set_input(commands.at(1), input_value);
			}
			catch (out_of_range)
			{				// If arguments are not found
			//	cout << endl << line << endl << " Invalid command, please try again." << endl << endl;
			}
			break;
		case 1:							// propagate_logic
				time.propagate_logic(mode,exc);//,i);//pc[0]);
				time.cycle_delay();
				clock_count++;
                     
			if (clock_count % 10000 == 0)
			{
			//	cout << " Clock Cycle: " << clock_count << endl;			
//				time.print_changed_nodes(commonality_file);
//				time.print_delay(output_filename);
			}
			//cout<<endl<<endl;
			break;
		case 2:							// print_outputs
		//	cout << " Clock Cycle: " << clock_count;
			CAD->print_outputs();
//			if (argc > 2)
//			{
//				outFile << " Clock Cycle: " << clock_count << endl;
//				CAD->print_outputs(outFile);
//			}
			break;
		case 3:							// print_inputs
		//	cout << " Clock Cycle: " << clock_count;
			CAD->print_inputs();
			
//			if (argc > 2)
//			{
//				outFile << " Clock Cycle: " << clock_count << endl;
//				CAD->print_inputs(outFile);
//			}
			break;
		case 4:							// run multiple iterations
			try
			{
				for (int i = 0; i < atoi(commands.at(1).c_str()); i++)
				{
					time.propagate_logic(mode,exc);//,0);//pc[0]);
				}
			}
			catch (out_of_range)
			{
			//	cout << endl << line << endl << " Invalid command, please try again." << endl << endl;
			}
			break;
		case 5:
			quit = true;					// Exit the program
			break;
		case 6:
		//	cout << endl;
			system(line.c_str());				// ls
		//	cout << endl;
			break;
		case 7:
			system("clear");				// Clear the command prompt
			break;
		case 8:
			delete inFile;
			inFile = new ifstream(commands.at(1).c_str());	// Read a new inFile
			break;
		case 9:
			continue;					// Do nothing
			break;
		case 10:
//			CAD->report_current_logic_states(nodeFile);	// Print the value of each node to nodeFile
			break;
		case 11:
			chdir(commands.at(1).c_str());			// change directory
			break;
		case 12:
//			time.print_changed_nodes(commonality_file);	// Print commonality file
			break;
//		case 13:
//			pc[0] = atoi(commands.at(1).c_str());		// Indicate pc0
//			break;
//		case 14:
//			pc[1] = atoi(commands.at(1).c_str());		// Indicate pc1
//			break;
//		case 15:
//			pc[2] = atoi(commands.at(1).c_str());		// ...
//			break;
//		case 16:
//			pc[3] = atoi(commands.at(1).c_str());		// ...
//			break;
		case 17:
			CAD->report_logic_depth();			// Calculates the logic depth of the circuit
			break;
		case 18:
			CAD->initialize_logic_state();			// Set all internal states to zero
			break;
		case 19:
			time.print_delay(output_filename);

			}
	}
//cout<<endl;
//cout<<endl;

	return 0;

}


	// Builds a map which points each instruction to an int for use in the case statement above
void initialize_command_list(map<string,int> &command_list)
{
	command_list.insert(make_pair<string,int>("set", 0));
	command_list.insert(make_pair<string,int>("propagate_logic", 1));
	command_list.insert(make_pair<string,int>("pl", 1));
	command_list.insert(make_pair<string,int>("print_outputs", 2));
	command_list.insert(make_pair<string,int>("po", 2));
	command_list.insert(make_pair<string,int>("print_inputs", 3));
	command_list.insert(make_pair<string,int>("pi", 3));
	command_list.insert(make_pair<string,int>("run", 4));
	command_list.insert(make_pair<string,int>("quit", 5));
	command_list.insert(make_pair<string,int>("ls", 6));
	command_list.insert(make_pair<string,int>("clear", 7));
	command_list.insert(make_pair<string,int>("read", 8));
	command_list.insert(make_pair<string,int>("", 9));
	command_list.insert(make_pair<string,int>("print_all_nodes", 10));
	command_list.insert(make_pair<string,int>("pan", 10));
	command_list.insert(make_pair<string,int>("cd", 11));
	command_list.insert(make_pair<string,int>("print_changed_nodes", 12));
	command_list.insert(make_pair<string,int>("pcn", 12));
//	command_list.insert(make_pair<string,int>("pc0", 13));
//	command_list.insert(make_pair<string,int>("pc1", 14));
//	command_list.insert(make_pair<string,int>("pc2", 15));
//	command_list.insert(make_pair<string,int>("pc3", 16));
	command_list.insert(make_pair<string,int>("report_logic_depth", 17));
	command_list.insert(make_pair<string,int>("rld", 17));
	command_list.insert(make_pair<string,int>("reset", 18));
	command_list.insert(make_pair<string,int>("pd", 19));
	command_list.insert(make_pair<string,int>("", 999999));
}

	// Generate a unique name for the output of the program
char* get_output_filename(char *input_filename, char *script_file, int program_time, char *voltage, char *mode)
{
	char* slash_location;
	char* next_slash_location;
	char* time_str = new char[20];
	sprintf(time_str, "%d\0", program_time%100000);
	next_slash_location = input_filename;
	
	while (next_slash_location !=  NULL)			// remove all text prior to final slash
	{
		slash_location = next_slash_location;
		next_slash_location = strchr(slash_location + 1, '/');	// Find next slash
	}
	
	if (slash_location[0] == '/')
	{
		slash_location++;				// Move one character beyond slash
	}

	char output_filename_temp[100];
	strcpy (output_filename_temp, slash_location);		// Text following final slash
	
	char *end = strstr(output_filename_temp, "_post_syn.v");
	char* dot_location;
	if (end != NULL)
	{
		*end = '\0';					// Remove _post_syn.v from name
	}
	else 
	{
		dot_location = strchr(output_filename_temp, '.');
		*dot_location = '\0';				// Remove .v from name
	}

	next_slash_location = script_file;
	
	while (next_slash_location !=  NULL)			// remove all text prior to final slash
	{
		slash_location = next_slash_location;
		next_slash_location = strchr(slash_location + 1, '/');	// Find next slash
	}
	
	if (slash_location[0] == '/')
	{
		slash_location++;				// Move one character beyond slash
	}

	char script_temp[100];
	strcpy (script_temp, slash_location);
	dot_location = strchr(script_temp, '.');
	*dot_location = '\0';

	char *output_filename = new char[100];
	strcpy (output_filename, "../output/output_");
	strcat (output_filename, output_filename_temp);
	strcat (output_filename, "_");
	strcat (output_filename, script_temp);
	//strcat (output_filename, time_str);
	strcat (output_filename, "_");
	strcat (output_filename, voltage);
	strcat (output_filename, "_");
    strcat (output_filename, mode);
	strcat (output_filename, ".txt");

	return output_filename;
}

	// Generate a unique name for the commonality file output
char* get_commonality_filename (char *input_filename, char *script_file, int program_time)
{
	char* slash_location;
	char* next_slash_location;
	char* time_str = new char[20];
	sprintf(time_str, "%d\0", program_time%100000);
	next_slash_location = input_filename;

	while (next_slash_location !=  NULL)			// remove all text prior to final slash
	{
		slash_location = next_slash_location;
		next_slash_location = strchr(slash_location + 1, '/');	// Find next slash
	}

	
	if (slash_location[0] == '/')
	{
		slash_location++;				// Move one character beyond slash
	}
	
	char commonality_file_temp[100];
	strcpy (commonality_file_temp, slash_location);		// Text following final slash

	char *end = strstr(commonality_file_temp, "_post_syn.v");	// Find location of _post_syn.v in file name
	char* dot_location;
	if (end != NULL)
	{
		*end = '\0';					// Remove _post_syn.v from name
	}
	else 
	{
		dot_location = strchr(commonality_file_temp, '.');
		*dot_location = '\0';				// Remove .v from name
	}

	next_slash_location = script_file;
	
	while (next_slash_location !=  NULL)			// remove all text prior to final slash
	{
		slash_location = next_slash_location;
		next_slash_location = strchr(slash_location + 1, '/');	// Find next slash
	}
	
	if (slash_location[0] == '/')
	{
		slash_location++;				// Move one character beyond slash
	}

	char script_temp[100];
	strcpy (script_temp, slash_location);
	dot_location = strchr(script_temp, '.');
	*dot_location = '\0';


	char *commonality_file = new char[100];
	strcpy (commonality_file, "/home/A00871817/Brennan/tools/sta_tool/output/");
	strcat (commonality_file, commonality_file_temp);	// Append name of input file
	strcat (commonality_file, "_");
	strcat (commonality_file, script_temp);
	strcat (commonality_file, time_str);			// Append number corresponding to the time
	strcat (commonality_file, ".txt");			// Append .txt

	return commonality_file;
}
