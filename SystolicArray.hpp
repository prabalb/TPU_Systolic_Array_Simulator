/*
 * File: SystolicArray.hpp
 * Author: Prabal Basu
 * Contact: prabalb@aggiemail.usu.edu
 */

#include <iostream>

#include <vector>
#include <list>
#include <map>
#include <set>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <string>
#include <cstdint>

using namespace std;

#define DETAILED_SIM
#define GUIDED_SIM
#define SKIP_STA_TOOL

//#define TEP
#define TE_DROP
#define OUTPUT_BIT_WIDTH 24
#define ERROR_PROP "PREV_MAC_OUT"
//#define ERROR_PROP "ZERO"

#define GREEN_TPU
//#define GREEN_TPU_LITE
#define USE_ADVANCED_ELUT
#define CREATE_ELUT_PER_ROW
#define PERFORM_ADVANCED_LOOK_UP
#define ADVANCED_LOOK_UP_PARAM 50
#define LRU_ERROR_LOOK_UP_TAB_SIZE 100
#define BOOST_GRANULARITY 16

#define REPORT_STATS

#define GLOBAL_CLOCK_PERIOD 5 // in nanosecond

// From Booster paper: Switching from 400mV
// to 600mV takes closer to 9ns
#define BOOSTER_DELAY 1
#define BOOSTER_CLOCK_CYCLE ceil(floor(BOOSTER_DELAY)/GLOBAL_CLOCK_PERIOD)

#ifndef SYST_ARR
#define SYST_ARR

class MACUnit {

  private:
    double weight;
    double activation;
    double bias;
    long double prev_mac_out;
    long double prev_mac_out_in_last_active_cycle;
    long double output;
    long double last_output;

    unsigned int num_active_cycle;
    unsigned int num_bypassed_cycle; // Victim MAC relinquishes its operation
    unsigned int num_errors; // Number of timing errors encountered

    unsigned int row; // Row coordinate
    unsigned int col; // Column coordinate

    bool bypass_operation;

    std::set<unsigned int> cycle_boosted; // GreenTPU

  public:
    MACUnit(double w, double b, unsigned int i, unsigned int j);

    MACUnit* getUpstreamMAC();
    MACUnit* getDownstreamMAC();
    void getRowCol(unsigned int& i, unsigned int& j);
    void doMAC(bool pass_prev_mac_out=false);
    long double getOutput();
    long double getLastOutput();

    // The clients of the following two methods should
    // be careful as to not use the stale values!
    long double getPrevMacOut();
    long double getPrevMacOutInLastActiveCycle();

    bool bypassOperation();
    void setBypassOperation();
    void logBoostCount(unsigned int cycle);

    unsigned int getNumActiveCycle();
    unsigned int getNumBypassedCycle();
    unsigned int getNumTimingErrors();
    unsigned int getNumTimesBoosted();
};

typedef std::vector<std::vector<MACUnit*> > SystArr;

class SystolicArray {

  private:
    SystArr SArr;

    SystolicArray() {}
    void operator=(SystolicArray&);
    SystolicArray(const SystolicArray&);

  public:
    static SystolicArray& getSystolicArray();
    SystArr& getArray();
    bool createSystolicArray(const std::string errorLookUpTableFile);
};

class Utils {

  public:
#ifdef USE_ADVANCED_ELUT
    class GreenTPULRUErrorLookUpTable {
    
      private:
        unsigned int m_lut_size;
        std::set<std::pair<double, double> > m_patterns;
    
      public:
        GreenTPULRUErrorLookUpTable(unsigned int size) : m_lut_size(size) {}
        void updateLUTFromPreviousRun(std::set<std::pair<double, double> >& patterns);
        void getPatterns(std::set<std::pair<double, double> >& patterns) {
          patterns = m_patterns;
        }
        void clearPatterns() {
          m_patterns.clear();
        }
        bool referAndUpdateLUT(MACUnit *mac);
        bool performAdvancedLookUp(double curr_active, double prev_active);
    };
#else
    class LRUErrorLookUpTable {

      private:
        unsigned int m_lut_size;

        std::list<MACUnit*> m_mac_list;
        std::unordered_map<MACUnit*, std::list<MACUnit*>::iterator> m_key_map;
        std::map<MACUnit*, std::set<std::pair<double, double> > > m_mac_to_patterns;

      public:
        LRUErrorLookUpTable(unsigned int size) : m_lut_size(size) {}
        void updateLUTFromPreviousRun(MACUnit *mac, std::set<std::pair<double, double> >& patterns);
        bool referAndUpdateLUT(MACUnit *mac);

        bool performAdvancedLookUp(double curr_active, double prev_active, std::set<std::pair<double, double> >& patterns);
        
        void getMACToPatternsDBs(std::list<MACUnit*>& mlist, std::map<MACUnit*, std::set<std::pair<double, double> > >& m_to_pat) {
          mlist = m_mac_list;
          m_to_pat = m_mac_to_patterns;
        }
    };
#endif

    static Utils& getUtils() {
      static Utils utils;
      return utils;
    }

    unsigned int getDimension();
    unsigned int getNumCycleSimulated();
    double getBias(unsigned int i, unsigned int j);
    double getWeight(unsigned int i, unsigned int j);
    double getActivation(unsigned int row, unsigned int num_active_cycle); // the 2nd arg name is a slight misnomer

    double get_wall_time();
    double get_cpu_time();

    unsigned countNumSetBits(int number);
    unsigned int getNumBatches();
    long double propagateTimingError(long double prev_mac_out);
    bool canGreenTPUAvoidTimingError(MACUnit *mac);
    bool simulateSystolicArray(const std::string outputFile);
    bool willEncounterTimingError(MACUnit *mac);
    bool reportStats(const std::string greenTPUStatsFileName);
    void logTEDrop();
    void updateErrorLUT(unsigned row, unsigned col);
    void updateBoostStat(MACUnit *mac);
    void getStaToolCmdLine(std::string& sta_tool_cmd_line, MACUnit *mac);
    bool updateLUTFromPreviousRun(const std::string errorLookUpTableFile);
    bool writeErrorLookUpTable();
    bool operateInCurrentClockCycle(unsigned int row, 
                                    unsigned int col, 
                                    bool& pass_prev_mac_out);
    bool populateDBs(unsigned int dimension,
                     const std::string biasFile,
                     const std::string weightFile,
                     const std::string activationFile);
  private:
    unsigned int dim; // e.g., dim = 256 means 256X256
    unsigned int num_cycle_simulated;
    unsigned int num_sta_tool_invoked;
    unsigned int num_te_drop;

    double **bias;
    double **weight;
    double **activation;

    std::string errorLUTFile;

#ifdef USE_ADVANCED_ELUT
    std::vector<GreenTPULRUErrorLookUpTable *> elu;
#else
    std::vector<LRUErrorLookUpTable *> elu;
#endif

    std::set<std::tuple<int, int, int> > high_delay_seq_db; // for guided detailed sim

    Utils() : dim(0), num_cycle_simulated (1), num_sta_tool_invoked(0), 
              num_te_drop(0), errorLUTFile("") {}
    void operator=(Utils&);
    Utils (const Utils&);
};

#endif // SYST_ARR
