import React, { useState, useEffect, useContext, useRef } from "react";
import { configContext, queryProviderContext } from "../Context/Context";
import { useLazyGetSensorValueQuery, useGetConfigQuery, useSetBrightnessMutation } from "../Components/ClockAPI";

import {
    LineChart,
    Line,
    YAxis,
    CartesianGrid,
    Tooltip,
    XAxis,
} from "recharts";


export default function Sensors() {
    const calibrationTime = 30;
    const intervalTime = 100;
    const { data, error, isLoading } = useGetConfigQuery();
    const [config, setConfig] = useState(undefined)
    const [getSensorValue, { data: sensorData, error: sensorError, isFetching: sensorLoading }] = useLazyGetSensorValueQuery();
        const [updateConfig,] =useSetBrightnessMutation();
    const [seconds, setSeconds] = useState(calibrationTime);
    const [doCalibrate, setDoCalibrate] = useState(false);
    const intervalRef = useRef();
    const [sensor, setSensor] = useState('');
    const [sensorList, setSensorList] = useState([]);
    const [sensorValues, setSensorValues] = useState([]);
    const numPoints = calibrationTime * (1000 / intervalTime); // number of points to display
    const [minValue, setMinValue] = useState(1024);
    const [maxValue, setMaxValue] = useState(0);

    useEffect(() => {
        if (data !== undefined) {
            setConfig(data);
            setSensorList(data.sensors);
        }
    }, [data]);

    function storeLDRRange() {
        if (minValue < maxValue) {

               let newBrightnessSettings = { 
                   ldrBrightness: { 
                       ldrRange:  { dark: minValue, bright: maxValue }, 
                       brightness: { ...config.brightness.settings.ldrBrightness.brightness } 
                   } 
               };
               updateConfig({...newBrightnessSettings});

        }
    }
    function tick() {
        setSeconds(prevSeconds => prevSeconds - intervalTime / 1000);
        getSensorValue({ sensor: sensor });


    }
    function sensorChange(sensor) {
        setSensor(sensor);
        setSensorValues([]); // reset sensor values

        if (sensor === "ldr") {
            //     setMinValue(config.brightness.settings.ldrBrightness.ldrRange.dark); // reset min value
            //     setMaxValue(config.brightness.settings.ldrBrightness.ldrRange.bright); // reset max value    
        }
    }
    useEffect(() => {
        if (sensorData) {
            const newData = [...sensorValues]
            // console.log("Sensor data:", sensorData);
            newData.push({ key: newData.length, ...sensorData });
            if (sensorData.sensorValue < minValue || minValue === undefined) {
                setMinValue(sensorData.sensorValue);
            }
            if (sensorData.sensorValue > maxValue || maxValue === undefined) {
                setMaxValue(sensorData.sensorValue);
            }

            setSensorValues(newData);
        }
    }
        , [sensorData]);

    useEffect(() => {
        if (doCalibrate) {
            setSensorValues([]);
            intervalRef.current = setInterval(tick, 100);
        } else if (intervalRef.current) {
            clearInterval(intervalRef.current);
            setSeconds(calibrationTime * (1000 / intervalTime)); // reset timer
            //setSensorValues([]); // reset sensor values
        }
        return () => {
            if (intervalRef.current) {
                clearInterval(intervalRef.current);
            }
        };
    }, [doCalibrate]);

    useEffect(() => {
        if (seconds <= 0 && intervalRef.current) {
            clearInterval(intervalRef.current);
            setSeconds(calibrationTime); // reset timer
            setDoCalibrate(false);
        }
    }, [seconds]);

    return (
        <div>
            <h3>Sensor calibration</h3>
            <div className="dropdown">
                <label htmlFor="sensorDropdown">Select Sensor:</label>
                <select
                    id="sensorDropdown"
                    value={sensor}
                    onChange={(e) => sensorChange(e.target.value)}
                >
                    <option value="" disabled>Select a sensor</option>
                    {sensorList.map((sensor) => (
                        <option key={sensor} value={sensor}>
                            {sensor}
                        </option>
                    ))}
                </select>
            </div>
            <button onClick={() => setDoCalibrate(!doCalibrate)}>
                {doCalibrate ? "Stop Calibration" : "Start Calibration"}
            </button>
            {doCalibrate && <p>Calibration in progress for {Math.round(seconds)} seconds</p>}
            {sensor !== '' && <div>
                <div className="section">
                    <div className="smallLabelCell">
                        <label>Minimum</label>
                    </div>
                    <div className="smallBodyCell">
                        <input type="number" value={minValue} readOnly />
                    </div>

                    <div className="smallLabelCell">
                        <label>Maximum</label>
                    </div>
                    <div className="smallBodyCell">
                        <input type="number" value={maxValue} readOnly />
                    </div>
                    <div className="bodyCell">
                        <button onClick={()=>storeLDRRange()}>Save as LDR range</button>
                    </div>
                </div>
            </div>}
            {sensorValues.length > 0 && (
                <div>
                    <div className="section">
                        <LineChart
                            width={730}
                            height={500}
                            data={sensorValues}
                            margin={{ top: 5, right: 30, left: 20, bottom: 5 }}
                        >
                            <CartesianGrid strokeDasharray="3 3" />
                            <XAxis
                                dataKey="key"
                                domain={[0, numPoints]}
                                type="number"
                                min={0}
                                max={numPoints}
                            />
                            <YAxis />
                            <Tooltip />
                            <Line
                                type="monotone"
                                dataKey="sensorValue"
                                stroke="#8884d8"
                                dot={false}
                                strokeWidth={2}
                            />
                        </LineChart>
                    </div>
                </div>
            )}
        </div>
    );
}