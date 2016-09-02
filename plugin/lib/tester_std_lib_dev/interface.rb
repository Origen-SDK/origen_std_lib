require 'origen_std_lib'
module TesterStdLibDev
  class Interface
    include OrigenTesters::ProgramGenerators
    include OrigenStdLib

    def func(name)
      i = test_suites.add(name)
      i.test_method = test_methods.origen.functional_test
      flow.test(i)
    end

    def dc_meas(name)
      i = test_suites.add(name)
      tm = test_methods.origen.dc_measurement
      tm.apply_shutdown = 1
      tm.shutdown_pattern = 'some_pattern'
      tm.measure = 'CURR'
      tm.settling_time = 100
      tm.pin = 'SOME_PIN'
      tm.force_value = 10.4
      tm.i_range = 1000
      tm.limits.lo = 1
      tm.limits.hi = 2
      i.test_method = tm
      flow.test(i)
    end

    def freq_meas(name)
      i = test_suites.add(name)
      tm = test_methods.origen.frequency_measurement
      tm.period_based = 1
      tm.pin = 'PINA'
      tm.samples = 4000
      tm.period_in_ns = 40
      tm.limits.lo = 1.Mhz
      tm.limits.hi = 2.Mhz
      i.test_method = tm
      flow.test(i)
    end
  end
end
