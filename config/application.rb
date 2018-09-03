require 'origen'
class OrigenStdLibApplication < Origen::Application

  # See http://origen-sdk.org/origen/api/Origen/Application/Configuration.html
  # for a full list of the configuration options available

  # These attributes should never be changed, the duplication here will be resolved in future
  # by condensing these attributes that do similar things
  self.name       = "origen_std_lib"
  self.namespace  = "OrigenStdLib"
  config.name     = "origen_std_lib"
  config.initials = "OrigenStdLib"
  # Change this to point to the revision control repository for this plugin
  config.rc_url   = "git@github.com:Origen-SDK/origen_std_lib.git"
  config.release_externally = true

  # To enable deployment of your documentation to a web server (via the 'origen web'
  # command) fill in these attributes.
  config.web_directory = "git@github.com:Origen-SDK/Origen-SDK.github.io.git/std_lib"
  config.web_domain = "http://origen-sdk.org/std_lib"

  # When false Origen will be less strict about checking for some common coding errors,
  # it is recommended that you leave this to true for better feedback and easier debug.
  # This will be the default setting in Origen v3.
  config.strict_errors = true

  # See: http://origen-sdk.org/origen/latest/guides/utilities/lint/
  config.lint_test = {
    # Require the lint tests to pass before allowing a release to proceed
    run_on_tag: true,
    # Auto correct violations where possible whenever 'origen lint' is run
    auto_correct: true, 
    # Limit the testing for large legacy applications
    #level: :easy,
    # Run on these directories/files by default
    #files: ["lib", "config/application.rb"],
  }

  config.semantically_version = true

  def before_release_gem
    # Remove all untracked files, e.g. compiled binaries
    system "git clean -f -d -x #{Origen.root}/src"
    system "git clean -f -d #{Origen.root}/src"
  end

  def after_web_site_compile(options={})
    if options[:api]
      f = File.join(Origen.root, "src", "advantest", "smt7")
      Dir.chdir f do
        system "doxygen .doxygen"
        d = "#{Origen.root}/web/output"
        FileUtils.mkdir_p(d)
        FileUtils.mv "html", "#{d}/v93k"
      end
    end
  end

  # An example of how to set application specific LSF parameters
  #config.lsf.project = "msg.te"
  
  # An example of how to specify a prefix to add to all generated patterns
  #config.pattern_prefix = "nvm"

  # An example of how to add header comments to all generated patterns
  #config.pattern_header do
  #  cc "This is a pattern created by the example origen application"
  #end

  # By default all generated output will end up in ./output.
  # Here you can specify an alternative directory entirely, or make it dynamic such that
  # the output ends up in a setup specific directory. 
  #config.output_directory do
  #  "#{Origen.root}/output/#{$dut.class}"
  #end

  # Similarly for the reference files, generally you want to setup the reference directory
  # structure to mirror that of your output directory structure.
  #config.reference_directory do
  #  "#{Origen.root}/.ref/#{$dut.class}"
  #end

  def before_release_tag(identifier, note, type, selector, options)
    v = Origen::VersionString.new(identifier)
    # Update the version in the C code
    f = File.join(Origen.root, "src", "advantest", "smt7", "origen", "origen.hpp")
    data = File.read(f) 
    filtered_data = data.sub(/#define ORIGEN_VERSION \"\d+\.\d+\.\d+\"/, "#define ORIGEN_VERSION \"#{v}\"") 
    File.open(f, "w") do |f|
      f.write(filtered_data)
    end
    Origen.app.rc.checkin f, comment: "Wrote new version in C++ code"
  end
 
  # This will automatically deploy your documentation after every tag
  def after_release_email(tag, note, type, selector, options)
    command = "origen web compile --remote --api"
    Dir.chdir Origen.root do
      system command
    end
  end

  # Ensure that all tests pass before allowing a release to continue
  def validate_release
    if !system("origen examples")
      puts "Sorry but you can't release with failing tests, please fix them and try again."
      exit 1
    else
      puts "All tests passing, proceeding with release process!"
    end
  end

  # To enabled source-less pattern generation create a class (for example PatternDispatcher)
  # to generate the pattern. This should return false if the requested pattern has been
  # dispatched, otherwise Origen will proceed with looking up a pattern source as normal.
  #def before_pattern_lookup(requested_pattern)
  #  PatternDispatcher.new.dispatch_or_return(requested_pattern)
  #end

  # If you use pattern iterators you may come across the case where you request a pattern
  # like this:
  #   origen g example_pat_b0.atp
  #
  # However it cannot be found by Origen since the pattern name is actually example_pat_bx.atp
  # In the case where the pattern cannot be found Origen will pass the name to this translator
  # if it exists, and here you can make any substitutions to help Origen find the file you 
  # want. In this example any instances of _b\d, where \d means a number, are replaced by
  # _bx.
  #config.pattern_name_translator do |name|
  #  name.gsub(/_b\d/, "_bx")
  #end
end
