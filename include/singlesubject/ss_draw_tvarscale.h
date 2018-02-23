#ifndef GUARD_ss_draw_tvarscale_h
#define GUARD_ss_draw_tvarscale_h

#include <RcppArmadillo.h>
#include <RInside.h>
#include <math.h>
#include "mh.h"
#include "patient.h"


// NOTE: I separated out function definitions here 

//
// SS_DrawFixedEffects
//   Modified Metropolis Hastings sampler instantiating the mmh class for
//   sample the mean mass & width
//

class SS_DrawTVarScale : public ModifiedMetropolisHastings<PulseEstimate, Patient, double, ProposalVariance>
{

  public:
    // Constructor
    SS_DrawTVarScale(double in_pv, int in_adjust_iter, int in_max_iter,
                     double in_target_ratio) :
      ModifiedMetropolisHastings
      <PulseEstimate, Patient, double,
       ProposalVariance>::ModifiedMetropolisHastings(in_pv,
                                                     in_adjust_iter,
                                                     in_max_iter,
                                                     in_target_ratio) { };

    // Pulse-specific estimate -- this function samples for each pulse
    void sample_pulses(Patient *patient) {

      std::list<PulseEstimate>::iterator pulse = patient->pulses.begin();
      std::list<PulseEstimate>::const_iterator pulse_end = patient->pulses.end();

      while (pulse != pulse_end) {
        sample(&(*pulse), &pulse->tvarscale_mass, patient);
        pulse++;
      }

    }

  private:
    bool parameter_support(double val, Patient *notused);
    double posterior_function(PulseEstimate *pulse, double proposal, Patient *patient);

};




//
// Defined functions for SD random effects MMH class
//

// parameter_support()
//   Defines whether the proposal value is within the parameter support
bool SS_DrawTVarScale::parameter_support(double val, Patient *notused) {
  return (val > 0.0);
}


// posterior_function()
//   Calculates the acceptance ratio for use in modified metropolis hastings
//   sampler (inherited SS_DrawTVarScale::sample() function)
double SS_DrawTVarScale::posterior_function(PulseEstimate *pulse, 
                                            double proposal, 
                                            Patient *patient) {

  double old_gamma   = 0.0;
  double new_gamma   = 0.0;
  double prior_ratio = 0.0;
  double stdold      = 0.0;
  double stdnew      = 0.0;
  double re_old      = 0.0;
  double re_new      = 0.0;
  double re_ratio    = 0.0;
  double mass_sd     = patient->estimates->mass_sd;
  double mass_mean   = patient->estimates->mass_mean;
  double pulse_mass  = pulse->mass;
  double curr_scale  = pulse->tvarscale_mass;

  // Shape, scale parameterized: 
  //    https://github.com/mmulvahill/r-source/blob/trunk/src/nmath/dgamma.c
  //    https://cran.r-project.org/doc/manuals/r-release/R-exts.html#Distribution-functions
  old_gamma = Rf_dgamma(curr_scale, 2, 0.5, 0); 
  new_gamma = Rf_dgamma(proposal, 2, 0.5, 0);

  prior_ratio  = log(new_gamma) - log(old_gamma);

  stdold       = (pulse_mass) / (mass_sd / sqrt(curr_scale));
  stdnew       = (pulse_mass) / (mass_sd / sqrt(proposal));
  re_old       = (pulse_mass - mass_mean) * 0.5 * re_old * curr_scale;
  re_new       = (pulse_mass - mass_mean) * 0.5 * re_new * proposal;
  re_ratio     = (re_old - re_new) / (mass_sd * mass_sd);
  re_ratio    += Rf_pnorm5(stdold, 0, 1, 1.0, 1.0) -  // second 1.0 does the log xform for us 
                 Rf_pnorm5(stdnew, 0, 1, 1.0, 1.0) -  // first 1.0 says to use lower tail      
                 0.5 * log(curr_scale) + 0.5 * log(proposal); // the 1/2pi term in normal distirbution

  // Compute and acceptance ratio
  return prior_ratio + re_ratio;

};

#endif

