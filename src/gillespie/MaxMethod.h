/**
 * @file MaxMethod.h
 * @date 02.08.2010
 * @author gerd
 */

#ifndef MAXMETHOD_H_
#define MAXMETHOD_H_

#include <boost/ptr_container/ptr_vector.hpp>
#include <cstddef>
#include <stdexcept>
#include <string>

namespace sim
{
namespace gillespie
{

class Agent
{
public:
	Agent() {}
	virtual ~Agent() {}
};

class MaxMethod
{
public:
	class Process
	{
	public:
		Process() {}
		virtual ~Process() {}
		virtual double maxRate() = 0;
		virtual double agentRate(Agent* a) = 0;
		virtual Agent* randomAgent() = 0;
		virtual size_t populationSize() = 0;
		virtual void operator()(Agent* a) = 0;
		virtual size_t numCalls() const { return 0; }	// for debugging
		virtual std::string name() const { return ""; }
	};
public:
	MaxMethod();
	void registerProcess(Process* p);
	size_t numberOfProcesses() const;
	double step();
	const Process* process(unsigned int index) const;

private:
	typedef boost::ptr_vector<Process> ProcessesVector;
	ProcessesVector processes_;
};

inline size_t MaxMethod::numberOfProcesses() const
{
	return processes_.size();
}

inline const MaxMethod::Process* MaxMethod::process(unsigned int index) const
{
	if (index < processes_.size())
		return &processes_[index];
	else
		throw(std::out_of_range("Process index out of range"));
}

}
}
#endif /* MAXMETHOD_H_ */
