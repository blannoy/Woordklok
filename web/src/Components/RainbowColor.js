import React, { useEffect, useState, useContext, useRef } from "react";
import { clockFaceContext } from "../Context/Context";
import ColorPicker from "./ColorPicker";
import tinycolor from "tinycolor2";

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
  const [cycleTime, setCycleTime] = useState(10);
  const [clockFaceConfig, setClockFaceConfig] = useContext(clockFaceContext);
  const [colorMap, setColorMap] = useState(null);
  const rainbowCycle = useRef();
  const firstColor=tinycolor({ h: prng(), s: 1, l: .5 });
  const [foregroundColor, setForegroundColor] = useState(firstColor);
  const [foregroundColorHex, setForegroundColorHex] = useState(firstColor.toHexString());


  // change in passed property, triggers update of state
  useEffect(() => {
    setColorConfig({ ...props.colorConfig });
    setCycleTime(props.colorConfig.cycleTime);
  }, [props.colorConfig])

  useEffect(() => {
    if (clockFaceConfig) {
      let colorMapArray = Array(clockFaceConfig.metadata.nrWords).fill(foregroundColorHex);
      colorMapArray[clockFaceConfig.metadata.backgroundIndex] = colorConfig.backgroundColor;
      setColorMap(colorMapArray);
      setClockFaceConfig({ ...clockFaceConfig, "colorMap": colorMapArray });
    }
  }, [colorConfig,foregroundColorHex]);

  useEffect(() => {
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
  }, [props.colorConfig]);

  function cycleTimeChange(event) {
    setCycleTime(event.target.value);
    setColorConfig({ ...props.colorConfig, cycleTime: event.target.value });
  }
  useEffect(() => {
    props.onColorConfig({ id: "cycleTime", value: cycleTime });
  }, [cycleTime])
  return (
    <div>
      <h3>Tijd (in s) waarna de klok terug van kleur verandert.</h3>
      <div>
        <input type="number" id="cycleTime" value={cycleTime} min="1" max="3600" onChange={cycleTimeChange} />
      </div>

      <h3>Kies een achtergrondkleur</h3>
      <div style={{ display: "inline-flex", justifyContent: "center" }}>
      <div style={{
                    backgroundColor: foregroundColorHex,
                    borderColor: "black",
                    borderWidth: "1px",
                    borderStyle: "solid",
                    width: '20px',
                    height: '20px',
                    marginRight: '10px'
                }}></div>
        <ColorPicker id="backgroundColor" currentVal={colorConfig.backgroundColor} foregroundColor={foregroundColorHex} onColorChoice={props.onColorConfig} />
      </div>
    </div>
  );
}
