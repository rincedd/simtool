/**
 * @file MaxMethod.cpp
 * @date 02.08.2010
 * @author gerd
 */

#include "MaxMethod.h"
#include <myrng1.2/myrngWELL.h>
#include <vector>

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
	typedef std::vector<double> RateVector;
	RateVector max_rates, max_rates_times_sizes;
	max_rates.reserve(processes_.size());
	max_rates_times_sizes.reserve(processes_.size());
	double total_max_rate_times_size = 0.0;
	for (ProcessesVector::iterator it = processes_.begin(); it
			!= processes_.end(); ++it)
	{
		double max_rate = it->maxRate();
		total_max_rate_times_size += max_rate;
		max_rates.push_back(max_rate);
		max_rates_times_sizes.push_back(max_rate * it->populationSize());
	}

	double tau = 0, inv_total_max_rate_times_size = 1.0 / total_max_rate_times_size;
	while (true)
	{
		tau += rng.Exponential(inv_total_max_rate_times_size);
		double x = rng.Uniform01() * total_max_rate_times_size;
		unsigned int k = 0;
		double s = max_rates_times_sizes[k];
		while (s < x)
			s += max_rates_times_sizes[++k];
		// now, try to execute process
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
