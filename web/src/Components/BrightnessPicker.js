import React, { useEffect, useState } from "react";
import ReactSlider from "react-slider";

export default function BrightnessPicker(props) {
    const [brightness, setBrightness] = useState(props.currentVal);
    const [currentColor, setCurrentColor] = useState("")
    
    const handleBrightnessChange = brightness => {
        setBrightness(brightness);
    };
    
    useEffect(() => {
        setBrightness(props.currentVal);
    }, [props.currentVal]);
    
    useEffect(() => {
        setCurrentColor("hsl(0,0%," + Math.round(brightness/255*100) +"%)");
       props.onBrightnessChoice(brightness);
    }, [brightness]);
    

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
                            min={0}
                            max={255}
                            step={5}
                            // renderThumb={(props, state) => <div {...props} ></div>}
                            onChange={handleBrightnessChange}
                        />

                    </div>
            </div>
        </div>
    );
}
