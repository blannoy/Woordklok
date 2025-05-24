import React, { useState, useContext, useEffect } from "react";
import { clockContext } from "../Context/Context";
import LDRBrightness from "../Components/Brightness/LDRBrightness";
import FixedBrightness from "../Components/Brightness/FixedBrightness";
import TimeBrightness from "../Components/Brightness/TimeBrightness";
import { useSetBrightnessMutation, useLazyGetConfigQuery, useTestBrightnessMutation } from "../Components/ClockAPI";
import { has } from "lodash";
// import LDRBrightness from "../Components/LDRBrightness";

function Brightness() {
  const [fullConfig, setFullConfig] = useContext(clockContext);
  const [config, setConfig] = useState(fullConfig ? fullConfig.config : undefined);
  const [setBrightness,] = useSetBrightnessMutation();
  const [testBrightness,] = useTestBrightnessMutation();
  const [getConfig, { data: configData, error: configError, isLoading: configIsLoading }] = useLazyGetConfigQuery();
  const [brightnessConfig, setBrightnessConfig] = useState(config && config.brightness);
  const [selectedOption, setSelectedOption] = useState(config ? config.brightness.brightnessMode : "fixedBrightness");

  const [fixedBrightnessConfig, setFixedBrightnessConfig] = useState({});
  const [ldrBrightnessConfig, setLdrBrightnessConfig] = useState({});
  const [timeBrightnessConfig, settimeBrightnessConfig] = useState({});
  const [hasLDR, setHasLDR] = useState(false);
 useEffect(() => {
     if (fullConfig !== undefined && fullConfig.config !== undefined) {
       setConfig({ ...fullConfig.config });
      if (
        Array.isArray(fullConfig.config.sensors) &&
        fullConfig.config.sensors.some(sensor => sensor.name === "ldr")
      ) {
        setHasLDR(true);
      }
      }
   }, [fullConfig]); 

  function onValueChange(event) {
    setSelectedOption(event.target.value);
    setBrightnessConfig({ ...config.brightness, "brightnessMode": event.target.value });
  }
  // keep config in sync when changed
  useEffect(() => {
    if (config !== null && config !== undefined && config.brightness !== undefined) {
      setBrightnessConfig({ ...config.brightness });
    }
  }, [config])

  function mapBrightnessConfigToVar() {
    setSelectedOption(brightnessConfig.brightnessMode);
    let settings = { ...brightnessConfig.settings };
    setFixedBrightnessConfig(settings.fixedBrightness);
    if (hasLDR) {setLdrBrightnessConfig(settings.ldrBrightness)};
    settimeBrightnessConfig(settings.timeBrightness);
  }

  function mapVarToBrightnessConfig() {
    let newConfig = {};
    newConfig[selectedOption] = { ...brightnessConfig.settings[selectedOption] };
    return newConfig;
  }

  useEffect(() => {
    if (brightnessConfig) {
      mapBrightnessConfigToVar();
    }
  }, [brightnessConfig])


    async function resetConfig() {
    ////dispatchRequest({ type: 'LOADCONFIG' });
    try {
      const payload = await getConfig().unwrap();
      setFullConfig({...payload});
    } catch { }

  }
  
  async function submitConfig() {
        try {
      const payload=await setBrightness(mapVarToBrightnessConfig()).unwrap();
      
    } catch (error) {
      console.error("Error testing brightness config:", error);
    }

  }

  function onBrightnessChoice(val) {
    var tempConfig = { ...brightnessConfig, settings: { ...brightnessConfig.settings, [selectedOption]: {...val} } };
    //var tempConfig = { ...brightnessConfig };

 //   tempConfig.settings[selectedOption] = { ...val };
    setBrightnessConfig(tempConfig);
  }

  return (
    <div>
      <h2>Helderheid</h2>
      <p>Stel de helderheid in! </p>
      Kies de helderheidsmode:
      <div>
        {/* <button name="Test" type="button" onClick={testConfig}>Test</button> */}
        <button name="Reset" type="button" onClick={resetConfig}>Reset</button><button name="Submit" type="button" onClick={submitConfig}>Submit</button></div>
      <input type="radio" value="fixedBrightness" name="brightnessMode" checked={selectedOption === "fixedBrightness"} onChange={onValueChange} /> <label>Vaste helderheid</label>
      { hasLDR &&
      <>
        <input type="radio" value="ldrBrightness" name="brightnessMode" checked={selectedOption === "ldrBrightness"} onChange={onValueChange} /><label>Helderheid volgens omgevingslicht</label>
      </>}
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