module OrigenStdLib
  def self.add_v93k_std_lib(i)
    i.add_tml :origen,
              class_name:            'origen_tml',

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
              }
  end
end
