/*
 * The MIT License
 *
 * Copyright 2017-2018 Norwegian University of Technology
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING  FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

package no.mechatronics.sfi.fmi4j

import no.mechatronics.sfi.fmi4j.codegen.CodeGenerator
import no.mechatronics.sfi.fmi4j.modeldescription.parser.ModelDescriptionParser
import org.gradle.api.Plugin
import org.gradle.api.Project
import org.gradle.api.Task
import org.gradle.api.artifacts.DependencyResolutionListener
import org.gradle.api.artifacts.ResolvableDependencies
import org.gradle.api.plugins.JavaPluginConvention
import org.gradle.api.tasks.compile.JavaCompile
import org.gradle.kotlin.dsl.getting
import org.gradle.kotlin.dsl.invoke
import org.gradle.kotlin.dsl.the
import java.io.File

open class FmuPluginExtension(
        project: Project
) {

    var configurationName: String = "implementation"
    var version: String = "0.10.1"

}

/**
 * @author Lars Ivar Hatledal
 */
open class FmuPlugin : Plugin<Project> {

    override fun apply(target: Project) {

        target.run {

            val srcDir = File(projectDir, "src/main/resources/fmus")
            val outDir = File(buildDir, "generated/fmus")

            the<JavaPluginConvention>().sourceSets {
                    getByName("main") {
                        java.srcDir(outDir)
                    }
            }

            val fmi4j = extensions.create("fmi4j", FmuPluginExtension::class.java, this)

            gradle.addListener(object: DependencyResolutionListener {
                override fun beforeResolve(_dependencies: ResolvableDependencies) {
                    dependencies.add(fmi4j.configurationName, "no.mechatronics.sfi.fmi4j:fmi-import:${fmi4j.version}")
                    gradle.removeListener(this)
                }

                override fun afterResolve(dependencies: ResolvableDependencies?) {

                }
            })

            tasks {

                val compileJava by tasks.getting(JavaCompile::class)

                "generateSources"(Task::class) {
                    compileSources(srcDir, outDir)
                    compileJava.dependsOn(this)

                }

            }
        }

    }

    private fun compileSources(srcDir: File, outputDir: File) {

        if (!srcDir.exists()) {
            throw IllegalArgumentException("No such file: '$srcDir'")
        }

        srcDir.listFiles().forEach { file ->

            if (file.name.toLowerCase().endsWith(".fmu")) {
                val md = ModelDescriptionParser.parse(file)
                CodeGenerator(md).apply {
                    generateBody().also {
                        File(outputDir, "no/mechatronics/sfi/fmi4j/${md.modelName}.java").apply {
                            parentFile.mkdirs()
                            writeText(it)
                        }
                    }
                }
            }

        }

    }

}



