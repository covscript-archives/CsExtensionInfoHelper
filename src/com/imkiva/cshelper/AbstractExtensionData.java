package com.imkiva.cshelper;

/**
 * @author kiva
 */

public class AbstractExtensionData {
    private String name;
    private String nativeTypeName;

    public String getNativeTypeName() {
        return nativeTypeName;
    }

    public void setNativeTypeName(String nativeTypeName) {
        this.nativeTypeName = nativeTypeName;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }
}
