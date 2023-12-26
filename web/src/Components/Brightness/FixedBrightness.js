import React, { useState, useEffect } from "react";
import BrightnessPicker from "./BrightnessPicker";

export default function FixedBrightness(props) {
    const [fixedBrightnessValue, setFixedBrightnessValue] = useState(0);

    useEffect(() => {
        if (props.currentVal.brightness !== fixedBrightnessValue){
            
        setFixedBrightnessValue(props.currentVal.brightness);
        }
    },[props.currentVal]);

    const onBrightnessChoice = (value) => {
        //console.log("onBrightnessChoice: " + value);
        if (value !== fixedBrightnessValue){
            props.onBrightnessChoice({ "brightness": value });
            setFixedBrightnessValue(value);
        }
    }


    return (
        <div>
        <BrightnessPicker id="fixedBrightnessValue" currentVal={fixedBrightnessValue} onBrightnessChoice={onBrightnessChoice} />
        </div>
      );
}