import React, { useEffect, useState, useContext, useRef } from "react";
import { clockContext } from "../../Context/Context";
import ColorPicker from "./ColorPicker";
import tinycolor from "tinycolor2";
import isEqual from 'lodash/isEqual';

/**
 * Builds screen for selection of the rainbow cycle time and  background color for non active LEDs
 * Possibility to select the complementary color for the background.
 * @param {*} props Should receive the JSON representation of a "rainbowColor" configuration
 * @returns 
 */
export default function RainbowColor(props) {
  let seedrandom = require("seedrandom")
  let prng = seedrandom('added entropy.', { entropy: true });

  const [colorConfig, setColorConfig] = useState(props.colorConfig);
  const [fullConfig, setFullConfig] = useContext(clockContext);
  const [colorMap, setColorMap] = useState(null);
  const [cycleTime, setCycleTime] = useState(undefined);
  const [clockVisible, setClockVisible] = useState(props.clockVisible);
  const rainbowCycle = useRef();
  const firstColor=tinycolor({ h: prng(), s: 1, l: .5 });
  const [foregroundColor, setForegroundColor] = useState(firstColor);
  const [foregroundColorHex, setForegroundColorHex] = useState(firstColor.toHexString());
 


  // change in passed property, triggers update of state
  useEffect(() => {
  //  console.log("RainbowColor: props.colorConfig", JSON.stringify(props.colorConfig), "-", JSON.stringify(colorConfig));
    setColorConfig({ ...props.colorConfig });
    setCycleTime(props.colorConfig.cycleTime);
    props.setClockColors({ 'rainbowColor': { 'cycleTime': colorConfig.cycleTime, 'backgroundColor': colorConfig.backgroundColor } });
  
  }, [props.colorConfig])

     useEffect(() => {
   //   console.log("RainbowColor: colorConfig", JSON.stringify(colorConfig), "-", foregroundColorHex);
      if (fullConfig !== undefined && fullConfig.clockface !== undefined) {
        let colorMapArray = Array(fullConfig.clockface.metadata.nrWords).fill(foregroundColorHex);
        colorMapArray[fullConfig.clockface.metadata.backgroundIndex] = colorConfig.backgroundColor;
        if (isEqual(colorMapArray, colorMap) === false) {
          setColorMap(colorMapArray);
          //setClockFaceConfig({ ...clockFaceConfig, "colorMap": colorMapArray });
          setFullConfig({ ...fullConfig, colorMap: colorMapArray });
        }
      }
    }, [colorConfig,foregroundColorHex]);

  useEffect(() => {
    if (props.clockVisible === true) {
    if (rainbowCycle.current) {
      clearInterval(rainbowCycle.current);
      rainbowCycle.current = undefined;
    }
    rainbowCycle.current = setInterval(() => {
      const newColor=tinycolor(foregroundColor).spin(1);
      setForegroundColor(newColor);
      setForegroundColorHex(newColor.toHexString());
    }
      , colorConfig.cycleTime * 1000);
    return () => clearInterval(rainbowCycle.current);
  }
  }, [props.colorConfig,props.clockVisible]); 

  function cycleTimeChange(value) {
    setCycleTime(value);
    setColorConfig({ ...props.colorConfig, cycleTime: value });
  } 
  useEffect(() => {
    if (cycleTime !== undefined && cycleTime !== props.colorConfig.cycleTime) {
      props.onColorConfig({ id: "cycleTime", value: cycleTime });
    }
  }, [cycleTime])

  return (
    <div>
      <h3>Tijd (in s) waarna de klok terug van kleur verandert.</h3>
      <div>
        <input type="number" id="cycleTime" value={cycleTime !== undefined ? cycleTime : 0} min="1" max="3600" onChange={e=>cycleTimeChange(e.target.value)} />
      </div>

      <h3>Kies een achtergrondkleur</h3>
      <div>
         <ColorPicker id="backgroundColor" currentVal={colorConfig.backgroundColor} foregroundColor={foregroundColorHex} onColorChoice={props.onColorConfig} /> 
      </div>
    </div>
  );
}
