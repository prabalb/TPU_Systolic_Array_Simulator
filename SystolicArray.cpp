/*
 * File: SystolicArray.cpp
 * Author: Prabal Basu
 * Contact: prabalb@aggiemail.usu.edu
 */

#include "./SystolicArray.hpp"
#include "./pstream.h"

#include <fstream>
#include <sstream>
#include <cassert>
#include <cmath>
#include <bitset>
#include <limits>
#include <stdlib.h>
#include <string.h>
#include <bitset>
#include <sys/time.h>

template <typename T> std::string to_string(const T& n)
{
  std::ostringstream stm ;
  stm << n ;
  return stm.str() ;
}

bool Utils::populateDBs(unsigned int dimension,
                        const std::string biasFile,
                        const std::string weightFile,
                        const std::string activationFile) {
  dim = dimension;
  if(dim <= 0) {
    std::cout << "The dimension of the systolic array should be a positive integer." << std::endl;
    return false;
  }

  std::ifstream aFile, bFile, wFile;
  aFile.open(activationFile.c_str());
  bFile.open(biasFile.c_str());
  wFile.open(weightFile.c_str());

  if(!aFile.is_open() || !bFile.is_open() || !wFile.is_open()) {
    std::cout << "Could not open one of the input files specified, please try again ..." << std::endl;
    return false;
  }

  activation = new double*[dim];
  for(unsigned int index = 0; index < dim; index++) {
    activation[index] = new double[dim];
  }

  bias = new double*[dim];
  for(unsigned int index = 0; index < dim; index++) {
    bias[index] = new double[dim];
  }

  weight = new double*[dim];
  for(unsigned int index = 0; index < dim; index++) {
    weight[index] = new double[dim];
  }

  unsigned int index_1 = 0;
  while(!aFile.eof()) {
    assert(index_1 <= dim);
    std::string line;
    if(!std::getline(aFile, line)) continue;
    std::stringstream ss(line);
    std::string tmpStr;
    unsigned int index_2 = 0;
    while(std::getline(ss, tmpStr, ' ')) {
      assert(index_2 <= dim);
      activation[index_1][index_2++] = std::stod(tmpStr);
    }
    index_1++;
  }

  index_1 = 0;
  while(!bFile.eof()) {
    assert(index_1 <= dim);
    std::string line;
    if(!std::getline(bFile, line)) continue;
    std::stringstream ss(line);
    std::string tmpStr;
    unsigned int index_2 = 0;
    while(std::getline(ss, tmpStr, ' ')) {
      assert(index_2 <= dim);
      bias[index_1][index_2++] = std::stod(tmpStr);
    }
    index_1++;
  }

  index_1 = 0;
  while(!wFile.eof()) {
    assert(index_1 <= dim);
    std::string line;
    if(!std::getline(wFile, line)) continue;
    std::stringstream ss(line);
    std::string tmpStr;
    unsigned int index_2 = 0;
    while(std::getline(ss, tmpStr, ' ')) {
      assert(index_2 <= dim);
      weight[index_1][index_2++] = std::stod(tmpStr);
    }
    index_1++;
  }

  aFile.close();
  bFile.close();
  wFile.close();

#ifdef GUIDED_SIM
  std::ifstream seqFile;
  seqFile.open("./high_delay_seqs_curtailed.txt");
  if(!seqFile.is_open()) {
    std::cout << "Could not open the high delay sequence file!" << std::endl;
    return false;
  }
  while(!seqFile.eof()) {
    assert(index_1 <= dim);
    std::string line;
    if(!std::getline(seqFile, line)) continue;
    std::stringstream ss(line);
    std::string tmpStr;
    double weight = 0.0;
    double curr_activ = 0.0;
    double prev_activ = 0.0;
    int count = 0;
    while(std::getline(ss, tmpStr, ' ')) {
      if(count == 0) {
        weight = std::stod(tmpStr);
      } else if(count == 1) {
        curr_activ = std::stod(tmpStr);
      } else if(count == 2) {
        prev_activ = std::stod(tmpStr);
      }
      count++;
    }
    high_delay_seq_db.insert(std::make_tuple(weight, curr_activ, prev_activ));
  }
#endif

#ifdef GREEN_TPU
#ifdef USE_ADVANCED_ELUT
  elu.resize(dim);
  unsigned tab_size = LRU_ERROR_LOOK_UP_TAB_SIZE;
#ifdef GREEN_TPU_LITE
  tab_size = 1;
#endif
  for(size_t i = 0; i < elu.size(); i++) {
    elu[i] = new GreenTPULRUErrorLookUpTable(tab_size);
  }
#else
#ifdef CREATE_ELUT_PER_ROW
  elu.resize(dim);
  for(size_t i = 0; i < elu.size(); i++) {
    elu[i] = new LRUErrorLookUpTable(LRU_ERROR_LOOK_UP_TAB_SIZE);
  }
#else
  elu.resize(1);
  elu[0] = new LRUErrorLookUpTable(LRU_ERROR_LOOK_UP_TAB_SIZE * LRU_ERROR_LOOK_UP_TAB_SIZE);
#endif
#endif
#endif

  return true;
}

