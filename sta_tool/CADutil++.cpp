#include "CAD.h"
int g_endian;
CADutil::CADutil(char* s_new, char* d_new){
        v = s_new;
        dl = d_new;
}

void CADutil::del_spaces(std::string& s)
{
	string::iterator end_pos = std::remove(s.begin(), s.end(), ' ');
	s.erase(end_pos, s.end());
}

void CADutil::parse_syn_v(MyGraphType& G_ref) {  //Calls for parsing of various gates like INV, NAND and NOR
        
	char buffer[8192];
	char buffer2[2048];

//	cout << "Parsing verilog file: " << v << endl;

        string s = "INV";
        ifstream f(v);
        istreambuf_iterator<char> eof;
        while(!f.eof()){
            if((eof == search(istreambuf_iterator<char>(f), eof, s.begin(), s.end())) )
                break;
            else{
                f.getline(buffer, 2048);
		//cout << buffer << endl;
		while (!strpbrk(buffer, ";"))		// If gate declaration appears on multiple lines
		{
			f.getline(buffer2, 2048);
			strcat(buffer, buffer2);
		}
                string str =buffer;
		//cout << "Printing str:" << str << endl;
		string s_type = s.substr(0, s.size()-1)+str;
		//cout << "Printing s_type" << s_type << endl;
		//cout << s_type << endl;
//cout<<"get_type"<<endl;
		int inv_type = CADutil::get_type(s_type);
//------------------------------ 
//		int inv_pos = s_type.find("U");
//		string inv_name = s_type.substr(inv_pos, inv_pos +5);
//		if (inv_name.compare(5,1," ")!=0)
//			inv_name.resize(6);
//		else
//			inv_name.resize(5);
		int inv_pos = s_type.find(" ");
		++inv_pos;
//		cout<<"this the pos "<<inv_pos<<endl;
		int inv_pos2 = s_type.find(" ",inv_pos+1);
	   	int inv_length =   inv_pos2 - inv_pos;
		string inv_name = s_type.substr(inv_pos, inv_length);
//------------------------------ 
//		cout << "The gate is: " << s_type << " and its type is: " << inv_type << endl;
//cout << inv_name<< endl;
//cout<<"parsei"<<endl;
                parse_inverter(str, inv_type, inv_name, CADutil::G_ref);
            }
            
        }
	f.close();

        s = "NOR";
        ifstream f1(v);
        while(!f1.eof()){
            if((eof == search(istreambuf_iterator<char>(f1), eof, s.begin(), s.end())) )
                break;
            else {
                f1.getline(buffer, 2048);
		while (!strpbrk(buffer, ";"))
		{
			f1.getline(buffer2, 2048);
			strcat(buffer, buffer2);
		}
                string str = buffer;
//		cout << "str = " << str << endl;
//              parse_nand_and_nor(str, 1, CADutil::G_ref);
                string s_type = s.substr(0, s.size()-1)+str;
//cout<<"get type"<<endl;
		int nor_type = CADutil::get_type(s_type);
//------------------------------ 
//		int nor_pos = s_type.find("U");
//		string nor_name = s_type.substr(nor_pos, nor_pos +5);		
//		if (nor_name.compare(5,1," ")!=0)
//			nor_name.resize(6);
//		else
//			nor_name.resize(5);
		int nor_pos = s_type.find(" ");
		++nor_pos;
		int nor_pos2 = s_type.find(" ", nor_pos + 1);
		int nor_length = nor_pos2 - nor_pos;
//		string nor_name = s_type.substr(nor_pos, nor_pos +5);

		string nor_name = s_type.substr(nor_pos, nor_length);

					
//		cout << "The gate is: " << s_type << " and its type is: " << nor_type << endl;
//cout<<nor_name<<endl;
//------------------------------ 
             	parse_nand_and_nor(str, nor_type, nor_name, CADutil::G_ref);
             
            }

        }
	f1.close();

        s = "NAND";
        ifstream f2(v);
        while(!f2.eof()) {
                if((eof == search(istreambuf_iterator<char>(f2), eof, s.begin(), s.end())) )
                    break;
                else {
                        f2.getline(buffer, 2048);
			while (!strpbrk(buffer, ";"))
			{
				f2.getline(buffer2, 2048);
				strcat(buffer, buffer2);
			}
                        string str = buffer;
//			cout << "str = " << str << endl;
//                      parse_nand_and_nor(str, 2, CADutil::G_ref);
	                string s_type = s.substr(0, s.size()-1)+str;
//cout<<"get type"<<endl;
			int nand_type = CADutil::get_type(s_type);
//------------------------------ 
//			int nand_pos = s_type.find("U");
//			string nand_name = s_type.substr(nand_pos, nand_pos +5);
//			if (nand_name.compare(5,1," ")!=0)
//				nand_name.resize(6);
//			else
//				nand_name.resize(5);
			int nand_pos = s_type.find(" ");
			++nand_pos;

			int nand_pos2 = s_type.find(" ", nand_pos + 1);
			int nand_length = nand_pos2 - nand_pos;
//			string nand_name = s_type.substr(nand_pos, nand_pos +5);
			string nand_name = s_type.substr(nand_pos, nand_length);

							
//			cout << "The gate is: " << s_type << " and its type is: " << nand_type << endl;
//cout<<nand_name<<endl;
//------------------------------ 
			parse_nand_and_nor(str, nand_type, nand_name, CADutil::G_ref);
                }
        }
	f2.close();

	s = "DFX";
	ifstream f3(v);
        while(!f3.eof()) {
                if((eof == search(istreambuf_iterator<char>(f3), eof, s.begin(), s.end())) )
                    break;
                else {
                        f3.getline(buffer, 2048);
			while (!strpbrk(buffer, ";"))	
			{
				f3.getline(buffer2, 2048);
				strcat(buffer, buffer2);
			}
                        string str = buffer;
//	                string s_type = s.substr(0, s.size()-1)+str;
////cout<<"get type"<<endl;
//			int dfx_type = CADutil::get_type(s_type);
                        parse_D_flipflop(str, CADutil::G_ref, DFX);
                }
        }
	f3.close();
	
	s = "DFC";
	ifstream f7(v);
  	 while(!f7.eof()) {
                if((eof == search(istreambuf_iterator<char>(f7), eof, s.begin(), s.end())) )
                    break;
                else {
                        f7.getline(buffer, 2048);
			while (!strpbrk(buffer, ";"))	
			{
				f7.getline(buffer2, 2048);
				strcat(buffer, buffer2);
			}
                        string str = buffer;
//	                string s_type = s.substr(0, s.size()-1)+str;
////cout<<"get type"<<endl;
//			int dfx_type = CADutil::get_type(s_type);
                        parse_D_flipflop(str, CADutil::G_ref, DFC);
                }
        }
	f7.close();


	s = "input";
	ifstream f4(v);
        while(!f4.eof()) {
                if((eof == search(istreambuf_iterator<char>(f4), eof, s.begin(), s.end())) )
                    break;
                else {
                        f4.getline(buffer, 2048);
			while (!strpbrk(buffer, ";"))	
			{
				f4.getline(buffer2, 2048);
				strcat(buffer, buffer2);
			}
                        string str = buffer;
                        parse_input(str);
                }
        }
	f4.close();

	s = "output";
	ifstream f5(v);
        while(!f5.eof()) {
                if((eof == search(istreambuf_iterator<char>(f5), eof, s.begin(), s.end())) )
                    break;
                else {
                        f5.getline(buffer, 2048);
			while (!strpbrk(buffer, ";"))	
			{
				f5.getline(buffer2, 2048);
				strcat(buffer, buffer2);
			}
                        string str = buffer;
                        parse_output(str);
                }
        }
	f5.close();

	s = "assign";
        ifstream f6(v);

        while(!f6.eof()) {
                if((eof == search(istreambuf_iterator<char>(f6), eof, s.begin(), s.end())) )
                    break;
                else {
                        f6.getline(buffer, 2048);
			while (!strpbrk(buffer, ";"))
			{
				f6.getline(buffer2, 2048);
				strcat(buffer, buffer2);
			}
                        string str = buffer;
                        parse_constant_output(str);
                }
        }
	f6.close();

    // debug
    //std::cout << "Size of node_name (after populating the map) : " << nodes_name.size() << std::endl;
    //for (map<string, int>::iterator it = nodes_name.begin(); it != nodes_name.end(); ++it)
	//{
    //  std::cout << "net name: " << it->first << ", value: " << it->second << std::endl;
    //}

 /***************************hchen for fan-out calculation******************************************/
	string line_lookup ="";
	string line1_lookup ="";
	ifstream f_lookup(v);
	while(getline(f_lookup,line_lookup)){
		if (!(line_lookup.find("INV")==string::npos&&
			line_lookup.find("NAND")==string::npos&&
			line_lookup.find("NOR")==string::npos&&
			line_lookup.find("DFX")==string::npos&&
			line_lookup.find("DFC")==string::npos)&&
			line_lookup.find(";") == string::npos){
			getline(f_lookup,line1_lookup);
			line_lookup = line_lookup + line1_lookup;
			while(line_lookup.find(";")==string::npos){
				getline(f_lookup,line1_lookup);
				line_lookup = line_lookup + line1_lookup;
			}
		}
		string line = line_lookup;
		if(line_lookup.find("INV")!=string::npos){
		//	cout<<"parseinv"<<endl;
			basic_string <char> line2 = line.substr ( line.find(".I") , line.find(")") + 1 - line.find(".I"));
			basic_string <char> in = line2.substr (line2.find("(") + 1,line2.find(")") - line2.find("(") - 1);
			while (in[0] == ' ')		// Remove blank spaces preceding argument when gate declaration utilizes many lines
			{
				in.erase(0, 1);
			}
			CADutil::del_spaces(in);

			basic_string <char> line3 = line.substr ( line.find(".ZN") , line.find(")") + 1 - line.find(".ZN"));
			basic_string <char> out = line3.substr ( line3.find("(") + 1,line3.find(")") - line3.find("(") - 1);
			while (out[0] == ' ')
			{
				out.erase(0, 1);
			}
			CADutil::del_spaces(out);
			if(GATE_LOAD.count(nodes_name[in])==0){//no load yet
				GATE_LOAD[nodes_name[in]] = get_size(GATES[nodes_name[out]]);
			}
			else{
				GATE_LOAD[nodes_name[in]] += get_size(GATES[nodes_name[out]]);
			}

			GATE_DRIVE[nodes_name[out]] = get_size(GATES[nodes_name[out]]);//specify the drive
		//	cout<<"parseend"<<endl;
		}
		
		else if(line_lookup.find("NAND2")!=string::npos || line_lookup.find("NOR2")!=string::npos){
		//	cout<<"parsenn"<<endl;
			basic_string <char> line2 = line.substr ( line.find(".A1") , line.find(")") + 1 - line.find(".A1"));
			basic_string <char> in1 = line2.substr ( line2.find("(") + 1,line2.find(")") - line2.find("(") - 1); 
			while (in1[0] == ' ')		// Remove blank spaces preceding argument when gate declaration utilizes many lines
			{
				in1.erase(0, 1);
			}
			CADutil::del_spaces(in1);
			
			basic_string <char> line3 = line.substr ( line.find(".A2") , line.find(")") + 1 - line.find(".A2"));
			basic_string <char> in2 = line3.substr ( line3.find("(") + 1,line3.find(")") - line3.find("(") - 1);	
			while (in2[0] == ' ')
			{
				in2.erase(0, 1);
			}  
			CADutil::del_spaces(in2);

			basic_string <char> line4 = line.substr ( line.find(".ZN") , line.find(")") + 1 - line.find(".ZN"));
			basic_string <char> out = line4.substr ( line4.find("(") + 1,line4.find(")") - line4.find("(") - 1);
			while (out[0] == ' ')
			{
				out.erase(0, 1);
			}
			CADutil::del_spaces(out);

			if(GATE_LOAD.count(nodes_name[in1])==0){//no load yet
				GATE_LOAD[nodes_name[in1]] = get_size(GATES[nodes_name[out]]);
			}
			else{
				GATE_LOAD[nodes_name[in1]] += get_size(GATES[nodes_name[out]]);
			}

			if(GATE_LOAD.count(nodes_name[in2])==0){//no load yet
				GATE_LOAD[nodes_name[in2]] = get_size(GATES[nodes_name[out]]);
			}
			else{
				GATE_LOAD[nodes_name[in2]] += get_size(GATES[nodes_name[out]]);
			}

			GATE_DRIVE[nodes_name[out]] = 2.0*get_size(GATES[nodes_name[out]]);//specify the drive
		//	cout<<"parseend"<<endl;
		}
		else if(line_lookup.find("DFX")!=string::npos){
		//	cout<<"parsedfx"<<endl;
			basic_string <char> DA_line = line.substr ( line.find(".DA") , line.find(")") + 1 - line.find(".DA"));
			basic_string <char> DA = DA_line.substr (DA_line.find("(") + 1,DA_line.find(")") - DA_line.find("(") - 1);
			while (DA[0] == ' ')		// Remove blank spaces
			{
				DA.erase(0, 1);
			}
			CADutil::del_spaces(DA);

			basic_string <char> DB_line = line.substr ( line.find(".DB") , line.find(")") + 1 - line.find(".DB"));
			basic_string <char> DB = DB_line.substr ( DB_line.find("(") + 1,DB_line.find(")") - DB_line.find("(") - 1);
			while (DB[0] == ' ')
			{
				DB.erase(0, 1);
			}
			CADutil::del_spaces(DB);

			basic_string <char> Q_line = line.substr ( line.find(".Q") , line.find(")") + 1 - line.find(".Q"));
			basic_string <char> Q = Q_line.substr ( Q_line.find("(") + 1,Q_line.find(")") - Q_line.find("(") - 1);
			while (Q[0] == ' ')
			{
				Q.erase(0, 1);
			}
			CADutil::del_spaces(Q);

			basic_string <char> SA_line = line.substr ( line.find(".SA") , line.find(")") + 1 - line.find(".SA"));
			basic_string <char> SA = SA_line.substr ( SA_line.find("(") + 1,SA_line.find(")") - SA_line.find("(") - 1);
			while (SA[0] == ' ')
			{
				SA.erase(0, 1);
			}
			CADutil::del_spaces(SA);

			if(GATE_LOAD.count(nodes_name[DA])==0){//no load yet
				GATE_LOAD[nodes_name[DA]] = 0.0004907;
			}
			else{
				GATE_LOAD[nodes_name[DA]] += 0.0004907;
			}

			if(GATE_LOAD.count(nodes_name[DB])==0){//no load yet
				GATE_LOAD[nodes_name[DB]] = 0.0007081;
			}
			else{
				GATE_LOAD[nodes_name[DB]] += 0.0007081;
			}
			if(GATE_LOAD.count(nodes_name[SA])==0){//no load yet
				GATE_LOAD[nodes_name[SA]] = 0.001042;
			}
			else{
				GATE_LOAD[nodes_name[SA]] += 0.001042;
			}
		//	cout<<"parseend"<<endl;
		}	
	else if(line_lookup.find("DFC")!=string::npos){
		//	cout<<"parsedfx"<<endl;
			basic_string <char> D_line = line.substr ( line.find(".D") , line.find(")") + 1 - line.find(".D"));
			basic_string <char> D = D_line.substr (D_line.find("(") + 1,D_line.find(")") - D_line.find("(") - 1);
			while (D[0] == ' ')		// Remove blank spaces
			{
				D.erase(0, 1);
			}
			CADutil::del_spaces(D);

			basic_string <char> Q_line = line.substr ( line.find(".Q") , line.find(")") + 1 - line.find(".Q"));
			basic_string <char> Q = Q_line.substr ( Q_line.find("(") + 1,Q_line.find(")") - Q_line.find("(") - 1);
			while (Q[0] == ' ')
			{
				Q.erase(0, 1);
			}
			CADutil::del_spaces(Q);

			basic_string <char> CDN_line = line.substr ( line.find(".CDN") , line.find(")") + 1 - line.find(".CDN"));
			basic_string <char> CDN = CDN_line.substr ( CDN_line.find("(") + 1,CDN_line.find(")") - CDN_line.find("(") - 1);
			while (CDN[0] == ' ')
			{
				CDN.erase(0, 1);
			}
			CADutil::del_spaces(CDN);
			
			//assumes DFCNQD1BWPLVT
			if(GATE_LOAD.count(nodes_name[D])==0){//no load yet
				GATE_LOAD[nodes_name[D]] = 0.0005979;
			}
			else{
				GATE_LOAD[nodes_name[D]] += 0.0005979;
			}
			if(GATE_LOAD.count(nodes_name[CDN])==0){//no load yet
				GATE_LOAD[nodes_name[CDN]] = 0.00122;
			}
			else{
				GATE_LOAD[nodes_name[CDN]] += 0.00122;
			}
		//	cout<<"parseend"<<endl;
		}
	}

	f_lookup.close();
//
	map<int, float>::const_iterator GATE_DRIVE_IT = GATE_DRIVE.begin();
	while (GATE_DRIVE_IT != GATE_DRIVE.end()) {
		GATE_FOUT[GATE_DRIVE_IT->first] = GATE_LOAD[GATE_DRIVE_IT->first]/GATE_DRIVE_IT->second;
//		cout << "type is: "<< GATES[GATE_DRIVE_IT->first] <<", fan-out is: "<<GATE_FOUT[GATE_DRIVE_IT->first]<<endl;
 		++GATE_DRIVE_IT; // increment iterator to denote the next element
 	}
//

/****************************************************************************************/
   

    //std::cout << "Size of node_name (inside parse_syn_v()) : " << nodes_name.size() << std::endl;
    //parameter 'string inv_name' added by Kenyon

}

