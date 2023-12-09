import React, { useState, useContext, useEffect} from "react";
import { configContext, queryProviderContext } from "../Context/Context";
import BrightnessPicker from "../Components/BrightnessPicker";
import useRequest from "../Context/Reducer";
import LDRBrightness from "../Components/LDRBrightness";

export default function Brightness() {
  const [config, setConfig] = useContext(configContext);
  const [requestState, dispatchRequest] = useContext(queryProviderContext);
  const [response, err, runRequest]  = useRequest();

  const [brightnessConfig, setBrightnessConfig] = useState(config && config.brightness);
  const [selectedOption, setSelectedOption] = useState(config ? config.brightness.brightnessMode : "fixedBrightness");
  const [fixedBrightnessValue, setFixedBrightnessValue] = useState(0);
  const [ldrBrightnessMax, setLdrBrightnessMax] = useState(0);
  const [ldrBrightnessMin, setLdrBrightnessMin] = useState(0);
  const [brightnessStartHour, setBrightnessStartHour] = useState(0);
  const [brightnessEndHour, setBrightnessEndHour] = useState(0);
  const [hourBrightnessMax, setHourBrightnessMax] = useState(0);
  const [hourBrightnessMin, setHourBrightnessMin] = useState(0);
  const [ldrDark, setLdrDark] = useState(0);
  const [ldrBright, setLdrBright] = useState(0);


  function onValueChange(event) {
    setSelectedOption(event.target.value);
    setBrightnessConfig({ ...config.brightness, "brightnessMode": event.target.value });
  }
  // keep config in sync when changed
  useEffect(() => {
    if (config !== null) {
      setBrightnessConfig({ ...config.brightness });
    }
  }, [config])

  function mapBrightnessConfigToVar() {
    setFixedBrightnessValue(brightnessConfig.settings.fixedBrightness.brightness);
    setLdrBright(brightnessConfig.settings.ldrBrightness.ldrRange.bright);
    setLdrDark(brightnessConfig.settings.ldrBrightness.ldrRange.dark);
    setLdrBrightnessMax(brightnessConfig.settings.ldrBrightness.brightness.max);
    setLdrBrightnessMin(brightnessConfig.settings.ldrBrightness.brightness.min);
    setBrightnessStartHour(brightnessConfig.settings.hourBrightness.hourSlot.startHour);
    setBrightnessEndHour(brightnessConfig.settings.hourBrightness.hourSlot.endHour);
    setHourBrightnessMax(brightnessConfig.settings.hourBrightness.brightness.max);
    setHourBrightnessMin(brightnessConfig.settings.hourBrightness.brightness.min);
  }

  function mapVarToBrightnessConfig(){
    let newConfig={"brightnessMode" : selectedOption,
    "settings" : {
        "fixedBrightness" : {
            "brightness" : fixedBrightnessValue
        },
        "ldrBrightness" : {
            "ldrRange" : {
                "dark" : ldrDark,
                "bright" : ldrBright
            },
            "brightness" : {
                "max" : ldrBrightnessMax, 
                "min" : ldrBrightnessMin
            }
        },
        "hourBrightness": {
            "hourSlot" : {
                "startHour" : brightnessStartHour,
                "endHour" : brightnessEndHour                
            },
            "brightness" : {
                "max" : hourBrightnessMin, 
                "min" : hourBrightnessMax
            }
        }
    }
  }
  return newConfig;
  }

  useEffect(() => {
    if (brightnessConfig) {
      setSelectedOption(brightnessConfig.brightnessMode);
      mapBrightnessConfigToVar();
    }
  }, [brightnessConfig])

  function testConfig() {
    let body = mapVarToBrightnessConfig();
    dispatchRequest({ type: "setBrightness", params: { test: true }, body: { [selectedOption]: body.settings[selectedOption]} });
  }
  function resetConfig() {
    setBrightnessConfig({ ...config.brightness })
  }

  function submitConfig() {
    let body = mapVarToBrightnessConfig();

    dispatchRequest({ type: "setBrightness", body: {[selectedOption]: body.settings[selectedOption]} });
  }

  function onBrightnessChoice(val) {
    let id=this.id;
    let fnString="set"+id.charAt(0).toUpperCase()+id.slice(1);
    let fn=eval(fnString);
    fn(val);
  }

  useEffect(() => {
    if (requestState.url !== ""){
      runRequest(requestState);
    }
}, [requestState]);
useEffect(() => {
  if (response !== null){
    if (response.config.url==="/calibrateLdr"){
      setLdrDark(response.data.min);
      setLdrBright(response.data.max);
      
    }}
}, [response]);


  function calibrateLdr() {

    dispatchRequest({ type: "calibrateLdr" });

  }

  function fixedBrightness() {
    return (
      <BrightnessPicker id="fixedBrightnessValue" currentVal={fixedBrightnessValue} onBrightnessChoice={onBrightnessChoice} />
    );
  }
function setLdr(event){
}
  function ldrBrightness() {
    // calibrate button to give 60s time to get min and max values
    // with feedback: first 30s : minimal value, get value every 1s, last 30s : max value, get value every 30s
    return (
      <div>
        <div>
          <button id="calibrateLdr" onClick={calibrateLdr}>Calibrate LDR</button>
          <div style={{ marginLeft: "20px" }}> <input type="number" name="ldrDark" min={0} max={255} value={ldrDark} onChange={setLdr}/></div>
          <div style={{ marginLeft: "20px" }}> <input type="number" name="ldrBright" min={0} max={255} value={ldrBright} onChange={setLdr}/></div>
          </div>
      <div>
        <BrightnessPicker id="ldrBrightnessMin" currentVal={ldrBrightnessMin} onBrightnessChoice={onBrightnessChoice} />
        <BrightnessPicker id="ldrBrightnessMax" currentVal={ldrBrightnessMax} onBrightnessChoice={onBrightnessChoice} />
      </div>
      </div>
    );
    // pick min and max ldr sensor values
    // pick min and max brightness values
  }

  function hourBrightness() {
    // brightnessStartHour start of brightnessMax
    // brightnessEndHour start of brightnessMin
    return (
      <div>
        Starthour - max brightness
        <div>
          <input id="brightnessStartHour" type="number" min={0} max={23} value={brightnessStartHour} onBrightnessChoice={onBrightnessChoice} />
          <BrightnessPicker id="hourBrightnessMax" currentVal={hourBrightnessMax} onBrightnessChoice={onBrightnessChoice} />
        </div>
        Endhour - min brightness
        <div>
          <input id="brightnessStartHour" type="number" min={0} max={23} value={brightnessEndHour} onBrightnessChoice={onBrightnessChoice} />
          <BrightnessPicker id="hourBrightnessMin" currentVal={hourBrightnessMin} onBrightnessChoice={onBrightnessChoice} />
        </div>

      </div>
    );
  }
  return (
    <div>

      <h2>Helderheid</h2>
      <p>Stel de helderheid in! </p>
      Kies de helderheidsmode:
      <div>
        <button name="Test" type="button" onClick={testConfig}>Test</button><button name="Reset" type="button" onClick={resetConfig}>Reset</button><button name="Submit" type="button" onClick={submitConfig}>Submit</button></div>
      <input type="radio" value="fixedBrightness" name="brightnessMode" checked={selectedOption === "fixedBrightness"} onChange={onValueChange} /> <label>Vaste helderheid</label>
      <input type="radio" value="ldrBrightness" name="brightnessMode" checked={selectedOption === "ldrBrightness"} onChange={onValueChange} /><label>Helderheid volgens omgevingslicht</label>
      <input type="radio" value="hourBrightness" name="brightnessMode" checked={selectedOption === "hourBrightness"} onChange={onValueChange} /><label>Helderheid volgens tijdstip</label>
      <div style={{ marginTop: "20px" }}>
         {{
          fixedBrightness: brightnessConfig&&fixedBrightness(),
          ldrBrightness: brightnessConfig&&<LDRBrightness/>,
          hourBrightness: brightnessConfig&&hourBrightness(),
        }[selectedOption]}

      </div>
    </div>
  );
}

