import React, { useState, useContext, useEffect } from "react";
import { configContext, queryProviderContext } from "../Context/Context";
import LDRBrightness from "../Components/Brightness/LDRBrightness";
import FixedBrightness from "../Components/Brightness/FixedBrightness";
import TimeBrightness from "../Components/Brightness/TimeBrightness";
// import LDRBrightness from "../Components/LDRBrightness";

function Brightness() {
  const [config, setConfig] = useContext(configContext);
  const [requestState, dispatchRequest] = useContext(queryProviderContext);

  const [brightnessConfig, setBrightnessConfig] = useState(config && config.brightness);
  const [selectedOption, setSelectedOption] = useState(config ? config.brightness.brightnessMode : "fixedBrightness");

  const [fixedBrightnessConfig, setFixedBrightnessConfig] = useState({});
  const [ldrBrightnessConfig, setLdrBrightnessConfig] = useState({});
  const [timeBrightnessConfig, settimeBrightnessConfig] = useState({});


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
    let settings = { ...brightnessConfig.settings };
    setFixedBrightnessConfig(settings.fixedBrightness);
    setLdrBrightnessConfig(settings.ldrBrightness);
    settimeBrightnessConfig(settings.timeBrightness);
  }

  function mapVarToBrightnessConfig() {
    let newConfig = {};
    newConfig[selectedOption] = {...brightnessConfig.settings[selectedOption]};
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
    let tempConfig = { ...brightnessConfig };
    tempConfig.settings[selectedOption] = { ...val };
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
      <input type="radio" value="timeBrightness" name="brightnessMode" checked={selectedOption === "timeBrightness"} onChange={onValueChange} /><label>Helderheid volgens tijdstip</label>
      <div style={{ marginTop: "20px" }}>
        {{
          fixedBrightness: brightnessConfig && <FixedBrightness currentVal={fixedBrightnessConfig} onBrightnessChoice={onBrightnessChoice} />,
          ldrBrightness: brightnessConfig && <LDRBrightness currentVal={ldrBrightnessConfig} onBrightnessChoice={onBrightnessChoice} />,
          timeBrightness: brightnessConfig && <TimeBrightness currentVal={timeBrightnessConfig} onBrightnessChoice={onBrightnessChoice} />,
        }[selectedOption]}

      </div>
    </div>
  );
}

export default React.memo(Brightness);