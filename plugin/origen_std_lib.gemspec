# coding: utf-8
config = File.expand_path('../config', __FILE__)
require "#{config}/version"

Gem::Specification.new do |spec|
  spec.name          = "origen_std_lib"
  spec.version       = OrigenStdLib::VERSION
  spec.authors       = ["Stephen McGinty"]
  spec.email         = ["stephen.mcginty@nxp.com"]
  spec.summary       = "Test program interface drivers for the Origen tester standard library"
  #spec.homepage      = "http://origen-sdk.org/std_lib"

  spec.required_ruby_version     = '>= 2'
  spec.required_rubygems_version = '>= 1.8.11'

  # Only the files that are hit by these wildcards will be included in the
  # packaged gem, the default should hit everything in most cases but this will
  # need to be added to if you have any custom directories
  spec.files         = Dir["lib/**/*.rb", "templates/**/*", "config/**/*.rb",
                           "bin/*", "lib/tasks/**/*.rake", "pattern/**/*.rb",
                           "program/**/*.rb", "stdlib/**/*"
                          ]
  spec.executables   = []
  spec.require_paths = ["lib"]

  # Add any gems that your plugin needs to run within a host application
  spec.add_runtime_dependency "origen", ">= 0.7.34"
  spec.add_runtime_dependency "origen_testers", ">= 0.8.1"
end
