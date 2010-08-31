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
#include <boost/format.hpp>

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
	transform(processes_.begin(), processes_.end(), max_rates.begin(), bind(
			&Process::maxRate, _1));

	// fill max_rates_times_sizes vector
	transform(processes_.begin(), processes_.end(), max_rates.begin(),
			max_rates_times_sizes.begin(), bind(multiplies<double> (), bind(
					&Process::populationSize, _1), _2));

	// calculate 1 / sum(max_rate*pop_size)
	const double total_max_rate_times_size = accumulate(
			max_rates_times_sizes.begin(), max_rates_times_sizes.end(), 0.0);
	if (total_max_rate_times_size == 0.0)
		return 1000;

	const double inv_total_max_rate_times_size = 1.0 / total_max_rate_times_size;

	double tau = 0;
	while (true)
	{
		// draw length of this time step from exponential distribution
		tau += rng.Exponential(inv_total_max_rate_times_size);

		// pick process with probability max_rate_times_size / total_max_rate_times_size
		// we do not use rng.Choices() here, as we would have to supply all probabilities with proper
		// normalization, potentially leading to rounding/truncation errors
		const double u = rng.Uniform01() * total_max_rate_times_size;
		unsigned int k = 0;
		double s = max_rates_times_sizes[k];
		while (s < u && k < max_rates_times_sizes.size() - 1)
			s += max_rates_times_sizes[++k];

		// execute process with probability agent_rate / max_rate
		Agent* a = processes_[k].randomAgent();
		const double prob = processes_[k].agentRate(a) / max_rates[k];
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
