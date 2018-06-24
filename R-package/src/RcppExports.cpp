// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <RcppArmadillo.h>
#include <Rcpp.h>

using namespace Rcpp;

// singlesubject_
Rcpp::List singlesubject_(Rcpp::NumericVector concentration, Rcpp::NumericVector time, Rcpp::List priors, Rcpp::List proposalvars, Rcpp::List startingvals, int mcmc_iterations, int thin, int burnin, bool verbose, int pv_adjust_iter, int pv_adjust_max_iter, double bivariate_pv_target_ratio, double univariate_pv_target_ratio);
RcppExport SEXP _bayespulse_singlesubject_(SEXP concentrationSEXP, SEXP timeSEXP, SEXP priorsSEXP, SEXP proposalvarsSEXP, SEXP startingvalsSEXP, SEXP mcmc_iterationsSEXP, SEXP thinSEXP, SEXP burninSEXP, SEXP verboseSEXP, SEXP pv_adjust_iterSEXP, SEXP pv_adjust_max_iterSEXP, SEXP bivariate_pv_target_ratioSEXP, SEXP univariate_pv_target_ratioSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< Rcpp::NumericVector >::type concentration(concentrationSEXP);
    Rcpp::traits::input_parameter< Rcpp::NumericVector >::type time(timeSEXP);
    Rcpp::traits::input_parameter< Rcpp::List >::type priors(priorsSEXP);
    Rcpp::traits::input_parameter< Rcpp::List >::type proposalvars(proposalvarsSEXP);
    Rcpp::traits::input_parameter< Rcpp::List >::type startingvals(startingvalsSEXP);
    Rcpp::traits::input_parameter< int >::type mcmc_iterations(mcmc_iterationsSEXP);
    Rcpp::traits::input_parameter< int >::type thin(thinSEXP);
    Rcpp::traits::input_parameter< int >::type burnin(burninSEXP);
    Rcpp::traits::input_parameter< bool >::type verbose(verboseSEXP);
    Rcpp::traits::input_parameter< int >::type pv_adjust_iter(pv_adjust_iterSEXP);
    Rcpp::traits::input_parameter< int >::type pv_adjust_max_iter(pv_adjust_max_iterSEXP);
    Rcpp::traits::input_parameter< double >::type bivariate_pv_target_ratio(bivariate_pv_target_ratioSEXP);
    Rcpp::traits::input_parameter< double >::type univariate_pv_target_ratio(univariate_pv_target_ratioSEXP);
    rcpp_result_gen = Rcpp::wrap(singlesubject_(concentration, time, priors, proposalvars, startingvals, mcmc_iterations, thin, burnin, verbose, pv_adjust_iter, pv_adjust_max_iter, bivariate_pv_target_ratio, univariate_pv_target_ratio));
    return rcpp_result_gen;
END_RCPP
}

RcppExport SEXP run_testthat_tests();

static const R_CallMethodDef CallEntries[] = {
    {"_bayespulse_singlesubject_", (DL_FUNC) &_bayespulse_singlesubject_, 13},
    {"run_testthat_tests",         (DL_FUNC) &run_testthat_tests,          0},
    {NULL, NULL, 0}
};

RcppExport void R_init_bayespulse(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
