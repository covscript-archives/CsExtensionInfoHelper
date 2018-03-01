#include <covscript/core.hpp>

#include <jni.h>

static JavaVM *vm;
static jclass com_imkiva_cshelper_AbstractExtensionData;
static jclass com_imkiva_cshelper_CsCallableInfo;
static jclass com_imkiva_cshelper_CsVariableInfo;
static jclass com_imkiva_cshelper_CsExtensionInfo;
static jmethodID CsExtensionInfo_new;
static jmethodID CsCallableInfo_new;
static jmethodID CsVariableInfo_new;
static jmethodID CsExtensionInfo_addExtensionData;
static jmethodID AbstractExtensionData_setName;
static jmethodID AbstractExtensionData_setNativeTypeName;

void parse_extension_info(const char *extensionPath) {
    cs::extension_holder ext(extensionPath);
    cs::domain_t domain = ext.get_domain();
}

JNIEXPORT jobject JNICALL
Java_com_imkiva_cshelper_jni_CsExtensionHelper_getExtensionInfo(JNIEnv *env, jclass type,
                                                                jstring extensionPath_) {
    const char *extensionPath = env->GetStringUTFChars(extensionPath_, 0);
    jobject csExtensionInfo = env->NewObject(com_imkiva_cshelper_CsExtensionInfo,
                                             CsExtensionInfo_new);
    parse_extension_info(extensionPath);
    env->ReleaseStringUTFChars(extensionPath_, extensionPath);
}

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    ::vm = vm;
    JNIEnv *env = nullptr;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    com_imkiva_cshelper_AbstractExtensionData = env->FindClass(
            "com/imkiva/cshelper/AbstractExtensionData");
    com_imkiva_cshelper_CsExtensionInfo = env->FindClass(
            "com/imkiva/cshelper/CsExtensionInfo");
    com_imkiva_cshelper_CsCallableInfo = env->FindClass(
            "com/imkiva/cshelper/CsCallableInfo");
    com_imkiva_cshelper_CsVariableInfo = env->FindClass(
            "com/imkiva/cshelper/CsVariableInfo");

    CsCallableInfo_new = env->GetMethodID(
            com_imkiva_cshelper_CsExtensionInfo,
            "<init>",
            "()V");
    CsVariableInfo_new = env->GetMethodID(
            com_imkiva_cshelper_CsVariableInfo,
            "<init>",
            "()V");
    CsCallableInfo_new = env->GetMethodID(
            com_imkiva_cshelper_CsCallableInfo,
            "<init>",
            "()V");

    CsCallableInfo_new = env->GetMethodID(
            com_imkiva_cshelper_CsExtensionInfo,
            "<init>",
            "()V");
    CsExtensionInfo_addExtensionData = env->GetMethodID(
            com_imkiva_cshelper_CsExtensionInfo,
            "addExtensionData",
            "(Lcom/imkiva/cshelper/AbstractExtensionData;)V");

    AbstractExtensionData_setName = env->GetMethodID(
            com_imkiva_cshelper_AbstractExtensionData,
            "setName",
            "(Ljava/lang/String;)V");

    AbstractExtensionData_setNativeTypeName = env->GetMethodID(
            com_imkiva_cshelper_AbstractExtensionData,
            "setNativeTypeName",
            "(Ljava/lang/String;)V");
    return JNI_VERSION_1_6;
}
