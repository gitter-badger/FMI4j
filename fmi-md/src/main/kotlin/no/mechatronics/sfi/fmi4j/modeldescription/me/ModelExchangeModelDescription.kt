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

package no.mechatronics.sfi.fmi4j.modeldescription.me

import no.mechatronics.sfi.fmi4j.modeldescription.*
import no.mechatronics.sfi.fmi4j.modeldescription.misc.SourceFile
import java.io.Serializable

/**
 * @author Lars Ivar Hatledal
 */
interface ModelExchangeModelDescription : ModelDescription {

    /**
     * The (fixed) number of event indicators for an FMU based on FMI for
     * Model Exchange.
     */
    val numberOfEventIndicators: Int

    /**
     * If true, function
     * fmi2CompletedIntegratorStep need not to
     * be called (which gives a slightly more efficient
     * integration). If it is called, it has no effect.
     * If false (the default), the function must be called
     * after every completed integrator step, see
     * section 3.2.2.
     */
    val completedIntegratorStepNotNeeded: Boolean
}

/**
 *
 * @author Lars Ivar Hatledal laht@ntnu.no.
 */
class ModelExchangeModelDescriptionImpl(
        private val modelDescription: ModelDescriptionImpl,
        private val me: ModelExchangeData
) : SimpleModelDescription by modelDescription, ModelExchangeModelDescription, Serializable {

    /**
     * @inheritDoc
     */
    override val numberOfEventIndicators: Int
        get() = modelDescription.numberOfEventIndicators

    /**
     * @inheritDoc
     */
    override val modelIdentifier: String
        get() = me.modelIdentifier

    /**
     * @inheritDoc
     */
    override val needsExecutionTool: Boolean
        get() = me.needsExecutionTool

    /**
     * @inheritDoc
     */
    override val canBeInstantiatedOnlyOncePerProcess: Boolean
        get() = me.canBeInstantiatedOnlyOncePerProcess

    /**
     * @inheritDoc
     */
    override val canNotUseMemoryManagementFunctions: Boolean
        get() = me.canNotUseMemoryManagementFunctions

    /**
     * @inheritDoc
     */
    override val canGetAndSetFMUstate: Boolean
        get() = me.canGetAndSetFMUstate

    /**
     * @inheritDoc
     */
    override val canSerializeFMUstate: Boolean
        get() = me.canSerializeFMUstate

    /**
     * @inheritDoc
     */
    override val providesDirectionalDerivative: Boolean
        get() = me.providesDirectionalDerivative

    /**
     * @inheritDoc
     */
    override val sourceFiles: List<SourceFile>
        get() = me.sourceFiles

    /**
     * @inheritDoc
     */
    override val completedIntegratorStepNotNeeded: Boolean
        get() = me.completedIntegratorStepNotNeeded

}
