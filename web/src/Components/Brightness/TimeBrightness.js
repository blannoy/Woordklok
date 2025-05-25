import React, { useState, useContext, useEffect, useRef } from "react";
import { configContext, queryProviderContext } from "../../Context/Context";
import BrightnessRangePicker from "./BrightnessRangePicker";
import isEqual from 'lodash/isEqual';
import HourRangePicker from "./HourRangePicker";

function TimeBrightness(props) {
    const [brightnessConfig, setBrightnessConfig] = useState(props.currentVal?props.currentVal:undefined);

    const [timeBrightness, setTimeBrightness] = useState([]);
    const [hourRange, setHourRange] = useState([]);

    useEffect(() => {
        if (props.currentVal !== null) {
            setBrightnessConfig({ ...props.currentVal });
        }
    }, [props.currentVal])

    useEffect(() => {
        if (brightnessConfig) {
            mapBrightnessConfigToVar();
        }
    }, [brightnessConfig])

    function mapBrightnessConfigToVar() {
        setHourRange([brightnessConfig.timeSlot.startHour, brightnessConfig.timeSlot.endHour]);
        setTimeBrightness([brightnessConfig.brightness.min, brightnessConfig.brightness.max]);
    }

    function mapVarToBrightnessConfig() {
        let newConfig = {
                "timeSlot": {
                    "startHour": hourRange[0],
                    "endHour": hourRange[1]
                },
                "brightness": {
                    "min": timeBrightness[0],
                    "max": timeBrightness[1]
                }
        }        
        return newConfig;
    }

    useEffect(() => {
        if (!isEqual(brightnessConfig,mapVarToBrightnessConfig()) & hourRange.length===2 & timeBrightness.length===2){
            props.onBrightnessChoice(mapVarToBrightnessConfig());
        }
    },[timeBrightness,hourRange]);

    function treattimeBrightness(val) {
        if (JSON.stringify(timeBrightness) !== JSON.stringify(val)){
            setTimeBrightness(val);
        }
    }
    function treatHourRange(val) {
        if (JSON.stringify(hourRange) !== JSON.stringify(val)){
            setHourRange(val);
        }
    }


    return (
        <div>  
            <HourRangePicker id="hourRange" currentVal={hourRange} onChange={treatHourRange} />
            <BrightnessRangePicker id="timeBrightness" currentVal={timeBrightness} onBrightnessChoice={treattimeBrightness} />

        </div>
    );
}

export default React.memo(TimeBrightness);