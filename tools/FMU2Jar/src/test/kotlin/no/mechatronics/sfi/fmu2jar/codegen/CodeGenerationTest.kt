package no.ntnu.ihb.fmu2jar.codegen

import no.ntnu.ihb.fmi4j.modeldescription.parser.ModelDescriptionParser
import no.ntnu.ihb.fmu2jar.util.TEST_FMUs
import no.ntnu.ihb.fmu2jar.util.currentOS
import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test
import org.junit.jupiter.api.condition.EnabledIfEnvironmentVariable
import org.slf4j.Logger
import org.slf4j.LoggerFactory
import java.io.File

@EnabledIfEnvironmentVariable(named = "TEST_FMUs", matches = ".*")
class CodeGenerationTest {

    companion object {
        private val LOG: Logger = LoggerFactory.getLogger(CodeGenerationTest::class.java)
    }

    @Test
    fun generateBody() {

        val file = File(TEST_FMUs,
                "2.0/cs/$currentOS/20sim/4.6.4.8004/" +
                        "ControlledTemperature/ControlledTemperature.fmu")
        Assertions.assertTrue(file.exists())
        ModelDescriptionParser.parse(file).also {
            LOG.info( CodeGenerator(it).generateBody() )
        }

    }

}