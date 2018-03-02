package com.imkiva.cshelper;

import java.util.HashMap;
import java.util.Map;

/**
 * Represent that this is a root-extension or a sub-extension.
 * {@link CsExtensionInfo#isSubExtension()}
 *
 * @author kiva
 */
public class CsExtensionInfo extends AbstractExtensionData {
    private Map<String, AbstractExtensionData> extensionData;

    public CsExtensionInfo() {
        this.extensionData = new HashMap<>();
    }

    /**
     * CovScript allows extension-in-extension, which is called sub-extension.
     * Returns if this extension is a sub extension.
     *
     * @return whether it is sub extension
     */
    public boolean isSubExtension() {
        return getName() != null
                && getNativeTypeName() != null;
    }

    /**
     * Returns all data in this extension.
     *
     * @return all data
     */
    public Map<String, AbstractExtensionData> getExtensionData() {
        return extensionData;
    }

    /**
     * Used by native methods.
     *
     * @param data
     * @see com.imkiva.cshelper.jni.CsExtensionHelper#getExtensionInfo(String)
     */
    public void addExtensionData(AbstractExtensionData data) {
        this.extensionData.put(data.getName(), data);
    }
}
