import React, { useEffect, useState, useContext } from "react";
import ColorPicker from "./ColorPicker";
import { clockContext } from "../../Context/Context";
import isEqual from 'lodash/isEqual';

const deepEqual = require('deep-equal');

/**
 * Builds screen for selection of a color for each clock-word and a global background color for non active LEDs
 * @param {*} props Should receive the JSON representation of a "wordColor" configuration
 * @returns 
 */
export default function WordColor(props) {
  const [colorConfig, setColorConfig] = useState(props.colorConfig);
  const [fullConfig, setFullConfig] = useContext(clockContext);
  const [clockFaceConfig,setClockFaceConfig]=useState((fullConfig!==undefined)?fullConfig.clockface:undefined);

  const [clockWords, setClockWords] = useState((clockFaceConfig ? clockFaceConfig.layout : []));
  const [colorList, setColorList] = useState(colorConfig.color ? colorConfig.color : []);
  const [colorMap, setColorMap] = useState(null);

    useEffect(() => {
      if (fullConfig !== undefined && fullConfig.clockFaceConfig !== undefined) {
        setClockFaceConfig({ ...fullConfig.clockface });
      }
  }, [fullConfig]); 
  // change in passed property, triggers update of state
  useEffect(() => {
    if (!deepEqual(props.colorConfig, colorConfig)) {
      setColorConfig({ ...props.colorConfig });
      setColorList(props.colorConfig.color);
    }
  }, [props.colorConfig])
  // 
  useEffect(() => {
    setClockWords(clockFaceConfig ? clockFaceConfig.layout : [])
  }, [clockFaceConfig])

  useEffect(() => {
        if (fullConfig !== undefined && fullConfig.clockface !== undefined) {
      let colorMapArray = colorList;
          if (isEqual(colorMapArray, colorMap) === false) {
            setColorMap(colorMapArray);
            //setClockFaceConfig({ ...clockFaceConfig, "colorMap": colorMapArray });
            setFullConfig({ ...fullConfig, colorMap: colorMapArray });
          }
        }
  }, [colorList, colorConfig]);

  function transformConfig(event) {
    if (event.value) {
      const pos = Number((event.id).split("_")[1]);
      const color = event.value;
      if (color !== colorList[pos]) {
        var tempConfig= [...colorConfig.color];
        //var tempConfig = { ...colorConfig };
        //tempConfig.color[pos] = color;
        tempConfig[pos] = color;
        props.onColorConfig({ id: "color", value: tempConfig });
      }
    }
  }
  return (
    <div>

      <h3>Kies een achtergrondkleur</h3>
      <div>
        <ColorPicker id="backgroundColor" currentVal={colorConfig.backgroundColor} onColorChoice={props.onColorConfig} />
      </div>
      <h3>Kies een kleur voor elk woord</h3>
      { clockWords &&
      <div>
        {clockWords.map((clockWord, index) => {
          const colorIndex = "color_" + String(index);
          if (clockWord.word) {
            return <div className="section" key={clockWord.word+String(index)} ><div className="smallLabelCell"> <label>{clockWord.word}</label></div><div className="wideBodyCell"><ColorPicker id={colorIndex} currentVal={colorList[index]} onColorChoice={transformConfig} /></div></div>

        }
        })}
      </div>}
      {/* <ColorPicker id="color" currentVal={colorConfig.color} onColorChoice={props.onColorConfig} /> */}
    </div>
  );
}
