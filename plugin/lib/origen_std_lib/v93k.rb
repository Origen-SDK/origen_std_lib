module OrigenStdLib
  def self.add_v93k_std_lib(i)
    i.add_tml :origen,
              class_name:            'origen',

              functional_test:       {
              },

              dc_measurement:        {
                class_name:       'DCMeasurement',

                apply_shutdown:   [:integer, 1],
                shutdown_pattern: [:string, nil],
                check_shutdown:   [:integer, 1],
                measure:          [:string, 'VOLT', %w(VOLT CURR v i voltage current)],
                settling_time:    [:double, 0],
                pin:              [:string, nil],
                force_value:      [:double, 0],
                i_range:          [:integer, nil],
                badc:             [:integer, 0],
                methods:          {
                  finalize: lambda  do |tm|
                    # Clean up the measurement type parameter
                    if tm.measure == 'v' || tm.measure == 'voltage'
                      tm.measure = 'VOLT'
                    elsif tm.measure == 'i' || tm.measure == 'current'
                      tm.measure = 'CURR'
                    end
                    tm.measure = tm.measure.upcase # In case 'volt' entered

                    # Calculate an autorange if not supplied for current meas
                    if tm.measure == 'CURR' && !tm.i_range
                      max = [tm.limits.lo.to_f.abs, tm.limits.hi.to_f.abs].max
                      tm.i_range = max
                    end
                  end
                }
              },

              frequency_measurement: {
                period_based: [:integer, 0],
                pin:          [:string, nil],
                samples:      [:integer, 2000],
                period_in_ns: [:integer, nil]
              },

              record_bin:            {
                bin:     [:integer, nil],
                softbin: [:integer, nil]
              },

              apply_bin:             {
              }
  end

  # Record the given bin / softbin
  def record_bin(options = {})
    if tester.v93k?
      n = 'delayed'
      n += "_bin#{options[:bin]}" if options[:bin]
      n += "_sbin#{options[:softbin]}" if options[:softbin]
      i = test_suites.add(n)
      i.test_method = test_methods.origen.record_bin(options)
      # The bins are included here to insert them into the flow, which is required by SMT to
      # allow them to be applied later.
      # However the bins really refer to some parent test, so we tell the TestIds plugin (if
      # it is being used), not to consider them attached to this record bin test suite.
      test(i, options.merge(test_ids: :notrack))
    end
  end

  # Bins out the device if a delayed bin has been set
  def apply_bin(options = {})
    if tester.v93k?
      i = test_suites.add('ApplyBin')
      i.test_method = test_methods.origen.apply_bin
      test(i, options.merge(on_fail: { render: 'multi_bin;' }, continue: true))
    end
  end
end
