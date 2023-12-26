import React, { useState, useContext, useEffect, useRef } from "react";
import { configContext, queryProviderContext } from "../../Context/Context";
import BrightnessRangePicker from "./BrightnessRangePicker";
import isEqual from 'lodash/isEqual';
import ReactSlider from "react-slider";
import HourRangePicker from "./HourRangePicker";

export default function HourBrightness(props) {
    const [brightnessConfig, setBrightnessConfig] = useState(props.currentVal?props.currentVal:undefined);

    const [hourBrightness, setHourBrightness] = useState([]);
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
        setHourRange([brightnessConfig.hourSlot.startHour, brightnessConfig.hourSlot.endHour]);
        setHourBrightness([brightnessConfig.brightness.min, brightnessConfig.brightness.max]);
    }

    function mapVarToBrightnessConfig() {
        let newConfig = {
                "hourSlot": {
                    "startHour": hourRange[0],
                    "endHour": hourRange[1]
                },
                "brightness": {
                    "min": hourBrightness[0],
                    "max": hourBrightness[1]
                }
        }        
        return newConfig;
    }

    useEffect(() => {
        if (!isEqual(brightnessConfig,mapVarToBrightnessConfig()) & hourRange.length===2 & hourBrightness.length===2){
            props.onBrightnessChoice(mapVarToBrightnessConfig());
        }
    },[hourBrightness,hourRange]);

    function treatHourBrightness(val) {
        if (JSON.stringify(hourBrightness) !== JSON.stringify(val)){
            setHourBrightness(val);
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
            <BrightnessRangePicker id="hourRange" currentVal={hourRange} min={0} max={24} minDistance={1} onBrightnessChoice={treatHourRange} />
            <BrightnessRangePicker id="hourBrightness" currentVal={hourBrightness} onBrightnessChoice={treatHourBrightness} />

        </div>
    );
}