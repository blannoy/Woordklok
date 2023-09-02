import React, { useEffect, useState, useContext } from "react";
import { configContext } from "../Context/Context";
import { calculateComplementary } from "../Utils/Utils";

export default function ClockFace(props) {
    const [config, setConfig] = useContext(configContext);
    const [colorConfig, setColorConfig] = useState(props.colorConfig);
    const [clockWords, setClockWords] = useState((config ? config.clockface.layout : []));
    // const [colorList,setColorList]=useState(colorConfig.color?colorConfig.color:[]);
    const [gridSize, setGridSize] = useState([0, 0, 0]);
    const [cells, setCells] = useState([]);
    const [rows, setRows] = useState([]);
    const [totalLetters, setTotalLetters] = useState(0);
    const [letterGrid, setLetterGrid] = useState([]);


    useEffect(() => {
        setClockWords(config ? config.clockface.layout : []);
    }, [config])

    useEffect(() => {
        if (config) {
            const dims = [config.clockface.wordGridHorizontal, config.clockface.wordGridVertical, config.clockface.extraLEDs];
            setGridSize(dims);
            setCells(new Array(dims[0]).fill(0));
            setRows(new Array(dims[1]).fill(0));
            setTotalLetters(dims[0] * dims[1] + dims[2]);

            const tempGrid = Array.from(Array(dims[0] * dims[1] + dims[2]).fill(0), () => new Array(3).fill(""));
            clockWords.forEach((clockWord, wordPos) => {
                const posArr = clockWord.leds;
                let letters = [];
                let wordColor = "";
                if (clockWord.word) {
                    letters = clockWord.word.split("");
                    wordColor = getColor(wordPos);
                } else if (clockWord.background) {
                    letters = clockWord.background.split("");
                    wordColor = getColor(-1);
                }
                posArr.forEach((pos, index) => {
                    tempGrid[pos][0] = String(letters[index]).toUpperCase();
                    tempGrid[pos][1] = wordPos;
                    tempGrid[pos][2] = wordColor;
                })
            }
            );
            setLetterGrid(tempGrid);
        };
    }, [clockWords, props.colorConfig])

    function getColor(pos) {
        if (pos < 0) {
                return props.colorConfig.backgroundColor;
        }
        switch (props.colorOption) {
            case "singleColor":
                return props.colorConfig.color;
            case "hourlyColor":

                break;
            case "wordColor":

                break;
            case "rainbowColor":

                break;

            default:
                break;
        }
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
            {rows.map((row, rowIndex) => (
                <ul className="row" key={rowIndex}>
                    {cells.map((cell, index) => {
                        const pos = rowIndex * gridSize[0] + index;
                        const letterPos = totalLetters - 1 - rowIndex * gridSize[0] - (rowIndex % 2 === 0 ? index : 10 - index);
                        const letterColor= (letterGrid[letterPos][2]=="#xxxxxx"? calculateComplementary(letterGrid[0][2]):letterGrid[letterPos][2])
                        const wordClass = { color: letterColor };
                        return <li key={pos} style={wordClass} className="item" onClick={handleClick} >{letterGrid[letterPos][0]}</li>
                    })}
                </ul>
            ))}
            <ul className="row" key={gridSize[0] + 1}>
                {[...Array(gridSize[2])].map((value, index) => {
                    const wordClass = { color: letterGrid[index][0] };
                    return <li key={index} style={wordClass} className="item" onClick={handleClick}> {String.fromCharCode(9642)} </li>
                })}
            </ul>

        </main>
    );
};


