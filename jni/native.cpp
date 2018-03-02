#include <covscript/extension.hpp>

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

static jobject new_ExtensionInfo(JNIEnv *env) {
    return env->NewObject(com_imkiva_cshelper_CsExtensionInfo,
                          CsExtensionInfo_new);
}

static jobject new_CallableInfo(JNIEnv *env) {
    return env->NewObject(com_imkiva_cshelper_CsCallableInfo,
                          CsCallableInfo_new);
}

static jobject new_VariableInfo(JNIEnv *env) {
    return env->NewObject(com_imkiva_cshelper_CsVariableInfo,
                          CsVariableInfo_new);
}

static void addExtensionData(JNIEnv *env, jobject extensionInfo, jobject extensionData) {
    env->CallVoidMethod(extensionInfo, CsExtensionInfo_addExtensionData, extensionData);
}

static void setNameImpl(JNIEnv *env, jmethodID setNameMethod,
                        jobject extensionData, const char *name) {
    jstring jname = env->NewStringUTF(name);
    env->CallVoidMethod(extensionData, setNameMethod, jname);
}

static void setName(JNIEnv *env, jobject extensionData, const char *name) {
    setNameImpl(env, AbstractExtensionData_setName, extensionData, name);
}

static void setNativeTypeName(JNIEnv *env, jobject extensionData, const char *name) {
    setNameImpl(env, AbstractExtensionData_setNativeTypeName, extensionData, name);
}

jobject parse_extension_info(JNIEnv *env, const cs::domain_t &domain) {
    jobject csExtensionInfo = env->NewObject(com_imkiva_cshelper_CsExtensionInfo,
                                             CsExtensionInfo_new);
    for (auto &element : *domain) {
        jobject data = nullptr;
        if (element.second.type() == typeid(cs::callable)) {
            data = new_CallableInfo(env);
        } else if (element.second.type() == typeid(cs::extension_t)) {
            const auto &sub_domain = element.second.const_val<cs::domain_t>();
            data = parse_extension_info(env, sub_domain);
        } else {
            data = new_VariableInfo(env);
        }
        setName(env, data, element.first.c_str());
        setNativeTypeName(env, data, element.second.get_type_name());
        addExtensionData(env, csExtensionInfo, data);
    }
    return csExtensionInfo;
}

jobject parse_extension_info(JNIEnv *env, const char *extensionPath) {
    cs::extension_holder ext(extensionPath);
    cs::domain_t domain = ext.get_domain();
    return parse_extension_info(env, domain);
}

JNIEXPORT jobject JNICALL
Java_com_imkiva_cshelper_jni_CsExtensionHelper_getExtensionInfo(JNIEnv *env, jclass type,
                                                                jstring extensionPath_) {
    const char *extensionPath = env->GetStringUTFChars(extensionPath_, 0);
    jobject result = parse_extension_info(env, extensionPath);
    env->ReleaseStringUTFChars(extensionPath_, extensionPath);
    return result;
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
