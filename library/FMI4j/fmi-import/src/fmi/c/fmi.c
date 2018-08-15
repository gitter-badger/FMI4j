
#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include "fmi2FunctionTypes.h"

#if defined(_MSC_VER) || defined(WIN32) || defined(__MINGW32__)
#include <windows.h>
#define DLL_HANDLE HANDLE
#else
#define DLL_HANDLE void*
#include <dlfcn.h>
#endif

#ifdef WIN32
#define function_ptr FARPROC
#else
typedef void* function_ptr; 
#endif

DLL_HANDLE handle;

function_ptr* load_function(const char* function_name) {
#ifdef WIN32
	return  (function_ptr) GetProcAddress(handle, function_name);
#else
	return dlsym(handle, function_name);
#endif
}

void logger(void* fmi2ComponentEnvironment, fmi2String instance_name, fmi2Status status, fmi2String category, fmi2String message, ...) {
     printf("instanceName = %s, category = %s: %s\n", instance_name, category, message);
}

fmi2CallbackFunctions callback = {
    .logger = logger,
    .allocateMemory = calloc,
    .freeMemory = free,
    .stepFinished = NULL,
    .componentEnvironment = NULL
};

JNIEXPORT jboolean JNICALL Java_no_mechatronics_sfi_fmi4j_jni_FmiLibrary_load(JNIEnv *env, jobject obj, jstring lib_name) {

    const char* _lib_name = (*env)->GetStringUTFChars(env, lib_name, 0);
    #ifdef WIN32
    	handle = LoadLibrary(_lib_name);
    #else
    	handle = dlopen(_lib_name, RTLD_NOW|RTLD_GLOBAL);
    #endif
    (*env)->ReleaseStringUTFChars(env, lib_name, _lib_name);

    if (!handle) {
        return JNI_FALSE;
    }

    return JNI_TRUE;

}

JNIEXPORT jint JNICALL Java_no_mechatronics_sfi_fmi4j_jni_FmiLibrary_setDebugLogging(JNIEnv *env, jobject obj, jlong c, jboolean loggingOn, jobjectArray categories) {

    const jsize nCategories = (*env)->GetArrayLength(env, categories);
    char* _categories = malloc(sizeof(char) * nCategories);

    for (int i = 0; i < nCategories; i++) {
        jstring str = (jstring) (*env)->GetObjectArrayElement(env, categories, i);
        _categories[i] = (*env)->GetStringUTFChars(env, str, NULL);
    }

    int (*fmi2SetDebugLogging)(void*, int, int, const char* []);
    fmi2SetDebugLogging = load_function("fmi2SetDebugLogging");
    int status = (*fmi2SetDebugLogging)((void*) c, loggingOn == JNI_FALSE ? 0 : 1, nCategories, _categories);

    free(_categories);

    return status;
}

JNIEXPORT jstring JNICALL Java_no_mechatronics_sfi_fmi4j_jni_FmiLibrary_getFmiVersion(JNIEnv *env, jobject obj) {
    const char* (*fmi2GetVersion)(void);
    fmi2GetVersion = load_function("fmi2GetVersion");
    const char* version = (*fmi2GetVersion)();
    return (*env)->NewStringUTF(env, version);
}

JNIEXPORT jstring JNICALL Java_no_mechatronics_sfi_fmi4j_jni_FmiLibrary_getTypesPlatform(JNIEnv *env, jobject obj) {
    const char* (*fmi2GetTypesPlatform)(void);
    fmi2GetTypesPlatform = load_function("fmi2GetTypesPlatform");
    const char* platform = (*fmi2GetTypesPlatform)();
    return (*env)->NewStringUTF(env, platform);
}

