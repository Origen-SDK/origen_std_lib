#ifndef ORIGEN_TEST_METHOD_FREQUENCY_MEASUREMENT_INCLUDED
#define ORIGEN_TEST_METHOD_FREQUENCY_MEASUREMENT_INCLUDED

#include "base.hpp"
#include "mapi.hpp"
#include "rdi.hpp"

using namespace std;

namespace Origen {
namespace TestMethod {

class FrequencyMeasurement : public Base {
  void serialProcessing(int site);

  int _periodBased;
  string _pin;
  string _port;
  int _samples;
  int _periodInNs;
  int _processResults;

 public:
  FrequencyMeasurement();
  virtual ~FrequencyMeasurement();
  void SMC_backgroundProcessing();

  FrequencyMeasurement& periodBased(int v);
  FrequencyMeasurement& pin(string v);
  FrequencyMeasurement& port(string v);
  FrequencyMeasurement& samples(int v);
  FrequencyMeasurement& periodInNs(int v);
  FrequencyMeasurement& processResults(int v);

 protected:
  typedef FrequencyMeasurement super;
  FrequencyMeasurement& getThis();
  void _setup();
  void _execute();

  string label;
  vector<int> funcResults;
};
}
}
#endif