void CADutil::parse_inverter(string line, int inv_type, string inv_name, MyGraphType& G_ref){			//Parsing of inverter in the circuit.
	
	property_map<MyGraphType, vertex_name_t>::type name = get(vertex_name, CADutil::G_ref);
	
	basic_string <char> line2 = line.substr ( line.find(".I") , line.find(")") + 1 - line.find(".I"));
	//cout << "Printing line2" << line2 << endl;
	basic_string <char> in = line2.substr (line2.find("(") + 1,line2.find(")") - line2.find("(") - 1);
	//string::iterator end_pos = std::remove(in.begin(), in.end(), ' ');
        //in.erase(end_pos, in.end());


	//cout << "Printing in" << in <<"space?"<< endl;
	while (in[0] == ' ')		// Remove blank spaces preceding argument when gate declaration utilizes many lines
	{
		in.erase(0, 1);
	}
	//cout << "Printing in" << in <<"hello" <<endl;
        //string::iterator end_pos = std::remove(in.begin(), in.end(), ' ');
        //in.erase(end_pos, in.end());
        CADutil::del_spaces(in);
	//cout << "Printing in" << in <<"space?"<< endl;


	basic_string <char> line3 = line.substr ( line.find(".ZN") , line.find(")") + 1 - line.find(".ZN"));
	//cout << "Printing line 3 " << line3 << endl;
	basic_string <char> out = line3.substr ( line3.find("(") + 1,line3.find(")") - line3.find("(") - 1);
	while (out[0] == ' ')
	{
		out.erase(0, 1);
	}
	CADutil::del_spaces(out);
	//cout << "Printing out" << out << endl; 	
	//cout << "Node Name " << out << endl;
	vertex_descriptor u, x;
	//cout << x << endl;
	bool inserted;

	tie(CADutil::pos, inserted) = CADutil::nodes.insert(std::make_pair(in, vertex_descriptor()));
	if (inserted) {
		x = add_vertex(CADutil::G_ref);
		CADutil::pos->second = x;
		CADutil::nodes_name[in] = x;
		index_to_node[x] = in;
	} 
	else{
		x = CADutil::pos->second;
	}
	//cout << in << endl;
	tie(CADutil::pos, inserted) = CADutil::nodes.insert(std::make_pair(out, vertex_descriptor()));	//Insert nodes in the graph 
	if (inserted) {									//If not inserted till now
		u = add_vertex(CADutil::G_ref);	
		pos->second = u;
		nodes_name[out] = u;
		index_to_node[u] = out;
	} else{
		u = CADutil::pos->second;
	}
	//cout << u << endl;
	edge_descriptor e;
	tie(e, inserted) = add_edge(u, x, CADutil::G_ref);				//Add Edge between the inserted nodes.
	
	
	CADutil::edges_from.insert(pair<int, int>(u, target(e, CADutil::G_ref)));
	//for (multimap<int, int>::iterator it = CADutil::edges_from.begin(); it != CADutil::edges_from.end(); ++it)
	//{
		//cout << (*it).first << "target" << (*it).second << endl;	
	//}  
//	cout << CADutil::edges_from[u] << endl;
//	CADutil::GATES[u] = 0;
	CADutil::GATES[u] = inv_type;
	CADutil::GATENAMES[u] = inv_name; //Added by Kenyon
//	cout <<out <<" "<<inv_type << endl;
	
    //std::cout << "within parse_inverter, setting " << index_to_node[u] << " to false ..." << std::endl;
    node_ready[u] = false;
}

