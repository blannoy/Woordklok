import React, { useState, useEffect } from "react";
import BrightnessRangePicker from "./BrightnessRangePicker";
import isEqual from 'lodash/isEqual';

function LDRBrightness(props) {
    const [brightnessConfig, setBrightnessConfig] = useState(props.currentVal?props.currentVal:{});

    const [ldrBrightness, setLdrBrightness] = useState([]);
    const [ldrRange, setLdrRange] = useState([]);

    useEffect(() => {
        if (props.currentVal !== null) {
            setBrightnessConfig({ ...props.currentVal });
        }
    }, [props.currentVal])

    useEffect(() => {
        if (Object.keys(brightnessConfig).length!==0) {
            mapBrightnessConfigToVar();
        }
    }, [brightnessConfig])

    function mapBrightnessConfigToVar() {
        setLdrRange([brightnessConfig.ldrRange.dark, brightnessConfig.ldrRange.bright]);
        setLdrBrightness([brightnessConfig.brightness.min, brightnessConfig.brightness.max]);
    }

    function mapVarToBrightnessConfig() {
        let newConfig = {
                "ldrRange": {
                    "dark": ldrRange[0],
                    "bright": ldrRange[1]
                },
                "brightness": {
                    "min": ldrBrightness[0],
                    "max": ldrBrightness[1]
                }
        }        
        return newConfig;
    }

    useEffect(() => {
        if (!isEqual(brightnessConfig,mapVarToBrightnessConfig()) & ldrRange.length===2 & ldrBrightness.length===2){
            props.onBrightnessChoice(mapVarToBrightnessConfig());
        }
    },[ldrBrightness,ldrRange]);


    function treatLdrBrightness(val) {
        if (JSON.stringify(ldrBrightness) !== JSON.stringify(val)){
            setLdrBrightness(val);
        }
    }
    function treatLdrRange(val) {
        if (JSON.stringify(ldrRange) !== JSON.stringify(val)){
            setLdrRange(val);
        }
    }


    return (
        <div>
            <label>Omgevingslicht</label><BrightnessRangePicker id="ldrRange" currentVal={ldrRange} min={0} max={1000} minDistance={1} onBrightnessChoice={treatLdrRange} />
            <label>Helderheid</label><BrightnessRangePicker id="ldrBrightness" currentVal={ldrBrightness} onBrightnessChoice={treatLdrBrightness} />
        </div>
    );
}

export default React.memo(LDRBrightness);