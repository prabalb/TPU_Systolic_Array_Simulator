/*
 * File: main.cpp
 * Author: Prabal Basu
 * Contact: prabalb@aggiemail.usu.edu
 */

#include "./SystolicArray.hpp"
#include <stdlib.h>
#include <getopt.h>
#include <time.h>

void reportUsage(char* argv []) {
  std::cout << "Usage: " << argv[0] << " [-h] -a <activation_file> -b <bias_file> -w <weight_file> "
    << "-l <error_lut_file> " << "-o <output_file> -g <green_tpu_stat_file> -d <dimension>" << std::endl;
  std::cout << "Options:" << std::endl;
  std::cout << "  -h    Optional help flag that prints the usage info" << std::endl;
  std::cout << "  -a    Activation file containing the activation values for the inference" << std::endl;
  std::cout << "  -b    Bias file containing the bias of each MAC obtained from training" << std::endl;
  std::cout << "  -w    Weight file containing the weight of each MAC obtained from training" << std::endl;
  std::cout << "  -l    Input/output file that will be read and/or written with last the simulation's Error LUT for GreenTPU" << std::endl;
  std::cout << "  -o    Output file that will contain the output matrix" << std::endl;
  std::cout << "  -g    Output file that will contain the the GreenTPU statistics" << std::endl;
  std::cout << "  -d    Dimenstion of the systolic array (dXd)" << std::endl;
  std::cout << "\ne.g.:\n";
  std::cout << argv[0] << " -a activation.txt -b bias.txt -w weight.txt -l lut.txt -o output.txt -g green_tpu_stats.txt -d 8" << std::endl;
}

int main(int argc, char **argv) {
  char *biasFileName = NULL;
  char *weightFileName = NULL;
  char *activationFileName = NULL;
  char *errorLookUpTableFileName = NULL;
  char *outputFileName = NULL;
  char *greenTPUStatsFileName = NULL;
  unsigned int dim = 0;
  char c;

  while((c = getopt(argc,argv,"a:b:w:l:o:g:d:h")) != -1) {
    switch(c) {
    case 'a':
      activationFileName = optarg;
      break;
    case 'b':
      biasFileName = optarg;
      break;
    case 'w':
      weightFileName = optarg;
      break;
    case 'l':
      errorLookUpTableFileName = optarg;
      break;
    case 'o':
      outputFileName = optarg;
      break;
    case 'g':
      greenTPUStatsFileName = optarg;
      break;
    case 'd':
      dim = atoi(optarg);
      break;
    case 'h':
      reportUsage(argv);
      exit(0);
    default:
      exit(0);
    }
  }

  if(dim == 0 || 
     activationFileName == NULL ||
     biasFileName == NULL ||
     weightFileName == NULL || 
     errorLookUpTableFileName == NULL ||
     outputFileName == NULL ||
     greenTPUStatsFileName == NULL) {
    reportUsage(argv);
    exit(1);
  }
  
  if(!Utils::getUtils().populateDBs(dim, biasFileName, weightFileName, activationFileName)) {
    exit(2);
  }

  if(!SystolicArray::getSystolicArray().createSystolicArray(errorLookUpTableFileName)) {
    exit(3);
  }

#ifdef DETAILED_SIM
  srand(3); // PV induced delay consideration
#endif

  std::cout << "\n" << "Started simulating the systolic array ..." << "\n\n";

  double wall0 = Utils::getUtils().get_wall_time();
  double cpu0  = Utils::getUtils().get_cpu_time();

  if(!Utils::getUtils().simulateSystolicArray(outputFileName)) {
    exit(4);
  }

  double wall1 = Utils::getUtils().get_wall_time();
  double cpu1  = Utils::getUtils().get_cpu_time();

  std::cout << "Done simulating the systolic array ..." << "\n\n";

#ifdef REPORT_STATS
  Utils::getUtils().reportStats(greenTPUStatsFileName);
#endif

  std::cout << "Simulation Time(s) : Wall Time : " << wall1 - wall0 << ", CPU Time : " << cpu1  - cpu0 << "\n\n";

  return 0;
}