void CADutil::parse_nand_and_nor(string line, int type, string gate_name, MyGraphType& G_ref) {	//Parse the nand and nor gates in the circuit
	
	property_map<MyGraphType, vertex_name_t>::type name = get(vertex_name, CADutil::G_ref);
	
	basic_string <char> line2 = line.substr ( line.find(".A1") , line.find(")") + 1 - line.find(".A1"));
	basic_string <char> in1 = line2.substr ( line2.find("(") + 1,line2.find(")") - line2.find("(") - 1); 
	while (in1[0] == ' ')		// Remove blank spaces preceding argument when gate declaration utilizes many lines
	{
		in1.erase(0, 1);
	}
	CADutil::del_spaces(in1);
	
	basic_string <char> line3 = line.substr ( line.find(".A2") , line.find(")") + 1 - line.find(".A2"));
	basic_string <char> in2 = line3.substr ( line3.find("(") + 1,line3.find(")") - line3.find("(") - 1);	
	while (in2[0] == ' ')
	{
		in2.erase(0, 1);
	}  
	CADutil::del_spaces(in2);

	basic_string <char> line4 = line.substr ( line.find(".ZN") , line.find(")") + 1 - line.find(".ZN"));
	basic_string <char> out = line4.substr ( line4.find("(") + 1,line4.find(")") - line4.find("(") - 1);
	while (out[0] == ' ')
	{
		out.erase(0, 1);
	}
	CADutil::del_spaces(out);
	
	//cout << "Node Name " << out << "  in1 =" << in1 << "   in2 == " << in2 ;
	vertex_descriptor u, x, w;							//Insert the nodes in the graph
	bool inserted;									//If not inserted till now.

	tie(CADutil::pos, inserted) = CADutil::nodes.insert(std::make_pair(in1, vertex_descriptor()));
	if (inserted) {
		x = add_vertex(CADutil::G_ref);
		CADutil::pos->second = x;
        //std::cout << "in1: " << in1 << "CHECK" << std::endl;
		nodes_name[in1] = x;
		index_to_node[x] = in1;
		//cout << "  INSERTING in1";
	} 
	else{
		x = CADutil::pos->second;
	}
	
	tie(CADutil::pos, inserted) = CADutil::nodes.insert(std::make_pair(in2, vertex_descriptor()));
	if (inserted) {
		w = add_vertex(CADutil::G_ref);
		CADutil::pos->second = w;
        //std::cout << "in2: " << in2 << "CHECK" << std::endl;
		nodes_name[in2] = w;
		index_to_node[w] = in2;
		//cout << "  INSERTING in2"; 
	} 
	else{
		w = CADutil::pos->second;
	}

	tie(CADutil::pos, inserted) = CADutil::nodes.insert(std::make_pair(out, vertex_descriptor()));
	if (inserted) {
		u = add_vertex(CADutil::G_ref);
		pos->second = u;
        //std::cout << "out: " << out << "CHECK" << std::endl;
		nodes_name[out] = u;
		index_to_node[u] = out;
	} 
	else{
		u = CADutil::pos->second;
	}
	
	//cout << endl;
	edge_descriptor e, f;
	tie(e, inserted) = add_edge(u, x, CADutil::G_ref);				//Add edges between the inserted nodes.
	tie(f, inserted) = add_edge(u, w, CADutil::G_ref);
	CADutil::edges_from.insert(pair<int, int>(u, target(e, CADutil::G_ref)));
	CADutil::edges_from.insert(pair<int, int>(u, target(f, CADutil::G_ref)));
	
	CADutil::GATES[u] = type;
	CADutil::GATENAMES[u] = gate_name; //Added by Kenyon
//	cout << out <<" "<<type << endl;

    //std::cout << "within parse_nand_and_nor, setting " << index_to_node[u] << " to false ..." << std::endl;
    node_ready[u] = false;
}

