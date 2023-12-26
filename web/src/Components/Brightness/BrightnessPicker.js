import React, { useEffect, useState } from "react";
import ReactSlider from "react-slider";
const defaultProps = { min: 0, max: 255, step: 5, minDistance: 10 };

export default function BrightnessPicker(props) {
    const [brightness, setBrightness] = useState(props.currentVal);
    const [currentColor, setCurrentColor] = useState("")
    const [sliderProps, setSliderProps] = useState(defaultProps);

    const handleBrightnessChange = brightness => {
        setBrightness(brightness);
    };
    
    useEffect(() => {
        const { id,currentVal, onBrightnessChoice, ...sliderPropsTemp } = { ...props }
        setBrightness(currentVal);
        setSliderProps({...defaultProps,...sliderPropsTemp});
    }, [props]);
    
    useEffect(() => {
        setCurrentColor("hsl(0,0%," + Math.round(brightness/255*100) +"%)");
       props.onBrightnessChoice(brightness);
    }, [brightness]);
    function onValueChange(e) {
        setBrightness(e.target.value)
    }

    return (
        <div>

            <div className="section">
                <div className="smallBodyCell">  
                    <div style={{
                        backgroundColor: currentColor,
                        borderColor: "black",
                        borderWidth: "1px",
                        borderStyle: "solid",
                        width: '20px',
                        height: '20px'
                    }}></div>
                </div>

                   <div style={{ '--backgroundColor':currentColor}} className="bodyCell">
                        <ReactSlider 
                            className="horizontal-brightness-slider"
                            thumbClassName="thumb"
                            trackClassName="hue-track"
                            value={brightness}
                            onChange={handleBrightnessChange}
                            {...sliderProps}
                        />

                    </div>
                    <div className="smallBodyCell">
                    <input type="number" id="brightness" name="brightness" min={sliderProps.min} max={sliderProps.max} value={brightness} onChange={onValueChange} />

                </div>
            </div>
        </div>
    );
}

