import React, { useState,useContext,useEffect } from "react";
import SingleColor from "../Components/SingleColor";
import { configContext,queryProviderContext } from "../Context/Context";

export default function LEDColors() {
  const [config, setConfig] = useContext(configContext);
  const [requestState, dispatchRequest]=useContext(queryProviderContext);
  const [selectedOption, setSelectedOption] = useState(config ? config.colors.ledMode :"singleColor");
  const [colorConfig,setColorConfig]=useState(config&&selectedOption ? { selectedOption: {...config.colors.ledConfig[selectedOption]} }:{});


    // keep config in sync when changed
  useEffect(() => {
    if (config!==null) {
       setColorConfig({ ...config.colors.ledConfig[selectedOption] });
     }
   }, [config,selectedOption])

  function onValueChange(event) {
    setSelectedOption(event.target.value);
  }
  
  function colorHandler(newColorConfig) {
    setColorConfig({...newColorConfig});
  }

  function testConfig() {
    var body={};
    body[selectedOption]={...colorConfig};
    dispatchRequest({type:"setColor", params: {test:true},body: body});
  }
  function resetConfig() {
    setColorConfig({ ...config.colors.ledConfig[selectedOption] })
  }

  function submitConfig() {
    var body={};
    body[selectedOption]={...colorConfig};
    dispatchRequest({type:"setColor", body: body});
  }


  return (
    <div>
      <h2>Colors</h2>
      <p>Stel hier de kleuren in!</p>
      <div>
        <div>      <button name="Test" type="button" onClick={testConfig}>Test</button><button name="Reset" type="button" onClick={resetConfig}>Reset</button><button name="Submit" type="button" onClick={submitConfig}>Submit</button></div>
        <input type="radio" value="singleColor" name="colorMode" checked={selectedOption === "singleColor"} onChange={onValueChange} /> <label>Eén kleur</label>
        <input type="radio" value="rainBow" name="colorMode" checked={selectedOption === "rainBow"} onChange={onValueChange} /><label>Regenboog</label>
        <input type="radio" value="wordColor" name="colorMode" checked={selectedOption === "wordColor"} onChange={onValueChange} /><label>Woordkleur</label>
        <input type="radio" value="hourlyColor" name="colorMode" checked={selectedOption === "hourlyColor"} onChange={onValueChange} /><label>Kleur per uur</label>
      </div>
      <div>
        <div>
          {{
            singleColor: <SingleColor colorConfig={colorConfig} onColorConfig={colorHandler} />,
            rainBow: <p>rainbow color</p>,
            wordColor: <p>color per word</p>,
            hourlyColor: <p>single color every hour</p>
          }[selectedOption]}
        </div>
      </div>
    </div>
  );
}
