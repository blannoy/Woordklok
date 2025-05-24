import React, { useContext, useEffect, useState } from "react";
import { useGetStatusQuery } from "../Components/ClockAPI";
import { clockContext } from "../Context/Context";
import Firmware from "./Firmware";


function Admin() {
    const [fullConfig, setFullConfig] = useContext(clockContext);
    const [config, setConfig] = useState(fullConfig ? fullConfig.config : undefined);
    const { data: currentStatus, error: statusError, isLoading: statusIsLoading } = useGetStatusQuery();
    return (
        <div><h2>Admin</h2>
            <div className="section">
                <form action='/api/configWifi'>
                    <div className="section">

                        <div className="labelCell"><label>Reset wifi & start AP:</label></div>
                        <div className="normalBodyCell">
                            <input type="submit" value="Config Wifi" />
                        </div>


                    </div>
                </form>
            </div>
            <Firmware />


        </div>
    );
}

export default React.memo(Admin);