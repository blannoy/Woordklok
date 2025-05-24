import React, { useState, useEffect, useContext, useRef } from "react";

import { useLazyGetSensorValueQuery,useSetSensorMutation } from "../Components/ClockAPI";
import { clockContext } from "../Context/Context";
import {
    LineChart,
    Line,
    YAxis,
    CartesianGrid,
    Tooltip,
    XAxis,
} from "recharts";
import { max, min, set } from "lodash";
import { use } from "react";


export default function Sensors() {
    const calibrationTime = 30;
    const intervalTime = 100;
    const [fullConfig, setFullConfig] = useContext(clockContext);

    //const [config, setConfig] = useState(undefined)
    const [getSensorValue, { data: sensorData, error: sensorError, isFetching: sensorLoading }] = useLazyGetSensorValueQuery();
    const [updateSensors,] = useSetSensorMutation();
    const [seconds, setSeconds] = useState(calibrationTime);
    const [doCalibrate, setDoCalibrate] = useState(false);
    const intervalRef = useRef();
    const [sensor, setSensor] = useState('');
    const [sensorList, setSensorList] = useState([]);
    const [sensorValues, setSensorValues] = useState([]);
    const numPoints = calibrationTime * (1000 / intervalTime); // number of points to display
    const [minValue, setMinValue] = useState(1024);
    const [maxValue, setMaxValue] = useState(0);
    const [touchThreshold, setTouchThreshold] = useState(0);

    useEffect(() => {
        if (fullConfig !== undefined) {

            setSensorList(fullConfig.config.sensors);
        }
    }, [fullConfig]);

    useEffect(() => {
        if (sensor==="touch"){
            if (minValue !== 0 && maxValue !==0) {
            setTouchThreshold(Math.trunc(minValue + (maxValue - minValue) / 3))
        } else {
            let touchThresholdConfig = sensorList.find(sensor => sensor.name === "touch").attributes.find(attr => attr.name === "threshold").value;
            setTouchThreshold(touchThresholdConfig);
        }
    }
    }, [minValue, maxValue]);


    function storeSensorData(e) {
        var sensorSettings = [];
        if (minValue < maxValue) {
            if (sensor === "ldr") {
                sensorSettings = [{
                    name: "ldr",
                    attributes: [
                        { name: "ldrDark", value: minValue },
                        { name: "ldrBright", value: maxValue }
                    ]
                }]
                sensorList.forEach((sensor) => {
                    if (sensor.name !== "ldr") {
                        sensorSettings.push({ ...sensor });
                    }
                });

            } else if (sensor === "touch") {
                sensorSettings = [{
                    name: "touch",
                    attributes: [
                        { name: "threshold", value: touchThreshold},
                    ]
                }]
                sensorList.forEach((sensor) => {
                    if (sensor.name !== "touch") {
                        sensorSettings.push({ ...sensor });
                    }
                });
            };
            updateSensors({ sensors: [...sensorSettings] });
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
                let minVal = sensorList.find(sensor => sensor.name === "ldr").attributes.find(attr => attr.name === "ldrDark").value;
                let maxVal = sensorList.find(sensor => sensor.name === "ldr").attributes.find(attr => attr.name === "ldrBright").value;
                setMinValue(minVal);
                setMaxValue(maxVal);
            }
            else if (sensor === "touch") {
                let touchThresholdConfig = sensorList.find(sensor => sensor.name === "touch").attributes.find(attr => attr.name === "threshold").value;

                setMinValue(0);
                setMaxValue(0);
                                setTouchThreshold(touchThresholdConfig);
            }
        }
        useEffect(() => {
            if (sensorData) {
                const newData = [...sensorValues]
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
                setMinValue(100000);
                setMaxValue(0);
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
                            <option key={sensor.name} value={sensor.name}>
                                {sensor.name}
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

                        {sensor === "ldr" &&
                            <>
                                <div className="smallLabelCell">
                                    <label>Minimum</label>
                                </div>
                                <div className="smallBodyCell">
                                    <input type="number" name="dark" value={minValue} onChange={(e)=>{setMinValue(e.target.value)}}/>
                                </div>

                                <div className="smallLabelCell">
                                    <label>Maximum</label>
                                </div>
                                <div className="smallBodyCell">
                                    <input type="number" name="bright" value={maxValue} onChange={(e)=>{setMaxValue(e.target.value)}}/>
                                </div>
                                <div className="bodyCell">
                                    <button onClick={(e) => storeSensorData(e)}>Save as LDR range</button>
                                </div>
                            </>}
                        {sensor === "touch" &&
                            <>
                                                          <div className="smallLabelCell">
                                    <label>Minimum</label>
                                </div>
                                <div className="smallBodyCell">
                                    <input type="number" name="dark" value={minValue} readOnly/>
                                </div>

                                <div className="smallLabelCell">
                                    <label>Maximum</label>
                                </div>
                                <div className="smallBodyCell">
                                    <input type="number" name="bright" value={maxValue} readOnly/>
                                </div>
                                <div className="smallLabelCell">
                                    <label>Threshold</label>
                                </div>
                                <div className="smallBodyCell">
                                    <input type="number" name="touchThreshold" value={touchThreshold} onChange={(e)=>{setTouchThreshold(e.target.value)}}/>
                                </div>
                                <div className="bodyCell">
                                    <button onClick={(e) => storeSensorData(e)}>Save as touch threshold</button>
                                </div>
                                
                            </>}
                           
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