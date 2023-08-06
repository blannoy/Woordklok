import React, { useEffect, useState } from "react";
import { HuePicker } from 'react-color';

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

export default function ColorPicker(props) {
    const [colorChoice, setColorChoice] = useState(props.currentVal);
    const [complementaryColor, setComplementaryColor] = useState(props.complementaryColor);
    const [previousColor, setPreviousColor] = useState("");
    const [hasColor, setHasColor] = useState(colorToType(props.currentVal));

    const handleColorChange = color => {
        setColorChoice(color.hex);
        if (!(["#ffffff","#000000","#xxxxxx"].includes(color.hex))) {
            setPreviousColor(color.hex);
        }

    };
    useEffect(() => {
        setColorChoice(props.currentVal);
        setHasColor(colorToType(props.currentVal));
        if (!previousColor){
            setPreviousColor(props.currentVal);
        }
    }, [props.currentVal]);
    useEffect(() => {
        setComplementaryColor(props.complementaryColor);
    }, [props.complementaryColor]);

    useEffect(() => {
        if (colorChoice) {
            props.onColorChoice({ id: props.id, color: colorChoice });
        }
    }, [colorChoice])

    const colorChange = e => {
        setHasColor(e.target.value);
        setColorChoice(typeToColor(e.target.value, previousColor));
    }


    return (
        <div>

            <div style={{ display: "inline-flex", justifyContent: "center" }}>
                <div style={{
                    backgroundColor: ((hasColor === "Complementary")?complementaryColor:colorChoice),
                    borderColor: "black",
                    borderWidth: "1px",
                    borderStyle: "solid",
                    width: '20px',
                    height: '20px'
                }}></div>
                <form>
                    <label><input type="radio" value="White" name="color" checked={hasColor === "White"} onChange={colorChange} />Wit</label>
                    <label><input type="radio" value="Black" name="color" checked={hasColor === "Black"} onChange={colorChange} />Zwart</label>
                    {complementaryColor && <label><input type="radio" value="Complementary" name="color" checked={hasColor === "Complementary"} onChange={colorChange} />Complementary</label>}
                    <label><input type="radio" value="Color" name="color" checked={hasColor === "Color"} onChange={colorChange} />Kleur</label>
                </form>    {hasColor === "Color" && <div style={{ marginLeft: "20px" }}><HuePicker width="150px" color={colorChoice} onChange={handleColorChange} /></div>}

            </div>
        </div>
    );
}