unsigned int Utils::getDimension() {
  return dim;
}

unsigned int Utils::getNumCycleSimulated() {
  return num_cycle_simulated;
}

double Utils::getBias(unsigned int i, unsigned int j) {
  assert(i >= 0 && i < dim && j >= 0 && j < dim);
  return bias[i][j];
}

double Utils::getWeight(unsigned int i, unsigned int j) {
  assert(i >= 0 && i < dim && j >= 0 && j < dim);
  return weight[i][j];
}

double Utils::getActivation(unsigned int row, unsigned int num_active_cycle) {
  assert(row >= 0 && row < dim);
  return activation[num_active_cycle][row];
}

void Utils::getStaToolCmdLine(std::string& sta_tool_cmd_line, MACUnit *mac) {
  std::string sta_tool_exec("./sta_tool/sta_tool ");
  std::string mac_netlist("./sta_tool/mac_netlist.v ");
  std::string rand_gate("./sta_tool/random.txt");

  std::string oper_voltage("0.45 ");
  std::string sta_tool_mode("1 ");

  int random_gate_row_num = rand() % 3 + 1;
  std::string random_gate_row = to_string(random_gate_row_num) + std::string(" ");
  std::string consider_rand_gate_sel("y ");

  unsigned int row, col;
  mac->getRowCol(row, col);

  unsigned int num_active_cycle = mac->getNumActiveCycle();
  unsigned int num_bypassed_cycle = mac->getNumBypassedCycle();
  int curr_activ = getActivation(row, num_active_cycle + num_bypassed_cycle);
  std::string curr_activ_str = to_string(curr_activ) + std::string(" ");
  int prev_activ = (num_active_cycle + num_bypassed_cycle) == 0 ? 0: getActivation(row, num_active_cycle + num_bypassed_cycle - 1);
  std::string prev_activ_str = to_string(prev_activ) + std::string(" ");

  int weight = getWeight(row, col);
  std::string weight_str = to_string(weight) + std::string(" ");

  long int prev_mac_out = mac->getPrevMacOut();
  std::string prev_mac_out_str = to_string(prev_mac_out) + std::string(" ");
  long int prev_mac_out_in_last_active_cycle = mac->getPrevMacOutInLastActiveCycle();
  std::string prev_mac_out_in_last_active_cycle_str = to_string(prev_mac_out_in_last_active_cycle) + std::string(" ");

  sta_tool_cmd_line = sta_tool_exec + mac_netlist + oper_voltage + sta_tool_mode
                      + random_gate_row + consider_rand_gate_sel + prev_activ_str + weight_str
                      + prev_mac_out_in_last_active_cycle_str + curr_activ_str + weight_str
                      + prev_mac_out_str + rand_gate;
}

void Utils::updateErrorLUT(unsigned row, unsigned col) {
#ifndef GREEN_TPU
  return;
#endif
#ifndef GREEN_TPU_LITE
  return;
#endif
  if(col < dim - 1) {
    return;
  }
  elu[row]->clearPatterns();
}

