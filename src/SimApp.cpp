/**
 * @file SimApp.cpp
 * @date 12.11.2009
 * @author gerd
 */

#include "SimApp.h"

using namespace std;

SimApp::SimApp(int argc, char** argv)
{
}

SimApp::~SimApp()
{
	// actually, this is not necessary
	for (ofstream_ptr_v::iterator i = outStreams_.begin(); i
			!= outStreams_.end(); ++i)
		i->close();
}

ofstream& SimApp::openOutputStream(string filename)
{
	auto_ptr<ofstream> o(new ofstream(filename.c_str()));
	if (o->bad())
	{
		o->close();
		throw("Could not open " + filename + " for writing!\n");
	}
	outStreams_.push_back(o);
	return outStreams_.back();
}

void SimApp::registerOutput(IntervalOutput* output)
{
	outputter_.addOutput(output);
}
