package no.ntnu.ihb.fmi4j.fmudriver

import no.ntnu.ihb.fmi4j.common.currentOS
import no.ntnu.ihb.fmu2jar.TEST_FMUs
import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test
import org.junit.jupiter.api.condition.EnabledIfEnvironmentVariable
import org.slf4j.Logger
import org.slf4j.LoggerFactory
import java.io.File

@EnabledIfEnvironmentVariable(named = "TEST_FMUs", matches = ".*")
class TestControlledTemperature_CS {

    private companion object {
        val LOG: Logger = LoggerFactory.getLogger(TestControlledTemperature_CS::class.java)
    }

    @Test
    fun test() {

        val path = "$TEST_FMUs/2.0/cs/$currentOS/" +
                "20sim/4.6.4.8004/ControlledTemperature/ControlledTemperature.fmu"
        Assertions.assertTrue(File(path).exists())

        val args = arrayOf(
                "-f", "\"$path\"",
                "-dt", "1E-4",
                "-stop", "5",
                "Temperature_Reference", "Temperature_Room"
        )

        Cmd.main(args)

        File("ControlledTemperature_out.csv").apply {
            if (exists()) {
                delete()
            }
        }

    }

}