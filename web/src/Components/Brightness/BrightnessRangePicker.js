import React, { useEffect, useState } from "react";
import ReactSlider from "react-slider";

const defaultProps = { min: 0, max: 255, step: 1, minDistance: 10 };

export default function BrightnessRangePicker(props) {
    const [brightness, setBrightness] = useState([0,255]);
    const [currentBrightness, setCurrentBrightness] = useState([0,100]);
    const [sliderProps, setSliderProps] = useState(defaultProps);

    const handleBrightnessChange = brightness => {
        setBrightness(brightness);
    };

    useEffect(() => {
        if (Object.keys(props).length !== 0 && props.currentVal.length===2){
        const { id,currentVal, onBrightnessChoice, ...sliderPropsTemp } = { ...props }
        setBrightness(currentVal);
        setSliderProps({...defaultProps,...sliderPropsTemp});
        }
    }, [props]);

    useEffect(() => {
        setCurrentBrightness(["hsl(0,0%," + Math.round(brightness[0] / sliderProps.max * 100) + "%)","hsl(0,0%," + Math.round(brightness[1] /  sliderProps.max * 100) + "%)"]);
        props.onBrightnessChoice(brightness);
    }, [brightness]);

    function onValueChange(e) {
        let inputName = e.target.id;
        let value = e.target.value;
        switch (inputName) {
            case "minBrightness":
                value = Math.min(value, brightness[1] - 10);
                setBrightness([value, brightness[1]]);
                break;
            case "maxBrightness":
                value = Math.max(value, brightness[0] + 10);
                setBrightness([brightness[0], value]);
                break;
            default:
                break;
        }
    }
    return (
        <div>

            <div className="section">
                <div className="smallBodyCell">
                    <input type="number" id="minBrightness" name="minBrightness" min={sliderProps.min} max={sliderProps.max} value={brightness[0]} onChange={onValueChange} />
                </div>
                <div style={{ '--backgroundColorMin': currentBrightness[0], '--backgroundColorMax': currentBrightness[1] }} className="bodyCell">
                    <ReactSlider
                        className="horizontal-brightness-slider"
                        thumbClassName="thumbMinMax"
                        trackClassName="brightness-track"
                        value={brightness}
                        onChange={handleBrightnessChange}
                        {...sliderProps}
                    />
                </div>
                <div className="smallBodyCell">
                    <input type="number" id="maxBrightness" name="maxBrightness" min={sliderProps.min} max={sliderProps.max} value={brightness[1]} onChange={onValueChange} />

                </div>
            </div>
        </div >
    );
}

