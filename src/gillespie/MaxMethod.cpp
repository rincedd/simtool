/**
 * @file MaxMethod.cpp
 * @date 02.08.2010
 * @author gerd
 */

#include "MaxMethod.h"
#include <myrng1.2/myrngWELL.h>
#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>
#include <boost/bind.hpp>

using namespace std;
using namespace boost;

namespace sim
{
namespace gillespie
{

MaxMethod::MaxMethod()
{
}

void MaxMethod::registerProcess(Process* p)
{
	processes_.push_back(p);
}

double MaxMethod::step()
{
	typedef vector<double> RateVector;
	RateVector max_rates(processes_.size(), 0.0), max_rates_times_sizes(
			processes_.size(), 0.0);

	// fill max_rates vector
	transform(processes_.begin(), processes_.end(), max_rates.begin(),
			bind(&Process::maxRate, _1));

	// fill max_rates_times_sizes vector
	transform(processes_.begin(), processes_.end(), max_rates.begin(),
			max_rates_times_sizes.begin(), bind(multiplies<double>(), bind(&Process::populationSize, _1), _2));

	// calculate 1 / sum(max_rate*pop_size)
	double inv_total_max_rate_times_size = 1.0 / accumulate(
			max_rates_times_sizes.begin(), max_rates_times_sizes.end(), 0.0);

	// calculate probabilities for each process
	// FIXME: potential performance issue
	RateVector process_probs(processes_.size(), 0.0);
	transform(max_rates_times_sizes.begin(), max_rates_times_sizes.end(),
			process_probs.begin(), bind(multiplies<double>(), _1, inv_total_max_rate_times_size));
	double tau = 0;
	while (true)
	{
		// draw length of this time step from exponential distribution
		tau += rng.Exponential(inv_total_max_rate_times_size);
		// pick process with probability max_rate * pop_size / sum(max_rate * pop_size)
		int k = rng.Choices(&(process_probs.front()), process_probs.size());
		Agent* a = processes_[k].randomAgent();
		double prob = processes_[k].agentRate(a) / max_rates[k];
		if (rng.Chance(prob))
		{
			processes_[k](a);
			break;
		}
	}
	return tau;
}

}
}
