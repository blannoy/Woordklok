import React, { useState, useContext, useEffect } from "react";
import { configContext, queryProviderContext } from "../Context/Context";
import BrightnessPicker from "../Components/BrightnessPicker";
import useRequest from "../Context/Reducer";

export default function LDRBrightness(props) {
    const [config, setConfig] = useContext(configContext);
    const [brightnessConfig, setBrightnessConfig] = useState(config && config.brightness);


    const [requestState, dispatchRequest] = useContext(queryProviderContext);
    const [response, err, runRequest] = useRequest();

    const [ldrBrightnessMax, setLdrBrightnessMax] = useState(0);
    const [ldrBrightnessMin, setLdrBrightnessMin] = useState(0);
    const [ldrDark, setLdrDark] = useState(0);
    const [ldrBright, setLdrBright] = useState(0);


    function onValueChange(event) {
        setBrightnessConfig({ ...config.brightness, "brightnessMode": event.target.value });
    }
    // keep config in sync when changed
    useEffect(() => {
        if (config !== null) {
            setBrightnessConfig({ ...config.brightness });
        }
    }, [config])

    function mapBrightnessConfigToVar() {
        setLdrBright(brightnessConfig.settings.ldrBrightness.ldrRange.bright);
        setLdrDark(brightnessConfig.settings.ldrBrightness.ldrRange.dark);
        setLdrBrightnessMax(brightnessConfig.settings.ldrBrightness.brightness.max);
        setLdrBrightnessMin(brightnessConfig.settings.ldrBrightness.brightness.min);
    }

    function mapVarToBrightnessConfig() {
        let newConfig = {
            "brightnessMode": ldrBrightness,
            "ldrBrightness": {
                "ldrRange": {
                    "dark": ldrDark,
                    "bright": ldrBright
                },
                "brightness": {
                    "max": ldrBrightnessMax,
                    "min": ldrBrightnessMin
                }
            }
        }
        return newConfig;
    }

    useEffect(() => {
        if (brightnessConfig) {
            mapBrightnessConfigToVar();
        }
    }, [brightnessConfig])


    useEffect(() => {
        if (requestState.url !== "") {
            runRequest(requestState);
        }
    }, [requestState]);
    useEffect(() => {
        if (response !== null) {
            if (response.config.url === "/calibrateLdr") {
                setLdrDark(response.data.min);
                setLdrBright(response.data.max);

            }
        }
    }, [response]);


    function calibrateLdr() {

        dispatchRequest({ type: "calibrateLdr" });

    }
    function onBrightnessChoice(val) {
        let id=this.id;
        let fnString="set"+id.charAt(0).toUpperCase()+id.slice(1);
        let fn=eval(fnString);
        fn(val);
      }
    function setLdr(event){
    }
    return (
        <div>
            <div>
                <button id="calibrateLdr" onClick={calibrateLdr}>Calibrate LDR</button>
                <div style={{ marginLeft: "20px" }}> <input type="number" name="ldrDark" min={0} max={255} value={ldrDark} onChange={setLdr} /></div>
                <div style={{ marginLeft: "20px" }}> <input type="number" name="ldrBright" min={0} max={255} value={ldrBright} onChange={setLdr} /></div>
            </div>
            <div>
                <BrightnessPicker id="ldrBrightnessMin" currentVal={ldrBrightnessMin} onBrightnessChoice={onBrightnessChoice} />
                <BrightnessPicker id="ldrBrightnessMax" currentVal={ldrBrightnessMax} onBrightnessChoice={onBrightnessChoice} />
            </div>
        </div>
    );
}