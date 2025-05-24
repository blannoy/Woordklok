import React, { useContext } from "react";
import { clockContext } from "../Context/Context";


export default function ShowConfig() {
 const [fullConfig, setFullConfig] = useContext(clockContext);
 
   return (
    <div>
    

                    <div><pre>{JSON.stringify(fullConfig,0,2)}</pre></div>
    </div>
  );
}