float CADutil::get_size(int type){//just the size of one/smallest pin, may need to be scaled up to get the whole gate size
	if (type==0)
		return 0.824335;
	else if (type==1)
		return 1.649773;
	else if (type==2)
		return 3.301213;
	else if (type==3)
		return 6.605341;
	else if (type==4)
		return 9.908848;
	else if (type==5)
		return 13.212561;
	else if (type==6)
		return 0.685393;
	else if (type==7)
		return 1.371191;
	else if (type==8)
		return 0.631908;
	else if (type==9)
		return 1.263983;
	else if (type==10)
		return 0.599944;
	else if (type==11)
		return 1.199280;
	else if (type==12)
		return 0.691696;
	else if (type==13)
		return 1.391460;
	else if (type==14)
		return 0.638205;
	else if (type==15)
		return 1.283452;
	else if (type==16)
		return 0.612535;
	else if (type==17)
		return 1.231682;
	
}


int CADutil::get_type(string s){

	// inverters (0 to 5) 
	size_t found = s.find("INV_X1");
	if (found != string::npos)
		return 0;
	found = s.find("INV_X2");
	if (found != string::npos)
		return 1;
	found = s.find("INV_X4");
	if (found != string::npos)
		return 2;
        found = s.find("INV_X8");
	if (found != string::npos)
                return 3;
        found = s.find("INV_X12");
        if (found != string::npos)
                return 4;
        found = s.find("INV_X16");
        if (found != string::npos)
                return 5;
        
	//nands (6 to 11)
        found = s.find("NAND2_X1");
	if (found != string::npos)
	        return 6;
	found = s.find("NAND2_X2");
	if (found != string::npos)
	        return 7;
	found = s.find("NAND3_X1");
	if (found != string::npos)
	        return 8;
	found = s.find("NAND3_X2");
	if (found != string::npos)
	        return 9;
	found = s.find("NAND4_X1");
	if (found != string::npos)
	        return 10;
	found = s.find("NAND4_X2");
	if (found != string::npos)
	        return 11;

	//nors (12 to 17)
        found = s.find("NOR2_X1");
        if (found != string::npos)
                return 12;
        found = s.find("NOR2_X2");
        if (found != string::npos)
                return 13;
        found = s.find("NOR3_X1");
        if (found != string::npos)
                return 14;
        found = s.find("NOR3_X2");
        if (found != string::npos)
                return 15;
        found = s.find("NOR4_X1");
        if (found != string::npos)
                return 16;
        found = s.find("NOR4_X2");
        if (found != string::npos)
                return 17;
        
	cout << "gate type can't be matched: " << s << endl;
	exit(0);
}

