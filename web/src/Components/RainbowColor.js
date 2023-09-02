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

  // change in passed property, triggers update of state
   useEffect(() => {
      setColorConfig({ ...props.colorConfig });
    }, [props.colorConfig])

    useEffect(() => {
      const interval = setInterval(() => {
      
      }, colorConfig.cycleTime*1000);
  
      return () => clearInterval(interval);
    }, []);
  return (
    <div>
      <h3>Tijd (in s) waarna de klok terug van kleur verandert.</h3>
      <div>
        <input type="number" id="cycleTime" value={colorConfig.cycleTime?? 10} min="1" max="3600" onChange={props.onColorConfig}></input>
      </div>
      <h3>Kies een achtergrondkleur</h3>
      <div>
        <ColorPicker id="backgroundColor" currentVal={colorConfig.backgroundColor} complementaryColor="#xxxxxx" onColorChoice={props.onColorConfig} />
      </div>
    </div>
  );
}
