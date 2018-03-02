package com.imkiva.cshelper.jni;

import com.imkiva.cshelper.CsExtensionInfo;

/**
 * @author kiva
 */
public final class CsExtensionHelper {
    static {
        System.loadLibrary("cshelper");
    }

    /**
     * Returns an extension info, including its symbols and sub-extensions.
     *
     * @param extensionPath path to extension file (*.cse)
     * @return extension info
     */
    public static native CsExtensionInfo getExtensionInfo(String extensionPath);
}