void CADutil::parse_D_flipflop(string line, MyGraphType& G_ref, DFF_type type)
{
	//property_map<MyGraphType, vertex_name_t>::type name = get(vertex_name, CADutil::G_ref);
	D_flipflop dff;
	basic_string <char> DA_line, DA, DB_line, DB, Q_line, Q, SA_line, SA;
	dff.type = type;

	switch (type)
	  {
	  case DFX://Kenneth added 2/4/15 - D-flip-flop with mux input
	    DA_line = line.substr ( line.find(".DA") , line.find(")") + 1 - line.find(".DA"));
	    DA = DA_line.substr (DA_line.find("(") + 1,DA_line.find(")") - DA_line.find("(") - 1);
	    while (DA[0] == ' ')		// Remove blank spaces
	      {
		DA.erase(0, 1);
	      }
             CADutil::del_spaces(DA);	    
  	
	    DB_line = line.substr ( line.find(".DB") , line.find(")") + 1 - line.find(".DB"));
	    DB = DB_line.substr ( DB_line.find("(") + 1,DB_line.find(")") - DB_line.find("(") - 1);
	    while (DB[0] == ' ')
	      {
		DB.erase(0, 1);
	      }
	    CADutil::del_spaces(DB);	

	    Q_line = line.substr ( line.find(".Q") , line.find(")") + 1 - line.find(".Q"));
	    Q = Q_line.substr ( Q_line.find("(") + 1,Q_line.find(")") - Q_line.find("(") - 1);
	    while (Q[0] == ' ')
	      {
		Q.erase(0, 1);
	      }
	    CADutil::del_spaces(Q);	

	    SA_line = line.substr ( line.find(".SA") , line.find(")") + 1 - line.find(".SA"));
	    SA = SA_line.substr ( SA_line.find("(") + 1,SA_line.find(")") - SA_line.find("(") - 1);
	    while (SA[0] == ' ')
	      {
		SA.erase(0, 1);
	      }
	    CADutil::del_spaces(SA);
	    break;
	  case DFC://D-flip-flop with asynchronous reset (asynchronous part unsupported)
	    DA_line = line.substr ( line.find(".D") , line.find(")") + 1 - line.find(".D"));
	    DA = DA_line.substr (DA_line.find("(") + 1,DA_line.find(")") - DA_line.find("(") - 1);
	    while (DA[0] == ' ')		// Remove blank spaces
	      {
		DA.erase(0, 1);
	      }
	    CADutil::del_spaces(DA);

	    DB = DA; //Map D to DA and DB

	    Q_line = line.substr ( line.find(".Q") , line.find(")") + 1 - line.find(".Q"));
	    Q = Q_line.substr ( Q_line.find("(") + 1,Q_line.find(")") - Q_line.find("(") - 1);
	    while (Q[0] == ' ')
	      {
		Q.erase(0, 1);
	      }
	    CADutil::del_spaces(Q);

	    SA_line = line.substr ( line.find(".CDN") , line.find(")") + 1 - line.find(".CDN"));
	    SA = SA_line.substr ( SA_line.find("(") + 1,SA_line.find(")") - SA_line.find("(") - 1);
	    while (SA[0] == ' ')
	      {
		SA.erase(0, 1); //Map CDN to SA
	      }
	    CADutil::del_spaces(SA);
	    break;
	  }
/*******************************************************************************************************************************/
	vertex_descriptor DA_index, DB_index, Q_index, SA_index;
	bool inserted;

	tie(CADutil::pos, inserted) = CADutil::nodes.insert(std::make_pair(DA, vertex_descriptor()));
	if (inserted) {
		DA_index = add_vertex(CADutil::G_ref);
		CADutil::pos->second = DA_index;
		nodes_name[DA] = DA_index;
		index_to_node[DA_index] = DA;
//		cout << "  INSERTING DA" << DA << endl;
	} 
	else{
		DA_index = CADutil::pos->second;
	}

	tie(CADutil::pos, inserted) = CADutil::nodes.insert(std::make_pair(DB, vertex_descriptor()));
	if (inserted) {
		DB_index = add_vertex(CADutil::G_ref);
		CADutil::pos->second = DB_index;
		nodes_name[DB] = DB_index;
		index_to_node[DB_index] = DB;
//		cout << "  INSERTING DB" << DB << endl;
	} 
	else{
		DB_index = CADutil::pos->second;
	}

	tie(CADutil::pos, inserted) = CADutil::nodes.insert(std::make_pair(Q, vertex_descriptor()));
	if (inserted) {
		Q_index = add_vertex(CADutil::G_ref);
		CADutil::pos->second = Q_index;
		nodes_name[Q] = Q_index;
		index_to_node[Q_index] = Q;
//		cout << "  INSERTING Q" << Q << endl;
	} 
	else{
		Q_index = CADutil::pos->second;
	}

	tie(CADutil::pos, inserted) = CADutil::nodes.insert(std::make_pair(SA, vertex_descriptor()));
	if (inserted) {
		SA_index = add_vertex(CADutil::G_ref);
		CADutil::pos->second = SA_index;
		nodes_name[SA] = SA_index;
		index_to_node[SA_index] = SA;
//		cout << "  INSERTING SA";
	} 
	else{
		SA_index = CADutil::pos->second;
	}

	dff.DA_index = nodes_name[DA];
	dff.DB_index = nodes_name[DB];
	dff.Q_index = nodes_name[Q];
	dff.SA_index = nodes_name[SA];

	CADutil::D_flipflops.push_back(dff);	// Store D flipflop in vector

	node_ready[Q_index] = true;

//	float fout = 0;// hchen for fan-out calculation
//	string line_lookup ="";
//	string line1_lookup = "";
//	ifstream f_lookup(v);
//	while(getline(f_lookup,line_lookup)){
//		if (!(line_lookup.find("INV")==string::npos&&
//			line_lookup.find("ND2")==string::npos&&
//			line_lookup.find("NR2")==string::npos&&
//			line_lookup.find("DFX")==string::npos&&
//			line_lookup.find("FFGEN")==string::npos)&&
//			line_lookup.find(";") == string::npos){
//			getline(f_lookup,line1_lookup);
//			line_lookup = line_lookup + line1_lookup;
//			while(line_lookup.find(";")==string::npos){
//				getline(f_lookup,line1_lookup);
//				line_lookup = line_lookup + line1_lookup;
//			}
////cout<<line_lookup<<endl;
//		}
//		if (line_lookup.find(".SA("+Q+")") != string::npos){
//			fout += 2*get_size(get_type(line_lookup))/(4*get_size(type));
//		}
//		else if (line_lookup.find("("+Q+")") != string::npos){
////cout<<line_lookup<<endl;
//			fout += get_size(get_type(line_lookup))/(4*get_size(type));
//		}
//			
//	}
//	fout -= 1;
//	CADutil::GATE_FOUT[u] = fout;
//	cout << "type is: "<< type <<", fan-out is: "<<fout<<endl;
//
//	node_ready[u] = false;
}

void CADutil::parse_input(string line)
{
	string inputs, curr_input, input_parse;
	int low, high;
	char buffer[256], input_name[256];

	if (line.find("[") != inputs.npos)			// Input is a vector
	{
		basic_string <char> limits = line.substr ( line.find("[") + 1 , line.find("]") - line.find("["));
		basic_string <char> upper_lim = limits.substr (0, limits.find(":"));
		basic_string <char> lower_lim = limits.substr ( limits.find(":") + 1 , limits.length() - limits.find(":") - 2 );
		inputs = line.substr ( line.find("] ") + 2 , line.find(";") - line.find("] ") - 2);

		strcpy(buffer, lower_lim.c_str());		// Set upper limit of the bus
		low = atoi(buffer);
		strcpy(buffer, upper_lim.c_str());		// Set lower limit of the bus
		high = atoi(buffer);		
	}
	else							// Input is a single bit
	{
		inputs = line.substr ( line.find("t ") + 2 , line.find(";") - line.find("t ") - 2 );
		low = 0;
		high = 0;
	}

	while(!inputs.empty())
	{	
		if (inputs.find(", ") != inputs.npos)		// Check for multiple inputs declared on the line
		{
			curr_input = inputs.substr(0, inputs.find(", "));
			inputs = inputs.substr( inputs.find(", ") + 2 , inputs.length() + 1 );
		}
		else
		{
			curr_input = inputs;
			inputs = "";
		}
		
		while(curr_input[0] == ' ')
		{
			curr_input.erase(0,1);			// Remove blank spaces preceding input
		}
		//cout << "The current input:" << curr_input <<"Hello"<< endl;
		//cout << "The high is: " << high << "and the low is:" << low << endl; 
	
		input_size[curr_input] = high - low + 1;

		if (low != high)				// Input is multiple bits
		{
            if (high < low)
			{
				int t;
				t = low;
				low = high;
				high = t;
				g_endian = 1;
			}
		else
		{
			g_endian = 0;
		}
		//	cout << curr_input << ", high: " << high << " low: " << low << endl;
			strcpy(input_name, curr_input.c_str());
			strcat(input_name, "[%d]");

			for (int i = low; i <= high; i++)
			{	
				sprintf(buffer, input_name, i);
				curr_input = buffer;
                //std::cout << curr_input << std::endl;
				node_ready[ nodes_name[curr_input] ] = true;	// Set node to be ready for calculation
				//input_list.push_back(nodes_name[curr_input]);
				input_list.insert(nodes_name[curr_input]);
			}
		}
		else						// Single bit
		{
			node_ready[ nodes_name[curr_input] ] = true;		// Node ready for calculation
			//input_list.push_back(nodes_name[curr_input]);
			input_list.insert(nodes_name[curr_input]);
		}
	}
}

