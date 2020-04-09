#include <string>
#include <stdlib.h>

#include <iostream>
#include <iomanip>
#include <cmath>

#include "RandomVariable.hpp"


#define True 1
#define False 0


int main(int argc, char* argv[]){
    
    RandomVariable rv;

    int numTosses       = 100;
    int numHeads        = 43;
    int chainLength     = 1000000;
    int sampleFrequency = 1;
    double window       = 0.1;


    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];

        if ( arg == "-h" || arg == "--help" )
        {
            std::cout << "\n\n Help page under construction\n" << std::endl;
            exit(0);
        }
        if ( arg == "-n" ) numTosses       = atoi(argv[++i]);
        if ( arg == "-p" ) numHeads        = atoi(argv[++i]);
        if ( arg == "-i" ) chainLength     = atoi(argv[++i]);
        if ( arg == "-s" ) sampleFrequency = atoi(argv[++i]);
        if ( arg == "-w" ) window          = atof(argv[++i]);
    }

    int numTails  = numTosses - numHeads;
    // std::cout << numTosses << std::endl;
    // std::cout << window << std::endl;
    // exit(0);


    // initialize the prior (theta)
    double theta = rv.uniformRv();

    int bins[100];
    for (int i = 0; i < 100; i++)
        bins[i] = 0;

    int numSamples = 0;

    // run Markov chains
    for (int i = 0; i < chainLength; i++)
    {
        double thetaPrime = theta + (rv.uniformRv() - 0.5) * window;

        // keep values inside the sliding window range
        if (thetaPrime < 0.0){
            thetaPrime = -thetaPrime;

        }else if (thetaPrime > 1.0){
            thetaPrime = 2.0 - thetaPrime;
        }

        double lnL_ratio = (numHeads*log(thetaPrime) + numTails*log(1.0 - thetaPrime)) - 
                           (numHeads*log(theta     ) + numTails*log(1.0 - theta     ));
        // because of flat prior assumption. All positive 
        // events have equal probability
        double lnPrior_ratio = 0.0;
        //  log((1/w)/(1/w)); where w is the window width
        double lnHastings_ratio = 0.0;
        double lnR = lnL_ratio + lnPrior_ratio + lnHastings_ratio;

        double R = 0.0;
        if (lnR < -300.0){ // this is done due to overflow errors (i.e. exp(-300) )
            R = 0.0;

        }else if (lnR > 0.0){
            R = 1.0;

        }else{
            R = exp(lnR);
        }

        // if (i % printFrequency == 0){
        //     std::cout << std::setw(5) << i << " -- ";
        //     std::cout << std::fixed << std::setprecision(3) << theta << " -> ";
        //     std::cout << std::fixed << std::setprecision(3) << thetaPrime << " ";
        // }
        
        double u = rv.uniformRv();
        // bool isAccepted = false;

        if (u < R){
            theta = thetaPrime;
            // isAccepted = true;
        }

        // if (i % printFrequency == 0){
        //     if (isAccepted == true){
        //         std::cout << "(Accepted)";
        //     }else{
        //         std::cout << "(Rejected)";
        //     }   
        //     std::cout << std::endl;
        // }

        // sample the chain
        if (i % sampleFrequency == 0)
        {
            numSamples++;
            bins[(int)(theta*100)]++;
        }        
    }

    // summarize results
    double cumulativeProbability = 0.0;
    for (int i = 0; i < 100; i++)
    {
        double intervalProbability = (double)bins[i] / numSamples;
        cumulativeProbability += intervalProbability;
        std::cout << std::fixed << std::setprecision(2) << i * 0.01 << " - ";
        std::cout << std::fixed << std::setprecision(2) << (i + 1) * 0.01 << " -- ";
        std::cout << std::setw(6) << bins[i] << " ";
        std::cout << std::fixed << std::setprecision(3) << intervalProbability << " ";
        std::cout << std::fixed << std::setprecision(3) << cumulativeProbability << " ";
        std::cout << std::endl;
    }
    return 0;
}
