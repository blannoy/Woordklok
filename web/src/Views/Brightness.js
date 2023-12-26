import React, { useState, useContext, useEffect} from "react";
import { configContext, queryProviderContext } from "../Context/Context";
import LDRBrightness from "../Components/Brightness/LDRBrightness";
import FixedBrightness from "../Components/Brightness/FixedBrightness";
import HourBrightness from "../Components/Brightness/HourBrightness";
// import LDRBrightness from "../Components/LDRBrightness";

export default function Brightness() {
  const [config, setConfig] = useContext(configContext);
  const [requestState, dispatchRequest] = useContext(queryProviderContext);

  const [brightnessConfig, setBrightnessConfig] = useState(config && config.brightness);
  const [selectedOption, setSelectedOption] = useState(config ? config.brightness.brightnessMode : "fixedBrightness");

  const [fixedBrightnessConfig, setFixedBrightnessConfig] = useState({});
  const [ldrBrightnessConfig, setLdrBrightnessConfig] = useState({});
  const [hourBrightnessConfig, setHourBrightnessConfig] = useState({});


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
    setSelectedOption(brightnessConfig.brightnessMode);
    let settings={...brightnessConfig.settings};
    setFixedBrightnessConfig(settings.fixedBrightness);
    setLdrBrightnessConfig(settings.ldrBrightness);
    setHourBrightnessConfig(settings.hourBrightness);
  }

  function mapVarToBrightnessConfig(){
    let newConfig={"brightnessMode" : selectedOption,
    "settings" : {
        "fixedBrightness" : fixedBrightnessConfig,
        "ldrBrightness" : ldrBrightnessConfig,
        "hourBrightness": hourBrightnessConfig
  }
}
  return newConfig;
  }

  useEffect(() => {
    if (brightnessConfig) {
      mapBrightnessConfigToVar();
    }
  }, [brightnessConfig])

  function testConfig() {
    dispatchRequest({ type: "setBrightness", params: { test: true }, body: mapVarToBrightnessConfig() });
  }

  function resetConfig() {
    setBrightnessConfig({ ...config.brightness })
  }

  function submitConfig() {
    let body = mapVarToBrightnessConfig();
    dispatchRequest({ type: "setBrightness", body: {[selectedOption]: body.settings[selectedOption]} });
  }

  function onBrightnessChoice(val) {
    let tempConfig={...brightnessConfig};
    tempConfig.settings[selectedOption]={...val};
    setBrightnessConfig(tempConfig);
/*    let id=this.id;
    let fnString="set"+id.charAt(0).toUpperCase()+id.slice(1);
    let fn=eval(fnString);
    fn(val);*/
  }

  // function hourBrightnessComp() {
  //   // brightnessStartHour start of brightnessMax
  //   // brightnessEndHour start of brightnessMin
  //   return (
  //     <div>
  //       Starthour - max brightness
  //       <div>
  //         <input id="brightnessStartHour" type="number" min={0} max={23} value={brightnessStartHour} onBrightnessChoice={onBrightnessChoice} />
  //         <BrightnessPicker id="hourBrightnessMax" currentVal={hourBrightnessMax} onBrightnessChoice={onBrightnessChoice} />
  //       </div>
  //       Endhour - min brightness
  //       <div>
  //         <input id="brightnessStartHour" type="number" min={0} max={23} value={brightnessEndHour} onBrightnessChoice={onBrightnessChoice} />
  //         <BrightnessPicker id="hourBrightnessMin" currentVal={hourBrightnessMin} onBrightnessChoice={onBrightnessChoice} />
  //       </div>

  //     </div>
  //   );
  // }
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
          fixedBrightness: brightnessConfig&&<FixedBrightness currentVal={fixedBrightnessConfig} onBrightnessChoice={onBrightnessChoice}/>,
          ldrBrightness: brightnessConfig&&<LDRBrightness currentVal={ldrBrightnessConfig} onBrightnessChoice={onBrightnessChoice}/>,
          hourBrightness: brightnessConfig&&<HourBrightness currentVal={hourBrightnessConfig} onBrightnessChoice={onBrightnessChoice}/>,
        }[selectedOption]}

      </div>
    </div>
  );
}