void CADutil::parse_output(string line)
{
	string outputs, curr_output, output_parse;
	int low, high;
	char buffer[256], output_name[256];

	if (line.find("[") != outputs.npos)
	{
		basic_string <char> limits = line.substr ( line.find("[") + 1 , line.find("]") - line.find("["));
		basic_string <char> upper_lim = limits.substr (0, limits.find(":"));
		basic_string <char> lower_lim = limits.substr ( limits.find(":") + 1 , limits.length() - limits.find(":") - 2 );
		outputs = line.substr ( line.find("] ") + 2 , line.find(";") - line.find("] ") - 2);

		strcpy(buffer, lower_lim.c_str());		// Set upper limit of the bus
		low = atoi(buffer);
		strcpy(buffer, upper_lim.c_str());		// Set lower limit of the bus
		high = atoi(buffer);		
	}
	else
	{
		outputs = line.substr ( line.find("t ") + 2 , line.find(";") - line.find("t ") - 2 );
		low = 0;					// Single bit
		high = 0;
	}

	while(!outputs.empty())
	{	
		if (outputs.find(", ") != outputs.npos)		// Check for multiple outputs declared on the line
		{
			curr_output = outputs.substr(0, outputs.find(", "));
			outputs = outputs.substr( outputs.find(", ") + 2 , outputs.length() + 1 );
		}
		else
		{
			curr_output = outputs;
			outputs = "";
		}

		while(curr_output[0] == ' ')
		{
			curr_output.erase(0, 1);		// Remove blank spaces preceding output
		}
	
		output_size[curr_output] = high - low + 1;

		if (low != high)				// Output is multiple bits
		{
			if (high < low)
			{
				int t;
				t = low;
				low = high;
				high = t;
				g_endian = 1;
			}
			else
			{
				g_endian = 0;
			}
				
				
				
		//	cout << curr_output << ", high: " << high << " low: " << low << endl;

			strcpy(output_name, curr_output.c_str());
			strcat(output_name, "[%d]");

			for (int i = low; i <= high; i++)
			{	
				sprintf(buffer, output_name, i);
				curr_output = buffer;
                //std::cout << curr_output << std::endl;
				node_ready[ nodes_name[curr_output] ] = true;	// Set node to be ready for calculation
				//output_list.push_back(nodes_name[curr_output]);
				output_list.insert(nodes_name[curr_output]);
			}
		}
		else						// Single bit
		{
			node_ready[ nodes_name[curr_output] ] = true;		// Node ready for calculation
			//output_list.push_back(nodes_name[curr_output]);
			output_list.insert(nodes_name[curr_output]);
		}
	}
}

void CADutil::parse_constant_output(string line)
{
	string node_str, value_str;
	int low, high;
	char buffer[256], input_name[256];

	node_str = line.substr ( line.find("n ") + 2 , line.find(" = ") - line.find("n ") - 2 );
    //std::cout << "node_str: " << node_str << std::endl;
	value_str = line.substr ( line.find(" = ") + 3, line.find(";") - line.find(" = ") - 3 );
    //std::cout << "value_str: " << value_str << std::endl;

	if (value_str == "1'b1")
	{
		constant_output[node_str] = 1;
	}
	else
	{
		constant_output[node_str] = 0;
	}

    // Prabal
    if(input_list.find(nodes_name[value_str]) != input_list.end()) {
      //std::cout << "setting " << node_str << " as ready ..." << std::endl;
      node_ready[ nodes_name[node_str] ] = true;
    }
}

void CADutil::report_current_logic_states()
{
    map<string, int>::iterator it;
    for (it = nodes_name.begin(); it != nodes_name.end(); it++)		
    {
       // cout << "(" << it->first << "," << it->second << ") = " << get_logic_state(it->second) << endl;
    }
}

void CADutil::report_current_logic_states(ofstream &outFile)
{
    map<string, int>::iterator it;
    for (it = nodes_name.begin(); it != nodes_name.end(); it++)		
    {
        outFile << "(" << it->first << "," << it->second << ") = " << get_logic_state(it->second) << endl;
    }
}

void CADutil::initialize_logic_state()
{
	map<string, int>::iterator it;

	for (it = nodes_name.begin(); it != nodes_name.end(); it++)
	{
		if (index_to_node[it->second] == "1'b1")
		{
			STATE[it->second] = 1;
		}
		else if (constant_output.find(it->first) != constant_output.end())
		{
            //std::cout << "constant output: " << it->first << ", " << constant_output[it->first] << std::endl;
			STATE[it->second] = constant_output[it->first];
		}
		else
		{
			STATE[it->second] = 0;
		}
	}	    
}

void CADutil::parse_delay_file(){					//Parsing the delay file to get the mean and sd delay values for various gates. 
	char buffer[2048];
	
	ifstream f(dl);
	string s;

	int type;
	s = "INV";							//Inverter
	istreambuf_iterator<char> eof;
	if((eof == search(istreambuf_iterator<char>(f), eof, s.begin(), s.end())) ){}
	else{
		f.getline(buffer, 2048);
		string line = buffer;
		
		type = CADutil::get_type(line);

		line.erase(line.find('\t'), 1);
		basic_string <char> line2 = line.substr ( line.find("0.") , line.find('\t'));
		char* pch = (char*)malloc( sizeof( char ) *(line2.length() +1) );
                strcpy( pch, line2.c_str() );
		CADutil::GATE_DELAY_MEAN[type] = std::atof(pch);
		line.erase( line.find("0.") , line.find('\t'));
		basic_string <char> line3 = line.substr ( line.find("0.") , line.find("ETX"));
		char* pcha = (char*)malloc( sizeof( char ) *(line3.length() +1) );
                strcpy( pcha, line3.c_str() );
		CADutil::GATE_DELAY_SD[type] = std::atof(pcha);
		
		
	}
	s = "NAND";						//Nand gate
	if((eof == search(istreambuf_iterator<char>(f), eof, s.begin(), s.end())) ) {}
	else{	
		f.getline(buffer, 2048);
                string line = buffer;

		type = CADutil::get_type(line);

		line.erase(line.find('\t'), 1);
                basic_string <char> line2 = line.substr ( line.find("0.") , line.find('\t'));
                char* pch = (char*)malloc( sizeof( char ) *(line2.length() +1) );
                strcpy( pch, line2.c_str() );
                CADutil::GATE_DELAY_MEAN[type] = std::atof(pch);
                line.erase( line.find("0.") , line.find('\t'));
                basic_string <char> line3 = line.substr ( line.find("0.") , line.find("ETX"));
                char* pcha = (char*)malloc( sizeof( char ) *(line3.length() +1) );
                strcpy( pcha, line3.c_str() );
                CADutil::GATE_DELAY_SD[type] = std::atof(pcha);


	}
	s = "NOR";						//Nor Gate
	if((eof == search(istreambuf_iterator<char>(f), eof, s.begin(), s.end())) ){}
	else{
		f.getline(buffer, 2048);
                string line = buffer;

		type = CADutil::get_type(line);

		line.erase(line.find('\t'), 1);
                basic_string <char> line2 = line.substr ( line.find("0.") , line.find('\t'));
                char* pch = (char*)malloc( sizeof( char ) *(line2.length() +1) );
                strcpy( pch, line2.c_str() );
                CADutil::GATE_DELAY_MEAN[type] = std::atof(pch);
                line.erase( line.find("0.") , line.find('\t'));
                basic_string <char> line3 = line.substr ( line.find("0.") , line.find("ETX"));
                char* pcha = (char*)malloc( sizeof( char ) *(line3.length() +1) );
                strcpy( pcha, line3.c_str() );
                CADutil::GATE_DELAY_SD[type] = std::atof(pcha); 

	}

	f.close();
}

