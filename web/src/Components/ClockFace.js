import React, { useEffect, useState, useContext} from "react";
import { clockFaceContext  } from "../Context/Context";
import { calculateComplementary } from "../Utils/Utils";

function ClockFace(props) {
    const [clockFaceConfig,setClockFaceConfig]=useContext(clockFaceContext);
    const [letterGrid,setLetterGrid]=useState(null);
    const [rows,setRows]=useState(0);
    const [cols,setCols]=useState(0);
    const [totalLetters,setTotalLetters]=useState(0);
    const [extraLEDs,setExtraLEDs]=useState(0);
    const [colorMap,setColorMap]=useState(null);

    useEffect(() => {
        if (clockFaceConfig !== null) {
            setRows(clockFaceConfig.metadata.rows);
            setCols(clockFaceConfig.metadata.cols);
            setExtraLEDs(clockFaceConfig.metadata.extraLEDs);
            setTotalLetters(clockFaceConfig.metadata.totalLetters);
            setLetterGrid(clockFaceConfig.letterGrid);
            setColorMap(clockFaceConfig.colorMap)
        }
    }
    ,[clockFaceConfig]);

    function getColor(letterPos) {
        const wordPos=letterGrid[letterPos][1];
        let color=colorMap[wordPos];
        if (color === "#xxxxxx") {
            color=calculateComplementary(colorMap[0]);
        }
        return color; 
    }
    const handleClick = (e) => {
        const item = e.target;
        if (item.classList.contains('selected')) {
            item.classList.remove('selected');
        } else {
            item.classList.add('selected');
        }
    };

    return (
        <main>
            { [...Array(rows)].map((row, rowIndex) => (
                <ul className="row" key={rowIndex}>
                    { [...Array(cols)].map((cell, index) => {
                        const pos = rowIndex * cols + index;
                        const letterPos = totalLetters - 1 - rowIndex * cols - (rowIndex % 2 === 0 ? index : cols - 1 - index);
                        //const letterColor= (letterGrid[letterPos][2]=="#xxxxxx"? calculateComplementary(letterGrid[0][2]):letterGrid[letterPos][2])
                        const wordClass = { color: getColor(letterPos) };
                        return <li key={pos} style={wordClass} className="item" onClick={handleClick} >{letterGrid[letterPos][0]}</li>
                    })}
                </ul>
            ))}
            <ul className="row" key={extraLEDs + 1}>
                {[...Array(extraLEDs)].map((value, index) => {
                    const wordClass = { color: getColor(extraLEDs-1-index) };
                    return <li key={index} style={wordClass} className="item" onClick={handleClick}> {String.fromCharCode(9642)} </li>
                })}
            </ul>

        </main>
    );
};


export default React.memo(ClockFace);