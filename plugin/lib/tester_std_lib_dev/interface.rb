require 'origen_std_lib'
module TesterStdLibDev
  class Interface
    include OrigenTesters::ProgramGenerators
    include OrigenStdLib

    def func(name)
    end

    def dc_meas(name)
    end

    def freq_meas(name)
    end
  end
end