bool Utils::willEncounterTimingError(MACUnit *mac) {
  bool willEncounterTimingError = false;

#ifndef DETAILED_SIM
  return willEncounterTimingError;
#endif

  unsigned int row, col;
  mac->getRowCol(row, col);

#ifdef GUIDED_SIM
  double weight = getWeight(row, col);
  if(weight == 0.0) {
    updateErrorLUT(row, col);
    return willEncounterTimingError;
  }

  unsigned int num_active_cycle = mac->getNumActiveCycle();
  unsigned int num_bypassed_cycle = mac->getNumBypassedCycle();
  double curr_activ = getActivation(row, num_active_cycle + num_bypassed_cycle);
  double prev_activ = (num_active_cycle + num_bypassed_cycle) == 0 ? 0 : getActivation(row, num_active_cycle + num_bypassed_cycle - 1);
  if(curr_activ == prev_activ) {
    updateErrorLUT(row, col);
    return willEncounterTimingError;
  }

  if(high_delay_seq_db.find(std::make_tuple(weight, curr_activ, prev_activ)) == high_delay_seq_db.end()) {
    updateErrorLUT(row, col);
    return willEncounterTimingError;
  }

#ifdef SKIP_STA_TOOL
  // If the activation sequence is found in the high delay database,
  // assume that sequence to cause a timing error. This is a reasonable
  // assumption that will improve the simulator run-time.
  willEncounterTimingError = !canGreenTPUAvoidTimingError(mac);
  updateErrorLUT(row, col);
  return willEncounterTimingError;
#endif

#endif

  /*
  num_sta_tool_invoked++;
  std::string sta_tool_cmd_line;
  getStaToolCmdLine(sta_tool_cmd_line, mac);
  assert(sta_tool_cmd_line.size() > 0);
  redi::ipstream proc(sta_tool_cmd_line.c_str(), redi::pstreams::pstdout | redi::pstreams::pstderr);
  std::string line;
  double delay = 0.0;
  while (std::getline(proc.out(), line)) {
    delay = std::stod(line);
    //std::cout << delay << std::endl;
  }
  if(delay > GLOBAL_CLOCK_PERIOD) {
    willEncounterTimingError = !canGreenTPUAvoidTimingError(mac);
  }
  updateErrorLUT(row, col);
  return willEncounterTimingError;
  */
}

bool Utils::canGreenTPUAvoidTimingError(MACUnit *mac) {
  bool retVal = false;

#ifndef GREEN_TPU
  return retVal;
#endif

  unsigned int row;
  unsigned int col;
  mac->getRowCol(row, col);
#ifdef USE_ADVANCED_ELUT
  retVal = elu[row]->referAndUpdateLUT(mac);
#else
#ifndef CREATE_ELUT_PER_ROW
  row = 0;
#endif
  retVal = elu[row]->referAndUpdateLUT(mac);
#endif
  if(retVal) {
    // unsigned int row, col;
    // mac->getRowCol(row, col);
    // std::cout << "GreenTPU invoked for (" << row << ", " << col << ")"  << std::endl;
    updateBoostStat(mac);
  }
  return retVal;
}

void Utils::updateBoostStat(MACUnit *mac) {
  unsigned int row, col;
  mac->getRowCol(row, col);
  SystolicArray& sa = SystolicArray::getSystolicArray();
  SystArr& sysarr = sa.getArray();
  for(unsigned j = 0; j < dim; j++) {
    sysarr[row][j]->logBoostCount(num_cycle_simulated);
  }
}

/* A MAC with coordinates (i,j) 'works' in the following time range: 
 * [i+j+1, i+j+dim]. Timing error is handled using TE-Drop. TE-Drop 
 * does not introduce any performance overhead, but, compromise on 
 * the inference accuracy by skipping the MAC operation that is 
 * immediately downstream to the MAC that encountered the timing error.
 */
bool Utils::operateInCurrentClockCycle(unsigned int row,
                                       unsigned int col,
                                       bool& pass_prev_mac_out) {

  unsigned int lower_lim = row + col + 1;
  unsigned int upper_lim = row + col + dim;
  if(num_cycle_simulated >= lower_lim && num_cycle_simulated <= upper_lim) {
    SystolicArray& sa = SystolicArray::getSystolicArray();
    SystArr& sysarr = sa.getArray();
    if(row > 0) {
      pass_prev_mac_out = sysarr[row][col]->bypassOperation();
    }
    return pass_prev_mac_out == false;
  }
  return false;
}

unsigned int Utils::getNumBatches() {
  // TODO: Implement based on bias/weight/activation input matrix size
  return 1;
}

