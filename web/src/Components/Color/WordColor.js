import React, { useEffect, useState, useContext } from "react";
import ColorPicker from "./ColorPicker";
import { configContext, clockFaceContext } from "../../Context/Context";
const deepEqual = require('deep-equal');
/**
 * Builds screen for selection of a color for each clock-word and a global background color for non active LEDs
 * @param {*} props Should receive the JSON representation of a "wordColor" configuration
 * @returns 
 */
export default function WordColor(props) {
  const [colorConfig, setColorConfig] = useState(props.colorConfig);
  const [clockFaceConfig, setClockFaceConfig] = useContext(clockFaceContext);
  const [config, setConfig] = useContext(configContext);
  const [clockWords, setClockWords] = useState((config ? config.clockface.layout : []));
  const [colorList, setColorList] = useState(colorConfig.color ? colorConfig.color : []);


  // change in passed property, triggers update of state
  useEffect(() => {
    if (!deepEqual(props.colorConfig, colorConfig)) {
      setColorConfig({ ...props.colorConfig });
      setColorList(props.colorConfig.color);
    }
  }, [props.colorConfig])
  // 
  useEffect(() => {
    setClockWords(config ? config.clockface.layout : [])
  }, [config])

  useEffect(() => {
    if (clockFaceConfig) {
      let colorMapArray = colorList;
      colorMapArray[clockFaceConfig.metadata.backgroundIndex] = colorConfig.backgroundColor;
      setClockFaceConfig({ ...clockFaceConfig, "colorMap": colorMapArray });
    }
  }, [colorList, colorConfig]);

  function transformConfig(event) {
    if (event.value) {
      const pos = Number((event.id).split("_")[1]);
      const color = event.value;
      if (color !== colorList[pos]) {
        var tempConfig = { ...colorConfig };
        tempConfig.color[pos] = color;
        props.onColorConfig({ id: "color", value: tempConfig.color });
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
      <div>
        {clockWords.map((clockWord, index) => {
          const colorIndex = "color_" + String(index);
          if (clockWord.word) {
            return <div style={{ display: "flex", marginTop: "20px" }}><div style={{ flex: 0.05 }}> <label>{clockWord.word}</label></div><div style={{ flex: 0.9 }}><ColorPicker id={colorIndex} currentVal={colorList[index]} onColorChoice={transformConfig} /></div></div>
          }
        })}
      </div>
      {/* <ColorPicker id="color" currentVal={colorConfig.color} onColorChoice={props.onColorConfig} /> */}
    </div>
  );
}
