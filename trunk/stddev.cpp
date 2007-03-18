#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>

using namespace std;

struct variance1 : public binary_function<double, double, double> {
  variance1(double avg): _avg(avg) { }
  // accumulate() doesn't accumulate when using binary_function!
  double operator()(double x, double y) { return x + ::pow(y - _avg, 2); }

private:
  double _avg;
};


int main(int argc, char* argv[])
{
  vector<double> factors;
  for( int i = 1; i < argc; i++ )
	factors.push_back(atof(argv[i]));

  cout << "Factors" << endl;
  for( int i = 0; i < factors.size(); i++ )
	cout << factors[i] << endl;
  cout << endl;

  cout << "Arithmetic Mean" << endl;
  double f_ari_mean = accumulate<vector<double>::iterator, double>(factors.begin(), factors.end(), 0) / factors.size();
  cout << f_ari_mean << endl;
  cout << endl;

  cout << "Sum of squared deviations" << endl;
  double f_sum_sqdev = accumulate<vector<double>::iterator, double>(factors.begin(), factors.end(), 0, variance1(f_ari_mean));
  cout << f_sum_sqdev << endl;
  cout << endl;

  cout << "Arithmetic Variance" << endl;
  double f_ari_variance = f_sum_sqdev / (factors.size() - 1);
  cout << f_ari_variance << endl;
  cout << endl;

  cout << "Arithmetic Standard Deviation" << endl;
  double f_ari_stddev = sqrt(f_ari_variance);
  cout << f_ari_stddev << endl;
  cout << endl;

  return 0;
}
