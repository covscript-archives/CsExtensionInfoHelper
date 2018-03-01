package com.imkiva.cshelper.jni;

import com.imkiva.cshelper.CsExtensionInfo;

/**
 * @author kiva
 */

public final class CsExtensionHelper {
    public static native CsExtensionInfo getExtensionInfo(String extensionPath);
}
