import React, {useState, useEffect,useReducer} from "react";
import Main from "./Main";
import {configContext, queryProviderContext, clockFaceContext} from "../Context/Context";
import {mapClockFace} from "../Utils/ClockFaceMapper";
import useRequest, {requestReducer,defaultRequest } from "../Context/Reducer";

export default function Root() {
    const [initialConfig,setInitialConfig]=useState(null);
    const [clockFace,setClockFace]=useState(null);
    const [error,setError]=useState(null);
    const [response, err, runRequest]  = useRequest();
    const [requestState, dispatchRequest] = useReducer(requestReducer, defaultRequest)
    useEffect(() => {
      if (requestState.url !== ""){
        runRequest(requestState);
      }
  }, [requestState]);
  
    useEffect(() => {
        if (initialConfig===null && response===null){
            dispatchRequest({type:'LOADCONFIG'});
          } else if  (initialConfig===null && !(response===null)){
            setInitialConfig(response.data);
            setClockFace(mapClockFace(response.data.clockface));
          }
          if (err){
            setError(err);
          }      
        }, [response,err])

    return (
        <configContext.Provider value={[initialConfig,setInitialConfig]}>
          <clockFaceContext.Provider value={[clockFace,setClockFace]}>
          <queryProviderContext.Provider value={[requestState,dispatchRequest]}>
            <Main />
            <div>
            <pre>{error}</pre>
            </div>
            </queryProviderContext.Provider>
            </clockFaceContext.Provider>
        </configContext.Provider>
    );
}
