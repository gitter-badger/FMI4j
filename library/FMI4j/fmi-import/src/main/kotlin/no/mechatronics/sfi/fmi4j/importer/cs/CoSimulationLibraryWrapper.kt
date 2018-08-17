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

package no.mechatronics.sfi.fmi4j.importer.cs

import no.mechatronics.sfi.fmi4j.importer.FmiLibraryWrapper
import no.mechatronics.sfi.fmi4j.importer.jni.*


/**
 *
 * @author Lars Ivar Hatledal
 */
class CoSimulationLibraryWrapper(
        c: Long,
        library: IFmiLibrary
) : FmiLibraryWrapper(c, library) {

    /**
     * Extension method
     */
    fun getMaxStepSize(): Double {
        return DoubleByReference().let {
            updateStatus(library.getMaxStepSize(c, it))
            it.value
        }
    }

    /**
     * @see FmiCoSimulationLibrary.fmi2SetRealInputDerivatives
     */
    fun setRealInputDerivatives(vr: IntArray, order: IntArray, value: DoubleArray): no.mechatronics.sfi.fmi4j.common.FmiStatus {
        return updateStatus(library.setRealInputDerivatives(c, vr, order, value))
    }

    /**
     * @see FmiCoSimulationLibrary.fmi2GetRealOutputDerivatives
     */
    fun getRealOutputDerivatives(vr: IntArray, order: IntArray, value: DoubleArray): no.mechatronics.sfi.fmi4j.common.FmiStatus {
        return updateStatus(library.getRealOutputDerivatives(c, vr, order, value))
    }

    /**
     * @see FmiCoSimulationLibrary.fmi2DoStep
     */
    fun doStep(t: Double, dt: Double, noSetFMUStatePriorToCurrent: Boolean): no.mechatronics.sfi.fmi4j.common.FmiStatus {
        return updateStatus(library.step(c, t, dt, noSetFMUStatePriorToCurrent))
    }

    /**
     * @see FmiCoSimulationLibrary.fmi2CancelStep
     */
    fun cancelStep(): no.mechatronics.sfi.fmi4j.common.FmiStatus {
        return (updateStatus(library.cancelStep(c)))
    }

    /**
     * @see FmiCoSimulationLibrary.fmi2GetStatus
     */
    fun getStatus(s: FmiStatusKind): no.mechatronics.sfi.fmi4j.common.FmiStatus {
        return IntByReference().let {
            updateStatus(library.getIntegerStatus(c, s.code, it))
            no.mechatronics.sfi.fmi4j.common.FmiStatus.valueOf(it.value)
        }
    }

    /**
     * @see FmiCoSimulationLibrary.fmi2GetRealStatus
     */
    fun getRealStatus(s: FmiStatusKind): Double {
        return DoubleByReference().let {
            updateStatus(library.getRealStatus(c, s.code, it))
            it.value
        }

    }

    /**
     * @see FmiCoSimulationLibrary.fmi2GetIntegerStatus
     */
    fun getIntegerStatus(s: FmiStatusKind): Int {
        return IntByReference().let {
            updateStatus(library.getIntegerStatus(c, s.code, it))
            it.value
        }

    }

    /**
     * @see FmiCoSimulationLibrary.fmi2GetBooleanStatus
     */
    fun getBooleanStatus(s: FmiStatusKind): Boolean {
        return BooleanByReference().let {
            updateStatus(library.getBooleanStatus(c, s.code, it))
            it.value
        }
    }

    /**
     * @see FmiCoSimulationLibrary.fmi2GetStringStatus
     */
    fun getStringStatus(s: FmiStatusKind): String {
        return StringByReference().let {
            updateStatus((library.getStringStatus(c, s.code, it)))
            it.value
        }

    }

}
