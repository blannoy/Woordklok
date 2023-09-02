import React, { useState,useContext,useEffect } from "react";
import SingleColor from "../Components/SingleColor";
import { configContext,queryProviderContext } from "../Context/Context";
import RainbowColor from "../Components/RainbowColor";
import WordColor from "../Components/WordColor";
import HourlyColor from "../Components/HourlyColor";
import ClockFace from "../Components/ClockFace";

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
    setColorConfig({ ...config.colors.ledConfig[event.target.value] });
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

  /**
   * Reconstruct color config and pass it to parent 
   * @param {*} configChoice
   * @todo lift to top level as a generic function?
   */
  function configHandler(configChoice) {
    var changedPar={...colorConfig};
    changedPar[configChoice["id"]]=configChoice.value;
    setColorConfig(changedPar);
  }

  return (
    <div>
      <h2>Colors</h2>
      <p>Stel hier de kleuren in!</p>
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
            singleColor: <SingleColor colorConfig={colorConfig} onColorConfig={configHandler} />,
            rainbowColor: <RainbowColor colorConfig={colorConfig} onColorConfig={configHandler}/>,
            wordColor: <WordColor colorConfig={colorConfig} onColorConfig={configHandler}/>,
            hourlyColor: <HourlyColor colorConfig={colorConfig} onColorConfig={configHandler}/>,
          }[selectedOption]}
          <div>
            <ClockFace colorConfig={colorConfig} colorOption={selectedOption}/>
          </div>
        </div>
      </div>
    </div>
  );
}
