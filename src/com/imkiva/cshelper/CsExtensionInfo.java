package com.imkiva.cshelper;

import java.util.ArrayList;
import java.util.List;

/**
 * @author kiva
 */

public class CsExtensionInfo extends AbstractExtensionData {
    private List<AbstractExtensionData> extensionData;

    public CsExtensionInfo() {
        this.extensionData = new ArrayList<>();
    }

    public void addExtensionData(AbstractExtensionData data) {
        this.extensionData.add(data);
    }
}
