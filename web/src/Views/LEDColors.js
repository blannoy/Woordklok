import React, { useState, useContext, useEffect } from "react";
import SingleColor from "../Components/Color/SingleColor";
import { configContext, queryProviderContext } from "../Context/Context";
import RainbowColor from "../Components/Color/RainbowColor";
import WordColor from "../Components/Color/WordColor";
import HourlyColor from "../Components/Color/HourlyColor";
import ClockFace from "../Components/ClockFace";
import { useMediaQuery } from 'react-responsive';
import { useGetConfigQuery } from "../Components/ClockAPI";

function LEDColors() {
  const { data, error, isLoading } = useGetConfigQuery();
  const [config, setConfig] = useState(undefined)
  /// const [config,] = useContext(configContext);
  const [, dispatchRequest] = useContext(queryProviderContext);
  const [selectedOption, setSelectedOption] = useState(config ? config.colors.ledMode : "singleColor");
  const [colorConfig, setColorConfig] = useState(config && selectedOption ? { selectedOption: { ...config.colors.ledConfig[selectedOption] } } : {});
  const [showPreview, setShowPreview] = useState(false);
  const [clockColors, setClockColors] = useState({})

  const isTooSmall = !useMediaQuery({
    query: '(min-width: 830px)'
  })

  function previewChangeHandler() {
    setShowPreview(!showPreview);
  }


  useEffect(() => {
    if (data !== undefined) {
      setConfig(data);
    }
  }, [data]);
  // keep config in sync when changed
  useEffect(() => {
    if (config !== undefined) {
      setColorConfig({ ...config.colors.ledConfig[selectedOption] });
    }
  }, [config, selectedOption])

  function onValueChange(event) {
    setSelectedOption(event.target.value);
    setColorConfig({ ...config.colors.ledConfig[event.target.value] });
  }

  function testConfig() {
    //    var body={...config.colors};
    let body = {};
    //body.ledMode=selectedOption;
    body[selectedOption] = { ...colorConfig };
    dispatchRequest({ type: "setColor", params: { test: true }, body: body });
  }
  function resetConfig() {
    dispatchRequest({ type: 'LOADCONFIG' });
  }

  function submitConfig() {
    //  var body={...config.colors};
    let body = {};
    //body.ledMode=selectedOption;
    body[selectedOption] = { ...colorConfig };
    dispatchRequest({ type: "setColor", body: body });
  }

  /**
   * Reconstruct color config and pass it to parent 
   * @param {*} configChoice
   * @todo lift to top level as a generic function?
   */
  function configHandler(configChoice) {
    var changedPar = { ...colorConfig };
    changedPar[configChoice["id"]] = configChoice.value;
    setColorConfig(changedPar);
  }

  return (
<div>
      <h2>Colors</h2>
      <p>Stel hier de kleuren in!</p>
      {colorConfig && <div>
        <div>
        <div>      <button name="Test" type="button" onClick={testConfig}>Test</button><button name="Reset" type="button" onClick={resetConfig}>Reset</button><button name="Submit" type="button" onClick={submitConfig}>Submit</button></div>
        <input type="radio" value="singleColor" name="colorMode" checked={selectedOption === "singleColor"} onChange={onValueChange} /> <label>Eén kleur</label>
        <input type="radio" value="rainbowColor" name="colorMode" checked={selectedOption === "rainbowColor"} onChange={onValueChange} /><label>Regenboog</label>
        <input type="radio" value="wordColor" name="colorMode" checked={selectedOption === "wordColor"} onChange={onValueChange} /><label>Woordkleur</label>
        <input type="radio" value="hourlyColor" name="colorMode" checked={selectedOption === "hourlyColor"} onChange={onValueChange} /><label>Kleur per uur</label>
      </div>
      <div>
        <div>
          {{
            singleColor: <SingleColor colorConfig={colorConfig} onColorConfig={configHandler} setClockColors={setClockColors} />,
            rainbowColor: <RainbowColor colorConfig={colorConfig} onColorConfig={configHandler} setClockColors={setClockColors} />,
            wordColor: <WordColor colorConfig={colorConfig} onColorConfig={configHandler} setClockColors={setClockColors} />,
            hourlyColor: <HourlyColor colorConfig={colorConfig} onColorConfig={configHandler} setClockColors={setClockColors} />,
          }[selectedOption]}

          {(selectedOption !== "hourlyColor" && !isTooSmall) && (
            <div>
              <div>
                <input type="checkbox" value="preview" name="showPreview" checked={showPreview} onChange={previewChangeHandler} /><label>Toon voorbeeld</label>
              </div>
              <div>
                {showPreview && <ClockFace />}
              </div>
            </div>)}
        </div>
      </div>
    </div>}
    </div>
  );
}

export default React.memo(LEDColors);