JNIEXPORT jlong JNICALL Java_no_mechatronics_sfi_fmi4j_jni_FmiLibrary_instantiate(JNIEnv *env, jobject obj, jstring instanceName, jint type, jstring guid,  jstring resourceLocation, jboolean visible, jboolean loggingOn) {
    void* (*fmi2Instantiate)(fmi2String, fmi2Type, fmi2String, fmi2String, const fmi2CallbackFunctions*, fmi2Boolean, fmi2Boolean);

    const char* _instanceName = (*env)->GetStringUTFChars(env, instanceName, 0);
    const char* _guid = (*env)->GetStringUTFChars(env, guid, 0);
    const char* _resourceLocation = (*env)->GetStringUTFChars(env, resourceLocation, 0);

    fmi2Instantiate = load_function("fmi2Instantiate");
    fmi2Component c = (*fmi2Instantiate)(_instanceName, type, _guid, _resourceLocation, &callback, visible == JNI_FALSE ? 0 : 1, loggingOn == JNI_FALSE ? 0 : 1);

    (*env)->ReleaseStringUTFChars(env, instanceName, _instanceName);
    (*env)->ReleaseStringUTFChars(env, guid, _guid);
    (*env)->ReleaseStringUTFChars(env, resourceLocation, _resourceLocation);

    return (jlong) c;
}

JNIEXPORT jint JNICALL Java_no_mechatronics_sfi_fmi4j_jni_FmiLibrary_setupExperiment(JNIEnv *env, jobject obj, jlong c, jboolean toleranceDefined, jdouble tolerance, jdouble startTime, jdouble stopTime) {

    fmi2Boolean stopTimeDefined = stopTime > startTime ? 0 : 1;

    int (*fmi2SetupExperiment)(fmi2Component, fmi2Boolean, fmi2Real, fmi2Real, fmi2Boolean, fmi2Real);
    fmi2SetupExperiment = load_function("fmi2SetupExperiment");
    int status = (*fmi2SetupExperiment)((void*) c, toleranceDefined, tolerance, startTime, stopTimeDefined, stopTime);
    return status;
}

JNIEXPORT jint JNICALL Java_no_mechatronics_sfi_fmi4j_jni_FmiLibrary_enterInitializationMode(JNIEnv *env, jobject obj, jlong c) {
    int (*fmi2EnterInitializationMode)(fmi2Component);
    fmi2EnterInitializationMode = load_function("fmi2EnterInitializationMode");
    int status = (*fmi2EnterInitializationMode)((void*) c);
    return status;
}

JNIEXPORT jint JNICALL Java_no_mechatronics_sfi_fmi4j_jni_FmiLibrary_exitInitializationMode(JNIEnv *env, jobject obj, jlong c) {
    int (*fmi2ExitInitializationMode)(fmi2Component);
    fmi2ExitInitializationMode = load_function("fmi2ExitInitializationMode");
    int status = (*fmi2ExitInitializationMode)((void*) c);
    return status;
}

JNIEXPORT jint JNICALL Java_no_mechatronics_sfi_fmi4j_jni_FmiLibrary_step(JNIEnv *env, jobject obj, jlong c, jdouble currentCommunicationPoint, jdouble communicationStepSize, jboolean noSetFMUStatePriorToCurrentPoint) {
    int (*fmi2DoStep)(fmi2Component, fmi2Real, fmi2Real, fmi2Boolean);
    fmi2DoStep = load_function("fmi2DoStep");
    int status = (*fmi2DoStep)((void*) c, currentCommunicationPoint, communicationStepSize, noSetFMUStatePriorToCurrentPoint == JNI_FALSE ? 0 : 1);
    return status;
}

JNIEXPORT jint JNICALL Java_no_mechatronics_sfi_fmi4j_jni_FmiLibrary_cancelStep(JNIEnv *env, jobject obj, jlong c) {
    int (*fmi2CancelStep)(fmi2Component);
    fmi2CancelStep = load_function("fmi2CancelStep");
    int status = (*fmi2CancelStep)((void*) c);
    return status;
}


JNIEXPORT jint JNICALL Java_no_mechatronics_sfi_fmi4j_jni_FmiLibrary_terminate(JNIEnv *env, jobject obj, jlong c) {
    int (*fmi2Terminate)(fmi2Component);
    fmi2Terminate = load_function("fmi2Terminate");
    int status = (*fmi2Terminate)((void*) c);
    return status;
}

