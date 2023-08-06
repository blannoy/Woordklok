import React, { useContext, useEffect,useState } from "react";
import ColorPicker from "./ColorPicker"
//import { configContext,queryProviderContext} from "../Context/Context";
function calculate(color, hex = false) {
  if (color){
  if (hex) {
    return '#' +
      ("0" + (255 - parseInt(color.substring(1, 3), 16)).toString(16)).slice(-2) +
      ("0" + (255 - parseInt(color.substring(3, 5), 16)).toString(16)).slice(-2) +
      ("0" + (255 - parseInt(color.substring(5, 7), 16)).toString(16)).slice(-2);
  } else {
    return [(255 - color[0]), (255 - color[1]), (255 - color[2])];
  }}
}

export default function SingleColor(props) {
  const [colorConfig,setColorConfig]=useState(props.colorConfig);
  // const [config, setConfig] = useContext(configContext);
  // const [requestState, dispatchRequest]=useContext(queryProviderContext);
  //const [colorConfig,setColorConfig]=useState((config===null)?{ "color": "#ffffff", "backgroundColor": "#000000" }:{ ...config.colors.ledConfig.singleColor });

  console.log('-----');
  console.log(colorConfig);
  console.log('-----');

   useEffect(() => {
      setColorConfig({ ...props.colorConfig });
    }, [props.colorConfig])

  // function testConfig() {
  //   // call API with color config, indicating a test

  //   dispatchRequest({type:"setColor", body: {...colorConfig}});
  //   props.onColorConfig(colorConfig);
  // }

  // function submitConfig() {
  //   // call API with color config
  // }
  function colorHandler(pickerColorChoice) {
    var changedPar={...colorConfig};
    changedPar[pickerColorChoice["id"]]=pickerColorChoice.color;
    setColorConfig({...changedPar});
    props.onColorConfig(changedPar);
  }
  return (
    <div>
      <h3>Kies één kleur voor alle LEDs</h3>
      <div>
        <ColorPicker id="color" currentVal={colorConfig.color} onColorChoice={colorHandler} />
      </div>
      <h3>Kies een achtergrondkleur</h3>
      <div>
        <ColorPicker id="backgroundColor" currentVal={colorConfig.backgroundColor} complementaryColor={calculate(colorConfig.color,true)} onColorChoice={colorHandler} />
      </div>
      {/* <button name="Test" type="button" onClick={testConfig}>Test</button>
      <button name="Submit" type="button" onClick={submitConfig}>Submit</button> */}
    </div>
  );
}
