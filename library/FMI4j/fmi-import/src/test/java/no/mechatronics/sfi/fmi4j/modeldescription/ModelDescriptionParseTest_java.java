package no.mechatronics.sfi.fmi4j.modeldescription;

import no.mechatronics.sfi.fmi4j.TestUtils;
import no.mechatronics.sfi.fmi4j.common.OSUtil;
import no.mechatronics.sfi.fmi4j.modeldescription.parser.ModelDescriptionParser;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.condition.EnabledIfEnvironmentVariable;

import java.io.File;

@EnabledIfEnvironmentVariable(named = "TEST_FMUs", matches = ".*")
public class ModelDescriptionParseTest_java {

    @Test
    public void test() {

        File fmuFile = new File(TestUtils.getTEST_FMUs(),
                "FMI_2.0/CoSimulation/" + OSUtil.getCurrentOS() +
                        "/20sim/4.6.4.8004/ControlledTemperature/ControlledTemperature.fmu");
        Assertions.assertTrue(fmuFile.exists());

        Assertions.assertNotNull(ModelDescriptionParser.parse(fmuFile).asCoSimulationModelDescription());

        String xml = ModelDescriptionParser.extractModelDescriptionXml(fmuFile);
        Assertions.assertNotNull(ModelDescriptionParser.parse(xml).asCoSimulationModelDescription());

    }

}
