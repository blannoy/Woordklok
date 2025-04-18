import React, { useState, useEffect, useContext, useRef } from "react";
import { useSetBrightnessMutation, useGetConfigQuery } from "../Components/ClockAPI";

export function StoreLDRRange(config, ldrRange) {
    const [updateConfig,] =useSetBrightnessMutation();
    let newBrightnessSettings = { ...config.brightness.settings, 
        ldrBrightness: { 
            ldrRange: { ...ldrRange}, 
            ldrBrightness: { ...config.brightness.settings.ldrBrightness.brightness } 
        } 
    };
console.log("newBrightnessConfig", {...config.brightness, settings: {...newBrightnessSettings}});

    updateConfig({...config.brightness, settings: {...newBrightnessSettings}});
}