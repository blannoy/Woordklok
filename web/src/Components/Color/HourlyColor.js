import React, { useEffect, useState, useContext } from "react";
import ColorPicker from "./ColorPicker";
import { configContext } from "../../Context/Context";

/**
 * Builds screen for selection of a color for each clock-word and a global background color for non active LEDs
 * @param {*} props Should receive the JSON representation of a "wordColor" configuration
 * @returns 
 */
export default function HourlyColor(props) {
  const [colorConfig,setColorConfig]=useState(props.colorConfig);
  const [config, setConfig] = useContext(configContext);
  const [colorList,setColorList]=useState(colorConfig.color?colorConfig.color:[]);


  // change in passed property, triggers update of state
   useEffect(() => {
      setColorConfig({ ...props.colorConfig });
      setColorList(props.colorConfig.color);
    }, [props.colorConfig])


    function transformConfig(event) {
      if (event.value){
        const pos=Number((event.id).split("_")[1]);
        const color=event.value;
        var tempConfig={...colorConfig};
        tempConfig.color[pos]=color;
        props.onColorConfig({id:"color", value: tempConfig.color});
    }
    }
  return (
    <div>

      <h3>Kies een achtergrondkleur</h3>
      <div>
        <ColorPicker id="backgroundColor" currentVal={colorConfig.backgroundColor} onColorChoice={props.onColorConfig} />
      </div>
      <h3>Kies een kleur voor elk uur (0 - 23h)</h3>
      
      <div>
      { [...Array(24)].map((value,index)=> {
        const colorIndex="color_"+String(index);
        return <div style={{display:"flex", marginTop:"20px"}}><div style={{flex: 0.05}}> <label>{String(index)+":00"}</label></div><div style={{flex:0.9}}><ColorPicker id={colorIndex} currentVal={colorList[index]} onColorChoice={transformConfig} /></div></div>
       })}
      </div> 
    </div>
  );
}