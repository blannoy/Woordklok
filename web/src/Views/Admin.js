import React, { useContext, useEffect, useState } from "react";
import { configContext,requestProviderContext } from "../Context/Context";


function Admin() {
    const [config, setConfig] = useContext(configContext);
    const [currentStatus, setCurrentStatus] = useContext(requestProviderContext);


    
    return (
        <div><h2>Admin</h2>
                        <form method='POST' action='/api/update/flash' encType='multipart/form-data'>
            <div className="section">

                    <div className="labelCell"><label>Firmware:</label></div>
                    <div className="normalBodyCell">
                        <input type='file' accept='.bin,.bin.gz' name='firmware' />
                        </div>
                        <div className="normalBodyCell">
                        <input type='submit' value='Update Firmware' />
</div>

            </div>                </form>
            <form method='POST' action='/api/update/file' encType='multipart/form-data'>
            <div className="section">

                    <div className="labelCell"><label>Filesystem:</label></div>
                    <div className="normalBodyCell">
                        <input type='file' accept='.bin,.bin.gz' name='filesystem' />
                        </div>
                        <div className="normalBodyCell">
                        <input type='submit' value='Update Filesystem' />
</div>

            </div>                </form>
            <div className="section">
 <div className="labelCell"><label>Network params:</label></div>
 <div className="normalBodyCell">
    { config && 
        <div>
            <div>IP: {currentStatus.IP}</div>
            <div>DNS: {currentStatus.DNS}</div>
            <div>MAC: {currentStatus.MAC}</div>
            <div>Connected SSID: {currentStatus.SSID}</div>
        </div>
    }
    </div>
            </div>
            <form action='/api/configWifi'>
            <div className="section">

                    <div className="labelCell"><label>Reset wifi & start AP:</label></div>
                    <div className="normalBodyCell">
                    <input type="submit" value="Config Wifi"/>
                        </div>


            </div>                </form>
        </div>
    );
}

export default React.memo(Admin);