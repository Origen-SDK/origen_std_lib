# This file is used to boot your plugin when it is running in standalone mode
# from its own workspace - i.e. when the plugin is being developed.
#
# It will not be loaded when the plugin is imported by a 3rd party app - in that
# case only lib/tester_std_lib.rb is loaded.
#
# Therefore this file can be used to load anything extra that you need to boot
# the development environment for this app. For example, this is typically used
# to load some additional test classes to use your plugin APIs so that they can
# be tested and/or interacted with in the console.
require "tester_std_lib"

module TesterStdLibDev
  # Example of how to explicitly require a file
  # require "tester_std_lib_dev/my_file"
    
  # Load all files in the lib/tester_std_lib_dev directory.
  # Note that there is no problem from requiring a file twice (Ruby will ignore
  # the second require), so if you have a file that must be required first, then
  # explicitly require it up above and then let this take care of the rest.
  Dir.glob("#{File.dirname(__FILE__)}/../lib/tester_std_lib_dev/**/*.rb").sort.each do |file|
    require file
  end
end