JNIEXPORT jint JNICALL Java_no_mechatronics_sfi_fmi4j_jni_FmiLibrary_reset(JNIEnv *env, jobject obj, jlong c) {
    int (*fmi2Reset)(fmi2Component);
    fmi2Reset = load_function("fmi2Reset");
    int status = (*fmi2Reset)((void*) c);
    return status;
}

JNIEXPORT void JNICALL Java_no_mechatronics_sfi_fmi4j_jni_FmiLibrary_freeInstance(JNIEnv *env, jobject obj, jlong c) {
    void (*fmi2FreeInstance)(fmi2Component);
    fmi2FreeInstance = load_function("fmi2FreeInstance");
    (*fmi2FreeInstance)((void*) c);
    return;
}

JNIEXPORT jint JNICALL Java_no_mechatronics_sfi_fmi4j_jni_FmiLibrary_getInteger(JNIEnv *env, jobject obj, jlong c, jintArray vr, jintArray ref) {

    const jsize size = (*env)->GetArrayLength(env, vr);
    const jint *_vr = (*env)->GetIntArrayElements(env, vr, 0);

    fmi2Status (*fmi2GetInteger)(fmi2Component, const fmi2ValueReference[], size_t, fmi2Integer[]);
    fmi2GetInteger = load_function("fmi2GetInteger");

    fmi2Integer* _ref = malloc(sizeof(fmi2Integer) * size);
    int status = (*fmi2GetInteger)((void*) c, _vr, size, _ref);

    (*env)->SetIntArrayRegion(env, ref, 0, size, _ref);

    free(_ref);

    return status;
}

JNIEXPORT jint JNICALL Java_no_mechatronics_sfi_fmi4j_jni_FmiLibrary_getReal(JNIEnv *env, jobject obj, jlong c, jintArray vr, jdoubleArray ref) {

    const jsize size = (*env)->GetArrayLength(env, vr);
    const jint *_vr = (*env)->GetIntArrayElements(env, vr, 0);

    fmi2Status (*fmi2GetReal)(fmi2Component, const fmi2ValueReference[], size_t, fmi2Real[]);
    fmi2GetReal = load_function("fmi2GetReal");

    fmi2Real* _ref = malloc(sizeof(fmi2Real) * size);
    int status = (*fmi2GetReal)((void*) c, _vr, size, _ref);

    (*env)->SetDoubleArrayRegion(env, ref, 0, size, _ref);

    free(_ref);
    (*env)->ReleaseIntArrayElements(env, vr, _vr, NULL);

    return status;
}

JNIEXPORT jint JNICALL Java_no_mechatronics_sfi_fmi4j_jni_FmiLibrary_getString(JNIEnv *env, jobject obj, jlong c, jintArray vr, jobjectArray ref) {

    const jsize size = (*env)->GetArrayLength(env, vr);
    const jint *_vr = (*env)->GetIntArrayElements(env, vr, 0);

    fmi2Status (*fmi2GetString)(fmi2Component, const fmi2ValueReference[], size_t, fmi2String[]);
    fmi2GetString = load_function("fmi2GetString");

    char* _ref = malloc(sizeof(char) * size);
    for (int i = 0; i < size; i++) {
        jstring str = (jstring) (*env)->GetObjectArrayElement(env, ref, i);
        _ref[i] = (*env)->GetStringUTFChars(env, str, NULL);
    }

    int status = (*fmi2GetString)((void*) c, _vr, size, _ref);

    for (int i = 0; i < size; i++) {
        jstring value = (*env)->NewStringUTF(env, _ref[i]);
        (*env)->SetObjectArrayElement(env, ref, i, value);
    }

    free(_ref);
    (*env)->ReleaseIntArrayElements(env, vr, _vr, NULL);

    return status;
}

