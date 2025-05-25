import React from "react";
import Firmware from "./Firmware";


function Admin() {

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