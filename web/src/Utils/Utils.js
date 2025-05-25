/**
 * Calculate complementary color 
 * @param {*} color 
 * @param {*} hex 
 * @returns 
 */
export function calculateComplementary(hexColor) {
    if (hexColor) {
        return '#' +
            ("0" + (255 - parseInt(hexColor.substring(1, 3), 16)).toString(16)).slice(-2) +
            ("0" + (255 - parseInt(hexColor.substring(3, 5), 16)).toString(16)).slice(-2) +
            ("0" + (255 - parseInt(hexColor.substring(5, 7), 16)).toString(16)).slice(-2);
    }
}


