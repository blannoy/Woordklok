import React, { useState, useContext, useEffect, useRef } from "react";
import { configContext, queryProviderContext } from "../../Context/Context";
import BrightnessRangePicker from "./BrightnessRangePicker";
import useRequest from "../../Context/Reducer";
import isEqual from 'lodash/isEqual';

export default function LDRBrightness(props) {
    const [brightnessConfig, setBrightnessConfig] = useState(props.currentVal?props.currentVal:undefined);

    const calibrationTime = 30;
    const [seconds, setSeconds] = useState(calibrationTime);
    const [doCalibrate, setDoCalibrate] = useState(false);
    const intervalRef = useRef();
    const [requestState, dispatchRequest] = useContext(queryProviderContext);
    const [response, err, runRequest] = useRequest();

    const [ldrBrightness, setLdrBrightness] = useState([]);
    const [ldrRange, setLdrRange] = useState([]);
    const [ldrCalibrated, setLdrCalibrated] = useState([0,100]);

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


    useEffect(() => {
        if (requestState.url !== "") {
            runRequest(requestState);
        }
    }, [requestState]);

    useEffect(() => {
        if (response !== null) {
            if (response.config.url === "/calibrateLdr") {
                setLdrCalibrated([response.data.min, response.data.max]);
            }
        }
    }, [response]);

    const tick = () => {
        setSeconds(prevSeconds => prevSeconds - 1);
        dispatchRequest({ type: "calibrateLdr" });
    }

    useEffect(() => {
        if (doCalibrate) {
            intervalRef.current = setInterval(tick, 1000);
        } else if (intervalRef.current) {
            clearInterval(intervalRef.current);
            setSeconds(calibrationTime); // reset timer
            setLdrRange(ldrCalibrated);
        }
        return () => {
            if (intervalRef.current) {
                clearInterval(intervalRef.current);
            }
        };
    }, [doCalibrate]);

    useEffect(() => {
        if (seconds <= 0 && intervalRef.current) {
            clearInterval(intervalRef.current);
            setSeconds(calibrationTime); // reset timer
            setDoCalibrate(false);
        }
    }, [seconds]);

    function calibrateLdr() {
        setDoCalibrate(true);
    }

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
            <div className="section">
                <button id="calibrateLdr" onClick={calibrateLdr}>Calibrate LDR</button>{doCalibrate && "Calibrating..." + ldrCalibrated[0] + " / " + ldrCalibrated[1]}
            </div>
 
    
            <BrightnessRangePicker id="ldrRange" currentVal={ldrRange} min={0} max={100} minDistance={1} onBrightnessChoice={treatLdrRange} />
            <BrightnessRangePicker id="ldrBrightness" currentVal={ldrBrightness} onBrightnessChoice={treatLdrBrightness} />

        </div>
    );
}