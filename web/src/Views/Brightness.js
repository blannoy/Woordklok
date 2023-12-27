import React, { useState, useContext, useEffect} from "react";
import { configContext, queryProviderContext } from "../Context/Context";
import LDRBrightness from "../Components/Brightness/LDRBrightness";
import FixedBrightness from "../Components/Brightness/FixedBrightness";
import HourBrightness from "../Components/Brightness/HourBrightness";
// import LDRBrightness from "../Components/LDRBrightness";

function Brightness() {
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
    dispatchRequest({ type: 'LOADCONFIG' });
  }

  function submitConfig() {
    dispatchRequest({ type: "setBrightness", body: mapVarToBrightnessConfig() });
  }

  function onBrightnessChoice(val) {
    let tempConfig={...brightnessConfig};
    tempConfig.settings[selectedOption]={...val};
    setBrightnessConfig(tempConfig);
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
          fixedBrightness: brightnessConfig&&<FixedBrightness currentVal={fixedBrightnessConfig} onBrightnessChoice={onBrightnessChoice}/>,
          ldrBrightness: brightnessConfig&&<LDRBrightness currentVal={ldrBrightnessConfig} onBrightnessChoice={onBrightnessChoice}/>,
          hourBrightness: brightnessConfig&&<HourBrightness currentVal={hourBrightnessConfig} onBrightnessChoice={onBrightnessChoice}/>,
        }[selectedOption]}

      </div>
    </div>
  );
}

export default React.memo(Brightness);