JNIEXPORT jint JNICALL Java_no_mechatronics_sfi_fmi4j_jni_FmiLibrary_getBoolean(JNIEnv *env, jobject obj, jlong c, jintArray vr, jbooleanArray ref) {

    const jsize size = (*env)->GetArrayLength(env, vr);
    const jboolean *_vr = (*env)->GetBooleanArrayElements(env, vr, 0);

    fmi2Status (*fmi2GetBoolean)(fmi2Component, const fmi2ValueReference[], size_t, fmi2Boolean[]);
    fmi2GetBoolean = load_function("fmi2GetBoolean");

    fmi2Boolean* _ref = malloc(sizeof(fmi2Boolean) * size);
    int status = (*fmi2GetBoolean)((void*) c, _vr, size, _ref);

    (*env)->SetBooleanArrayRegion(env, ref, 0, size, _ref);

    free(_ref);
    (*env)->ReleaseIntArrayElements(env, vr, _vr, NULL);

    return status;
}

JNIEXPORT jint JNICALL Java_no_mechatronics_sfi_fmi4j_jni_FmiLibrary_setInteger(JNIEnv *env, jobject obj, jlong c, jintArray vr, jintArray values) {

    const jsize size = (*env)->GetArrayLength(env, vr);
    const jint *_vr = (*env)->GetIntArrayElements(env, vr, 0);
    const jint *_values = (*env)->GetIntArrayElements(env, vr, 0);

    fmi2Status (*fmi2SetInteger)(fmi2Component, const fmi2ValueReference[], size_t, fmi2Integer[]);
    fmi2SetInteger = load_function("fmi2SetInteger");

    int status = (*fmi2SetInteger)((void*) c, _vr, size, _values);

    (*env)->ReleaseIntArrayElements(env, vr, _vr, NULL);
    (*env)->ReleaseIntArrayElements(env, values, _values, NULL);

    return status;
}

JNIEXPORT jint JNICALL Java_no_mechatronics_sfi_fmi4j_jni_FmiLibrary_setReal(JNIEnv *env, jobject obj, jlong c, jintArray vr, jdoubleArray values) {

    const jsize size = (*env)->GetArrayLength(env, vr);
    const jint *_vr = (*env)->GetIntArrayElements(env, vr, 0);
    const jdouble *_values = (*env)->GetDoubleArrayElements(env, vr, 0);

    fmi2Status (*fmi2SetReal)(fmi2Component, const fmi2ValueReference[], size_t, fmi2Real[]);
    fmi2SetReal = load_function("fmi2SetReal");

    int status = (*fmi2SetReal)((void*) c, _vr, size, _values);

    (*env)->ReleaseIntArrayElements(env, vr, _vr, NULL);
    (*env)->ReleaseDoubleArrayElements(env, values, _values, NULL);

    return status;
}

JNIEXPORT jint JNICALL Java_no_mechatronics_sfi_fmi4j_jni_FmiLibrary_setString(JNIEnv *env, jobject obj, jlong c, jintArray vr, jobjectArray values) {

    const jsize size = (*env)->GetArrayLength(env, vr);
    const jint *_vr = (*env)->GetIntArrayElements(env, vr, 0);

    char* _values = malloc(sizeof(char) * size);
    for (int i = 0; i < size; i++) {
       jstring str = (jstring) (*env)->GetObjectArrayElement(env, values, i);
       _values[i] = (*env)->GetStringUTFChars(env, str, NULL);
    }

    fmi2Status (*fmi2SetString)(fmi2Component, const fmi2ValueReference[], size_t, fmi2String[]);
    fmi2SetString = load_function("fmi2SetString");

    int status = (*fmi2SetString)((void*) c, _vr, size, _values);

    free(_values);
    (*env)->ReleaseIntArrayElements(env, vr, _vr, NULL);

    return status;
}

JNIEXPORT jint JNICALL Java_no_mechatronics_sfi_fmi4j_jni_FmiLibrary_setBoolean(JNIEnv *env, jobject obj, jlong c, jintArray vr, jbooleanArray values) {

    const jsize size = (*env)->GetArrayLength(env, vr);
    const jint *_vr = (*env)->GetIntArrayElements(env, vr, 0);
    const jboolean *_values = (*env)->GetBooleanArrayElements(env, vr, 0);

    fmi2Status (*fmi2SetBoolean)(fmi2Component, const fmi2ValueReference[], size_t, fmi2Boolean[]);
    fmi2SetBoolean = load_function("fmi2SetBoolean");

    int status = (*fmi2SetBoolean)((void*) c, _vr, size, _values);

    (*env)->ReleaseIntArrayElements(env, vr, _vr, NULL);
    (*env)->ReleaseBooleanArrayElements(env, values, _values, NULL);

    return status;
}


