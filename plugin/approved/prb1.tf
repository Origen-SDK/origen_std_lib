hp93000,testflow,0.1
language_revision = 1;
 
information
 
 
end
--------------------------------------------------
implicit_declarations

end
-----------------------------------------------------------------
testmethodparameters
tm_2:
  "applyShutdown" = "1";
  "shutdownPattern" = "some_pattern";
  "measure" = "CURR";
  "settlingTime" = "100";
  "pin" = "SOME_PIN";
  "forceValue" = "10.4";
  "iRange" = "1000";
tm_3:
  "periodBased" = "1";
  "pin" = "PINA";
  "samples" = "4000";
  "periodInNs" = "40";
end
--------------------------------------------------
testmethodlimits
tm_1:
  "Functional" = "":"NA":"":"NA":"":"":"";
tm_2:
  "Functional" = "1":"GE":"2":"LE":"":"":"0";
tm_3:
  "Functional" = "1000000":"GE":"2000000":"LE":"":"":"0";
end
--------------------------------------------------
testmethods
tm_1:
  testmethod_class = "origen_tml.FunctionalTest";
tm_2:
  testmethod_class = "origen_tml.DCMeasurement";
tm_3:
  testmethod_class = "origen_tml.FrequencyMeasurement";
end
--------------------------------------------------
test_suites
test1:
  override = 1;
 override_testf = tm_1;
local_flags  = output_on_pass, output_on_fail, value_on_pass, value_on_fail, per_pin_on_pass, per_pin_on_fail;
 site_match = 2;
 site_control = "parallel:";
test2_990CF69:
  override = 1;
 override_testf = tm_2;
local_flags  = output_on_pass, output_on_fail, value_on_pass, value_on_fail, per_pin_on_pass, per_pin_on_fail;
 site_match = 2;
 site_control = "parallel:";
test3_990CF69:
  override = 1;
 override_testf = tm_3;
local_flags  = output_on_pass, output_on_fail, value_on_pass, value_on_fail, per_pin_on_pass, per_pin_on_fail;
 site_match = 2;
 site_control = "parallel:";
end
--------------------------------------------------
test_flow
{
  run(test1);
  run(test2_990CF69);
  run(test3_990CF69);
}, open,"PRB1", ""
end
-------------------------------------------------
binning
otherwise bin = "db", "", , bad, noreprobe, red, , not_over_on;
end
-------------------------------------------------
context
 
end
--------------------------------------------------
hardware_bin_descriptions
end
