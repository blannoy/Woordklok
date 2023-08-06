import React, { useContext } from "react";
import { configContext } from "../Context/Context";


export default function ShowConfig() {
   const config=useContext(configContext);
   return (
    <div>
    

                    <div><pre>{JSON.stringify(config,0,2)}</pre></div>
    </div>
  );
}
