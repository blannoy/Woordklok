import React, { useEffect, useState } from "react";
import ColorPicker from "./ColorPicker";
import { calculateComplementary } from "../Utils/Utils";
import { useContext } from "react";
import { clockFaceContext } from "../Context/Context";

/**
 * Builds screen for selection of a single color for clock-words and background color for non active LEDs
 * Possibility to select the complementary color for the background.
 * @param {*} props Should receive the JSON representation of a "singleColor" configuration
 * @returns 
 */
export default function SingleColor(props) {
  const [colorConfig,setColorConfig]=useState(props.colorConfig);
  const [clockFaceConfig,setClockFaceConfig]=useContext(clockFaceContext);
  const [colorMap,setColorMap]=useState(null);
  // change in passed property, triggers update of state
   useEffect(() => {
      setColorConfig({ ...props.colorConfig });
      props.setClockColors({ 'singleColor': { 'foreground' : colorConfig.color, 'background' : colorConfig.backgroundColor}});
      // loop over number of words, assign singlecolor; on index backgroundindex, assign backgroundcolor
   }, [props.colorConfig]);

   useEffect(() => {
    if (clockFaceConfig){
      let colorMapArray=Array(clockFaceConfig.metadata.nrWords).fill(colorConfig.color);
      colorMapArray[clockFaceConfig.metadata.backgroundIndex]=colorConfig.backgroundColor;
      setColorMap(colorMapArray);
      setClockFaceConfig({...clockFaceConfig,"colorMap":colorMapArray});
    }
  },[colorConfig]);

  return (
    <div>
      <h3>Kies één kleur voor alle LEDs</h3>
      <div>
        <ColorPicker id="color" currentVal={colorConfig.color} onColorChoice={props.onColorConfig} />
      </div>
      <h3>Kies een achtergrondkleur</h3>
      <div>
        <ColorPicker id="backgroundColor" currentVal={colorConfig.backgroundColor} foregroundColor={colorConfig.color} onColorChoice={props.onColorConfig} />
      </div>
    </div>
  );
}
