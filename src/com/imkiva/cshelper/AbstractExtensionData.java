package com.imkiva.cshelper;

/**
 * Represent something in an extension.
 *
 * @author kiva
 * @see CsExtensionInfo
 * @see CsVariableInfo
 * @see CsCallableInfo
 */
public class AbstractExtensionData {
    private String name;
    private String nativeTypeName;

    /**
     * Native representation name (namespaceName::typeName).
     * This also can be CovScript type names sometimes,
     * Due to the existence of CNI,
     * any C++ class can be a data type of CovScript.
     *
     * @return native type name
     */
    public String getNativeTypeName() {
        return nativeTypeName;
    }

    /**
     * Used by native methods.
     *
     * @param nativeTypeName native representation name
     * @see com.imkiva.cshelper.jni.CsExtensionHelper#getExtensionInfo(String)
     */
    public void setNativeTypeName(String nativeTypeName) {
        this.nativeTypeName = nativeTypeName;
    }

    /**
     * Symbol name
     *
     * @return symbol name
     */
    public String getName() {
        return name;
    }

    /**
     * Used by native methods.
     *
     * @param name symbol name
     * @see com.imkiva.cshelper.jni.CsExtensionHelper#getExtensionInfo(String)
     */
    public void setName(String name) {
        this.name = name;
    }
}
