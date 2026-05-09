#include <jni.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <android/log.h>
#include <sys/ptrace.h>
#include <unistd.h>

#define SEC_LOG_TAG "ShieldNative"
#define DBG_INFO(...) __android_log_print(ANDROID_LOG_INFO, SEC_LOG_TAG, __VA_ARGS__)
#define DBG_WARN(...) __android_log_print(ANDROID_LOG_WARN, SEC_LOG_TAG, __VA_ARGS__)
#define DBG_ERR(...) __android_log_print(ANDROID_LOG_ERROR, SEC_LOG_TAG, __VA_ARGS__)

/**
 * Verifies if the process is currently being debugged using ptrace.
 */
static bool checkPtraceTracing() {
    if (ptrace(PTRACE_TRACEME, 0, 1, 0) < 0) {
        DBG_ERR("Security alert: Debugging session detected via ptrace");
        return true;
    }
    // Release ptrace if successful
    // Note: PTRACE_TRACEME doesn't need a detach from the same process side usually in this context
    // but some implementations might check if it can be called again.
    return false;
}

/**
 * Scans /proc/self/maps for known instrumentation frameworks.
 */
static bool scanProcessMaps() {
    std::ifstream mapsFile("/proc/self/maps");
    if (!mapsFile.is_open()) {
        DBG_WARN("Could not access process memory maps");
        return false;
    }

    std::string line;
    const std::vector<std::string> blacklist = {
        "frida", "xposed", "libfrida", "gdbserver", "libgdb", "magisk", "substrate"
    };

    while (std::getline(mapsFile, line)) {
        for (const auto& pattern : blacklist) {
            if (line.find(pattern) != std::string::npos) {
                DBG_ERR("Security breach: Found restricted library -> %s", pattern.c_str());
                return true;
            }
        }
    }

    return false;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_example_lab23_MainActivity_verifySystemIntegrity(
        JNIEnv* env,
        jobject /* this */) {

    bool isTraced = checkPtraceTracing();
    bool hasSuspiciousLibs = scanProcessMaps();

    if (isTraced || hasSuspiciousLibs) {
        DBG_ERR("System integrity check failed!");
        return JNI_TRUE;
    }

    DBG_INFO("System integrity verified successfully");
    return JNI_FALSE;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_lab23_MainActivity_fetchSecureMessage(
        JNIEnv* env,
        jobject /* this */) {
    return env->NewStringUTF("Access Granted: Secure Native Communication established.");
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_example_lab23_MainActivity_computeFactorialNative(
        JNIEnv* env,
        jobject /* this */,
        jint val) {

    if (val < 0) return -1;
    if (val == 0) return 1;

    unsigned int result = 1;
    for (int i = 1; i <= val; ++i) {
        result *= i;
    }

    return static_cast<jint>(result);
}
