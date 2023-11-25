import React, { useEffect, useState } from "react";
import { LightnessPicker } from "../react-color_mod/components/lightness/Lightness";

/**
 * 
 * @param {*} props.currentVal 
 * Brightness byte value 0-255
 * @returns 
 */
export default function BrightnessPicker(props) {
    const [brightnessChoice, setBrightnessChoice] = useState(props.currentVal);
    const [color, setColor] = useState({ h: 0, s: 0, l: props.currentVal / 255});

    /**
   * Set color and keep track of it (if not White, Black or Complementary)
   * @param {*} color 
   */
    const handleBrightnessChange = color => {
        setBrightnessChoice(Math.round(color.l * 255));
        setColor(color);
    };

    useEffect(() => {
        setBrightnessChoice(props.currentVal);
    }, [props.currentVal]);

    useEffect(() => {
        setColor({ h: 0, s: 0, l: brightnessChoice / 255 });
       // props.onBrightnessChoice(props.id,brightnessChoice);
       props.onBrightnessChoice(brightnessChoice);
    }, [brightnessChoice]);

    function mapToValue(event) {
        setBrightnessChoice(Math.round(event.target.value));
    }
    function getColor(){
        let colorString="hsl(0,0%," + Math.round(brightnessChoice/255*100) +"%)"
        return colorString;
    }

    return (
        <div>

            <div style={{ display: "inline-flex", justifyContent: "center"}}>
                <div style={{
                    backgroundColor: getColor(),
                    borderColor: "black",
                    borderWidth: "1px",
                    borderStyle: "solid",
                    width: '20px',
                    height: '20px'
                }}></div>
                <div style={{ marginLeft: "20px", paddingTop: "3px"}}><LightnessPicker hsl={color} onChange={handleBrightnessChange} /></div>
                <div style={{ marginLeft: "20px" }}> <input type="number" name="lightness" min={0} max={255} value={Math.round(color.l * 255)} onChange={mapToValue} /></div>
            </div>
        </div>
    );
}
