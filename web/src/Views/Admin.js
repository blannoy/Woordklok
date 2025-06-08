import React from "react";
import Firmware from "./Firmware";
import { useTranslation } from "react-i18next";

function Admin() {
    const { t } = useTranslation();

    return (
        <div>
            <h2>{t("admin.title")}</h2>
            <div className="section">
                <form action='/api/configWifi'>
                    <div className="section">
                        <div className="labelCell">
                            <label>{t("admin.resetWifi")}</label>
                        </div>
                        <div className="normalBodyCell">
                            <input type="submit" value={t("admin.configWifiBtn")} />
                        </div>
                    </div>
                </form>
            </div>


            <Firmware />
        </div>
    );
}

export default React.memo(Admin);