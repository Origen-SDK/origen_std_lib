module OrigenStdLib
  def self.add_v93k_std_lib(i)
    i.add_tml :origen,
              class_name:            'origen_tml',

              functional_test:       {
              },

              dc_measurement:        {
                class_name:       'DCMeasurement',

                apply_shutdown:   [:integer, 0],
                shutdown_pattern: [:string, nil],
                measure:          [:string, 'VOLT', %w(VOLT CURR)],
                settling_time:    [:double, 0],
                pin:              [:string, nil],
                force_value:      [:double, 0],
                i_range:          [:integer, 0]
              },

              frequency_measurement: {
                period_based: [:integer, 0],
                pin:          [:string, nil],
                samples:      [:integer, 2000],
                period_in_ns: [:integer, nil]
              }
  end
end
