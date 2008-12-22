/** \file Testing the discrete distribution for selecting output path for a
 * given set of input particles.
 *
 * This test is meant to help design the
 * particledb::interaction::PreComputedSet class.
 * */


#include <math.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <vector>

#include <olson-tools/random/random.h>
#include <olson-tools/Distribution.h>
#include <olson-tools/GenericBin.h>
#include <olson-tools/physical/physical.h>

using physical::constant::amu;
using physical::constant::K_B;
using physical::unit::uK;

/** SOME DISTRIBUTION FUNCTION. */
struct OutputDistribution {
    const std::vector<double> & cs;
    OutputDistribution(const std::vector<double> & cs) : cs(cs) {}

    inline double distrib (const double & x) const {
        return cs[int(x)];
    }
};

struct DiscreteDistribution {
    int * bins;
    int L;

    DiscreteDistribution(const std::vector<double> & cs, const int & min_bins_per_iterval = 3) {
        const int &  Nmin= min_bins_per_iterval;

        /* first find the minimum interval */
        double cs_im1 = 0, min_intv = DBL_MAX;
        for (std::vector<double>::const_iterator i = cs.begin(); i < cs.end() ; i++) {
            double intv = (*i) - cs_im1;
            min_intv = std::min(min_intv, intv);
        }
        min_intv = std::min(min_intv, 1.0 - cs.back());

        L = int(Nmin * ( 1 / min_intv ));

        bins = new int[(unsigned int)L];

        std::cout << "filling " << L << " elements" << std::endl;

        /* now assign the bins */
        cs_im1 = 0;
        int bi = 0;
        for (unsigned int i = 0; i < cs.size() ; i++) {
            double intv = cs[i] - cs_im1;
            double Li = int(Nmin * intv / min_intv);

            for (int j = 0; j < Li; j++, bi++) {
                if (bi == L) {
                    std::cout << "overflow:  " << (Li - j) << std::endl;
                    break;
                }
                bins[bi] = i;
            }
        }
        /* for kicks, complain about the remaining. */
        for (; bi < L; bi++) {
            std::cout << "had remaining element(s):  " << (L - bi) << std::endl;
            bins[bi] = cs.size()-1;
        }
    }

    ~DiscreteDistribution() {
        delete[] bins;
    }

    int operator()() const {
        double r = MTRNGrand() * (L * 0.9999999999);
        return bins[int(r)];
    }
};


static double beta = 0.5 * (87.0 * amu) / (K_B * 500.0 * uK);
static double sigma = sqrt( 0.5 / beta);

int main() {
    std::vector<double> cs;
//  cs.push_back(10);
//  cs.push_back(40);
//  cs.push_back(30);
//  cs.push_back(100);
//  cs.push_back(30);

//  cs.push_back(0.047619);
//  cs.push_back(0.190476);
//  cs.push_back(0.142857);
//  cs.push_back(0.47619 );
//  cs.push_back(0.142857);

//  cs.push_back(0.0879095);
//  cs.push_back(0.0563767);
//  cs.push_back(0.259974 );
//  cs.push_back(0.350471 );
//  cs.push_back(0.245269 );

    for (int i = 0; i < 10; i++)
        cs.push_back(MTRNGrand() * 100);

    /* normalize is really unnecessary, but I'll do it anyway for right now. */
    double sum = 0;
    for (std::vector<double>::iterator i = cs.begin(); i < cs.end() ; i++) {
        sum += (*i);
    }
    double mx = 0;
    for (std::vector<double>::iterator i = cs.begin(); i < cs.end() ; i++) {
        (*i) /= sum;
        mx = std::max(mx, (*i));
    }

#if 0
    DiscreteDistribution D(cs,10);
    GenericBin<double,1000,double> bin(-1,cs.size()+1);
#else

#if 1
    Distribution D = Distribution(OutputDistribution(cs), 0, cs.size(), cs.size()*10);
    GenericBin<double,1000,double> bin(-1,cs.size()+1);
#else
    Distribution D = Distribution(GaussianDistrib(beta), -4*sigma, 4*sigma, 10000);
    GenericBin<double,100,double> bin(-4*sigma,4*sigma);
#endif

#endif

    int iter = 0;
    std::cout << "Enter the number of samples:  " << std::flush;
    std::cin >> iter;

    for (int i = 0; i < iter; i++) {
        bin.bin((int)D());
    }

    double m = 0;
    for (unsigned int i = 0; i < bin.nBins(); i++) {
        m = std::max(m,bin.bins[i]);
    }
    bin *= mx/((double)m);

//  for (unsigned int i = 0; i < bin.nBins(); i++) {
//      bin.bins[i] = ((double)bin.bins[i])* 100.0*mx/((double)m);
//  }


    std::ofstream outf("dist.dat");
    bin.print(outf,"");
    outf.close();

    std::ofstream outv("cs.dat");
    for (unsigned int i = 0; i < cs.size() ; i++) {
        outv << i << '\t' << cs[i] << std::endl;
    }
    outv.close();

    return 0;
}

