#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>

#include <gsl/gsl_statistics_double.h>


using namespace std;

struct variance1 : public binary_function<double, double, double> {
  variance1(double avg): _avg(avg) { }
  double operator()(double x, double y) { return x + ::pow(_avg - y, 2); }
private:
  double _avg;
};



int main(int argc, char* argv[])
{
  if( argc != 2 ) {
	cerr << argv[0] << " factors_file" << endl;
	exit(EXIT_FAILURE);
  }

  ifstream ifile;
  string line;
  vector<double> factors;

  ifile.open(argv[1]);
  while( !ifile.eof() ) {
	getline(ifile, line);
	if( !line.size() ) continue;
	factors.push_back(atof(line.c_str()));
  }
  cout << "Tot Factors: " << factors.size() << endl;

  vector<double> factors_rates;
  transform(factors.begin(), factors.end(), back_inserter(factors_rates), bind2nd(minus<double>(), 1));

  vector<double> factors_logs;
  transform(factors.begin(), factors.end(), back_inserter(factors_logs), ::log10);

  cout << "Factors" << endl;
  for( int i = 0; i < factors.size(); i++ )
	cout << factors[i] << endl;
  cout << endl;

  double* cfactors = new double[factors.size()];
  copy(factors.begin(), factors.end(), cfactors);

  cout << "Rates" << endl;
  for( int i = 0; i < factors.size(); i++ )
	cout << factors_rates[i] << endl;
  cout << endl;

  cout << "Logs" << endl;
  for( int i = 0; i < factors.size(); i++ )
	cout << factors_logs[i] << endl;
  cout << endl;

  double* clfactors = new double[factors_logs.size()];
  copy(factors_logs.begin(), factors_logs.end(), clfactors);

  cout << "Arithmetic Mean" << endl;
  double f_ari_mean = accumulate<vector<double>::iterator, double>(factors.begin(), factors.end(), 0) / factors.size();
  cout << f_ari_mean << endl;
  cout << endl;

  cout << "GSL Arithmetic Mean" << endl;
  cout << gsl_stats_mean(cfactors, 1, factors.size()) << endl;
  cout << endl;

  cout << "Arithmetic Variance" << endl;
  double f_ari_variance = accumulate<vector<double>::iterator, double>(factors.begin(), factors.end(), 0, variance1(f_ari_mean)) /
	(factors.size() - 1);
  cout << f_ari_variance << endl;
  cout << endl;

  cout << "GSL Arithmetic Variance" << endl;
  cout << gsl_stats_variance(cfactors, 1, factors.size()) << endl;
  cout << endl;

  cout << "Arithmetic Standard Deviation" << endl;
  double f_ari_stddev = sqrt(f_ari_variance);
  cout << f_ari_stddev << endl;
  cout << endl;

  cout << "GSL Arithmetic Standard Deviation" << endl;
  cout << gsl_stats_sd(cfactors, 1, factors.size()) << endl;
  cout << endl;

  cout << "GSL Arithmetic Skew" << endl;
  cout << gsl_stats_skew(cfactors, 1, factors.size()) << endl;
  cout << endl;

  cout << "Geometric Mean" << endl;
  double f_geo_mean = ::pow(accumulate<vector<double>::iterator, double>(factors.begin(), factors.end(), 1, multiplies<double>()),
						  1/(double)factors.size());
  cout << f_geo_mean << endl;
  cout << endl;

  cout << "Logs Arithmetic Mean" << endl;
  double fl_ari_mean = accumulate<vector<double>::iterator, double>(factors_logs.begin(), factors_logs.end(), 0) / factors_logs.size();
  cout << fl_ari_mean << endl;
  cout << endl;

  cout << "Logs Arithmetic Variance" << endl;
  double fl_ari_variance = accumulate<vector<double>::iterator, double>(factors_logs.begin(), factors_logs.end(), 0, variance1(fl_ari_mean)) /
	(factors_logs.size() - 1);
  cout << fl_ari_variance << endl;
  cout << endl;

  cout << "Logs Arithmetic Standard Deviation" << endl;
  double fl_ari_stddev = sqrt(fl_ari_variance);
  cout << fl_ari_stddev << endl;
  cout << endl;

  cout << "GSL Logs Arithmetic Standard Deviation" << endl;
  cout << gsl_stats_sd(clfactors, 1, factors_logs.size()) << endl;
  cout << endl;

  double fl_skew = gsl_stats_skew(clfactors, 1, factors_logs.size());
  cout << "GSL Logs Skewness" << endl;
  cout << fl_skew << endl;
  cout << endl;	

  cout << "Geometric Standard Deviation (factors logs)" << endl;
  double fl_geo_stddev = pow(10, fl_ari_stddev);
  cout << fl_geo_stddev << endl;
  cout << endl;

  double gsd2 = pow(10, fl_ari_stddev);
  cout << "GSD 2: " << gsd2 << endl;

  double gsd2_monthly = pow(10, fl_ari_stddev * sqrt(12));
  cout << "GSD 2 (monthly): " << (gsd2_monthly-1)*100 << '%' << endl;

  double gs_monthly = pow(10, fl_skew * sqrt(12));
  cout << "GSm: " << (gs_monthly-1)*100 << '%' << endl;

  return 0;
}
