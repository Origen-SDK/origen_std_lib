require 'origen'
require_relative '../config/application.rb'
require 'origen_testers'
module OrigenStdLib
  # THIS FILE SHOULD ONLY BE USED TO LOAD RUNTIME DEPENDENCIES
  # If this plugin has any development dependencies (e.g. dummy DUT or other models that are only used
  # for testing), then these should be loaded from config/boot.rb

  # Example of how to explicitly require a file
  # require "origen_std_lib/my_file"

  # Load all files in the lib/origen_std_lib directory.
  # Note that there is no problem from requiring a file twice (Ruby will ignore
  # the second require), so if you have a file that must be required first, then
  # explicitly require it up above and then let this take care of the rest.
  Dir.glob("#{File.dirname(__FILE__)}/origen_std_lib/**/*.rb").sort.each do |file|
    require file
  end

  def initialize(*args)
    super
    if tester.v93k?
      OrigenStdLib.add_v93k_std_lib(self)
    end
  end
end
