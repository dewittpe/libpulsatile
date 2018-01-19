#ifndef GUARD_metropolishastings_h
#define GUARD_metropolishastings_h

#include <RcppArmadillo.h>
#include <RInside.h>
#include "proposalvariance.h"
#include "utils.h"

// metropolishastings.h
//   Abstract class for defining Modified Metropolis Hastings samplers
//
// Author: Matt Mulvahill
// Created: 10/13/17
// Notes:
//   Outstanding questions:
//    - Where does this class get implemented? -- implementation handled here,
//      instantiation handled in either mcmc function or MMH classes?
//    - draw_proposal() takes SD's - straighten this out.
//    - How do I get priors, parms, data into MMH methods?
//    - Pointer *is --may be-- best.  Is there a meaningful/simple way to take
//      simple args and return a value? Or is it best to update internally
//

template <typename T, typename S, typename PV>
class ModifiedMetropolisHastings
{

  public:
    //T * sampling_unit; // either patient or population class
    //S current_val;   // current sample value (double or arma::vec)
    PV pv;                   // needs to be a ProposalVariance object

    // sample from posterior
    //   - runs 1 iteration
    //   - inputs/outputs or changes internally?
    //   - pass Patient as pointer?
    //  S will be double (or int) or arma::vec depending on single or two
    //    parameter MMH
    void sample(T *sampling_unit, S *current_val) {

      double accept_prob, alpha;

      // Draw new proposal
      S proposal     = draw_proposal(*current_val, pv.getpsd());
      bool supported = parameter_support(proposal);

      if (!supported) {

        pv.addreject();

      } else {

        accept_prob = posterior_function(sampling_unit, proposal);
        alpha = (0 < accept_prob) ? 0 : accept_prob;

        if (log(R::runif(0, 1)) < alpha) {

          pv.addaccept();
          *current_val = proposal;

        } else {

          pv.addreject();

        }
      }
    }

  protected:
    // Constructors
    ModifiedMetropolisHastings() { ProposalVariance pv; }
    ModifiedMetropolisHastings(PV proposal_variance) {
      pv = proposal_variance;
    }
    ModifiedMetropolisHastings(S in_pv, // double or arma::vec
                               int in_adjust_iter,
                               int in_max_iter,
                               double in_target_ratio) : 
      PV(in_pv, in_adjust_iter, in_max_iter, in_target_ratio) {
    }

  private:

    PulseUtils pu;
    double draw_proposal(double current, double proposal_sd) {
      return Rf_rnorm(current, proposal_sd);
    };
    arma::vec draw_proposal(const arma::vec current, arma::mat proposal_sd){
      return pu.rmvnorm(current, proposal_sd);
    };
    virtual bool parameter_support(S val); // i.e. truncation logic
    virtual double posterior_function(T * sampling_unit, S proposal);   // logrho_calculation

};






#endif
