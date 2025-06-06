import React from "react";
import Firmware from "./Firmware";


function Admin() {

    return (
        <div><h2>Admin</h2>
            <div className="section">
                <form action='/api/configWifi'>

                    <div className="labelCell"><label>Reset wifi & start AP:</label></div>
                    <div className="normalBodyCell">
                        <input type="submit" value="Config Wifi" />
                    </div>


                </form>
            </div>
            <div className="section">
                <form method="POST" action='/api/uploadConfig' enctype="multipart/form-data">
                    <div className="labelCell"><label>Upload clockface.json:</label></div>
                    <div className="normalBodyCell">
                        <input type="file" name="clockface.json" accept=".json"></input>
                        <input class="button" type="submit" value="Upload clockface"></input>
                    </div>

                </form>
            </div>
            <div className="section">
                <form method="POST" action='/api/uploadConfig' enctype="multipart/form-data">

                    <div className="labelCell"><label>Upload config.json:</label></div>
                    <div className="normalBodyCell">
                        <input type="file" name="config.json" accept=".json"></input>
                        <input class="button" type="submit" value="Upload config"></input>
                    </div>
                </form>
            </div>


            <Firmware />


        </div >
    );
}

export default React.memo(Admin);