JNIEXPORT jint JNICALL Java_no_mechatronics_sfi_fmi4j_jni_FmiLibrary_getFMUstate(JNIEnv *env, jobject obj, jlong c, jobject state) {

    jclass cls;
    jfieldID id;

    cls = (*env)->FindClass(env, "no/mechatronics/sfi/fmi4j/jni/Pointer");
    id = (*env)->GetFieldID(env, cls, "value", "I");

    fmi2Status (*fmi2GetFMUstate)(fmi2Component, fmi2FMUstate*);
    fmi2GetFMUstate = load_function("fmi2GetFMUstate");

    void* _state;

    int status = (*fmi2GetFMUstate)((void*) c, _state);

    (*env)->SetIntField(env, state, id, (jint) _state);

    return status;

}

JNIEXPORT jint JNICALL Java_no_mechatronics_sfi_fmi4j_jni_FmiLibrary_setFMUstate(JNIEnv *env, jobject obj, jlong c, jlong state) {

    jclass cls;
    jfieldID id;

    cls = (*env)->FindClass(env, "no/mechatronics/sfi/fmi4j/jni/Pointer");
    id = (*env)->GetFieldID(env, cls, "value", "I");

    fmi2Status (*fmi2setFMUstate)(fmi2Component, fmi2FMUstate);
    fmi2setFMUstate = load_function("fmi2SetFMUstate");

    int status = (*fmi2setFMUstate)((void*) c, (void*) state);

    return status;

}

JNIEXPORT jint JNICALL Java_no_mechatronics_sfi_fmi4j_jni_FmiLibrary_getDirectionalDerivative(JNIEnv *env, jobject obj, jlong c, jintArray vUnknown_ref, jintArray vKnown_ref, jdoubleArray dvKnown_ref, jdoubleArray dvUnknown_ref) {

    const jsize nUknown = (*env)->GetArrayLength(env, vUnknown_ref);
    const jsize nKnown = (*env)->GetArrayLength(env, vUnknown_ref);

    const jint *_vUnknown_ref = (*env)->GetIntArrayElements(env, vUnknown_ref, 0);
    const jint *_vKnown_ref = (*env)->GetIntArrayElements(env, vKnown_ref, 0);
    const jdouble *_dvKnown_ref = (*env)->GetDoubleArrayElements(env, vKnown_ref, 0);
    const jdouble *_dvUnknown_ref = (*env)->GetDoubleArrayElements(env, vUnknown_ref, 0);

    fmi2Status (*fmi2GetDirectionalDerivative)(fmi2Component, const fmi2ValueReference[], size_t, const fmi2ValueReference[], size_t, const fmi2Real[], fmi2Real[]);
    fmi2GetDirectionalDerivative = load_function("fmi2GetDirectionalDerivative");

    int status = (*fmi2GetDirectionalDerivative)((void*) c, _vUnknown_ref, nUknown, _vKnown_ref, nKnown, _dvKnown_ref, _dvUnknown_ref);

    (*env)->ReleaseIntArrayElements(env, vUnknown_ref, _vUnknown_ref, NULL);
    (*env)->ReleaseIntArrayElements(env, vKnown_ref, _vKnown_ref, NULL);

    (*env)->ReleaseDoubleArrayElements(env, dvKnown_ref, _dvKnown_ref, NULL);
    (*env)->ReleaseDoubleArrayElements(env, dvUnknown_ref, _dvUnknown_ref, NULL);

    return status;

}

JNIEXPORT jboolean JNICALL Java_no_mechatronics_sfi_fmi4j_jni_FmiLibrary_close(JNIEnv *env, jobject obj) {
    if (handle) {
        #ifdef WIN32
            return FreeLibrary(handle) == 0 ? JNI_FALSE : JNI_FALSE;
        #else
            return dlclose(handle)  == 0 ? JNI_FALSE : JNI_FALSE;
        #endif
    }
    return JNI_FALSE;
}
