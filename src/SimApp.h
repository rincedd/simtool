/**
 * @file SimApp.h
 * @date 12.11.2009
 * @author gerd
 */

#ifndef SIMAPP_H_
#define SIMAPP_H_

#include "output/Outputter.h"
#include <string>
#include <fstream>
#include <boost/ptr_container/ptr_vector.hpp>

class SimApp
{
public:
	SimApp(int argc, char** argv);
	virtual ~SimApp();
	virtual int exec() { return 0; }
	void setAppName(std::string name);
	void setAppName(const char* name);
	std::string appName() const;

protected:
	std::ofstream& openOutputStream(std::string filename);
	void registerOutput(IntervalOutput* output);
	void writeHeaders();
	void output(double t);

private:
	std::string appName_;
	Outputter outputter_;
	typedef boost::ptr_vector<std::ofstream> ofstream_ptr_v;
	ofstream_ptr_v outStreams_;
};

inline void SimApp::setAppName(std::string name)
{
	appName_ = name;
}

inline void SimApp::setAppName(const char* name)
{
	appName_ = name;
}

inline std::string SimApp::appName() const
{
	return appName_;
}

inline void SimApp::writeHeaders()
{
	outputter_.writeHeaders();
}

inline void SimApp::output(const double t)
{
	outputter_.output(t);
}

#endif /* SIMAPP_H_ */
