import React, { useEffect, useState } from "react";
import ReactSlider from "react-slider";

const defaultProps = { min: 0, max: 24, step: 1, minDistance: 1 };

export default function HourRangePicker(props) {
    const [hours, setHours] = useState(props.currentVal);
    const [currentHours, setCurrentHours] = useState([6,22]);
    const [sliderProps, setSliderProps] = useState(defaultProps);

    const handleHourChange = hours => {
        setHours(hours);
    };

    useEffect(() => {
        const { id,currentVal,  onChange, ...sliderPropsTemp } = { ...props }
        setHours(currentVal);
        setSliderProps({...defaultProps,...sliderPropsTemp});
    }, [props]);

    useEffect(() => {
        setCurrentHours(hours);
        props.onChange(hours);
    }, [hours]);

    return (
        <div>

            <div className="section">
                <div className="wideBodyCell">
                    <ReactSlider
                        className="horizontal-slider"
                        thumbClassName="thumb"
                        trackClassName="htrack"
                        value={hours}
                        onChange={handleHourChange}
                        renderThumb={(props, state) => <div {...props}>{state.valueNow}</div>}
                        {...sliderProps}
                    />
                </div>
            </div>
        </div >
    );
}

