/**
 * @file TimeSeriesOutput.h
 * @date 29.07.2009
 * @author gerd
 */

#ifndef TIMESERIESOUTPUT_H_
#define TIMESERIESOUTPUT_H_

#include "IntervalOutput.h"
#include <network/TripleMultiNetwork.h>
#include <network/TripleNetwork.h>
#include <network/motifs/motifs.h>

/**
 * Outputs numbers of nodes, links, and possibly triples in all states in the network.
 */
template<class _Network>
class TimeSeriesOutput: public IntervalOutput
{
public:
	TimeSeriesOutput(std::ostream& out, const _Network& net, double interval);
	virtual ~TimeSeriesOutput();

private:
	void doOutput(double t);
	void doWriteHeader();
	void addTripleOutput(const char sep) {}
	void addTripleHeader(const char sep) {}
	const _Network& net_;
	const lnet::motifs::NodeMotifSet nodeMotifs_;
	const lnet::motifs::LinkMotifSet linkMotifs_;
	const lnet::motifs::TripleMotifSet tripleMotifs_;
};

template<class _Network>
TimeSeriesOutput<_Network>::TimeSeriesOutput(std::ostream& out,
		const _Network& net, const double interval) :
	IntervalOutput(out, interval), net_(net), nodeMotifs_(
			net.numberOfNodeStates()), linkMotifs_(net.numberOfNodeStates()),
			tripleMotifs_(net.numberOfNodeStates())
{
}

template<class _Network>
TimeSeriesOutput<_Network>::~TimeSeriesOutput()
{
}

template<class _Network>
void TimeSeriesOutput<_Network>::doOutput(const double t)
{
	const char sep = '\t';
	stream() << std::setprecision(9) << t;
	for (lnet::motifs::NodeMotifSet::const_iterator it = nodeMotifs_.begin(); it
			!= nodeMotifs_.end(); ++it)
		stream() << sep << net_.numberOfNodes(*it);
	for (lnet::motifs::LinkMotifSet::const_iterator it = linkMotifs_.begin(); it
			!= linkMotifs_.end(); ++it)
		stream() << sep << net_.numberOfLinks(net_.linkStateCalculator()(*it));

	addTripleOutput(sep);
	stream() << "\n";
}

template<class _Network>
void TimeSeriesOutput<_Network>::doWriteHeader()
{
	const char tab = '\t';
	stream() << commentChar() << 't';
	for (lnet::motifs::NodeMotifSet::const_iterator it = nodeMotifs_.begin(); it
			!= nodeMotifs_.end(); ++it)
		stream() << tab << *it;
	for (lnet::motifs::LinkMotifSet::const_iterator it = linkMotifs_.begin(); it
			!= linkMotifs_.end(); ++it)
		stream() << tab << *it;

	addTripleHeader(tab);
	stream() << "\n";
}

template<>
void TimeSeriesOutput<lnet::TripleMultiNetwork>::addTripleHeader(const char sep)
{
	for (lnet::motifs::TripleMotifSet::const_iterator it =
			tripleMotifs_.begin(); it != tripleMotifs_.end(); ++it)
		stream() << sep << *it;
}

template<>
void TimeSeriesOutput<lnet::TripleNetwork>::addTripleHeader(const char sep)
{
	for (lnet::motifs::TripleMotifSet::const_iterator it =
			tripleMotifs_.begin(); it != tripleMotifs_.end(); ++it)
		stream() << sep << *it;
}

template<>
void TimeSeriesOutput<lnet::TripleMultiNetwork>::addTripleOutput(const char sep)
{
	for (lnet::motifs::TripleMotifSet::const_iterator it =
			tripleMotifs_.begin(); it != tripleMotifs_.end(); ++it)
		stream() << sep << net_.numberOfTriples(
				net_.getTripleStateCalculator()(*it));
}

template<>
void TimeSeriesOutput<lnet::TripleNetwork>::addTripleOutput(const char sep)
{
	for (lnet::motifs::TripleMotifSet::const_iterator it =
			tripleMotifs_.begin(); it != tripleMotifs_.end(); ++it)
		stream() << sep << net_.numberOfTriples(
				net_.getTripleStateCalculator()(*it));
}

#endif /* TIMESERIESOUTPUT_H_ */