void CADutil::set_input(string input_name, mpz_class value)
{
	char name_buf[256], print_buf[256];
	mpz_class mask = 1;
	mpz_t mask_buffer;
	mpz_init(mask_buffer);
	int bit, bitA;
	string single_input;

	if (input_size.find(input_name) != input_size.end())	// Ensure that the input is present
	{
		if (input_size[input_name] == 1)				// Single bit input
		{
			set_logic_state(nodes_name[input_name], (value != 0)? 1: 0);
		}
		else								// Multiple bit input
		{
			strcpy(name_buf, (input_name + '\0').c_str());
			strcat(name_buf, "[%d]");
			
			if( g_endian == 0)  // Set each bit in the input to the value specified by "value"
			{	
				//cout << "entering here!"<< endl;
				for (int i = 0; i < input_size[input_name]; i++)	
				{
				sprintf(print_buf, name_buf, i);
				single_input = print_buf;
				mpz_and(mask_buffer, value.get_mpz_t(), mask.get_mpz_t());	// Mask value to obtain single bit
				bit = (mpz_cmp_ui(mask_buffer, 0))? 1: 0;		// Extract bit if result of and operation is nonzero
				set_logic_state(nodes_name[single_input], bit);
				mask <<= 1;
				}
			}
			else if(g_endian == 1)
			{
				for (int i = (static_cast<int>(input_size[input_name]) - 1); i >= 0; --i)	
				{
					//cout << "Entering here" << endl;
					
					sprintf(print_buf, name_buf, i);
					single_input = print_buf;
					mpz_and(mask_buffer, value.get_mpz_t(), mask.get_mpz_t());	// Mask value to obtain single bit
					bit = (mpz_cmp_ui(mask_buffer, 0))? 1: 0;		// Extract bit if result of and operation is nonzero
					set_logic_state(nodes_name[single_input], bit);
					mask <<= 1;
				}
			}

		}
	}
	else
	{
	//	cout << endl << " Invalid input, please try again." << endl << endl;
	//	cout <<" the input name " << input_name << endl;
	}		
}

void CADutil::print_outputs(ofstream &outFile)
{
	char name_buf[256], print_buf[256], output_value[256];
	string output_name, single_output;
	mpz_class mask = 1;
	mpz_class output = 0;
	mpz_class mask_buffer = 0;
	
//	cout << endl;
						// Loop through all outputs
	for (map<string, int>::iterator i = output_size.begin(); i != output_size.end(); i++)
	{
		output = 0;
		mask = 1;
		output_name = i->first;
//		cout << " (" << i->second << ")\t" << output_name << " = ";
///		outFile << " (" << i->second << ")\t" << output_name << " = ";
		outFile << output_name << " = ";

		if (i->second == 1)		// Output is single bit
		{
//			cout << get_logic_state(nodes_name[output_name]) << endl;
			outFile << get_logic_state(nodes_name[output_name]) << endl;
		}
		else				// Output is multiple bits
		{
			strcpy(name_buf, (output_name + '\0').c_str());
			strcat(name_buf, "[%d]");
			
			if( g_endian == 0)
			{
			for (int j = 0; j < i->second; j++)				// Loop up to the size of the output and get each bit
			{								// store weighted sum in "output"
				sprintf(print_buf, name_buf, j);
				single_output = print_buf;
				output += mask * get_logic_state(nodes_name[single_output]);
				mask <<= 1;
			}
			}
			else if(g_endian == 1)
			{
			 for (int j = (static_cast<int>(i->second) - 1); j >= 0; --j) // Loop down the size of the output and get each bit
			      {								// store weighted sum in "output"
				sprintf(print_buf, name_buf, j);
				single_output = print_buf;
				output += mask * get_logic_state(nodes_name[single_output]);
				mask <<= 1;
			      }
			}

		

//			cout << output << endl;
			outFile << output << endl;
		}
			
	}

//	cout << endl;
	outFile << endl;
}

void CADutil::print_inputs()
{
	char name_buf[256], print_buf[256];
	string input_name, single_input;
	unsigned long long mask;
	unsigned long long input;

//	cout << endl;
						// Loop through all outputs
	for (map<string, int>::iterator i = input_size.begin(); i != input_size.end(); i++)
	{
		input = 0;
		mask = 1;
		input_name = i->first;
//		cout << " (" << i->second << ")\t" << input_name << " = ";
		
		if (i->second == 1)		// Output is single bit
		{
//			cout << get_logic_state(nodes_name[input_name]) << endl;			
		}
		else				// Output is multiple bits
		{
			strcpy(name_buf, (input_name + '\0').c_str());
			strcat(name_buf, "[%d]");

			for (int j = 0; j < i->second; j++)		// Loop up to the size of the output and get each bit
			{							// store weighted sum in "output"
				sprintf(print_buf, name_buf, j);
				single_input = print_buf;
				input += (mask) * (get_logic_state(nodes_name[single_input]));
				mask <<= 1;
			}

//			cout << input << endl;
		}
			
	}

//	cout << endl;
}

void CADutil::print_outputs()
{
	char name_buf[256], print_buf[256], output_value[256];
	string output_name, single_output;
	mpz_class mask = 1;
	mpz_class output = 0;
	mpz_class mask_buffer = 0;
	
//	cout << endl;
						// Loop through all outputs
	for (map<string, int>::iterator i = output_size.begin(); i != output_size.end(); i++)
	{
		output = 0;
		mask = 1;
		output_name = i->first;
//		cout << " (" << i->second << ")\t" << output_name << " = ";

		if (i->second == 1)		// Output is single bit
		{
//			cout << get_logic_state(nodes_name[output_name]) << endl;
		}
		else				// Output is multiple bits
		{
			strcpy(name_buf, (output_name + '\0').c_str());
			strcat(name_buf, "[%d]");

			for (int j = 0; j < i->second; j++)				// Loop up to the size of the output and get each bit
			{								// store weighted sum in "output"
				sprintf(print_buf, name_buf, j);
				single_output = print_buf;
				output += mask * get_logic_state(nodes_name[single_output]);
				mask <<= 1;
			}

//			cout << output << endl;
		}
			
	}

//	cout << endl;
}

