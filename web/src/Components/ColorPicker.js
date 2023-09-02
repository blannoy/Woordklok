import React, { useEffect, useState } from "react";
import { HuePicker } from 'react-color';
import { calculateComplementary } from "../Utils/Utils";
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
    const [foregroundColor,setForegroundColor] = useState(props.foregroundColor);   
    const [previousColor, setPreviousColor] = useState("");
    const [hasColor, setHasColor] = useState(colorToType(props.currentVal));

    /**
     * Set color and keep track of it (if not White, Black or Complementary)
     * @param {*} color 
     */
    const handleColorChange = color => {
        setColorChoice(color.hex);
        if (!(["#ffffff", "#000000", "#xxxxxx"].includes(color.hex))) {
            setPreviousColor(color.hex);
        }

    };

    useEffect(() => {
        setColorChoice(props.currentVal);
        setHasColor(colorToType(props.currentVal));
        if (!previousColor) {
            setPreviousColor(props.currentVal);
        }
    }, [props.currentVal]);

    useEffect(() => {
        setForegroundColor(props.foregroundColor);
    }, [props.foregroundColor]);

    useEffect(() => {
        if (colorChoice !== props.currentVal) {
            props.onColorChoice({ id: props.id, value: colorChoice });
        }
    }, [colorChoice])

    const colorChange = e => {
        setHasColor(e.target.value);
        setColorChoice(typeToColor(e.target.value, previousColor));
    }

    const getColor = () => {
        if (hasColor === "Complementary") {
            return calculateComplementary(foregroundColor);
        } else {
            return colorChoice;
        }
    }

    return (
        <div>

            <div style={{ display: "inline-flex", justifyContent: "center" }}>
                <div style={{
                    backgroundColor: getColor(),
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
                </form>    {hasColor === "Color" && <div style={{ marginLeft: "20px" }}><HuePicker  color={colorChoice} onChange={handleColorChange} /></div>}

            </div>
        </div>
    );
}
