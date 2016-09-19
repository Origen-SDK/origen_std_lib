require 'origen'
require_relative '../config/application.rb'
require 'origen_testers'
module OrigenStdLib
  # THIS FILE SHOULD ONLY BE USED TO LOAD RUNTIME DEPENDENCIES
  # If this plugin has any development dependencies (e.g. dummy DUT or other models that are only used
  # for testing), then these should be loaded from config/boot.rb
  require 'origen_std_lib/v93k'

  def includes_origen_std_lib?
    true
  end

  # Injects the library into the test program interface at the start of
  # the flow
  class PersistentCallbackHandlers
    include Origen::PersistentCallbacks

    def on_flow_start(options)
      if Origen.interface.respond_to?(:includes_origen_std_lib?) && tester.v93k?
        OrigenStdLib.add_v93k_std_lib(Origen.interface)
      end
    end
  end
  # Instantiate an instance of this class immediately when this file is loaded, this object will
  # then listen for the remainder of the Origen thread
  PersistentCallbackHandlers.new
end
