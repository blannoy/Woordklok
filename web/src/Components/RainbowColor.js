import React, { useEffect, useState } from "react";
import ColorPicker from "./ColorPicker";

/**
 * Builds screen for selection of the rainbow cycle time and  background color for non active LEDs
 * Possibility to select the complementary color for the background.
 * @param {*} props Should receive the JSON representation of a "rainbowColor" configuration
 * @returns 
 */
export default function RainbowColor(props) {
  const [colorConfig,setColorConfig]=useState(props.colorConfig);
  const [cycleTime,setCycleTime]=useState(10);

  // change in passed property, triggers update of state
   useEffect(() => {
    setColorConfig({ ...props.colorConfig });
    setCycleTime(props.colorConfig.cycleTime);
    }, [props.colorConfig])
  
    function cycleTimeChange(event) {
      setCycleTime(event.target.value);
      setColorConfig({ ...props.colorConfig, cycleTime: event.target.value});
    }
    useEffect(() => {
          props.onColorConfig({ id: "cycleTime", value: cycleTime });
  }, [cycleTime])
  return (
    <div>
      <h3>Tijd (in s) waarna de klok terug van kleur verandert.</h3>
      <div>
        <input type="number" id="cycleTime" value={cycleTime} min="1" max="3600" onChange={cycleTimeChange}/>
      </div>
      <h3>Kies een achtergrondkleur</h3>
      <div>
        <ColorPicker id="backgroundColor" currentVal={colorConfig.backgroundColor} foregroundColor= onColorChoice={props.onColorConfig} />
      </div>
    </div>
  );
}