void CADutil::print_inputs(ofstream &outFile)
{
	char name_buf[256], print_buf[256];
	string input_name, single_input;
	unsigned long long mask;
	unsigned long long input;

//	cout << endl;
						// Loop through all outputs
	for (map<string, int>::iterator i = input_size.begin(); i != input_size.end(); i++)
	{
		input = 0;
		mask = 1;
		input_name = i->first;
//		cout << " (" << i->second << ")\t" << input_name << " = ";
		outFile << " (" << i->second << ")\t" << input_name << " = ";
		
		if (i->second == 1)		// Output is single bit
		{
//			cout << get_logic_state(nodes_name[input_name]) << endl;
			outFile << get_logic_state(nodes_name[input_name]) << endl;			
		}
		else				// Output is multiple bits
		{
			strcpy(name_buf, (input_name + '\0').c_str());
			strcat(name_buf, "[%d]");

			for (int j = 0; j < i->second; j++)		// Loop up to the size of the output and get each bit
			{							// store weighted sum in "output"
				sprintf(print_buf, name_buf, j);
				single_input = print_buf;
				input += (mask) * (get_logic_state(nodes_name[single_input]));
				mask <<= 1;
			}

//			cout << input << endl;
			outFile << input << endl;
		}
			
	}

	cout << endl;
	outFile << endl;
}

/*****************************************************
/*	This function recursively calculates the logic 
/*	depth of a circuit and prints the result to 
/*	the screen.  It iterates through the outputs,
/*	passing them to the find_depth() function
/*	which is explained below.
/****************************************************/
void CADutil::report_logic_depth()
{
	int logic_depth;
	int count = 0;
	int curr_count;

	for (set<int>::iterator it = output_list.begin(); it!= output_list.end(); it++)
	{
		curr_count = find_depth(*it);
		sleep (1);
	
		if (curr_count > count)		count = curr_count;
	}

//	cout << count << endl;
}

/*****************************************************
/*	This function recursively finds the logic
/*	depth of a particular output. The first
/*	instance of this function is called in the 
/*	report_logic_depth() function, which passes
/*	an output.  This function calls itself on each
/*	node attached to the current node, keeping
/*	track of its depth.
/****************************************************/
int CADutil::find_depth(int curr_node)
{
	int c1, c2;
	multimap<int,int>::iterator edge_from_curr_node = edges_from.equal_range(curr_node).first;
	int count = 1;

	if (edges_from.count(curr_node) == 1)		// Inverter, only has one input
	{
		count += find_depth(edge_from_curr_node->second);	// Follow input and keep track of lower paths
	}
	else if (edges_from.count(curr_node) == 2)	// NAND or NOR, has two inputs
	{
		c1 = find_depth(edge_from_curr_node->second);		// Follow first input
		edge_from_curr_node++;
		c2 = find_depth(edge_from_curr_node->second);		// Follow second input

		count += (c1 > c2)? c1: c2;				// Choose the deeper path
	}
	else
	{
		count = 0;		// Node is an input, depth at this node is zero.
	}	

	return count;			// return the depth of the current node
}






/*
// Specialized to sparc alu
void CADutil::report_inputs()
{
    char buffer[256];
    cout << "rs1 = ";
    for (int i = 63; i >= 0; i--)
    {
        sprintf(buffer, "byp_alu_rs1_data_e[%d]", i);
        string m_str(buffer);
        assert(nodes_name.find(m_str) != nodes_name.end());
        int node = nodes_name[m_str];
        cout << get_logic_state(node);
    }
    cout << endl;
    
    cout << "rs2 = ";
    for (int i = 63; i >= 0; i--)
    {
        sprintf(buffer, "byp_alu_rs2_data_e_l[%d]", i);
        string m_str(buffer);
        assert(nodes_name.find(m_str) != nodes_name.end());
        int node = nodes_name[m_str];
        cout << get_logic_state(node);
    }
    cout << endl;
    // vector<string> logic_inputs;
    // logic_inputs.insert(logic_inputs.begin(), string("ecl_alu_cin_e"));
    // logic_inputs.insert(logic_inputs.begin(), string("ifu_exu_invert_d"));
    // for (unsigned int i = 0; i < logic_inputs.size(); i++)
    // {
    //     assert(nodes_name.find(logic_inputs[i]) != nodes_name.end());
    //     cout << logic_inputs[i] << "   " << get_logic_state(nodes_name[logic_inputs[i]]) << endl;
    // }
}

// Specialized to sparc alu
void CADutil::report_adder_out()
{
    char buffer[256];
    cout << "out = ";
    for (int i = 63; i >= 0; i--)
    {
        sprintf(buffer, "adder_out[%d]", i);
        string m_str(buffer);
        int node = nodes_name[m_str];
        cout << get_logic_state(node);
    }
    cout << endl;
    
    cout << "spr = ";
    for (int i = 63; i >= 0; i--)
    {
        sprintf(buffer, "spr_out[%d]", i);
        string m_str(buffer);
        int node = nodes_name[m_str];
        cout << get_logic_state(node);
    }
    cout << endl;
}



/***********************************/
//	Obselete Functions
/***********************************/


/*
// Specialized to sparc alu
void CADutil::create_input_set(string inp1, string inp2, bool sub)
{
    long long input1 = atoll(inp1.c_str());
    long long input2 = atoll(inp2.c_str());
    unsigned long long mask = 1;
    // rs1
    //cout << " RS1 " << input1 << " RS2 " << input2 << "  sub=" << sub << endl;
    char buff[2048];
    unsigned long long b = 1;
    for (unsigned long long i = 0; i < 64; i++)
    {
        
        mask = b << i;
        
        sprintf(buff, "byp_alu_rs1_data_e[%llu]", i);
        string name(buff);
        // if (!neg_inp) {
        //     set_logic_state(nodes_name[name], (input1 & mask) ? 1 : 0);
        // } else {
        set_logic_state(nodes_name[name], (input1 & mask) ? 1 : 0);
        sprintf(buff, "byp_alu_rs2_data_e_l[%llu]", i);
        string name2(buff);
        set_logic_state(nodes_name[name2], (input2 & mask) ? 1 : 0);
        
        
    }

    string prim = "ecl_alu_cin_e";
    set_logic_state(nodes_name[prim], sub);
    set_logic_state(nodes_name["ifu_exu_invert_d"], !sub);
    set_logic_state(nodes_name["ecl_alu_log_sel_and_e"], 0);
    set_logic_state(nodes_name["ecl_alu_log_sel_or_e"], 0);
    set_logic_state(nodes_name["ecl_alu_log_sel_xor_e"], 0);
    set_logic_state(nodes_name["ecl_alu_log_sel_move_e"], 0);
    set_logic_state(nodes_name["ecl_alu_sethi_inst_e"], 0);
    
    
}


void CADutil::parse_input_file(char * fname)
{

	map<string, int>::iterator it;
	//for (it = nodes_name.begin(); it != nodes_name.end(); it++)
	//    cout << it->first << endl;

	cout << "Parsing Input File " << fname << endl;


	ifstream f(fname);
	istreambuf_iterator<char> eof;
	char buffer[2048];
	while(!f.eof()){	
	f.getline(buffer, 2048);
	string m_line(buffer);
	if (m_line.length() == 0)
		continue;
	vector<string> vals;
	boost::split(vals, m_line, boost::is_any_of("\t "));
	assert(vals.size() > 1);			
	if (nodes_name.find(vals[0]) == nodes_name.end()) {
		cout << "Invalid input node " << vals[0] << "& w/ input line " << m_line << "  length = " << m_line.length() << endl;
		exit(0);
	}
	int logic_val = atoi(vals[1].c_str());		
	int indx = nodes_name[vals[0]];	
	set_logic_state(indx, logic_val);
	//cout << m_line << " Setting (" << vals[0] << "," << indx << ")  to " << logic_val << "  VERIFICATION=" << get_logic_state(indx) << endl;
        
	}   
}

*/
