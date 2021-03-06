package no.ntnu.ihb.fmi4j.modeldescription.gson

import com.google.gson.GsonBuilder
import no.ntnu.ihb.fmi4j.TestFMUs
import no.ntnu.ihb.fmi4j.modeldescription.ModelDescriptionImpl
import org.junit.jupiter.api.Test
import org.junit.jupiter.api.condition.EnabledIfEnvironmentVariable
import org.slf4j.Logger
import org.slf4j.LoggerFactory

@EnabledIfEnvironmentVariable(named = "TEST_FMUs", matches = ".*")
class GsonTest {

    private companion object {
        val LOG: Logger = LoggerFactory.getLogger(Test::class.java)
    }

    @Test
    fun test() {

        val modelDescription = no.ntnu.ihb.fmi4j.TestFMUs.fmi20().cs()
                .vendor("MapleSim").version("2017")
                .modelDescription("ControlledTemperature")

        GsonBuilder()
                .setPrettyPrinting()
                .create().also { gson ->

                    gson.toJson(modelDescription).also { json ->
                        LOG.info("$json")
                        gson.fromJson(json, ModelDescriptionImpl::class.java).also { md ->
                            LOG.info("${md.modelVariables}")
                        }

                    }

                }

    }

}