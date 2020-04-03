hp93000,testflow,0.1
language_revision = 1;

testmethodparameters

tm_1:
  "forcePass" = "0";
  "onFailFlag" = "";
  "onPassFlag" = "";
  "port" = "";
  "testName" = "test1_916CE79";
tm_2:
  "applyShutdown" = "1";
  "badc" = "0";
  "checkShutdown" = "1";
  "forcePass" = "0";
  "forceValue" = "10.4";
  "iRange" = "1000";
  "measure" = "CURR";
  "onFailFlag" = "";
  "onPassFlag" = "";
  "pin" = "SOME_PIN";
  "port" = "";
  "settlingTime" = "100";
  "shutdownPattern" = "some_pattern";
  "testName" = "test2_916CE79";
tm_3:
  "forcePass" = "0";
  "onFailFlag" = "";
  "onPassFlag" = "";
  "periodBased" = "1";
  "periodInNs" = "40";
  "pin" = "PINA";
  "port" = "";
  "samples" = "4000";
  "testName" = "test3_916CE79";

end
-----------------------------------------------------------------
testmethodlimits

tm_1:
  "test1_916CE79" = "":"NA":"":"NA":"":"":"";
tm_2:
  "test2_916CE79" = "1":"GE":"2":"LE":"":"":"0";
tm_3:
  "test3_916CE79" = "1000000":"GE":"2000000":"LE":"":"":"0";

end
-----------------------------------------------------------------
testmethods

tm_1:
  testmethod_class = "origen.FunctionalTest";
tm_2:
  testmethod_class = "origen.DCMeasurement";
tm_3:
  testmethod_class = "origen.FrequencyMeasurement";

end
-----------------------------------------------------------------
test_suites

test1_916CE79:
  local_flags = output_on_pass, output_on_fail, value_on_pass, value_on_fail, per_pin_on_pass, per_pin_on_fail;
  override = 1;
  override_testf = tm_1;
  site_control = "parallel:";
  site_match = 2;
test2_916CE79:
  local_flags = output_on_pass, output_on_fail, value_on_pass, value_on_fail, per_pin_on_pass, per_pin_on_fail;
  override = 1;
  override_testf = tm_2;
  site_control = "parallel:";
  site_match = 2;
test3_916CE79:
  local_flags = output_on_pass, output_on_fail, value_on_pass, value_on_fail, per_pin_on_pass, per_pin_on_fail;
  override = 1;
  override_testf = tm_3;
  site_control = "parallel:";
  site_match = 2;

end
-----------------------------------------------------------------
test_flow

  {
    run(test1_916CE79);
    run(test2_916CE79);
    run(test3_916CE79);

  }, open,"PRB1",""

end
-----------------------------------------------------------------
binning
end
-----------------------------------------------------------------
oocrule


end
-----------------------------------------------------------------
context


end
-----------------------------------------------------------------
hardware_bin_descriptions


end
-----------------------------------------------------------------
