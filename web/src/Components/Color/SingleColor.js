import React, { useEffect, useState } from "react";
import ColorPicker from "./ColorPicker";
import { calculateComplementary } from "../../Utils/Utils";
import { useContext } from "react";
import { clockContext } from "../../Context/Context";
import isEqual from 'lodash/isEqual';

/**
 * Builds screen for selection of a single color for clock-words and background color for non active LEDs
 * Possibility to select the complementary color for the background.
 * @param {*} props Should receive the JSON representation of a "singleColor" configuration
 * @returns 
 */
export default function SingleColor(props) {
  const [colorConfig, setColorConfig] = useState(props.colorConfig);
  const [fullConfig, setFullConfig] = useContext(clockContext);
  const [clockFaceConfig, setClockFaceConfig] = useState(undefined);
  const [colorMap, setColorMap] = useState(null);


  // change in passed property, triggers update of state
  useEffect(() => {
    console.log("SingleColor: props.colorConfig", JSON.stringify(props.colorConfig), "-", JSON.stringify(colorConfig));
    setColorConfig({ ...props.colorConfig });
    props.setClockColors({ 'singleColor': { 'foreground': colorConfig.color, 'background': colorConfig.backgroundColor } });
  }, [props.colorConfig]);

  useEffect(() => {
    console.log("SingleColor: colorConfig", JSON.stringify(colorConfig));
    if (fullConfig !== undefined && fullConfig.clockface !== undefined) {
      let colorMapArray = Array(fullConfig.clockface.metadata.nrWords).fill(colorConfig.color);
      colorMapArray[fullConfig.clockface.metadata.backgroundIndex] = colorConfig.backgroundColor;
      if (isEqual(colorMapArray, colorMap) === false) {
        setColorMap(colorMapArray);
        //setClockFaceConfig({ ...clockFaceConfig, "colorMap": colorMapArray });
        setFullConfig({ ...fullConfig, colorMap: colorMapArray });
      }
    }
  }, [colorConfig]);

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
      {JSON.stringify(colorMap)}
    </div>
  );
}
