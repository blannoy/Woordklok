import React, { useEffect, useState } from "react";
import ColorPicker from "./ColorPicker";
import { useContext } from "react";
import { clockContext } from "../../Context/Context";
import isEqual from 'lodash/isEqual';
import { useTranslation } from "react-i18next"; 
/**
 * Builds screen for selection of a single color for clock-words and background color for non active LEDs
 * Possibility to select the complementary color for the background.
 * @param {*} props Should receive the JSON representation of a "singleColor" configuration
 * @returns 
 */
export default function SingleColor(props) {
  const [colorConfig, setColorConfig] = useState(props.colorConfig);
  const [fullConfig, setFullConfig] = useContext(clockContext);
  const [colorMap, setColorMap] = useState(null);
  const { t } = useTranslation(); // Add this line

  // change in passed property, triggers update of state
  useEffect(() => {
    setColorConfig({ ...props.colorConfig });
    props.setClockColors({ 'singleColor': { 'foreground': colorConfig.color, 'background': colorConfig.backgroundColor } });
  }, [props.colorConfig]);

  useEffect(() => {
    if (fullConfig !== undefined && fullConfig.clockface !== undefined) {
      let colorMapArray = Array(fullConfig.clockface.metadata.nrWords).fill(colorConfig.color);
      colorMapArray[fullConfig.clockface.metadata.backgroundIndex] = colorConfig.backgroundColor;
      if (isEqual(colorMapArray, colorMap) === false) {
        setColorMap(colorMapArray);
        setFullConfig({ ...fullConfig, colorMap: colorMapArray });
      }
    }
  }, [colorConfig]);

  return (
    <div>
      <h3>{t("colors.chooseSingleColor")}</h3>
      <div>
        <ColorPicker id="color" currentVal={colorConfig.color} onColorChoice={props.onColorConfig} />
      </div>
      <h3>{t("colors.chooseBackground")}</h3>
      <div>
        <ColorPicker id="backgroundColor" currentVal={colorConfig.backgroundColor} foregroundColor={colorConfig.color} onColorChoice={props.onColorConfig} />
      </div>
    </div>
  );
}
