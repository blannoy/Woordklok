import React, { useEffect, useState } from "react";
import ReactSlider from "react-slider";
import { calculateComplementary } from "../../Utils/Utils";
import tinycolor from "tinycolor2";
/**
 * Determine hasColor property based on color
 * @param {*} color 
 * @returns 
 */
function colorToType(color) {
    switch (color) {
        case "#ffffff":
            return "White";
        case "#000000":
            return "Black";
        case "#xxxxxx":
            return "Complementary";
        default:
            return "Color";
    }
}
/**
 * Return hex color based on hasColor property 
 * @param {*} colorType 
 * @param {*} prevColor 
 * @returns 
 */
function typeToColor(colorType, prevColor) {
    switch (colorType) {
        case "White":
            return "#ffffff";
        case "Black":
            return "#000000";
        case "Complementary":
            return "#xxxxxx";
        default:
            return prevColor;
    }
}
/**
 * Colorpicker component showing White, Black, Color slider and possibility for complementary color if specified
 * Complementary color will be "#xxxxxx"
 * @param {*} props.currentVal - current configured color
 * @param {*} props.foregroundColor - optional - complementary color calculated in parent 
 * @returns 
 */
export default function ColorPicker(props) {
    const [colorChoice, setColorChoice] = useState(props.currentVal);
    const [foregroundColor, setForegroundColor] = useState(props.foregroundColor);
    const [previousColor, setPreviousColor] = useState("");
    const [hasColor, setHasColor] = useState(colorToType(props.currentVal));
    const [currentHue, setCurrentHue] = useState(Math.round(tinycolor(props.currentVal).toHsl().h));
    const [currentColor, setCurrentColor] = useState(props.currentVal);


    useEffect(() => {
        if (props.currentVal && props.currentVal !== colorChoice){
            setColorChoice(props.currentVal);
            setCurrentHue(Math.round(tinycolor(props.currentVal).toHsl().h));
            setHasColor(colorToType(props.currentVal));
            if (!previousColor) {
                setPreviousColor(props.currentVal);
            }
        }
    }, [props.currentVal]);

    useEffect(() => {
        setForegroundColor(props.foregroundColor);
        if (hasColor === "Complementary"){
            setCurrentColor(calculateComplementary(props.foregroundColor));
        }
    }, [props.foregroundColor]);

     useEffect(() => {
        if (colorChoice!== undefined){ //&& colorChoice !== props.currentVal) {
            props.onColorChoice({ id: props.id, value: colorChoice });
         }
         if (hasColor === "Complementary"){
            setCurrentColor(calculateComplementary(props.foregroundColor));
        } else {
            setCurrentColor(typeToColor(hasColor, colorChoice));
        }

     }, [colorChoice])

    const colorChange = e => {
        setHasColor(e.target.value);
        setColorChoice(typeToColor(e.target.value, tinycolor({ h: currentHue, s: 100, l: 50 }).toHexString()));
     /*   if (e.target.value === "Complementary"){
            setColorChoice(calculateComplementary(foregroundColor));
           setColorChoice(typeToColor)
        } else 
        {
            setColorChoice(typeToColor(e.target.value, tinycolor({ h: currentHue, s: 100, l: 50 }).toHexString()));
        }*/

    }

        /**
     * Set color and keep track of it (if not White, Black or Complementary)
     * @param {*} color 
     */
        const handleColorChange = hue => {
            setCurrentHue(hue);
            setColorChoice(tinycolor({ h: hue, s: 100, l: 50 }).toHexString());
        };

    return (
        <div>

            <div className="section">
                <div className="normalBodyCell">  
                    <div style={{
                        backgroundColor: currentColor,
                        borderColor: "black",
                        borderWidth: "1px",
                        borderStyle: "solid",
                        width: '20px',
                        height: '20px'
                    }}></div>
                    <form>
                        <label><input type="radio" value="White" name="color" checked={hasColor === "White"} onChange={colorChange} />Wit</label>
                        <label><input type="radio" value="Black" name="color" checked={hasColor === "Black"} onChange={colorChange} />Zwart</label>
                        {foregroundColor && <label><input type="radio" value="Complementary" name="color" checked={hasColor === "Complementary"} onChange={colorChange} />Complementary</label>}
                        <label><input type="radio" value="Color" name="color" checked={hasColor === "Color"} onChange={colorChange} />Kleur</label>
                    </form>
                </div>

                    {hasColor === "Color" &&                 <div style={{ '--backgroundColor':currentColor}} className="bodyCell">
                        <ReactSlider 
                            className="horizontal-hue-slider"
                            thumbClassName="thumb"
                            trackClassName="hue-track"
                            value={currentHue}
                            min={0}
                            max={359}
                            step={3}
                            // renderThumb={(props, state) => <div {...props} ></div>}
                            onChange={handleColorChange}
                        />

                    </div>}
            </div>
        </div>
    );
}
