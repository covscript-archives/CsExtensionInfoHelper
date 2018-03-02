#include <covscript/core.hpp>
#include <stdio.h>
#include <jni.h>

#define LOG(FMT, ...) printf("%s: " FMT "\n", __FUNCTION__, ##__VA_ARGS__)
#define LOGP(V) LOG("%s = %p", #V, V)

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
    LOG("Enter");
    return env->NewObject(com_imkiva_cshelper_CsExtensionInfo,
                          CsExtensionInfo_new);
}

static jobject new_CallableInfo(JNIEnv *env) {
    LOG("Enter");
    return env->NewObject(com_imkiva_cshelper_CsCallableInfo,
                          CsCallableInfo_new);
}

static jobject new_VariableInfo(JNIEnv *env) {
    LOG("Enter");
    return env->NewObject(com_imkiva_cshelper_CsVariableInfo,
                          CsVariableInfo_new);
}

static void addExtensionData(JNIEnv *env, jobject extensionInfo, jobject extensionData) {
    LOG("Enter");
    env->CallVoidMethod(extensionInfo, CsExtensionInfo_addExtensionData, extensionData);
}

static void setNameImpl(JNIEnv *env, jmethodID setNameMethod,
                        jobject extensionData, const char *name) {
    LOG("Enter");
    jstring jname = env->NewStringUTF(name);
    env->CallVoidMethod(extensionData, setNameMethod, jname);
}

static void setName(JNIEnv *env, jobject extensionData, const char *name) {
    LOG("Enter");
    setNameImpl(env, AbstractExtensionData_setName, extensionData, name);
}

static void setNativeTypeName(JNIEnv *env, jobject extensionData, const char *name) {
    LOG("Enter");
    setNameImpl(env, AbstractExtensionData_setNativeTypeName, extensionData, name);
}

jobject parseExtensionInfo(JNIEnv *env, const cs::extension_holder &ext) {
    LOG("Enter");
    cs::domain_t domain = ext.get_domain();

    jobject csExtensionInfo = new_ExtensionInfo(env);
    LOG("Object csExtensionInfo created");
    LOGP(csExtensionInfo);

    for (auto &element : *domain) {
        jobject data = nullptr;
        if (element.second.type() == typeid(cs::callable)) {
            LOG("===> CallableInfo: %s", element.first.c_str());
            data = new_CallableInfo(env);
            LOG("<=== Created");
        } else if (element.second.type() == typeid(cs::extension_t)) {
            LOG("===> ExtensionInfo: %s", element.first.c_str());
            const auto &sub_extension = element.second.const_val<cs::extension_t>();
            data = parseExtensionInfo(env, *sub_extension);
            LOG("<=== Created");
        } else {
            LOG("===> VariableInfo: %s", element.first.c_str());
            data = new_VariableInfo(env);
            LOG("<=== Created");
        }
        LOG("===> setName");
        setName(env, data, element.first.c_str());
        LOG("===> setNativeTypeName");
        setNativeTypeName(env, data, element.second.get_type_name());
        LOG("===> addExtensionData");
        addExtensionData(env, csExtensionInfo, data);
        LOG("<=== Added");
    }
    LOG("OK");
    return csExtensionInfo;
}

extern "C" JNIEXPORT jobject JNICALL
Java_com_imkiva_cshelper_jni_CsExtensionHelper_getExtensionInfo(JNIEnv *env, jclass type,
                                                                jstring extensionPath_) {
    LOG("Enter");
    const char *extensionPath = env->GetStringUTFChars(extensionPath_, 0);
    LOG("extensionPath: %s", extensionPath);
    cs::extension_holder ext(extensionPath);
    jobject result = parseExtensionInfo(env, ext);
    env->ReleaseStringUTFChars(extensionPath_, extensionPath);
    LOG("OK");
    return result;
}

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    LOG("Enter");
    ::vm = vm;
    JNIEnv *env = nullptr;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    LOG("Got JNIEnv: %p", env);

    com_imkiva_cshelper_AbstractExtensionData = (jclass) env->NewGlobalRef(env->FindClass(
            "com/imkiva/cshelper/AbstractExtensionData"));
    com_imkiva_cshelper_CsExtensionInfo = (jclass) env->NewGlobalRef(env->FindClass(
            "com/imkiva/cshelper/CsExtensionInfo"));
    com_imkiva_cshelper_CsCallableInfo = (jclass) env->NewGlobalRef(env->FindClass(
            "com/imkiva/cshelper/CsCallableInfo"));
    com_imkiva_cshelper_CsVariableInfo = (jclass) env->NewGlobalRef(env->FindClass(
            "com/imkiva/cshelper/CsVariableInfo"));

    LOG("Got classes");
    LOGP(com_imkiva_cshelper_AbstractExtensionData);
    LOGP(com_imkiva_cshelper_CsExtensionInfo);
    LOGP(com_imkiva_cshelper_CsVariableInfo);
    LOGP(com_imkiva_cshelper_CsCallableInfo);

    CsExtensionInfo_new = env->GetMethodID(
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

    LOG("Got methods");
    LOGP(CsVariableInfo_new);
    LOGP(CsExtensionInfo_new);
    LOGP(CsCallableInfo_new);
    LOGP(CsExtensionInfo_addExtensionData);
    LOGP(AbstractExtensionData_setName);
    LOGP(AbstractExtensionData_setNativeTypeName);

    LOG("OK");
    return JNI_VERSION_1_6;
}

void JNI_OnUnload(JavaVM *vm, void *reserved) {
    JNIEnv *env = nullptr;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return;
    }

    if (com_imkiva_cshelper_AbstractExtensionData) {
        env->DeleteGlobalRef(com_imkiva_cshelper_AbstractExtensionData);
    }

    if (com_imkiva_cshelper_CsCallableInfo) {
        env->DeleteGlobalRef(com_imkiva_cshelper_CsCallableInfo);
    }

    if (com_imkiva_cshelper_CsVariableInfo) {
        env->DeleteGlobalRef(com_imkiva_cshelper_CsVariableInfo);
    }

    if (com_imkiva_cshelper_CsExtensionInfo) {
        env->DeleteGlobalRef(com_imkiva_cshelper_CsExtensionInfo);
    }
}