double Utils::get_wall_time()
{
  struct timeval time;
  if (gettimeofday(&time,NULL)) {
    // Handle error
    return 0;
  }
  return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

double Utils::get_cpu_time()
{
  return (double)clock() / CLOCKS_PER_SEC;
}

/* Total number of clock cycles to simulate one layer is 2N+B-2.
 * We consider B = N. So, total simulation cycles is 3N-2, for 
 * one batch/layer of computation.
 */
bool Utils::simulateSystolicArray(const std::string outputFile) {
  std::ofstream oFile;
  oFile.open(outputFile.c_str());
  if(!oFile.is_open()) {
    return false;
  }

  double **output = new double*[dim];
  for(unsigned int index = 0; index < dim; index++) {
    output[index] = new double[dim];
  }

  SystolicArray& sa = SystolicArray::getSystolicArray();
  SystArr& sysarr = sa.getArray();
  unsigned int num_batches = Utils::getNumBatches();
  unsigned int batch_counter = 1;
  unsigned int num_cycles_per_batch = 3 * dim - 2;
  while(batch_counter <= num_batches) {
    // std::cout << "=== Processing Batch " << batch_counter << " ===" << std::endl;
    while(num_cycle_simulated <= num_cycles_per_batch) {
      for(size_t i = 0; i < dim; i++) {
        for(size_t j = 0; j < dim; j++) {
          bool pass_prev_mac_out = false;
          if(!operateInCurrentClockCycle(i, j, pass_prev_mac_out)) {
            if(pass_prev_mac_out) {
              sysarr[i][j]->doMAC(pass_prev_mac_out);
            }
            continue;
          }
          sysarr[i][j]->doMAC();
          if(i == dim - 1) {
            unsigned int row_index = sysarr[i][j]->getNumActiveCycle() + 
                                     sysarr[i][j]->getNumBypassedCycle() - 1;
            output[row_index][j] = sysarr[i][j]->getOutput();
          }
        }
      }
      num_cycle_simulated++;
    }
    batch_counter++;
  }
  for(size_t index_1 = 0; index_1 < dim; index_1++) {
    for(size_t index_2 = 0; index_2 < dim; index_2++) {
      oFile << output[index_1][index_2] << " ";    
    }
    oFile << "\n";
  }
  oFile.close();
  return true;
}

void Utils::logTEDrop() {
  num_te_drop++;
}

bool Utils::updateLUTFromPreviousRun(const std::string errorLookUpTableFile) {
  errorLUTFile = errorLookUpTableFile;
  std::ifstream eFile;
  eFile.open(errorLookUpTableFile.c_str());
  if(!eFile.is_open()) {
    std::cout << "Could not open the error look-up table file!" << std::endl;
    return false;
  }
  while(!eFile.eof()) {
    std::string line;
    if(!std::getline(eFile, line)) continue;
    std::stringstream ss(line);
    std::string tmpStr;
    std::vector<double> activations;
    unsigned counter = 0;
    int row = 0;
#ifdef USE_ADVANCED_ELUT
    while(std::getline(ss, tmpStr, ' ')) {
      if(counter == 0) row = std::stoi(tmpStr);
      if(counter > 0)  activations.push_back(std::stod(tmpStr));
      counter++;
    }
#else
    int col = 0;
    while(std::getline(ss, tmpStr, ' ')) {
      if(counter == 0) row = std::stoi(tmpStr);
      if(counter == 1) col = std::stoi(tmpStr);
      if(counter > 1)  activations.push_back(std::stod(tmpStr));
      counter++;
    }
    SystolicArray& sa = SystolicArray::getSystolicArray();
    SystArr& sysarr = sa.getArray();
    MACUnit *mac = sysarr[row][col];
    assert(mac);
#endif
    assert(activations.size() > 0 && activations.size() % 2 == 0);
    std::set<std::pair<double, double> > patterns;
    for(size_t i = 0; i < activations.size();) {
      double curr_activ = activations[i++];
      double prev_activ = activations[i++];
      patterns.insert(std::make_pair(curr_activ, prev_activ));
    }
#ifdef USE_ADVANCED_ELUT
    elu[row]->updateLUTFromPreviousRun(patterns);
#else
#ifdef CREATE_ELUT_PER_ROW
    elu[row]->updateLUTFromPreviousRun(mac, patterns);
#else
    elu[0]->updateLUTFromPreviousRun(mac, patterns);
#endif
#endif
  }
  eFile.close();
  return true;
}

#ifdef USE_ADVANCED_ELUT
void Utils::GreenTPULRUErrorLookUpTable::updateLUTFromPreviousRun(std::set<std::pair<double, double> >& patterns) {
  assert(patterns.size() <= m_lut_size);
  m_patterns = patterns;
}

bool Utils::GreenTPULRUErrorLookUpTable::performAdvancedLookUp(double curr_active, 
                                                               double prev_active) {
#ifndef PERFORM_ADVANCED_LOOK_UP
  return false;
#endif

#ifdef GREEN_TPU_LITE
  return false;
#endif

  std::set<std::pair<double, double> >::const_iterator sIter = m_patterns.begin();
  for(; sIter != m_patterns.end(); sIter++) {
    double lut_curr_active = (*sIter).first;
    double lut_prev_active = (*sIter).second;
    int lut_pattern_xor = int(lut_curr_active) ^ int(lut_prev_active);
    int incoming_pattern_xor = int(curr_active) ^ int(prev_active);
    unsigned num_reset_bits_1 = 8 - Utils::getUtils().countNumSetBits((lut_pattern_xor | incoming_pattern_xor));
    unsigned num_reset_bits_2 = 8 - Utils::getUtils().countNumSetBits(lut_pattern_xor);
    if(num_reset_bits_1 >= floor(ADVANCED_LOOK_UP_PARAM * 0.01 * num_reset_bits_2)) {
      return true;
    }
  }
  return false;
}

bool Utils::GreenTPULRUErrorLookUpTable::referAndUpdateLUT(MACUnit *mac) {
  unsigned int row, col;
  mac->getRowCol(row, col);

  unsigned int num_active_cycle = mac->getNumActiveCycle();
  unsigned int num_bypassed_cycle = mac->getNumBypassedCycle();

  double curr_activ = Utils::getUtils().getActivation(row, num_active_cycle + num_bypassed_cycle);
  double prev_activ = (num_active_cycle + num_bypassed_cycle) == 0 ? 0: 
                      Utils::getUtils().getActivation(row, num_active_cycle + num_bypassed_cycle - 1);

  if(m_patterns.find(std::make_pair(curr_activ, prev_activ)) == m_patterns.end()
     && !performAdvancedLookUp(curr_activ, prev_activ)) {
    if(m_patterns.size() >= m_lut_size) {
#ifdef GREEN_TPU_LITE
    return false;
#endif
      // std::cout << "will randomly evict one pattern for (" << row << ", " << col << ")" << std::endl;
      std::set<std::pair<double, double> >::iterator sIter = m_patterns.begin();
      unsigned int rand_num = m_lut_size < 2 ? 0 : rand() % (m_lut_size - 2);
      while(rand_num > 0) {
        sIter = ++sIter;
        rand_num--;
      }
      m_patterns.erase(sIter); // randomly evict one entry
    }
    m_patterns.insert(std::make_pair(curr_activ, prev_activ));
    return false;
  }

  // GreenTPU can prevent errors in the MACs with col coordinate in the range [BOOSTER_CLOCK_CYCLE, dim-1].
  return col >= BOOSTER_CLOCK_CYCLE;
}
#endif

#ifndef USE_ADVANCED_ELUT
void Utils::LRUErrorLookUpTable::updateLUTFromPreviousRun(MACUnit *mac, std::set<std::pair<double, double> >& patterns) {
  assert(m_mac_list.size() <= m_lut_size);
  m_mac_to_patterns[mac] = patterns;
  m_mac_list.push_front(mac);
  m_key_map[mac] = m_mac_list.begin();
}

bool Utils::LRUErrorLookUpTable::performAdvancedLookUp(double curr_active,
                                                       double prev_active,
                                                       std::set<std::pair<double, double> >& patterns) {
#ifndef PERFORM_ADVANCED_LOOK_UP
  return false;
#endif

  std::set<std::pair<double, double> >::const_iterator sIter = patterns.begin();
  for(; sIter != patterns.end(); sIter++) {
    double lut_curr_active = (*sIter).first;
    double lut_prev_active = (*sIter).second;
    int lut_pattern_xor = int(lut_curr_active) ^ int(lut_prev_active);
    int incoming_pattern_xor = (int)(curr_active) ^ (int)(prev_active);
    unsigned num_reset_bits_1 = 8 - Utils::getUtils().countNumSetBits((lut_pattern_xor | incoming_pattern_xor));
    unsigned num_reset_bits_1 = 8 - Utils::getUtils().countNumSetBits(lut_pattern_xor);
    if(num_reset_bits_1 >= floor(ADVANCED_LOOK_UP_PARAM * 0.01 * num_reset_bits_2)) {
      return true;
    }
  }
  return false;
}

bool Utils::LRUErrorLookUpTable::referAndUpdateLUT(MACUnit *mac) {
  unsigned int row, col;
  mac->getRowCol(row, col);

  unsigned int num_active_cycle = mac->getNumActiveCycle();
  unsigned int num_bypassed_cycle = mac->getNumBypassedCycle();

  double curr_activ = Utils::getUtils().getActivation(row, num_active_cycle + num_bypassed_cycle);
  double prev_activ = (num_active_cycle + num_bypassed_cycle) == 0 ? 0: 
                      Utils::getUtils().getActivation(row, num_active_cycle + num_bypassed_cycle - 1);

  if(m_key_map.find(mac) == m_key_map.end()) { // not present
    if(m_mac_list.size() == m_lut_size) { // table full, LRU-based eviction
      // std::cout << "Table full!" << std::endl;
      MACUnit *last_mac = m_mac_list.back();
      m_mac_list.pop_back();
      m_key_map.erase(last_mac);
      m_mac_to_patterns.erase(last_mac);
    }
    // std::cout << "Inserted new entry: (" << row << ", " << col << "), pattern: " 
    // << curr_activ << " and " << prev_activ << std::endl;
    m_mac_list.push_front(mac);
    m_key_map[mac] = m_mac_list.begin();
    m_mac_to_patterns[mac].insert(std::make_pair(curr_activ, prev_activ));
    return false;
  }

  // present
  m_mac_list.erase(m_key_map[mac]);
  m_mac_list.push_front(mac);
  m_key_map[mac] = m_mac_list.begin();
  // std::cout << "Found MAC (" << row << ", " << col << ") in the table! Current pattern: " 
  // << curr_activ << " and " << prev_activ << std::endl;
  if(m_mac_to_patterns[mac].find(std::make_pair(curr_activ, prev_activ)) == m_mac_to_patterns[mac].end()
     && !performAdvancedLookUp(curr_activ, prev_activ, m_mac_to_patterns[mac])) {
    if(m_mac_to_patterns[mac].size() > m_lut_size) {
      // std::cout << "will randomly evict one pattern for (" << row << ", " << col << ")" << std::endl;
      std::set<std::pair<double, double> >::iterator sIter = m_mac_to_patterns[mac].begin();
      unsigned int rand_num = m_lut_size < 2 ? 0 : rand() % (m_lut_size - 2);
      while(rand_num > 0) {
        sIter = ++sIter;
        rand_num--;
      }
      m_mac_to_patterns[mac].erase(sIter); // randomly evict one entry
    }
    m_mac_to_patterns[mac].insert(std::make_pair(curr_activ, prev_activ));
    return false;
  }
  // std::cout << "Found recurring entry for (" << row << ", " << col << ")" << std::endl;
  // GreenTPU can prevent errors in the MACs with col coordinate in the range [BOOSTER_CLOCK_CYCLE, dim-1].
  return col >= BOOSTER_CLOCK_CYCLE;
}
#endif

bool Utils::writeErrorLookUpTable() {
  assert(errorLUTFile.size() > 0);
  std::ofstream oFile;
  oFile.open(errorLUTFile.c_str());
  if(!oFile.is_open()) {
    std::cout << "Could not open the error look-up table file!" << std::endl;
    return false;
  }
  for(size_t i = 0; i < elu.size(); i++) {
#ifdef USE_ADVANCED_ELUT
    std::set<std::pair<double, double> > patterns;
    elu[i]->getPatterns(patterns);
    if(patterns.size() > 0) {
      oFile << i << " ";
      for(std::set<std::pair<double, double> >::const_iterator iter = patterns.begin();
          iter != patterns.end(); iter++) {
        oFile << (*iter).first << " " << (*iter).second << " ";
      }
      oFile << "\n";
    }
#else
    std::list<MACUnit*> m_mac_list;
    std::map<MACUnit*, std::set<std::pair<double, double> > > m_mac_to_patterns;
    elu[i]->getMACToPatternsDBs(m_mac_list, m_mac_to_patterns);
    for(std::list<MACUnit*>::reverse_iterator rIter = m_mac_list.rbegin();
        rIter != m_mac_list.rend(); ++rIter) {
      MACUnit *mac = *rIter;
      unsigned int row, col;
      mac->getRowCol(row, col);
      oFile << row << " " << col << " ";
      std::set<std::pair<double, double> > patterns = m_mac_to_patterns[mac];
      for(std::set<std::pair<double, double> >::const_iterator iter = patterns.begin();
          iter != patterns.end(); iter++) {
        oFile << (*iter).first << " " << (*iter).second << " ";
      }
      oFile << "\n";
    }
#endif
  }
  oFile.close();
  return true;
}

unsigned Utils::countNumSetBits(int number) {
  unsigned num_set_bits = 0;
  std::string binary = std::bitset<8>(number).to_string(); // to binary
  for(int i = 0; i < 8; i++) {
    if (binary[i] == '1') {
      num_set_bits += 1;
    }
  }
  return num_set_bits;
}

// TEP implementation: flip half of the total bits in prev_mac_out, chosen randomly
long double Utils::propagateTimingError(long double prev_mac_out) {
#ifndef TEP
  return prev_mac_out;
#endif
  std::string prev_mac_out_bin = std::bitset<OUTPUT_BIT_WIDTH>(unsigned(prev_mac_out)).to_string();
  unsigned int start_pos = rand() % (OUTPUT_BIT_WIDTH/2);
  unsigned int end_pos = start_pos + OUTPUT_BIT_WIDTH/2 - 1;
  for(unsigned int pos = start_pos; pos <= end_pos; pos++) {
    std::string bit_at_pos = std::string(1, prev_mac_out_bin[pos]);
    std::string flipped_bit =  bit_at_pos == "0" ? "1" : "0";
    prev_mac_out_bin.replace(pos, 1, flipped_bit);
  }
  return (long double)(std::bitset<OUTPUT_BIT_WIDTH>(prev_mac_out_bin).to_ulong());
}

bool Utils::reportStats(const std::string greenTPUStatsFile) {
  SystolicArray& sa = SystolicArray::getSystolicArray();
  SystArr& sysarr = sa.getArray();

  unsigned int total_num_errors = 0;
  for(unsigned i = 0; i < dim; i++) {
    for(unsigned j = 0; j < dim; j++) {
      total_num_errors += sysarr[i][j]->getNumTimingErrors();
    }
  }

  std::ofstream gFile;
  gFile.open(greenTPUStatsFile.c_str());
  if(!gFile.is_open()) {
    return false;
  }
  // gFile << "Row #" << "      " << "# Energy Factor" << "\n";
  // gFile << "=====" << "      " << "===============" << "\n";

  unsigned int energy_factor = dim / BOOST_GRANULARITY;
  assert(energy_factor % 2 == 0);
  for(unsigned i = 0; i < dim; i++) {
    gFile << i << "          " << sysarr[i][0]->getNumTimesBoosted() * energy_factor << std::endl;
  }
  gFile.close();

#ifdef GREEN_TPU
  writeErrorLookUpTable();
#endif

  std::cout << "----------- Simulation Statistics -----------" << std::endl;
  std::cout << "Number of cycle simulated: " << getNumBatches() * (3 * dim - 2) << std::endl;
  std::cout << "Total number of timing errors: " << total_num_errors << std::endl;
  std::cout << "Number of times STA tool invoked: " << num_sta_tool_invoked << std::endl;
  std::cout << "Number of erroneous ops due to TE-Drop: " << num_te_drop << std::endl;
  std::cout << "GreenTPU statistics: " << greenTPUStatsFile << std::endl;
  std::cout << "---------------------------------------------" << std::endl << std::endl;
  
  return true;
}

SystolicArray& SystolicArray::getSystolicArray() {
  static SystolicArray SA; 
  return SA;
}

SystArr& SystolicArray::getArray() {
  return SArr;
}

bool SystolicArray::createSystolicArray(const std::string errorLookUpTableFile) {
  unsigned int dim = Utils::getUtils().getDimension();
  SArr.resize(dim);
  for(size_t i = 0; i < dim; i++) {
    SArr[i].resize(dim);
  }
  for(size_t i = 0; i < dim; i++) {
    for(size_t j = 0; j < dim; j++) {
      double weight = Utils::getUtils().getWeight(i, j);
      double bias = Utils::getUtils().getBias(i, j);
      SArr[i][j] = new MACUnit(weight, bias, i, j);
    }
  }

#ifdef GREEN_TPU
  if(!Utils::getUtils().updateLUTFromPreviousRun(errorLookUpTableFile)) {
    std::cout << "Could not update error look-up table from the snapshot of the previous run!" << std::endl;
    return false;
  }
#endif
  return true;
}

MACUnit::MACUnit(double w, double b, unsigned int i, unsigned int j) {
  weight = w;
  bias = b;
  prev_mac_out = 0;
  prev_mac_out_in_last_active_cycle = 0;
  output = std::numeric_limits<double>::lowest();
  last_output = std::numeric_limits<double>::lowest();
  activation = std::numeric_limits<double>::lowest();

  num_active_cycle = 0;
  num_bypassed_cycle = 0;
  num_errors = 0;

  row = i;
  col = j;

  bypass_operation = false;
}

bool MACUnit::bypassOperation() {
  return bypass_operation;
}

void MACUnit::setBypassOperation() {
  bypass_operation = true;
}

MACUnit* MACUnit::getUpstreamMAC() {
  int i = row - 1;
  if(i < 0) return NULL;
  return SystolicArray::getSystolicArray().getArray()[unsigned(i)][col];
}

MACUnit* MACUnit::getDownstreamMAC() {
  unsigned int i = row + 1;
  if(i >= Utils::getUtils().getDimension()) return NULL;
  return SystolicArray::getSystolicArray().getArray()[unsigned(i)][col];
}

/* If the immediately upstream MAC in the same column encountered
 * a timing error, the current MAC will skip multiply-and-accumulate
 * operation. Instead, the current MAC will pass the upstream MAC's 
 * output to the immediately downstream MAC in the same column. In 
 * this case, the current MAC is not doing any real work. So, its 
 * num_active_cycle will not be incremented. However, the upstream
 * MAC's num_active_cycle must have been incremented twice, as a
 * way to reflect the corrective action of TE-Drop. Also, no activity
 * in the current MAC ensures no timing error in the current cycle.
 */
void MACUnit::doMAC(bool pass_prev_mac_out) {
  MACUnit *up_mac = getUpstreamMAC();
  if(up_mac) {
    prev_mac_out_in_last_active_cycle = prev_mac_out;
    prev_mac_out = up_mac->getLastOutput();
  }
  if(pass_prev_mac_out) {
    assert(up_mac); // One should never request the upstream MAC of a MAC in row #0.
    last_output = output;
    output = prev_mac_out;
    num_bypassed_cycle++;
    bypass_operation = false;
    return;
  }
  unsigned int i, j;
  getRowCol(i, j);
  activation = Utils::getUtils().getActivation(i, num_active_cycle + num_bypassed_cycle);
  last_output = output;
  bool willEncounterTimingError = Utils::getUtils().willEncounterTimingError(this);
  num_active_cycle++;

  output = weight * activation + prev_mac_out + bias;
  if(willEncounterTimingError) {
    // std::cout << "encountered timing error in (" << row << "," << col << ")" << std::endl;
    num_errors++;
#ifndef TE_DROP
    output = Utils::getUtils().propagateTimingError(prev_mac_out);
    // output = strcmp(ERROR_PROP, "PREV_MAC_OUT") == 0 ? prev_mac_out : 0.0;
    return;
#endif
    Utils::getUtils().logTEDrop();
    MACUnit *down_mac = getDownstreamMAC();
    if(down_mac) {
      down_mac->setBypassOperation();
    } else { // a MAC in the last row
      // ThUnderVolt paper does not mention what happens when a MAC 
      // in the last row errs. In such a scenario, we assume that an 
      // incorrect value is latched to the output based on TEP.
      output = Utils::getUtils().propagateTimingError(prev_mac_out);
      // output = strcmp(ERROR_PROP, "PREV_MAC_OUT") == 0 ? prev_mac_out : 0.0;
    }
  }
}

long double MACUnit::getOutput() {
  return output;
}

long double MACUnit::getLastOutput() {
  bool pass_prev_mac_out = false;
  return Utils::getUtils().operateInCurrentClockCycle(row, col, pass_prev_mac_out) ? last_output : output;
}

void MACUnit::getRowCol(unsigned int& i, unsigned int& j) {
  i = row;
  j = col;
}

unsigned int MACUnit::getNumActiveCycle() {
  return num_active_cycle;
}

unsigned int MACUnit::getNumBypassedCycle() {
  return num_bypassed_cycle;
}

unsigned int MACUnit::getNumTimingErrors() {
  return num_errors;
}

unsigned int MACUnit::getNumTimesBoosted() {
  return cycle_boosted.size();
}

long double MACUnit::getPrevMacOut() {
  return prev_mac_out;
}

long double MACUnit::getPrevMacOutInLastActiveCycle() {
  return prev_mac_out_in_last_active_cycle;
}

void MACUnit::logBoostCount(unsigned int cycle) {
  if(cycle_boosted.find(cycle) != cycle_boosted.end()) return;
  cycle_boosted.insert(cycle);
}
