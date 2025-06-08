import React, { useEffect, useState } from "react";
import ColorPicker from "./ColorPicker";
import { useTranslation } from "react-i18next"; // Add this import

const deepEqual = require('deep-equal');
/**
 * Builds screen for selection of a color for each clock-word and a global background color for non active LEDs
 * @param {*} props Should receive the JSON representation of a "wordColor" configuration
 * @returns 
 */
export default function HourlyColor(props) {
  const { t } = useTranslation(); // Add this line
  const [colorConfig, setColorConfig] = useState(props.colorConfig);
  const [colorList, setColorList] = useState(colorConfig.color ? colorConfig.color : []);


  // change in passed property, triggers update of state
   useEffect(() => {
    if (!deepEqual(props.colorConfig, colorConfig)) {
      setColorConfig({ ...props.colorConfig });
      setColorList(props.colorConfig.color);
    }
    }, [props.colorConfig])


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

      <h3>{t("colors.chooseBackground")}</h3>
      <div>
        <ColorPicker id="backgroundColor" foregroundColor={'#888888'} currentVal={colorConfig.backgroundColor} onColorChoice={props.onColorConfig} />
      </div>
      <h3>{t("colors.chooseHourly")}</h3>
      {colorList &&
      <div>
      { [...Array(24)].map((value,index)=> {
        const colorIndex="color_"+String(index);
      return <div className="section" key={colorIndex}><div className="smallLabelCell"> <label>{String(index)+":00"}</label></div><div className="wideBodyCell"><ColorPicker id={colorIndex} currentVal={colorList[index]} onColorChoice={transformConfig} /></div></div>
})}
      </div> }
    </div>
  );
}
