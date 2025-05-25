
export function mapClockFace(clockConfig) {
    const dims = [clockConfig.wordGridCols, clockConfig.wordGridRows, clockConfig.extraLEDs];
    const clockWords = clockConfig.layout;
    const nrWords = clockWords.length;
    const totalLetters = dims[0] * dims[1] + dims[2];
    const backgroundIndex = clockWords.findIndex(p => p.background)
    const letterGrid = Array.from(Array(totalLetters).fill(0), () => new Array(2).fill(""));
    clockWords.forEach((clockWord, wordPos) => {
        const posArr = clockWord.leds;
        let letters = [];
        if (clockWord.word) {
            letters = clockWord.word.split("");
        } else if (clockWord.background) {
            letters = clockWord.background.split("");
        }
        posArr.forEach((pos, index) => {
            letterGrid[pos][0] = String(letters[index]).toUpperCase();
            letterGrid[pos][1] = wordPos;
            // tempGrid[pos][2] = wordColor;
        })
    }
    );
    const clockFaceConfig = { "metadata": 
    { "nrWords": nrWords, "totalLetters": totalLetters, "rows": dims[1], "cols": dims[0], "backgroundIndex": backgroundIndex, "extraLEDs": dims[2] }, 
    "letterGrid": letterGrid, 
    "layout": clockWords,
    "colorMap": [] };
    return clockFaceConfig;
}