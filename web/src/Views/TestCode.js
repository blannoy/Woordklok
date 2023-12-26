import React, { useEffect, useState } from "react";
import ColorPicker from "../Components/Color/ColorPicker";
import { useContext } from "react";
import { clockFaceContext } from "../Context/Context";

/**
 * Builds screen for selection of a single color for clock-words and background color for non active LEDs
 * Possibility to select the complementary color for the background.
 * @param {*} props Should receive the JSON representation of a "singleColor" configuration
 * @returns 
 */
export default function TestCode(props) {
  const [colorConfig,setColorConfig]=useState(props.colorConfig);
  const [clockFaceConfig,setClockFaceConfig]=useContext(clockFaceContext);
  const [colorMap,setColorMap]=useState(null);
  const [colconf,setColconf]=useState({color:"#ff0000", backgroundColor:"#000000"});
  // change in passed property, triggers update of state

  function onColorConfig(event) {
    //console.log("onColorConfig: " + event.id + " " + event.value);
    switch (event.id) {
      case "color":
        setColconf({...colconf,color:event.value});
        break;
    case "backgroundColor":
      setColconf({...colconf,backgroundColor:event.value});
      break;
      default:
        break;
    }
  }

  return (
    <div>
      <h3>Kies één kleur voor alle LEDs</h3>
      <div>
        <ColorPicker id="color" currentVal={colconf.color} onColorChoice={onColorConfig} />
      </div>
      <h3>Kies een achtergrondkleur</h3>
      <div>
        <ColorPicker id="backgroundColor" currentVal={colconf.backgroundColor} foregroundColor={colconf.color} onColorChoice={onColorConfig} />
      </div>
    </div>
  );
}
