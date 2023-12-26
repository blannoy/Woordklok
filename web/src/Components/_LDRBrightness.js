import React, { useState, useContext, useEffect, useRef, useCallback } from "react";
import { configContext, queryProviderContext } from "../Context/Context";
import { Slider, RangeSlider } from 'rsuite';
import BrightnessPicker from "../Components/BrightnessPicker";
import useRequest from "../Context/Reducer";
import '../css/styles.css';
import '../css/slider.less';


export default function LDRBrightness(props) {
    const [config, setConfig] = useContext(configContext);
    const [brightnessConfig, setBrightnessConfig] = useState(config && config.brightness);

    const calibrationTime=30;
    const [seconds, setSeconds] = useState(calibrationTime);
    const [doCalibrate, setDoCalibrate] = useState(false);
    const intervalRef = useRef();
    const [requestState, dispatchRequest] = useContext(queryProviderContext);
    const [response, err, runRequest] = useRequest();

    const [ldrBrightnessMax, setLdrBrightnessMax] = useState(0);
    const [ldrBrightnessMin, setLdrBrightnessMin] = useState(0);
    const [ldrDark, setLdrDark] = useState(0);
    const [ldrBright, setLdrBright] = useState(0);

    const [ldrCalibratedDark, setLdrCalibratedLdrDark] = useState(0);
    const [ldrCalibratedBright, setLdrCalibratedLdrBright] = useState(0);

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
                setLdrCalibratedLdrDark(response.data.min);
                setLdrCalibratedLdrBright(response.data.max);

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

    function onBrightnessChoice(val) {
        let id = this.id;
        let fnString = "set" + id.charAt(0).toUpperCase() + id.slice(1);
        let fn = eval(fnString);
        fn(val);
    }

    function setLdr(event) {
console.log(event);
    }

    return (
        <div>
            <div>
                <button id="calibrateLdr" onClick={calibrateLdr}>Calibrate LDR</button>{doCalibrate && "Calibrating..." + ldrCalibratedDark + " / " + ldrCalibratedBright}
                <div style={{ marginLeft: "20px" }}> <input type="number" name="ldrDark" min={0} max={255} value={ldrDark} onChange={setLdr} /></div>
                <div style={{ marginLeft: "20px" }}> <input type="number" name="ldrBright" min={0} max={255} value={ldrBright} onChange={setLdr} /></div>
            </div>
            <div>
            <RangeSlider

          defaultValue={[ldrDark, ldrBright]}
          min={0}
          max={100}
          onChange={value => {
            console.log(value);
          }}
        />
          {/*   <MultiRangeSlider
            baseClassName='multi-range-slider'
			min={0}
			max={100}
			step={1}
            label={true}
            ruler={false}
            style={{ border: "none", boxShadow: "none", padding: "15px 10px" ,
                width:"400px"}}
			minValue={ldrDark}
			maxValue={ldrBright}
            onInput={(e: ChangeResult) => {
                setLdrDark(e.minValue);
                setLdrBright(e.maxValue);
            }}
		/> */}
            </div>
            <div>
                <BrightnessPicker id="ldrBrightnessMin" currentVal={ldrBrightnessMin} onBrightnessChoice={onBrightnessChoice} />
                <BrightnessPicker id="ldrBrightnessMax" currentVal={ldrBrightnessMax} onBrightnessChoice={onBrightnessChoice} />
            </div>
        </div>
    );
}